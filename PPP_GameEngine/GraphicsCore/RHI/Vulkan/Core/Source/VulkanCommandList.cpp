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
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanRenderPass.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanFrameBuffer.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUTexture.hpp"
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

void RHICommandList::BeginRenderPass(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::shared_ptr<core::RHIFrameBuffer>& frameBuffer)
{
	const auto vkRenderPass  = std::static_pointer_cast<vulkan::RHIRenderPass>(renderPass);
	const auto vkFrameBuffer = std::static_pointer_cast<vulkan::RHIFrameBuffer>(frameBuffer);
	/*-------------------------------------------------------------------
	-          Layout Transition (Present -> RenderTarget)
	---------------------------------------------------------------------*/
	std::vector<core::ResourceState> states(frameBuffer->GetRenderTargetSize(), core::ResourceState::RenderTarget);
	TransitionResourceStates(frameBuffer->GetRenderTargetSize(), frameBuffer->GetRenderTargets().data(), states.data());

	/*-------------------------------------------------------------------
	-               Set clear values
	---------------------------------------------------------------------*/
	std::vector<VkClearValue> clearValues = vkRenderPass->GetVkClearValues();
	/*-------------------------------------------------------------------
	-               Set Up VkRenderPassBeginInfo
	---------------------------------------------------------------------*/
	VkRenderPassBeginInfo beginInfo = {};
	beginInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	beginInfo.renderPass        = vkRenderPass->GetRenderPass();
	beginInfo.framebuffer       = vkFrameBuffer->GetFrameBuffer();
	beginInfo.pClearValues      = clearValues.data();
	beginInfo.clearValueCount   = static_cast<std::uint32_t>(clearValues.size());
	beginInfo.renderArea.extent = vkFrameBuffer->GetExtent2D();
	beginInfo.pNext             = nullptr;
	/*-------------------------------------------------------------------
	-               Set Command Begin Render Pass
	---------------------------------------------------------------------*/
	vkCmdBeginRenderPass(_commandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);

	_renderPass  = renderPass;
	_frameBuffer = frameBuffer;
}
void RHICommandList::EndRenderPass()
{
	vkCmdEndRenderPass(_commandBuffer);
	/*-------------------------------------------------------------------
	-          Layout Transition (RenderTarget -> Present)
	---------------------------------------------------------------------*/
	std::vector<core::ResourceState> states(_frameBuffer->GetRenderTargetSize(), core::ResourceState::Present);
	TransitionResourceStates(_frameBuffer->GetRenderTargetSize(), _frameBuffer->GetRenderTargets().data(), states.data());
}
#pragma endregion SetUp Draw Frame
#pragma region GPU Command 

#pragma region TransitionResourceLayout
void RHICommandList::TransitionResourceState(const std::shared_ptr<core::GPUTexture>& texture, core::ResourceState after)
{
	TransitionResourceStates(1, &texture, &after);
}
void RHICommandList::TransitionResourceStates(const std::uint32_t numStates, const std::shared_ptr<core::GPUTexture>* textures, core::ResourceState* afters)
{
	if (numStates <= 0) { return; }

	std::vector<VkImageMemoryBarrier> imageMemoryBarriers(numStates);
	for (size_t i = 0; i < numStates; ++i)
	{
		const auto vkTexture = std::static_pointer_cast<vulkan::GPUTexture>(textures[i]);

		auto& imageMemoryBarrier               = imageMemoryBarriers[i];
		imageMemoryBarrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.image               = vkTexture->GetImage();
		imageMemoryBarrier.oldLayout           = EnumConverter::Convert(vkTexture->GetResourceState());
		imageMemoryBarrier.newLayout           = EnumConverter::Convert(afters[i]);
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.srcAccessMask       = SelectVkAccessFlag(imageMemoryBarrier.oldLayout);
		imageMemoryBarrier.dstAccessMask       = SelectVkAccessFlag(imageMemoryBarrier.newLayout);
		imageMemoryBarrier.subresourceRange.aspectMask     = EnumConverter::Convert(vkTexture->GetPixelFormat(), vkTexture->GetUsage());
		imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
		imageMemoryBarrier.subresourceRange.baseMipLevel   = 0;
		imageMemoryBarrier.subresourceRange.layerCount     = vkTexture->GetArrayLength();
		imageMemoryBarrier.subresourceRange.levelCount     = vkTexture->GetMipMapLevels();
		imageMemoryBarrier.pNext = nullptr;

		vkTexture->TransitionResourceState(afters[i]);
	}

	vkCmdPipelineBarrier(_commandBuffer, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
		VkDependencyFlagBits::VK_DEPENDENCY_BY_REGION_BIT,
		0, nullptr, // memory barrier
		0, nullptr,  // buffer memory barrier
		static_cast<std::uint32_t>(imageMemoryBarriers.size()), imageMemoryBarriers.data()
	);
}

VkAccessFlags RHICommandList::SelectVkAccessFlag(const VkImageLayout imageLayout)
{
	switch (imageLayout)
	{
		case VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED:                { return VkAccessFlagBits::VK_ACCESS_NONE; }
		case VkImageLayout::VK_IMAGE_LAYOUT_GENERAL                 : { return VkAccessFlagBits::VK_ACCESS_SHADER_READ_BIT; }
		case VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: { return VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT; }
		case VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL: { return VkAccessFlagBits::VK_ACCESS_SHADER_READ_BIT; }
		case VkImageLayout::VK_IMAGE_LAYOUT_PREINITIALIZED:           { return VkAccessFlagBits::VK_ACCESS_HOST_WRITE_BIT; }
		case VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL: { return VkAccessFlagBits::VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VkAccessFlagBits::VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT; }
		case VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL    : { return VkAccessFlagBits::VK_ACCESS_TRANSFER_READ_BIT;  }
		case VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL    : { return VkAccessFlagBits::VK_ACCESS_TRANSFER_WRITE_BIT; }
	}
}
#pragma endregion TransitionResourceLayout
#pragma endregion GPU Command