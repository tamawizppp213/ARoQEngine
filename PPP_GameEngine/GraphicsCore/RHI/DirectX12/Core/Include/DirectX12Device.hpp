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
		bool Create(HWND hwnd, HINSTANCE hInstance, bool useRaytracing = false) override;
		std::shared_ptr<core::RHIFence>            CreateFence() override;
		std::shared_ptr<core::RHICommandList>      CreateCommandList(const std::shared_ptr<core::RHICommandAllocator>& commandAllocator) override;
		std::shared_ptr<core::RHICommandQueue>     CreateCommandQueue() override;
		std::shared_ptr<core::RHICommandAllocator> CreateCommandAllocator() override;
		std::shared_ptr<core::RHISwapchain>        CreateSwapchain(const std::shared_ptr<core::RHICommandQueue>& commandQueue, const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat, const size_t frameBufferCount = 2, const std::uint32_t vsync = 0) override;
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
		void CheckMultiSampleQualityLevels();
	};
}
#endif