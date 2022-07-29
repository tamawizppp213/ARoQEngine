//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12DescriptorHeap.hpp
///             @brief  DescriptorHeap
///             @author Toide Yutaro
///             @date   2022_07_16
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_DESCRIPTOR_HEAP_HPP
#define DIRECTX12_DESCRIPTOR_HEAP_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIDescriptorHeap.hpp"
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
	*************************************************************************//**
	*  @class     GPUResource
	*  @brief     Resource (å„Ç≈NoncopyableÇ…ïœçXÇ∑ÇÈ)
	*****************************************************************************/
	class RHIDescriptorHeap : public rhi::core::RHIDescriptorHeap
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDescriptorHeap() = default;
		~RHIDescriptorHeap();
		explicit RHIDescriptorHeap(const std::shared_ptr<core::RHIDevice>& device, const core::DescriptorHeapType heapType, const size_t maxDescriptorCount);
		explicit RHIDescriptorHeap(const std::shared_ptr<core::RHIDevice>& device, const std::vector<core::DescriptorHeapType>& heapTypeList, const std::vector<size_t>& descriptorCounts);
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
		VkDescriptorPool   _descriptorPool = nullptr;
		static const std::uint32_t MAX_SET_COUNT = 128;
		//VkDescriptorType _descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

	};
}
#endif