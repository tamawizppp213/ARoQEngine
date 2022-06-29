//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12EnumConverter.cpp
///             @brief  Enum Converter
///             @author Toide Yutaro
///             @date   2022_06_29
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
#include <stdexcept>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
VkSamplerAddressMode   EnumConverter::Convert(const rhi::core::TextureAddressingMode addressingMode)
{
	switch (addressingMode)
	{
		case core::TextureAddressingMode::Wrap   : return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT;
		case core::TextureAddressingMode::Mirror : return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		case core::TextureAddressingMode::Clamp  : return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		case core::TextureAddressingMode::Boarder: return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		default:
			throw std::runtime_error("Not support texture addressing mode");
	}
}
VkFormat EnumConverter::Convert(const rhi::core::PixelFormat pixelFormat)
{
	switch (pixelFormat)
	{
		case core::PixelFormat::R8G8B8A8_UNORM    : return VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
		case core::PixelFormat::B8G8R8A8_UNORM    : return VkFormat::VK_FORMAT_B8G8R8A8_UNORM;
		case core::PixelFormat::R16G16B16A16_FLOAT: return VkFormat::VK_FORMAT_R16G16B16A16_SFLOAT;
		case core::PixelFormat::R32G32B32A32_FLOAT: return VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
		case core::PixelFormat::R32G32B32_FLOAT   : return VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
		case core::PixelFormat::D24_UNORM_S8_UINT : return VkFormat::VK_FORMAT_D24_UNORM_S8_UINT;
		case core::PixelFormat::R10G10B10A2_UNORM : return VkFormat::VK_FORMAT_A2R10G10B10_UNORM_PACK32;
		case core::PixelFormat::D32_FLOAT         : return VkFormat::VK_FORMAT_D32_SFLOAT;
		case core::PixelFormat::Unknown           : return VkFormat::VK_FORMAT_UNDEFINED;
		default:
			throw std::runtime_error("not supported Pixel Format type");
	}
}
#pragma region BlendState
VkBlendOp EnumConverter::Convert(const rhi::core::BlendOperator blendOperator)
{
	switch (blendOperator)
	{
		case core::BlendOperator::Add             : return VkBlendOp::VK_BLEND_OP_ADD;
		case core::BlendOperator::Subtract        : return VkBlendOp::VK_BLEND_OP_SUBTRACT;
		case core::BlendOperator::Reverse_Subtract: return VkBlendOp::VK_BLEND_OP_REVERSE_SUBTRACT;
		case core::BlendOperator::Min             : return VkBlendOp::VK_BLEND_OP_MIN;
		case core::BlendOperator::Max             : return VkBlendOp::VK_BLEND_OP_MAX;
		default:
			throw std::runtime_error("not supported Blend operator type");
	}
}
VkBlendFactor  EnumConverter::Convert(const rhi::core::BlendFactor blendFactor)
{
	switch (blendFactor)
	{
		case core::BlendFactor::Zero: return VkBlendFactor::VK_BLEND_FACTOR_ZERO;
		case core::BlendFactor::One: return VkBlendFactor::VK_BLEND_FACTOR_ONE;
		case core::BlendFactor::Source_Color: return VkBlendFactor::VK_BLEND_FACTOR_SRC_COLOR;
		case core::BlendFactor::Inverse_Source_Color: return VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
		case core::BlendFactor::Source_Alpha: return VkBlendFactor::VK_BLEND_FACTOR_SRC_ALPHA;
		case core::BlendFactor::Inverse_Source_Alpha: return VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		case core::BlendFactor::Dest_Alpha: return VkBlendFactor::VK_BLEND_FACTOR_DST_ALPHA;
		case core::BlendFactor::Inverse_Dest_Alpha: return VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
		case core::BlendFactor::Dest_Color: return VkBlendFactor::VK_BLEND_FACTOR_DST_COLOR;
		case core::BlendFactor::Inverse_Dest_Color: return VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
		default:
			throw std::runtime_error("not supported Blend Factor type");
	}
}
VkColorComponentFlags EnumConverter::Convert(const rhi::core::ColorMask colorMask)
{
	return static_cast<VkColorComponentFlags>(colorMask); // all the same
}
#pragma endregion BlendState
#pragma region RasterizerState
VkPolygonMode EnumConverter::Convert(const rhi::core::FillMode fillMode)
{
	switch (fillMode)
	{
		case core::FillMode::Solid    : return VkPolygonMode::VK_POLYGON_MODE_FILL;
		case core::FillMode::WireFrame: return VkPolygonMode::VK_POLYGON_MODE_LINE;
		default: 
			throw std::runtime_error("not support fillmode type");
	}
}
VkCullModeFlags  EnumConverter::Convert(const rhi::core::CullingMode cullingMode)
{
	switch (cullingMode)
	{
		case core::CullingMode::None : return VkCullModeFlagBits::VK_CULL_MODE_NONE;
		case core::CullingMode::Front: return VkCullModeFlagBits::VK_CULL_MODE_FRONT_BIT;
		case core::CullingMode::Back : return VkCullModeFlagBits::VK_CULL_MODE_BACK_BIT;
		default:
			throw std::runtime_error("not support culling mode");
	}
}
VkFrontFace EnumConverter::Convert(const rhi::core::FrontFace frontFace)
{
	switch (frontFace)
	{
		case core::FrontFace::Clockwise       : return VkFrontFace::VK_FRONT_FACE_CLOCKWISE;
		case core::FrontFace::CounterClockwise: return VkFrontFace::VK_FRONT_FACE_COUNTER_CLOCKWISE;
		default:
			throw std::runtime_error("not support front face type.");
	}
}
#pragma endregion RasterizerState