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
	/****************************************************************************
	*				  			RHIFence
	*************************************************************************//**
	*  @class     RHIFence
	*  @brief     CPU-GPU synchronization
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
		DescriptorHeapComPtr GetRTVHeap() const noexcept { return _renderTargetHeap; }
		DescriptorHeapComPtr GetDSVHeap() const noexcept { return _depthStencilHeap; }
		size_t               GetRTVSize() const noexcept { return _rtvSize; }
		size_t               GetDSVSize() const noexcept { return _dsvSize; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIFrameBuffer() = default;
		~RHIFrameBuffer() = default;
		explicit RHIFrameBuffer(const std::shared_ptr<core::RHIDevice>& device, const std::shared_ptr<core::GPUTexture>& renderTarget, const std::shared_ptr<core::GPUTexture>& depthStencil);
		explicit RHIFrameBuffer(const std::shared_ptr<core::RHIDevice>& device, const std::vector<std::shared_ptr<core::GPUTexture>>& renderTargets, const std::shared_ptr<core::GPUTexture>& depthStencil);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		DescriptorHeapComPtr _renderTargetHeap = nullptr;
		DescriptorHeapComPtr _depthStencilHeap = nullptr;
		size_t _rtvSize = 0;
		size_t _dsvSize = 0;
	private: 
		void Prepare();
	};
}
#endif