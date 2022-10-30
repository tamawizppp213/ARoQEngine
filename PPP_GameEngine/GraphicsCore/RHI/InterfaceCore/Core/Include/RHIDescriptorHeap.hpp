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
#include <memory>
#include <map>
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
	*				  			GPUResource
	*************************************************************************//**
	*  @class     GPUResource
	*  @brief     Resource (後でNoncopyableに変更する)
	*****************************************************************************/
	class RHIDescriptorHeap : public NonCopyable
	{
	protected:
		using MaxDescriptorSize = size_t;
		using DescriptorID      = std::uint32_t;
	public:
		enum class ResetFlag
		{
			All,        // 中身を消す (need reAllocate)
			OnlyOffset  // 中身を消さない (don't need reallocate)
		};
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Allocate view. Return descriptor index (only use resourceLayout in vulkan api : )*/
		virtual DescriptorID Allocate(const DescriptorHeapType heapType, const std::shared_ptr<RHIResourceLayout>& resourceLayout) = 0;
		/* @brief : Resize max view count size heap*/
		virtual void Resize(const DescriptorHeapType type, const size_t viewCount) = 0;
		/* @brief : Resize max view count size heap*/
		virtual void Resize(const std::map<DescriptorHeapType, MaxDescriptorSize>& heapInfo) = 0;
		/* @brief : Reset view offset*/
		virtual void Reset(const ResetFlag flag = ResetFlag::OnlyOffset) = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		size_t GetMaxCount (const DescriptorHeapType type) const noexcept { return _heapInfo.find(type) != _heapInfo.end() ? _heapInfo.at(type) : 0; }
		// @brief: Return descriptor heap type (cbv, rtv, dsv)
		bool HasHeapType(const DescriptorHeapType desiredType) const noexcept { return _heapInfo.find(desiredType) != _heapInfo.end();; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		explicit RHIDescriptorHeap(const std::shared_ptr<RHIDevice>& device) : _device(device){};
		virtual ~RHIDescriptorHeap();

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice> _device = nullptr;
		MaxDescriptorSize _totalHeapCount = 0;
		std::map<DescriptorHeapType, MaxDescriptorSize> _heapInfo;
		static constexpr int INVALID_ID = -1;
	};
}


#endif