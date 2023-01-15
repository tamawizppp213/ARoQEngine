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
std::wstring GPUShaderState::GetShaderTypeName(core::ShaderType shaderType)
{
	switch (shaderType)
	{
		case ShaderType::Vertex       : return L"vs";
		case ShaderType::Pixel        : return L"ps";
		case ShaderType::Compute      : return L"cs";
		case ShaderType::Mesh         : return L"ms";
		case ShaderType::Amplification: return L"as";
		case ShaderType::Domain       : return L"ds";
		case ShaderType::Geometry     : return L"gs";
		case ShaderType::Hull         : return L"hs";
		case ShaderType::RayGeneration:
		case ShaderType::Intersection:
		case ShaderType::AnyHit:
		case ShaderType::ClosestHit:
		case ShaderType::Miss:         return L"lib";
		default:
			throw std::runtime_error("not supported shader type");
	}
}

std::wstring GPUShaderState::Format(float version)
{
	std::wstringstream stream;
	stream << std::fixed << std::setprecision(1) << version;
	return stream.str().substr(0, 1) + L"_" + stream.str().substr(2, 1); // main version 5, 6, sub version .1, .2
}