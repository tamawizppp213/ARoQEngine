//////////////////////////////////////////////////////////////////////////////////
//             Title:  ShaderSSAO.hlsl
//             Content:  ssao (compute shader)
//             Author:  Reference : Frack D Luna
//             Create:  2023_04_01
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_SSAO_HPP
#define SHADER_SSAO_HPP
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderConstantBuffer3D.hlsli"
#include "../Core/ShaderVertexType.hlsli"

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
//       1.0     -------------\
//               |           |  \
//               |           |    \
//               |           |      \
//               |           |        \
//               |           |          \
//               |           |            \
//  ------|------|-----------|-------------|---------|--> zv
//        0     Eps          z0            z1        
//
#ifndef SAMPLE_COUNT
    #define SAMPLE_COUNT (14)
#endif

cbuffer SSAOSetting : register(b3)
{
    float2 TextureSize;
    float  Radius;         // view space
    float  Sharpness;
    
    float  FadeStart;      // view space
    float  FadeEnd;        // view space
    float  SurfaceEpsilon; // view space
    int    PaddingSSAO;
    
    float4 OffsetVectors[SAMPLE_COUNT];
}

cbuffer BlurMode : register(b4)
{
    float4 Weights[2];
    float  BlurRadius;
    bool   IsHorizontal; // Horizontal or vertical blur.
}

// Texture
Texture2D<float4>   NormalTexture   : register(t0);
Texture2D<float>    DepthTexture    : register(t1);
Texture2D<float4>   RandomTexture   : register(t2);
Texture2D<float4>   InputTexture    : register(t3);
RWTexture2D<float4> OutputTexture   : register(u0);

// Sampler
SamplerState SamplerDepth      : register(s0);
SamplerState SamplerPointClamp : register(s1);
SamplerState SamplerLinearWrap : register(s2);

// Variables
static const float2 RectUVs[6] =
{
    float2(0.0f, 1.0f),
    float2(0.0f, 0.0f),
    float2(1.0f, 0.0f),
    float2(0.0f, 1.0f),
    float2(1.0f, 0.0f),
    float2(1.0f, 1.0f)
};

struct VertexOut
{
    float4   Position          : SV_Position; // NDC space
    float3   NearPlaneViewPosition : POSITION;     
    float2   UV                    : TEXCOORD0;
    float4x4 ProjectionTexture : PROJECTION;
};


//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			NDCDepthToViewDepth
*************************************************************************//**
*  @fn        float NDCDepthToViewDepth(const float z_ndc)

*  @brief     Convert ndc Depth to view space depth

*  @param[in] const float z depth in ndc space.

*  @return    float
*****************************************************************************/
float NDCDepthToViewDepth(const float z_ndc)
{
    // invert the calculation from NDC space to view space for the z-zoordinate. 
    // We have that z_ndc = A + B / viewZ, where gProj[2,2] = A and gProj[3,2] = B. 
    return Projection[3][2] / (z_ndc - Projection[2][2]);
}

/****************************************************************************
*				  			CalculateOcclusion
*************************************************************************//**
*  @fn        float CalculateOcclusion(const float distZ)

*  @brief     Return calculate occlusion ratio from 0.0f to 1.0f

*  @param[in] const float distZ

*  @return    float
*****************************************************************************/
float CalculateOcclusion(const float distZ)
{
    // if (dist <= surface epsilon, don't occlude the object)
    if (distZ <= SurfaceEpsilon){ return 0.0f; }
    
    const float fadeLength = FadeEnd - FadeStart;
    const float occlusion  = saturate((FadeEnd - distZ) / fadeLength);
    return occlusion;
}

float4 GetBlurWeightedPixelColor(const float weight, const float2 uv)
{
    return weight * InputTexture.Sample(SamplerPointClamp, uv);
}

VertexOut VSMain( const VSInputVertex vertexIn)
{
    VertexOut result;
    
    // based on the vertex id, you select the rect UV value. 
    // vertex data must be set rectangle polygon. 
    result.UV = vertexIn.UV;
    
    // Rectangle covering screen in NDC space
    result.Position = vertexIn.Position;
    
    // Transform rectangle corners to view space near plane
    const float4 position = mul(InverseProjection, vertexIn.Position);
    result.NearPlaneViewPosition   = position.xyz / position.w;

    float4x4 T =
    {
        0.5f, 0.0f, 0.0f, 0.5f,
        0.0f, -0.5f, 0.0f, 0.5f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    
    // Rect uv value in projection space.
    result.ProjectionTexture = mul(T, Projection);
    return result;
}

float4 ExecuteSSAO(VertexOut input) : SV_Target
{
    /*-------------------------------------------------------------------
	-        Get ssao variables
	---------------------------------------------------------------------*/
    // Acquire z-coord of this pixel in NDC space from depth map
    const float ndcDepth  = DepthTexture.Sample(SamplerDepth, input.UV).r; // LOD‚ðŽg‚í‚È‚¢‚±‚Æ‚ª•ª‚©‚é‚È‚çSampleLevel
    const float viewDepth = NDCDepthToViewDepth(ndcDepth);
    
    // Reconstruct the view space position of the point with depth pz.
    const float  distanceRatio = (viewDepth / input.NearPlaneViewPosition.z);
    const float3 viewPosition  = distanceRatio * input.NearPlaneViewPosition; // NearPlane‚Í’PˆÊƒxƒNƒgƒ‹“I‚Èˆµ‚¢.
    
    // Acquire world normal: the normal value range is converted 0`1 into -1 ` 1.
    const float3 normal = normalize((NormalTexture.Sample(SamplerPointClamp, input.UV).xyz * 2) - 1);
    
    // Acquire random value : the random value range is converted 0`1 into |1 ` 1.
    const float3 random = normalize(2.0f * RandomTexture.Sample(SamplerLinearWrap, 4.0f * input.UV, 0).xyz - 1);
    
    float occlusionSum = 0.0f;
    float division     = 0.0f;
    
    /*-------------------------------------------------------------------
	-        Execute SSAO
	---------------------------------------------------------------------*/
    for(int i = 0; i < SAMPLE_COUNT; ++i)
    {
        // Offset vectors are fixed and uniformly distributed.
        // If we reflect them about a random vector then we get a random uniform distribution of offset vectors.
        const float3 offset = reflect(OffsetVectors[i].xyz, random);
        
        // Flip offset vector if it is behind the plane defined by (p, n).
        const float  flip   = sign(dot(offset, normal));
        
        // random sample position must not have been occluded already.
        const float3 randomSamplePosition = random * Radius * flip + viewPosition.xyz;
        
        // Project random sample position and generate projective tex-coords.
        const float4 projectionSample = mul(float4(randomSamplePosition, 1.0f), input.ProjectionTexture) / projectionSample.w;
        
        // Find the nearest depth value along the ray from the eye to random sample position.
        const float ndcSampleDepth = DepthTexture.Sample(SamplerDepth, projectionSample.xy);
        
        const float viewSampleDepth = NDCDepthToViewDepth(ndcSampleDepth);
        
        // Reconstruct the view space position of the point with depth.
        const float3 viewSamplePosition = (viewSampleDepth / randomSamplePosition.z) * randomSamplePosition;
        
        // Test whether random sample position occludes position.
        const float distZ = viewPosition.z - viewSamplePosition.z;
        const float dp    = max(dot(normal, normalize(viewSamplePosition - viewPosition)), 0.0f);
        
        const float occlusion = dp * CalculateOcclusion(distZ);
        occlusionSum += occlusion;
    }
    occlusionSum /= (float) (SAMPLE_COUNT);
    
    const float4 result = saturate(pow(1.0f - occlusionSum, Sharpness));
    return result;
}

float4 ExecuteSSAOBlur(const VertexOut input) : SV_Target
{
    /*-------------------------------------------------------------------
	-        Prepare weight setting
	---------------------------------------------------------------------*/
    const int arraySize = 8;
    
    // prepare the one dimensional list from float4[2] data to use the loop.
    const float blurWeights[arraySize] =
    {
        Weights[0].x, Weights[0].y, Weights[0].z, Weights[0].w,
        Weights[1].x, Weights[1].y, Weights[1].z, Weights[1].w,
    };
    
    // UV offset based on the one pixel size. This value is set in horizontal or vertical direction.
    float2 uvOffset;
    if(IsHorizontal){uvOffset = float2(InverseRenderTargetSize.x, 0.0f);}
    else            {uvOffset = float2(0.0f, InverseRenderTargetSize.y);}
    
    // center position normal and depth 
    const float3 centerNormal = NormalTexture.Sample(SamplerPointClamp, input.UV).xyz;
    const float  centerDepth  = DepthTexture .Sample(SamplerDepth, input.UV);
    
    /*-------------------------------------------------------------------
	-        Calculate total weight
	---------------------------------------------------------------------*/
    float4 color       = GetBlurWeightedPixelColor(blurWeights[0], input.UV); // center color
    float  totalWeight = blurWeights[0];
    
    for (int i = 1; i < arraySize; ++i)
    {
        // plus : right or up direction, minus : left or down direction to the center uv position.
        const int moveDirection[2] = { +1, -1 };
        
        [unroll]
        for (int j = 0; j < 2; ++j)
        {
            const float2 uv             = input.UV + (float)moveDirection[j] * uvOffset * (float)i;
            const float3 neighborNormal = NormalTexture.Sample(SamplerPointClamp, uv).xyz;
            const float  neighborDepth  = DepthTexture .Sample(SamplerDepth, uv);
            
            // if the cventer value and neighbor values differ too much (either depth or normal), 
            // then we assume we are sampling across a discontinuity. 
            // We discard such samples from the blur.
            if (dot(neighborNormal, centerNormal) < 0.8f)  {continue;}
            if (abs(neighborDepth - centerDepth)  > 0.2f)  {continue;}

            const float weight = blurWeights[i];
            
            // Add neighbor pixel to blur
            color       += GetBlurWeightedPixelColor(weight, uv);
            totalWeight += weight;
        }
    }
    
    return color / totalWeight;
}

#endif
