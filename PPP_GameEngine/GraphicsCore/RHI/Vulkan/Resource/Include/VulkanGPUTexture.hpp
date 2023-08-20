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
	*				  			GPUTexture
	*************************************************************************//**
	*  @class     GPUTexture
	*  @brief     Texture 
	*****************************************************************************/
	class GPUTexture : public core::GPUTexture
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Load(const std::wstring& filePath, const std::shared_ptr<core::RHICommandList>& commandList) override;

		void Save(const std::wstring& filePath, const std::shared_ptr<core::RHICommandList>& commandList, const std::shared_ptr<core::RHICommandQueue>& commandQueue) override 
		{ printf("Non Function\n"); }

		void Write(const std::shared_ptr<core::RHICommandList>& commandList, const gm::RGBA* pixel) override
		{
			printf("Non Function\n");
		}

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkImage GetImage() const noexcept     { return _image; }
		
		VkImageViewCreateInfo& GetImageInfo() { return _imageViewDesc; }
		
		const VkImageViewCreateInfo& GetImageInfo() const noexcept { return _imageViewDesc; }
		
		void SetName(const std::wstring& name) override;
		
		// SetMetaDataçÏÇ¡ÇΩï˚Ç™ÇÊÇ¢.
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUTexture() = default;
		
		~GPUTexture();
		 
		explicit GPUTexture(const std::shared_ptr<core::RHIDevice>& device, const std::wstring& name = L"Texture");
		
		explicit GPUTexture(const std::shared_ptr<core::RHIDevice>& device, const core::GPUTextureMetaData& metaData, const std::wstring& name = L"Texture");
		
		explicit GPUTexture(const std::shared_ptr<core::RHIDevice>& device, const core::GPUTextureMetaData& metaData, const VkImage image, const std::wstring& name = L"Texture");

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

		std::shared_ptr<core::GPUBuffer> _stagingBuffer = nullptr;

		bool _hasAllocated = false;

	private: 
		void Prepare();

	};


}
#endif