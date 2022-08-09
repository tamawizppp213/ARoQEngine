//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanGPUSampler.hpp
///             @brief  Sampler State 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_GPU_TEXTURE_HPP
#define Vulkan_GPU_TEXTURE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUTexture.hpp"
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
	*				  			GPUBuffer
	*************************************************************************//**
	*  @class     GPUBuffer
	*  @brief     Buffer
	*****************************************************************************/
	class GPUTexture : public core::GPUTexture
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkImage GetImage() const noexcept { return _image; }
		VkImageViewCreateInfo& GetImageInfo() { return _imageViewDesc; }
		const VkImageViewCreateInfo& GetImageInfo() const noexcept { return _imageViewDesc; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUTexture() = default;
		~GPUTexture();
		explicit GPUTexture(const std::shared_ptr<core::RHIDevice>& device, const core::GPUTextureMetaData& metaData);
		explicit GPUTexture(const std::shared_ptr<core::RHIDevice>& device, const core::GPUTextureMetaData& metaData, const VkImage image);

	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkDeviceMemory _memory    = nullptr;
		VkImage        _image     = nullptr;
		VkImageViewCreateInfo  _imageViewDesc = {};
	private: 
		void Prepare();
	};


}
#endif