//////////////////////////////////////////////////////+-
+////////////////////////////
//              Title:  ShaderVignette.hlsl
//            Content:  ビネット効果. 画面端を暗くするポストエフェクトです
//             Author:  Toide Yutaro
//             Create:  2023_10_06
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_VIGNETTE_HLSL
#define SHADER_VIGNETTE_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderVertexType.hlsli"
#define VertexIn  VSInputVertex
#define VertexOut VSOutputVertex
//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////
SamplerState SamplerLinearClamp : register(s0);
Texture2D    DiffuseMap         : register(t0);

cbuffer VignetteSettings : register(b0)
{
    float4 Color;     // ビネットの色
    float2 UVCenter;  // 中心座標 (ビネットは通常(0.5, 0.5))の位置から広がります.
    float  Intensity; // ビネットの強さ\, 0.0～1.0まで適用できます
    float  Smoothness; // ビネットの端っこをどれだけぼかすか-
};
//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
VertexOut VSMain(VertexIn vertexIn)
{
    // そのままPixel shaderに渡す
    VertexOut result;
    result.Position = vertexIn.Position;
    result.UV       = vertexIn.UV;
    result.Normal   = vertexIn.Normal;
    result.Color    = vertexIn.Color;
    
    return result;
}

/****************************************************************************
*                       PSMain
*************************************************************************//**
*  @fn        float4 PSMain(VertexOut input) : SV_TARGET

*  @brief     Vignette post effect

*  @param[in] VertexOut input

*  @return 　　float4
*****************************************************************************/
float4 PSMain(VertexOut input) : SV_TARGET
{
    const float4 diffuse = float4(DiffuseMap.Sample(SamplerLinearClamp, input.UV));
    
    // uvとuvCenter間の距離を取得する
    const float2 uvDistance = abs(input.UV - UVCenter) * Intensity;
    
    // 色をcenterからの距離が遠くなるほど, Colorの影響を及ぼすようにするため, 乗算値の調整.
    const float vignetteFactor = pow(saturate(1.0f - dot(uvDistance, uvDistance)), Smoothness);
    
    // centerに近いほど元のDiffuse色, 遠いほどColor色にする
    return diffuse * lerp(Color, (1.0f.xxxx), vignetteFactor); 

}
#endif