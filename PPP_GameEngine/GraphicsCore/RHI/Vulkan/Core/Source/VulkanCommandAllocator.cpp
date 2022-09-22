//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanCommandAllocator.cpp
///             @brief  CommandAllocator
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanCommandAllocator.hpp"
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
RHICommandAllocator::RHICommandAllocator(const std::shared_ptr<core::RHIDevice>& device, const std::uint32_t queueFamilyIndex) : core::RHICommandAllocator(device)
{
	const auto vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device);
	
	VkCommandPoolCreateInfo createInfo = {};
	createInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	createInfo.queueFamilyIndex = queueFamilyIndex;
	createInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // enable each command buffer reset and buffer's lifetime is long.
	createInfo.pNext            = nullptr;
	if (vkCreateCommandPool(vkDevice->GetDevice(), &createInfo, nullptr, &_commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool (vulkan api)");
	}
}

RHICommandAllocator::~RHICommandAllocator()
{
	const auto vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device);
	if (_commandPool) { vkDestroyCommandPool(vkDevice->GetDevice(), _commandPool, nullptr); }
}
/****************************************************************************
*                     Reset
*************************************************************************//**
*  @fn        void RHICommandAllocator::Reset()
*  @brief     Reset command pool
*  @param[in] void
*  @return Å@Å@void
*****************************************************************************/
void RHICommandAllocator::Reset()
{
	const auto vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device);
	vkResetCommandPool(vkDevice->GetDevice(), _commandPool, VkCommandPoolResetFlagBits::VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
}