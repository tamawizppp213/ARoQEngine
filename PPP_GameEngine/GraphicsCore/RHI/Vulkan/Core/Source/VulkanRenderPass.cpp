//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanRenderPass.cpp
///             @brief  RenderPass
///             @author Toide Yutaro
///             @date   2022_08_03
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/VulkanRenderPass.hpp"
#include "../Include/VulkanDevice.hpp"
#include "../Include/VulkanEnumConverter.hpp"
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
RHIRenderPass::~RHIRenderPass()
{
	VkDevice vkDevice = nullptr;
	vkDevice = gu::StaticPointerCast<vulkan::RHIDevice>(_device)->GetDevice();

	if (_renderPass) 
	{ 
		vkDestroyRenderPass(vkDevice, _renderPass, nullptr); 
		_renderPass = nullptr;
	}
}

RHIRenderPass::RHIRenderPass(const gu::SharedPointer<core::RHIDevice>& device, const gu::DynamicArray<core::Attachment>& colors, const std::optional<core::Attachment>& depth)
	: core::RHIRenderPass(device, colors, depth)
{
	Prepare();
}
RHIRenderPass::RHIRenderPass(const gu::SharedPointer<core::RHIDevice>& device, const core::Attachment& color, const std::optional<core::Attachment>& depth)
	: core::RHIRenderPass(device, color, depth)
{
	Prepare();
}

#pragma endregion Constructor and Destructor
#pragma region Property
/****************************************************************************
*							GetVkClearValues
*************************************************************************//**
*  @fn        gu::DynamicArray<VkClearValue> rhi::vulkan::RHIRenderPass::GetVkClearValues() const 
*
*  @brief     Return clear value array including each render target and depth stencil buffer
*
*  @param[in] void
*
*  @return 　　gu::DynamicArray<VkClearValue>
*****************************************************************************/
gu::DynamicArray<VkClearValue> rhi::vulkan::RHIRenderPass::GetVkClearValues() const 
{
	/*-------------------------------------------------------------------
	-               Set clear values
	---------------------------------------------------------------------*/
	gu::DynamicArray<VkClearValue> clearValues(_colorClearValues.Size());
	// render target
	{
		for (size_t i = 0; i < clearValues.Size(); ++i)
		{
			clearValues[i].color.float32[0] = _colorClearValues[i].Color[core::ClearValue::Red];
			clearValues[i].color.float32[1] = _colorClearValues[i].Color[core::ClearValue::Green];
			clearValues[i].color.float32[2] = _colorClearValues[i].Color[core::ClearValue::Blue];
			clearValues[i].color.float32[3] = _colorClearValues[i].Color[core::ClearValue::Alpha];
		}
	}
	// depth stencil
	if (_depthClearValue.has_value())
	{
		VkClearValue clearValue = {};
		clearValue.depthStencil.depth   = _depthClearValue->Depth;
		clearValue.depthStencil.stencil = _depthClearValue->Stencil;
		clearValues.Push(clearValue);
	}

	return clearValues;
}

void RHIRenderPass::SetName(const gu::tstring& name)
{
	const auto device = gu::StaticPointerCast<vulkan::RHIDevice>(_device);
	device->SetVkResourceName(name, VK_OBJECT_TYPE_RENDER_PASS, reinterpret_cast<std::uint64_t>(_renderPass));
}

#pragma endregion Property
#pragma region Prepare Function
void rhi::vulkan::RHIRenderPass::Prepare()
{
	VkDevice vkDevice = nullptr;
	vkDevice = gu::StaticPointerCast<vulkan::RHIDevice>(_device)->GetDevice();

	/*-------------------------------------------------------------------
	-                  Get attachment size (color + depth)
	---------------------------------------------------------------------*/
	size_t attachmentCount = _colorAttachments.Size() + (_depthAttachment.has_value() ? 1 : 0);
	/*-------------------------------------------------------------------
	-                  Resize Attachments
	---------------------------------------------------------------------*/
	gu::DynamicArray<VkAttachmentDescription> attachments(attachmentCount);
	gu::DynamicArray<VkAttachmentReference>   colorsReference(_colorAttachments.Size());
	VkAttachmentReference                depthReference={};
	/*-------------------------------------------------------------------
	-                  Color Attachments
	---------------------------------------------------------------------*/
	for (size_t index = 0; index < _colorAttachments.Size(); ++index)
	{
		const auto& colorAttachment       = _colorAttachments[index];
		colorsReference[index].attachment = static_cast<std::uint32_t>(index);
		colorsReference[index].layout     = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		attachments[index].flags          = 0;
		attachments[index].format         = EnumConverter::Convert(colorAttachment.Format);
		attachments[index].samples        = EnumConverter::Convert(colorAttachment.SampleCount);
		attachments[index].loadOp         = EnumConverter::Convert(colorAttachment.LoadOp);
		attachments[index].storeOp        = EnumConverter::Convert(colorAttachment.StoreOp);
		attachments[index].stencilLoadOp  = EnumConverter::Convert(colorAttachment.StencilLoad);
		attachments[index].stencilStoreOp = EnumConverter::Convert(colorAttachment.StencilStore);
		attachments[index].initialLayout  = EnumConverter::Convert(colorAttachment.InitialLayout);
		attachments[index].finalLayout    = EnumConverter::Convert(colorAttachment.FinalLayout);
	}
	/*-------------------------------------------------------------------
	-                  Depth Attachment
	---------------------------------------------------------------------*/
	if (_depthAttachment.has_value())
	{
		size_t index = attachments.Size() - 1;
		attachments[index].flags          = 0;
		attachments[index].format         = EnumConverter::Convert(_depthAttachment->Format);
		attachments[index].samples        = EnumConverter::Convert(_depthAttachment->SampleCount);
		attachments[index].loadOp         = EnumConverter::Convert(_depthAttachment->LoadOp);
		attachments[index].storeOp        = EnumConverter::Convert(_depthAttachment->StoreOp);
		attachments[index].stencilLoadOp  = EnumConverter::Convert(_depthAttachment->StencilLoad);
		attachments[index].stencilStoreOp = EnumConverter::Convert(_depthAttachment->StencilStore);
		attachments[index].initialLayout  = EnumConverter::Convert(_depthAttachment->InitialLayout);
		attachments[index].finalLayout    = EnumConverter::Convert(_depthAttachment->FinalLayout);
		
		depthReference.attachment = static_cast<std::uint32_t>(colorsReference.Size()); // sizeのタイミングでdepthになる.
		depthReference.layout     = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	}

	/*-------------------------------------------------------------------
	-                  Set sub pass descriptor
	---------------------------------------------------------------------*/
	const VkSubpassDescription subpassDescription = 
	{
		.flags                   = 0,
		.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
		.inputAttachmentCount    = 0,
		.pInputAttachments       = nullptr,
		.colorAttachmentCount    = static_cast<std::uint32_t>(colorsReference.Size()),
		.pColorAttachments       = colorsReference.Data(),
		.pResolveAttachments     = nullptr,
		.pDepthStencilAttachment = _depthAttachment.has_value() ? &depthReference : nullptr,
		.preserveAttachmentCount = 0,
		.pPreserveAttachments    = nullptr
	};


	/*-------------------------------------------------------------------
	-                  Set render pass create info
	---------------------------------------------------------------------*/
	const VkRenderPassCreateInfo renderPassCreateInfo = 
	{
		.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.pNext           = nullptr,
		.flags           = 0,
		.attachmentCount = static_cast<std::uint32_t>(attachments.Size()),
		.pAttachments    = attachments.Data(),
		.subpassCount    = 1,
		.pSubpasses      = &subpassDescription,
		.dependencyCount = 0,
		.pDependencies   = nullptr
	};

	/*-------------------------------------------------------------------
	-                  Create render pass
	---------------------------------------------------------------------*/
	if (vkCreateRenderPass(vkDevice, &renderPassCreateInfo, nullptr, &_renderPass) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create render pass (vulkan api)");
	}
}

#pragma endregion Prepare Function

