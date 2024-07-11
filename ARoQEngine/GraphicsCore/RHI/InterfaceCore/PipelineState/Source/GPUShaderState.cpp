//////////////////////////////////////////////////////////////////////////////////
//              @file   GPUShaderState.cpp
///             @brief  Shader State 
///             @author Toide Yutaro
///             @date   2022_07_02
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUShaderState.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
gu::tstring GPUShaderState::GetShaderTypeName(core::ShaderType shaderType)
{
	switch (shaderType)
	{
		case ShaderType::Vertex       : return SP("vs");
		case ShaderType::Pixel        : return SP("ps");
		case ShaderType::Compute      : return SP("cs");
		case ShaderType::Mesh         : return SP("ms");
		case ShaderType::Amplification: return SP("as");
		case ShaderType::Domain       : return SP("ds");
		case ShaderType::Geometry     : return SP("gs");
		case ShaderType::Hull         : return SP("hs");
		case ShaderType::RayGeneration:
		case ShaderType::Intersection:
		case ShaderType::AnyHit:
		case ShaderType::ClosestHit:
		case ShaderType::Miss:         return SP("lib");
		default:
			throw "not supported shader type";
	}
}


gu::tstring GPUShaderState::Format(float version)
{
	tstring number = gu::tstring::FromNumber(version);
	number = number.Left(number.Find(SP(".")) + 2); // +2なのは, 小数第一位 + indexのずれ

	return number.SubString(0, 1) + SP("_") + number.SubString(2, 1); // main version 5, 6, sub version .1, .2
}

/*!**********************************************************************
*  @brief     有効なシェーダー設定かどうかを返します
*  @param[in] void
*  @return    bool
*************************************************************************/
bool GPUShaderState::IsValidShaderType() const noexcept
{
	return true;
}