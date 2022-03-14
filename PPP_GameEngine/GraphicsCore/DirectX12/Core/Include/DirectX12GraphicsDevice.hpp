//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GrahicsDevice.hpp
///             @brief  Grahics Device for DirectX12
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GRAPHICS_DEVICE_HPP
#define DIRECTX12_GRAPHICS_DEVICE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/Interface/Include/IGraphicsDevice.hpp"
#include "DirectX12Core.hpp"
#include <dxgiformat.h>
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
enum class HeapFlag
{
	RTV = 0x0001,
	DSV = 0x0002,
	CBV = 0x0004,
	SRV = 0x0008,
	UAV = 0x0010,
};
class ResourceAllocator;
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  		 GraphicsDeviceDirectX12
*************************************************************************//**
*  @class     GraphicsDeviceDirectX12
*  @brief     DirectX12
*****************************************************************************/
class GraphicsDeviceDirectX12 : public IGraphicsDevice
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void Initialize        (HWND hwnd) override;
	void OnResize          () override;
	void Finalize          () override;
	void ClearScreen       () override;
	void CompleteInitialize() override;
	void CompleteRendering () override;
	void FlushCommandQueue () override;

	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	void LoadPipeline();
	void LoadAssets();
	/*-------------------------------------------------------------------
	-                        Initialize
	---------------------------------------------------------------------*/
	void CreateDevice();
	void CreateCommandObject();
	void CreateSwapChain();
	void CreateAllDescriptorHeap();
	void CreateDescriptorHeap(HeapFlag heapFlag);
	void BuildAllResourceAllocator();
	void BuildResourceAllocator(int heapFlag);
	void CreatePSOs();
	void BuildRenderTargetView();
	void BuildDepthStencilView();


	/*-------------------------------------------------------------------
	-                        Debug
	---------------------------------------------------------------------*/
	void EnabledDebugLayer();
	void EnabledGPUBasedValidation();
	void LogAdapters     ();
	void LogAdapterOutputs(Adapter* adapter);
	void LogOutputDisplayModes(Output* output, DXGI_FORMAT format);
	void ReportLiveObjects();
	/*-------------------------------------------------------------------
	-                        HDR
	---------------------------------------------------------------------*/
	void EnsureSwapChainColorSpace();
	bool CheckHDRDisplaySupport();
	void SetHDRMetaData();
	/*-------------------------------------------------------------------
	-                        DXR
	---------------------------------------------------------------------*/
	void CheckDXRSupport();
	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	/*-------------------------------------------------------------------
	-                        Debug
	---------------------------------------------------------------------*/
	DeviceComPtr                 _device;             /// Device
	FactoryComPtr                _dxgiFactory;        /// DXGI
	AdapterComPtr                _useAdapter;
	SwapchainComPtr              _swapchain;          /// SwapChain
	CommandQueueComPtr           _commandQueue;       /// Command Queue (Command Execution Unit)
	CommandListComPtr            _commandList;        /// Graphics Command List
	CommandAllocatorComPtr       _commandAllocator[FRAME_BUFFER_COUNT];
	DescriptorHeapComPtr         _rtvHeap;            /// Heap For Render Target View 
	DescriptorHeapComPtr         _dsvHeap;            /// Heap For Depth Stencil View
	DescriptorHeapComPtr         _cbvSrvUavHeap;      /// Heap For Constant Buffer View
	ResourceComPtr               _depthStencilBuffer; /// DepthStencl Buffer   
	ResourceComPtr               _renderTargetList[FRAME_BUFFER_COUNT];
	ResourceAllocator*           _rtvAllocator;
	ResourceAllocator*           _dsvAllocator;
	ResourceAllocator*           _cbvAllocator;
	ResourceAllocator*           _srvAllocator;
	ResourceAllocator*           _uavAllocator;

	UINT _rtvDescriptorSize       = 0;
	UINT _dsvDescriptorSize       = 0;
	UINT _cbvSrvUavDescriptorSize = 0;
	INT  _currentFrameIndex       = 0;
	
	DXGI_FORMAT _backBufferFormat   = DXGI_FORMAT_R16G16B16A16_FLOAT;
	DXGI_FORMAT _depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	bool _isWarpAdapter    = false;
	bool _isHDRSupport     = true;
	bool _enableRayTracing = true;

	// MSAA: One of the Anti-Alias
	bool _4xMsaaState = false;
	UINT _4xMsaaQuality = 0;

	static constexpr int RTV_DESC_COUNT    = 1000;
	static constexpr int DSV_DESC_COUNT    = 100;
	static constexpr int CBV_DESC_COUNT    = 1024 * 10;
	static constexpr int UAV_DESC_COUNT    = 1024 * 10;
	static constexpr int SRV_DESC_COUNT    = 1024 * 10;
	static constexpr int MAX_SAMPLER_STATE = 16;
};

#endif