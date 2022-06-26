//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanFence.cpp
///             @brief  Fence
///             @author Toide Yutaro
///             @date   2022_06_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanFence.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanCommandQueue.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::vulkan;
using namespace rhi;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
RHIFence::RHIFence(const std::shared_ptr<core::RHIDevice>& device)
{
	_device = device;
	VkDevice vkDevice = nullptr;
	vkDevice = static_cast<rhi::vulkan::RHIDevice*>(_device.get())->GetDevice();

	VkFenceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	createInfo.pNext = nullptr;
}
RHIFence::~RHIFence()
{
	const auto vkDevice = static_cast<rhi::vulkan::RHIDevice*>(_device.get())->GetDevice();
	vkDestroyFence(vkDevice, _fence, nullptr);
}

void RHIFence::Signal(const std::shared_ptr<rhi::core::RHICommandQueue>& queue)
{
	const auto vkQueue = static_cast<rhi::vulkan::RHICommandQueue*>(queue.get())->GetQueue();
	vkQueueWaitIdle(vkQueue);
}