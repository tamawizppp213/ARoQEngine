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
#include <string>
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
		void Load(
			[[maybe_unused]] const gu::tstring& filePath,
			[[maybe_unused]] const gu::SharedPointer<core::RHICommandList>& commandList) override;

		void Save(
			[[maybe_unused]]const gu::tstring& filePath, 
			[[maybe_unused]] const gu::SharedPointer<core::RHICommandList>& commandList,
			[[maybe_unused]] const gu::SharedPointer<core::RHICommandQueue>& commandQueue) override
		{ printf("Non Function\n"); }

		void Write([[maybe_unused]]const gu::SharedPointer<core::RHICommandList>& commandList,[[maybe_unused]] const gm::RGBA* pixel) override
		{
			printf("Non Function\n");
		}

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		VkImage GetImage() const noexcept     { return _image; }
		
		VkImageViewCreateInfo& GetImageInfo() { return _imageViewDesc; }
		
		const VkImageViewCreateInfo& GetImageInfo() const noexcept { return _imageViewDesc; }
		
		void SetName(const gu::tstring& name) override;
		
		// SetMetaData����������悢.

		/*!**********************************************************************
		*  @brief     �o�b�t�@�Ƃ��Ă�GPU���\�[�X���ǂ����𔻒肵�܂�.
		*************************************************************************/
		__forceinline virtual bool IsBuffer() const override { return false; }

		/*!**********************************************************************
		*  @brief     �e�N�X�`���Ƃ��Ă�GPU���\�[�X���ǂ����𔻒肵�܂�.
		*************************************************************************/
		__forceinline virtual bool IsTexture() const override { return true; }

		/*!**********************************************************************
		*  @brief     �����_��GPUResource�̈����� (IndexBuffer�Ƃ��Ďg�p����Ȃ�...)��ݒ肵�܂�
		*  @attention �蓮�ł̐؂�ւ��͊�{�I�ɍs��Ȃ��ł�������. (���̊֐��̓o���A�̎g�p��ړI�Ƃ��Ďg�p���܂�.)
		*  @return    void
		*************************************************************************/
		__forceinline virtual void SetResourceState(const core::ResourceState state) override { _metaData.State = state; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUTexture() = default;
		
		~GPUTexture();
		 
		explicit GPUTexture(const gu::SharedPointer<core::RHIDevice>& device, const gu::tstring& name = SP("Texture"));
		
		explicit GPUTexture(const gu::SharedPointer<core::RHIDevice>& device, const core::GPUTextureMetaData& metaData, const gu::tstring& name = SP("Texture"));
		
		explicit GPUTexture(const gu::SharedPointer<core::RHIDevice>& device, const core::GPUTextureMetaData& metaData, const VkImage image, const gu::tstring& name = SP("Texture"));

	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		
		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		VkDeviceMemory _memory    = nullptr;

		VkImage        _image     = nullptr;

		VkImageViewCreateInfo  _imageViewDesc = {};

		gu::SharedPointer<core::GPUBuffer> _stagingBuffer = nullptr;

		bool _hasAllocated = false;

	private: 
		void Prepare();

	};


}
#endif