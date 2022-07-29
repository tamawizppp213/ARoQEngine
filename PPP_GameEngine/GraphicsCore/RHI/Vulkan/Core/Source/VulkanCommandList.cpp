//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanCommandAllocator.cpp
///             @brief  CommandAllocator
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanCommandList.hpp"
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
#pragma region Constructor and Destructor
RHICommandList::RHICommandList(const std::shared_ptr<core::RHIDevice>& device, const std::shared_ptr<core::RHICommandAllocator>& allocator): 
	core::RHICommandList(device, allocator)
{
	VkDevice      vkDevice    = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetDevice();
	VkCommandPool vkAllocator = std::static_pointer_cast<vulkan::RHICommandAllocator>(_commandAllocator)->GetCommandAllocator();

	VkCommandBufferAllocateInfo allocateInfo = {};
	allocateInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.commandPool        = vkAllocator;
	allocateInfo.commandBufferCount = 1;
	allocateInfo.level              = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY; // secondary buffer‚àŒŸ“¢‚·‚é
	allocateInfo.pNext = nullptr;

	if (vkAllocateCommandBuffers(vkDevice, &allocateInfo, &_commandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffer (primary)");
	}
}

RHICommandList::~RHICommandList()
{
	VkDevice      vkDevice    = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetDevice();
	VkCommandPool vkAllocator = std::static_pointer_cast<vulkan::RHICommandAllocator>(_commandAllocator)->GetCommandAllocator();
	vkFreeCommandBuffers(vkDevice, vkAllocator, 1, &_commandBuffer);

}

#pragma endregion Constructor and Destructor
#pragma region SetUp Draw Frame
void RHICommandList::BeginRecording()
{
	if (vkResetCommandBuffer(_commandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT) != VK_SUCCESS) { throw std::runtime_error("failed to reset command buffers\n"); };
	/*-------------------------------------------------------------------
	-               Set Up
	---------------------------------------------------------------------*/
	VkCommandBufferBeginInfo commandBeginInfo = {};
	commandBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	/*-------------------------------------------------------------------
	-               Command record start
	---------------------------------------------------------------------*/
	if (vkBeginCommandBuffer(_commandBuffer, &commandBeginInfo) != VK_SUCCESS) { throw std::runtime_error("failed to begin command buffers\n"); }

}

void RHICommandList::EndRecording()
{
	if (vkEndCommandBuffer(_commandBuffer) != VK_SUCCESS) { throw std::runtime_error("failed to end command buffer\n"); };

}
#pragma endregion SetUp Draw Frame
#pragma region GPU Command 

#pragma endregion GPU Command