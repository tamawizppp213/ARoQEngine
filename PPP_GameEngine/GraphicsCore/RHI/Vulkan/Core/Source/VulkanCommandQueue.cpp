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

RHICommandQueue::RHICommandQueue(const gu::SharedPointer<rhi::core::RHIDevice>& device, const core::CommandListType type, const std::uint32_t queueFamilyIndex, const std::uint32_t queueIndex, const gu::tstring& name) : rhi::core::RHICommandQueue(device, type)
{
	const auto vkDevice = gu::StaticPointerCast<RHIDevice>(device);
	_queueFamilyIndex = queueFamilyIndex;
	_queueIndex       = queueIndex;
	vkGetDeviceQueue(vkDevice->GetDevice(), _queueFamilyIndex, _queueIndex, &_queue);
	SetName(name);
}

#pragma endregion Constructor and Destructor

#pragma region Public Function
/****************************************************************************
*							Wait
*************************************************************************//**
*  @fn        void RHICommandQueue::Wait(const gu::SharedPointer<core::RHIFence>& fence, std::uint64_t value)
* 
*  @brief     Used to wait for another Command queue to complete execution. (in GPU)
* 
*  @param[in] const gu::SharedPointer<core::RHIFence>& fence
* 
*  @param[in] std::uint64_t value
* 
*  @return 　　void
*****************************************************************************/
void RHICommandQueue::Wait(const gu::SharedPointer<core::RHIFence>& fence, const std::uint64_t value)
{
	const auto& vkFence = gu::StaticPointerCast<RHIFence>(fence);

	/*-------------------------------------------------------------------
	-               Set up timeline submit info
	---------------------------------------------------------------------*/
	VkTimelineSemaphoreSubmitInfo timelineInfo = 
	{
		.sType                     = VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO,
		.pNext                     = nullptr,
		.waitSemaphoreValueCount   = 1,
		.pWaitSemaphoreValues      = &value,
		.signalSemaphoreValueCount = 0,
		.pSignalSemaphoreValues    = nullptr
	};

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
*  @fn        void RHICommandQueue::Signal(const gu::SharedPointer<core::RHIFence>& fence, std::uint64_t value)
*
*  @brief     Update value when the Command Queue execution completes.
*             GPU内で処理が完結します.　
*
*  @param[in] const gu::SharedPointer<core::RHIFence>& fence
*
*  @param[in] std::uint64_t value
*
*  @return 　　void
*****************************************************************************/
void RHICommandQueue::Signal(const gu::SharedPointer<core::RHIFence>& fence, const std::uint64_t value)
{
	const auto& vkFence = gu::StaticPointerCast<RHIFence>(fence);

	/*-------------------------------------------------------------------
	-               Set up timeline submit info
	---------------------------------------------------------------------*/
	VkTimelineSemaphoreSubmitInfo timelineInfo = 
	{
		.sType                     = VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO,
		.pNext                     = nullptr,
		.waitSemaphoreValueCount   = 0,
		.pWaitSemaphoreValues      = nullptr,
		.signalSemaphoreValueCount = 1,
		.pSignalSemaphoreValues    = &value
	};

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
*							Execute
*************************************************************************//**
*  @fn        void RHICommandQueue::Execute(const std::vector<gu::SharedPointer<rhi::core::RHICommandList>>& commandLists)
*
*  @brief     Execute command list contents. normally set graphics, compute, transfer commandlist
*             All CommandLists to be assigned must be Closed.
*
*  @param[in] const std::vector<gu::SharedPointer<rhi::core::RHICommandList>>& commandLists
*
*  @return 　　void
*****************************************************************************/
void RHICommandQueue::Execute(const std::vector<gu::SharedPointer<rhi::core::RHICommandList>>& commandLists)
{
	if (commandLists.empty()) { return; }

	/*-------------------------------------------------------------------
	-               Push back vulkan command lists
	-                to use VkCommandBuffer
	---------------------------------------------------------------------*/
	std::vector<VkCommandBuffer> vkLists;
	for (auto& commandList : commandLists)
	{
		vkLists.push_back(gu::StaticPointerCast<rhi::vulkan::RHICommandList>(commandList)->GetCommandList());
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

void RHICommandQueue::SetName(const gu::tstring& name)
{
	const auto device = gu::StaticPointerCast<vulkan::RHIDevice>(_device);
	device->SetVkResourceName(name, VK_OBJECT_TYPE_QUEUE, reinterpret_cast<std::uint64_t>(_queue));
}
#pragma endregion Public Function