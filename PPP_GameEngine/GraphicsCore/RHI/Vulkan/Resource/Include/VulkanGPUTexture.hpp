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
		void Load(const std::wstring& filePath, const std::shared_ptr<core::RHICommandList>& commandList) override { printf("Non Function\n"); };
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkImage GetImage() const noexcept { return _image; }
		VkImageViewCreateInfo& GetImageInfo() { return _imageViewDesc; }
		const VkImageViewCreateInfo& GetImageInfo() const noexcept { return _imageViewDesc; }
		
		void SetName(const std::wstring& name) override;
		
		// SetMetaDataçÏÇ¡ÇΩï˚Ç™ÇÊÇ¢.
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUTexture() = default;
		~GPUTexture();
		 
		explicit GPUTexture(const std::shared_ptr<core::RHIDevice>& device);
		explicit GPUTexture(const std::shared_ptr<core::RHIDevice>& device, const core::GPUTextureMetaData& metaData);
		explicit GPUTexture(const std::shared_ptr<core::RHIDevice>& device, const core::GPUTextureMetaData& metaData, const VkImage image);

	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void Pack(const std::shared_ptr<core::RHICommandList>& commandList) override{};
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