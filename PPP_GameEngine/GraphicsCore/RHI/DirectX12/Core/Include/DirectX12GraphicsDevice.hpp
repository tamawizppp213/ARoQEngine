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
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIGraphicsDevice.hpp"
#include "DirectX12BaseStruct.hpp"
#include "DirectX12GPUResource.hpp"
#include <dxgiformat.h>
#include <array>

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
	Sampler = 0x0020
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
class GraphicsDeviceDirectX12 : public rhi::core::RHIGraphicsDevice
{
	using StaticSamplerArray = std::array<const STATIC_SAMPLER_DESC, 6>;
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void StartUp        (HWND hwnd, HINSTANCE hInstance) override;
	void OnResize          () override;
	void ShutDown          () override;
	void BeginDrawFrame    () override;
	void CompleteInitialize();
	void EndDrawFrame () override;
	void FlushCommandQueue ();
	void ResetCommandList();
	void OnTerminateRenderScene();
	void CopyTextureToBackBuffer(ResourceComPtr& resource, D3D12_RESOURCE_STATES resourceState);
	void IsEnabledHDR();
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	inline       IDevice*      GetDevice              ()                           { return _device.Get(); }
	inline const IDevice*      GetDevice              ()                     const { return _device.Get(); }
	inline       ICommandList* GetCommandList         ()                           { return _commandList.Get(); }
	inline const ICommandList* GetCommandList         ()                     const { return _commandList.Get(); }
	inline      ICommandQueue* GetCommandQueue       ()                           { return _commandQueue.Get(); }
	inline const ICommandQueue* GetCommandQueue       ()                     const { return _commandQueue.Get(); }
	inline       Resource*    GetDepthStencil        ()                            { return _depthStencilBuffer.Get(); }
	inline const Resource*    GetDepthStencil        ()                      const { return _depthStencilBuffer.Get(); }
	inline       GPUResource* GetCurrentRenderTarget ()                            { return &_renderTargetList[_currentFrameIndex]; }
	inline const GPUResource*    GetCurrentRenderTarget ()                      const { return &_renderTargetList[_currentFrameIndex]; }
	inline       Resource*    GetRenderTargetResource(RenderTargetType type)       { return _renderTargetList[(int)type].GetResource(); }
	inline const Resource*    GetRenderTargetResource(RenderTargetType type) const { return _renderTargetList[(int)type].GetResource(); }
	inline      IDescriptorHeap* GetCbvSrvUavHeap()       { return _cbvSrvUavHeap.Get(); }
	inline const IDescriptorHeap* GetCbvSrvUavHeap() const { return _cbvSrvUavHeap.Get(); }
	inline D3D12_GRAPHICS_PIPELINE_STATE_DESC GetDefaultPSOConfig() const { return _defaultPSODesc; }
	inline D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDsvHeapStart      () const { return _dsvHeap.Get()->GetCPUDescriptorHandleForHeapStart(); }
	inline D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDsvHeapStart      () const { return _dsvHeap.Get()->GetGPUDescriptorHandleForHeapStart(); }
	inline D3D12_CPU_DESCRIPTOR_HANDLE GetCPURtvHeapStart      () const { return _rtvHeap.Get()->GetCPUDescriptorHandleForHeapStart(); }
	inline D3D12_GPU_DESCRIPTOR_HANDLE GetGPURtvHeapStart      () const { return _rtvHeap.Get()->GetGPUDescriptorHandleForHeapStart(); }
	inline D3D12_CPU_DESCRIPTOR_HANDLE GetCPUCbvSrvUavHeapStart() const { return _cbvSrvUavHeap.Get()->GetCPUDescriptorHandleForHeapStart(); }
	inline D3D12_GPU_DESCRIPTOR_HANDLE GetGPUCbvSrvUavHeapStart() const { return _cbvSrvUavHeap.Get()->GetGPUDescriptorHandleForHeapStart(); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUResourceView(HeapFlag heapFlag, int offsetIndex) const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUResourceView(HeapFlag heapFlag, int offsetIndex) const;

	inline D3D12_VIEWPORT GetViewport              () const { return _screenViewport; }
	inline D3D12_RECT     GetScissorRect           () const { return _scissorRect; }
	inline INT            GetCurrentFrameIndex     () const { return _currentFrameIndex; }
	INT            GetCurrentBackBufferIndex() const;
	inline DXGI_FORMAT    GetBackBufferRenderFormat() const { return _backBufferFormat; }
	inline DXGI_FORMAT    GetDepthStencilViewFormat() const { return _depthStencilFormat; }
	inline INT         GetCbvSrvUavDescriptorHeapSize     () const { return _cbvSrvUavDescriptorSize; }
	StaticSamplerArray GetStaticSamplers();
	UINT IssueViewID(HeapFlag heapFlag);
	void ResetViewID(HeapFlag heapFlag);
	inline bool Get4xMsaaState() const { return _4xMsaaState; }
	inline UINT Get4xMsaaQuality() const { return _4xMsaaQuality; }
	inline void SetHWND(HWND hwnd) { _hwnd = hwnd; }
	
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	GraphicsDeviceDirectX12();
	~GraphicsDeviceDirectX12();
	GraphicsDeviceDirectX12(const GraphicsDeviceDirectX12&)            = delete;
	GraphicsDeviceDirectX12& operator=(const GraphicsDeviceDirectX12&) = delete;
	GraphicsDeviceDirectX12(GraphicsDeviceDirectX12&&)                 = default;
	GraphicsDeviceDirectX12& operator=(GraphicsDeviceDirectX12&&)      = default;
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
	void CreateDefaultPSO();
	void CreateViewport();
	void BuildRenderTargetView();
	void BuildDepthStencilView();


	/*-------------------------------------------------------------------
	-                        Debug
	---------------------------------------------------------------------*/
	void EnabledDebugLayer();
	void EnabledGPUBasedValidation();
	void LogAdapters     ();
	void LogAdapterOutputs(IAdapter* adapter);
	void LogOutputDisplayModes(IOutput* output, DXGI_FORMAT format);
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
	DeviceComPtr                 _device       = nullptr;        /// Device 
	FactoryComPtr                _dxgiFactory  = nullptr;        /// DXGI
	AdapterComPtr                _useAdapter   = nullptr;
	SwapchainComPtr              _swapchain    = nullptr;        /// SwapChain
	CommandQueueComPtr           _commandQueue = nullptr;        /// Command Queue (Command Execution Unit)
	CommandListComPtr            _commandList  = nullptr;        /// Graphics Command List
	CommandAllocatorComPtr       _commandAllocator[FRAME_BUFFER_COUNT];
	DescriptorHeapComPtr         _rtvHeap            = nullptr;      /// Heap For Render Target View 
	DescriptorHeapComPtr         _dsvHeap            = nullptr;      /// Heap For Depth Stencil View
	DescriptorHeapComPtr         _cbvSrvUavHeap      = nullptr;      /// Heap For Constant Buffer View
	DescriptorHeapComPtr         _samplerHeap        = nullptr;
	PipelineStateComPtr          _pipelineState      = nullptr;      /// Graphic Pipeline State
	ResourceComPtr               _depthStencilBuffer = nullptr; /// DepthStencl Buffer   
	GPUResource                  _renderTargetList[FRAME_BUFFER_COUNT];
	ResourceAllocator*           _rtvAllocator     = nullptr;
	ResourceAllocator*           _dsvAllocator     = nullptr;
	ResourceAllocator*           _cbvAllocator     = nullptr;
	ResourceAllocator*           _srvAllocator     = nullptr;
	ResourceAllocator*           _uavAllocator     = nullptr;
	ResourceAllocator*           _samplerAllocator = nullptr;
	D3D12_VIEWPORT              _screenViewport;
	D3D12_RECT                 _scissorRect;
	UINT _rtvDescriptorSize       = 0; // Dependency: GPU type
	UINT _dsvDescriptorSize       = 0; // Dependency: GPU type
	UINT _cbvSrvUavDescriptorSize = 0; // Dependency: GPU type
	UINT _samplerDescriptorSize   = 0; // Dependency: GPU type
	INT  _currentFrameIndex       = 0; // Dependency: GPU type
	UINT _variableRateShadingImageTileSize = 0;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC _defaultPSODesc = D3D12_GRAPHICS_PIPELINE_STATE_DESC();

	DXGI_FORMAT _backBufferFormat   = DXGI_FORMAT_R16G16B16A16_FLOAT;
	DXGI_FORMAT _depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DXGI_SWAP_CHAIN_FLAG _swapchainFlag;

	bool _hasInitialized = false;
	
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
	FenceComPtr _fence      = nullptr;
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
	static constexpr UINT SINGLE_GPU = 0;
};

#endif