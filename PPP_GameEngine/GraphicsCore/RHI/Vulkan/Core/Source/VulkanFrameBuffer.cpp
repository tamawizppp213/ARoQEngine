//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanFrameBuffer.cpp
///             @brief  Frame Buffer
///             @author Toide Yutaro
///             @date   2022_08_204
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanFrameBuffer.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanRenderPass.hpp"
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUTexture.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
#include <stdexcept>
#undef max
#undef min
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::vulkan;
using namespace rhi;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHIFrameBuffer::RHIFrameBuffer(const std::shared_ptr<core::RHIDevice>& device, const std::vector<std::shared_ptr<core::GPUTexture>>& renderTargets, const std::shared_ptr<core::GPUTexture>& depthStencil)
	: core::RHIFrameBuffer(device, renderTargets, depthStencil)
{
	Prepare();
}
RHIFrameBuffer::RHIFrameBuffer(const std::shared_ptr<core::RHIDevice>& device, const std::shared_ptr<core::GPUTexture>& renderTarget, const std::shared_ptr<core::GPUTexture>& depthStencil)
	:core::RHIFrameBuffer(device, renderTarget, depthStencil)
{
	Prepare();
}

RHIFrameBuffer::~RHIFrameBuffer()
{
	VkDevice vkDevice = nullptr;
	vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetDevice();

	for (auto& renderTargetView : _renderTargetView)
	{
		if (renderTargetView)
		{
			vkDestroyImageView(vkDevice, renderTargetView, nullptr);
		}
	}
	if (_depthStencilView)
	{
		vkDestroyImageView(vkDevice, _depthStencilView, nullptr);
	}

	vkDestroyFramebuffer(vkDevice, _frameBuffer, nullptr);
}
#pragma endregion Constructor and Destructor
#pragma region Prepare
/****************************************************************************
*                       Prepare
*************************************************************************//**
*  @fn        void RHIFrameBuffer::Prepare()
*  @brief     Prepare Frame Buffer 
*  @param[in] void
*  @return Å@Å@void
*****************************************************************************/
void RHIFrameBuffer::Prepare()
{
	VkDevice vkDevice = nullptr;
	vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetDevice();

	/*-------------------------------------------------------------------
	-               Set up attachments
	---------------------------------------------------------------------*/
	std::vector<core::Attachment>   colorAttachments(_renderTargets.size());
	std::optional<core::Attachment> depthAttachments = _depthStencil == nullptr ? std::nullopt : std::optional<core::Attachment>(core::Attachment::DepthStencilMultiSample(_depthStencil->GetPixelFormat(), _depthStencil->GetMultiSample()));
	for (size_t index = 0; index < _renderTargets.size(); ++index)
	{
		colorAttachments[index] = core::Attachment::RenderTargetMultiSample(_renderTargets[index]->GetPixelFormat(), _renderTargets[index]->GetMultiSample());
	}
	/*-------------------------------------------------------------------
	-               Create render pass
	---------------------------------------------------------------------*/
	_renderPass = std::make_shared<vulkan::RHIRenderPass>(_device, colorAttachments, depthAttachments);
	/*-------------------------------------------------------------------
	-               Create Image view
	---------------------------------------------------------------------*/
	std::vector<VkImageView> imageViews(_renderTargets.size());
	// Render Target
	_renderTargetView.resize(_renderTargets.size());
	for (size_t index = 0; index < _renderTargets.size(); ++index)
	{
		if (vkCreateImageView(vkDevice, &std::static_pointer_cast<vulkan::GPUTexture>(_renderTargets[index])->GetImageInfo(), nullptr, &_renderTargetView[index]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create image view");
		}
		imageViews[index] = _renderTargetView[index];
		_width  = std::max(_width, _renderTargets[index]->GetWidth());
		_height = std::max(_height, _renderTargets[index]->GetHeight());
	}
	// Depth Stencil
	if (_depthStencil != nullptr)
	{
		if (vkCreateImageView(vkDevice, &std::static_pointer_cast<vulkan::GPUTexture>(_depthStencil)->GetImageInfo(), nullptr, &_depthStencilView) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create image view");
		}
		imageViews.push_back(_depthStencilView);
		_width  = std::max(_width, _depthStencil->GetWidth());
		_height = std::max(_height,_depthStencil->GetHeight());
	}

	_width  = std::max(_width, 1LLU);
	_height = std::max(_height, 1LLU);
	/*-------------------------------------------------------------------
	-               Create Frame Buffer Info
	---------------------------------------------------------------------*/
	VkFramebufferCreateInfo createInfo = {};
	createInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	createInfo.flags           = 0;
	createInfo.pNext           = nullptr;
	createInfo.renderPass      = std::static_pointer_cast<vulkan::RHIRenderPass>(_renderPass)->GetRenderPass();
	createInfo.attachmentCount = static_cast<std::uint32_t>(imageViews.size());
	createInfo.pAttachments    = imageViews.data();
	createInfo.width           = static_cast<std::uint32_t>(_width);
	createInfo.height          = static_cast<std::uint32_t>(_height);
	/*-------------------------------------------------------------------
	-               Create Frame Buffer
	---------------------------------------------------------------------*/
	if (vkCreateFramebuffer(vkDevice, &createInfo, nullptr, &_frameBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create frame buffer (vulkan api)");
	}
}
#pragma endregion Prepare