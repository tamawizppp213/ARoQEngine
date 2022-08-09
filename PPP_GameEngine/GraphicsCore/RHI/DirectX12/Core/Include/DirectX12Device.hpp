//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12Device.hpp
///             @brief  Device
///             @author Toide Yutaro
///             @date   2022_06_21
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_DEVICE_HPP
#define DIRECTX12_DEVICE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIDevice.hpp"
#include "DirectX12Core.hpp"
#include <dxgiformat.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                          Device class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{
	/****************************************************************************
	*				  			Device class
	*************************************************************************//**
	*  @class     Device
	*  @brief     Logical Device : shared_ptrÇ≈ä«óùÇ∑ÇÈÇ±Ç∆
	*****************************************************************************/
	class RHIDevice : public core::RHIDevice
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool Create(HWND hwnd, HINSTANCE hInstance, bool useHDR = false, bool useRaytracing = false) override;
		std::shared_ptr<core::RHIFrameBuffer>      CreateFrameBuffer(const std::vector<std::shared_ptr<core::GPUTexture>>& renderTargets, const std::shared_ptr<core::GPUTexture>& depthStencil = nullptr) override;
		std::shared_ptr<core::RHIFrameBuffer>      CreateFrameBuffer(const std::shared_ptr<core::GPUTexture>& renderTarget, const std::shared_ptr<core::GPUTexture>& depthStencil = nullptr) override;
		std::shared_ptr<core::RHIFence>            CreateFence() override;
		std::shared_ptr<core::RHICommandList>      CreateCommandList(const std::shared_ptr<core::RHICommandAllocator>& commandAllocator) override;
		std::shared_ptr<core::RHICommandQueue>     CreateCommandQueue() override;
		std::shared_ptr<core::RHICommandAllocator> CreateCommandAllocator() override;
		std::shared_ptr<core::RHISwapchain>        CreateSwapchain(const std::shared_ptr<core::RHICommandQueue>& commandQueue, const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat, const size_t frameBufferCount = 2, const std::uint32_t vsync = 0) override;
		std::shared_ptr<core::RHIDescriptorHeap>   CreateDescriptorHeap(const core::DescriptorHeapType heapType, const size_t maxDescriptorCount) override;
		std::shared_ptr<core::RHIDescriptorHeap>   CreateDescriptorHeap(const std::vector<core::DescriptorHeapType>& heapTypes, const std::vector<size_t>& maxDescriptorCounts) override;
		std::shared_ptr<core::RHIRenderPass>       CreateRenderPass(const std::vector<core::Attachment>& colors, const std::optional<core::Attachment>& depth) override;
		std::shared_ptr<core::RHIRenderPass>       CreateRenderPass(const core::Attachment& color, const std::optional<core::Attachment>& depth) override;
		std::shared_ptr<core::GPUPipelineFactory>  CreatePipelineFactory() override;
		std::shared_ptr<core::GPUSampler>          CreateSampler(const core::SamplerInfo& samplerInfo); // both
		std::shared_ptr<core::GPUBuffer>           CreateBuffer(const core::GPUBufferMetaData& metaData) override;
		std::shared_ptr<core::GPUTexture>          CreateTexture(const core::GPUTextureMetaData& metaData) override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		DeviceComPtr  GetDevice () const noexcept { return _device; }
		FactoryComPtr GetFactory() const noexcept { return _dxgiFactory; }
		AdapterComPtr GetAdapter         () const noexcept { return _useAdapter; }
		DXGI_FORMAT   GetBackBufferFormat() const noexcept { return _backBufferFormat; }
		bool          IsTearingSupport   () const noexcept { return _isTearingSupport; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDevice();
		~RHIDevice();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		DeviceComPtr  _device        = nullptr;
		FactoryComPtr _dxgiFactory   = nullptr;
		AdapterComPtr _useAdapter    = nullptr;
		bool          _isWarpAdapter = true;
		DXGI_FORMAT   _backBufferFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;
		bool          _enableRayTracing = false;
		bool          _isTearingSupport = true;
		bool          _isVariableRateShadingTier1Supported = true;
		bool          _isVariableRateShadingTier2Supported = true;
		std::uint32_t _variableRateShadingImageTileSize = 0;
		std::uint32_t _4xMsaaQuality = 0;
		/* @brief : Use debug mode. GPU validation. ÉtÉåÅ[ÉÄÉåÅ[ÉgÇ…ëÂÇ´Ç»âeãøÇó^Ç¶Ç‹Ç∑)*/
		bool          _enableGPUBasedValidation = false;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void EnabledDebugLayer();
		void EnabledGPUBasedValidation();
		void SearchHardwareAdapter();
		void LogAdapters();
		void LogAdapterOutputs(IAdapter* adapter);
		void LogOutputDisplayModes(IOutput* output, DXGI_FORMAT format);
		void ReportLiveObjects();
		void CheckDXRSupport();
		void CheckTearingSupport();
		void CheckVRSSupport();
		void CheckHDRDisplaySupport();
		void CheckMultiSampleQualityLevels();
	};
}
#endif