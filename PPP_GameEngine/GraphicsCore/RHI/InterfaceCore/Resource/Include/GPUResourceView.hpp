//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUResource.hpp
///             @brief  GPU Resource 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_RESOURCE_VIEW_HPP
#define GPU_RESOURCE_VIEW_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::core
{
	class RHIDevice;
	class RHICommandList;
	class GPUBuffer;
	class GPUTexture;
	class RHIDescriptorHeap;
	/****************************************************************************
	*				  			GPUResourceView
	*************************************************************************//**
	*  @class     GPUResourceView
	*  @brief     Resource View(å„Ç≈NoncopyableÇ…ïœçXÇ∑ÇÈ)
	*****************************************************************************/
	class GPUResourceView : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual void Bind(const std::shared_ptr<core::RHICommandList>& commandList, const std::uint32_t index) = 0;
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @brief: Return descriptor heap pointer. */
		std::shared_ptr<core::RHIDescriptorHeap> GetHeap() const noexcept { return _heap; };
		
		/* @brief: Return resource view type*/
		core::ResourceViewType GetResourceViewType() const noexcept { return _resourceViewType; }

		std::uint32_t GetDescriptorID() const { return _descriptorID; }

		std::shared_ptr<GPUTexture> GetTexture() const noexcept{ return _texture; }

		std::shared_ptr<GPUBuffer> GetBuffer() const noexcept { return _buffer; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUResourceView() = default;

		virtual ~GPUResourceView() = default;

		explicit GPUResourceView(const std::shared_ptr<RHIDevice>& device, const core::ResourceViewType type, const std::shared_ptr<core::RHIDescriptorHeap>& customHeap = nullptr) : _device(device), _resourceViewType(type), _heap(customHeap) {};
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice>  _device = nullptr;

		std::shared_ptr<GPUBuffer>  _buffer = nullptr;

		std::shared_ptr<GPUTexture> _texture = nullptr;

		std::shared_ptr<RHIDescriptorHeap> _heap = nullptr;

		core::ResourceViewType      _resourceViewType = core::ResourceViewType::Unknown;
		
		std::uint32_t _descriptorID = 0;
	};
}


#endif