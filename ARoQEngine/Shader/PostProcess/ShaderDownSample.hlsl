//////////////////////////////////////////////////////////////////////////////////
///  @file   ShaderDownSample.cpp
///  @brief  描画シーンを縦横半分に縮小するシェーダ. ぼかしを入れることで品質を向上しています
///  @author toide
///  @date   2024/07/06 14:20:34
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_DOWN_SAMPLE_HLSL
#define SHADER_DOWN_SAMPLE_HLSL

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
Texture2D    SceneTexture      : register(t0);
SamplerState SamplerLinearWrap : register(s0);

/****************************************************************************
*				  			SceneTextureInfo
****************************************************************************/
/* @brief シーンテクスチャを使用する情報
*****************************************************************************/
cbuffer SceneTextureInfo : register(b0)
{
    float2 SceneTextureInvertSize; // シーンテクスチャの逆サイズ
    uint2  ViewPortMax;            // ビューポートの最大ピクセル値 
    uint2  ViewPortMin;            // ビューポートの最小ピクセル値 
};

struct PSInput
{
    float4 Position : SV_POSITION;
    float2 UV       : TEXCOORD;
};

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
/*!**********************************************************************
*  @brief     UV値にしたがってテクスチャをサンプリング
*  @param[in] const float2 UV値
*  @return    float4 サンプリングした色
*************************************************************************/
float4 SampleTexture(const float2 uv)
{
    return SceneTexture.SampleLevel(SamplerLinearWrap, saturate(uv), 0);
}

/*!**********************************************************************
*  @brief     Pixel ShaderでもCompute Shaderでも実行するダウンサンプリング関数
*  @param[in] const float2 UV値
*  @return    float4 サンプリングした色
*************************************************************************/
float4 ExecuteDownSample(const float2 uv) : SV_TARGET
{
    // サンプリングするUV値を取得 (ぼかしを入れることでDownSampleの品質を向上しています)
    float2 UVs[4];
    UVs[0] = uv + SceneTextureInvertSize * float2(-1, 1);
    UVs[1] = uv + SceneTextureInvertSize * float2(1, 1);
    UVs[2] = uv + SceneTextureInvertSize * float2(-1, -1);
    UVs[3] = uv + SceneTextureInvertSize * float2(1, -1);
    
    // ぼかしによる周囲のテクセルを取得
    float4 Sample[4];
    
    [UNROLL]
    for (int i = 0; i < 4; ++i)
    {
        Sample[i] = SampleTexture(UVs[i]);
    }
    
    // ぼかしによる周囲のテクセルを合計して平均を取得
    float4 result = (Sample[0] + Sample[1] + Sample[2] + Sample[3]) * 0.25f;
    
    // UEによると, まれに黄色になることがあるらしいので, Clampのような扱い
    result.rgb = max(float3(0,0,0), result.rgb);

    return result;
}

#ifdef USE_COMPUTE_SHADER
RWTexture2D<float4> OutputTexture : register(u0);

// デフォルト値の設定
#ifndef THREADGROUP_SIZE_X
    #define THREADGROUP_SIZE_X 8
#endif
#ifndef THREADGROUP_SIZE_Y
    #define THREADGROUP_SIZE_Y 8
#endif

/*!**********************************************************************
*  @brief     コンピュートシェーダーの実行
*  @param[in] const uint2 dispatchThreadID
*  @return    void
*************************************************************************/
[numthreads(THREADGROUP_SIZE_X, THREADGROUP_SIZE_Y, 1)]
void MainCS(uint2 dispatchThreadID : SV_DispatchThreadID)
{
    const uint2  pixelPosition = dispatchThreadID + ViewPortMin;
    const float2 uv            = (float2)dispatchThreadID * SceneTextureInvertSize;
    
    if (all(pixelPosition < ViewPortMax))
    {
        OutputTexture[pixelPosition] = ExecuteDownSample(uv);
    }
}
#else // PixelShader
/*!**********************************************************************
*  @brief     ピクセルシェーダを実行
*  @param[in] const PSInput input
*  @return    float4
*************************************************************************/
float4 MainPS(const PSInput input) : SV_TARGET
{
    return ExecuteDownSample(input.UV);
}
#endif // USE_PIXEL_SHADER


#endif