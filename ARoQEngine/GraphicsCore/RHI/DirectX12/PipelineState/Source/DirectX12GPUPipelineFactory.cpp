//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12GPUFactory.cpp
///             @brief  Pipeline each stage creator
///             @author Toide Yutaro
///             @date   2022_06_30
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DirectX12GPUPipelineFactory.hpp"
#include "../Include/DirectX12GPUBlendState.hpp"
#include "../Include/DirectX12GPUDepthStencilState.hpp"
#include "../Include/DirectX12GPUInputAssemblyState.hpp"
#include "../Include/DirectX12GPURasterizerState.hpp"
#include "../Include/DirectX12GPUShaderState.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
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
		gu::MakeShared<directX12::GPUInputAssemblyState>(_device, elements, primitiveTopology));
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
		gu::MakeShared<GPUDepthStencilState>(_device,depthStencilProperty));
}

gu::SharedPointer<core::GPUShaderState> GPUPipelineFactory::CreateShaderState()
{
	return gu::StaticPointerCast<core::GPUShaderState>(gu::MakeShared<GPUShaderState>(_device));
}

gu::SharedPointer<core::GPUBlendState> GPUPipelineFactory::CreateBlendState(
	const gu::DynamicArray<core::BlendProperty>& properties)
{
	return gu::StaticPointerCast<core::GPUBlendState>(gu::MakeShared<GPUBlendState>(_device, properties));
}

gu::SharedPointer<core::GPUBlendState> GPUPipelineFactory::CreateSingleBlendState(
	const core::BlendProperty& blendProperty)
{
	return gu::StaticPointerCast<core::GPUBlendState>(gu::MakeShared<GPUBlendState>(_device, blendProperty));
}

gu::SharedPointer<core::GPUBlendState> GPUPipelineFactory::CreateBlendState(const size_t numRenderTargets)
{
	return gu::StaticPointerCast<core::GPUBlendState>(gu::MakeShared<GPUBlendState>(_device, gu::DynamicArray<rhi::core::BlendProperty>(numRenderTargets)));
}