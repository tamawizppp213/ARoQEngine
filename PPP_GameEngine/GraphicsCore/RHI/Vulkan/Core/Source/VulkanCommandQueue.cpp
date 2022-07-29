//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanCommandQueue.cpp
///             @brief  Command Queue
///             @author Toide Yutaro
///             @date   2022_06_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanCommandQueue.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanCommandList.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::vulkan;
using namespace rhi;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
RHICommandQueue::~RHICommandQueue()
{
	std::static_pointer_cast<rhi::vulkan::RHIDevice>(_device)->FreeQueue(_queueFamilyIndex);
}

RHICommandQueue::RHICommandQueue(const std::shared_ptr<rhi::core::RHIDevice>& device) : rhi::core::RHICommandQueue(device)
{
	const auto vkDevice = std::static_pointer_cast<RHIDevice>(_device);
	_queueFamilyIndex = static_cast<std::uint32_t>(vkDevice->AllocateQueue());
	vkGetDeviceQueue(vkDevice->GetDevice(), (std::uint32_t)vkDevice->GetGraphicsQueueFamilyIndex(), _queueFamilyIndex, &_queue);
}
/****************************************************************************
*                       Execute
*************************************************************************//**
*  @fn        void RHICommandQueue::Execute(const std::vector<std::shared_ptr<rhi::core::RHICommandList>>& commandLists)
*  @brief     Execute Command queue (ìØä˙ÇÕÇµÇƒÇ»Ç¢ÇÃÇ≈íçà”)
*  @param[in] const std::vector<std::shared_ptr<rhi::core::RHICommandList>>& commandLists
*  @return Å@Å@void
*****************************************************************************/
void RHICommandQueue::Execute(const std::vector<std::shared_ptr<rhi::core::RHICommandList>>& commandLists)
{
	if (commandLists.empty()) { return; }

	std::vector<VkCommandBuffer> vkLists;
	for (auto& commandList : commandLists)
	{
		vkLists.push_back(std::static_pointer_cast<rhi::vulkan::RHICommandList>(commandList)->GetCommandList());
	}
	/*-------------------------------------------------------------------
	-               Submit command to command queue
	---------------------------------------------------------------------*/
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount      = static_cast<std::uint32_t>(vkLists.size());
	submitInfo.pCommandBuffers         = vkLists.data();
	submitInfo.pWaitDstStageMask       = nullptr;
	submitInfo.waitSemaphoreCount      = 0;
	submitInfo.pWaitSemaphores         = nullptr;
	submitInfo.signalSemaphoreCount    = 0;
	submitInfo.pSignalSemaphores       = nullptr;

	auto result = vkQueueSubmit(_queue, 1, &submitInfo, nullptr);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit queue");
	}
}