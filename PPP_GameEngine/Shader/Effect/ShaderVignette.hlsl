//////////////////////////////////////////////////////+-
+////////////////////////////
//              Title:  ShaderVignette.hlsl
//            Content:  �r�l�b�g����. ��ʒ[���Â�����|�X�g�G�t�F�N�g�ł�
//             Author:  Toide Yutaro
//             Create:  2023_10_06
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_VIGNETTE_HLSL
#define SHADER_VIGNETTE_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderVertexType.hlsli"
#define VertexIn  VSInputVertex
#define VertexOut VSOutputVertex
//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////
SamplerState SamplerLinearClamp : register(s0);
Texture2D    DiffuseMap         : register(t0);

cbuffer VignetteSettings : register(b0)
{
    float4 Color;     // �r�l�b�g�̐F
    float2 UVCenter;  // ���S���W (�r�l�b�g�͒ʏ�(0.5, 0.5))�̈ʒu����L����܂�.
    float  Intensity; // �r�l�b�g�̋���\, 0.0�`1.0�܂œK�p�ł��܂�
    float  Smoothness; // �r�l�b�g�̒[�������ǂꂾ���ڂ�����-
};
//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
VertexOut VSMain(VertexIn vertexIn)
{
    // ���̂܂�Pixel shader�ɓn��
    VertexOut result;
    result.Position = vertexIn.Position;
    result.UV       = vertexIn.UV;
    result.Normal   = vertexIn.Normal;
    result.Color    = vertexIn.Color;
    
    return result;
}

/****************************************************************************
*                       PSMain
*************************************************************************//**
*  @fn        float4 PSMain(VertexOut input) : SV_TARGET

*  @brief     Vignette post effect

*  @param[in] VertexOut input

*  @return �@�@float4
*****************************************************************************/
float4 PSMain(VertexOut input) : SV_TARGET
{
    const float4 diffuse = float4(DiffuseMap.Sample(SamplerLinearClamp, input.UV));
    
    // uv��uvCenter�Ԃ̋������擾����
    const float2 uvDistance = abs(input.UV - UVCenter) * Intensity;
    
    // �F��center����̋����������Ȃ�ق�, Color�̉e�����y�ڂ��悤�ɂ��邽��, ��Z�l�̒���.
    const float vignetteFactor = pow(saturate(1.0f - dot(uvDistance, uvDistance)), Smoothness);
    
    // center�ɋ߂��قǌ���Diffuse�F, �����ق�Color�F�ɂ���
    return diffuse * lerp(Color, (1.0f.xxxx), vignetteFactor); 

}
#endif