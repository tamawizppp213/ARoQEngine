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
	_attachments.clear(); _attachments.shrink_to_fit();
}
GPUBlendState::GPUBlendState(const std::shared_ptr<rhi::core::RHIDevice>& device, const std::vector<rhi::core::BlendProperty>& blendProperties)
	: rhi::core::GPUBlendState(device, blendProperties)
{
	_attachments.resize(_blendProperties.size());
	for (size_t index = 0; index < _blendProperties.size(); index++)
	{
		const auto& blendProperty = _blendProperties[index];

		_attachments[index].colorWriteMask      = EnumConverter::Convert(blendProperty.ColorMask);
		_attachments[index].blendEnable         = blendProperty.Enable;
		_attachments[index].alphaBlendOp        = EnumConverter::Convert(blendProperty.AlphaOperator);
		_attachments[index].colorBlendOp        = EnumConverter::Convert(blendProperty.ColorOperator);
		_attachments[index].srcColorBlendFactor = EnumConverter::Convert(blendProperty.Source);
		_attachments[index].dstColorBlendFactor = EnumConverter::Convert(blendProperty.Destination);
		_attachments[index].srcAlphaBlendFactor = EnumConverter::Convert(blendProperty.SourceAlpha);
		_attachments[index].dstAlphaBlendFactor = EnumConverter::Convert(blendProperty.DestinationAlpha);
	}

	float blendConstants[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	_blendDesc.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	_blendDesc.attachmentCount = static_cast<std::uint32_t>(_blendProperties.size());
	_blendDesc.pAttachments    = _attachments.data();
	_blendDesc.logicOpEnable   = VK_FALSE;
	_blendDesc.logicOp         = VkLogicOp::VK_LOGIC_OP_NO_OP;
	_blendDesc.blendConstants[0] = 1.0f;
	_blendDesc.blendConstants[1] = 1.0f;
	_blendDesc.blendConstants[2] = 1.0f;
	_blendDesc.blendConstants[3] = 1.0f;
	_blendDesc.flags = 0;
	_blendDesc.pNext = nullptr;
}
GPUBlendState::GPUBlendState(const std::shared_ptr<rhi::core::RHIDevice>& device, const rhi::core::BlendProperty& blendProperty)
	: rhi::core::GPUBlendState(device, blendProperty)
{
	_attachments.resize(_blendProperties.size());
	for (size_t index = 0; index < _blendProperties.size(); index++)
	{
		const auto& blendProperty = _blendProperties[index];

		_attachments[index].colorWriteMask      = EnumConverter::Convert(blendProperty.ColorMask);
		_attachments[index].blendEnable         = blendProperty.Enable;
		_attachments[index].alphaBlendOp        = EnumConverter::Convert(blendProperty.AlphaOperator);
		_attachments[index].colorBlendOp        = EnumConverter::Convert(blendProperty.ColorOperator);
		_attachments[index].srcColorBlendFactor = EnumConverter::Convert(blendProperty.Source);
		_attachments[index].dstColorBlendFactor = EnumConverter::Convert(blendProperty.Destination);
		_attachments[index].srcAlphaBlendFactor = EnumConverter::Convert(blendProperty.SourceAlpha);
		_attachments[index].dstAlphaBlendFactor = EnumConverter::Convert(blendProperty.DestinationAlpha);
	}

	float blendConstants[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	_blendDesc.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	_blendDesc.attachmentCount = static_cast<std::uint32_t>(_blendProperties.size());
	_blendDesc.pAttachments    = _attachments.data();
	_blendDesc.logicOpEnable   = VK_FALSE;
	_blendDesc.logicOp         = VkLogicOp::VK_LOGIC_OP_NO_OP;
	_blendDesc.blendConstants[0] = 1.0f;
	_blendDesc.blendConstants[1] = 1.0f;
	_blendDesc.blendConstants[2] = 1.0f;
	_blendDesc.blendConstants[3] = 1.0f;
	_blendDesc.flags = 0;
	_blendDesc.pNext = nullptr;
}
