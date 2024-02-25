//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUDescriptorHeap.hpp
///             @brief  GPU DescriptorHeap 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_DESCRIPTOR_HEAP_HPP
#define GPU_DESCRIPTOR_HEAP_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUSortedMap.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::core
{
	class RHIDevice;
	class RHIResourceLayout;
	class GPUResource;
	class GPUBuffer;
	/****************************************************************************
	*				  			RHIDescriptorHeap
	*************************************************************************//**
	*  @class     RHIDescriptorHeap
	*  @brief     Register descriptor view heap 
	*****************************************************************************/
	class RHIDescriptorHeap : public NonCopyable
	{
	protected:
		using MaxDescriptorSize = size_t;
		using DescriptorID      = std::uint32_t;
	public:
		enum class ResetFlag
		{
			All,        // íÜêgÇè¡Ç∑ (need reAllocate)
			OnlyOffset  // íÜêgÇè¡Ç≥Ç»Ç¢ (don't need reallocate)
		};
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Allocate view. Return descriptor index (only use resourceLayout in vulkan api : )*/
		virtual DescriptorID Allocate(const DescriptorHeapType heapType, const gu::SharedPointer<RHIResourceLayout>& resourceLayout) = 0;
		
		/* @brief : Free offset ID*/
		virtual void Free(const core::DescriptorHeapType heapType, const DescriptorID offsetIndex) = 0;

		/* @brief : Resize max view count size heap*/
		virtual void Resize(const DescriptorHeapType type, const size_t viewCount) = 0;
		/* @brief : Resize max view count size heap*/
		virtual void Resize(const gu::SortedMap<DescriptorHeapType, MaxDescriptorSize>& heapInfo) = 0;
		
		/* @brief : Reset view offset*/
		virtual void Reset(const ResetFlag flag = ResetFlag::OnlyOffset) = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		size_t GetMaxCount (const DescriptorHeapType type) const noexcept { return _heapInfo.Contains(type) ? _heapInfo.At(type) : 0; }
		
		// @brief: Return descriptor heap type (cbv, rtv, dsv)
		bool HasHeapType(const DescriptorHeapType desiredType) const noexcept { return _heapInfo.Contains(desiredType); }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		explicit RHIDescriptorHeap(const gu::SharedPointer<RHIDevice>& device) : _device(device){};
		
		virtual ~RHIDescriptorHeap();

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<RHIDevice> _device = nullptr;
		/* @brief : max total heap count (ex. CBV + SRV + UAV)*/
		MaxDescriptorSize _totalHeapCount = 0;
		/* @brief : max descriptor count in each descriptor heap type*/
		gu::SortedMap<DescriptorHeapType, MaxDescriptorSize> _heapInfo;
		static constexpr int INVALID_ID = -1;
	};
}


#endif