//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderGaussianBlur.hlsl
//            Content:  Draw Blur Effect (compute shader)
//             Author:  Toide Yutaro
//             Create:  2022_05_01
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_GAUSSIAN_BLUR_HLSL
#define SHADER_GAUSSIAN_BLUR_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderVertexType.hlsli"
#include "../Core/ShaderConstantBuffer3D.hlsli"

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
static const uint Thread = 16;

cbuffer WeightTable : register(b1)
{
    float4 Weights[2];
}
cbuffer TextureSize : register(b2)
{
    uint2 OriginalTexture;
    uint2 XBlurTexture;
    uint2 YBlurTexture;
}
Texture2D           inputImage  : register(t0);
RWTexture2D<float4> outputImage : register(u0);
RWTexture2D<float4> xHalfImage  : register(u1); // 1 / 2
RWTexture2D<float4> xyHalfImage : register(u2); // 1 / 4

struct PSBlurInput
{
    float4 Position : SV_POSITION;
    float4 UV0      : TEXCOORD0;
    float4 UV1      : TEXCOORD1;
    float4 UV2      : TEXCOORD2;
    float4 UV3      : TEXCOORD3;
    float4 UV4      : TEXCOORD4;
    float4 UV5      : TEXCOORD5;
    float4 UV6      : TEXCOORD6;
    float4 UV7      : TEXCOORD7;
};

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			GetPixelColor
*************************************************************************//**
*  @fn        float4 GetPixelColor(int x, int y, int2 textureSize)
*  @brief     Convert RGBA32 to float4 format
*  @param[in] int x
*  @param[in] int y
*  @param[in] int2 textureSizefloat4 UV0      : TEXCOORD0;
*****************************************************************************/
float4 GetPixelColor(int x, int y)
{
    return inputImage[int2(x, y)];
}

[numthreads(Thread, Thread, 1)]
void ExecuteBlur(uint3 id : SV_DispatchThreadID)
{
    uint2 basePosition = uint2(id.x * 2, id.y);
    float4 color = GetPixelColor(basePosition.x, basePosition.y) * Weights[0].x;
    color += GetPixelColor(basePosition.x + 1, basePosition.y) * Weights[0].y;
    color += GetPixelColor(basePosition.x + 2, basePosition.y) * Weights[0].z;
    color += GetPixelColor(basePosition.x + 3, basePosition.y) * Weights[0].w;
    color += GetPixelColor(basePosition.x + 4, basePosition.y) * Weights[1].x;
    color += GetPixelColor(basePosition.x + 5, basePosition.y) * Weights[1].y;
    color += GetPixelColor(basePosition.x + 6, basePosition.y) * Weights[1].z;
    color += GetPixelColor(basePosition.x + 7, basePosition.y) * Weights[1].w;
    
    color += GetPixelColor(basePosition.x - 1, basePosition.y) * Weights[0].y;
    color += GetPixelColor(basePosition.x - 2, basePosition.y) * Weights[0].z;
    color += GetPixelColor(basePosition.x - 3, basePosition.y) * Weights[0].w;
    color += GetPixelColor(basePosition.x - 4, basePosition.y) * Weights[1].x;
    color += GetPixelColor(basePosition.x - 5, basePosition.y) * Weights[1].y;
    color += GetPixelColor(basePosition.x - 6, basePosition.y) * Weights[1].z;
    color += GetPixelColor(basePosition.x - 7, basePosition.y) * Weights[1].w;
    
    xHalfImage[id.xy] = color;
    GroupMemoryBarrierWithGroupSync();
    
    /*-------------------------------------------------------------------
    -         Y Blur
    ---------------------------------------------------------------------*/
    basePosition = uint2(id.x, id.y * 2);
    color  = xHalfImage[uint2(basePosition.x, basePosition.y + 0)] * Weights[0].x;
    color += xHalfImage[uint2(basePosition.x, basePosition.y + 1)] * Weights[0].y;
    color += xHalfImage[uint2(basePosition.x, basePosition.y + 2)] * Weights[0].z;
    color += xHalfImage[uint2(basePosition.x, basePosition.y + 3)] * Weights[0].w;
    color += xHalfImage[uint2(basePosition.x, basePosition.y + 4)] * Weights[1].x;
    color += xHalfImage[uint2(basePosition.x, basePosition.y + 5)] * Weights[1].y;
    color += xHalfImage[uint2(basePosition.x, basePosition.y + 6)] * Weights[1].z;
    color += xHalfImage[uint2(basePosition.x, basePosition.y + 7)] * Weights[1].w;
    
    color += xHalfImage[uint2(basePosition.x, basePosition.y - 1)] * Weights[0].y;
    color += xHalfImage[uint2(basePosition.x, basePosition.y - 2)] * Weights[0].z;
    color += xHalfImage[uint2(basePosition.x, basePosition.y - 3)] * Weights[0].w;
    color += xHalfImage[uint2(basePosition.x, basePosition.y - 4)] * Weights[1].x;
    color += xHalfImage[uint2(basePosition.x, basePosition.y - 5)] * Weights[1].y;
    color += xHalfImage[uint2(basePosition.x, basePosition.y - 6)] * Weights[1].z;
    color += xHalfImage[uint2(basePosition.x, basePosition.y - 7)] * Weights[1].w;
    
    xyHalfImage[id.xy] = color;
    GroupMemoryBarrierWithGroupSync();
    
    /*-------------------------------------------------------------------
    -         Bilinear filter
    ---------------------------------------------------------------------*/
    basePosition = uint2(id.x / 2, id.y / 2);
    
    color = xyHalfImage[uint2(basePosition.x + 0, basePosition.y + 0)];
    color += xyHalfImage[uint2(basePosition.x + 0, basePosition.y + 1)];
    color += xyHalfImage[uint2(basePosition.x + 1, basePosition.y + 0)];
    color += xyHalfImage[uint2(basePosition.x + 1, basePosition.y + 1)];
    color /= 4.0f;
    
    outputImage[id.xy] = color;
}

PSBlurInput VS_XBlur(VSInputVertex input)
{
    PSBlurInput result;
    
	result.Position = input.Position;
    
    /*-------------------------------------------------------------------
	-        Acquire the texture size
	---------------------------------------------------------------------*/
    float2 textureSize = OriginalTexture;
    
    /*-------------------------------------------------------------------
	-        Set blur uv
	---------------------------------------------------------------------*/
    const float2 baseUV = float2(input.UV.x * 4, input.UV.y); 
    // Ç»Ç∫4î{Ç≈è„éËÇ≠Ç¢Ç≠ÇÃÇ©ï™Ç©ÇÁÇ»Ç¢... å„ÇŸÇ«óvåèìô
    
    result.UV0.xy = float2(1.0f  / textureSize.x, 0.0f);
    result.UV1.xy = float2(3.0f  / textureSize.x, 0.0f);
    result.UV2.xy = float2(5.0f  / textureSize.x, 0.0f);
    result.UV3.xy = float2(7.0f  / textureSize.x, 0.0f);
    result.UV4.xy = float2(9.0f  / textureSize.x, 0.0f);
    result.UV5.xy = float2(11.0f / textureSize.x, 0.0f);
    result.UV6.xy = float2(13.0f / textureSize.x, 0.0f);
    result.UV7.xy = float2(15.0f / textureSize.x, 0.0f);
    
    result.UV0.zw = result.UV0.xy * (-1.0f);
    result.UV1.zw = result.UV1.xy * (-1.0f);
    result.UV2.zw = result.UV2.xy * (-1.0f);
    result.UV3.zw = result.UV3.xy * (-1.0f);
    result.UV4.zw = result.UV4.xy * (-1.0f);
    result.UV5.zw = result.UV5.xy * (-1.0f);
    result.UV6.zw = result.UV6.xy * (-1.0f);
    result.UV7.zw = result.UV7.xy * (-1.0f);
    
    // Add offset
    result.UV0 += float4(baseUV, baseUV);
    result.UV1 += float4(baseUV, baseUV);
    result.UV2 += float4(baseUV, baseUV);
    result.UV3 += float4(baseUV, baseUV);
    result.UV4 += float4(baseUV, baseUV);
    result.UV5 += float4(baseUV, baseUV);
    result.UV6 += float4(baseUV, baseUV);
    result.UV7 += float4(baseUV, baseUV);
    return result;
}

PSBlurInput VS_YBlur(VSInputVertex input)
{
    PSBlurInput result;
    
    /*-------------------------------------------------------------------
	-        transform to world space 
	---------------------------------------------------------------------*/
    result.Position = input.Position;
    
    /*-------------------------------------------------------------------
	-        Acquire the texture size
	---------------------------------------------------------------------*/
    float2 textureSize = XBlurTexture;
    
    /*-------------------------------------------------------------------
	-        Set blur uv
	---------------------------------------------------------------------*/
    const float2 baseUV = float2(input.UV.x, input.UV.y * 2);
    
    result.UV0.xy = float2(0.0f, 1.0f  / textureSize.y);
    result.UV1.xy = float2(0.0f, 3.0f  / textureSize.y);
    result.UV2.xy = float2(0.0f, 5.0f  / textureSize.y);
    result.UV3.xy = float2(0.0f, 7.0f  / textureSize.y);
    result.UV4.xy = float2(0.0f, 9.0f  / textureSize.y);
    result.UV5.xy = float2(0.0f, 11.0f / textureSize.y);
    result.UV6.xy = float2(0.0f, 13.0f / textureSize.y);
    result.UV7.xy = float2(0.0f, 15.0f / textureSize.y);
    
    result.UV0.zw = result.UV0.xy * (-1.0f);
    result.UV1.zw = result.UV1.xy * (-1.0f);
    result.UV2.zw = result.UV2.xy * (-1.0f);
    result.UV3.zw = result.UV3.xy * (-1.0f);
    result.UV4.zw = result.UV4.xy * (-1.0f);
    result.UV5.zw = result.UV5.xy * (-1.0f);
    result.UV6.zw = result.UV6.xy * (-1.0f);
    result.UV7.zw = result.UV7.xy * (-1.0f);
    
    // Add offset
    result.UV0 += float4(baseUV, baseUV);
    result.UV1 += float4(baseUV, baseUV);
    result.UV2 += float4(baseUV, baseUV);
    result.UV3 += float4(baseUV, baseUV);
    result.UV4 += float4(baseUV, baseUV);
    result.UV5 += float4(baseUV, baseUV);
    result.UV6 += float4(baseUV, baseUV);
    result.UV7 += float4(baseUV, baseUV);
    return result;
}

float4 PSBlur(const PSBlurInput input) : SV_Target0
{
    float4 color;
    
    color  = Weights[0].x * inputImage.Sample(SamplerLinearWrap, input.UV0.xy);
    color += Weights[0].y * inputImage.Sample(SamplerLinearWrap, input.UV1.xy);
    color += Weights[0].z * inputImage.Sample(SamplerLinearWrap, input.UV2.xy);
    color += Weights[0].w * inputImage.Sample(SamplerLinearWrap, input.UV3.xy);
    color += Weights[1].x * inputImage.Sample(SamplerLinearWrap, input.UV4.xy);
    color += Weights[1].y * inputImage.Sample(SamplerLinearWrap, input.UV5.xy);
    color += Weights[1].z * inputImage.Sample(SamplerLinearWrap, input.UV6.xy);
    color += Weights[1].w * inputImage.Sample(SamplerLinearWrap, input.UV7.xy);
    
    color += Weights[0].x * inputImage.Sample(SamplerLinearWrap, input.UV0.zw);
    color += Weights[0].y * inputImage.Sample(SamplerLinearWrap, input.UV1.zw);
    color += Weights[0].z * inputImage.Sample(SamplerLinearWrap, input.UV2.zw);
    color += Weights[0].w * inputImage.Sample(SamplerLinearWrap, input.UV3.zw);
    color += Weights[1].x * inputImage.Sample(SamplerLinearWrap, input.UV4.zw);
    color += Weights[1].y * inputImage.Sample(SamplerLinearWrap, input.UV5.zw);
    color += Weights[1].z * inputImage.Sample(SamplerLinearWrap, input.UV6.zw);
    color += Weights[1].w * inputImage.Sample(SamplerLinearWrap, input.UV7.zw);
    
    return color;
}

VSOutputVertex VSFinal(VSInputVertex vertexIn)
{
    VSOutputVertex result;
    result.Position = vertexIn.Position;
    result.UV       = vertexIn.UV;
    result.Normal   = vertexIn.Normal;
    result.Color    = vertexIn.Color;
    
    return result;
}

float4 PSFinal(VSOutputVertex input) : SV_Target0
{
    return float4(inputImage.Sample(SamplerLinearWrap, float2(input.UV.x, input.UV.y)));
}
#endif