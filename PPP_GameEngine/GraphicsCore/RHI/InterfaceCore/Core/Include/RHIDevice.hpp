//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIDevice.hpp
///             @brief  Device
///             @author Toide Yutaro
///             @date   2022_06_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_DEVICE_HPP
#define RHI_DEVICE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "RHICommonState.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <memory>
#include <vector>
#include <optional>
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIFrameBuffer;
	class RHIFence;
	class RHICommandList;
	class RHICommandQueue;
	class RHICommandAllocator;
	class RHISwapchain;
	class RHIDescriptorHeap;
	class RHIRenderPass;
	class GPUSampler;
	class GPUBuffer;
	class GPUTexture;
	class GPUPipelineFactory;
	/****************************************************************************
	*				  			RHIDevice
	*************************************************************************//**
	*  @class     RHIDevice 
	*  @brief     Logical Device interface. Device : GPU resources generator.
	*****************************************************************************/
	class RHIDevice : public NonCopyable, public std::enable_shared_from_this<RHIDevice>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual bool Create(HWND hwnd, HINSTANCE hInstance, bool useHDR = false,  bool useRaytracing = false) = 0;
		virtual std::shared_ptr<RHIFrameBuffer>      CreateFrameBuffer(const std::vector<std::shared_ptr<GPUTexture>>& renderTargets, const std::shared_ptr<GPUTexture>& depthStencil = nullptr) = 0;
		virtual std::shared_ptr<RHIFrameBuffer>      CreateFrameBuffer(const std::shared_ptr<GPUTexture>& renderTarget, const std::shared_ptr<GPUTexture>& depthStencil = nullptr) = 0;
		virtual std::shared_ptr<RHIFence>            CreateFence() = 0;
		virtual std::shared_ptr<RHICommandList>      CreateCommandList(const std::shared_ptr<RHICommandAllocator>& commandAllocator) = 0;
		virtual std::shared_ptr<RHICommandQueue>     CreateCommandQueue() = 0;
		virtual std::shared_ptr<RHICommandAllocator> CreateCommandAllocator() = 0;
		virtual std::shared_ptr<RHISwapchain>        CreateSwapchain(const std::shared_ptr<RHICommandQueue>& commandQueue, const WindowInfo& windowInfo, const PixelFormat& pixelFormat, const size_t frameBufferCount = 2, const std::uint32_t vsync = 0) = 0;
		virtual std::shared_ptr<RHIDescriptorHeap>   CreateDescriptorHeap(const DescriptorHeapType heapType, const size_t maxDescriptorCount) = 0;
		virtual std::shared_ptr<RHIDescriptorHeap>   CreateDescriptorHeap(const std::vector<DescriptorHeapType>& heapTypes, const std::vector<size_t>& maxDescriptorCounts) = 0;
		virtual std::shared_ptr<GPUPipelineFactory>  CreatePipelineFactory() = 0;
		virtual std::shared_ptr<RHIRenderPass>       CreateRenderPass(const std::vector<Attachment>& colors, const std::optional<Attachment>& depth) = 0;
		virtual std::shared_ptr<RHIRenderPass>       CreateRenderPass(const Attachment& color, const std::optional<Attachment>& depth) = 0;
		virtual std::shared_ptr<GPUSampler>          CreateSampler(const core::SamplerInfo& samplerInfo) = 0; // both
		virtual std::shared_ptr<GPUBuffer>           CreateBuffer(const core::GPUBufferMetaData& metaData) = 0;
		virtual std::shared_ptr<GPUTexture>          CreateTexture(const core::GPUTextureMetaData& metaData) = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		APIVersion GetAPIVersion() const noexcept { return _apiVersion; }
		
		bool       IsHDRSupport () const noexcept { return _isHDRSupport; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
	protected:
		~RHIDevice() = default;
		RHIDevice(APIVersion version = APIVersion::Unknown) { _apiVersion = version; }
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		APIVersion _apiVersion  = APIVersion::Unknown;
		HWND       _hwnd        = nullptr;
		HINSTANCE _hInstance    = nullptr;
		bool      _isInitialize = false;

		/*setting*/
		bool      _isHDRSupport = false;
	};
}
#endif