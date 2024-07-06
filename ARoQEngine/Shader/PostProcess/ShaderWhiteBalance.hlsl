//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderWhiteBalance.hlsl
//            Content:  �z���C�g�o�����X�����肵�܂�. �F���x�ƐF������ύX�ł��܂�
//             Author:  Toide Yutaro
//             Create:  2023_10_06
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_WHITE_BALANCE_HLSL
#define SHADER_WHITE_BALANCE_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderVertexType.hlsli"
#include "../Core/ShaderColor.hlsli"
#define VertexIn  VSInputVertex
#define VertexOut VSOutputVertex
//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////
SamplerState SamplerLinearClamp : register(s0);
Texture2D DiffuseMap : register(t0);

cbuffer WhiteBalanceSettings : register(b0)
{
    float Temperature; // �F���x (-1�`1)
    float Tint;        // �F����
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
****************************************************************************/
/* @fn        float4 PSMain(VertexOut input) : SV_TARGET

*  @brief     Vignette post effect

*  @param[in] VertexOut input

*  @return �@�@float4
*****************************************************************************/
float4 PSMain(VertexOut input) : SV_TARGET
{
    const float4 diffuse = float4(DiffuseMap.Sample(SamplerLinearClamp, input.UV));
    
    // �͈͂Ƃ��Ă�[-1.67 �` 1.67]���œK�Ƃ̂���
    // https://docs.unity3d.com/ja/Packages/com.unity.shadergraph@10.0/manual/White-Balance-Node.html
    const float temperature = Temperature * 10 / 6;
    const float tint        = Tint * 10 / 6;
    
    /*-------------------------------------------------------------------
	-        �Q�Ɣ��F�_��CIE_RGB�\�F�n��p����xy�F�x���擾����
	---------------------------------------------------------------------*/
    const float whitePointX = 0.31271 - temperature * (temperature < 0 ? 0.1 : 0.05);;
    const float standardIlluminantY = 2.87 * whitePointX - 3 * whitePointX * whitePointX - 0.27509507;
    const float whitePointY = standardIlluminantY + tint * 0.05;
    
    // LMS��Ԃ̑��݌��ʂ��v�Z
    const float3 w   = float3(0.949237, 1.03542, 1.08728);
    const float3 LMS = xyYToLMS(float2(whitePointX, whitePointY));
    
    // �z���C�g�o�����X�̎擾
    const float3 balance = float3(w.x / LMS.x, w.y / LMS.y, w.z / LMS.z);
    
    /*-------------------------------------------------------------------
	-        LMS��Ԃ��g���ăz���C�g�o�����X�̒������s��.
	---------------------------------------------------------------------*/
    float3 lmsColor = LinearToLMS(diffuse.rgb);
    lmsColor *= balance;
    const float3 linearColor = LMSToLinear(lmsColor);
    
    return float4(linearColor, 1);

}
#endif