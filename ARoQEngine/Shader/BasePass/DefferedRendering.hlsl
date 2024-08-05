//////////////////////////////////////////////////////////////////////////////////
///  @file   DefferedRenderingPixelShader.hpp
///  @brief  Defferedレンダリングで使用するPixelShaderの定義
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
	/*! @brief 頂点の位置 */
    float4 SVPosition : SV_POSITION;
	
	/*! @brief 頂点のUV */
    float2 UV       : TEXCOORD;
};

struct PSOutput
{
	/*! @brief ピクセルの出力色 */
	float4 Albedo : SV_Target0;
};

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
/*!**********************************************************************
*  @brief     DifferedRenderingを行うPixelShader
*  @param[in] void
*  @return    float4 : 出力色
*************************************************************************/
PSOutput MainPS(BasePassVStoPS input)
{
	PSOutput output;
	output.Albedo = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	/*-------------------------------------------------------------------
	-        GBufferの設定
	---------------------------------------------------------------------*/
    GBufferDesc GBuffer = (GBufferDesc)0;
	GBuffer.Albedo = half3(1.0h, 1.0h, 1.0h);
	
	return output;
}

#endif