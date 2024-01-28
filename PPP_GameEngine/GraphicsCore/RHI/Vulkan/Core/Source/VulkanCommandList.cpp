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
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanResourceLayout.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDescriptorHeap.hpp"
#include "GraphicsCore/RHI/Vulkan/PipelineState/Include/VulkanGPUPipelineState.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUTexture.hpp"
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUBuffer.hpp"
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
RHICommandList::RHICommandList(const gu::SharedPointer<core::RHIDevice>& device, const gu::SharedPointer<core::RHICommandAllocator>& allocator, const gu::tstring& name): 
	core::RHICommandList(device, allocator)
{
	VkDevice      vkDevice    = gu::StaticPointerCast<vulkan::RHIDevice>(_device)->GetDevice();
	VkCommandPool vkAllocator = gu::StaticPointerCast<vulkan::RHICommandAllocator>(_commandAllocator)->GetCommandAllocator();

	VkCommandBufferAllocateInfo allocateInfo = {};
	allocateInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.commandPool        = vkAllocator;
	allocateInfo.commandBufferCount = 1;
	allocateInfo.level              = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY; // secondary bufferも検討する
	allocateInfo.pNext = nullptr;

	if (vkAllocateCommandBuffers(vkDevice, &allocateInfo, &_commandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffer (primary)");
	}
	_commandListType = _commandAllocator->GetCommandListType();
	_isOpen          = false;
	SetName(name);
}

RHICommandList::~RHICommandList()
{
	VkDevice      vkDevice    = gu::StaticPointerCast<vulkan::RHIDevice>(_device)->GetDevice();
	VkCommandPool vkAllocator = gu::StaticPointerCast<vulkan::RHICommandAllocator>(_commandAllocator)->GetCommandAllocator();
	vkFreeCommandBuffers(vkDevice, vkAllocator, 1, &_commandBuffer);

}

#pragma endregion Constructor and Destructor
#pragma region SetUp Draw Frame
/****************************************************************************
*                     BeginRecording
*************************************************************************//**
*  @fn        void RHICommandList::BeginRecording()
*
*  @brief     This function must be called at draw function initially (stillMidFrame = false
*             If still mid frame is set false, this function clears the command allocator
*
*  @param[in] const bool stillMidFrame (default: false)
*
*  @return    void
*****************************************************************************/
void RHICommandList::BeginRecording(const bool stillMidFrame)
{
	// recorded command allocator clean up
	if (!stillMidFrame) { _commandAllocator->CleanUp(); }

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

	_isOpen = true;
}

/****************************************************************************
*                     EndRecording
*************************************************************************//**
*  @fn        void RHICommandList::EndRecording()
*
*  @brief     Call at end draw frame. Close executed command list.
*              The command list is closed, it transits the executable state.
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void RHICommandList::EndRecording()
{
	if (!_isOpen) { OutputDebugStringA("Already closed \n"); return; }
	if (vkEndCommandBuffer(_commandBuffer) != VK_SUCCESS) { throw std::runtime_error("failed to end command buffer\n"); };
	_isOpen = false;
	_beginRenderPass = false;
}

void RHICommandList::BeginRenderPass(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::RHIFrameBuffer>& frameBuffer)
{
	const auto vkRenderPass  = gu::StaticPointerCast<vulkan::RHIRenderPass>(renderPass);
	const auto vkFrameBuffer = gu::StaticPointerCast<vulkan::RHIFrameBuffer>(frameBuffer);
	/*-------------------------------------------------------------------
	-          Layout Transition (Present -> RenderTarget)
	---------------------------------------------------------------------*/
	//std::vector<core::ResourceState> states(frameBuffer->GetRenderTargetSize(), core::ResourceState::RenderTarget);
	//TransitionResourceStates(static_cast<std::uint32_t>(frameBuffer->GetRenderTargetSize()), frameBuffer->GetRenderTargets().data(), states.data());

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
	_beginRenderPass = true;
}
void RHICommandList::EndRenderPass()
{
	if (!_beginRenderPass) { return; }
	vkCmdEndRenderPass(_commandBuffer);
	_beginRenderPass = false;
	/*-------------------------------------------------------------------
	-          Layout Transition (RenderTarget -> Present)
	---------------------------------------------------------------------*/
	//std::vector<core::ResourceState> states(_frameBuffer->GetRenderTargetSize(), core::ResourceState::Present);
	//TransitionResourceStates(static_cast<std::uint32_t>(_frameBuffer->GetRenderTargetSize()), _frameBuffer->GetRenderTargets().data(), states.data());
}
#pragma endregion SetUp Draw Frame
#pragma region GPU Command
/****************************************************************************
*                       SetViewport
*************************************************************************//**
*  @fn        void rhi::vulkan::RHICommandList::SetViewport(const core::Viewport* viewport, std::uint32_t numViewport)
*  @brief     Regist multi viewport to command list
*  @param[in] const core::Viewport view port
*  @param[in] std::uint32_t viewport count
*  @return 　　void
*****************************************************************************/
void RHICommandList::SetViewport(const core::Viewport* viewport, const std::uint32_t numViewport)
{
	std::vector<VkViewport> v(numViewport);
	for (std::uint32_t i = 0; i < numViewport; ++i)
	{
		v[i].x        = viewport->TopLeftX;
		v[i].y        = viewport->TopLeftY;
		v[i].width    = viewport->Width;
		v[i].height   = viewport->Height;
		v[i].maxDepth = viewport->MaxDepth;
		v[i].minDepth = viewport->MinDepth;
 	}
	vkCmdSetViewport(_commandBuffer, 0, numViewport, v.data());
}
/****************************************************************************
*                       SetScissorRect
*************************************************************************//**
*  @fn        void RHICommandList::SetScissor(const core::ScissorRect* rect, std::uint32_t numRect)
*  @brief     Regist multi scissorRect to command list
*  @param[in] const core::ScissorRect*
*  @param[in] std::uint32_t numRect
*  @return 　　void
*****************************************************************************/
void RHICommandList::SetScissor(const core::ScissorRect* rect, const std::uint32_t numRect)
{
	std::vector<VkRect2D> r(numRect);
	for (UINT i = 0; i < numRect; ++i)
	{
		r[i].offset.x      = rect->Left;
		r[i].offset.y      = rect->Top;
		r[i].extent.width  = rect->Right;
		r[i].extent.height = rect->Bottom;
	}
	vkCmdSetScissor(_commandBuffer, 0, numRect, r.data());
}
/****************************************************************************
*                       SetViewportAndScissor
*************************************************************************//**
*  @fn        void RHICommandList::SetViewportAndScissor(const core::Viewport& viewport, const core::ScissorRect& rect)
*  @brief     Regist viewport and scissorRect to command list
*  @param[in] const core::Viewport& viewport
*  @param[in] const core::ScissorRect& rect
*  @return 　　void
*****************************************************************************/
void RHICommandList::SetViewportAndScissor(const core::Viewport& viewport, const core::ScissorRect& rect)
{
	VkViewport v = {};
	v.x        = viewport.TopLeftX;
	v.y        = viewport.TopLeftY;
	v.width    = viewport.Width;
	v.height   = viewport.Height;
	v.maxDepth = viewport.MaxDepth;
	v.minDepth = viewport.MinDepth;

	VkRect2D r = {};
	r.offset.x      = rect.Left;
	r.extent.width  = rect.Right;
	r.extent.height = rect.Bottom;
	r.offset.y      = rect.Top;

	vkCmdSetViewport(_commandBuffer, 0, 1, &v);
	vkCmdSetScissor (_commandBuffer, 0, 1, &r);
}

#pragma region Graphics Command 
/****************************************************************************
*                       SetPrimitiveTopology
*************************************************************************//**
*  @fn        void RHICommandList::SetPrimitiveTopology(core::PrimitiveTopology topology)
*
*  @brief     Regist Primitive topology to command list
*
*  @param[in] const core::PrimitiveTopology
*
*  @return 　　void
*****************************************************************************/
void RHICommandList::SetPrimitiveTopology(const core::PrimitiveTopology topology)
{
	vkCmdSetPrimitiveTopology(_commandBuffer, EnumConverter::Convert(topology));
}
void RHICommandList::SetResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout)
{
	_resourceLayout = gu::StaticPointerCast<vulkan::RHIResourceLayout>(resourceLayout);
}
void RHICommandList::SetGraphicsPipeline(const gu::SharedPointer<core::GPUGraphicsPipelineState>& pipelineState)
{
	vkCmdBindPipeline(_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, gu::StaticPointerCast<vulkan::GPUGraphicsPipelineState>(pipelineState)->GetPipeline());
}

void RHICommandList::SetVertexBuffer(const gu::SharedPointer<core::GPUBuffer>& buffer)
{
	auto vkBuffer = gu::StaticPointerCast<vulkan::GPUBuffer>(buffer)->GetBuffer();
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(_commandBuffer, 0, 1, &vkBuffer, offsets);
}
void RHICommandList::SetVertexBuffers(const std::vector<gu::SharedPointer<core::GPUBuffer>>& buffers, const size_t startSlot)
{
	auto vkBuffers = std::vector<VkBuffer>(buffers.size());
	auto offsets   = std::vector<VkDeviceSize>(buffers.size(), 0);

	for (size_t i = 0; i < vkBuffers.size(); ++i)
	{
		vkBuffers[i] = gu::StaticPointerCast<vulkan::GPUBuffer>(buffers[i])->GetBuffer();
	}

	vkCmdBindVertexBuffers(_commandBuffer, static_cast<std::uint32_t>(startSlot), 
		static_cast<std::uint32_t>(vkBuffers.size()), vkBuffers.data(), offsets.data());
}
void RHICommandList::SetIndexBuffer(const gu::SharedPointer<core::GPUBuffer>& buffer, const core::IndexType indexType)
{
	const auto vkBuffer = gu::StaticPointerCast<vulkan::GPUBuffer>(buffer)->GetBuffer();
	vkCmdBindIndexBuffer(_commandBuffer, vkBuffer, 0, EnumConverter::Convert(indexType));
}
#pragma endregion Graphics Command
#pragma region TransitionResourceLayout
/****************************************************************************
*                     TransitionResourceStates
*************************************************************************//**
*  @fn        void RHICommandList::TransitionResourceStates(const gu::SharedPointer<core::GPUTexture>& textures, core::ResourceState afters)
*
*  @brief     Transition a single resource layout using barrier
*
*  @param[in] const gu::SharedPointer<core::GPUTexture>& texture array,
*  @param[in] core::ResourceState state array

*  @return 　　void
*****************************************************************************/
void RHICommandList::TransitionResourceState(const gu::SharedPointer<core::GPUTexture>& texture, core::ResourceState after)
{
	TransitionResourceStates(1, &texture, &after);
}

/****************************************************************************
*                     TransitionResourceStates
*************************************************************************//**
*  @fn        void RHICommandList::TransitionResourceStates(const std::uint32_t numStates, const gu::SharedPointer<core::GPUTexture>* textures, core::ResourceState* afters)
*
*  @brief     Transition resource layout using barrier
*
*  @param[in] const std::uint32_t numStates
*  @param[in] const gu::SharedPointer<core::GPUTexture>* texture array,
*  @param[in] core::ResourceState* state array

*  @return 　　void
*****************************************************************************/
void RHICommandList::TransitionResourceStates(const std::uint32_t numStates, const gu::SharedPointer<core::GPUTexture>* textures, core::ResourceState* afters)
{
	if (numStates <= 0) { return; }

	std::vector<VkImageMemoryBarrier> imageMemoryBarriers(numStates);
	for (size_t i = 0; i < numStates; ++i)
	{
		const auto vkTexture = gu::StaticPointerCast<vulkan::GPUTexture>(textures[i]);

		auto& imageMemoryBarrier               = imageMemoryBarriers[i];
		imageMemoryBarrier.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.pNext               = nullptr;
		imageMemoryBarrier.srcAccessMask       = SelectVkAccessFlag(imageMemoryBarrier.oldLayout);
		imageMemoryBarrier.dstAccessMask       = SelectVkAccessFlag(imageMemoryBarrier.newLayout);
		imageMemoryBarrier.oldLayout           = EnumConverter::Convert(vkTexture->GetResourceState());
		imageMemoryBarrier.newLayout           = EnumConverter::Convert(afters[i]);
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.image               = vkTexture->GetImage();
		imageMemoryBarrier.subresourceRange.aspectMask     = EnumConverter::Convert(vkTexture->GetPixelFormat(), vkTexture->GetUsage());
		imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
		imageMemoryBarrier.subresourceRange.baseMipLevel   = 0;
		imageMemoryBarrier.subresourceRange.layerCount     = static_cast<std::uint32_t>(vkTexture->GetArrayLength());
		imageMemoryBarrier.subresourceRange.levelCount     = static_cast<std::uint32_t>(vkTexture->GetMipMapLevels());

		vkTexture->TransitionResourceState(afters[i]);
	}

	vkCmdPipelineBarrier(_commandBuffer, 
		VkPipelineStageFlagBits::VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 
		VkPipelineStageFlagBits::VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
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
		default:
			return 0;
	}
}
#pragma endregion TransitionResourceLayout
#pragma endregion GPU Command

#pragma region Property
void RHICommandList::SetName(const gu::tstring& name)
{
	const auto device = gu::StaticPointerCast<vulkan::RHIDevice>(_device);
	device->SetVkResourceName(name, VK_OBJECT_TYPE_COMMAND_BUFFER, reinterpret_cast<std::uint64_t>(_commandBuffer));
}
#pragma endregion Property