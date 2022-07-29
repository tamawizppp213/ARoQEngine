//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12GPUFactory.cpp
///             @brief  DepthStencil State
///             @author Toide Yutaro
///             @date   2022_06_30
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/PipelineState/Include/DirectX12GPUPipelineFactory.hpp"
#include "GraphicsCore/RHI/DirectX12/PipelineState/Include/DirectX12GPUBlendState.hpp"
#include "GraphicsCore/RHI/DirectX12/PipelineState/Include/DirectX12GPUDepthStencilState.hpp"
#include "GraphicsCore/RHI/DirectX12/PipelineState/Include/DirectX12GPUInputAssemblyState.hpp"
#include "GraphicsCore/RHI/DirectX12/PipelineState/Include/DirectX12GPURasterizerState.hpp"
#include "GraphicsCore/RHI/DirectX12/PipelineState/Include/DirectX12GPUShaderState.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUPipelineFactory::GPUPipelineFactory(const std::shared_ptr<core::RHIDevice>& device)
	:core::GPUPipelineFactory(device)
{

}
std::shared_ptr<core::GPUInputAssemblyState> GPUPipelineFactory::CreateInputAssemblyState(
	const std::vector<core::InputLayoutElement>& elements,
	const core::PrimitiveTopology primitiveTopology)
{
	return std::static_pointer_cast<core::GPUInputAssemblyState>(
		std::make_shared<directX12::GPUInputAssemblyState>(_device, elements, primitiveTopology));
}

std::shared_ptr<core::GPURasterizerState> GPUPipelineFactory::CreateRasterizerState(
	const core::FrontFace   frontFace, 
	const core::CullingMode cullingMode,
	const core::FillMode    fillMode,
	const bool depthClamp)
{
	return std::static_pointer_cast<core::GPURasterizerState>(
		std::make_shared<GPURasterizerState>(_device, frontFace, cullingMode, fillMode, depthClamp));
}

std::shared_ptr<core::GPUDepthStencilState> GPUPipelineFactory::CreateDepthStencilState(
	const bool                 depthEnable,
	const bool                 depthWriteEnable,
	const bool                 stencilEnable,
	const core::CompareOperator      depthOperator,
	const core::StencilOperatorInfo& front,
	const core::StencilOperatorInfo& back)
{
	return std::static_pointer_cast<core::GPUDepthStencilState>(
		std::make_shared<GPUDepthStencilState>(_device, depthEnable, depthWriteEnable, stencilEnable, depthOperator, front, back));
}

std::shared_ptr<core::GPUShaderState> GPUPipelineFactory::CreateShaderState(
	const core::ShaderType type,
	const std::string& fileName,
	const std::string& entryPoint,
	const std::string& shaderVersion
)
{
	return std::static_pointer_cast<core::GPUShaderState>(std::make_shared<GPUShaderState>(_device, type, fileName, entryPoint, shaderVersion));
}

std::shared_ptr<core::GPUBlendState> GPUPipelineFactory::CreateBlendState(
	const std::vector<core::BlendProperty>& properties)
{
	return std::static_pointer_cast<core::GPUBlendState>(std::make_shared<GPUBlendState>(_device, properties));
}

std::shared_ptr<core::GPUBlendState> GPUPipelineFactory::CreateSingleBlendState(
	const core::BlendProperty& blendProperty)
{
	return std::static_pointer_cast<core::GPUBlendState>(std::make_shared<GPUBlendState>(_device, blendProperty));
}

std::shared_ptr<core::GPUBlendState> GPUPipelineFactory::CreateBlendState(const size_t numRenderTargets)
{
	return std::static_pointer_cast<core::GPUBlendState>(std::make_shared<GPUBlendState>(_device, std::vector<rhi::core::BlendProperty>(numRenderTargets)));
}