//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12FrameBuffer.hpp
///             @brief  Frame Buffer (render target and depth stencil)
///             @author Toide Yutaro
///             @date   2022_06_23
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
	*************************************************************************//**
	*  @class     RHIFrameBuffer
	*  @brief     Render and Depth Stencil Buffer
	*****************************************************************************/
	class RHIFrameBuffer : public rhi::core::RHIFrameBuffer
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @brief : Return render target view descriptor directX12 heap*/
		DescriptorHeapComPtr GetRTVHeap() const noexcept;

		/* @brief: Return depth stencil view descriptor directX12 heap*/
		DescriptorHeapComPtr GetDSVHeap() const noexcept;

		/* @brief: Return render target descriptor view byte size*/
		size_t               GetRTVByteSize() const noexcept { return _rtvSize; }

		/* @brief: Return depth stencil descriptor view byte size*/
		size_t               GetDSVByteSize() const noexcept { return _dsvSize; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIFrameBuffer() = default;

		~RHIFrameBuffer();

		explicit RHIFrameBuffer(const gu::SharedPointer<core::RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass,  const gu::SharedPointer<core::GPUTexture>& renderTarget, const gu::SharedPointer<core::GPUTexture>& depthStencil);
		
		explicit RHIFrameBuffer(const gu::SharedPointer<core::RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass, const std::vector<gu::SharedPointer<core::GPUTexture>>&renderTargets, const gu::SharedPointer<core::GPUTexture>& depthStencil);
	
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<directX12::RHIDescriptorHeap> _renderTargetHeap = nullptr;
		gu::SharedPointer<directX12::RHIDescriptorHeap> _depthStencilHeap = nullptr;
		size_t _rtvSize = 0;
		size_t _dsvSize = 0;
	private: 
		void Prepare();

	};
}
#endif