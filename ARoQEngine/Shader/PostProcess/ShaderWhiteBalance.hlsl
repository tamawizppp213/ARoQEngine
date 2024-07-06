//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderWhiteBalance.hlsl
//            Content:  ホワイトバランスを決定します. 色温度と色合いを変更できます
//             Author:  Toide Yutaro
//             Create:  2023_10_06
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_WHITE_BALANCE_HLSL
#define SHADER_WHITE_BALANCE_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderVertexType.hlsli"
#include "../Core/ShaderColor.hlsli"
#define VertexIn  VSInputVertex
#define VertexOut VSOutputVertex
//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////
SamplerState SamplerLinearClamp : register(s0);
Texture2D DiffuseMap : register(t0);

cbuffer WhiteBalanceSettings : register(b0)
{
    float Temperature; // 色温度 (-1～1)
    float Tint;        // 色合い
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
****************************************************************************/
/* @fn        float4 PSMain(VertexOut input) : SV_TARGET

*  @brief     Vignette post effect

*  @param[in] VertexOut input

*  @return 　　float4
*****************************************************************************/
float4 PSMain(VertexOut input) : SV_TARGET
{
    const float4 diffuse = float4(DiffuseMap.Sample(SamplerLinearClamp, input.UV));
    
    // 範囲としては[-1.67 ～ 1.67]が最適とのこと
    // https://docs.unity3d.com/ja/Packages/com.unity.shadergraph@10.0/manual/White-Balance-Node.html
    const float temperature = Temperature * 10 / 6;
    const float tint        = Tint * 10 / 6;
    
    /*-------------------------------------------------------------------
	-        参照白色点のCIE_RGB表色系を用いたxy色度を取得する
	---------------------------------------------------------------------*/
    const float whitePointX = 0.31271 - temperature * (temperature < 0 ? 0.1 : 0.05);;
    const float standardIlluminantY = 2.87 * whitePointX - 3 * whitePointX * whitePointX - 0.27509507;
    const float whitePointY = standardIlluminantY + tint * 0.05;
    
    // LMS空間の相互効果を計算
    const float3 w   = float3(0.949237, 1.03542, 1.08728);
    const float3 LMS = xyYToLMS(float2(whitePointX, whitePointY));
    
    // ホワイトバランスの取得
    const float3 balance = float3(w.x / LMS.x, w.y / LMS.y, w.z / LMS.z);
    
    /*-------------------------------------------------------------------
	-        LMS空間を使ってホワイトバランスの調整を行う.
	---------------------------------------------------------------------*/
    float3 lmsColor = LinearToLMS(diffuse.rgb);
    lmsColor *= balance;
    const float3 linearColor = LMSToLinear(lmsColor);
    
    return float4(linearColor, 1);

}
#endif