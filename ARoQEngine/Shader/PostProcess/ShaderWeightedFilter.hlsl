//////////////////////////////////////////////////////////////////////////////////
///  @file   ShaderWeightedFilter.hlsl
///  @brief  ���͂̃s�N�Z���̐F���d�ݕt�����ăt�B���^�����O����V�F�[�_ @n
///          ��̓I�ɂ�, �K�E�V�A���u���[�̏����ȂǂɎg�p���܂�. Weight��CPU���Ő��K�����Ă����K�v������܂�. @n
///  @note   �g�p�\�}�N�� @n
///          - WEIGHT_SAMPLE_COUNT(Num   ) : �d�݃T���v�������w�肵�܂�. �f�t�H���g��9�ł�. @n
///          - USE_UNPACKED_UV    (Switch) : UV�l�𔼕��ɂ��邱�ƂŃ��[�v�񐔂����炷���Ƃ��ł��܂�.
///          - USE_COMPUTE_SHADER (Switch) : �R���s���[�g�V�F�[�_�[���g�p���܂�. �g�p����ꍇ��THREADGROUP_SIZE_X, Y���`���Ă�������@n
///  @author toide
///  @date   2024/07/15 20:00:26
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_WEIGHTED_FILTER_HLSL
#define SHADER_WEIGHTED_FILTER_HLSL

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderVertexType.hlsli"

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
Texture2D    SceneTexture      : register(t0);
SamplerState Sampler           : register(s0);

// �f�t�H���g�̏d�݃T���v����
#ifndef WEIGHT_SAMPLE_COUNT
    #define WEIGHT_SAMPLE_COUNT (9)
#endif

// ���[�v�񐔂����炷�̂ɖ𗧂��܂�. float4��xy, zw�ɂ��ꂼ��ʂ�UVOffset�l���i�[���邽��, �����ɂȂ�܂�.
#ifdef USE_UNPACKED_UV
    #define PACKED_SAMPLE_COUNT WEIGHT_SAMPLE_COUNT
#else
    #define PACKED_SAMPLE_COUNT ((WEIGHT_SAMPLE_COUNT + 1) / 2)
#endif

/****************************************************************************
*				  		SceneTextureInfo
****************************************************************************/
/* @brief  �V�[���e�N�X�`�����g�p������
*****************************************************************************/
cbuffer SceneTextureInfo : register(b0)
{
    float2 SceneTextureInvertSize; // �V�[���e�N�X�`���̋t�T�C�Y
    uint2  ViewPortMax;            // �r���[�|�[�g�̍ő�s�N�Z���l
    uint2  ViewPortMin;            // �r���[�|�[�g�̍ŏ��s�N�Z���l
};

/****************************************************************************
*				  		WeightTable
****************************************************************************/
/* @brief  ���͂̃s�N�Z���̏d�݂��i�[����\����. �d�݂�CPU���Ő��K�����Ă����K�v������܂�.
*****************************************************************************/
cbuffer WeightTable : register(b1)
{
    float4 Weights[WEIGHT_SAMPLE_COUNT]; //!< �d�݃e�[�u��
    float4 Offsets[PACKED_SAMPLE_COUNT]; //!< �I�t�Z�b�g�e�[�u��
};

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////

/*!**********************************************************************
*  @brief     UV�l�ɂ��������ăe�N�X�`�����T���v�����O
*  @param[in] const float2 UV�l
*  @return    float4 �T���v�����O�����F
*************************************************************************/
float4 SampleFilterTexture(const float2 uv)
{
    return SceneTexture.SampleLevel(Sampler, uv, 0);
}

/*-------------------------------------------------------------------
-       Compute Shader
---------------------------------------------------------------------*/
#ifdef USE_COMPUTE_SHADER
RWTexture2D<float4> OutputTexture : register(u0);

// �f�t�H���g�l�̐ݒ�
#ifndef THREADGROUP_SIZE_X
    #define THREADGROUP_SIZE_X 8
#endif // THREADGROUP_SIZE_X
#ifndef THREADGROUP_SIZE_Y
    #define THREADGROUP_SIZE_Y 8
#endif // THREADGROUP_SIZE_Y

/*!**********************************************************************
*  @brief     �R���s���[�g�V�F�[�_�[�̎��s
*  @param[in] const uint2 dispatchThreadID
*  @return    void
*************************************************************************/
[numthreads(THREADGROUP_SIZE_X, THREADGROUP_SIZE_Y, 1)]
void MainCS(uint2 dispatchThreadID : SV_DispatchThreadID)
{
    // �T���v�����O����UV�l���擾
    const uint2  pixelPosition = dispatchThreadID + ViewPortMin;
    const float2 baseUV        = (float2(pixelPosition) + 0.5f) * SceneTextureInvertSize;
    
    // Compute Shader�ɂ�����, �s�N�Z���̗̈�O�ɃA�N�Z�X���邱�Ƃ͂ł��Ȃ�����, �������Ȃ�
    const float2 centerUVDistance = abs(baseUV - 0.5f);
    if (max(centerUVDistance.x, centerUVDistance.y) >= 0.5f) // UV�l��0.0f ~ 1.0f�͈̔͂Ɏ��܂��Ă��邩
    {
        return;
    }
    
    float4 result      = float4(0, 0, 0, 0);
    int    sampleIndex = 0;
    // �אڃs�N�Z�����d�݂��Â��Ȃ�����Z����. (���ω����s���Ă��Ȃ�����, �K��CPU���ŗאڃs�N�Z����WeightTable�̘a�𐳋K�����Ă����Ă�������)
#ifdef USE_UNPACKED_UV 
    [Unroll]
    for (; sampleIndex < WEIGHT_SAMPLE_COUNT; ++sampleIndex)
    {
        result += SampleFilterTexture(Offsets[sampleIndex]) * Weights[sampleIndex];
    }
#else
    [Unroll]
    for (; sampleIndex < WEIGHT_SAMPLE_COUNT - 1; sampleIndex += 2)
    {
        float4 offset = baseUV.xyxy + Offsets[sampleIndex / 2];
        result += SampleFilterTexture(offset.xy) * Weights[sampleIndex + 0];
        result += SampleFilterTexture(offset.zw) * Weights[sampleIndex + 1];
    }
    
    [FLATTEN]  
    if (sampleIndex < WEIGHT_SAMPLE_COUNT)
    {
        const float2 uv = baseUV.xy + Offsets[sampleIndex / 2].xy;
        result += SampleFilterTexture(uv) * Weights[sampleIndex + 0];
    }
#endif // USE_UNPACKED_UV
    
    OutputTexture[dispatchThreadID.xy] = result;
}
/*-------------------------------------------------------------------
-       Pixel Shader
---------------------------------------------------------------------*/
#else // USE_COMPUTE_SHADER

/****************************************************************************
*				  		PSInput
****************************************************************************/
/* @brief  �s�N�Z���V�F�[�_�[�ɓ��͂���ۂɎg�p����\����. �אڃs�N�Z���̏�񂪓����Ă��܂�.
*****************************************************************************/
struct PSInput
{
    /*! @brief  �s�N�Z���̈ʒu */
    float4 Position : SV_POSITION;
    
    /*! @brief  ���S��UV�l */
    float2 UV : TEXCOORD;
    
    /*! @brief  �אڃs�N�Z����UV�l (���S����̍������擾���Ă���킯�ł͂���܂���)*/
#if USE_UNPACKED_UV
    float2 Offsets[WEIGHT_SAMPLE_COUNT]  : TEXCOORD1;
#else
    float4 Offsets[PACKED_SAMPLE_COUNT] : TEXCOORD1;
#endif
};

/*!**********************************************************************
*  @brief     ���_�V�F�[�_�����s
*  @param[in] void
*  @return    PSInputPositionTexture
*************************************************************************/
PSInput MainVS(VSInputPositionTexture input)
{
    PSInput output = (PSInput)0;
    output.Position = float4(input.Position, 0.0f, 1.0f);
    output.UV       = input.UV;
    
#ifdef USE_UNPACKED_UV
    [Unroll]
    for (int i = 0; i < WEIGHT_SAMPLE_COUNT; ++i)
    {
        output.Offsets[i] = input.UV + Offsets[i];
    }
#else
    [Unroll]
    for (int i = 0; i < PACKED_SAMPLE_COUNT; ++i)
    {
        output.Offsets[i] = input.UV.xyxy + Offsets[i];
    }
#endif
    
    return output;
}

/*!**********************************************************************
*  @brief     �s�N�Z���V�F�[�_�����s
*  @param[in] const PSInput input
*  @return    float4
*************************************************************************/
float4 MainPS(const PSInput input) : SV_TARGET
{
    // �T���v�����O����UV�l���擾 (�ڂ��������邱�Ƃ�DownSample�̕i�������サ�Ă��܂�)
    float4 result = float4(0, 0, 0, 0); 
    
    // �אڃs�N�Z�����d�݂��Â��Ȃ�����Z����. (���ω����s���Ă��Ȃ�����, �K��CPU���ŗאڃs�N�Z����WeightTable�̘a�𐳋K�����Ă����Ă�������)
#ifdef USE_UNPACKED_UV 
    [Unroll]
    for (int i = 0; i < WEIGHT_SAMPLE_COUNT; ++i)
    {
        result += SampleFilterTexture(input.Offsets[i]) * Weights[i];
    }
#else // USE_UNPACKED_UV
    [Unroll]
    for (int i = 0; i < WEIGHT_SAMPLE_COUNT - 1; i += 2)
    {
        float4 offset = input.Offsets[i / 2];
        result += SampleFilterTexture(offset.xy) * Weights[i + 0];
        result += SampleFilterTexture(offset.zw) * Weights[i + 1];
    }
    
    // �d�݃T���v��������̏ꍇ, �Ō�̃s�N�Z�����T���v�����O (�ŉ��ʃr�b�g���擾���邱�ƂŊ������ł���)
    [FLATTEN] // �قƂ�ǂ��̏����ɓ���܂�. 
    if (WEIGHT_SAMPLE_COUNT & 1) 
    {
        result += SampleFilterTexture(input.Offsets[PACKED_SAMPLE_COUNT - 1].xy) * Weights[WEIGHT_SAMPLE_COUNT - 1];
    }
#endif // USE_UNPACKED_UV
    
    return result;
} 

#endif // USE_COMPUTE_SHADER

#endif 
