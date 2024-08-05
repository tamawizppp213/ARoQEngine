//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanDescriptorHeap.hpp
///             @brief  DescriptorHeap
///             @author Toide Yutaro
///             @date   2022_10_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_DESCRIPTOR_HEAP_HPP
#define VULKAN_DESCRIPTOR_HEAP_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIDescriptorHeap.hpp"
#include "VulkanResourceAllocator.hpp"
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
	*				  			GPUResource
	****************************************************************************/
	/* @class     GPUResource
	*  @brief     Resource (å„Ç≈NoncopyableÇ…ïœçXÇ∑ÇÈ)
	*****************************************************************************/
	class RHIDescriptorHeap : public rhi::core::RHIDescriptorHeap
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Allocate view. Return descriptor index*/
		DescriptorID Allocate(const core::DescriptorHeapType heapType, const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override;
		
		/* @brief : Free offset ID*/
		void Free(const core::DescriptorHeapType heapType, const DescriptorID offsetIndex) override;

		/* @brief : Allocate max view count size heap*/
		void Resize(const gu::SortedMap<core::DescriptorHeapType, MaxDescriptorSize>& heapInfo) override;
		
		/* @brief : Allocate max view count size heap*/
		void Resize(const core::DescriptorHeapType type, const size_t viewCount) override;
		
		/* @brief : Reset view offset*/
		void Reset(const ResetFlag flag = ResetFlag::OnlyOffset) override;
		
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		VkDescriptorSet GetDescriptorSet(DescriptorID id = 0) 
		{ 
			return _resourceAllocator.GetDescriptorSet(id); 
		}
		
		VkDescriptorPool GetVkDescriptorPool() const{ return _descriptorPool; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDescriptorHeap() = default;
		
		~RHIDescriptorHeap();
		
		explicit RHIDescriptorHeap(const gu::SharedPointer<core::RHIDevice>& device);
	
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
		VkDescriptorPool   _descriptorPool = nullptr;
		ResourceAllocator _resourceAllocator;
	};
}
#endif