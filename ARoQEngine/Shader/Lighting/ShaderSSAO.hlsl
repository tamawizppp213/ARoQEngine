//////////////////////////////////////////////////////////////////////////////////
//             Title:  ShaderSSAO.hlsl
//             Content:  ssao (compute shader)
//             Author:  Reference : Frack D Luna
//             Create:  2023_04_01
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_SSAO_HPP
#define SHADER_SSAO_HPP
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderConstantBuffer3D.hlsli"
#include "../Core/ShaderVertexType.hlsli"

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
//       1.0     -------------\
//               |           |  \
//               |           |    \
//               |           |      \
//               |           |        \
//               |           |          \
//               |           |            \
//  ------|------|-----------|-------------|---------|--> zv
//        0     Eps          z0            z1        
//
#ifndef SAMPLE_COUNT
    #define SAMPLE_COUNT (14)
#endif

cbuffer SSAOSetting : register(b3)
{
    float2 TextureSize;
    float  Radius;         // view space
    float  Sharpness;
    
    float  FadeStart;      // view space
    float  FadeEnd;        // view space
    float  SurfaceEpsilon; // view space
    int    PaddingSSAO;
    
    float4 OffsetVectors[SAMPLE_COUNT];
}

cbuffer BlurMode : register(b4)
{
    float4 Weights[2];
    float  BlurRadius;
    bool   IsHorizontal; // Horizontal or vertical blur.
}

// Texture
Texture2D<float4>   NormalTexture    : register(t0);
Texture2D<float>    DepthTexture     : register(t1);
Texture2D<float4>   BlueNoiseTexture : register(t2);
Texture2D<float4>   InputTexture     : register(t3);
RWTexture2D<float4> OutputTexture    : register(u0);

// Sampler
SamplerState SamplerPointClamp : register(s1);
SamplerState SamplerDepth      : register(s2);

// Variables
struct VertexOut
{
    float4   Position          : SV_Position; // NDC space
    float3   NearPlaneViewPosition : POSITION;     
    float2   UV                    : TEXCOORD0;
    float4x4 ProjectionTexture : PROJECTION;
};


//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
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
*				  			CalculateOcclusion
****************************************************************************/
/* @fn        float CalculateOcclusion(const float distZ)

*  @brief     Return calculate occlusion ratio from 0.0f to 1.0f

*  @param[in] const float distZ

*  @return    float
*****************************************************************************/
float CalculateOcclusion(const float distanceViewZ)
{
    // if (dist <= surface epsilon, don't occlude the object)
    if (distanceViewZ <= SurfaceEpsilon){ return 0.0f; }
    
    const float fadeLength = FadeEnd - FadeStart;
    const float occlusion  = saturate((FadeEnd - distanceViewZ) / fadeLength);
    return occlusion;
}

float4 GetBlurWeightedPixelColor(const float weight, const float2 uv)
{
    return weight * InputTexture.Sample(SamplerPointClamp, uv);
}

VertexOut VSMain( const VSInputVertex vertexIn)
{
    VertexOut result;
    
    // ここでは, 長方形メッシュを対象にしています. 
    result.UV = vertexIn.UV;
    
    // Rectangle covering screen in NDC space
    result.Position = vertexIn.Position;
    
    // Transform rectangle corners to view space near plane
    const float4 position = mul(InverseProjection, vertexIn.Position);
    result.NearPlaneViewPosition   = position.xyz / position.w;

    float4x4 T =
    {
        0.5f, 0.0f, 0.0f, 0.5f,
        0.0f, -0.5f, 0.0f, 0.5f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    
    // Rect uv value in projection space.
    result.ProjectionTexture = mul(T, Projection);
    return result;
}

float4 ExecuteSSAO(VertexOut input) : SV_Target
{
    /*-------------------------------------------------------------------
	-        Get ssao variables
	---------------------------------------------------------------------*/
    // 深度マップからNDC空間におけるこのピクセルのz座標を取得する
    // 深度マップの定義はzPrefass.hlslを参照してください.
    const float ndcDepth  = DepthTexture.Sample(SamplerDepth, input.UV).r; // LODを使わないことが分かるならSampleLevel
    
    // ビュー空間における深度 (カメラ -> 視点までの深度)
    const float viewSurfaceDepth = NDCDepthToViewDepth(ndcDepth);
    
    // 深度の値をもとに, View空間におけるサンプリング位置を再構成する
    const float  distanceRatio       = (viewSurfaceDepth / input.NearPlaneViewPosition.z); // 
    const float3 viewSurfacePosition = distanceRatio * input.NearPlaneViewPosition; // NearPlaneは単位ベクトル的な扱い.
    
    // Acquire world normal: the normal value range is converted 0～1 into -1 ～ 1.
    // 法線マップを0～1から, -1～1に変換する. 法線はワールド空間のものを使用する. 
    const float3 normal = (NormalTexture.Sample(SamplerPointClamp, input.UV).xyz * 2) - 1;
    
    // Blue noiseをランダム値には使用し, なるべく均等にサンプリングされるようにする. また, 範囲を0～1から-1～1に変換する
    const float3 randomVector = normalize(2.0f * BlueNoiseTexture.Sample(SamplerLinearWrap, 4.0f * input.UV, 0).xyz - 1);
    
    float occlusionSum = 0.0f;
    float division     = 0.0f;
    
    /*-------------------------------------------------------------------
	-        Execute SSAO
	---------------------------------------------------------------------*/
    // 法線方向に半球状に放射する近接点をサンプリング
    for(int i = 0; i < SAMPLE_COUNT; ++i)
    {
        // offset Vectorは固定され, 一様に分散している.(offset vectorが固まらないようにする処置です. )
        // offset vectorをランダムなベクトルに対して適用すると, オフセットベクトルのランダムな一様分布が得られます. 
        const float3 randomOffset = reflect(OffsetVectors[i].xyz, randomVector);
        
        // offset vectorが(p, n)で定義される平面の後ろにある場合、それを反転させる。
        const float  flip   = sign(dot(randomOffset, normal));
        
        // ランダムなサンプリング位置は, Occlude radius内に取得できるようにする. 
        const float3 randomSamplePosition = Radius * flip * randomOffset + viewSurfacePosition.xyz;
        
        // Project random sample position and generate projective tex-coords.
        const float4 projectionRandomSample = mul(float4(randomSamplePosition, 1.0f), input.ProjectionTexture) / projectionRandomSample.w;
        
        // 目からランダムなサンプル位置までのレイに沿って, 最も近い深度値を見つける
        // これはランダムなサンプル位置についての深度値ではないことは注意しておく
        const float ndcRandomSampleDepth = DepthTexture.Sample(SamplerDepth, projectionRandomSample.xy);
        const float viewSampleDepth      = NDCDepthToViewDepth(ndcRandomSampleDepth);
        
        // 奥行きのある点のビュー空間の位置を再構築する。
        const float3 viewRandomSamplePosition = (viewSampleDepth / randomSamplePosition.z) * randomSamplePosition;
        
        // Random Sample位置がsurface positionを遮蔽するかどうか
        // dot(normal, normalize(viewRandomSamplePosition - viewSurfacePosition))は, occluder pointがどれだけplaneの正面にいるかを測定
        // 正面になるほど, 遮蔽可能性が高いため, 遮蔽重みを追加する. 
        const float distanceViewZ = viewSurfacePosition.z - viewRandomSamplePosition.z; 
        const float dp            = max(dot(normal, normalize(viewRandomSamplePosition - viewSurfacePosition)), 0.0f);
        const float occlusion = CalculateOcclusion(distanceViewZ) * dp;
        occlusionSum += occlusion;
    }
    
    // 平均
    occlusionSum /= (float) (SAMPLE_COUNT);
    
    const float4 result = saturate(pow(1.0f - occlusionSum, Sharpness));
    return result;
}

float4 ExecuteSSAOBlur(const VertexOut input) : SV_Target
{
    /*-------------------------------------------------------------------
	-        Prepare weight setting
	---------------------------------------------------------------------*/
    const int arraySize = 8;
    
    // prepare the one dimensional list from float4[2] data to use the loop.
    const float blurWeights[arraySize] =
    {
        Weights[0].x, Weights[0].y, Weights[0].z, Weights[0].w,
        Weights[1].x, Weights[1].y, Weights[1].z, Weights[1].w,
    };
    
    // UV offset based on the one pixel size. This value is set in horizontal or vertical direction.
    float2 uvOffset;
    if(IsHorizontal){uvOffset = float2(InverseRenderTargetSize.x, 0.0f);}
    else            {uvOffset = float2(0.0f, InverseRenderTargetSize.y);}
    
    // center position normal and depth 
    const float3 centerNormal = NormalTexture.Sample(SamplerPointClamp, input.UV).xyz;
    const float  centerDepth  = DepthTexture .Sample(SamplerDepth, input.UV);
    
    /*-------------------------------------------------------------------
	-        Calculate total weight
	---------------------------------------------------------------------*/
    float4 color       = GetBlurWeightedPixelColor(blurWeights[0], input.UV); // center color
    float  totalWeight = blurWeights[0];
    
    for (int i = 1; i < arraySize; ++i)
    {
        // plus : right or up direction, minus : left or down direction to the center uv position.
        const int moveDirection[2] = { +1, -1 };
        
        [unroll]
        for (int j = 0; j < 2; ++j)
        {
            const float2 uv             = input.UV + (float)moveDirection[j] * uvOffset * (float)i;
            const float3 neighborNormal = NormalTexture.Sample(SamplerPointClamp, uv).xyz;
            const float  neighborDepth  = DepthTexture .Sample(SamplerDepth, uv);
            
            // if the cventer value and neighbor values differ too much (either depth or normal), 
            // then we assume we are sampling across a discontinuity. 
            // We discard such samples from the blur.
            if (dot(neighborNormal, centerNormal) < 0.8f)  {continue;}
            if (abs(neighborDepth - centerDepth)  > 0.2f)  {continue;}

            const float weight = blurWeights[i];
            
            // Add neighbor pixel to blur
            color       += GetBlurWeightedPixelColor(weight, uv);
            totalWeight += weight;
        }
    }
    
    return color / totalWeight;
}

#endif
