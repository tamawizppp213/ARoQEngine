//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderGBuffer.hlsl
//            Content:  GBuffer
//             Author:  Toide Yutaro
//             Create:  2023_02_07
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_CASCADE_SHADOW_HLSL
#define SHADER_CASCADE_SHADOW_HLSL
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
cbuffer ShadowInfo : register(b3)
{
    float4x4 LVPC; // Light view projection crop matrix 
    bool     IsSoftShadow;
}

struct PSShadowMap
{
    float4 Position      : SV_Position; // pixel position in screen space.
    float3 Normal        : NORMAL;      // normal vector in the world space
    float2 UV            : TEXCOORD0;   // uv coordinate
};

struct PSShadowReceiver
{
    float4 Position      : SV_Position; // pixel position in screen space.
    float3 Normal        : NORMAL;      // normal vector in the world space
    float2 UV            : TEXCOORD0;   // uv coordinate
    float4 PositionInLVP[SHADOW_MAP_COUNT] : TEXCOORD1; // pixel position in the light view screen space.
};

Texture2D DiffuseMap                   : register(t0, space0); // Albedo map
Texture2D ShadowMap[SHADOW_MAP_COUNT]  : register(t0, space1); // Cascaede
SamplerState SamplerLinearWrap         : register(s0);

//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
float2 NormalizeShadowMapUV(const float4 posInLVP);
float  CalculateChebyshev  (const float2 moments, const float depth);

PSShadowReceiver VSReceiveMain(const VSInputSkinVertex vertexIn)
{
    PSShadowReceiver result;
    /*-------------------------------------------------------------------
	-        transform to world space 
	---------------------------------------------------------------------*/
    float4 positionWorld = mul(World, vertexIn.Position);
	result.Position      = mul(ProjectionView, positionWorld);
	result.Normal        = normalize(mul(World, vertexIn.Normal).xyz);
	result.UV            = vertexIn.UV;
    
    /*-------------------------------------------------------------------
	-        Calculate light view screen space
	---------------------------------------------------------------------*/
    for (int i = 0; i < SHADOW_MAP_COUNT; ++i)
    {
        result.PositionInLVP[i] = mul(LVPC[i], positionWorld);
    }
    
    return result;
}


float4 PSReceiveMain(const PSShadowReceiver input) : SV_Target
{
    float4 color = DiffuseMap.Sample(SamplerLinearWrap, input.UV);
    
    for (int i = 0; i < SHADOW_MAP_COUNT; ++i)
    {
        const float zInLVP = input.PositionInLVP[i].z / input.PositionInLVP[i].w;
        
        // In range depth z in the light view projection
        if (zInLVP <0.0f ||1.0f <  zInLVP ){continue;}

        /*-------------------------------------------------------------------
	    -   Determine if this pixel is included in this shadow map by looking at the Z value.
	    ---------------------------------------------------------------------*/
        const float2 shadowMapUV = NormalizeShadowMapUV(input.PositionInLVP[i]);
        
        // Check in range from 0.0f to 1.0f
        if (shadowMapUV.x < 0.0f || 1.0f < shadowMapUV.x){continue;}
        if (shadowMapUV.y < 0.0f || 1.0f < shadowMapUV.y){continue;}

        // Sampling from the shadow map
        const float2 shadow = ShadowMap[i].Sample(SamplerLinearWrap, shadowMapUV).xy;
        
        // Check the occluded the pixel
        if (zInLVP < shadow.r){continue;}
        
        // ‚Æ‚è‚ ‚¦‚¸. (Todo. soft shadow‚É‚·‚é)
        const float shadowColor = color.xyz * 0.5f;
        const float shadowRatio = CalculateChebyshev(shadow.xy, zInLVP);
        color.xyz = lerp(shadowColor, color.xyz, shadowRatio);
        break;
    }
    return color;
}

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
#endif