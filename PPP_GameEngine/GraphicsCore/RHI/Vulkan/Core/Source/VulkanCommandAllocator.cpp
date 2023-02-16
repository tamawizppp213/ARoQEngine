//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanCommandAllocator.cpp
///             @brief  Command Allocator
///                     This class is used to store the command list memory.
///                     Every time a command list is reset, it will allocate a block of memory.
///                     Even if a command list is reset, these memories aren't lost. 
///                     When the Reset function in this class is called, these memories cleans up. 
///                     To acieve the maximum frame rate, you should create each command list one by one.
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
RHICommandAllocator::RHICommandAllocator(const std::shared_ptr<core::RHIDevice>& device, const core::CommandListType type, const std::uint32_t queueFamilyIndex, const std::wstring& name) : core::RHICommandAllocator(device, type)
{
	const auto vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device);
	
	VkCommandPoolCreateInfo createInfo = 
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.pNext = nullptr,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, // enable each command buffer reset and buffer's lifetime is long.
		.queueFamilyIndex = queueFamilyIndex,
	};

	if (vkCreateCommandPool(vkDevice->GetDevice(), &createInfo, nullptr, &_commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool (vulkan api)");
	}

	SetName(name);
}

RHICommandAllocator::~RHICommandAllocator()
{
	const auto vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device);
	if (_commandPool) { vkDestroyCommandPool(vkDevice->GetDevice(), _commandPool, nullptr); }
}

/****************************************************************************
*                     CleanUp
*************************************************************************//**
*  @fn        void RHICommandAllocator::CleanUp()
* 
*  @brief     Reset command pool
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void RHICommandAllocator::CleanUp()
{
	const auto vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device);
	vkResetCommandPool(vkDevice->GetDevice(), _commandPool, VkCommandPoolResetFlagBits::VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
}

/****************************************************************************
*                     SetName
*************************************************************************//**
*  @fn        void RHICommandAllocator::SetName(const std::wstring& name)
*
*  @brief     Set debugging name
*
*  @param[in] const std::wstring& name
*
*  @return 　　void
*****************************************************************************/
void RHICommandAllocator::SetName(const std::wstring& name)
{
	const auto vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device);
	vkDevice->SetVkResourceName(name, VK_OBJECT_TYPE_COMMAND_POOL, reinterpret_cast<std::uint64_t>(_commandPool));
}