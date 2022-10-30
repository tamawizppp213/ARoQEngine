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
		DescriptorHeapComPtr GetRTVHeap() const noexcept;
		DescriptorHeapComPtr GetDSVHeap() const noexcept;
		size_t               GetRTVByteSize() const noexcept { return _rtvSize; }
		size_t               GetDSVByteSize() const noexcept { return _dsvSize; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIFrameBuffer() = default;
		~RHIFrameBuffer();
		explicit RHIFrameBuffer(const std::shared_ptr<core::RHIDevice>& device, const std::shared_ptr<core::RHIRenderPass>& renderPass,  const std::shared_ptr<core::GPUTexture>& renderTarget, const std::shared_ptr<core::GPUTexture>& depthStencil);
		explicit RHIFrameBuffer(const std::shared_ptr<core::RHIDevice>& device, const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::vector<std::shared_ptr<core::GPUTexture>>&renderTargets, const std::shared_ptr<core::GPUTexture>& depthStencil);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<directX12::RHIDescriptorHeap> _renderTargetHeap = nullptr;
		std::shared_ptr<directX12::RHIDescriptorHeap> _depthStencilHeap = nullptr;
		size_t _rtvSize = 0;
		size_t _dsvSize = 0;
	private: 
		void Prepare();
	};
}
#endif