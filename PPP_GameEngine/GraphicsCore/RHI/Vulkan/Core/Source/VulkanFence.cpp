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
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::vulkan;
using namespace rhi;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
RHIFence::RHIFence(const std::shared_ptr<core::RHIDevice>& device, const std::uint64_t initialValue)
	: core::RHIFence(device)
{
	VkDevice vkDevice = nullptr;
	vkDevice = static_cast<rhi::vulkan::RHIDevice*>(_device.get())->GetDevice();

	/*-------------------------------------------------------------------
	-   Set up semaphore type create info (TimelineInfo)
	---------------------------------------------------------------------*/
	VkSemaphoreTypeCreateInfo timelineInfo = {};
	timelineInfo.initialValue  = initialValue;
	timelineInfo.sType         = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
	timelineInfo.pNext         = nullptr;
	timelineInfo.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;

	/*-------------------------------------------------------------------
	-   Set up semaphore create info
	---------------------------------------------------------------------*/
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreInfo.pNext = &timelineInfo;
	semaphoreInfo.flags = 0;

	if (vkCreateSemaphore(vkDevice, &semaphoreInfo, nullptr, &_timelineSemaphore) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create fence (vulkan api)");
	}
	
}
RHIFence::~RHIFence()
{
	const auto vkDevice = static_cast<rhi::vulkan::RHIDevice*>(_device.get())->GetDevice();
	if (_timelineSemaphore) { vkDestroySemaphore(vkDevice, _timelineSemaphore, nullptr); }
}

std::uint64_t RHIFence::GetCompletedValue()
{
	VkDevice vkDevice = static_cast<rhi::vulkan::RHIDevice*>(_device.get())->GetDevice();

	std::uint64_t value = 0;
	vkGetSemaphoreCounterValue(vkDevice, _timelineSemaphore, &value);
	return value;
}

/****************************************************************************
*                     Wait
*************************************************************************//**
*  @fn        void RHIFence::Wait(std::uint64_t value)
*  @brief     Wait until value
*  @param[in] std::uint64_t value
*  @return 　　void
*****************************************************************************/
void RHIFence::Wait(const std::uint64_t value)
{
	VkDevice vkDevice = static_cast<rhi::vulkan::RHIDevice*>(_device.get())->GetDevice();

	VkSemaphoreWaitInfo waitInfo = {};
	waitInfo.sType          = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO;
	waitInfo.semaphoreCount = 1;
	waitInfo.pSemaphores    = &_timelineSemaphore;
	waitInfo.pValues        = &value;
	waitInfo.pNext          = nullptr;
	waitInfo.flags          = 0;
	if (vkWaitSemaphores(vkDevice, &waitInfo, UINT64_MAX) < VK_SUCCESS) // error only acquire
	{
		throw std::runtime_error("couldn't wait semaphore");
	}
}

/****************************************************************************
*                     Signal
*************************************************************************//**
*  @fn        void RHIFence::Signal(const std::shared_ptr<rhi::core::RHICommandQueue>& queue)
*  @brief     Signal issued
*  @param[in] std::uint64_t value
*  @return 　　void
*****************************************************************************/
void RHIFence::Signal(const std::uint64_t value)
{
	VkDevice vkDevice = static_cast<rhi::vulkan::RHIDevice*>(_device.get())->GetDevice();

	VkSemaphoreSignalInfo signalInfo = {};
	signalInfo.sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO;
	signalInfo.value     = value;
	signalInfo.pNext     = nullptr;
	signalInfo.semaphore = _timelineSemaphore;
	if (vkSignalSemaphore(vkDevice, &signalInfo) < VK_SUCCESS) // error only acquire
	{
		throw std::runtime_error("couldn't signal semaphore");
	}
}