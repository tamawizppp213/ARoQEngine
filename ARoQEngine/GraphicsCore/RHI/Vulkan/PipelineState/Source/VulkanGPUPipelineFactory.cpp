//////////////////////////////////////////////////////////////////////////////////
//              @file   GPUPipelineFactory.cpp
///             @brief  Pipeline each stage creator
///             @author Toide Yutaro
///             @date   2022_06_30
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/VulkanGPUPipelineFactory.hpp"
#include "../Include/VulkanGPUBlendState.hpp"
#include "../Include/VulkanGPUDepthStencilState.hpp"
#include "../Include/VulkanGPUInputAssemblyState.hpp"
#include "../Include/VulkanGPURasterizerState.hpp"
#include "../Include/VulkanGPUShaderState.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUPipelineFactory::GPUPipelineFactory(const gu::SharedPointer<core::RHIDevice>& device)
	:core::GPUPipelineFactory(device)
{

}
gu::SharedPointer<core::GPUInputAssemblyState> GPUPipelineFactory::CreateInputAssemblyState(
	const gu::DynamicArray<core::InputLayoutElement>& elements,
	const core::PrimitiveTopology primitiveTopology)
{
	return gu::StaticPointerCast<core::GPUInputAssemblyState>(
		gu::MakeShared<GPUInputAssemblyState>(_device, elements, primitiveTopology));
}

gu::SharedPointer<core::GPURasterizerState> GPUPipelineFactory::CreateRasterizerState(
	const core::RasterizerProperty& rasterizerProperty)
{
	return gu::StaticPointerCast<core::GPURasterizerState>(
		gu::MakeShared<GPURasterizerState>(_device, rasterizerProperty));
}

gu::SharedPointer<core::GPUDepthStencilState> GPUPipelineFactory::CreateDepthStencilState(
	const core::DepthStencilProperty& depthStencilProperty)
{
	return gu::StaticPointerCast<core::GPUDepthStencilState>(
		gu::MakeShared<GPUDepthStencilState>(_device, depthStencilProperty));
}

gu::SharedPointer<core::GPUShaderState> GPUPipelineFactory::CreateShaderState()
{
	return gu::StaticPointerCast<core::GPUShaderState>(gu::MakeShared<GPUShaderState>(_device));
}

gu::SharedPointer<core::GPUBlendState> GPUPipelineFactory::CreateBlendState(
	const gu::DynamicArray<core::BlendProperty>& properties, const bool alphaToCoverageEnable)
{
	return gu::StaticPointerCast<core::GPUBlendState>(gu::MakeShared<GPUBlendState>(_device, properties, alphaToCoverageEnable));
}

gu::SharedPointer<core::GPUBlendState> GPUPipelineFactory::CreateSingleBlendState(
	const core::BlendProperty& blendProperty, const bool alphaToCoverageEnable)
{
	return gu::StaticPointerCast<core::GPUBlendState>(gu::MakeShared<GPUBlendState>(_device, blendProperty, alphaToCoverageEnable));
}

gu::SharedPointer<core::GPUBlendState> GPUPipelineFactory::CreateBlendState(const size_t numRenderTargets, const bool alphaToCoverageEnable)
{
	return gu::StaticPointerCast<core::GPUBlendState>(gu::MakeShared<GPUBlendState>(_device, gu::DynamicArray<rhi::core::BlendProperty>(numRenderTargets), alphaToCoverageEnable));
}