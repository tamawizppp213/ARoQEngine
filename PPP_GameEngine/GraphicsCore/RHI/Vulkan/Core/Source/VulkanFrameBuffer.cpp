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
RHIFrameBuffer::RHIFrameBuffer(const gu::SharedPointer<core::RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::DynamicArray<gu::SharedPointer<core::GPUTexture>>& renderTargets, const gu::SharedPointer<core::GPUTexture>& depthStencil)
	: core::RHIFrameBuffer(device, renderPass, renderTargets, depthStencil)
{
	Prepare();
}
RHIFrameBuffer::RHIFrameBuffer(const gu::SharedPointer<core::RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::GPUTexture>& renderTarget, const gu::SharedPointer<core::GPUTexture>& depthStencil)
	:core::RHIFrameBuffer(device, renderPass, renderTarget, depthStencil)
{
	Prepare();
}

RHIFrameBuffer::~RHIFrameBuffer()
{
	VkDevice vkDevice = nullptr;
	vkDevice = gu::StaticPointerCast<vulkan::RHIDevice>(_device)->GetDevice();
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
	const auto rhiDevice = gu::StaticPointerCast<vulkan::RHIDevice>(_device);
	VkDevice    vkDevice = rhiDevice->GetDevice();

	/*-------------------------------------------------------------------
	-               Create Image view
	---------------------------------------------------------------------*/
	gu::DynamicArray<VkImageView> imageViews(_renderTargets.Size());
	// Render Target
	for (size_t index = 0; index < _renderTargets.Size(); ++index)
	{
		// set up render target resource view
		_renderTargetViews[index] = rhiDevice->CreateResourceView(core::ResourceViewType::RenderTarget, _renderTargets[index],0,0, nullptr);
		_renderTargetSRVs [index] = rhiDevice->CreateResourceView(core::ResourceViewType::Texture     , _renderTargets[index],0,0, nullptr);
		_renderTargetUAVs [index] = rhiDevice->CreateResourceView(core::ResourceViewType::RWTexture   , _renderTargets[index],0,0, nullptr);

		imageViews[index] = gu::StaticPointerCast<vulkan::GPUResourceView>(_renderTargetViews[index])->GetImageView();

		// set rener target size
		_width  = std::max(_width , _renderTargets[index]->GetWidth());
		_height = std::max(_height, _renderTargets[index]->GetHeight());
	}

	// Depth Stencil
	if (_depthStencil)
	{
		// set up depth stencil resource view
		_depthStencilView = rhiDevice->CreateResourceView(core::ResourceViewType::DepthStencil,_depthStencil,0,0, nullptr);
		_depthStencilSRV  = rhiDevice->CreateResourceView(core::ResourceViewType::Texture     ,_depthStencil,0,0, nullptr);

		const auto vkDepthStancilView = gu::StaticPointerCast<vulkan::GPUResourceView>(_depthStencilView);
		imageViews.Push(vkDepthStancilView->GetImageView());
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
		.renderPass      = gu::StaticPointerCast<vulkan::RHIRenderPass>(_renderPass)->GetRenderPass(),
		.attachmentCount = static_cast<std::uint32_t>(imageViews.Size()),
		.pAttachments    = imageViews.Data(),
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