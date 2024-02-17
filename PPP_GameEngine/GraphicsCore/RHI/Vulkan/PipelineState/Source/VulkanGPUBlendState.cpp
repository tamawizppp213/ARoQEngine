//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanBlendState.cpp
///             @brief  Blend State
///             @author Toide Yutaro
///             @date   2022_06_29
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/PipelineState/Include/VulkanGPUBlendState.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUBlendState::~GPUBlendState()
{
	_attachments.Clear(); _attachments.ShrinkToFit();
}

GPUBlendState::GPUBlendState(const gu::SharedPointer<rhi::core::RHIDevice>& device, const gu::DynamicArray<rhi::core::BlendProperty>& blendProperties)
	: rhi::core::GPUBlendState(device, blendProperties)
{
	Prepare();
}

GPUBlendState::GPUBlendState(const gu::SharedPointer<rhi::core::RHIDevice>& device, const rhi::core::BlendProperty& blendProperty)
	: rhi::core::GPUBlendState(device, blendProperty)
{
	Prepare();
}

void GPUBlendState::Prepare()
{
	_attachments.Resize(_blendProperties.Size());
	for (size_t index = 0; index < _blendProperties.Size(); index++)
	{
		const auto& prop = _blendProperties[index];
		_attachments[index].colorWriteMask      = EnumConverter::Convert(prop.ColorMask);
		_attachments[index].blendEnable         = prop.Enable;
		_attachments[index].alphaBlendOp        = EnumConverter::Convert(prop.AlphaOperator);
		_attachments[index].colorBlendOp        = EnumConverter::Convert(prop.ColorOperator);
		_attachments[index].srcColorBlendFactor = EnumConverter::Convert(prop.SourceRGB);
		_attachments[index].dstColorBlendFactor = EnumConverter::Convert(prop.DestinationRGB);
		_attachments[index].srcAlphaBlendFactor = EnumConverter::Convert(prop.SourceAlpha);
		_attachments[index].dstAlphaBlendFactor = EnumConverter::Convert(prop.DestinationAlpha);
	}

	_blendDesc.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	_blendDesc.attachmentCount = static_cast<std::uint32_t>(_blendProperties.Size());
	_blendDesc.pAttachments    = _attachments.Data();
	_blendDesc.logicOpEnable   = VK_FALSE;
	_blendDesc.logicOp         = VkLogicOp::VK_LOGIC_OP_NO_OP;
	_blendDesc.blendConstants[0] = 1.0f;
	_blendDesc.blendConstants[1] = 1.0f;
	_blendDesc.blendConstants[2] = 1.0f;
	_blendDesc.blendConstants[3] = 1.0f;
	_blendDesc.flags = 0;
	_blendDesc.pNext = nullptr;
}

