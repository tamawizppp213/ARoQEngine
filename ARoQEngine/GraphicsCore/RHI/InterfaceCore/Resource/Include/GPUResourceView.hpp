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
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
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
	****************************************************************************/
	/* @class     GPUResourceView
	*  @brief     Specify the access mode to resource. 
	*             Use the bind function to bind resource layout array index to the command list. 
	*****************************************************************************/
	class GPUResourceView : public gu::NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : Bind resource layout array index to the command list.
		            index : resource layout array index
		/*----------------------------------------------------------------------*/
		virtual void Bind(const gu::SharedPointer<core::RHICommandList>& commandList, const gu::uint32 index, const gu::SharedPointer<RHIResourceLayout>& layout = nullptr) = 0;
		
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : Return the descriptor heap pointer.
		/*----------------------------------------------------------------------*/
		gu::SharedPointer<core::RHIDescriptorHeap> GetHeap() const noexcept { return _heap; };
		
		/*----------------------------------------------------------------------
		*  @brief : Return the resource view type
		/*----------------------------------------------------------------------*/
		core::ResourceViewType GetResourceViewType() const noexcept { return _resourceViewType; }

		/*----------------------------------------------------------------------
		*  @brief : Return the descriptor index in the descriptor heap
		/*----------------------------------------------------------------------*/
		gu::uint32 GetDescriptorID() const { return _descriptorID; }

		/*----------------------------------------------------------------------
		*  @brief : テクスチャ配列において, 同一のmipmapレベルのテクスチャをまとめるためのIndex
		https://learn.microsoft.com/ja-jp/windows/win32/direct3d12/subresources
		/*----------------------------------------------------------------------*/
		gu::uint32 GetMipSlice() const { return _mipSlice; }

		/*----------------------------------------------------------------------
		*  @brief : Return the plane slice index (使い道が現状不明なので今後調べる必要あり)
		https://learn.microsoft.com/ja-jp/windows/win32/direct3d12/subresources
		/*----------------------------------------------------------------------*/
		gu::uint32 GetPlaneSlice() const { return _planeSlice; }

		/*----------------------------------------------------------------------
		*  @brief : Return texture pointer or nullptr.
		/*----------------------------------------------------------------------*/
		gu::SharedPointer<GPUTexture> GetTexture() const noexcept{ return _texture; }

		/*----------------------------------------------------------------------
		*  @brief : Return buffer pointer or nullptr. 
		/*----------------------------------------------------------------------*/
		gu::SharedPointer<GPUBuffer> GetBuffer() const noexcept { return _buffer; }

		/*----------------------------------------------------------------------
		*  @brief : Set texture pointer. (本来バッファ用途であるなら使用しないでください)
		/*----------------------------------------------------------------------*/
		void SetTexture(const gu::SharedPointer<GPUTexture>& texture) { _texture = texture; }

		/*----------------------------------------------------------------------
		*  @brief : Set buffer pointer. (本来テクスチャ用途であるなら使用しないでください)
		/*----------------------------------------------------------------------*/
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

		explicit GPUResourceView(const gu::SharedPointer<RHIDevice>& device, const core::ResourceViewType type,
			const gu::uint32 mipSlice = 0, const gu::uint32 planeSlice = 0,
			const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap = nullptr) : _device(device), _resourceViewType(type), _heap(customHeap), _mipSlice(mipSlice), _planeSlice(planeSlice)
		{
		};
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		// @brief : GPU logical device
		gu::SharedPointer<RHIDevice>  _device = nullptr;

		// @brief : buffer pointer
		gu::SharedPointer<GPUBuffer>  _buffer = nullptr;

		// @brief : texture pointer
		gu::SharedPointer<GPUTexture> _texture = nullptr;

		// @brief : descriptor heap pointer
		gu::SharedPointer<RHIDescriptorHeap> _heap = nullptr;

		// @brief : resource view type
		core::ResourceViewType      _resourceViewType = core::ResourceViewType::Unknown;
		
		// @brief : descriptor index in the descriptor heap array
		gu::uint32 _descriptorID = 0;

		// @brief : specified mip map level in the texture array
		gu::uint32 _mipSlice = 0;

		// @brief : plane slice index
		gu::uint32 _planeSlice = 0;
	};
}


#endif