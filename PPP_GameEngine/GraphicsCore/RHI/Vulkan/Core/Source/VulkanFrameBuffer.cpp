//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanFrameBuffer.cpp
///             @brief  Frame Buffer
///             @author Toide Yutaro
///             @date   2022_08_204
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/VulkanFrameBuffer.hpp"
#include "../Include/VulkanDevice.hpp"
#include "../Include/VulkanRenderPass.hpp"
#include "../../Resource/Include/VulkanGPUTexture.hpp"
#include "../Include/VulkanEnumConverter.hpp"
#include "../../Resource/Include/VulkanGPUResourceView.hpp"
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
* 
*  @brief     Prepare Frame Buffer 
* 
*  @param[in] void
* 
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
		// set up render target resource view
		_renderTargetViews[index] = rhiDevice->CreateResourceView(core::ResourceViewType::RenderTarget, _renderTargets[index], nullptr);
		_renderTargetSRVs [index] = rhiDevice->CreateResourceView(core::ResourceViewType::Texture     , _renderTargets[index], nullptr);
		_renderTargetUAVs [index] = rhiDevice->CreateResourceView(core::ResourceViewType::RWTexture   , _renderTargets[index], nullptr);

		imageViews[index] = std::static_pointer_cast<vulkan::GPUResourceView>(_renderTargetViews[index])->GetImageView();

		// set rener target size
		_width  = std::max(_width , _renderTargets[index]->GetWidth());
		_height = std::max(_height, _renderTargets[index]->GetHeight());
	}

	// Depth Stencil
	if (_depthStencil != nullptr)
	{
		// set up depth stencil resource view
		_depthStencilView = rhiDevice->CreateResourceView(core::ResourceViewType::DepthStencil, _depthStencil, nullptr);
		_depthStencilSRV  = rhiDevice->CreateResourceView(core::ResourceViewType::Texture     , _depthStencil, nullptr);

		const auto vkDepthStancilView = std::static_pointer_cast<vulkan::GPUResourceView>(_depthStencilView);
		imageViews.push_back(vkDepthStancilView->GetImageView());
	}

	// width, height must be set more than 0.
	_width  = std::max(_width , 1LLU);
	_height = std::max(_height, 1LLU);

	/*-------------------------------------------------------------------
	-               Create Frame Buffer Info
	---------------------------------------------------------------------*/
	const VkFramebufferCreateInfo createInfo = 
	{
		.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		.pNext           = nullptr,
		.flags           = 0,
		.renderPass      = std::static_pointer_cast<vulkan::RHIRenderPass>(_renderPass)->GetRenderPass(),
		.attachmentCount = static_cast<std::uint32_t>(imageViews.size()),
		.pAttachments    = imageViews.data(),
		.width           = static_cast<std::uint32_t>(_width),
		.height          = static_cast<std::uint32_t>(_height),
		.layers          = 1
	};

	/*-------------------------------------------------------------------
	-               Create Frame Buffer
	---------------------------------------------------------------------*/
	if (vkCreateFramebuffer(vkDevice, &createInfo, nullptr, &_frameBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create frame buffer (vulkan api)");
	}
}
#pragma endregion Prepare