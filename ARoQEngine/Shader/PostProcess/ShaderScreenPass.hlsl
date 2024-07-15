//////////////////////////////////////////////////////////////////////////////////
///  @file   ShaderScreenPass.hlsl
///  @brief  �l�p�`�̃X�N���[����\�����鋤�ʃV�F�[�_�[
///  @author toide
///  @date   2024/07/15 1:52:22
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_SCREEN_PASS_HLSL
#define SHADER_SCREEN_PASS_HLSL

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
struct VSInputPositionTexture
{
    float2 Position : POSITION;
    float2 UV       : TEXCOORD0;
};

struct PSInputPositionTexture
{
    float4 Position : SV_POSITION;
    float2 UV       : TEXCOORD0;
};

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////

/*!**********************************************************************
*  @brief     ���_�V�F�[�_�����s
*  @param[in] const VSInput input
*  @return    float4
*************************************************************************/
PSInputPositionTexture MainVS(VSInputPositionTexture input)
{
    // �ŏ�����NDC���W�n�ő�����邱�Ƃ�z�肵�Ă��܂�. ���̂��߂��̂܂ܕԂ��܂�
    PSInputPositionTexture output = (PSInputPositionTexture) 0;
    output.Position = float4(input.Position.xy, 0.0f, 1.0f);
    output.UV       = input.UV;
    return output;
}

#endif 
