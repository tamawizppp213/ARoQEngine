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
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUResourceView.hpp"
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
RHIFrameBuffer::RHIFrameBuffer(const std::shared_ptr<core::RHIDevice>& device, const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::vector<std::shared_ptr<core::GPUTexture>>& renderTargets, const std::shared_ptr<core::GPUTexture>& depthStencil)
	: core::RHIFrameBuffer(device, renderPass, renderTargets, depthStencil)
{
	Prepare();
}
RHIFrameBuffer::RHIFrameBuffer(const std::shared_ptr<core::RHIDevice>& device, const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::shared_ptr<core::GPUTexture>& renderTarget, const std::shared_ptr<core::GPUTexture>& depthStencil)
	:core::RHIFrameBuffer(device, renderPass, renderTarget, depthStencil)
{
	Prepare();
}

RHIFrameBuffer::~RHIFrameBuffer()
{
	VkDevice vkDevice = nullptr;
	vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetDevice();
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
	const auto rhiDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device);
	VkDevice    vkDevice = rhiDevice->GetDevice();

	/*-------------------------------------------------------------------
	-               Create Image view
	---------------------------------------------------------------------*/
	std::vector<VkImageView> imageViews(_renderTargets.size());
	// Render Target
	for (size_t index = 0; index < _renderTargets.size(); ++index)
	{
		_renderTargetViews[index] = rhiDevice->CreateResourceView(core::ResourceViewType::RenderTarget, _renderTargets[index], nullptr);

		const auto vkRenderTargetView = std::static_pointer_cast<vulkan::GPUResourceView>(_renderTargetViews[index]);
		imageViews[index] = vkRenderTargetView->GetImageView();

		// set rener target size
		_width  = std::max(_width, _renderTargets[index]->GetWidth());
		_height = std::max(_height, _renderTargets[index]->GetHeight());
	}
	// Depth Stencil
	if (_depthStencil != nullptr)
	{
		_depthStencilView = rhiDevice->CreateResourceView(core::ResourceViewType::DepthStencil, _depthStencil, nullptr);

		const auto vkDepthStancilView = std::static_pointer_cast<vulkan::GPUResourceView>(_depthStencilView);
		imageViews.push_back(vkDepthStancilView->GetImageView());

		_width  = std::max(_width , _depthStencil->GetWidth());
		_height = std::max(_height, _depthStencil->GetHeight());
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
	createInfo.layers          = 1;
	/*-------------------------------------------------------------------
	-               Create Frame Buffer
	---------------------------------------------------------------------*/
	if (vkCreateFramebuffer(vkDevice, &createInfo, nullptr, &_frameBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create frame buffer (vulkan api)");
	}
}
#pragma endregion Prepare