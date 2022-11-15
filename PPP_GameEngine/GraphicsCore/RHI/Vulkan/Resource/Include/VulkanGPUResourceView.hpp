//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPUBuffer.hpp
///             @brief  GPU Buffer 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_GPU_RESOURCE_VIEW_HPP
#define VULKAN_GPU_RESOURCE_VIEW_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include <vulkan/vulkan.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	class RHIDescriptorHeap;
	/****************************************************************************
	*				  			GPUBuffer
	*************************************************************************//**
	*  @class     GPUBuffer
	*  @brief     Buffer
	*****************************************************************************/
	class GPUResourceView : public core::GPUResourceView
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Bind(const std::shared_ptr<core::RHICommandList>& commandList, const std::uint32_t index) override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkImageView  GetImageView () const noexcept{ return _imageView; }
		VkBufferView GetBufferView() const noexcept { return _bufferView; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUResourceView() = default;
		~GPUResourceView();
		explicit GPUResourceView(const std::shared_ptr<core::RHIDevice>& device, const core::ResourceViewType type, const std::shared_ptr<core::GPUBuffer>& buffer, const std::shared_ptr<core::RHIDescriptorHeap>& customHeap = nullptr);
		explicit GPUResourceView(const std::shared_ptr<core::RHIDevice>& device, const core::ResourceViewType type, const std::shared_ptr<core::GPUTexture>& texture, const std::shared_ptr<core::RHIDescriptorHeap>& customHeap = nullptr);
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void CreateView();
		void CreateImageView();
		void CreateBufferView();

		const std::shared_ptr<vulkan::RHIDescriptorHeap> SelectDescriptorHeap(const core::ResourceViewType type);
		VkImageAspectFlags GetImageAspectFlags(VkFormat format);
		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		VkImageView _imageView   = VK_NULL_HANDLE;
		VkBufferView _bufferView = VK_NULL_HANDLE;
		std::uint32_t _heapOffset = 0;
		
	};
}
#endif