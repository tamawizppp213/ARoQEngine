//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12EnumConverter.hpp
///             @brief  EnumConverter
///             @author Toide Yutaro
///             @date   2022_06_28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_ENUM_CONVERTER_HPP
#define VULKAN_ENUM_CONVERTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include <vulkan/vulkan.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	/****************************************************************************
	*				  			EnumConverter
	*************************************************************************//**
	*  @class     EnumConverter
	*  @brief     RHI::Core -> DirectX12
	*****************************************************************************/
	class EnumConverter
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
#pragma region ShaderState
		static VkShaderStageFlagBits  Convert(const rhi::core::ShaderType type);
#pragma endregion ShaderState
#pragma region SamplerState
		static VkFilter               Convert(const rhi::core::FilterOption filter, const rhi::core::FilterMask mask);
		static VkSamplerMipmapMode    Convert(const rhi::core::FilterOption filter);
		static VkSamplerAddressMode   Convert(const rhi::core::SamplerAddressMode addressMode);
		static VkBorderColor          Convert(const rhi::core::BorderColor borderColor);
		static VkSampleCountFlagBits  Convert(const rhi::core::MultiSample sample);
#pragma endregion SamplerState
#pragma region BlendState
		static VkFormat               Convert(const rhi::core::PixelFormat   pixelFormat);
		static VkBlendOp              Convert(const rhi::core::BlendOperator blendOperator);
		static VkBlendFactor          Convert(const rhi::core::BlendFactor   blendFactor);
		static VkColorComponentFlags  Convert(const rhi::core::ColorMask     colorMask);
#pragma endregion BlendState
#pragma region RasterizerState
		static VkPolygonMode          Convert(const rhi::core::FillMode    fillMode);
		static VkCullModeFlags        Convert(const rhi::core::CullingMode cullingMode);
		static VkFrontFace            Convert(const rhi::core::FrontFace   frontFace);
#pragma endregion RasterizerState
#pragma region DepthStencilState
		static VkCompareOp            Convert(const rhi::core::CompareOperator compareOperator);
		static VkStencilOp            Convert(const rhi::core::StencilOperator stencilOperator);
#pragma endregion DepthStencilState
#pragma region Input Layout
		static VkImageType            Convert(const rhi::core::ResourceDimension dimension);
		static VkImageViewType        Convert(const rhi::core::ResourceType type);
		static VkPrimitiveTopology    Convert(const rhi::core::PrimitiveTopology primitiveTopology);
#pragma endregion Input Layout
#pragma region GPUResource
		static VkDescriptorType       Convert(const rhi::core::DescriptorHeapType heapType);
#pragma region GPUBuffer
		static VkDescriptorType       Convert(const rhi::core::DescriptorType resourceType);
		static VkMemoryPropertyFlags  Convert(const rhi::core::MemoryHeap memoryHeap);
#pragma endregion GPUBuffer
#pragma endregion GPUResource
#pragma region RenderPass
		static VkAttachmentLoadOp  Convert(const rhi::core::AttachmentLoad load);
		static VkAttachmentStoreOp Convert(const rhi::core::AttachmentStore store);
		static VkImageLayout       Convert(const rhi::core::ResourceLayout layout);
#pragma endregion RenderPass
	};
}
#endif