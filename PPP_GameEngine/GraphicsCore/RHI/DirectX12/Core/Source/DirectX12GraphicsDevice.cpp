//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12GraphicsDevice.cpp
///             @brief  Graphics Device
///             @author Toide Yutaro
///             @date   -
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12GraphicsDevice.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Include.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12ResourceAllocator.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12VertexTypes.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12RootSignature.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12PipelineState.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include "GameUtility/Math/Include/GMColor.hpp"
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace Microsoft::WRL;
enum class GPUVender
{
	NVidia,
	AMD,
	Intel,
	CountOfGPUVender
};

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GraphicsDeviceDirectX12::GraphicsDeviceDirectX12()
{
	printf("DirectX12Started\n");
}
GraphicsDeviceDirectX12::~GraphicsDeviceDirectX12()
{
	printf("DirectX12Finished\n");
}
#pragma region Public Function
/****************************************************************************
*							Initialize
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::Initialize(void)
*  @brief     Initialize Back Screen
*  @param[in] HWND hwnd
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::Initialize(HWND hwnd)
{
	if (_hasInitialized) { return; }
	SetHWND(hwnd);
	LoadPipeline();
	LoadAssets();
	_hasInitialized = true;
}
/****************************************************************************
*							Finalize
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::Finalize()
*  @brief     Release DirectX12 All Memory
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::Finalize()
{
	RootSignature::DestroyAll();
	PipelineState::DestroyAll();
	if (_rtvAllocator)     { delete _rtvAllocator; }
	if (_dsvAllocator)     { delete _dsvAllocator; }
	if (_cbvAllocator)     { delete _cbvAllocator; }
	if (_srvAllocator)     { delete _srvAllocator; }
	if (_uavAllocator)     { delete _uavAllocator; }
	if (_samplerAllocator) { delete _samplerAllocator; }

	if (_depthStencilBuffer) { _depthStencilBuffer = nullptr;}
	for (auto& renderTarget : _renderTargetList) 
	{ 
		renderTarget.Destroy();
	}
	if (_pipelineState) { _pipelineState= nullptr;}
	if (_rtvHeap      ) { _rtvHeap      = nullptr; }
	if (_dsvHeap      ) { _dsvHeap      = nullptr; }
	if (_cbvSrvUavHeap) { _cbvSrvUavHeap=nullptr; }
	if (_cbvSrvUavHeap) { _samplerHeap  = nullptr; }
	if (_useAdapter   ) { _useAdapter   = nullptr; }
	if (_swapchain    ) { _swapchain    = nullptr;}
	for (auto& allocator : _commandAllocator) 
	{
		if (allocator) { allocator = nullptr; }
	}
	if (_fence)        { _fence       = nullptr;}
	if (_fenceEvent)   { _fenceEvent = nullptr; }
	if (_commandQueue) { _commandQueue= nullptr;}
	if (_commandList ) { _commandList = nullptr;}
	if (_dxgiFactory ) { _dxgiFactory = nullptr;}

#ifdef _DEBUG
	ReportLiveObjects();
#endif
	_hwnd = nullptr;

	if (_device) { _device = nullptr; }

	
}
/****************************************************************************
*							OnResize
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::OnResize(void)
*  @brief     Use when doing screen resizing
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::OnResize()
{
	/*-------------------------------------------------------------------
	-              Nullptr check
	---------------------------------------------------------------------*/
#ifdef _DEBUG
	assert(_device);
	assert(_swapchain);
	assert(_commandAllocator);
#endif
	/*-------------------------------------------------------------------
	-              Flush before chainging any resources
	---------------------------------------------------------------------*/
	FlushCommandQueue();
	ThrowIfFailed(_commandList->Reset(_commandAllocator[_currentFrameIndex].Get(), nullptr));
	
	/*-------------------------------------------------------------------
	-              Release the previous resources
	---------------------------------------------------------------------*/
	for (int i = 0; i <FRAME_BUFFER_COUNT; ++i)
	{
		_renderTargetList[i].Destroy();
	}
	_depthStencilBuffer.Reset();

	/*-------------------------------------------------------------------
	-                   Resize Swapchain
	---------------------------------------------------------------------*/
	ThrowIfFailed(_swapchain->ResizeBuffers(
		FRAME_BUFFER_COUNT,
		Screen::GetScreenWidth(),
		Screen::GetScreenHeight(),
		_backBufferFormat,
		_swapchainFlag));	
	_currentFrameIndex = 0;

	CreateViewport();
	BuildRenderTargetView();
	BuildDepthStencilView();
	CompleteInitialize();
	FlushCommandQueue();
	
}
/****************************************************************************
*                     ClearScreen
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::ClearScreen(void)
*  @brief     Clear Screen. Describe in the first of Draw functions for each scene.
*             The background color is SteelBlue.
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::ClearScreen()
{
	/*-------------------------------------------------------------------
	-               Start recording commands (移動FlushCommandQueueに移動)
	---------------------------------------------------------------------*/
	ResetCommandList(); // start recording

	/*-------------------------------------------------------------------
	-       Indicate a state transition (Present -> Render Target)
	---------------------------------------------------------------------*/
	auto barrier = BARRIER::Transition(GetCurrentRenderTarget()->GetResource(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	GetCurrentRenderTarget()->TransitionState(D3D12_RESOURCE_STATE_RENDER_TARGET);
	_commandList->ResourceBarrier(1, &barrier);

	/*-------------------------------------------------------------------
	-          Set the viewport and scissor rect. 
	-  This needs to be reset whenever the command list is reset.
	---------------------------------------------------------------------*/
	_commandList->RSSetViewports(1, &_screenViewport);
	_commandList->RSSetScissorRects(1, &_scissorRect);
	/*-------------------------------------------------------------------
	-               Clear the back buffer and depth buffer.
	---------------------------------------------------------------------*/
	// rtv : current frame, clear color : steel blue, not use rects, all render target clear = nullptr
	_commandList->ClearRenderTargetView(_rtvAllocator->GetCPUDescHandler(_currentFrameIndex), gm::color::SteelBlue, 0, nullptr);
	_commandList->ClearDepthStencilView(_dsvAllocator->GetCPUDescHandler(0), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	/*-------------------------------------------------------------------
	-                   Set Render Target
	---------------------------------------------------------------------*/
	auto rtv = _rtvAllocator->GetCPUDescHandler(_currentFrameIndex);
	auto dsv = _dsvAllocator->GetCPUDescHandler(0);

	// Descriptor handle : 1, rtv, Single handle to descriptor range, dsv
	_commandList->OMSetRenderTargets(1, &rtv, FALSE, &dsv);
	/*-------------------------------------------------------------------
	-                   Set Descriptor Heaps
	---------------------------------------------------------------------*/
	ID3D12DescriptorHeap* heapList[] = { _cbvSrvUavHeap.Get() };
	_commandList->SetDescriptorHeaps(_countof(heapList), heapList);
}

/****************************************************************************
*                     CompleteRendering
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::CompleteRendering(void)
*  @brief     End of the drawing process (describe in the last of
*             Draw functions for each scene)
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::CompleteRendering()
{
	/*-------------------------------------------------------------------
	-      // Indicate a state transition (Render Target -> Present)
	---------------------------------------------------------------------*/
	auto barrier = BARRIER::Transition(GetCurrentRenderTarget()->GetResource(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	GetCurrentRenderTarget()->TransitionState(D3D12_RESOURCE_STATE_PRESENT);
	_commandList->ResourceBarrier(1, &barrier);

	/*-------------------------------------------------------------------
	-      Finish recording commands list
	---------------------------------------------------------------------*/
	ThrowIfFailed(_commandList->Close());

	/*-------------------------------------------------------------------
	-          Add command list to the queue for execution
	---------------------------------------------------------------------*/
	ID3D12CommandList* commandLists[] = { _commandList.Get() };
	_commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists); // Send command list contents to command queue 

	/*-------------------------------------------------------------------
	-						Flip screen
	---------------------------------------------------------------------*/
	ThrowIfFailed(_swapchain->Present(VSYNC, 0));
	_currentFrameIndex = (_currentFrameIndex + 1) % FRAME_BUFFER_COUNT;

	FlushCommandQueue(); // Flush command queue
}

void GraphicsDeviceDirectX12::ResetCommandList()
{
	// Start recording commands.
	ThrowIfFailed(_commandAllocator[_currentFrameIndex]->Reset());
	ThrowIfFailed(_commandList->Reset(_commandAllocator[_currentFrameIndex].Get(), nullptr));
}
/****************************************************************************
*                     CompleteRendering
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::CompleteRendering(void)
*  @brief     End of the drawing process (describe in the last of
*             Draw functions for each scene)
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::CopyTextureToBackBuffer(ResourceComPtr& resource, D3D12_RESOURCE_STATES resourceState)
{
	BARRIER before[] =
	{
		BARRIER::Transition(GetCurrentRenderTarget()->GetResource(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_DEST),
		BARRIER::Transition(resource.Get(),
		resourceState, D3D12_RESOURCE_STATE_COPY_SOURCE),
	};
	GetCurrentRenderTarget()->TransitionState(D3D12_RESOURCE_STATE_COPY_DEST);
	_commandList->ResourceBarrier(_countof(before), before);
	_commandList->CopyResource(GetCurrentRenderTarget()->GetResource(), resource.Get());
	BARRIER after[] =
	{
		BARRIER::Transition(GetCurrentRenderTarget()->GetResource(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_RENDER_TARGET),
		BARRIER::Transition(resource.Get(),
		D3D12_RESOURCE_STATE_COPY_SOURCE, resourceState)
	};
	GetCurrentRenderTarget()->TransitionState(D3D12_RESOURCE_STATE_RENDER_TARGET);
	_commandList->ResourceBarrier(_countof(after), after);
}
/****************************************************************************
*                     FlushCommandQueue
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::FlushCommandQueue(void)
*  @brief     Flush Command Queue
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::FlushCommandQueue()
{
	++_currentFenceValue[_currentFrameIndex];
	ThrowIfFailed(_commandQueue->Signal(_fence.Get(), _currentFenceValue[_currentFrameIndex]));

	/*-------------------------------------------------------------------
	-   Wait until the GPU has completed commands up to this fence point
	---------------------------------------------------------------------*/
	if (_fence->GetCompletedValue() < _currentFenceValue[_currentFrameIndex])
	{
		_fenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
		ThrowIfFailed(_fence->SetEventOnCompletion(_currentFenceValue[_currentFrameIndex], _fenceEvent));

		if (_fenceEvent != 0)
		{
			WaitForSingleObject(_fenceEvent, INFINITE);
			CloseHandle(_fenceEvent);
		}

	}
	_currentFrameIndex = _swapchain->GetCurrentBackBufferIndex();

}
void GraphicsDeviceDirectX12::OnTerminateRenderScene()
{
	FlushCommandQueue();
}
/****************************************************************************
*							CompleteInitialize
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::CompleteInitialize(void)
*  @brief     Finish Initialize
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::CompleteInitialize()
{
	ThrowIfFailed(_commandList->Close());// Finish recording commands

	/*-------------------------------------------------------------------
	-          Add command list to the queue for execution
	---------------------------------------------------------------------*/
	ID3D12CommandList* commandLists[] = { _commandList.Get() };
	_commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

	/*-------------------------------------------------------------------
	-						Flip screen
	---------------------------------------------------------------------*/
	ThrowIfFailed(_swapchain->Present(VSYNC, 0));
	_currentFrameIndex = (_currentFrameIndex + 1) % FRAME_BUFFER_COUNT;

}
#pragma region Property
/****************************************************************************
*                        GetCPUResourceView
*************************************************************************//**
*  @fn        D3D12_CPU_DESCRIPTOR_HANDLE  GraphicsDeviceDirectX12::GetCPUResourceView(HeapType heapType, int offsetIndex) const
*  @brief     Get current frame constant buffer view pointer
*  @param[in] int offsetIndex
*  @return 　　D3D12_CPU_DESCRIPTOR_HANDLE
*****************************************************************************/
D3D12_CPU_DESCRIPTOR_HANDLE  GraphicsDeviceDirectX12::GetCPUResourceView(HeapFlag heapFlag, int offsetIndex) const
{
	switch (heapFlag)
	{
		case HeapFlag::RTV:
			return _rtvAllocator->GetCPUDescHandler(offsetIndex);
		case HeapFlag::DSV:
			return _dsvAllocator->GetCPUDescHandler(offsetIndex);
		case HeapFlag::CBV:
			return _cbvAllocator->GetCPUDescHandler(offsetIndex);
		case HeapFlag::SRV:
			return _srvAllocator->GetCPUDescHandler(offsetIndex);
		case HeapFlag::UAV:
			return _uavAllocator->GetCPUDescHandler(offsetIndex);
		default:
			return _samplerAllocator->GetCPUDescHandler(offsetIndex);
	}
}

/****************************************************************************
*                        GetGPUResourceView
*************************************************************************//**
*  @fn        D3D12_GPU_DESCRIPTOR_HANDLE DirectX12::GetGPUResourceView(HeapType heapType, int offsetIndex) const
*  @brief     Get current frame constant buffer view pointer
*  @param[in] int offsetIndex
*  @return 　　D3D12_GPU_DESCRIPTOR_HANDLE
*****************************************************************************/
D3D12_GPU_DESCRIPTOR_HANDLE GraphicsDeviceDirectX12::GetGPUResourceView(HeapFlag heapType, int offsetIndex) const
{
	switch (heapType)
	{
		case HeapFlag::RTV:
			return _rtvAllocator->GetGPUDescHandler(offsetIndex);
		case HeapFlag::DSV:
			return _dsvAllocator->GetGPUDescHandler(offsetIndex);
		case HeapFlag::CBV:
			return _cbvAllocator->GetGPUDescHandler(offsetIndex);
		case HeapFlag::SRV:
			return _srvAllocator->GetGPUDescHandler(offsetIndex);
		case HeapFlag::UAV:
			return _uavAllocator->GetGPUDescHandler(offsetIndex);
		default:
			return _samplerAllocator->GetGPUDescHandler(offsetIndex);
	}
	
}
/****************************************************************************
*                        GetGPUResourceView
*************************************************************************//**
*  @fn        void DirectX12::GetGPUResourceView(HeapType heapType, int offsetIndex) const
*  @brief     Get current frame constant buffer view pointer
*  @param[in] int offsetIndex
*  @return 　　D3D12_GPU_DESCRIPTOR_HANDLE
*****************************************************************************/
void GraphicsDeviceDirectX12::ResetViewID(HeapFlag heapType)
{
	switch (heapType)
	{
		case HeapFlag::RTV:
			return _rtvAllocator->ResetID((UINT)RenderTargetType::CountOfRenderTarget);
		case HeapFlag::DSV:
			return _dsvAllocator->ResetID(1);
		case HeapFlag::CBV:
			return _cbvAllocator->ResetID();
		case HeapFlag::SRV:
			return _srvAllocator->ResetID();
		case HeapFlag::UAV:
			return _uavAllocator->ResetID();
		default:
			return _samplerAllocator->ResetID();
	}
}
/****************************************************************************
*                        GetCurrentBackBufferIndex
*************************************************************************//**
*  @fn        INT GraphicsDeviceDirectX12::GetCurrentBackBufferIndex() const
*  @brief     Get Current BackBufferIndex
*  @param[in] void
*  @return 　　INT
*****************************************************************************/
INT GraphicsDeviceDirectX12::GetCurrentBackBufferIndex() const
{
	if (_currentFrameIndex != FRAME_BUFFER_COUNT - 1)
	{
		return _currentFrameIndex + 1;
	}
	else
	{
		return 0;
	}
}
/****************************************************************************
*                        IssueViewID
*************************************************************************//**
*  @fn        UINT GraphicsDeviceDirectX12::IssueViewID(HeapType heapType)
*  @brief     Issue View ID
*  @param[in] void
*  @return 　　UINT
*****************************************************************************/
UINT GraphicsDeviceDirectX12::IssueViewID(HeapFlag heapType)
{
	switch (heapType)
	{
		case HeapFlag::RTV:
			return _rtvAllocator->IssueID();
		case HeapFlag::DSV:
			return _dsvAllocator->IssueID();
		case HeapFlag::CBV:
			return _cbvAllocator->IssueID();
		case HeapFlag::SRV:
			return _srvAllocator->IssueID();
		case HeapFlag::UAV:
			return _uavAllocator->IssueID();
		default:
			return _samplerAllocator->IssueID();
	}
}

GraphicsDeviceDirectX12::StaticSamplerArray GraphicsDeviceDirectX12::GetStaticSamplers()
{
	const STATIC_SAMPLER_DESC samplerPointWrap
	(
		0,
		D3D12_FILTER_MIN_MAG_MIP_POINT,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP
	);

	const STATIC_SAMPLER_DESC samplerPointClamp
	(
		1,
		D3D12_FILTER_MIN_MAG_MIP_POINT,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP
	);

	const STATIC_SAMPLER_DESC samplerLinearWrap
	(
		2,
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP
	);

	const STATIC_SAMPLER_DESC samplerLinearClamp
	(
		3,
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP
	);

	const STATIC_SAMPLER_DESC samplerAnisotropicWrap
	(
		4,
		D3D12_FILTER_ANISOTROPIC,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP
	);

	const STATIC_SAMPLER_DESC samplerAnisotropicClamp
	(
		5,
		D3D12_FILTER_ANISOTROPIC,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP
	);

	return
	{
		samplerPointWrap,
		samplerPointClamp,
		samplerLinearWrap,
		samplerLinearClamp,
		samplerAnisotropicWrap,
		samplerAnisotropicClamp
	};
}
#pragma endregion Property
#pragma endregion Public Function
#pragma region Private Function
/****************************************************************************
*                     LoadPipeLine
*************************************************************************//**
*  @fn        void DirectX12::LoadPipeLine(void)
*  @brief     Load DirectX Basic PipeLine (for initialize)
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::LoadPipeline()
{
	/*-------------------------------------------------------------------
	-                   Set Debug Layer
	---------------------------------------------------------------------*/
#if _DEBUG
	EnabledDebugLayer();
#endif
	/*-------------------------------------------------------------------
	-					 Create Device
	---------------------------------------------------------------------*/
	CreateDevice();
	/*-------------------------------------------------------------------
	-                   Log Adapters (for debug)
	---------------------------------------------------------------------*/
#if _DEBUG
	LogAdapters();
#endif
	/*-------------------------------------------------------------------
	-				     Check DXR support
	---------------------------------------------------------------------*/
	CheckDXRSupport();
	/*-------------------------------------------------------------------
	-				     Check Tearing support
	---------------------------------------------------------------------*/
	CheckTearingSupport();
	/*-------------------------------------------------------------------
	-				     Check Variable Rate Shading support
	---------------------------------------------------------------------*/
	CheckVRSSupport();
	/*-------------------------------------------------------------------
	-				       Set 4xMsaa
	---------------------------------------------------------------------*/
	CheckMultiSampleQualityLevels();
	/*-------------------------------------------------------------------
	-				       Set 4xMsaa
	---------------------------------------------------------------------*/
	_isHDRSupport = CheckHDRDisplaySupport();
	/*-------------------------------------------------------------------
	-                     Create Fence
	---------------------------------------------------------------------*/
	ThrowIfFailed(_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)));
	for (int i = 0; i < FRAME_BUFFER_COUNT; ++i) { _currentFenceValue[i] = 0; }
	/*-------------------------------------------------------------------
	-                   Create Command Object
	---------------------------------------------------------------------*/
	CreateCommandObject();
	/*-------------------------------------------------------------------
	-                   Create Swapchain
	---------------------------------------------------------------------*/
	CreateSwapChain();
	
	/*-------------------------------------------------------------------
	-                   Set HDR
	---------------------------------------------------------------------*/
	if (_isHDRSupport)
	{
		EnsureSwapChainColorSpace();
		SetHDRMetaData();
	}
	/*-------------------------------------------------------------------
	-                 Create Descriptor Heap
	---------------------------------------------------------------------*/
	CreateAllDescriptorHeap();
	BuildAllResourceAllocator();
	for (int i = 0; i < FRAME_BUFFER_COUNT; ++i) { IssueViewID(HeapFlag::RTV); }
	IssueViewID(HeapFlag::DSV);


	/*-------------------------------------------------------------------
	-                 Create Default PSO
	---------------------------------------------------------------------*/
	CreateDefaultPSO();


}
void GraphicsDeviceDirectX12::LoadAssets()
{
	OnResize();
}
#pragma region           Initialize Function
/****************************************************************************
*                     CreateDevice
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::CreateDevice(void)
*  @brief     Create DirectX12 Device
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::CreateDevice()
{
	/*-------------------------------------------------------------------
	-                   Crate DXGIFactory
	---------------------------------------------------------------------*/
#ifdef _DEBUG
	ThrowIfFailed(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&_dxgiFactory)));
#else
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&_dxgiFactory)));
#endif

	/*-------------------------------------------------------------------
	-                   Search Hardware Adapter
	---------------------------------------------------------------------*/
	AdapterComPtr adapter;
	AdapterComPtr adapterVender[(int)GPUVender::CountOfGPUVender];
	size_t maxVideoMemory[(int)GPUVender::CountOfGPUVender] = { 0 };
	for (int i = 0; _dxgiFactory->EnumAdapters1(i, (IDXGIAdapter1**)adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		DXGI_ADAPTER_DESC1 adapterDesc;
		adapter->GetDesc1(&adapterDesc);
		if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) { continue; }
		if (FAILED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr))) { continue; }
		if (wcsstr(adapterDesc.Description, L"NVIDIA") != nullptr)
		{
			if (adapterDesc.DedicatedVideoMemory > maxVideoMemory[(int)GPUVender::NVidia])
			{
				if (adapterVender[(int)GPUVender::NVidia]) { adapterVender[(int)GPUVender::NVidia]->Release(); }
				adapterVender[(int)GPUVender::NVidia] = adapter;
				adapterVender[(int)GPUVender::NVidia]->AddRef();
			}
		}
		else if (wcsstr(adapterDesc.Description, L"AMD") != nullptr)
		{
			if (adapterDesc.DedicatedVideoMemory > maxVideoMemory[(int)GPUVender::AMD])
			{
				if (adapterVender[(int)GPUVender::AMD]) { adapterVender[(int)GPUVender::AMD]->Release(); }
				adapterVender[(int)GPUVender::AMD] = adapter;
				adapterVender[(int)GPUVender::AMD]->AddRef();
			}
		}
		else if (wcsstr(adapterDesc.Description, L"Intel") != nullptr)
		{
			if (adapterDesc.DedicatedVideoMemory > maxVideoMemory[(int)GPUVender::Intel])
			{
				if (adapterVender[(int)GPUVender::Intel]) { adapterVender[(int)GPUVender::Intel]->Release(); }
				adapterVender[(int)GPUVender::Intel] = adapter;
				adapterVender[(int)GPUVender::Intel]->AddRef();
			}
		}
		//adapter->Release();
	}
	for (int i = 0; i < (int)GPUVender::CountOfGPUVender; ++i)
	{
		if (adapterVender[i] != nullptr)
		{
			_useAdapter = adapterVender[i].Detach();
			break;
		}
	}

	/*-------------------------------------------------------------------
	-                   Create Hardware Device
	---------------------------------------------------------------------*/
	HRESULT hardwareResult = D3D12CreateDevice(
		_useAdapter.Get(),      // default adapter
		D3D_FEATURE_LEVEL_12_0, // minimum feature level
		IID_PPV_ARGS(&_device));


	/*-------------------------------------------------------------------
	-                  Fallback to WARP Device
	-    (WARP: hight speed fully conformant software rasterizer)
	---------------------------------------------------------------------*/
	if (FAILED(hardwareResult))
	{
		AdapterComPtr warpAdapter;
		ThrowIfFailed(_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

		ThrowIfFailed(D3D12CreateDevice(
			warpAdapter.Get(),        // video adapter
			D3D_FEATURE_LEVEL_12_0,   // minimum feature level
			IID_PPV_ARGS(&_device))); // device interface
		_isWarpAdapter = true;
	}
	_device->SetName(L"DirectX12::Device");

	for (int i = 0; i < (int)GPUVender::CountOfGPUVender; ++i)
	{
		if (adapterVender[i] != nullptr)
		{
			adapterVender[i]->Release();
		}
	}
}
/****************************************************************************
*							CreateCommandObject
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::CreateCommandObject(void)
*  @brief     Create command object (command queue, command allocator, and command list)@n 
*             for initialize.
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::CreateCommandObject()
{
	/*-------------------------------------------------------------------
	-                   Create Command Queue
	---------------------------------------------------------------------*/
	D3D12_COMMAND_QUEUE_DESC cmdQDesc = {};
	cmdQDesc.NodeMask = SINGLE_GPU;                          // Single GPU
	cmdQDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL; // Command queue priority
	cmdQDesc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;      // Enable to execute all command 
	cmdQDesc.Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE;       // Default 

	ThrowIfFailed(_device->CreateCommandQueue(&cmdQDesc, IID_PPV_ARGS(&_commandQueue)));
	_commandQueue->SetName(L"DirectX12::CommandQueue");

	/*-------------------------------------------------------------------
	-                   Create Command Allocator
	---------------------------------------------------------------------*/
	for (int i = 0; i < FRAME_BUFFER_COUNT; ++i)
	{
		ThrowIfFailed(_device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,              // Enable to execute all command 
			IID_PPV_ARGS(&_commandAllocator[i])));
		_commandAllocator[i]->SetName(L"DirectX12::CommandAllocator");
	}
	

	/*-------------------------------------------------------------------
	-                   Create Command List
	---------------------------------------------------------------------*/

	ThrowIfFailed(_device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		_commandAllocator[_currentFrameIndex].Get(), // Associated command allocator
		nullptr,                                     // Initial PipeLine State Object
		IID_PPV_ARGS(_commandList.GetAddressOf())));
	_commandList->SetName(L"DirectX12::CommandList");

	// Start off in a closed state.
	// This is because the first time we refer to the command list when it is reset.
	_commandList->Close();
}
/****************************************************************************
*							CreateAllDescriptorHeap
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::CreateAllDescriptorHeap(void)
*  @brief     Create RTV and DSV descriptor heap
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::CreateAllDescriptorHeap()
{
	CreateDescriptorHeap(HeapFlag::RTV);
	CreateDescriptorHeap(HeapFlag::DSV);
	CreateDescriptorHeap(HeapFlag::SRV); // cbv, srv, uav
	CreateDescriptorHeap(HeapFlag::Sampler);
}
void GraphicsDeviceDirectX12::CreateDescriptorHeap(HeapFlag heapFlag)
{
	switch (heapFlag)
	{
		/*-------------------------------------------------------------------
		-			     Set RTV Heap
		---------------------------------------------------------------------*/
		case HeapFlag::RTV:
		{
			if (_rtvHeap) { _rtvHeap.Reset(); }
			/*-------------------------------------------------------------------
			-                   Get Descriptor Inclement Size
			---------------------------------------------------------------------*/
			_rtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			/*-------------------------------------------------------------------
			-			     Describe heap desc
			---------------------------------------------------------------------*/
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
			rtvHeapDesc.NumDescriptors = RTV_DESC_COUNT;
			rtvHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			rtvHeapDesc.NodeMask       = SINGLE_GPU;
			ThrowIfFailed(_device->CreateDescriptorHeap(
				&rtvHeapDesc, IID_PPV_ARGS(_rtvHeap.GetAddressOf())));
			_rtvHeap->SetName(L"DirectX12::RenderTargetHeap");
			break;
		}
		/*-------------------------------------------------------------------
		-			     Set DSV Heap
		---------------------------------------------------------------------*/
		case HeapFlag::DSV:
		{
			if (_dsvHeap) { _dsvHeap.Reset(); }
			/*-------------------------------------------------------------------
			-                   Get Descriptor Inclement Size
			---------------------------------------------------------------------*/
			_dsvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
			/*-------------------------------------------------------------------
			-			     Describe heap desc
			---------------------------------------------------------------------*/
			D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
			dsvHeapDesc.NumDescriptors = DSV_DESC_COUNT;
			dsvHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			dsvHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			dsvHeapDesc.NodeMask       = SINGLE_GPU;
			ThrowIfFailed(_device->CreateDescriptorHeap(
				&dsvHeapDesc, IID_PPV_ARGS(_dsvHeap.GetAddressOf())));
			_dsvHeap->SetName(L"DirectX12::DepthStencilViewHeap");
			break;
		}
		case HeapFlag::CBV:
		case HeapFlag::SRV:
		case HeapFlag::UAV:
		{
			if (_cbvSrvUavHeap) { _cbvSrvUavHeap.Reset(); }
			_cbvSrvUavDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			/*-------------------------------------------------------------------
			-			     Set CBV, SRV, UAV Heap
			---------------------------------------------------------------------*/
			D3D12_DESCRIPTOR_HEAP_DESC csuHeapDesc = {};
			csuHeapDesc.NumDescriptors = CBV_DESC_COUNT + SRV_DESC_COUNT + UAV_DESC_COUNT;
			csuHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			csuHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			csuHeapDesc.NodeMask       = SINGLE_GPU;
			ThrowIfFailed(_device->CreateDescriptorHeap(&csuHeapDesc, IID_PPV_ARGS(&_cbvSrvUavHeap)));
			_cbvSrvUavHeap->SetName(L"DirectX12::CBV,SRV,UAVHeap");
			break;
		}
		default:
		{
			if (_samplerHeap) { _samplerHeap.Reset(); }
			_samplerDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
			/*-------------------------------------------------------------------
			-			     Set CBV, SRV, UAV Heap
			---------------------------------------------------------------------*/
			D3D12_DESCRIPTOR_HEAP_DESC samplerHeapDesc = {};
			samplerHeapDesc.NumDescriptors = MAX_SAMPLER_STATE;
			samplerHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
			samplerHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			samplerHeapDesc.NodeMask       = SINGLE_GPU;
			ThrowIfFailed(_device->CreateDescriptorHeap(&samplerHeapDesc, IID_PPV_ARGS(&_samplerHeap)));
			_cbvSrvUavHeap->SetName(L"DirectX12::SamplerHeap");
		}
	}
}
/****************************************************************************
*							BuildAllResourceAllocator
*************************************************************************//**
*  @fn        void DirectX12::BuildAllResourceAllocator()
*  @brief     Build Resource Allocator
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::BuildAllResourceAllocator()
{
	BuildResourceAllocator((int)HeapFlag::RTV | (int)HeapFlag::DSV | (int)HeapFlag::CBV | (int)HeapFlag::SRV | (int)HeapFlag::UAV | (int)HeapFlag::Sampler);
}
void GraphicsDeviceDirectX12::BuildResourceAllocator(int heapFlag)
{
	/*-------------------------------------------------------------------
	-			     Set RTV Allocator
	---------------------------------------------------------------------*/
	if ((int)heapFlag & (int)HeapFlag::RTV)
	{
		if (_rtvAllocator) { delete _rtvAllocator; }

		auto rtvCpuHandler = _rtvHeap->GetCPUDescriptorHandleForHeapStart();
		D3D12_GPU_DESCRIPTOR_HANDLE rtvGpuHandler = {};
		rtvGpuHandler.ptr  = 0;// = _rtvHeap->GetGPUDescriptorHandleForHeapStart();
		_rtvAllocator      = new ResourceAllocator();
		_rtvAllocator->SetResourceAllocator(RTV_DESC_COUNT, _rtvDescriptorSize, rtvCpuHandler, rtvGpuHandler);
	}
	/*-------------------------------------------------------------------
	-			     Set DSV Allocator
	---------------------------------------------------------------------*/
	if ((int)heapFlag & (int)HeapFlag::DSV)
	{
		if (_dsvAllocator) { delete _dsvAllocator; }

		auto dsvCpuHandler = _dsvHeap->GetCPUDescriptorHandleForHeapStart();
		D3D12_GPU_DESCRIPTOR_HANDLE dsvGpuHandler = {};
		dsvGpuHandler.ptr  = 0;// = _dsvHeap->GetGPUDescriptorHandleForHeapStart();
		_dsvAllocator      = new ResourceAllocator();
		_dsvAllocator->SetResourceAllocator(DSV_DESC_COUNT, _dsvDescriptorSize, dsvCpuHandler, dsvGpuHandler);
	}
	/*-------------------------------------------------------------------
	-			     Set CBV Allocator
	---------------------------------------------------------------------*/
	if ((int)heapFlag & (int)HeapFlag::CBV)
	{
		if (_cbvAllocator) { delete _cbvAllocator; }

		auto cbvCpuHandler = _cbvSrvUavHeap->GetCPUDescriptorHandleForHeapStart();
		auto cbvGpuHandler = _cbvSrvUavHeap->GetGPUDescriptorHandleForHeapStart();
		_cbvAllocator = new ResourceAllocator();
		_cbvAllocator->SetResourceAllocator(CBV_DESC_COUNT, _cbvSrvUavDescriptorSize, cbvCpuHandler, cbvGpuHandler);
	}
	/*-------------------------------------------------------------------
	-			     Set SRV Heap
	---------------------------------------------------------------------*/
	if ((int)heapFlag & (int)HeapFlag::SRV)
	{
		if (_srvAllocator) { delete _srvAllocator; }

		auto srvCpuHandler = _cbvSrvUavHeap->GetCPUDescriptorHandleForHeapStart();
		srvCpuHandler.ptr += (UINT64)CBV_DESC_COUNT * _cbvSrvUavDescriptorSize;
		auto srvGpuHandler = _cbvSrvUavHeap->GetGPUDescriptorHandleForHeapStart();
		srvGpuHandler.ptr += (UINT64)CBV_DESC_COUNT * _cbvSrvUavDescriptorSize;
		_srvAllocator = new ResourceAllocator();
		_srvAllocator->SetResourceAllocator(SRV_DESC_COUNT, _cbvSrvUavDescriptorSize, srvCpuHandler, srvGpuHandler);
	}
	/*-------------------------------------------------------------------
	-			     Set UAV Heap
	---------------------------------------------------------------------*/
	if ((int)heapFlag & (int)HeapFlag::UAV)
	{
		if (_uavAllocator) { delete _uavAllocator; }

		auto uavCpuHandler = _cbvSrvUavHeap->GetCPUDescriptorHandleForHeapStart();
		uavCpuHandler.ptr += (UINT64)(CBV_DESC_COUNT + SRV_DESC_COUNT) * _cbvSrvUavDescriptorSize;
		auto uavGpuHandler = _cbvSrvUavHeap->GetGPUDescriptorHandleForHeapStart();
		uavGpuHandler.ptr += (UINT64)(CBV_DESC_COUNT + SRV_DESC_COUNT) * _cbvSrvUavDescriptorSize;
		_uavAllocator = new ResourceAllocator();
		_uavAllocator->SetResourceAllocator(UAV_DESC_COUNT, _cbvSrvUavDescriptorSize, uavCpuHandler, uavGpuHandler);
	}
	/*-------------------------------------------------------------------
	-			     Set UAV Heap
	---------------------------------------------------------------------*/
	if ((int)heapFlag & (int)HeapFlag::Sampler)
	{
		if (_samplerAllocator) { delete _samplerAllocator; }

		auto samplerCpuHandler = _samplerHeap->GetCPUDescriptorHandleForHeapStart();
		samplerCpuHandler.ptr += (UINT64)(MAX_SAMPLER_STATE) * _samplerDescriptorSize;
		auto samplerGpuHandler = _samplerHeap->GetGPUDescriptorHandleForHeapStart();
		samplerGpuHandler.ptr += (UINT64)(MAX_SAMPLER_STATE) * _samplerDescriptorSize;
		_samplerAllocator = new ResourceAllocator();
		_samplerAllocator->SetResourceAllocator(MAX_SAMPLER_STATE, _samplerDescriptorSize, samplerCpuHandler, samplerGpuHandler);
	}
}
/****************************************************************************
*							BuildRenderTargetView
*************************************************************************//**
*  @fn        void DirectX12::BuildRenderTargetView(void)
*  @brief     Create Render Target View
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::BuildRenderTargetView()
{
	// for drawing 
	CPU_DESC_HANDLER rtvHeapHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < FRAME_BUFFER_COUNT; ++i)
	{
		ThrowIfFailed(_swapchain->GetBuffer(i, IID_PPV_ARGS(_renderTargetList[i].GetAddressOf())));
		_device->CreateRenderTargetView(_renderTargetList[i].GetResource(), nullptr, _rtvAllocator->GetCPUDescHandler(i));
		_renderTargetList[i]->SetName(L"DirectX12::RenderTargetList");
	}
	
}
/****************************************************************************
*                     CreateViewport
*************************************************************************//**
*  @fn        void DirectX12::CreateViewport(void)
*  @brief     Create default viewport
*  @param[in] void
*  @return 　　void @n
*  @details   viewport: we use the viewport when we want to draw the 3D scene @n
			  into a subrectangle of the back buffer.
*****************************************************************************/
void GraphicsDeviceDirectX12::CreateViewport()
{
	_screenViewport.TopLeftX = 0;
	_screenViewport.TopLeftY = 0;
	_screenViewport.Width    = static_cast<float>(Screen::GetScreenWidth());
	_screenViewport.Height   = static_cast<float>(Screen::GetScreenHeight());
	_screenViewport.MinDepth = 0.0f;
	_screenViewport.MaxDepth = 1.0f;

	_scissorRect = { 0,0, Screen::GetScreenWidth(), Screen::GetScreenHeight() };
}
/****************************************************************************
*                     CreateDefaultPSO
*************************************************************************//**
*  @fn        void DirectX12::CreateDefaultPSO(void)
*  @brief     Create default pipeline default object
*  @param[in] void
*  @return 　　void @n
*****************************************************************************/
void GraphicsDeviceDirectX12::CreateDefaultPSO()
{
	VertexPositionNormalColorTexture vertex;

	ZeroMemory(&_defaultPSODesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	_defaultPSODesc.InputLayout           = vertex.InputLayout;
	_defaultPSODesc.pRootSignature        = nullptr;
	_defaultPSODesc.RasterizerState       = RASTERIZER_DESC(D3D12_DEFAULT);
	_defaultPSODesc.BlendState            = BLEND_DESC(D3D12_DEFAULT);
	_defaultPSODesc.SampleMask            = D3D12_DEFAULT_SAMPLE_MASK;
	_defaultPSODesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	_defaultPSODesc.NumRenderTargets      = 1;
	_defaultPSODesc.RTVFormats[0]         = _backBufferFormat;
	_defaultPSODesc.SampleDesc.Count      = _4xMsaaState ? 4 : 1;
	_defaultPSODesc.SampleDesc.Quality    = _4xMsaaState ? (_4xMsaaQuality - 1) : 0;
	_defaultPSODesc.DSVFormat             = _depthStencilFormat;
	_defaultPSODesc.DepthStencilState = DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	_defaultPSODesc.DepthStencilState.DepthEnable    = true;
	_defaultPSODesc.DepthStencilState.DepthFunc      = D3D12_COMPARISON_FUNC_LESS;
	_defaultPSODesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	_defaultPSODesc.DepthStencilState.StencilEnable  = true;
	//pipeline.DSVFormat                        = DXGI_FORMAT_D32_FLOAT;
	_defaultPSODesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (_isWarpAdapter)
	{
		_defaultPSODesc.Flags = D3D12_PIPELINE_STATE_FLAG_TOOL_DEBUG;
	}
}
/****************************************************************************
*							BuildDepthStencilView
*************************************************************************//**
*  @fn        void DirectX12::BuildDepthStencilView(void)
*  @brief     Create Depth / Stencil View
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::BuildDepthStencilView()
{
	/*-------------------------------------------------------------------
	-				 Set Depth / Stencil Descriptor
	---------------------------------------------------------------------*/

	D3D12_RESOURCE_DESC depthStencilDesc ={};
	depthStencilDesc.Dimension          = D3D12_RESOURCE_DIMENSION_TEXTURE2D;       // Depth Texture 2D
	depthStencilDesc.Alignment          = 0;                                        // No alignment
	depthStencilDesc.Width              = Screen::GetScreenWidth();                 // Screen width
	depthStencilDesc.Height             = Screen::GetScreenHeight();                // Screen height
	depthStencilDesc.DepthOrArraySize   = 1;                                        // Not array
	depthStencilDesc.MipLevels          = 1;                                        // Miplevel: 1
	depthStencilDesc.Format             = DXGI_FORMAT_R24G8_TYPELESS;               // R24 Depth G8: Stencil View
	depthStencilDesc.SampleDesc.Count   = _4xMsaaState ? 4 : 1; 
	depthStencilDesc.SampleDesc.Quality = _4xMsaaState ? (_4xMsaaQuality - 1) : 0;
	depthStencilDesc.Layout             = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags              = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	/*-------------------------------------------------------------------
	-					Set Clear Value
	---------------------------------------------------------------------*/
	D3D12_CLEAR_VALUE optClear={};
	optClear.Format               = _depthStencilFormat;
	optClear.DepthStencil.Depth   = 1.0f;
	optClear.DepthStencil.Stencil = 0;

	auto heapProp = HEAP_PROPERTY(D3D12_HEAP_TYPE_DEFAULT);
	ThrowIfFailed(_device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(_depthStencilBuffer.GetAddressOf())));

	/*-------------------------------------------------------------------
	-				Create Depth Stencil View
	---------------------------------------------------------------------*/
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc={};
	dsvDesc.Flags              = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension      = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format             = _depthStencilFormat;
	dsvDesc.Texture2D.MipSlice = 0;
	
	_device->CreateDepthStencilView(_depthStencilBuffer.Get(), &dsvDesc, _dsvAllocator->GetCPUDescHandler(0));

	/*-------------------------------------------------------------------
	- Transition the resource from its initial state to be used as a depth buffer.
	---------------------------------------------------------------------*/
	auto barrier = BARRIER::Transition(_depthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	_commandList->ResourceBarrier(1, &barrier);
}
/****************************************************************************
*							CreateSwapChain
*************************************************************************//**
*  @fn        void DirectX12::CreateSwapChain(void)
*  @brief     Create SwapChain
*  @param[in] void
*  @return 　　void 
*****************************************************************************/
void GraphicsDeviceDirectX12::CreateSwapChain()
{
	/*-------------------------------------------------------------------
	-        Release the previous swapchain we will be recreating
	---------------------------------------------------------------------*/
	_swapchain.Reset();

	/*-------------------------------------------------------------------
	-        SwapChain Flag
	---------------------------------------------------------------------*/
	int flag = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if (_isTearingSupport) { flag |= (int)DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING; }
	_swapchainFlag = static_cast<DXGI_SWAP_CHAIN_FLAG>(flag);
	/*-------------------------------------------------------------------
	-                   Create Swapchain Descriptor
	---------------------------------------------------------------------*/
	DXGI_SWAP_CHAIN_DESC1 sd={};

	sd.BufferCount = FRAME_BUFFER_COUNT;                     // Current: Triple Buffer
	sd.Width       = Screen::GetScreenWidth();               // Window Size Width
	sd.Height      = Screen::GetScreenHeight();              // Window Size Height 
	sd.Format      = _backBufferFormat;                      // Back Buffer Format 
	sd.AlphaMode   = DXGI_ALPHA_MODE_UNSPECIFIED;            // Alpha Mode => transparency behavior is not specified
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;        // Use the surface or resource as an output render target
	sd.Flags       = _swapchainFlag;                         // Allow Resize Window
	sd.Scaling     = DXGI_SCALING_STRETCH;                   // scaling: stretch
	sd.SwapEffect  = DXGI_SWAP_EFFECT_FLIP_DISCARD;          // bit-block transfer model
	sd.Stereo      = false;
	sd.SampleDesc.Count   = _4xMsaaState ? 4 : 1;                     // MSAA: Anti-Alias
	sd.SampleDesc.Quality = _4xMsaaState ? (_4xMsaaQuality - 1) : 0;  // MSAA: Anti-Alias

	/*-------------------------------------------------------------------
	-                   Create Swapchain for hwnd
	---------------------------------------------------------------------*/
	ThrowIfFailed(_dxgiFactory->CreateSwapChainForHwnd(
		_commandQueue.Get(),
		_hwnd,
		&sd,
		nullptr,
		nullptr, // main monitor display
		(IDXGISwapChain1**)(_swapchain.GetAddressOf())
	));


}
/****************************************************************************
*                     MultiSampleQualityLevels
*************************************************************************//**
*  @fn        void DirectX12::CheckMultiSampleQualityLevels(void)
*  @brief     Multi Sample Quality Levels for 4xMsaa (Anti-Alias)
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::CheckMultiSampleQualityLevels()
{
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels = {};
	msQualityLevels.Format           = _backBufferFormat;
	msQualityLevels.SampleCount      = 4;
	msQualityLevels.Flags            = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;

	ThrowIfFailed(_device->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,
		sizeof(msQualityLevels)));

	_4xMsaaQuality = msQualityLevels.NumQualityLevels;
#ifdef _DEBUG
	assert(_4xMsaaQuality > 0 && "Unexpected MSAA quality level.");
#endif
}
/****************************************************************************
*                     CheckTearingSupport
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::CheckTearingSupport()
*  @brief     Tearing support
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::CheckTearingSupport()
{
	if (!_dxgiFactory) { _isTearingSupport = false; return; }

	int allowTearing = false;
	if (FAILED(_dxgiFactory->CheckFeatureSupport(
		DXGI_FEATURE_PRESENT_ALLOW_TEARING,
		&allowTearing, sizeof(allowTearing))))
	{
		_isTearingSupport = false;
	}
	else
	{
		_isTearingSupport = true;
	}
	
}
/****************************************************************************
*                     CheckVRSSupport
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::CheckVRSSupport()
*  @brief     Variable Rate Shading support
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::CheckVRSSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS6 options = {};
	if (SUCCEEDED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS6, &options, sizeof(options))))
	{
		if (options.VariableShadingRateTier == D3D12_VARIABLE_SHADING_RATE_TIER_1)
		{
			OutputDebugStringA("Gpu api: Variable Rate Shading Tier1 supported");
			_isVariableRateShadingTier1Supported = true;
			_isVariableRateShadingTier2Supported = false;
		}
		else if (options.VariableShadingRateTier == D3D12_VARIABLE_SHADING_RATE_TIER_2)
		{
			OutputDebugStringA("Gpu api: Valiable Rate Shading Tier2 supported");
			_isVariableRateShadingTier1Supported = true;
			_isVariableRateShadingTier2Supported = true;
			_variableRateShadingImageTileSize    = options.ShadingRateImageTileSize;
		}
	}
	else
	{
		OutputDebugStringA("GpuApi : Variable Rate Shading note supported on current gpu hardware.");
		_isVariableRateShadingTier1Supported = false;
		_isVariableRateShadingTier2Supported = false;
	}
}
#pragma endregion Initialize Function
#pragma region           Debug      Function
/****************************************************************************
*                     EnabledDebugLayer
*************************************************************************//**
*  @fn        void DirectX12::EnabledDebugLayer(void)
*  @brief     Enabled CPU debug layer
*  @param[in] void
*  @return 　　void @n
*  @details   it must be called before the D3D12 device is created.
*****************************************************************************/
void GraphicsDeviceDirectX12::EnabledDebugLayer()
{
	DebugComPtr debugController;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	debugController->EnableDebugLayer();
	debugController.Reset();
}

/****************************************************************************
*                   EnabledGPUBasedValiation
*************************************************************************//**
*  @fn        void DirectX12::EnabledGPUBasedValidation(void)
*  @brief     Enabled GPU debugger
*  @param[in] void
*  @return 　　void
*  @details   GPU-based valiation helps to identify the following errors.@n
*             1. Use of uninitialized or incompatible descriptors in a shader.@n
*             2. Use of descriptors referencing deleted Resources in a shader.@n
*             3. Validation of promoted resource states and resource state decay.@n
*             4. Indexing beyond the end of the descriptor heap in a shader.@n
*             5. Shader accesses of resources in incompatible state.@n
*             6. Use of uninitialized or incompatible Samplers in a shader.@n
*****************************************************************************/
void GraphicsDeviceDirectX12::EnabledGPUBasedValidation()
{
	DebugComPtr debugController;
	DebugComPtr debug;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	ThrowIfFailed(debugController->QueryInterface(IID_PPV_ARGS(&debug)));
	debug->SetEnableGPUBasedValidation(true);

}

/****************************************************************************
*                     LogAdapters
*************************************************************************//**
*  @fn        void DirectX12::LogAdapters(void)
*  @brief     Show all adapter name (GeForce... )
*  @param[in] Adapter* adapter
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::LogAdapters()
{
	UINT i = 0;
	IAdapter* adapter = nullptr;
	std::vector<IAdapter*> adapterList;

	/*-------------------------------------------------------------------
	-                  Enum Adapter List
	---------------------------------------------------------------------*/
	while (_dxgiFactory->EnumAdapters(i, (IDXGIAdapter**)&adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		std::wstring text = L"***Adapter: ";
		text += desc.Description;
		text += L"\n";

		OutputDebugString(text.c_str());

		adapterList.push_back(adapter);

		++i;
	}

	/*-------------------------------------------------------------------
	-                  Show Adapter List
	---------------------------------------------------------------------*/
	for (size_t j = 0; j < adapterList.size(); ++j)
	{
		LogAdapterOutputs(adapterList[j]);
		SAFE_RELEASE(adapterList[j]);
	}
}

/****************************************************************************
*                     LogAdapterOutputs
*************************************************************************//**
*  @fn        void DirectX12::LogAdapterOutputs(Adapter* adapter)
*  @brief     Show all display output name
*  @param[in] Adapter* adapter
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::LogAdapterOutputs(IAdapter* adapter)
{
	/*-------------------------------------------------------------------
	-                     Initialize
	---------------------------------------------------------------------*/
	UINT i = 0;
	IOutput* output = nullptr;

	/*-------------------------------------------------------------------
	-                Show all adapter name
	---------------------------------------------------------------------*/
	while (adapter->EnumOutputs(i, (IDXGIOutput**)&output) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC desc;
		output->GetDesc(&desc);

		std::wstring text = L"***Output: ";
		text += desc.DeviceName;
		text += L"n";
		OutputDebugString(text.c_str());

		LogOutputDisplayModes(output, _backBufferFormat);

		SAFE_RELEASE(output);

		++i;
	}
}

/****************************************************************************
*                     LogOutputDisplayModes
*************************************************************************//**
*  @fn        void DirectX12::LogOutputDisplayModes(Output* output, DXGI_FORMAT format)
*  @brief     Show display modes (output screen width, height and refresh rates)
*  @param[in] Output* output: for display modelist
*  @param[in] DXGI_FORMAT   : format
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::LogOutputDisplayModes(IOutput* output, DXGI_FORMAT format)
{
	UINT count = 0;
	UINT flags = 0;

	/*-------------------------------------------------------------------
	-             Call with nullptr to get list count
	---------------------------------------------------------------------*/
	output->GetDisplayModeList(format, flags, &count, nullptr);

	std::vector<DXGI_MODE_DESC> modeList(count);
	output->GetDisplayModeList(format, flags, &count, &modeList[0]);

	/*-------------------------------------------------------------------
	-                 Show display modes
	---------------------------------------------------------------------*/
	for (auto& x : modeList)
	{
		UINT n = x.RefreshRate.Numerator;
		UINT d = x.RefreshRate.Denominator;
		std::wstring text =
			L"Width = " + std::to_wstring(x.Width) + L" " +
			L"Height = " + std::to_wstring(x.Height) + L" " +
			L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
			L"\n";

		::OutputDebugString(text.c_str());
	}
}
/****************************************************************************
*                     ReportLiveObjects
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::ReportLiveObjects()
*  @brief     ReportLiveObjects
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::ReportLiveObjects()
{
	ID3D12DebugDevice2* debugInterface = nullptr;
	if (SUCCEEDED(_device.Get()->QueryInterface(&debugInterface)))
	{
		debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL); // Only Objects with external reference counts
		debugInterface->Release();
	}
}
#pragma endregion      Debug Function
#pragma region HDR Function
/****************************************************************************
*                     EnsureSwapChainColorSpace
*************************************************************************//**
*  @fn        void DirectX12::EnsureSwapChainColorSpace()
*  @brief     Check SwapChain Color Space
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::EnsureSwapChainColorSpace()
{

	DXGI_SWAP_CHAIN_DESC1 desc;
	ThrowIfFailed(_swapchain->GetDesc1(&desc));

	DXGI_COLOR_SPACE_TYPE colorSpace;
	switch (desc.Format)
	{
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;    // scrgb
			break;
		case DXGI_FORMAT_R10G10B10A2_UNORM:
			colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020;
			break;
		default:
			colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;
			break;
	}

	_swapchain->SetColorSpace1(colorSpace);
}

/****************************************************************************
*                     SetHDRMetaData
*************************************************************************//**
*  @fn        void DirectX12::SetHDRMetaData()
*  @brief     Set HDR Meta Data
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::SetHDRMetaData()
{

	DXGI_SWAP_CHAIN_DESC1 desc;
	ThrowIfFailed(_swapchain->GetDesc1(&desc));

	/*-------------------------------------------------------------------
	-          In case False (isHDRSupport)
	---------------------------------------------------------------------*/
	if (!_isHDRSupport)
	{
		// not supported
		ThrowIfFailed(_swapchain->SetHDRMetaData(DXGI_HDR_METADATA_TYPE_NONE, 0, nullptr));
		return;
	}

	/*-------------------------------------------------------------------
	-          Define Display Chromacity
	---------------------------------------------------------------------*/
	struct DisplayChromacities
	{
		float RedX  ; float RedY;
		float GreenX; float GreenY;
		float BlueX ; float BlueY;
		float WhiteX; float WhiteY;
	};

	static const DisplayChromacities DisplayChromacityList[] =
	{
		 { 0.64000f, 0.33000f, 0.30000f, 0.60000f, 0.15000f, 0.06000f, 0.31270f, 0.32900f }, // Display Gamut Rec709 
		 { 0.70800f, 0.29200f, 0.17000f, 0.79700f, 0.13100f, 0.04600f, 0.31270f, 0.32900f }  // Display Gamut Rec2020
	};

	/*-------------------------------------------------------------------
	-          Select Chroma Format
	---------------------------------------------------------------------*/
	int selectedChroma = 0;
	if (desc.Format == DXGI_FORMAT_R16G16B16A16_FLOAT)
	{
		selectedChroma = 1;
	}

	/*-------------------------------------------------------------------
	-          Set HDR10Meta Data
	---------------------------------------------------------------------*/
	const auto& chroma = DisplayChromacityList[selectedChroma];
	DXGI_HDR_METADATA_HDR10 HDR10MetaData = {};
	HDR10MetaData.RedPrimary[0]   = UINT16(chroma.RedX * 50000.0f);
	HDR10MetaData.RedPrimary[1]   = UINT16(chroma.RedY * 50000.0f);
	HDR10MetaData.GreenPrimary[0] = UINT16(chroma.GreenX * 50000.0f);
	HDR10MetaData.GreenPrimary[1] = UINT16(chroma.GreenY * 50000.0f);
	HDR10MetaData.BluePrimary[0]  = UINT16(chroma.BlueX * 50000.0f);
	HDR10MetaData.BluePrimary[1]  = UINT16(chroma.BlueY * 50000.0f);
	HDR10MetaData.WhitePoint[0]   = UINT16(chroma.WhiteX * 50000.0f);
	HDR10MetaData.WhitePoint[1]   = UINT16(chroma.WhiteY * 50000.0f);
	HDR10MetaData.MaxMasteringLuminance = UINT(1000.0f * 10000.0f);
	HDR10MetaData.MinMasteringLuminance = UINT(0.001f  * 10000.0f);
	HDR10MetaData.MaxContentLightLevel  = UINT16(2000.0f);
	HDR10MetaData.MaxFrameAverageLightLevel = UINT16(500.0f);

	/*-------------------------------------------------------------------
	-          Set HDRMetaData
	---------------------------------------------------------------------*/
	_swapchain->SetHDRMetaData(DXGI_HDR_METADATA_TYPE_HDR10, sizeof(HDR10MetaData), &HDR10MetaData);
}

void GraphicsDeviceDirectX12::IsEnabledHDR()
{
	if (CheckHDRDisplaySupport())
	{
		OnResize();
		EnsureSwapChainColorSpace();
		SetHDRMetaData();
	}

}
/****************************************************************************
*                     CheckHDRDisplaySupport
*************************************************************************//**
*  @fn        bool DirectX12::CheckHDRDisplaySupport()
*  @brief     CheckHDRDisplaySupport()
*  @param[in] void
*  @return 　　void
*****************************************************************************/
bool GraphicsDeviceDirectX12::CheckHDRDisplaySupport()
{
	_backBufferFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;
	bool isEnabledHDR =
		_backBufferFormat == DXGI_FORMAT_R16G16B16A16_FLOAT ||
		_backBufferFormat == DXGI_FORMAT_R10G10B10A2_UNORM;

	if (isEnabledHDR)
	{
		bool isDisplayHDR10 = false;
		UINT index = 0;

		ComPtr<IDXGIOutput> currentOutput = nullptr;
		ComPtr<IDXGIOutput> bestOutput    = nullptr;
		while (_useAdapter->EnumOutputs(index, &currentOutput) != DXGI_ERROR_NOT_FOUND)
		{
			OutputComPtr output;
			currentOutput.As(&output);

			DXGI_OUTPUT_DESC1 desc;
			output->GetDesc1(&desc);

			isDisplayHDR10 = (desc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020);
			++index;
			if (isDisplayHDR10) { break; }
		}

		if (!isDisplayHDR10)
		{
			//_backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			//isEnabledHDR = false;
		}
	}

	return isEnabledHDR;

}
#pragma endregion    HDR Function
#pragma region DXR Function
/****************************************************************************
*						CheckDXRSupport
*************************************************************************//**
*  @fn        void DirectX12::CheckDXRSupport
*  @brief     Check DXRSupport
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::CheckDXRSupport()
{
	bool enableRayTracing = true;
	D3D12_FEATURE_DATA_D3D12_OPTIONS5 options{};
	HRESULT result = _device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &options, UINT(sizeof(options)));
	if (FAILED(result) || options.RaytracingTier == D3D12_RAYTRACING_TIER_NOT_SUPPORTED)
	{
		std::wstring errorMessage = L"DirectX Raytracing not supported.";
		OutputDebugString(errorMessage.c_str());
		enableRayTracing = false;
	}

	_enableRayTracing = enableRayTracing;
}
#pragma endregion    DXR Function
#pragma endregion Private Function