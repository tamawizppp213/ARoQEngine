//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderGaussianBlur.hlsl
//            Content:  Draw Blur Effect (compute shader)
//             Author:  Toide Yutaro
//             Create:  2022_05_01
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_BLUR_HPP
#define SHADER_BLUR_HPP
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
static const uint Thread = 16;

cbuffer WeightTable : register(b0)
{
    float4 Weights[2];
}
cbuffer TextureSize : register(b1)
{
    uint2 OriginalTexture;
    uint2 XBlurTexture;
    uint2 YBlurTexture;
}
Texture2D           inputImage  : register(t0);
RWTexture2D<float4> outputImage : register(u0);
RWTexture2D<float4> xHalfImage  : register(u1); // 1 / 2
RWTexture2D<float4> xyHalfImage : register(u2); // 1 / 4
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
*  @param[in] int2 textureSize
*****************************************************************************/
float4 GetPixelColor(int x, int y)
{
    return inputImage[int2(x, y)];
}
/****************************************************************************
*				  			GetPixelIndexFromXYCoordinate
*************************************************************************//**
*  @fn        uint GetPixelIndexFromXYCoordinate(int x, int y, int width)
*  @brief     Get Pixel index from xy coordinate
*  @param[in] int x
*  @param[in] int y
*  @param[in] int width
*****************************************************************************/
uint GetPixelIndexFromXYCoordinate(int x, int y, int width)
{
    return width * y + x;
} 
[numthreads(Thread, Thread, 1)]
void XBlur(uint3 id : SV_DispatchThreadID)
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
    
    outputImage[id.xy] = color;
    //outputImage[pixelIndex] = ConvertFloat4ToRGBA32(color);
}

[numthreads(Thread, Thread, 1)]
void YBlur(uint3 id : SV_DispatchThreadID)
{
    /*-------------------------------------------------------------------
    -         Y Blur
    ---------------------------------------------------------------------*/
    uint2 basePosition = uint2(id.x, id.y * 2);
    float4 color = GetPixelColor(basePosition.x, basePosition.y) * Weights[0].x;
    color += GetPixelColor(basePosition.x, basePosition.y + 1) * Weights[0].y;
    color += GetPixelColor(basePosition.x, basePosition.y + 2) * Weights[0].z;
    color += GetPixelColor(basePosition.x, basePosition.y + 3) * Weights[0].w;
    color += GetPixelColor(basePosition.x, basePosition.y + 4) * Weights[1].x;
    color += GetPixelColor(basePosition.x, basePosition.y + 5) * Weights[1].y;
    color += GetPixelColor(basePosition.x, basePosition.y + 6) * Weights[1].z;
    color += GetPixelColor(basePosition.x, basePosition.y + 7) * Weights[1].w;
    
    color += GetPixelColor(basePosition.x, basePosition.y - 1) * Weights[0].y;
    color += GetPixelColor(basePosition.x, basePosition.y - 2) * Weights[0].z;
    color += GetPixelColor(basePosition.x, basePosition.y - 3) * Weights[0].w;
    color += GetPixelColor(basePosition.x, basePosition.y - 4) * Weights[1].x;
    color += GetPixelColor(basePosition.x, basePosition.y - 5) * Weights[1].y;
    color += GetPixelColor(basePosition.x, basePosition.y - 6) * Weights[1].z;
    color += GetPixelColor(basePosition.x, basePosition.y - 7) * Weights[1].w;
    
    outputImage[id.xy] = color;
}


[numthreads(Thread, Thread, 1)]
void FinalBlur(uint3 id : SV_DispatchThreadID)
{
    uint2 basePosition = uint2(id.x / 2, id.y / 2);
    
    // bilinear filter
    float4 color = GetPixelColor(basePosition.x, basePosition.y);
    color += GetPixelColor(basePosition.x, basePosition.y + 1);
    color += GetPixelColor(basePosition.x + 1, basePosition.y);
    color += GetPixelColor(basePosition.x + 1, basePosition.y + 1);
    color /= 4.0f;
    
    outputImage[id.xy] = color;
}

[numthreads(Thread, Thread, 1)]
void Blur(uint3 id : SV_DispatchThreadID)
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
#endif