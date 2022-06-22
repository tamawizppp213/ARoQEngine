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
	*  @brief     temp
	*****************************************************************************/
	class RHIDevice : public core::RHIDevice
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool Create(HWND hwnd, HINSTANCE hInstance, bool useRaytracing = false) override;
		std::shared_ptr<core::RHIFence>            CreateFence();
		std::shared_ptr<core::RHICommandList>      CreateCommandList();
		std::shared_ptr<core::RHICommandQueue>     CreateCommandQueue();
		std::shared_ptr<core::RHICommandAllocator> CreateCommandAllocator();
		std::shared_ptr<core::RHISwapchain>        CreateSwapchain();
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		DeviceComPtr GetDevice() const noexcept { return _device; }
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
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void EnabledDebugLayer();
		void SearchHardwareAdapter();
		void LogAdapters();
		void LogAdapterOutputs(IAdapter* adapter);
		void LogOutputDisplayModes(IOutput* output, DXGI_FORMAT format);
		void ReportLiveObjects();
		void CheckDXRSupport();
	};
}
#endif