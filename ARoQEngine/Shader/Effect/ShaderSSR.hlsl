//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShadeSSR.hlsl
//            Content:  Screen space reflection shader
//                      https://zenn.dev/mebiusbox/articles/43ecf1bb12831c 
//                      https://tips.hecomi.com/entry/2016/04/04/022550
//             Author:  Toide Yutaro
//             Create:  2023_02_23
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_SSR_HLSL
#define SHADER_SSR_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderConstantBuffer3D.hlsli"
#include "../Core/ShaderVertexType.hlsli"
#define VertexIn  VSInputVertex
#define VertexOut VSOutputVertex
//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////
SamplerState SamplerPointClamp : register(s1);
SamplerState SamplerDepth       : register(s2);

// Textyre
Texture2D<float4> BaseColorTexture : register(t0);
Texture2D<float4> NormalTexture : register(t1);  // 現状ワールド法線になっているので, 修正必要
Texture2D<float>  DepthTexture  : register(t2);  // NDC空間の深度マップ

cbuffer SSRSetting : register(b1)
{
    int   MaxRayMarchIteration;     // レイマーチングを行う最大の試行回数 (デフォルトは16)
    int   MaxBinarySearchIteration; // 二分探索を行う試行回数
    float MaxRayDistance;           // レイの最大距離
    float RayThicknessInViewSpace;  // レイの厚み
    float MaxFadeDistance;
};

struct PSInput
{
    float4 Position    : SV_Position;
    float3 NearPlaneViewPosition : POSITION;
    float2 UV          : TEXCOORD;
};

//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
/* @brief : NDC空間の深度をView空間に復元する*/
float NDCDepthToViewDepth(const float z_ndc);
/* @brief : 反射ベクトルをもとにシーンの衝突判定を行う.*/
bool ExecuteRayMarchInCameraSpace(const float3 rayOrigin, const float3 rayDirection, out float3 hitRayPosition, out float2 hitUV);
/* @brief : レイの衝突判定*/
bool IntersectThicknessRay(const float z, const float2 uv);
/* @brief : アルファ値の取得*/
float CalculateAlpha(const bool isIntersect, const float2 uv, const float3 hitRayPosition, const float3 rayOrigin);

PSInput VSMain(VertexIn vertexIn)
{
    PSInput result;
    result.Position = vertexIn.Position;
    result.UV       = vertexIn.UV;
    
    // Transform rectangle corners to view space near plane
    const float4 position = mul(InverseProjection, vertexIn.Position);
    result.NearPlaneViewPosition = position.xyz / position.w;
    return result;
}

/****************************************************************************
*                       PSMain
****************************************************************************/
/* @fn        float4 PSMain(VertexOut input) : SV_TARGET

*  @brief     ScreenSpacerReflection post effect

*  @param[in] VertexOut input

*  @return 　　float4
*****************************************************************************/
float4 ExecuteSSR(PSInput input) : SV_TARGET
{
    /*-------------------------------------------------------------------
	-        反射ベクトルを求める.
	---------------------------------------------------------------------*/
    // 深度マップからNDC空間におけるこのピクセルのz座標を取得する
    // 深度マップの定義はzPrepass.hlslを参照してください, 
    const float ndcDepth         = DepthTexture.Sample(SamplerDepth, input.UV).r; 
    
    // ビュー空間における深度 (カメラ -> 視点までの深度)
    const float viewSurfaceDepth = NDCDepthToViewDepth(ndcDepth);
    
    // 深度値をもとに, View空間におけるサンプリング位置を再構成する.
    const float  distanceRatio       = (viewSurfaceDepth / input.NearPlaneViewPosition.z); // 
    const float3 viewSurfacePosition = distanceRatio * input.NearPlaneViewPosition; // NearPlaneは単位ベクトル的な扱い.
    
    // ワールド法線からビュー法線を取得する
    const float3 worldSurfaceNormal = (NormalTexture.Sample(SamplerPointClamp, input.UV).xyz * 2) - 1;
    const float3 viewSurfaceNormal = normalize(mul((float3x3)View, worldSurfaceNormal));
    
    // レイ初期位置
    const float3 rayOrigin    = viewSurfacePosition;
    
    // カメラ原点から物質表面に入射し, 物質表面の法線に対して反射を行う 
    const float3 rayDirection = reflect(normalize(viewSurfacePosition), viewSurfaceNormal);
    
    /*-------------------------------------------------------------------
	-        シーンとの衝突判定を行う
	---------------------------------------------------------------------*/
    float2 hitRayUV;
    float3 hitRayPosition;
    const bool isIntersect = ExecuteRayMarchInCameraSpace(rayOrigin, rayDirection, hitRayPosition, hitRayUV);
    
     /*-------------------------------------------------------------------
	-        画面の外側になるほど画面買いに向かってレイトレーシングが行われるようになり, 
     　　　　 正しい結果にならないので, 反射を外側に行くほど反射を弱くする (ビネットと同じような原理です)
	---------------------------------------------------------------------*/
    const float alpha = CalculateAlpha(isIntersect, hitRayUV, hitRayPosition, rayOrigin);
    
    const float3 beforeColor = BaseColorTexture.Sample(SamplerLinearWrap, input.UV).rgb;
    const float3 afterColor  = BaseColorTexture.Sample(SamplerLinearWrap, hitRayUV).rgb;
    const float3 color = lerp(beforeColor, afterColor, alpha);
    return float4(color,1.0f);
}

/****************************************************************************
*				  			NDCDepthToViewDepth
****************************************************************************/
/* @fn        float NDCDepthToViewDepth(const float z_ndc)

*  @brief     Convert ndc Depth to view space depth

*  @param[in] const float z depth in ndc space.

*  @return    float
*****************************************************************************/
float NDCDepthToViewDepth(const float z_ndc)
{
    // invert the calculation from NDC space to view space for the z-zoordinate. 
    // We have that z_ndc = A + B / viewZ, where gProj[2,2] = A and gProj[3,2] = B. 
    return Projection[3][2] / (z_ndc - Projection[2][2]);
}

/****************************************************************************
*				  			ExecuteRayMarchInCameraSpace
****************************************************************************/
/* @fn        bool ExecuteRayMarchInCameraSpace(const float3 rayOrigin, const float3 rayDirection)

*  @brief     カメラ空間でのレイマーチングの実行

*  @param[in] const float3 rayOrigin (ビュー空間での原点)
*  @param[in] const float3 rayDirection (ビュー空間でのレイの方向)

*  @return    bool
*****************************************************************************/
bool ExecuteRayMarchInCameraSpace(const float3 rayOrigin, const float3 rayDirection, out float3 hitRayPosition, out float2 hitRayUV)
{
    /*-------------------------------------------------------------------
	-        1イテレーションごとのdepthの深度距離を設定する
	---------------------------------------------------------------------*/
    // clipping領域の範囲内
    const float3 rayEnd       = rayOrigin + rayDirection * MaxRayDistance;
    float3 rayStepDelta = (rayEnd - rayOrigin) / MaxRayMarchIteration;
    
    /*-------------------------------------------------------------------
	-        レイマーチングの実行 (粗めに探索)
	---------------------------------------------------------------------*/
    float3 rayPosition = rayOrigin;
    bool   isIntersect = false;
    
    for (int i = 0; i < MaxRayMarchIteration; ++i)
    {
        // 衝突していたら終了
        if (isIntersect){ break; }

        /*-------------------------------------------------------------------
	    -        開始点から反射ベクトルを1ステップごとに進めた位置における深度情報を計算し, 衝突判定を行う
	    ---------------------------------------------------------------------*/
        // レイを進める
        rayPosition += rayStepDelta;
        
        // NDC空間に変換後, RayPositionのUV座標を取得する
        const float4 ndcRayPosition = mul(Projection, float4(rayPosition, 1.0f));
        hitRayUV = rayPosition.xy * 0.5f + 0.5f; // -1～1 -> 0～1
        
        // 衝突判定
        isIntersect = IntersectThicknessRay(rayPosition.z, hitRayUV);
    }
    
    /*-------------------------------------------------------------------
	-        品質向上のため, 二分探索を行いより詳細に
	---------------------------------------------------------------------*/
    if (isIntersect)
    {
        rayPosition  -= rayStepDelta;             // 一個前に戻す
        rayStepDelta /= (float)MaxBinarySearchIteration; // さらにステップ数を細かく設定する
        
        float strideOrigin = (float) MaxBinarySearchIteration * 0.5f;
        float rayStride    = strideOrigin;
        
        for (int i = 0; i < MaxBinarySearchIteration; ++i)
        {
            rayPosition += rayStepDelta * rayStride; // 半分だけ進める
            
            // NDC空間に変換後, RayPositionのUV座標を取得する
            const float4 ndcRayPosition = mul(Projection, float4(rayPosition, 1.0f));
            hitRayUV = rayPosition.xy * 0.5f + 0.5f; // -1～1 -> 0～1
            
            // 当たればより反射面に近い方に, 当たらなければより反射面に遠い方に進める
            strideOrigin *= 0.5f;
            rayStride    = IntersectThicknessRay(rayPosition.z, hitRayUV) ? -strideOrigin : strideOrigin;
        }

    }
    
    hitRayPosition = rayPosition;
    return isIntersect;
}

/****************************************************************************
*				  			CalculateAlpha
****************************************************************************/
/* @fn        float CalculateAlpha(const bool isIntersect, const float2 hitRayUV, const float3 hitRayPosition, const float3 rayOrigin)

*  @brief     画面の外側になるほど画面外に向かってレイトレーシングが行われ, 正しい結果にならないため, 画面の外側に行くほどSSRによる反射を弱くする

*  @param[in] const bool isIntersect : 衝突判定が成功しているか
   @param[in] const float2 hitRayUV  : hitしているレイのUV値
   @param[in] const float3 hitRayPosition : hitしたRayのView座標
   @param[in] const float3 rayOrigin : rayの原点

*  @return    float alpha
*****************************************************************************/
float CalculateAlpha(const bool isIntersect, const float2 hitRayUV, const float3 hitRayPosition, const float3 rayOrigin)
{
    if(!isIntersect){ return 0.0f; }
    
    float alpha = 1.0f;
    
    /*-------------------------------------------------------------------
	-        距離フェード
	---------------------------------------------------------------------*/
    const float rayDistance = length(hitRayPosition - rayOrigin);
    alpha *= saturate(1.0f - rayDistance / MaxFadeDistance);
    
    /*-------------------------------------------------------------------
	-        スクリーンのエッジフェード
	---------------------------------------------------------------------*/
    const float2 uvCenter   = float2(0.5f, 0.5f);
    
    float2 uvDistance = 2.0f * length(hitRayUV - 0.5f); 
    uvDistance *= uvDistance;
    
    const float screenEdgeFactor = max(0.0f, 1.0f - screenEdgeFactor);
    alpha *= screenEdgeFactor;
    
    return alpha;
}

bool IntersectThicknessRay(const float viewZ, const float2 uv)
{
    const float ndcSceneDepth  = DepthTexture.Sample(SamplerDepth, uv).r; // zBuffer中で描画されているオブジェクトの深度情報
    const float viewSceneDepth = NDCDepthToViewDepth(ndcSceneDepth);      
    const float deltaViewZ     = viewZ - viewSceneDepth; 
    return deltaViewZ < 0.0f && deltaViewZ > -RayThicknessInViewSpace; 
}
#endif