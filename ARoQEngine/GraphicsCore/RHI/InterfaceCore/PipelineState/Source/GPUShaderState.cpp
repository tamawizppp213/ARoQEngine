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
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <iomanip>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;


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
			throw std::runtime_error("not supported shader type");
	}
}

gu::tstring GPUShaderState::Format(float version)
{
	std::wstringstream stream;
	stream << std::fixed << std::setprecision(1) << version;
	std::wstring temp = stream.str().substr(0, 1) + L"_" + stream.str().substr(2, 1); // main version 5, 6, sub version .1, .2
	return gu::tstring(temp.c_str());
}