//////////////////////////////////////////////////////////////////////////////////
///  @file   DefferedRenderingPixelShader.hpp
///  @brief  Deffered�����_�����O�Ŏg�p����PixelShader�̒�`
///  @author toide
///  @date   2024/06/23 23:42:51
//////////////////////////////////////////////////////////////////////////////////
#ifndef DEFFERED_RENDERING
#define DEFFERED_RENDERING

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../BasePass/DefferedRenderingSettings.hlsli"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
struct BasePassVStoPS
{
	/*! @brief ���_�̈ʒu */
    float4 SVPosition : SV_POSITION;
	
	/*! @brief ���_��UV */
    float2 UV       : TEXCOORD;
};

struct PSOutput
{
	/*! @brief �s�N�Z���̏o�͐F */
	float4 Albedo : SV_Target0;
};

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
/*!**********************************************************************
*  @brief     DifferedRendering���s��PixelShader
*  @param[in] void
*  @return    float4 : �o�͐F
*************************************************************************/
PSOutput MainPS(BasePassVStoPS input)
{
	PSOutput output;
	output.Albedo = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	/*-------------------------------------------------------------------
	-        GBuffer�̐ݒ�
	---------------------------------------------------------------------*/
    GBufferDesc GBuffer = (GBufferDesc)0;
	GBuffer.Albedo = half3(1.0h, 1.0h, 1.0h);
	
	return output;
}

#endif