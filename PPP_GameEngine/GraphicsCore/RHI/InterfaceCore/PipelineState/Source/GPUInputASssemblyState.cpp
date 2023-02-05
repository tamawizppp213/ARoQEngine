//////////////////////////////////////////////////////////////////////////////////
//              @file   GPUShaderState.cpp
///             @brief  Shader State 
///             @author Toide Yutaro
///             @date   2022_07_02
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUInputAssemblyState.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
const std::vector<InputLayoutElement> GPUInputAssemblyState::DEFAULT_VERTEX_ELEMENTS =
{
	InputLayoutElement("POSITION", InputFormat::R32G32B32_FLOAT, 0),
	InputLayoutElement("NORMAL"  , InputFormat::R32G32B32_FLOAT, 0),
	InputLayoutElement("COLOR"   , InputFormat::R32G32B32A32_FLOAT, 0),
	InputLayoutElement("TEXCOORD", InputFormat::R32G32_FLOAT, 0)
};

const std::vector<InputLayoutElement> GPUInputAssemblyState::DEFAULT_SKINVERTEX_ELEMENTS =
{
	InputLayoutElement("POSITION", InputFormat::R32G32B32_FLOAT, 0),
	InputLayoutElement("NORMAL"  , InputFormat::R32G32B32_FLOAT, 0),
	InputLayoutElement("TEXCOORD", InputFormat::R32G32_FLOAT, 0),
	InputLayoutElement("BONE_NO" , InputFormat::R32G32B32A32_INT),
	InputLayoutElement("WEIGHT"  , InputFormat::R32G32B32A32_FLOAT)
};