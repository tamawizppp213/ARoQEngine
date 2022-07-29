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
#include "DirectX12Core.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::directX12
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
		DescriptorHeapComPtr GetHeap() const noexcept { return _descriptorHeap; }
		size_t     GetDescriptorSize() const noexcept { return _descriptorSize; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDescriptorHeap() = default;
		~RHIDescriptorHeap();
		explicit RHIDescriptorHeap(const std::shared_ptr<core::RHIDevice>& device, const core::DescriptorHeapType heapType, const size_t maxDescriptorCount);
		explicit RHIDescriptorHeap(const std::shared_ptr<core::RHIDevice>& device, const std::vector<core::DescriptorHeapType>& heapTypes, const std::vector<size_t>& maxDescriptorCounts);

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
		DescriptorHeapComPtr _descriptorHeap = nullptr;
		size_t _descriptorSize = 0;

	};
}
#endif