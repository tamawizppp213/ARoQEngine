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
	class RHIResourceLayout;
	/****************************************************************************
	*				  			GPUResourceView
	*************************************************************************//**
	*  @class     GPUResourceView
	*  @brief     Specify the access mode to resource. 
	*             Use the bind function to bind resource layout array index to the command list. 
	*****************************************************************************/
	class GPUResourceView : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Bind resource layout array index to the command list.
		            index : resource layout array index*/
		virtual void Bind(const std::shared_ptr<core::RHICommandList>& commandList, const std::uint32_t index, const std::shared_ptr<RHIResourceLayout>& layout = nullptr) = 0;
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @brief: Return descriptor heap pointer. */
		std::shared_ptr<core::RHIDescriptorHeap> GetHeap() const noexcept { return _heap; };
		
		/* @brief: Return resource view type*/
		core::ResourceViewType GetResourceViewType() const noexcept { return _resourceViewType; }

		/* @brief: Return descriptorID in the descriptor heap. */
		std::uint32_t GetDescriptorID() const { return _descriptorID; }

		/* @brief: Return texture pointer or nullptr */
		std::shared_ptr<GPUTexture> GetTexture() const noexcept{ return _texture; }

		/* @brief : Return buffer pointer or nullptr*/
		std::shared_ptr<GPUBuffer> GetBuffer() const noexcept { return _buffer; }

		/* @brief : Set texture*/
		void SetTexture(const std::shared_ptr<GPUTexture>& texture) { _texture = texture; }

		/* @brief : Set buffer*/
		void SetBuffer(const std::shared_ptr<GPUBuffer>& buffer) { _buffer = buffer; }

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