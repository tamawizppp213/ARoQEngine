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
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Fence.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandQueue.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandAllocator.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandList.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Swapchain.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include "GameUtility/Math/Include/GMColor.hpp"
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace Microsoft::WRL;
using namespace rhi::directX12;
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
*  @fn        void GraphicsDeviceDirectX12::StartUp(void)
*  @brief     Initialize Back Screen
*  @param[in] HWND hwnd
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::StartUp(HWND hwnd, HINSTANCE hInstance)
{
	if (_hasInitialized) { return; }
	SetHWND(hwnd); _hInstance = hInstance;
	LoadPipeline();
	LoadAssets();
	_hasInitialized = true;
}
/****************************************************************************
*							Finalize
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::ShutDown()
*  @brief     Release DirectX12 All Memory
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::ShutDown()
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
	if (_commandQueue) { _commandQueue= nullptr;}
	if (_commandList ) { _commandList = nullptr;}
	if (_dxgiFactory ) { _dxgiFactory = nullptr;}

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
	_rhiSwapchain->Resize(Screen::GetScreenWidth(), Screen::GetScreenHeight());
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
void GraphicsDeviceDirectX12::BeginDrawFrame()
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

void GraphicsDeviceDirectX12::EndDrawFrame()
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
	std::vector<std::shared_ptr<rhi::core::RHICommandList>> commandLists;
	commandLists.push_back(_rhiCommandList);
	_rhiCommandQueue->Execute(commandLists);
	/*-------------------------------------------------------------------
	-						Flip screen
	---------------------------------------------------------------------*/
	_rhiSwapchain->Present(nullptr, 0);
	_currentFrameIndex = (_currentFrameIndex + 1) % FRAME_BUFFER_COUNT;

	FlushCommandQueue(); // Flush command queue
}

void GraphicsDeviceDirectX12::ResetCommandList()
{
	// Start recording commands.
	_commandAllocator[_currentFrameIndex]->Reset();
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

	//_rhiFences[_currentFrameIndex]->Signal(_rhiCommandQueue);
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
	std::vector<std::shared_ptr<rhi::core::RHICommandList>> commandLists;
	commandLists.push_back(_rhiCommandList);
	_rhiCommandQueue->Execute(commandLists);

	/*-------------------------------------------------------------------
	-						Flip screen
	---------------------------------------------------------------------*/
	_rhiSwapchain->Present(nullptr, 0);
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
	_rhiDevice = std::make_shared<rhi::directX12::RHIDevice>();
	//_rhiDevice.get()->Create(_hwnd, _hInstance, _useHDR, _enableRayTracing);
	_device      = _rhiDevice.get()->GetDevice();
	//_dxgiFactory = _rhiDevice.get()->GetFactory();
	//_useAdapter  = _rhiDevice.get()->GetAdapter();
	
	/*-------------------------------------------------------------------
	-                     Create Fence
	---------------------------------------------------------------------*/
	for (int i = 0; i < FRAME_BUFFER_COUNT; ++i)
	{
		_rhiFences[i] = _rhiDevice->CreateFence();
	}

	/*ThrowIfFailed(_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)));
	for (int i = 0; i < FRAME_BUFFER_COUNT; ++i) { _currentFenceValue[i] = 0; }*/
	/*-------------------------------------------------------------------
	-                   Create Command Object
	---------------------------------------------------------------------*/
	CreateCommandObject();
	/*-------------------------------------------------------------------
	-                   Create Swapchain
	---------------------------------------------------------------------*/
	CreateSwapChain();

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
	_rhiCommandQueue = _rhiDevice->CreateCommandQueue(rhi::core::CommandListType::Graphics);
	_commandQueue    = static_pointer_cast<rhi::directX12::RHICommandQueue>(_rhiCommandQueue)->GetCommandQueue();
	/*-------------------------------------------------------------------
	-                   Create Command Allocator
	---------------------------------------------------------------------*/
	for (int i = 0; i < FRAME_BUFFER_COUNT; ++i)
	{
		_rhiCommandAllocator[i] = _rhiDevice->CreateCommandAllocator(rhi::core::CommandListType::Graphics);
		_commandAllocator[i]    = static_pointer_cast<rhi::directX12::RHICommandAllocator>(_rhiCommandAllocator[i])->GetAllocator();
	}
	_rhiCommandList = _rhiDevice->CreateCommandList(_rhiCommandAllocator[_currentFrameIndex]);
	_commandList    = static_pointer_cast<rhi::directX12::RHICommandList>(_rhiCommandList)->GetCommandList();
	
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
		_rtvAllocator      = new rhi::directX12::ResourceAllocator();
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
		_dsvAllocator      = new rhi::directX12::ResourceAllocator();
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
		_cbvAllocator = new rhi::directX12::ResourceAllocator();
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
		_srvAllocator = new rhi::directX12::ResourceAllocator();
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
		_uavAllocator = new rhi::directX12::ResourceAllocator();
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
		_samplerAllocator = new rhi::directX12::ResourceAllocator();
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

	rhi::core::WindowInfo  windowInfo   = { static_cast<size_t>(Screen::GetScreenWidth()), static_cast<size_t>(Screen::GetScreenHeight()), _hwnd };
	rhi::core::PixelFormat pixelFormat = rhi::core::PixelFormat::R16G16B16A16_FLOAT;
	_rhiSwapchain = _rhiDevice->CreateSwapchain(_rhiCommandQueue, windowInfo, pixelFormat, FRAME_BUFFER_COUNT, VSYNC);
	_swapchain    = static_pointer_cast<rhi::directX12::RHISwapchain>(_rhiSwapchain)->GetSwapchain();
}

#pragma endregion Initialize Function


void GraphicsDeviceDirectX12::IsEnabledHDR()
{
	/*if (CheckHDRDisplaySupport())
	{
		OnResize();
		EnsureSwapChainColorSpace();
		SetHDRMetaData();
	}*/

}

#pragma endregion Private Function