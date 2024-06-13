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
	****************************************************************************/
	/* @class     GPUBuffer
	*  @brief     Buffer
	*****************************************************************************/
	class GPUResourceView : public core::GPUResourceView
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Bind(const gu::SharedPointer<core::RHICommandList>& commandList, const std::uint32_t index, const gu::SharedPointer<core::RHIResourceLayout>& layout = nullptr) override;
		
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		VkImageView  GetImageView () const noexcept{ return _imageView; }
		
		VkBufferView GetBufferView() const noexcept { return _bufferView; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUResourceView() = default;
		
		~GPUResourceView();
		
		explicit GPUResourceView(const gu::SharedPointer<core::RHIDevice>& device, const core::ResourceViewType type, const gu::SharedPointer<core::GPUBuffer>& buffer, const gu::uint32 mipSlice = 0, const gu::uint32 planeSlice = 0, const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap = nullptr);
		
		explicit GPUResourceView(const gu::SharedPointer<core::RHIDevice>& device, const core::ResourceViewType type, const gu::SharedPointer<core::GPUTexture>& texture, const gu::uint32 mipSlice = 0, const gu::uint32 planeSlice = 0, const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap = nullptr);
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void CreateView();
		void CreateImageView();
		void CreateBufferView();

		const gu::SharedPointer<vulkan::RHIDescriptorHeap> SelectDescriptorHeap(const core::ResourceViewType type);

		VkImageAspectFlags GetImageAspectFlags(const VkFormat format);
		/****************************************************************************
		**                Private Property
		*****************************************************************************/
		VkImageView _imageView   = VK_NULL_HANDLE;

		VkBufferView _bufferView = VK_NULL_HANDLE;

		bool _calledCreateBufferView = false;

		int _heapOffset = INVALID_ID;
		
		rhi::core::DescriptorHeapType _heapType;

		static constexpr int INVALID_ID = -1;

	};
}
#endif