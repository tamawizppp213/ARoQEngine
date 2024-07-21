//////////////////////////////////////////////////////////////////////////////////
//              @file   GraphicsCoreEngine.cpp
///             @brief  Graphics Low Level Engine
///             @author Toide Yutaro
///             @date   -
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
// DirectX12
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include <d3d12.h>
#include <dxgi1_6.h>
// Vulkan
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include <Windows.h>
#include <stdexcept>
// Common
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIInstance.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIAdapter.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandAllocator.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandQueue.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandList.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHISwapchain.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIRenderpass.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFence.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIDescriptorHeap.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::core;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Destructor
LowLevelGraphicsEngine::~LowLevelGraphicsEngine()
{
	if (!_hasCalledShutDown) { ShutDown(); }
}
#pragma endregion Destructor

#pragma region Main Function
/*!**********************************************************************
*  @brief     Graphics Engineを起動し, 描画フレームをスタートします.
*  @param[in] const rhi::core::GraphicsAPI
*  @param[in] void* window handle
*  @param[in] void* instance Handle
*  @return    void
*************************************************************************/
void LowLevelGraphicsEngine::StartUp(const GraphicsAPI apiVersion, void* hwnd, void* hInstance)
{
	_apiVersion = apiVersion;
	/*-------------------------------------------------------------------
	-      Create Instance
	---------------------------------------------------------------------*/
#if _DEBUG
	_instance = rhi::core::RHIInstance::CreateInstance(apiVersion, { true, false, false });
	_instance->LogAdapters();
#else
	_instance = rhi::core::RHIInstance::CreateInstance(apiVersion, { false, false, false });
#endif

	/*-------------------------------------------------------------------
	-      Select proper physical device 
	---------------------------------------------------------------------*/
	_adapter = _instance->SearchHighPerformanceAdapter();

	/*-------------------------------------------------------------------
	-      Create logical device
	---------------------------------------------------------------------*/
	_device = _adapter->CreateDevice();

	/*-------------------------------------------------------------------
	-      Get command queue (Graphics, compute, copy command queue )
	---------------------------------------------------------------------*/
	_commandQueues[CommandListType::Graphics] = _device->CreateCommandQueue(CommandListType::Graphics, SP("GraphicsQueue"));
	_commandQueues[CommandListType::Compute]  = _device->CreateCommandQueue(CommandListType::Compute , SP("ComputeQueue"));
	_commandQueues[CommandListType::Copy]     = _device->CreateCommandQueue(CommandListType::Copy    , SP("CopyQueue"));

	/*-------------------------------------------------------------------
	-      Set up command list
	---------------------------------------------------------------------*/
	_commandLists[core::CommandListType::Graphics] = _device->CreateCommandList(_device->CreateCommandAllocator(core::CommandListType::Graphics, SP("GraphicsAllocator")), SP("GraphicsCommandList"));
	_commandLists[core::CommandListType::Compute]  = _device->CreateCommandList(_device->CreateCommandAllocator(core::CommandListType::Compute , SP("ComputeAllocator")) , SP("ComputeCommandList"));
	_commandLists[core::CommandListType::Copy]     = _device->CreateCommandList(_device->CreateCommandAllocator(core::CommandListType::Copy    , SP("CopyAllocator"))    , SP("CopyCommandList"));

	/*-------------------------------------------------------------------
	-      Create fence
	---------------------------------------------------------------------*/
	SetUpFence();

	/*-------------------------------------------------------------------
	-      Set up swapchain
	---------------------------------------------------------------------*/
	const core::WindowInfo    windowInfo = core::WindowInfo(Screen::GetScreenWidth(), Screen::GetScreenHeight(), hwnd, hInstance);
	const core::SwapchainDesc swapchainDesc = 
	{
		.CommandQueue     = _commandQueues[CommandListType::Graphics],
		.WindowInfo       = windowInfo,
		.PixelFormat      = _pixelFormat,
		.FrameBufferCount = FRAME_BUFFER_COUNT,
		.VSync            = VSYNC,
		.IsValidHDR       = true,
		.IsValidStereo    = false,
		.IsFullScreen     = false,
	};

	_swapchain   = _device   ->CreateSwapchain(swapchainDesc);
	_pixelFormat = _swapchain->GetPixelFormat(); // sdrの場合は修正する/

	/*-------------------------------------------------------------------
	-      Set up descriptor heap
	---------------------------------------------------------------------*/
	SetUpHeap();

	/*-------------------------------------------------------------------
	-      Set up color and depth render pass
	---------------------------------------------------------------------*/
	SetUpRenderResource();

	/*-------------------------------------------------------------------
	-      Set up query heap
	---------------------------------------------------------------------*/
	SetUpQuery();

	_hasInitialized = true;
}

/*!**********************************************************************
*  @brief     CommandListを開き, 描画フレームをスタートします.
*  @param[in] void
*  @return    void
*************************************************************************/
void LowLevelGraphicsEngine::BeginDrawFrame()
{
	/*-------------------------------------------------------------------
	-      Get each command list
	---------------------------------------------------------------------*/
	const auto& graphicsCommandList = _commandLists[core::CommandListType::Graphics];
	const auto& computeCommandList  = _commandLists[core::CommandListType::Compute];

	/*-------------------------------------------------------------------
	-      Start Recording Command List
	---------------------------------------------------------------------*/
	graphicsCommandList->BeginRecording(false);
	computeCommandList ->BeginRecording(false);
	_beginDrawFrameTimeStamp = _commandQueues[core::CommandListType::Graphics]->GetCalibrationTimestamp();
}

/*!**********************************************************************
*  @brief     CommandListを閉じ, 描画フレームを終了し, バックバッファを切り替えます.
*  @param[in] void
*  @return    void
*************************************************************************/
void LowLevelGraphicsEngine::EndDrawFrame()
{
	/*-------------------------------------------------------------------
	-      Finish recording commands list
	---------------------------------------------------------------------*/
	// close compute command list
	const auto& computeCommandList = _commandLists[core::CommandListType::Compute];
	computeCommandList->EndRecording();

	// close graphics command list
	const auto& graphicsCommandList = _commandLists[core::CommandListType::Graphics];
	if (graphicsCommandList->IsOpen())
	{
		graphicsCommandList->EndRenderPass();
		graphicsCommandList->CopyResource(_swapchain->GetBuffer((gu::uint8)_currentFrameIndex), _frameBuffers[_currentFrameIndex]->GetRenderTarget());
		graphicsCommandList->EndRecording();
	}

	/*-------------------------------------------------------------------
	-          Execute GPU Command
	---------------------------------------------------------------------*/
	_commandQueues[core::CommandListType::Compute]->Execute({ computeCommandList });
	_commandQueues[core::CommandListType::Compute]->Signal(_fence, ++_fenceValue);

	_commandQueues[core::CommandListType::Graphics]->Wait(_fence, _fenceValue);
	_commandQueues[core::CommandListType::Graphics]->Execute({ graphicsCommandList });
	_commandQueues[core::CommandListType::Graphics]->Signal(_fence, ++_fenceValue);

	/*-------------------------------------------------------------------
	-          Flip Screen
	---------------------------------------------------------------------*/
	_swapchain->Present(_fence, _fenceValue);
	_fence->Wait(_fenceValue);

	/*-------------------------------------------------------------------
	-      GPU Command Wait
	---------------------------------------------------------------------*/
	_currentFrameIndex = _swapchain->PrepareNextImage(_fence, ++_fenceValue);
	SetUpFence(); // reset fence value for the next frame

	_endDrawFrameTimeStamp = _commandQueues[core::CommandListType::Graphics]->GetCalibrationTimestamp();
	_gpuTimer = (_endDrawFrameTimeStamp.GPUMicroseconds - _beginDrawFrameTimeStamp.GPUMicroseconds) / 1e6;
	_cpuTimer = (_endDrawFrameTimeStamp.CPUMicroseconds - _beginDrawFrameTimeStamp.CPUMicroseconds) / 1e6;
	
}

/*!**********************************************************************
*  @brief     対象のコマンドリストを実行し, フェンスのシグナル値を返します.
*  @param[in] const rhi::core::CommandListType コマンドリストの種類
*  @param[in] const bool まだフレーム中かどうか
*  @return    gu::uint64 フェンスのシグナル値
*************************************************************************/
std::uint64_t LowLevelGraphicsEngine::FlushGPUCommands(const rhi::core::CommandListType type, const bool stillMidFrame)
{
	// set command lists
	const auto& commandList = _commandLists[type];
	
	/*-------------------------------------------------------------------
	-          Transit the recorded state into the executable state
	---------------------------------------------------------------------*/
	if (commandList->IsOpen())
	{
		if (commandList->GetType() == CommandListType::Graphics)
		{
			commandList->EndRenderPass();
		}
		commandList->EndRecording();
	}

	/*-------------------------------------------------------------------
	-          Execute gpu commands
	---------------------------------------------------------------------*/
	_commandQueues[type]->Execute({commandList});
	_commandQueues[type]->Signal(_fence, ++_fenceValue);

	/*-------------------------------------------------------------------
	-         Restart command list 
	---------------------------------------------------------------------*/
	if (stillMidFrame)
	{
		commandList->BeginRecording(stillMidFrame);
		if (commandList->GetType() == CommandListType::Graphics)
		{
			commandList->BeginRenderPass(_drawContinueRenderPass, _frameBuffers[_currentFrameIndex]);
		}
	}
	return _fenceValue;
}

/*!**********************************************************************
*  @brief     コマンドキューを呼び出して前までの処理が完了するまでGPUを待機します. 必要に応じてCPUも待機します.
*  @param[in] const rhi::core::CommandListType コマンドリストの種類
*  @param[in] const gu::uint64 待機するフェンスの値
*  @param[in] const bool CPUも待機するかどうか
*  @return    void
*************************************************************************/
void LowLevelGraphicsEngine::WaitExecutionGPUCommands(const rhi::core::CommandListType type, const gu::uint64 waitValue, const bool stopCPU)
{
	const auto& commandQueue = _commandQueues[type];

	commandQueue->Wait(_fence, waitValue);    // gpu stop
	if (stopCPU) { _fence->Wait(waitValue); } // cpu stop
}

/****************************************************************************
*                     OnResize
****************************************************************************/
/* @fn        void LowLevelGraphicsEngine::OnResize(const size_t newWidth, const size_t newHeight)
* 
*  @brief     Resize swapchain
* 
*  @param[in] size_t newWidth
* 
*  @param[in] size_t newHeight
* 
*  @return 　　void
*****************************************************************************/
void LowLevelGraphicsEngine::OnResize(const gu::uint32 newWidth, const gu::uint32 newHeight)
{
	if (!(_width != newWidth || _height != newHeight)) { return; }

	_swapchain->Resize(newWidth, newHeight);
	SetFrameBuffers((int)newWidth, (int)newHeight);

	/*-------------------------------------------------------------------
	-          Wait Graphics Queue
	---------------------------------------------------------------------*/
	_commandQueues[CommandListType::Graphics]->Signal(_fence, ++_fenceValue);
	_fence->Wait(_fenceValue);

	/*-------------------------------------------------------------------
	-          Flip Screen
	---------------------------------------------------------------------*/
	_swapchain->Present(_fence,_fenceValue);

	/*-------------------------------------------------------------------
	-      GPU Command Wait
	---------------------------------------------------------------------*/
	_currentFrameIndex = _swapchain->PrepareNextImage(_fence,++_fenceValue);
	_fence->Wait(_fenceValue);

}

void LowLevelGraphicsEngine::BeginSwapchainRenderPass()
{
	const auto& graphicsCommandList = _commandLists[core::CommandListType::Graphics];
	graphicsCommandList->BeginRenderPass(_renderPass, _frameBuffers[_currentFrameIndex]);
}

/****************************************************************************
*                     ShutDown
****************************************************************************/
/* @fn        void LowLevelGraphicsEngine::ShutDown()
*
*  @brief     Release all render resources
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void LowLevelGraphicsEngine::ShutDown()
{
	/*-------------------------------------------------------------------
	-      This function is called only once
	---------------------------------------------------------------------*/
	if (_hasCalledShutDown) { return; }

	/*-------------------------------------------------------------------
	-      Clear render resouces
	---------------------------------------------------------------------*/
	if (_swapchain) { _swapchain.Reset(); }
	
	for (auto& frameBuffer : _frameBuffers)
	{
		frameBuffer.Reset();
	}
	_frameBuffers.Clear(); 
	_frameBuffers.ShrinkToFit();

	if (_renderPass) { _renderPass.Reset(); }

	_queryHeaps.Clear();

	/*-------------------------------------------------------------------
	-      Clear command list
	---------------------------------------------------------------------*/
	if (_fence) { _fence.Reset(); }

	if(!_commandLists.IsEmpty()){_commandLists.Clear(); }

	if (!_commandQueues.IsEmpty()) { _commandQueues.Clear(); }

	/*-------------------------------------------------------------------
	-      Clear Device resources
	---------------------------------------------------------------------*/
	_device->Destroy();
	if (_device)    { _device.Reset(); }
	if (_adapter)   { _adapter.Reset(); }
	if (_instance)  { _instance.Reset(); }

	_hasCalledShutDown = true;
}

#pragma endregion Main Function

#pragma region Private Function
#pragma region SetUp
/****************************************************************************
*                     SetUpHeap
****************************************************************************/
/* @fn        void LowLevelGraphicsEngine::SetUpHeap()
* 
*  @brief     Prepare Logical Device's Default Heap. (Each size is defined by this class static variables X_DESC_COUNT)
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void LowLevelGraphicsEngine::SetUpHeap()
{
	core::DefaultHeapCount heapCount = {};
	heapCount.CBVDescCount = CBV_DESC_COUNT; 
	heapCount.SRVDescCount = SRV_DESC_COUNT;
	heapCount.UAVDescCount = UAV_DESC_COUNT;
	heapCount.DSVDescCount = _apiVersion == GraphicsAPI::DirectX12 ? DSV_DESC_COUNT : 0;
	heapCount.RTVDescCount = _apiVersion == GraphicsAPI::DirectX12 ? RTV_DESC_COUNT : 0;
	heapCount.SamplerDescCount = MAX_SAMPLER_STATE;
	_device->SetUpDefaultHeap(heapCount);

}

void LowLevelGraphicsEngine::SetUpQuery()
{
	_queryHeaps[QueryHeapType::Occulusion]         = _device->CreateQuery(QueryHeapType::Occulusion);
	_queryHeaps[QueryHeapType::TimeStamp]          = _device->CreateQuery(QueryHeapType::TimeStamp);
	_queryHeaps[QueryHeapType::CopyQueueTimeStamp] = _device->CreateQuery(QueryHeapType::CopyQueueTimeStamp);
	_queryHeaps[QueryHeapType::PipelineStatistics] = _device->CreateQuery(QueryHeapType::PipelineStatistics);
}

void LowLevelGraphicsEngine::SetUpFence()
{
	if (_fence) { _fence.Reset(); }
	_fence = _device->CreateFence();
	_fenceValue = 0;
}
/****************************************************************************
*                     SetUpRenderResource
****************************************************************************/
/* @fn        void LowLevelGraphicsEngine::SetUpRenderResource()
* 
*  @brief     Prepare render pass and frame buffer
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void LowLevelGraphicsEngine::SetUpRenderResource()
{
	const auto clearColor      = core::ClearValue(0.0f, 0.3f, 0.3f, 1.0f);
	const auto clearDepthColor = core::ClearValue(1.0f, 0);

	/*-------------------------------------------------------------------
	-      Set render pass
	---------------------------------------------------------------------*/
	{
		core::Attachment colorAttachment = core::Attachment::RenderTarget(_pixelFormat);
		core::Attachment depthAttachment = core::Attachment::DepthStencil(_depthStencilFormat);

		// vulkanの場合, 初期RenderTargetはUnlnownである必要があるとのこと
		if (_apiVersion == GraphicsAPI::Vulkan) { colorAttachment.InitialLayout = core::ResourceState::Common; }

		_renderPass = _device->CreateRenderPass(colorAttachment, depthAttachment);
		_renderPass->SetClearValue(clearColor, clearDepthColor);
	}

	/*-------------------------------------------------------------------
	-      set continue drawing render pass (for texture rendering)
	---------------------------------------------------------------------*/
	{
		core::Attachment colorAttachment = core::Attachment::DrawContinue(_pixelFormat, core::ResourceState::RenderTarget, core::ResourceState::Present);

		core::Attachment depthAttachment = core::Attachment::DepthStencil(_depthStencilFormat, core::ResourceState::DepthStencil, 
			core::ResourceState::DepthStencil,core::AttachmentLoad::Load);

		_drawContinueRenderPass = _device->CreateRenderPass(colorAttachment, depthAttachment);
		_drawContinueRenderPass->SetClearValue(clearColor, clearDepthColor);
	}

	/*-------------------------------------------------------------------
	-      Create frame buffer
	---------------------------------------------------------------------*/
	_frameBuffers.Resize(FRAME_BUFFER_COUNT);
	SetFrameBuffers(Screen::GetScreenWidth(), Screen::GetScreenHeight(), clearColor, clearDepthColor);
}

void LowLevelGraphicsEngine::SetFrameBuffers(const int width, const int height, const core::ClearValue& clearColor, const core::ClearValue& clearDepthColor)
{
	_width = width;
	_height = height;
	for (size_t i = 0; i < _frameBuffers.Size(); ++i)
	{
		/*-------------------------------------------------------------------
		-      Create Depth Texture
		---------------------------------------------------------------------*/
		auto renderInfo = core::GPUTextureMetaData::RenderTarget(width, height, _pixelFormat, clearColor);
		auto depthInfo  = core::GPUTextureMetaData::DepthStencil( width, height, 
			_depthStencilFormat, clearDepthColor);

		const auto renderTexture = _device->CreateTexture(renderInfo, SP("FrameBuffer"));
		const auto depthTexture  = _device->CreateTexture(depthInfo , SP("FrameBufferDepth"));

		/*-------------------------------------------------------------------
		-      Create Frame Buffer
		---------------------------------------------------------------------*/
		if (_frameBuffers[i]) { _frameBuffers[i].Reset(); }
		_frameBuffers[i] = _device->CreateFrameBuffer(_renderPass, renderTexture, depthTexture);
	}
}
#pragma endregion Set Up
#pragma endregion Private Function
