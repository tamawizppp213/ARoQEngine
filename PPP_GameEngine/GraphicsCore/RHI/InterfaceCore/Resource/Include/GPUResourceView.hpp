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
#include "GameUtility/Base/Include/GUSharedPointer.hpp"
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
		virtual void Bind(const gu::SharedPointer<core::RHICommandList>& commandList, const std::uint32_t index, const gu::SharedPointer<RHIResourceLayout>& layout = nullptr) = 0;
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @brief: Return descriptor heap pointer. */
		gu::SharedPointer<core::RHIDescriptorHeap> GetHeap() const noexcept { return _heap; };
		
		/* @brief: Return resource view type*/
		core::ResourceViewType GetResourceViewType() const noexcept { return _resourceViewType; }

		/* @brief: Return descriptorID in the descriptor heap. */
		std::uint32_t GetDescriptorID() const { return _descriptorID; }

		/* @brief: Return texture pointer or nullptr */
		gu::SharedPointer<GPUTexture> GetTexture() const noexcept{ return _texture; }

		/* @brief : Return buffer pointer or nullptr*/
		gu::SharedPointer<GPUBuffer> GetBuffer() const noexcept { return _buffer; }

		/* @brief : Set texture*/
		void SetTexture(const gu::SharedPointer<GPUTexture>& texture) { _texture = texture; }

		/* @brief : Set buffer*/
		void SetBuffer(const gu::SharedPointer<GPUBuffer>& buffer) { _buffer = buffer; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUResourceView() = default;

		virtual ~GPUResourceView() = default;

		explicit GPUResourceView(const gu::SharedPointer<RHIDevice>& device, const core::ResourceViewType type, const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap = nullptr) : _device(device), _resourceViewType(type), _heap(customHeap) {};
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<RHIDevice>  _device = nullptr;

		gu::SharedPointer<GPUBuffer>  _buffer = nullptr;

		gu::SharedPointer<GPUTexture> _texture = nullptr;

		gu::SharedPointer<RHIDescriptorHeap> _heap = nullptr;

		core::ResourceViewType      _resourceViewType = core::ResourceViewType::Unknown;
		
		std::uint32_t _descriptorID = 0;
	};
}


#endif