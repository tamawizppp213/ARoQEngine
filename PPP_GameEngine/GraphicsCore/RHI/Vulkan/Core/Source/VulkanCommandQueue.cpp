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
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanFence.hpp"
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
#pragma region Constructor and Destructor
RHICommandQueue::~RHICommandQueue()
{
	
}

RHICommandQueue::RHICommandQueue(const std::shared_ptr<rhi::core::RHIDevice>& device, const core::CommandListType type, const std::uint32_t queueFamilyIndex, const std::uint32_t queueIndex) : rhi::core::RHICommandQueue(type)
{
	const auto vkDevice = std::static_pointer_cast<RHIDevice>(device);
	_queueFamilyIndex = queueFamilyIndex;
	_queueIndex       = queueIndex;
	vkGetDeviceQueue(vkDevice->GetDevice(), _queueFamilyIndex, _queueIndex, &_queue);
}
#pragma endregion Constructor and Destructor
#pragma region Public Function
/****************************************************************************
*							Wait
*************************************************************************//**
*  @fn        void RHICommandQueue::Wait(const std::shared_ptr<core::RHIFence>& fence, std::uint64_t value)
*  @brief     Used to wait for another Command queue to complete execution. (in GPU)
*  @param[in] const std::shared_ptr<core::RHIFence>& fence
*  @param[in] std::uint64_t value
*  @return 　　void
*****************************************************************************/
void RHICommandQueue::Wait(const std::shared_ptr<core::RHIFence>& fence, const std::uint64_t value)
{
	const auto& vkFence = std::static_pointer_cast<RHIFence>(fence);

	/*-------------------------------------------------------------------
	-               Set up timeline submit info
	---------------------------------------------------------------------*/
	VkTimelineSemaphoreSubmitInfo timelineInfo = {};
	timelineInfo.sType                     = VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO;
	timelineInfo.pNext                     = nullptr;
	timelineInfo.pWaitSemaphoreValues      = &value;
	timelineInfo.waitSemaphoreValueCount   = 1;
	timelineInfo.signalSemaphoreValueCount = 0;
	timelineInfo.pSignalSemaphoreValues    = nullptr;

	/*-------------------------------------------------------------------
	-               Set up submit info
	---------------------------------------------------------------------*/
	VkSubmitInfo submitInfo = {};
	submitInfo.sType                       = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext                       = &timelineInfo;
	submitInfo.waitSemaphoreCount          = 1;
	submitInfo.pWaitSemaphores             = &vkFence->GetFence();
	VkPipelineStageFlags waitDestStageMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
	submitInfo.pWaitDstStageMask           = &waitDestStageMask;

	if (vkQueueSubmit(_queue, 1, &submitInfo,VK_NULL_HANDLE) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit queue");
	}

}
/****************************************************************************
*							Signal
*************************************************************************//**
*  @fn        void RHICommandQueue::Signal(const std::shared_ptr<core::RHIFence>& fence, std::uint64_t value)
*  @brief     Update the fence value (value) when the Command Queue execution completes.
*  @param[in] const std::shared_ptr<core::RHIFence>& fence
*  @param[in] std::uint64_t value
*  @return 　　void
*****************************************************************************/
void RHICommandQueue::Signal(const std::shared_ptr<core::RHIFence>& fence, const std::uint64_t value)
{
	const auto& vkFence = std::static_pointer_cast<RHIFence>(fence);

	/*-------------------------------------------------------------------
	-               Set up timeline submit info
	---------------------------------------------------------------------*/
	VkTimelineSemaphoreSubmitInfo timelineInfo = {};
	timelineInfo.sType                     = VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO;
	timelineInfo.pNext                     = nullptr;
	timelineInfo.pWaitSemaphoreValues      = nullptr;
	timelineInfo.waitSemaphoreValueCount   = 0;
	timelineInfo.signalSemaphoreValueCount = 1;
	timelineInfo.pSignalSemaphoreValues    = &value;

	/*-------------------------------------------------------------------
	-               Set up submit info
	---------------------------------------------------------------------*/
	VkSubmitInfo submitInfo = {};
	submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext                = &timelineInfo;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores    = &vkFence->GetFence();

	if (vkQueueSubmit(_queue, 1, &submitInfo,VK_NULL_HANDLE) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit queue");
	}
}

/****************************************************************************
*                       Execute
*************************************************************************//**
*  @fn        void RHICommandQueue::Execute(const std::vector<std::shared_ptr<rhi::core::RHICommandList>>& commandLists)
*  @brief     Execute Command queue (同期はしてないので注意)
*  @param[in] const std::vector<std::shared_ptr<rhi::core::RHICommandList>>& commandLists
*  @return 　　void
*****************************************************************************/
void RHICommandQueue::Execute(const std::vector<std::shared_ptr<rhi::core::RHICommandList>>& commandLists)
{
	if (commandLists.empty()) { return; }

	/*-------------------------------------------------------------------
	-               Push back vulkan command lists
	-                to use VkCommandBuffer
	---------------------------------------------------------------------*/
	std::vector<VkCommandBuffer> vkLists;
	for (auto& commandList : commandLists)
	{
		vkLists.push_back(std::static_pointer_cast<rhi::vulkan::RHICommandList>(commandList)->GetCommandList());
	}
	/*-------------------------------------------------------------------
	-                Set Up
	---------------------------------------------------------------------*/
	VkPipelineStageFlags waitDestStageMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount      = static_cast<std::uint32_t>(vkLists.size());
	submitInfo.pCommandBuffers         = vkLists.data();
	submitInfo.pWaitDstStageMask       = &waitDestStageMask;
	submitInfo.waitSemaphoreCount      = 0;
	submitInfo.pWaitSemaphores         = nullptr;
	submitInfo.signalSemaphoreCount    = 0;
	submitInfo.pSignalSemaphores       = nullptr;

	/*-------------------------------------------------------------------
	-               Submit command to command queue
	---------------------------------------------------------------------*/
	auto result = vkQueueSubmit(_queue, 1, &submitInfo, nullptr);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit queue");
	}
}
#pragma endregion Public Function