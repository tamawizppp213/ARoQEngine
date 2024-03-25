//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderMosaic.hlsl
//            Content:  Mosaic post effect
//             Author:  Toide Yutaro
//             Create:  2023_02_23
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_MOSAIC_HLSL
#define SHADER_MOSAIC_HLSL
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
Texture2D    DestDiffuseMap     : register(t0);
cbuffer MosaicInfo : register(b0)
{
    float2 WindowSize; // pixel
    float  BlockSize;  // pixel
    float  Padding;
}

//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
VertexOut VSMain(VertexIn vertexIn)
{
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

*  @brief     Mosaic post effect

*  @param[in] VertexOut input

*  @return Å@Å@float4
*****************************************************************************/
float4 PSMain(VertexOut input) : SV_TARGET
{
    const float2 originalPixel = input.UV * WindowSize;
    const float2 blockPixelF   = originalPixel / BlockSize;
    const float2 blockPixel    = floor(blockPixelF) * BlockSize;
    const float2 newUV         = blockPixel / WindowSize;
    
    return DestDiffuseMap.Sample(SamplerLinearClamp, newUV);
}
#endif