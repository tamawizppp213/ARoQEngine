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
#include <vector>
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
	*  @brief     Resource (å„Ç≈NoncopyableÇ…ïœçXÇ∑ÇÈ)
	*****************************************************************************/
	class RHIDescriptorHeap : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		//void    BindGPUResource(const std::shared_ptr<GPUResource>& resource, const size_t index);
		//virtual void BindBuffer(const std::shared_ptr<GPUBuffer>& buffer, const size_t index) = 0;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @brief: Return max descriptor heap count*/
		size_t GetCount(int index = 0) const noexcept { return _maxDescriptorCounts[index]; }
		/* @brief: Return gpu resource layout */
		std::shared_ptr<RHIResourceLayout> GetResourceLayout() const noexcept { return _resourceLayout; }
		// @brief: Return descriptor heap type (cbv, rtv, dsv)
		DescriptorHeapType GetHeapType(int index = 0) const noexcept { return _heapTypeLists[index]; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDescriptorHeap() = default;
		virtual ~RHIDescriptorHeap();
		explicit RHIDescriptorHeap(const std::shared_ptr<RHIDevice>& device, const DescriptorHeapType heapType, const size_t maxDescriptorCount);
		/* only vulkan constructor*/
		explicit RHIDescriptorHeap(const std::shared_ptr<RHIDevice>& device, const std::vector<DescriptorHeapType>& heapTypeList, const std::vector<size_t>& maxDescriptorCounts);

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice>         _device         = nullptr;
		std::shared_ptr<RHIResourceLayout> _resourceLayout = nullptr;
		std::vector<DescriptorHeapType> _heapTypeLists;
		std::vector<size_t> _maxDescriptorCounts;
	};
}


#endif