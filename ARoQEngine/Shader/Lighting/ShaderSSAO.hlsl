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
Texture2D<float4>   NormalTexture    : register(t0);
Texture2D<float>    DepthTexture     : register(t1);
Texture2D<float4>   BlueNoiseTexture : register(t2);
Texture2D<float4>   InputTexture     : register(t3);
RWTexture2D<float4> OutputTexture    : register(u0);

// Sampler
SamplerState SamplerPointClamp : register(s1);
SamplerState SamplerDepth      : register(s2);

// Variables
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
****************************************************************************/
/* @fn        float NDCDepthToViewDepth(const float z_ndc)

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
****************************************************************************/
/* @fn        float CalculateOcclusion(const float distZ)

*  @brief     Return calculate occlusion ratio from 0.0f to 1.0f

*  @param[in] const float distZ

*  @return    float
*****************************************************************************/
float CalculateOcclusion(const float distanceViewZ)
{
    // if (dist <= surface epsilon, don't occlude the object)
    if (distanceViewZ <= SurfaceEpsilon){ return 0.0f; }
    
    const float fadeLength = FadeEnd - FadeStart;
    const float occlusion  = saturate((FadeEnd - distanceViewZ) / fadeLength);
    return occlusion;
}

float4 GetBlurWeightedPixelColor(const float weight, const float2 uv)
{
    return weight * InputTexture.Sample(SamplerPointClamp, uv);
}

VertexOut VSMain( const VSInputVertex vertexIn)
{
    VertexOut result;
    
    // �����ł�, �����`���b�V����Ώۂɂ��Ă��܂�. 
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
    // �[�x�}�b�v����NDC��Ԃɂ����邱�̃s�N�Z����z���W���擾����
    // �[�x�}�b�v�̒�`��zPrefass.hlsl���Q�Ƃ��Ă�������.
    const float ndcDepth  = DepthTexture.Sample(SamplerDepth, input.UV).r; // LOD���g��Ȃ����Ƃ�������Ȃ�SampleLevel
    
    // �r���[��Ԃɂ�����[�x (�J���� -> ���_�܂ł̐[�x)
    const float viewSurfaceDepth = NDCDepthToViewDepth(ndcDepth);
    
    // �[�x�̒l�����Ƃ�, View��Ԃɂ�����T���v�����O�ʒu���č\������
    const float  distanceRatio       = (viewSurfaceDepth / input.NearPlaneViewPosition.z); // 
    const float3 viewSurfacePosition = distanceRatio * input.NearPlaneViewPosition; // NearPlane�͒P�ʃx�N�g���I�Ȉ���.
    
    // Acquire world normal: the normal value range is converted 0�`1 into -1 �` 1.
    // �@���}�b�v��0�`1����, -1�`1�ɕϊ�����. �@���̓��[���h��Ԃ̂��̂��g�p����. 
    const float3 normal = (NormalTexture.Sample(SamplerPointClamp, input.UV).xyz * 2) - 1;
    
    // Blue noise�������_���l�ɂ͎g�p��, �Ȃ�ׂ��ϓ��ɃT���v�����O�����悤�ɂ���. �܂�, �͈͂�0�`1����-1�`1�ɕϊ�����
    const float3 randomVector = normalize(2.0f * BlueNoiseTexture.Sample(SamplerLinearWrap, 4.0f * input.UV, 0).xyz - 1);
    
    float occlusionSum = 0.0f;
    float division     = 0.0f;
    
    /*-------------------------------------------------------------------
	-        Execute SSAO
	---------------------------------------------------------------------*/
    // �@�������ɔ�����ɕ��˂���ߐړ_���T���v�����O
    for(int i = 0; i < SAMPLE_COUNT; ++i)
    {
        // offset Vector�͌Œ肳��, ��l�ɕ��U���Ă���.(offset vector���ł܂�Ȃ��悤�ɂ��鏈�u�ł�. )
        // offset vector�������_���ȃx�N�g���ɑ΂��ēK�p�����, �I�t�Z�b�g�x�N�g���̃����_���Ȉ�l���z�������܂�. 
        const float3 randomOffset = reflect(OffsetVectors[i].xyz, randomVector);
        
        // offset vector��(p, n)�Œ�`����镽�ʂ̌��ɂ���ꍇ�A����𔽓]������B
        const float  flip   = sign(dot(randomOffset, normal));
        
        // �����_���ȃT���v�����O�ʒu��, Occlude radius���Ɏ擾�ł���悤�ɂ���. 
        const float3 randomSamplePosition = Radius * flip * randomOffset + viewSurfacePosition.xyz;
        
        // Project random sample position and generate projective tex-coords.
        const float4 projectionRandomSample = mul(float4(randomSamplePosition, 1.0f), input.ProjectionTexture) / projectionRandomSample.w;
        
        // �ڂ��烉���_���ȃT���v���ʒu�܂ł̃��C�ɉ�����, �ł��߂��[�x�l��������
        // ����̓����_���ȃT���v���ʒu�ɂ��Ă̐[�x�l�ł͂Ȃ����Ƃ͒��ӂ��Ă���
        const float ndcRandomSampleDepth = DepthTexture.Sample(SamplerDepth, projectionRandomSample.xy);
        const float viewSampleDepth      = NDCDepthToViewDepth(ndcRandomSampleDepth);
        
        // ���s���̂���_�̃r���[��Ԃ̈ʒu���č\�z����B
        const float3 viewRandomSamplePosition = (viewSampleDepth / randomSamplePosition.z) * randomSamplePosition;
        
        // Random Sample�ʒu��surface position���Օ����邩�ǂ���
        // dot(normal, normalize(viewRandomSamplePosition - viewSurfacePosition))��, occluder point���ǂꂾ��plane�̐��ʂɂ��邩�𑪒�
        // ���ʂɂȂ�ق�, �Օ��\������������, �Օ��d�݂�ǉ�����. 
        const float distanceViewZ = viewSurfacePosition.z - viewRandomSamplePosition.z; 
        const float dp            = max(dot(normal, normalize(viewRandomSamplePosition - viewSurfacePosition)), 0.0f);
        const float occlusion = CalculateOcclusion(distanceViewZ) * dp;
        occlusionSum += occlusion;
    }
    
    // ����
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
