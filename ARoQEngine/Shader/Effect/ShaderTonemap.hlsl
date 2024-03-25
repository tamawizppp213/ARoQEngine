//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderTonemap.hlsl
//            Content:  Color change post effect
//             Author:  Toide Yutaro
//             Create:  2020_04_30
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_TONEMAP_HLSL
#define SHADER_TONEMAP_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderVertexType.hlsli"
#include "../Core/ShaderColor.hlsli"
#include "ShaderTonemapType.hlsli"
#define VertexIn  VSInputVertex
#define VertexOut VSOutputVertex

//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////
SamplerState SamplerLinearWrap : register(s0);
Texture2D    DiffuseMap : register(t0);

// tone map type
static const int TONEMAP_NONE     = 0;
static const int TONEMAP_REINHARD = 1;
static const int TONEMAP_GT       = 2;
static const int COLOR_SPACE_BT709 = 0;
static const int COLOR_SPACE_BT2100 = 1;
//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			SelectTonemapping
*************************************************************************//**
*  @fn        float4 SelectTonemapping(float4 color)
*  @brief     Select tonemapping
*  @param[in] float4 color
*  @return    float4
*****************************************************************************/
float4 SelectTonemapping(float4 color)
{
    float4 result = 0;
    
    switch (TonemapType)
    {
        case TONEMAP_NONE:
            result.rgb = color.rgb;
            break;
        case TONEMAP_REINHARD:
            result.rgb = Reinhard(color.rgb);
            break;
        case TONEMAP_GT:
            result.rgb = GT(color.rgb);
            break;
    }
    return result;
}

float4 ConvertColorSpace(float4 color)
{
    float4 result = 0;
    
    switch (ColorSpaceType)
    {
        case COLOR_SPACE_BT709: 
            result = color;
            break;
        
        case COLOR_SPACE_BT2100:
            result.rgb = Rec709ToRec2020(color.rgb);
            break;
    }
    return result;
}

//-----------------------------------------------------------------------------
//      ITU-R BT.709 OETFを適用します.
//-----------------------------------------------------------------------------
float3 OETF_BT709(float3 color)
{
    float3 result;
    result.r = (color.r <= 0.018f) ? 4.500 * color.r : (1.0f + 0.099) * pow(abs(color.r), 0.45f) - 0.099f;
    result.g = (color.g <= 0.018f) ? 4.500 * color.g : (1.0f + 0.099) * pow(abs(color.g), 0.45f) - 0.099f;
    result.b = (color.b <= 0.018f) ? 4.500 * color.b : (1.0f + 0.099) * pow(abs(color.b), 0.45f) - 0.099f;
    return result;
}

//-----------------------------------------------------------------------------
//      ITU-R BT.2100 PQ System OETFを適用します.
//-----------------------------------------------------------------------------
float3 OETF_BT2100_PQ(float3 color)
{
    float m1 = 2610.0 / 4096.0 / 4;
    float m2 = 2523.0 / 4096.0 * 128;
    float c1 = 3424.0 / 4096.0;
    float c2 = 2413.0 / 4096.0 * 32;
    float c3 = 2392.0 / 4096.0 * 32;
    float3 cp = pow(abs(color.rgb), m1);
    return pow((c1 + c2 * cp) / (1 + c3 * cp), m2);
}

//-----------------------------------------------------------------------------
//      OETF(Opt Electronic Transfer Function)を適用します.
//-----------------------------------------------------------------------------
float4 ApplyOETF(float4 color)
{
    float4 result = 0;

    switch (ColorSpaceType)
    {
    // ITU-R BT.709 OEFT
        case COLOR_SPACE_BT709:
            result.rgb = OETF_BT709(color.rgb);
            break;

    // ITU-R BT.2100 PQ OETF
        case COLOR_SPACE_BT2100:
            result.rgb = OETF_BT2100_PQ(color.rgb);
            break;
    }

    return result;
}

VertexOut VSMain(VertexIn vertexIn)
{
    VertexOut result;
    result.Position = vertexIn.Position;
    result.UV       = vertexIn.UV;
    result.Normal   = vertexIn.Normal;
    result.Color    = vertexIn.Color;
    
    return result;
}

float4 PSMain(VertexOut input) : SV_TARGET
{
    float4 textureColor = DiffuseMap.Sample(SamplerLinearWrap, input.UV);
    
    textureColor = ConvertColorSpace(textureColor);
    textureColor = SelectTonemapping(textureColor);
    textureColor = ApplyOETF(textureColor);
    return textureColor;
}
#endif