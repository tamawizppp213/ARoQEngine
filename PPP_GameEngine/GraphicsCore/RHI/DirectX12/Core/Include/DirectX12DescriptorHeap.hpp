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
#include "DirectX12ResourceAllocator.hpp"
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
	*				  			RHIDescriptorHeap
	*************************************************************************//**
	*  @class     RHIDescriptorHeap
	*  @brief     Descriptor Heap (manage resource view) 
	*****************************************************************************/
	class RHIDescriptorHeap : public rhi::core::RHIDescriptorHeap
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		DescriptorID Allocate(const core::DescriptorHeapType heapType, const std::shared_ptr<core::RHIResourceLayout>& resourceLayout = nullptr) override;
		/* @brief : Resize max view count size heap*/
		void Resize(const std::map<core::DescriptorHeapType, MaxDescriptorSize>& heapInfo) override;
		/* @brief : Resize max view count size heap*/
		void Resize(const core::DescriptorHeapType type, const size_t viewCount) override;
		/* @brief : Reset view offset*/
		void Reset(const ResetFlag flag = ResetFlag::OnlyOffset) override;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @brief : Return directX12 cpu pointer handler*/
		inline CPU_DESC_HANDLER GetCPUDescHandler(const core::DescriptorHeapType type, const std::uint32_t offsetIndex = 0) 
		{ 
			return _resourceAllocators.at(type).GetCPUDescHandler(offsetIndex); 
		}
		/* @brief : Return directX12 gpu virtual pointer handler*/
		inline GPU_DESC_HANDLER GetGPUDescHandler(const core::DescriptorHeapType type, const std::uint32_t offsetIndex = 0)
		{
			return _resourceAllocators.at(type).GetGPUDescHandler(offsetIndex);
		}
		/* brief : Return Descriptor Heap pointer (COM)*/
		inline DescriptorHeapComPtr GetHeap() const noexcept { return _descriptorHeap; }
		inline size_t GetDescriptorByteSize() const noexcept { return _descriptorByteSize; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDescriptorHeap() = default;
		~RHIDescriptorHeap();
		explicit RHIDescriptorHeap(const std::shared_ptr<core::RHIDevice>& device);

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
		size_t               _descriptorByteSize = 0;
		std::map<core::DescriptorHeapType, ResourceAllocator> _resourceAllocators;
	
	private:
		bool CheckCorrectViewConbination(const std::map<core::DescriptorHeapType, MaxDescriptorSize>& heapInfos);
	};
}
#endif