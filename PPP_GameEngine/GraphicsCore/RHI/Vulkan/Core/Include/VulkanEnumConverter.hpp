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
		static VkSamplerAddressMode   Convert(const rhi::core::TextureAddressingMode addressingMode);
		static VkFormat               Convert(const rhi::core::PixelFormat pixelFormat);
		static VkBlendOp              Convert(const rhi::core::BlendOperator blendOperator);
		static VkBlendFactor          Convert(const rhi::core::BlendFactor blendFactor);
		static VkColorComponentFlags  Convert(const rhi::core::ColorMask colorMask);

	};
}
#endif