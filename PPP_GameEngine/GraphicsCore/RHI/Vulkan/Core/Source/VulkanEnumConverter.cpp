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
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
/*-------------------------------------------------------------------
-                        Pixel format mode
---------------------------------------------------------------------*/
VkFormat EnumConverter::Convert(const rhi::core::PixelFormat pixelFormat)
{
	switch (pixelFormat)
	{
		case core::PixelFormat::R8G8B8A8_UNORM    : return VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
		case core::PixelFormat::B8G8R8A8_UNORM    : return VkFormat::VK_FORMAT_B8G8R8A8_UNORM;
		case core::PixelFormat::B8G8R8A8_UNORM_SRGB: return VkFormat::VK_FORMAT_B8G8R8A8_SRGB;
		case core::PixelFormat::R16G16B16A16_FLOAT: return VkFormat::VK_FORMAT_R16G16B16A16_SFLOAT;
		case core::PixelFormat::R32G32B32A32_FLOAT: return VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
		case core::PixelFormat::R32G32B32_FLOAT   : return VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
		case core::PixelFormat::D24_UNORM_S8_UINT : return VkFormat::VK_FORMAT_D24_UNORM_S8_UINT;
		case core::PixelFormat::R10G10B10A2_UNORM : return VkFormat::VK_FORMAT_A2R10G10B10_UNORM_PACK32;
		case core::PixelFormat::R32_FLOAT         : return VkFormat::VK_FORMAT_R32_SFLOAT;
		case core::PixelFormat::D32_FLOAT         : return VkFormat::VK_FORMAT_D32_SFLOAT;
		case core::PixelFormat::BC1_UNORM         : return VkFormat::VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
		case core::PixelFormat::Unknown           : return VkFormat::VK_FORMAT_UNDEFINED;
		default:
			throw std::runtime_error("not supported Pixel Format type (vulkan api)");
	}
}
VkIndexType EnumConverter::Convert(const rhi::core::IndexType indexFormat)
{
	switch (indexFormat)
	{
		case core::IndexType::UInt32: return VkIndexType::VK_INDEX_TYPE_UINT32;
		case core::IndexType::UInt16: return VkIndexType::VK_INDEX_TYPE_UINT16;
		default:
			throw std::runtime_error("not supported Index Format type (vulkan api)");
	}
}
#pragma region Shader Stage
VkShaderStageFlagBits  EnumConverter::Convert(const rhi::core::ShaderType type)
{
	switch (type)
	{
		case core::ShaderType::Vertex       : return VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
		case core::ShaderType::Pixel        : return VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
		case core::ShaderType::Compute      : return VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
		case core::ShaderType::Geometry     : return VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT;
		case core::ShaderType::Amplification: return VkShaderStageFlagBits::VK_SHADER_STAGE_TASK_BIT_NV;
		case core::ShaderType::Mesh         : return VkShaderStageFlagBits::VK_SHADER_STAGE_MESH_BIT_NV;
		case core::ShaderType::Hull         : return VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		case core::ShaderType::Domain       : return VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		default:
			throw std::runtime_error("Not support shader stage");
	}
}
VkShaderStageFlagBits EnumConverter::Convert(const rhi::core::ShaderVisibility visibility)
{
	switch (visibility)
	{
		case core::ShaderVisibility::All          : return VkShaderStageFlagBits::VK_SHADER_STAGE_ALL;
		case core::ShaderVisibility::Vertex       : return VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
		case core::ShaderVisibility::Pixel        : return VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
		case core::ShaderVisibility::Geometry     : return VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT;
		case core::ShaderVisibility::Amplification: return VkShaderStageFlagBits::VK_SHADER_STAGE_TASK_BIT_NV;
		case core::ShaderVisibility::Mesh         : return VkShaderStageFlagBits::VK_SHADER_STAGE_MESH_BIT_NV;
		case core::ShaderVisibility::Hull         : return VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		case core::ShaderVisibility::Domain       : return VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		default:
			throw std::runtime_error("Not support shader visibility");
	}
}
#pragma endregion      Shader  Stage
#pragma region Sampler State
/*-------------------------------------------------------------------
-                        Filter Mode
---------------------------------------------------------------------*/
VkFilter EnumConverter::Convert(const rhi::core::FilterOption filter, const rhi::core::FilterMask mask)
{
	if (filter == core::FilterOption::Anisotropy) { return VkFilter::VK_FILTER_LINEAR; }

	// this equation indicates that the true value shows the filter has linear sampling option, 
	// and the false value has point sampling option.  
	return (static_cast<std::uint8_t>(filter) & static_cast<std::uint8_t>(mask)) != 0 
		? VkFilter::VK_FILTER_LINEAR : VkFilter::VK_FILTER_NEAREST;
}

/*-------------------------------------------------------------------
-                        Mipmap mode
---------------------------------------------------------------------*/
VkSamplerMipmapMode EnumConverter::Convert(const rhi::core::FilterOption filter)
{
	if (filter == core::FilterOption::Anisotropy) return VkSamplerMipmapMode::VK_SAMPLER_MIPMAP_MODE_LINEAR;
	return (static_cast<std::uint8_t>(filter) & 1) != 0 ?
		VkSamplerMipmapMode::VK_SAMPLER_MIPMAP_MODE_LINEAR:
		VkSamplerMipmapMode::VK_SAMPLER_MIPMAP_MODE_NEAREST;
}

/*-------------------------------------------------------------------
-                        Texture Addressing mode
---------------------------------------------------------------------*/
VkSamplerAddressMode EnumConverter::Convert(const rhi::core::SamplerAddressMode addressingMode)
{
	switch (addressingMode)
	{
		case core::SamplerAddressMode::Wrap   : return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT;
		case core::SamplerAddressMode::Mirror : return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		case core::SamplerAddressMode::Clamp  : return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		case core::SamplerAddressMode::Border : return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		default:
			throw std::runtime_error("Not support texture addressing mode (vulkan api)");
	}
}

/*-------------------------------------------------------------------
-                        Border color mode
---------------------------------------------------------------------*/
VkBorderColor  EnumConverter::Convert(const rhi::core::BorderColor borderColor)
{
	switch (borderColor)
	{
		case core::BorderColor::TransparentBlack: return VkBorderColor::VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
		case core::BorderColor::OpaqueBlack: return VkBorderColor::VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
		case core::BorderColor::OpaqueWhite: return VkBorderColor::VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		default:
			throw std::runtime_error("Not support border color (vulkan api)");
	}
}
/*-------------------------------------------------------------------
-                        Multi sample mode
---------------------------------------------------------------------*/
VkSampleCountFlagBits EnumConverter::Convert(const rhi::core::MultiSample sample)
{
	switch (sample)
	{
		case core::MultiSample::Count1 : return VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
		case core::MultiSample::Count2 : return VkSampleCountFlagBits::VK_SAMPLE_COUNT_2_BIT;
		case core::MultiSample::Count4 : return VkSampleCountFlagBits::VK_SAMPLE_COUNT_4_BIT;
		case core::MultiSample::Count8 : return VkSampleCountFlagBits::VK_SAMPLE_COUNT_8_BIT;
		case core::MultiSample::Count16: return VkSampleCountFlagBits::VK_SAMPLE_COUNT_16_BIT;
		case core::MultiSample::Count32: return VkSampleCountFlagBits::VK_SAMPLE_COUNT_32_BIT;
		case core::MultiSample::Count64: return VkSampleCountFlagBits::VK_SAMPLE_COUNT_64_BIT;
		default:
			throw std::runtime_error("Not support multi sample count (vulkan api)");
	}
}
#pragma endregion     Sampler State
#pragma region BlendState
/*-------------------------------------------------------------------
-                        Blend ioperate mode
---------------------------------------------------------------------*/
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
/*-------------------------------------------------------------------
-                        Blend factor mode
---------------------------------------------------------------------*/
VkBlendFactor  EnumConverter::Convert(const rhi::core::BlendFactor blendFactor)
{
	switch (blendFactor)
	{
		case core::BlendFactor::Zero                : return VkBlendFactor::VK_BLEND_FACTOR_ZERO;
		case core::BlendFactor::One                 : return VkBlendFactor::VK_BLEND_FACTOR_ONE;
		case core::BlendFactor::Source_Color        : return VkBlendFactor::VK_BLEND_FACTOR_SRC_COLOR;
		case core::BlendFactor::Inverse_Source_Color: return VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
		case core::BlendFactor::Source_Alpha        : return VkBlendFactor::VK_BLEND_FACTOR_SRC_ALPHA;
		case core::BlendFactor::Inverse_Source_Alpha: return VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		case core::BlendFactor::Dest_Alpha          : return VkBlendFactor::VK_BLEND_FACTOR_DST_ALPHA;
		case core::BlendFactor::Inverse_Dest_Alpha  : return VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
		case core::BlendFactor::Dest_Color          : return VkBlendFactor::VK_BLEND_FACTOR_DST_COLOR;
		case core::BlendFactor::Inverse_Dest_Color  : return VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
		default:
			throw std::runtime_error("not supported Blend Factor type");
	}
}
/*-------------------------------------------------------------------
-                        Color mask mode
---------------------------------------------------------------------*/
VkColorComponentFlags EnumConverter::Convert(const rhi::core::ColorMask colorMask)
{
	return static_cast<VkColorComponentFlags>(colorMask); // all the same
}
#pragma endregion        Blend   State
#pragma region RasterizerState
/*-------------------------------------------------------------------
-                        Polygon filling mode
---------------------------------------------------------------------*/
VkPolygonMode   EnumConverter::Convert(const rhi::core::FillMode fillMode)
{
	switch (fillMode)
	{
		case core::FillMode::Solid    : return VkPolygonMode::VK_POLYGON_MODE_FILL;
		case core::FillMode::WireFrame: return VkPolygonMode::VK_POLYGON_MODE_LINE;
		case core::FillMode::Point    : return VkPolygonMode::VK_POLYGON_MODE_POINT;
		default: 
			throw std::runtime_error("not support fillmode type");
	}
}
/*-------------------------------------------------------------------
-                        Culling mode
---------------------------------------------------------------------*/
VkCullModeFlags EnumConverter::Convert(const rhi::core::CullingMode cullingMode)
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
/*-------------------------------------------------------------------
-                        Front face mode
---------------------------------------------------------------------*/
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
#pragma endregion   Rasterizer State
#pragma region DepthStencilState
/*-------------------------------------------------------------------
-                        Compare operator mode
---------------------------------------------------------------------*/
VkCompareOp  EnumConverter::Convert(const rhi::core::CompareOperator compareOperator)
{
	switch (compareOperator)
	{
		case core::CompareOperator::Never       : return VkCompareOp::VK_COMPARE_OP_NEVER;
		case core::CompareOperator::Less        : return VkCompareOp::VK_COMPARE_OP_LESS;
		case core::CompareOperator::Equal       : return VkCompareOp::VK_COMPARE_OP_EQUAL;
		case core::CompareOperator::LessEqual   : return VkCompareOp::VK_COMPARE_OP_LESS_OR_EQUAL;
		case core::CompareOperator::Greater     : return VkCompareOp::VK_COMPARE_OP_GREATER;
		case core::CompareOperator::NotEqual    : return VkCompareOp::VK_COMPARE_OP_NOT_EQUAL;
		case core::CompareOperator::GreaterEqual: return VkCompareOp::VK_COMPARE_OP_GREATER_OR_EQUAL;
		case core::CompareOperator::Always      : return VkCompareOp::VK_COMPARE_OP_ALWAYS;
		default:
			throw std::runtime_error("not supported compare operator type");
	}
}
/*-------------------------------------------------------------------
-                        Stencil operator mode
---------------------------------------------------------------------*/
VkStencilOp  EnumConverter::Convert(const rhi::core::StencilOperator stencilOperator)
{
	switch (stencilOperator)
	{
		case core::StencilOperator::Keep             : return VkStencilOp::VK_STENCIL_OP_KEEP;
		case core::StencilOperator::Zero             : return VkStencilOp::VK_STENCIL_OP_ZERO;
		case core::StencilOperator::Replace          : return VkStencilOp::VK_STENCIL_OP_REPLACE;
		case core::StencilOperator::IncrementAndClamp: return VkStencilOp::VK_STENCIL_OP_INCREMENT_AND_CLAMP;
		case core::StencilOperator::DecrementAndClamp: return VkStencilOp::VK_STENCIL_OP_DECREMENT_AND_CLAMP;
		case core::StencilOperator::Invert           : return VkStencilOp::VK_STENCIL_OP_INVERT;
		case core::StencilOperator::IncrementAndWrap : return VkStencilOp::VK_STENCIL_OP_INCREMENT_AND_WRAP;
		case core::StencilOperator::DecrementAndWrap : return VkStencilOp::VK_STENCIL_OP_DECREMENT_AND_WRAP;
		default:
			throw std::runtime_error("not supported depth stencil operator type");
	}
}
#pragma endregion DepthStencil State
#pragma region Input Layout
/*-------------------------------------------------------------------
-                        Primitive topology mode
---------------------------------------------------------------------*/
VkPrimitiveTopology EnumConverter::Convert(const rhi::core::PrimitiveTopology primitiveTopology)
{
	switch (primitiveTopology)
	{
		case core::PrimitiveTopology::LineList     : return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		case core::PrimitiveTopology::LineStrip    : return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
		case core::PrimitiveTopology::TriangleList : return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		case core::PrimitiveTopology::TriangleStrip: return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		default:
			throw std::runtime_error("not supported primitive topology type (vulkan api) ");
	}
}

/*-------------------------------------------------------------------
-                        Input classification
---------------------------------------------------------------------*/
VkVertexInputRate EnumConverter::Convert(const rhi::core::InputClassification classification)
{
	using enum core::InputClassification;
	switch (classification)
	{
		case PerVertex  : return VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;
		case PerInstance: return VkVertexInputRate::VK_VERTEX_INPUT_RATE_INSTANCE;
		default:
			throw std::runtime_error("not support input classification type");
	}
}

VkFormat EnumConverter::Convert(const rhi::core::InputFormat inputFormat)
{
	switch (inputFormat)
	{
		case core::InputFormat::R32G32_FLOAT      : return VkFormat::VK_FORMAT_R32G32_SFLOAT;
		case core::InputFormat::R32G32B32_FLOAT   : return VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
		case core::InputFormat::R32G32B32A32_FLOAT: return VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
		case core::InputFormat::R32G32B32A32_INT  : return VkFormat::VK_FORMAT_R32G32B32A32_SINT;
		case core::InputFormat::R32_FLOAT         : return VkFormat::VK_FORMAT_R32_SFLOAT;
		default:
			throw std::runtime_error("not supported input layout format type (vulkan api)");
	}
}
#pragma endregion      Input Layout
#pragma region GPUResource
/*-------------------------------------------------------------------
-                      Resource Usage type
---------------------------------------------------------------------*/
EnumConverter::VulkanResourceUsage EnumConverter::Convert(const core::ResourceUsage usage)
{
	static std::vector<core::ResourceUsage> sourcePool =
	{
		core::ResourceUsage::None,
		core::ResourceUsage::VertexBuffer,
		core::ResourceUsage::IndexBuffer,
		core::ResourceUsage::ConstantBuffer,
		core::ResourceUsage::RenderTarget,
		core::ResourceUsage::DepthStencil,
		core::ResourceUsage::UnorderedAccess,
	};

	static std::vector<VulkanResourceUsage> targetPool = {
		VulkanResourceUsage(0, 0) ,
		VulkanResourceUsage(VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 0),
		VulkanResourceUsage(VkBufferUsageFlagBits::VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 0),
		VulkanResourceUsage(VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 0),
		VulkanResourceUsage(0, VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT),
		VulkanResourceUsage(0, VkImageUsageFlagBits::VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT),
		VulkanResourceUsage(VkBufferUsageFlagBits::VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, 0)
	};

	auto result = VulkanResourceUsage(0, 0);
	for (size_t i = 0; i < sourcePool.size(); ++i)
	{
		if (core::EnumHas(usage, sourcePool[i]))
		{
			result.first |= targetPool[i].first;     // for buffer
			result.second |= targetPool[i].second;   // for texture
		}
	}

	result.first  |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_DST_BIT | VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	result.second |= VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	result.second |= VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT;
	return result;
}
/*-------------------------------------------------------------------
-                        Image resource dimension mode
---------------------------------------------------------------------*/
VkImageType EnumConverter::Convert(const rhi::core::ResourceDimension dimension)
{
	switch (dimension)
	{
		case core::ResourceDimension::Dimension1D: return VkImageType::VK_IMAGE_TYPE_1D;
		case core::ResourceDimension::Dimension2D: return VkImageType::VK_IMAGE_TYPE_2D;
		case core::ResourceDimension::Dimension3D: return VkImageType::VK_IMAGE_TYPE_3D;
		default:
			throw std::runtime_error("not supported resource dimension (vulkan api) ");
	}
}
/*-------------------------------------------------------------------
-                        Image resource mode
---------------------------------------------------------------------*/
VkImageViewType  EnumConverter::Convert(const rhi::core::ResourceType type)
{
	switch (type)
	{
		case core::ResourceType::Texture1D       : return VkImageViewType::VK_IMAGE_VIEW_TYPE_1D;
		case core::ResourceType::Texture1DArray  : return VkImageViewType::VK_IMAGE_VIEW_TYPE_1D_ARRAY;
		case core::ResourceType::Texture2D       : return VkImageViewType::VK_IMAGE_VIEW_TYPE_2D;
		case core::ResourceType::Texture2DArray  : return VkImageViewType::VK_IMAGE_VIEW_TYPE_2D_ARRAY;
		case core::ResourceType::Texture3D       : return VkImageViewType::VK_IMAGE_VIEW_TYPE_3D;
		case core::ResourceType::TextureCube     : return VkImageViewType::VK_IMAGE_VIEW_TYPE_CUBE;
		case core::ResourceType::TextureCubeArray: return VkImageViewType::VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
		default:
			throw std::runtime_error("not supported resource type (vulkan api)");
	}
}
/*-------------------------------------------------------------------
-                        Descriptor heap type (resource view type) mode
---------------------------------------------------------------------*/
VkDescriptorType EnumConverter::Convert(const rhi::core::DescriptorHeapType heapType)
{
	switch (heapType)
	{
		case core::DescriptorHeapType::CBV    : return VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		case core::DescriptorHeapType::SRV    : return VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		case core::DescriptorHeapType::UAV    : return VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		case core::DescriptorHeapType::SAMPLER: return VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
		default:
			throw std::runtime_error("not supported heap type (vulkan api)" );
	}
}
#pragma region GPUTexture
/*-------------------------------------------------------------------
-                        Resource Usage type
---------------------------------------------------------------------*/
VkImageAspectFlags  EnumConverter::Convert(const rhi::core::PixelFormat format, const rhi::core::ResourceUsage usage)
{
	if (core::EnumHas(usage, core::ResourceUsage::DepthStencil))
	{
		if (core::PixelFormatSizeOf::IsDepthOnly(format)) { return VkImageAspectFlagBits::VK_IMAGE_ASPECT_DEPTH_BIT; }
		return VkImageAspectFlagBits::VK_IMAGE_ASPECT_DEPTH_BIT | VkImageAspectFlagBits::VK_IMAGE_ASPECT_STENCIL_BIT;
	}
	return VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
}
/*-------------------------------------------------------------------
-                       ImageViewType
---------------------------------------------------------------------*/
VkImageViewType EnumConverter::Convert(const rhi::core::ResourceDimension dimension, const rhi::core::ResourceType usage, const size_t length)
{
	switch (dimension)
	{
		case core::ResourceDimension::Dimension1D: return length == 1 ? VkImageViewType::VK_IMAGE_VIEW_TYPE_1D : VkImageViewType::VK_IMAGE_VIEW_TYPE_1D_ARRAY;
		case core::ResourceDimension::Dimension2D:
		{
			return  usage == core::ResourceType::TextureCube ? (length > 6 ? VkImageViewType::VK_IMAGE_VIEW_TYPE_CUBE_ARRAY : VkImageViewType::VK_IMAGE_VIEW_TYPE_CUBE) :
				(length == 1 ? VkImageViewType::VK_IMAGE_VIEW_TYPE_2D : VkImageViewType::VK_IMAGE_VIEW_TYPE_2D_ARRAY);
		}
		case core::ResourceDimension::Dimension3D: return VkImageViewType::VK_IMAGE_VIEW_TYPE_3D;
		default:
			throw std::runtime_error("not support image view type");
	}
}
/*-------------------------------------------------------------------
-                        Image Create Flags
---------------------------------------------------------------------*/
VkImageCreateFlags EnumConverter::Convert(const rhi::core::ResourceType type, const size_t arrayLength)
{
	auto result = VkImageCreateFlags(VkImageCreateFlagBits::VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT);       // Different resource view is able to be created.
	if (type == core::ResourceType::TextureCube || type == core::ResourceType::TextureCubeArray)
	{
		result |= VkImageCreateFlagBits::VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
	}
	else
	{
		if (arrayLength > 1)
		{
			result |= VkImageCreateFlagBits::VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT;
		}
	}
	return result;
}
#pragma endregion GPUTexture
#pragma region GPUBuffer

/*-------------------------------------------------------------------
-                        Memory Heap mode
---------------------------------------------------------------------*/
VkMemoryPropertyFlags  EnumConverter::Convert(const rhi::core::MemoryHeap memoryHeap)
{
	switch (memoryHeap)
	{
		case core::MemoryHeap::Default: return VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT; // not write in CPU
		case core::MemoryHeap::Upload : return VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			                                 | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		case core::MemoryHeap::Readback: return VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		default:
			throw std::runtime_error("not support Memory Heap type (vulkan api)");
	}
}
#pragma endregion GPUBuffer
#pragma endregion GPUResource
#pragma region Render Pass
/*-------------------------------------------------------------------
-                        Attachment load mode
---------------------------------------------------------------------*/
VkAttachmentLoadOp  EnumConverter::Convert(const rhi::core::AttachmentLoad load)
{
	switch (load)
	{
		case core::AttachmentLoad::Clear   : return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
		case core::AttachmentLoad::Load    : return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_LOAD;
		case core::AttachmentLoad::DontCare: return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		default:
			throw std::runtime_error("not support AttachmentLoad (vulkan api)");
	}
}
/*-------------------------------------------------------------------
-                        Attachment store mode
---------------------------------------------------------------------*/
VkAttachmentStoreOp EnumConverter::Convert(const rhi::core::AttachmentStore store)
{
	switch (store)
	{
		case core::AttachmentStore::Store: return VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
		case core::AttachmentStore::DontCare: return VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
		default:
			throw std::runtime_error("not support Attachment Store (vulkan api)");
	}
}
/*-------------------------------------------------------------------
-                        Image layout mode
---------------------------------------------------------------------*/
VkImageLayout EnumConverter::Convert(const rhi::core::ResourceState layout)
{
	switch (layout)
	{
		case core::ResourceState::Common         : return VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
		case core::ResourceState::GeneralRead    : return VkImageLayout::VK_IMAGE_LAYOUT_GENERAL;
		case core::ResourceState::UnorderedAccess: return VkImageLayout::VK_IMAGE_LAYOUT_GENERAL;
		case core::ResourceState::RenderTarget   : return VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		case core::ResourceState::DepthStencil   : return VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		case core::ResourceState::CopyDestination: return VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		case core::ResourceState::CopySource     : return VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		case core::ResourceState::Present        : return VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		default:
			throw std::runtime_error("not support imageresource layout");
			
	}
}
#pragma endregion Render Pass
#pragma region RayTracing
VkGeometryFlagsKHR EnumConverter::Convert(const rhi::core::RayTracingGeometryFlags flags)
{
	switch (flags)
	{
		case core::RayTracingGeometryFlags::None: return 0;
		case core::RayTracingGeometryFlags::Opaque: return VkGeometryFlagBitsKHR::VK_GEOMETRY_OPAQUE_BIT_KHR;
		case core::RayTracingGeometryFlags::NoDuplicateAnyHitInvocation: return VkGeometryFlagBitsKHR::VK_GEOMETRY_NO_DUPLICATE_ANY_HIT_INVOCATION_BIT_KHR;
		default:
		{
			throw std::runtime_error("not support geometry flags (vulkan api)");
		}
	}
}
VkGeometryInstanceFlagBitsKHR EnumConverter::Convert(const rhi::core::RayTracingInstanceFlags flags)
{
	switch (flags)
	{
		case core::RayTracingInstanceFlags::None: return (VkGeometryInstanceFlagBitsKHR)0;
		case core::RayTracingInstanceFlags::TriangleCullDisable          : return VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
		case core::RayTracingInstanceFlags::TriangleFrontCounterClockwise: return VK_GEOMETRY_INSTANCE_TRIANGLE_FRONT_COUNTERCLOCKWISE_BIT_KHR;
		case core::RayTracingInstanceFlags::ForceOpaque                  : return VK_GEOMETRY_INSTANCE_FORCE_OPAQUE_BIT_KHR;
		case core::RayTracingInstanceFlags::ForceNonOpaque               : return VK_GEOMETRY_INSTANCE_FORCE_NO_OPAQUE_BIT_KHR;
		default:
			throw std::runtime_error("not support instance flags (vulkan api)");

	}
}
VkBuildAccelerationStructureFlagsKHR EnumConverter::Convert(const rhi::core::BuildAccelerationStructureFlags flags)
{
	switch (flags)
	{
		case core::BuildAccelerationStructureFlags::None: return (VkBuildAccelerationStructureFlagBitsKHR)0;
		case core::BuildAccelerationStructureFlags::AllowUpdate: return VkBuildAccelerationStructureFlagBitsKHR::VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR;
		case core::BuildAccelerationStructureFlags::AllowCompaction: return VkBuildAccelerationStructureFlagBitsKHR::VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_KHR;
		case core::BuildAccelerationStructureFlags::MinimizeMemory: return VkBuildAccelerationStructureFlagBitsKHR::VK_BUILD_ACCELERATION_STRUCTURE_LOW_MEMORY_BIT_KHR;
		case core::BuildAccelerationStructureFlags::PreferFastBuild: return VkBuildAccelerationStructureFlagBitsKHR::VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_BUILD_BIT_KHR;
		case core::BuildAccelerationStructureFlags::PreferFastTrace: return VkBuildAccelerationStructureFlagBitsKHR::VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
		//case core::BuildAccelerationStructureFlags::PreformUpdate: return VkBuildAccelerationStructureFlagBitsKHR::VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_OPACITY_MICROMAP_DATA_UPDATE_EXT;
		default:
			throw std::runtime_error("not support build acceleration flags (vulkan api)");
	}
}
#pragma endregion RayTracing