//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanDepthStencilState.cpp
///             @brief  DepthStencil State
///             @author Toide Yutaro
///             @date   2022_06_30
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/PipelineState/Include/VulkanGPUDepthStencilState.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUDepthStencilState::GPUDepthStencilState( const std::shared_ptr<rhi::core::RHIDevice>& device,
	const bool  depthEnable, const bool depthWriteEnable,
	const bool  stencilEnable,
	const core::CompareOperator depthOperator,
	const core::StencilOperatorInfo& front, const core::StencilOperatorInfo& back
):
	core::GPUDepthStencilState(device, depthEnable, depthWriteEnable, stencilEnable, depthOperator, front, back)
{
	_depthStencilDesc.pNext = nullptr;
	_depthStencilDesc.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	_depthStencilDesc.flags = 0;
	_depthStencilDesc.depthTestEnable       = _depthEnable;
	_depthStencilDesc.depthWriteEnable      = _depthWriteEnable;
	_depthStencilDesc.depthCompareOp        = EnumConverter::Convert(_depthOperator);
	_depthStencilDesc.depthBoundsTestEnable = false;
	_depthStencilDesc.minDepthBounds = 0.0f;
	_depthStencilDesc.maxDepthBounds = 0.0f;
	_depthStencilDesc.stencilTestEnable = _stencilEnable;
	_depthStencilDesc.front = VkStencilOpState(
		EnumConverter::Convert(_frontFace.FailOperator),
		EnumConverter::Convert(_frontFace.PassOperator),
		EnumConverter::Convert(_frontFace.DepthFailOperator),
		EnumConverter::Convert(_frontFace.CompareOperator)
	);
	_depthStencilDesc.back = VkStencilOpState(
		EnumConverter::Convert(_backFace.FailOperator),
		EnumConverter::Convert(_backFace.PassOperator),
		EnumConverter::Convert(_backFace.DepthFailOperator),
		EnumConverter::Convert(_backFace.CompareOperator)
	);
}