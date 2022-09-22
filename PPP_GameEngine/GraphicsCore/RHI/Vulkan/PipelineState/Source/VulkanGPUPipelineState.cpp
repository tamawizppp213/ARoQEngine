//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanGPUPipelineState.cpp
///             @brief  Pipeline State (Graphic and Compute)
///             @author Toide Yutaro
///             @date   2022_08_14
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/PipelineState/Include/VulkanGPUPipelineState.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Graphics Pipeline State
GPUGraphicsPipelineState::GPUGraphicsPipelineState(
	const std::shared_ptr<core::RHIDevice>& device,
	const std::shared_ptr<core::RHIRenderPass>& renderPass,
	const std::shared_ptr<core::RHIResourceLayout>& layout) : core::GPUGraphicsPipelineState(device, renderPass, layout)
{

}

GPUGraphicsPipelineState::~GPUGraphicsPipelineState()
{
	VkDevice vkDevice = nullptr;
	vkDevice = std::static_pointer_cast<RHIDevice>(_device)->GetDevice();

	vkDestroyPipeline(vkDevice, _pipeline, nullptr);
	_dynamicStates.clear();
}

void GPUGraphicsPipelineState::CompleteSetting()
{

}
#pragma endregion Graphcis Pipeline State