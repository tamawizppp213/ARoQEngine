//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderColorChange.hlsl
//            Content:  Color change post effect
//             Author:  Toide Yutaro
//             Create:  2020_04_30
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_COLOR_CHANGE_HLSL
#define SHADER_COLOR_CHANGE_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderVertexType.hlsli"
#define VertexIn VSInputPositionNormalTexture
#define VertexOut VSOutputPositionNormalTexture
//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////
SamplerState SamplerLinearClamp : register(s0);
Texture2D    DestDiffuseMap     : register(t0);
//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
VertexOut VSMain(VertexIn vertexIn)
{
    VertexOut result;
    result.Position = vertexIn.Position;
    result.UV       = vertexIn.UV;
    result.Normal   = vertexIn.Normal;
    
    return result;
}
/****************************************************************************
*                       PSMonochrome
*************************************************************************//**
*  @fn        float4 PSMonochrome(VertexOut input) : SV_TARGET
*  @brief     Monochrome post effect
*  @param[in] void
*  @return 　　float4
*****************************************************************************/
float4 PSMonochrome(VertexOut input) : SV_TARGET
{
    float4 dest = float4(DestDiffuseMap.Sample(SamplerLinearClamp, input.UV));
    float  Y = dot(dest.rgb, float3(0.299, 0.587, 0.114));
    return float4(Y, Y, Y, 1);
}
/****************************************************************************
*                       PSSepia
*************************************************************************//**
*  @fn        float4 PSSepia(VertexOut input) : SV_TARGET
*  @brief     Sepia post effect
*  @param[in] void
*  @return 　　float4
*****************************************************************************/
float4 PSSepia(VertexOut input) : SV_TARGET
{
    float4 dest = float4(DestDiffuseMap.Sample(SamplerLinearClamp, input.UV));
    float4 result;
    result.r = 0.393 * dest.r + 0.769 * dest.g + 0.189 * dest.b;
    result.g = 0.349 * dest.r + 0.666 * dest.g + 0.168 * dest.b;
    result.b = 0.272 * dest.r + 0.534 * dest.g + 0.131 * dest.b;
    result.a = dest.a;
    return result;
}
/****************************************************************************
*                       PSGrayScale
*************************************************************************//**
*  @fn        float4 PSGrayScale(VertexOut input) : SV_TARGET
*  @brief     Gray scale post effect
*  @param[in] void
*  @return 　　float4
*****************************************************************************/
float4 PSGrayScale(VertexOut input) : SV_TARGET
{
    float4 dest = float4(DestDiffuseMap.Sample(SamplerLinearClamp, input.UV));
    float Y     = dot(dest.rgb, float3(0.2126, 0.7152, 0.0722));
    return float4(Y, Y, Y, 1);
}
/****************************************************************************
*                       PSBinary
*************************************************************************//**
*  @fn        float4 PSBinary(VertexOut input) : SV_TARGET
*  @brief     Binary post effect
*  @param[in] void
*  @return 　　float4
*****************************************************************************/
float4 PSBinary(VertexOut input) : SV_TARGET
{
    float4 dest = float4(DestDiffuseMap.Sample(SamplerLinearClamp, input.UV));
    float Y = dot(dest.rgb, float3(0.2126, 0.7152, 0.0722));
    Y = (Y < 128.0f / 255.0f) ? 0 : 1;
    float4 result = float4(Y, Y, Y, 1);
    return result;

}
/****************************************************************************
*                       PSInvert
*************************************************************************//**
*  @fn        float4 PSInvert(VertexOut input) : SV_TARGET
*  @brief     Invert post effect
*  @param[in] void
*  @return 　　float4
*****************************************************************************/
float4 PSInvert(VertexOut input) : SV_TARGET
{
    float4 dest = float4(DestDiffuseMap.Sample(SamplerLinearClamp, input.UV));
    float4 result = float4(1.0f - dest.rgb, dest.a);
    return result;
}
#endif