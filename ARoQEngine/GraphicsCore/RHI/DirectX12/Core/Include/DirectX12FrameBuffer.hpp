//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12FrameBuffer.hpp
///  @brief  レンダーターゲットとデプスステンシルのテクスチャやResourceViewを使用するクラスです.
///  @author Toide Yutaro
///  @date   2024_07_21
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_FRAME_BUFFER_HPP
#define DIRECTX12_FRAME_BUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include "DirectX12Core.hpp"
#include <dxgi1_6.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{
	class RHIDescriptorHeap;
	/****************************************************************************
	*				  			RHIFrameBuffer
	****************************************************************************/
	/* @brief  レンダーターゲットとデプスステンシルのテクスチャやResourceViewを使用するクラスです.
	*****************************************************************************/
	class RHIFrameBuffer : public rhi::core::RHIFrameBuffer
	{
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property

		/*!**********************************************************************
		*  @brief     Render Target Viewを格納するDirectX12のDescriptorHeapを返します.
		*  @param[in] void
		*  @return    DescriptorHeapComPtr Render Target Viewを格納するDirectX12のDescriptorHeap
		*************************************************************************/
		DescriptorHeapComPtr GetRTVHeap() const noexcept;

		/*!**********************************************************************
		*  @brief     Depth Stencil Viewを格納するDirectX12のDescriptorHeapを返します.
		*  @param[in] void
		*  @return    DescriptorHeapComPtr Depth Stencil Viewを格納するDirectX12のDescriptorHeap
		*************************************************************************/
		DescriptorHeapComPtr GetDSVHeap() const noexcept;

		/*!**********************************************************************
		*  @brief     Render Target Viewのバイトサイズを返します.
		*  @param[in] void
		*  @return    gu::uint32 Render Target Viewのバイトサイズ
		*************************************************************************/
		gu::uint32  GetRTVByteSize() const noexcept { return static_cast<gu::uint32>(_rtvByteSize); }

		/*!**********************************************************************
		*  @brief     Depth Stencil Viewのバイトサイズを返します.
		*  @param[in] void
		*  @return    gu::uint32Depth Stencil Viewのバイトサイズ
		*************************************************************************/
		gu::uint32  GetDSVByteSize() const noexcept { return static_cast<gu::uint32>(_dsvByteSize); }

		#pragma endregion

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		RHIFrameBuffer() = default;

		/*! @brief デストラクタ*/
		~RHIFrameBuffer();

		/*! @brief レンダーターゲットとデプスステンシルを使った初期化*/
		explicit RHIFrameBuffer(const gu::SharedPointer<core::RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass,  const gu::SharedPointer<core::GPUTexture>& renderTarget, const gu::SharedPointer<core::GPUTexture>& depthStencil);
		
		/*! @brief 一枚のレンダーターゲットとデプスステンシルを使った初期化*/
		explicit RHIFrameBuffer(const gu::SharedPointer<core::RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::DynamicArray<gu::SharedPointer<core::GPUTexture>>&renderTargets, const gu::SharedPointer<core::GPUTexture>& depthStencil);

		#pragma endregion
	
	protected:
		#pragma region Protected Function

		#pragma endregion

		#pragma region Protected Property
		/*! @brief Render Target Viewを格納するDirectX12のDescriptorHeap*/
		gu::SharedPointer<directX12::RHIDescriptorHeap> _renderTargetHeap = nullptr;

		/*! @brief Depth Stencil Viewを格納するDirectX12のDescriptorHeap*/
		gu::SharedPointer<directX12::RHIDescriptorHeap> _depthStencilHeap = nullptr;

		/*! @brief Render Target Viewのバイトサイズ*/
		gu::uint32 _rtvByteSize = 0;

		/*! @brief Depth Stencil Viewのバイトサイズ*/
		gu::uint32 _dsvByteSize = 0;
		#pragma endregion

	private: 
		#pragma region Private Function
		/*!**********************************************************************
		*  @brief     Descriptor Heapと各ビューを作成します. 
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void Prepare();
		#pragma endregion

	};
}
#endif