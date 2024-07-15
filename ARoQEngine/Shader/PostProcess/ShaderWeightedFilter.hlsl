//////////////////////////////////////////////////////////////////////////////////
///  @file   ShaderWeightedFilter.hlsl
///  @brief  周囲のピクセルの色を重み付けしてフィルタリングするシェーダ @n
///          具体的には, ガウシアンブラーの処理などに使用します. WeightはCPU側で正規化しておく必要があります. @n
///  @note   使用可能マクロ @n
///          - WEIGHT_SAMPLE_COUNT(Num   ) : 重みサンプル数を指定します. デフォルトは9です. @n
///          - USE_UNPACKED_UV    (Switch) : UV値を半分にすることでループ回数を減らすことができます.
///          - USE_COMPUTE_SHADER (Switch) : コンピュートシェーダーを使用します. 使用する場合はTHREADGROUP_SIZE_X, Yを定義してください@n
///  @author toide
///  @date   2024/07/15 20:00:26
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_WEIGHTED_FILTER_HLSL
#define SHADER_WEIGHTED_FILTER_HLSL

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderVertexType.hlsli"

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
Texture2D    SceneTexture      : register(t0);
SamplerState Sampler           : register(s0);

// デフォルトの重みサンプル数
#ifndef WEIGHT_SAMPLE_COUNT
    #define WEIGHT_SAMPLE_COUNT (9)
#endif

// ループ回数を減らすのに役立ちます. float4のxy, zwにそれぞれ別のUVOffset値を格納するため, 半分になります.
#ifdef USE_UNPACKED_UV
    #define PACKED_SAMPLE_COUNT WEIGHT_SAMPLE_COUNT
#else
    #define PACKED_SAMPLE_COUNT ((WEIGHT_SAMPLE_COUNT + 1) / 2)
#endif

/****************************************************************************
*				  		SceneTextureInfo
****************************************************************************/
/* @brief  シーンテクスチャを使用する情報
*****************************************************************************/
cbuffer SceneTextureInfo : register(b0)
{
    float2 SceneTextureInvertSize; // シーンテクスチャの逆サイズ
    uint2  ViewPortMax;            // ビューポートの最大ピクセル値
    uint2  ViewPortMin;            // ビューポートの最小ピクセル値
};

/****************************************************************************
*				  		WeightTable
****************************************************************************/
/* @brief  周囲のピクセルの重みを格納する構造体. 重みはCPU側で正規化しておく必要があります.
*****************************************************************************/
cbuffer WeightTable : register(b1)
{
    float4 Weights[WEIGHT_SAMPLE_COUNT]; //!< 重みテーブル
    float4 Offsets[PACKED_SAMPLE_COUNT]; //!< オフセットテーブル
};

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////

/*!**********************************************************************
*  @brief     UV値にしたがってテクスチャをサンプリング
*  @param[in] const float2 UV値
*  @return    float4 サンプリングした色
*************************************************************************/
float4 SampleFilterTexture(const float2 uv)
{
    return SceneTexture.SampleLevel(Sampler, uv, 0);
}

/*-------------------------------------------------------------------
-       Compute Shader
---------------------------------------------------------------------*/
#ifdef USE_COMPUTE_SHADER
RWTexture2D<float4> OutputTexture : register(u0);

// デフォルト値の設定
#ifndef THREADGROUP_SIZE_X
    #define THREADGROUP_SIZE_X 8
#endif // THREADGROUP_SIZE_X
#ifndef THREADGROUP_SIZE_Y
    #define THREADGROUP_SIZE_Y 8
#endif // THREADGROUP_SIZE_Y

/*!**********************************************************************
*  @brief     コンピュートシェーダーの実行
*  @param[in] const uint2 dispatchThreadID
*  @return    void
*************************************************************************/
[numthreads(THREADGROUP_SIZE_X, THREADGROUP_SIZE_Y, 1)]
void MainCS(uint2 dispatchThreadID : SV_DispatchThreadID)
{
    // サンプリングするUV値を取得
    const uint2  pixelPosition = dispatchThreadID + ViewPortMin;
    const float2 baseUV        = (float2(pixelPosition) + 0.5f) * SceneTextureInvertSize;
    
    // Compute Shaderにおいて, ピクセルの領域外にアクセスすることはできないため, 何もしない
    const float2 centerUVDistance = abs(baseUV - 0.5f);
    if (max(centerUVDistance.x, centerUVDistance.y) >= 0.5f) // UV値が0.0f ~ 1.0fの範囲に収まっているか
    {
        return;
    }
    
    float4 result      = float4(0, 0, 0, 0);
    int    sampleIndex = 0;
    // 隣接ピクセルを重みをづけながら加算する. (平均化を行っていないため, 必ずCPU側で隣接ピクセルのWeightTableの和を正規化しておいてください)
#ifdef USE_UNPACKED_UV 
    [Unroll]
    for (; sampleIndex < WEIGHT_SAMPLE_COUNT; ++sampleIndex)
    {
        result += SampleFilterTexture(Offsets[sampleIndex]) * Weights[sampleIndex];
    }
#else
    [Unroll]
    for (; sampleIndex < WEIGHT_SAMPLE_COUNT - 1; sampleIndex += 2)
    {
        float4 offset = baseUV.xyxy + Offsets[sampleIndex / 2];
        result += SampleFilterTexture(offset.xy) * Weights[sampleIndex + 0];
        result += SampleFilterTexture(offset.zw) * Weights[sampleIndex + 1];
    }
    
    [FLATTEN]  
    if (sampleIndex < WEIGHT_SAMPLE_COUNT)
    {
        const float2 uv = baseUV.xy + Offsets[sampleIndex / 2].xy;
        result += SampleFilterTexture(uv) * Weights[sampleIndex + 0];
    }
#endif // USE_UNPACKED_UV
    
    OutputTexture[dispatchThreadID.xy] = result;
}
/*-------------------------------------------------------------------
-       Pixel Shader
---------------------------------------------------------------------*/
#else // USE_COMPUTE_SHADER

/****************************************************************************
*				  		PSInput
****************************************************************************/
/* @brief  ピクセルシェーダーに入力する際に使用する構造体. 隣接ピクセルの情報が入っています.
*****************************************************************************/
struct PSInput
{
    /*! @brief  ピクセルの位置 */
    float4 Position : SV_POSITION;
    
    /*! @brief  中心のUV値 */
    float2 UV : TEXCOORD;
    
    /*! @brief  隣接ピクセルのUV値 (中心からの差分を取得しているわけではありません)*/
#if USE_UNPACKED_UV
    float2 Offsets[WEIGHT_SAMPLE_COUNT]  : TEXCOORD1;
#else
    float4 Offsets[PACKED_SAMPLE_COUNT] : TEXCOORD1;
#endif
};

/*!**********************************************************************
*  @brief     頂点シェーダを実行
*  @param[in] void
*  @return    PSInputPositionTexture
*************************************************************************/
PSInput MainVS(VSInputPositionTexture input)
{
    PSInput output = (PSInput)0;
    output.Position = float4(input.Position, 0.0f, 1.0f);
    output.UV       = input.UV;
    
#ifdef USE_UNPACKED_UV
    [Unroll]
    for (int i = 0; i < WEIGHT_SAMPLE_COUNT; ++i)
    {
        output.Offsets[i] = input.UV + Offsets[i];
    }
#else
    [Unroll]
    for (int i = 0; i < PACKED_SAMPLE_COUNT; ++i)
    {
        output.Offsets[i] = input.UV.xyxy + Offsets[i];
    }
#endif
    
    return output;
}

/*!**********************************************************************
*  @brief     ピクセルシェーダを実行
*  @param[in] const PSInput input
*  @return    float4
*************************************************************************/
float4 MainPS(const PSInput input) : SV_TARGET
{
    // サンプリングするUV値を取得 (ぼかしを入れることでDownSampleの品質を向上しています)
    float4 result = float4(0, 0, 0, 0); 
    
    // 隣接ピクセルを重みをづけながら加算する. (平均化を行っていないため, 必ずCPU側で隣接ピクセルのWeightTableの和を正規化しておいてください)
#ifdef USE_UNPACKED_UV 
    [Unroll]
    for (int i = 0; i < WEIGHT_SAMPLE_COUNT; ++i)
    {
        result += SampleFilterTexture(input.Offsets[i]) * Weights[i];
    }
#else // USE_UNPACKED_UV
    [Unroll]
    for (int i = 0; i < WEIGHT_SAMPLE_COUNT - 1; i += 2)
    {
        float4 offset = input.Offsets[i / 2];
        result += SampleFilterTexture(offset.xy) * Weights[i + 0];
        result += SampleFilterTexture(offset.zw) * Weights[i + 1];
    }
    
    // 重みサンプル数が奇数の場合, 最後のピクセルをサンプリング (最下位ビットを取得することで奇数か判定できる)
    [FLATTEN] // ほとんどこの条件に入ります. 
    if (WEIGHT_SAMPLE_COUNT & 1) 
    {
        result += SampleFilterTexture(input.Offsets[PACKED_SAMPLE_COUNT - 1].xy) * Weights[WEIGHT_SAMPLE_COUNT - 1];
    }
#endif // USE_UNPACKED_UV
    
    return result;
} 

#endif // USE_COMPUTE_SHADER

#endif 
