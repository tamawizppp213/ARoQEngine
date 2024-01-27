//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanFence.cpp
///             @brief  CPU-GPU synchronization
///             @author Toide Yutaro
///             @date   2022_06_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/VulkanFence.hpp"
#include "../Include/VulkanDevice.hpp"
#include "../Include/VulkanCommandQueue.hpp"
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
RHIFence::RHIFence(const gu::SharedPointer<core::RHIDevice>& device, const std::uint64_t initialValue, const gu::wstring& name)
	: core::RHIFence(device)
{
	VkDevice vkDevice = nullptr;
	vkDevice = static_cast<rhi::vulkan::RHIDevice*>(_device.Get())->GetDevice();

	/*-------------------------------------------------------------------
	-   Set up semaphore type create info (TimelineInfo)
	---------------------------------------------------------------------*/
	const VkSemaphoreTypeCreateInfo timelineInfo = 
	{
		.sType         = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
		.pNext         = nullptr,
		.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
		.initialValue  = initialValue
	};

	/*-------------------------------------------------------------------
	-   Set up semaphore create info
	---------------------------------------------------------------------*/
	const VkSemaphoreCreateInfo semaphoreInfo = 
	{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		.pNext = &timelineInfo,
		.flags = 0
	};

	if (vkCreateSemaphore(vkDevice, &semaphoreInfo, nullptr, &_timelineSemaphore) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create fence (vulkan api)");
	}
	
	SetName(name);
}

RHIFence::~RHIFence()
{
	const auto vkDevice = static_cast<rhi::vulkan::RHIDevice*>(_device.Get())->GetDevice();
	if (_timelineSemaphore) 
	{
		vkDestroySemaphore(vkDevice, _timelineSemaphore, nullptr); 
		_timelineSemaphore = nullptr;
	}
}

#pragma endregion Constructor and Destructor
#pragma region Public Function
/****************************************************************************
*                     GetCompletedValue
*************************************************************************//**
*  @fn        std::uint64_t RHIFence::GetCompletedValue()
* 
*  @brief     Return current fence value
* 
*  @param[in] void
* 
*  @return 　　std::uint64_t fenceValue
*****************************************************************************/
std::uint64_t RHIFence::GetCompletedValue()
{
	VkDevice vkDevice = static_cast<rhi::vulkan::RHIDevice*>(_device.Get())->GetDevice();

	std::uint64_t value = 0;
	if (vkGetSemaphoreCounterValue(vkDevice, _timelineSemaphore, &value) != VK_SUCCESS)
	{
		throw std::runtime_error("couldn't get semaphore counter value");
	}

	return value;
}

/****************************************************************************
*                     Wait
*************************************************************************//**
*  @fn        void RHIFence::Wait(std::uint64_t value)
* 
*  @brief     Detect the completion of GPU processing on the CPU side. If not, it waits.
* 
*  @param[in] std::uint64_t value
* 
*  @return 　　void
*****************************************************************************/
void RHIFence::Wait(const std::uint64_t value)
{
	VkDevice vkDevice = static_cast<rhi::vulkan::RHIDevice*>(_device.Get())->GetDevice();

	const VkSemaphoreWaitInfo waitInfo = 
	{
		.sType          = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO,
		.pNext          = nullptr,
		.flags          = 0,
		.semaphoreCount = 1,
		.pSemaphores    = &_timelineSemaphore,
		.pValues        = &value
	};

	if (vkWaitSemaphores(vkDevice, &waitInfo, UINT64_MAX) < VK_SUCCESS) // error only acquire
	{
		throw std::runtime_error("couldn't wait semaphore");
	}
}

/****************************************************************************
*                     Signal
*************************************************************************//**
*  @fn        void RHIFence::Signal(const gu::SharedPointer<rhi::core::RHICommandQueue>& queue)
* 
*  @brief     @brief: Set fence value from CPU side. (in case RHICommandQueue::Signal -> Set fence value from GPU side)
* 
*  @param[in] std::uint64_t value
* 
*  @return 　　void
*****************************************************************************/
void RHIFence::Signal(const std::uint64_t value)
{
	VkDevice vkDevice = static_cast<rhi::vulkan::RHIDevice*>(_device.Get())->GetDevice();

	const VkSemaphoreSignalInfo signalInfo = 
	{
		.sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO,
		.pNext     = nullptr,
		.semaphore = _timelineSemaphore,
		.value     = value
	};

	if (vkSignalSemaphore(vkDevice, &signalInfo) < VK_SUCCESS) // error only acquire
	{
		throw std::runtime_error("couldn't signal semaphore");
	}
}
#pragma endregion Public Function

#pragma region Property
void RHIFence::SetName(const gu::wstring& name)
{
	const auto device = gu::StaticPointerCast<vulkan::RHIDevice>(_device);
	device->SetVkResourceName(name, VK_OBJECT_TYPE_SEMAPHORE, reinterpret_cast<std::uint64_t>(_timelineSemaphore));
}

#pragma endregion Property