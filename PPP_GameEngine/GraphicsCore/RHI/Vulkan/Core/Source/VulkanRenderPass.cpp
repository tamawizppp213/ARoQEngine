//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanRenderPass.cpp
///             @brief  RenderPass
///             @author Toide Yutaro
///             @date   2022_08_03
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanRenderPass.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
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
	vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetDevice();

	if (_renderPass) 
	{ 
		vkDestroyRenderPass(vkDevice, _renderPass, nullptr); 
		_renderPass = nullptr;
	}
}

RHIRenderPass::RHIRenderPass(const std::shared_ptr<core::RHIDevice>& device, const std::vector<core::Attachment>& colors, const std::optional<core::Attachment>& depth)
	: core::RHIRenderPass(device, colors, depth)
{
	Prepare();
}
RHIRenderPass::RHIRenderPass(const std::shared_ptr<core::RHIDevice>& device, const core::Attachment& color, const std::optional<core::Attachment>& depth)
	: core::RHIRenderPass(device, color, depth)
{
	Prepare();
}

#pragma endregion Constructor and Destructor

#pragma region Prepare Function
void rhi::vulkan::RHIRenderPass::Prepare()
{
	VkDevice vkDevice = nullptr;
	vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetDevice();

	/*-------------------------------------------------------------------
	-                  Get attachment size (color + depth)
	---------------------------------------------------------------------*/
	size_t attachmentCount = _colorAttachments.size() + (_depthAttachment.has_value() ? 1 : 0);
	/*-------------------------------------------------------------------
	-                  Resize Attachments
	---------------------------------------------------------------------*/
	std::vector<VkAttachmentDescription> attachments(attachmentCount);
	std::vector<VkAttachmentReference>   colorsReference(_colorAttachments.size());
	VkAttachmentReference                depthReference={};
	/*-------------------------------------------------------------------
	-                  Color Attachments
	---------------------------------------------------------------------*/
	for (size_t index = 0; index < _colorAttachments.size(); ++index)
	{
		const auto& colorAttachment       = _colorAttachments[index];
		colorsReference[index].attachment = static_cast<std::uint32_t>(attachmentCount);
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
		size_t index = attachments.size() - 1;
		attachments[index].flags          = 0;
		attachments[index].format         = EnumConverter::Convert(_depthAttachment->Format);
		attachments[index].samples        = EnumConverter::Convert(_depthAttachment->SampleCount);
		attachments[index].loadOp         = EnumConverter::Convert(_depthAttachment->LoadOp);
		attachments[index].storeOp        = EnumConverter::Convert(_depthAttachment->StoreOp);
		attachments[index].stencilLoadOp  = EnumConverter::Convert(_depthAttachment->StencilLoad);
		attachments[index].stencilStoreOp = EnumConverter::Convert(_depthAttachment->StencilStore);
		attachments[index].initialLayout  = EnumConverter::Convert(_depthAttachment->InitialLayout);
		attachments[index].finalLayout    = EnumConverter::Convert(_depthAttachment->FinalLayout);
		
		depthReference.attachment = static_cast<std::uint32_t>(colorsReference.size()); // size�̃^�C�~���O��depth�ɂȂ�.
		depthReference.layout     = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	}

	/*-------------------------------------------------------------------
	-                  Set sub pass descriptor
	---------------------------------------------------------------------*/
	VkSubpassDescription subpassDescription = {};
	subpassDescription.flags                   = 0;
	subpassDescription.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.inputAttachmentCount    = 0;
	subpassDescription.pInputAttachments       = nullptr;
	subpassDescription.colorAttachmentCount    = static_cast<std::uint32_t>(colorsReference.size());
	subpassDescription.pColorAttachments       = colorsReference.data();
	subpassDescription.pDepthStencilAttachment = _depthAttachment.has_value() ? & depthReference : nullptr;
	subpassDescription.pPreserveAttachments    = nullptr;
	subpassDescription.preserveAttachmentCount = 0;
	subpassDescription.pResolveAttachments     = nullptr;
	/*-------------------------------------------------------------------
	-                  Set render pass create info
	---------------------------------------------------------------------*/
	VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.flags           = 0;
	renderPassCreateInfo.attachmentCount = static_cast<std::uint32_t>(attachments.size());
	renderPassCreateInfo.pAttachments    = attachments.data();
	renderPassCreateInfo.subpassCount    = 1;
	renderPassCreateInfo.pSubpasses      = &subpassDescription;
	renderPassCreateInfo.dependencyCount = 0;
	renderPassCreateInfo.pDependencies   = nullptr;
	renderPassCreateInfo.pNext           = nullptr; // �σ��[�g�V�F�[�f�B���O���s���ꍇ�͊g������
	/*-------------------------------------------------------------------
	-                  Create render pass
	---------------------------------------------------------------------*/
	if (vkCreateRenderPass(vkDevice, &renderPassCreateInfo, nullptr, &_renderPass) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create render pass (vulkan api)");
	}
}

#pragma endregion Prepare Function
