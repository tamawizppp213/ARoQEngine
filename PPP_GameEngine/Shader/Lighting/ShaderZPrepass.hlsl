//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderZPrepass.hlsl
//            Content:  �s�����I�u�W�F�N�g�̐[�x�l(0�`1)���e�N�X�`���ɕ`�悵�܂�
//             Author:  Toide Yutaro
//             Create:  2023_10_05
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_ZPREPASS_HLSL
#define SHADER_ZPREPASS_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderVertexType.hlsli"
#include "../Core/ShaderConstantBuffer3D.hlsli"

//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////
struct PSIn
{
    float4 Position : SV_POSITION;
    float3 Depth    : TEXCOORD0;	
};

//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
float LinearizeDepth(const float depth, const float near, const float far)
{
    return (2.0f * near) / (far + near - depth * (far - near));
}

PSIn VSMain( VSInputSkinVertex vertexIn)
{
	PSIn result;
	
	/*-------------------------------------------------------------------
	-        transform to world space 
	---------------------------------------------------------------------*/
	const float4 positionWorld      = mul(World, vertexIn.Position);      // ���f���̃��[�J�����W�n -> ���[���h���W�n�ɕϊ�
    const float4 positionView       = mul(View, positionWorld);           // ���[���h���W�n       -> �r���[���W�n
    result.Position = mul(Projection, positionView);                      // �r���[���W�n     -> �X�N���[�����W�n
		
    result.Depth.x = result.Position.z / result.Position.w;       // ����`�Ȑ[�x��Ԃ̐��K��(0�`1)    : Raw
    result.Depth.y = positionView.z;                              // �J��������̃��[���h���W�Ƃ��Ă̋��� : Eye
    result.Depth.z = LinearizeDepth(result.Depth.x, NearZ, FarZ); // ���`�Ȑ[�x��Ԃ̐��K��(0�`1)      : Linear01
	return result;
}

float4 PSMain(PSIn psIn) : SV_TARGET
{
	// Raw, Eye, Linear01�̏��Ԃœ����Ă��܂�. 
	// ������, �`�敉�ׂ̊֌W�Ō���R32_Float�œ��삳���Ă���̂�, y,z,w�����͖�������܂�. ��������R32G32B32A32�œ������Ă�������.
	return float4(psIn.Depth.x, psIn.Depth.y, psIn.Depth.z, 1.0f);
}
#endif