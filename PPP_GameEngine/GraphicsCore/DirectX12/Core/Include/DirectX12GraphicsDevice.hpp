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
#include "DirectX12Core.hpp"
#include "DirectX12Config.hpp"
#include <dxgiformat.h>
#include <d3d12.h>
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
enum class RenderTargetType
{
	BackBuffer0,
	BackBuffer1,
	BackBuffer2,
	ColorBuffer,
	Shadowing,
	CountOfRenderTarget
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
class GraphicsDeviceDirectX12
{
public:
	static const UINT32 FRAME_BUFFER_COUNT = 3;
	static const UINT32 VSYNC = 1; // 0: don't wait, 1:wait(60fps)
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void Initialize        (HWND hwnd);
	void OnResize          ();
	void Finalize          ();
	void ClearScreen       ();
	void CompleteInitialize();
	void CompleteRendering ();
	void FlushCommandQueue ();
	void ResetCommandList();
	void OnTerminateRenderScene();
	void CopyTextureToBackBuffer(ResourceComPtr& resource, D3D12_RESOURCE_STATES resourceState);
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	      DeviceComPtr&      GetDevice              ()                            { return _device; }
	const DeviceComPtr&      GetDevice              ()                      const { return _device; }
	      CommandListComPtr& GetCommandList         ()                            { return _commandList; }
	const CommandListComPtr& GetCommandList         ()                      const { return _commandList; }
	      CommandQueueComPtr& GetCommandQueue       ()                            { return _commandQueue; }
	const CommandQueueComPtr& GetCommandQueue       ()                      const { return _commandQueue; }
	      ResourceComPtr&    GetDepthStencil        ()                            { return _depthStencilBuffer; }
	const ResourceComPtr&    GetDepthStencil        ()                      const { return _depthStencilBuffer; }
	      ResourceComPtr&    GetCurrentRenderTarget ()                            { return _renderTargetList[_currentFrameIndex]; }
	const ResourceComPtr&    GetCurrentRenderTarget ()                      const { return _renderTargetList[_currentFrameIndex]; }
	      ResourceComPtr&    GetRenderTargetResource(RenderTargetType type)       { return _renderTargetList[(int)type]; }
	const ResourceComPtr&    GetRenderTargetResource(RenderTargetType type) const { return _renderTargetList[(int)type]; }
	      DescriptorHeapComPtr& GetCbvSrvUavHeap()       { return _cbvSrvUavHeap; }
	const DescriptorHeapComPtr& GetCbvSrvUavHeap() const { return _cbvSrvUavHeap; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDsvHeapStart      () const { return _dsvHeap.Get()->GetCPUDescriptorHandleForHeapStart(); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDsvHeapStart      () const { return _dsvHeap.Get()->GetGPUDescriptorHandleForHeapStart(); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPURtvHeapStart      () const { return _rtvHeap.Get()->GetCPUDescriptorHandleForHeapStart(); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPURtvHeapStart      () const { return _rtvHeap.Get()->GetGPUDescriptorHandleForHeapStart(); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUCbvSrvUavHeapStart() const { return _cbvSrvUavHeap.Get()->GetCPUDescriptorHandleForHeapStart(); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUCbvSrvUavHeapStart() const { return _cbvSrvUavHeap.Get()->GetGPUDescriptorHandleForHeapStart(); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUResourceView(HeapFlag heapFlag, int offsetIndex) const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUResourceView(HeapFlag heapFlag, int offsetIndex) const;

	D3D12_VIEWPORT GetViewport              () const { return _screenViewport; }
	D3D12_RECT     GetScissorRect           () const { return _scissorRect; }
	INT            GetCurrentFrameIndex     () const { return _currentFrameIndex; }
	INT            GetCurrentBackBufferIndex() const;
	DXGI_FORMAT    GetBackBufferRenderFormat() const { return _backBufferFormat; }
	INT  GetCbvSrvUavDescriptorHeapSize     () const { return _cbvSrvUavDescriptorSize; }
	UINT IssueViewID(HeapFlag heapFlag);
	void ResetViewID(HeapFlag heapFlag);
	
	bool Get4xMsaaState() const { return _4xMsaaState; }

	void SetHWND(HWND hwnd) { _hwnd = hwnd; }
	
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	static GraphicsDeviceDirectX12& Instance()
	{
		static GraphicsDeviceDirectX12 directX12;
		return directX12;
	}
	GraphicsDeviceDirectX12(const GraphicsDeviceDirectX12&)            = delete;
	GraphicsDeviceDirectX12& operator=(const GraphicsDeviceDirectX12&) = delete;
	GraphicsDeviceDirectX12(GraphicsDeviceDirectX12&&)                 = delete;
	GraphicsDeviceDirectX12& operator=(GraphicsDeviceDirectX12&&)      = delete;
private:
	GraphicsDeviceDirectX12() = default;
	
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
	void CreateViewport();
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
	/*-------------------------------------------------------------------
	-                        MSAA
	---------------------------------------------------------------------*/
	void CheckMultiSampleQualityLevels();
	/*-------------------------------------------------------------------
	-                        Tearing
	---------------------------------------------------------------------*/
	void CheckTearingSupport();
	/*-------------------------------------------------------------------
	-                 Variable Rate Shading
	---------------------------------------------------------------------*/
	void CheckVRSSupport();
	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	/*-------------------------------------------------------------------
	-                        Standard
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
	D3D12_VIEWPORT              _screenViewport;
	D3D12_RECT                 _scissorRect;
	UINT _rtvDescriptorSize       = 0;
	UINT _dsvDescriptorSize       = 0;
	UINT _cbvSrvUavDescriptorSize = 0;
	INT  _currentFrameIndex       = 0;
	UINT _variableRateShadingImageTileSize = 0;
	
	DXGI_FORMAT _backBufferFormat   = DXGI_FORMAT_R16G16B16A16_FLOAT;
	DXGI_FORMAT _depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	bool _hasInitialized = false;
	HWND _hwnd = nullptr;
	/*-------------------------------------------------------------------
	-                    Support
	---------------------------------------------------------------------*/
	bool _isWarpAdapter         = false;
	bool _isHDRSupport          = true;
	bool _isTearingSupport      = true;
	bool _enableRayTracing      = true;
	bool _isVariableRateShadingTier1Supported = true;
	bool _isVariableRateShadingTier2Supported = true;
	/*-------------------------------------------------------------------
	-                     Syncronization object
	---------------------------------------------------------------------*/
	UINT64      _currentFenceValue[FRAME_BUFFER_COUNT] = { 0,0 };
	FenceComPtr _fence;
	HANDLE      _fenceEvent = nullptr;
	/*-------------------------------------------------------------------
	-                     MSAA: One of the Anti-Alias
	---------------------------------------------------------------------*/
	bool _4xMsaaState = false;
	UINT _4xMsaaQuality = 0;
	/*-------------------------------------------------------------------
	-                     Heap config
	---------------------------------------------------------------------*/
	static constexpr int RTV_DESC_COUNT    = 1000;
	static constexpr int DSV_DESC_COUNT    = 100;
	static constexpr int CBV_DESC_COUNT    = 1024 * 10;
	static constexpr int UAV_DESC_COUNT    = 1024 * 10;
	static constexpr int SRV_DESC_COUNT    = 1024 * 10;
	static constexpr int MAX_SAMPLER_STATE = 16;
	
};

#endif