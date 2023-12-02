/////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderShadow.hlsl
//            Content:  Cascade shadow 
//             Author:  Toide Yutaro
//             Create:  2023_04_19
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_CASCADE_SHADOW_HLSLI
#define SHADER_CASCADE_SHADOW_HLSLI
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderVertexType.hlsli"
#include "../Core/ShaderConstantBuffer3D.hlsli"

//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////
#ifndef SHADOW_MAP_COUNT 
#define SHADOW_MAP_COUNT (3)
#endif 

// Set light view projection crop matrix
cbuffer ShadowInfo : register(b10)
{
    float4x4 LVPC[SHADOW_MAP_COUNT]; // Light view projection crop matrix 
    bool IsSoftShadow;
}

//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
float CalculateChebyshev(const float2 moments, const float depth)
{
    /*-------------------------------------------------------------------
	-   If the target pixel is occluded, use Chebysheb's inequality 
        to find the probability of the light hitting it.
	---------------------------------------------------------------------*/
    if(depth <= moments.x){return 0.0f;}
    
    // Calculate variance
    const float depth2   = moments.x * moments.x;
    const float variance = min(max(moments.y - depth2, 0.0001f), 1.0f);
    
    // Find the difference between the depth value from the light of this pixel and 
    // the average depth value of the shadow map
    const float md = depth - moments.x;
    
    // calculate the probability of the transferable light
    // The larger lit_factor is, the more likely it is to be illuminated.
    float litFactor = variance / (variance + md * md);
    
    // Below the lower limit of the probability of light reaching the shadow.
    const float minLitFactor = 0.3f; // 
    litFactor = saturate((litFactor - minLitFactor) / (1.0f - minLitFactor));
    
    const float shadowRatio = 1.0f - litFactor;
    return shadowRatio;
}

float2 NormalizeShadowMapUV(const float4 posInLVP)
{
    float2 uv = posInLVP.xy / posInLVP.w;
    uv *= float2(0.5f, -0.5f);
    uv += 0.5f;
    return uv;
}

float CalculateShadowRate(
const float2 shadowPixel  [SHADOW_MAP_COUNT],
const float4 positionInLVP[SHADOW_MAP_COUNT])
{
    float shadowRatio = 0.0f;
    
    for (int i = 0; i < SHADOW_MAP_COUNT; ++i)
    {
        const float zInLVP = positionInLVP[i].z / positionInLVP[i].w;
        
        // In range depth z in the light view projection
        if (zInLVP <0.0f ||1.0f <  zInLVP ){continue;}

        /*-------------------------------------------------------------------
	    -   Determine if this pixel is included in this shadow map by looking at the Z value.
	    ---------------------------------------------------------------------*/
        const float2 shadowMapUV = NormalizeShadowMapUV(positionInLVP[i]);
        
        // Check in range from 0.0f to 1.0f
        if (shadowMapUV.x < 0.0f || 1.0f < shadowMapUV.x){continue;}
        if (shadowMapUV.y < 0.0f || 1.0f < shadowMapUV.y){continue;}

        // Sampling from the shadow map
        const float2 shadow = shadowPixel[i];
        
        // Check the occluded the pixel
        if (zInLVP < shadow.r){continue;}
        
        // decide the shadow ratio
        if (IsSoftShadow)
        {
            shadowRatio = CalculateChebyshev(shadow.xy, zInLVP);
        }
        else
        {
            shadowRatio = 1.0f;
        }
        break;
    }
    return shadowRatio;
}
#endif