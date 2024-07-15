//////////////////////////////////////////////////////////////////////////////////
///  @file   ShaderDownSample.cpp
///  @brief  �`��V�[�����c�������ɏk������V�F�[�_. �ڂ��������邱�Ƃŕi�������サ�Ă��܂�
///  @author toide
///  @date   2024/07/06 14:20:34
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_DOWN_SAMPLE_HLSL
#define SHADER_DOWN_SAMPLE_HLSL

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
Texture2D    SceneTexture      : register(t0);
SamplerState SamplerLinearWrap : register(s0);

/****************************************************************************
*				  			SceneTextureInfo
****************************************************************************/
/* @brief �V�[���e�N�X�`�����g�p������
*****************************************************************************/
cbuffer SceneTextureInfo : register(b0)
{
    float2 SceneTextureInvertSize; // �V�[���e�N�X�`���̋t�T�C�Y
    uint2  ViewPortMax;            // �r���[�|�[�g�̍ő�s�N�Z���l 
    uint2  ViewPortMin;            // �r���[�|�[�g�̍ŏ��s�N�Z���l 
};

struct PSInput
{
    float4 Position : SV_POSITION;
    float2 UV       : TEXCOORD;
};

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
/*!**********************************************************************
*  @brief     UV�l�ɂ��������ăe�N�X�`�����T���v�����O
*  @param[in] const float2 UV�l
*  @return    float4 �T���v�����O�����F
*************************************************************************/
float4 SampleTexture(const float2 uv)
{
    return SceneTexture.SampleLevel(SamplerLinearWrap, saturate(uv), 0);
}

/*!**********************************************************************
*  @brief     Pixel Shader�ł�Compute Shader�ł����s����_�E���T���v�����O�֐�
*  @param[in] const float2 UV�l
*  @return    float4 �T���v�����O�����F
*************************************************************************/
float4 ExecuteDownSample(const float2 uv) : SV_TARGET
{
    // �T���v�����O����UV�l���擾 (�ڂ��������邱�Ƃ�DownSample�̕i�������サ�Ă��܂�)
    float2 UVs[4];
    UVs[0] = uv + SceneTextureInvertSize * float2(-1, 1);
    UVs[1] = uv + SceneTextureInvertSize * float2(1, 1);
    UVs[2] = uv + SceneTextureInvertSize * float2(-1, -1);
    UVs[3] = uv + SceneTextureInvertSize * float2(1, -1);
    
    // �ڂ����ɂ����͂̃e�N�Z�����擾
    float4 Sample[4];
    
    [UNROLL]
    for (int i = 0; i < 4; ++i)
    {
        Sample[i] = SampleTexture(UVs[i]);
    }
    
    // �ڂ����ɂ����͂̃e�N�Z�������v���ĕ��ς��擾
    float4 result = (Sample[0] + Sample[1] + Sample[2] + Sample[3]) * 0.25f;
    
    // UE�ɂ���, �܂�ɉ��F�ɂȂ邱�Ƃ�����炵���̂�, Clamp�̂悤�Ȉ���
    result.rgb = max(float3(0,0,0), result.rgb);

    return result;
}

#ifdef USE_COMPUTE_SHADER
RWTexture2D<float4> OutputTexture : register(u0);

// �f�t�H���g�l�̐ݒ�
#ifndef THREADGROUP_SIZE_X
    #define THREADGROUP_SIZE_X 8
#endif
#ifndef THREADGROUP_SIZE_Y
    #define THREADGROUP_SIZE_Y 8
#endif

/*!**********************************************************************
*  @brief     �R���s���[�g�V�F�[�_�[�̎��s
*  @param[in] const uint2 dispatchThreadID
*  @return    void
*************************************************************************/
[numthreads(THREADGROUP_SIZE_X, THREADGROUP_SIZE_Y, 1)]
void MainCS(uint2 dispatchThreadID : SV_DispatchThreadID)
{
    const uint2  pixelPosition = dispatchThreadID + ViewPortMin;
    const float2 uv            = (float2)dispatchThreadID * SceneTextureInvertSize;
    
    if (all(pixelPosition < ViewPortMax))
    {
        OutputTexture[pixelPosition] = ExecuteDownSample(uv);
    }
}
#else // PixelShader
/*!**********************************************************************
*  @brief     �s�N�Z���V�F�[�_�����s
*  @param[in] const PSInput input
*  @return    float4
*************************************************************************/
float4 MainPS(const PSInput input) : SV_TARGET
{
    return ExecuteDownSample(input.UV);
}
#endif // USE_PIXEL_SHADER


#endif