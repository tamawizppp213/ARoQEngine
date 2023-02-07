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
/****************************************************************************
*                     Start Up
*************************************************************************//**
*  @fn        void LowLevelGraphicsEngine::StartUp(APIVersion apiVersion, HWND hwnd, HINSTANCE hInstance)
* 
*  @brief     Windows api start up lowlevel graphics engine
* 
*  @param[in] APIVersion apiVersion
*  @param[in] HWND hwnd
*  @param[in] HINSTANCE hInstance
* 
*  @return 　　void
*****************************************************************************/
void LowLevelGraphicsEngine::StartUp(APIVersion apiVersion, HWND hwnd, HINSTANCE hInstance)
{
	_hwnd = hwnd; _hInstance = hInstance; _apiVersion = apiVersion;

	/*-------------------------------------------------------------------
	-      Create Instance
	---------------------------------------------------------------------*/
#if _DEBUG
	_instance = rhi::core::RHIInstance::CreateInstance(apiVersion, true, false);
#else
	_instance = rhi::core::RHIInstance::CreateInstance(apiVersion, false, false);
#endif
	_instance->LogAdapters();

	/*-------------------------------------------------------------------
	-      Select proper physical device 
	---------------------------------------------------------------------*/
	_adapter = _instance->SearchHighPerformanceAdapter();

	/*-------------------------------------------------------------------
	-      Create logical device
	---------------------------------------------------------------------*/
	_device = _adapter->CreateDevice(FRAME_BUFFER_COUNT);

	/*-------------------------------------------------------------------
	-      Get command queue (Graphics, compute, copy command queue )
	---------------------------------------------------------------------*/
	_commandQueues[CommandListType::Graphics] = _device->GetCommandQueue(CommandListType::Graphics);
	_commandQueues[CommandListType::Compute]  = _device->GetCommandQueue(CommandListType::Compute);
	_commandQueues[CommandListType::Copy]     = _device->GetCommandQueue(CommandListType::Copy);

	/*-------------------------------------------------------------------
	-      Create fence
	---------------------------------------------------------------------*/
	_fence = _device->CreateFence();

	/*-------------------------------------------------------------------
	-      Set up swapchain
	---------------------------------------------------------------------*/
	core::WindowInfo windowInfo = core::WindowInfo(Screen::GetScreenWidth(), Screen::GetScreenHeight(), _hwnd, _hInstance);

	_swapchain   = _device->CreateSwapchain( _commandQueues[CommandListType::Graphics], windowInfo, _pixelFormat,  FRAME_BUFFER_COUNT, VSYNC, false);
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
	-      Set up command list
	---------------------------------------------------------------------*/
	_commandLists.resize(FRAME_BUFFER_COUNT);
	for (std::uint32_t i = 0; i < FRAME_BUFFER_COUNT; ++i)
	{
		_commandLists[i][core::CommandListType::Graphics] = _device->CreateCommandList(_device->GetCommandAllocator(core::CommandListType::Graphics, i));
		_commandLists[i][core::CommandListType::Compute]  = _device->CreateCommandList(_device->GetCommandAllocator(core::CommandListType::Compute , i));
		_commandLists[i][core::CommandListType::Copy]     = _device->CreateCommandList(_device->GetCommandAllocator(core::CommandListType::Copy    , i));
	}

}

/****************************************************************************
*                     BeginDrawFrame
*************************************************************************//**
*  @fn        void LowLevelGraphicsEngine::BeginDrawFrame()
* 
*  @brief     The first call to the Draw function generates the back buffer image and executes the Default render pass.
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void LowLevelGraphicsEngine::BeginDrawFrame()
{
	/*-------------------------------------------------------------------
	-      Get each command list
	---------------------------------------------------------------------*/
	const auto& graphicsCommandList = _commandLists[_currentFrameIndex][core::CommandListType::Graphics];
	const auto& computeCommandList  = _commandLists[_currentFrameIndex][core::CommandListType::Compute];

	/*-------------------------------------------------------------------
	-      Start Recording Command List
	---------------------------------------------------------------------*/
	graphicsCommandList->BeginRecording();
	computeCommandList->BeginRecording();
}

/****************************************************************************
*                     EndDrawFrame
*************************************************************************//**
*  @fn        void LowLevelGraphicsEngine::EndDrawFrame()
* 
*  @brief     Call at the end of the Draw function to execute the command list and Flip the Swapchain. 
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void LowLevelGraphicsEngine::EndDrawFrame()
{
	/*-------------------------------------------------------------------
	-      Finish recording commands list
	---------------------------------------------------------------------*/
	// close compute command list
	const auto& computeCommandList = _commandLists[_currentFrameIndex][core::CommandListType::Compute];
	computeCommandList->EndRecording();

	// close graphics command list
	const auto& graphicsCommandList = _commandLists[_currentFrameIndex][core::CommandListType::Graphics];
	graphicsCommandList->EndRenderPass();
	graphicsCommandList->EndRecording();

	/*-------------------------------------------------------------------
	-          Execute GPU Command
	---------------------------------------------------------------------*/
	_commandQueues[core::CommandListType::Compute]->Execute({ computeCommandList });
	_commandQueues[core::CommandListType::Compute]->Signal(_fence, _fenceValues[_currentFrameIndex] = ++_fenceValue);

	_commandQueues[core::CommandListType::Graphics]->Wait(_fence, _fenceValues[_currentFrameIndex]);
	_commandQueues[core::CommandListType::Graphics]->Execute({graphicsCommandList});
	_commandQueues[core::CommandListType::Graphics]->Signal(_fence, _fenceValues[_currentFrameIndex] = ++_fenceValue);
	
	/*-------------------------------------------------------------------
	-          Flip Screen
	---------------------------------------------------------------------*/
	_swapchain->Present(_fence, _fenceValues[_currentFrameIndex]);

	/*-------------------------------------------------------------------
	-      GPU Command Wait
	---------------------------------------------------------------------*/
	_currentFrameIndex = _swapchain->PrepareNextImage(_fence, ++_fenceValue);
	_fence->Wait(_fenceValue);

}

/****************************************************************************
*                     FlushCommandQueue
*************************************************************************//**
*  @fn        void LowLevelGraphicsEngine::FlushCommandQueue(const rhi::core::CommandListType type)
*
*  @brief     Execute command queue 
*
*  @param[in] const rhi::core::CommandListType type
*
*  @return 　　void
*****************************************************************************/
void LowLevelGraphicsEngine::FlushCommandQueue(const rhi::core::CommandListType type)
{
	// set command lists
	std::vector<std::shared_ptr<rhi::core::RHICommandList>> commandLists;
	const auto& commandList = _commandLists[_currentFrameIndex][type];
	commandLists.push_back(commandList);

	// execute command queue 
	_commandQueues[type]->Execute({commandList});
	_commandQueues[type]->Signal(_fence, _fenceValues[_currentFrameIndex] = ++_fenceValue);
}

/****************************************************************************
*                     OnResize
*************************************************************************//**
*  @fn        void LowLevelGraphicsEngine::OnResize(const size_t newWidth, const size_t newHeight)
*  @brief     Resize swapchain
*  @param[in] size_t newWidth
*  @param[in] size_t newHeight
*  @return 　　void
*****************************************************************************/
void LowLevelGraphicsEngine::OnResize(const size_t newWidth, const size_t newHeight)
{
	_swapchain->Resize(newWidth, newHeight);
}

void LowLevelGraphicsEngine::BeginSwapchainRenderPass()
{
	const auto& graphicsCommandList = _commandLists[_currentFrameIndex][core::CommandListType::Graphics];
	graphicsCommandList->BeginRenderPass(_renderPass, _frameBuffers[_currentFrameIndex]);
}

/****************************************************************************
*                     ShutDown
*************************************************************************//**
*  @fn        void LowLevelGraphicsEngine::ShutDown()
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
	if (_swapchain) { _swapchain.reset(); }
	
	_frameBuffers.clear(); 
	_frameBuffers.shrink_to_fit();

	if (_renderPass) { _renderPass.reset(); }

	/*-------------------------------------------------------------------
	-      Clear command list
	---------------------------------------------------------------------*/
	if (_fence) { _fence.reset(); }
	
	_commandLists.clear(); 
	_commandLists.shrink_to_fit();

	if (!_commandQueues.empty()) { _commandQueues.clear(); }

	/*-------------------------------------------------------------------
	-      Clear Device resources
	---------------------------------------------------------------------*/
	_device->Destroy();
	if (_device)    { _device.reset(); }
	if (_adapter)   { _adapter.reset(); }
	if (_instance)  { _instance.reset(); }

	_hasCalledShutDown = true;
}

#pragma endregion Main Function

#pragma region Private Function
#pragma region SetUp
/****************************************************************************
*                     SetUpHeap
*************************************************************************//**
*  @fn        void LowLevelGraphicsEngine::SetUpHeap()
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
	heapCount.DSVDescCount = _apiVersion == APIVersion::DirectX12 ? DSV_DESC_COUNT : 0;
	heapCount.RTVDescCount = _apiVersion == APIVersion::DirectX12 ? RTV_DESC_COUNT : 0;
	_device->SetUpDefaultHeap(heapCount);

}

/****************************************************************************
*                     SetUpRenderResource
*************************************************************************//**
*  @fn        void LowLevelGraphicsEngine::SetUpRenderResource()
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
	const auto clearDepthColor = core::ClearValue(0.0f, 0.0f, 0.0f, 1.0f);

	/*-------------------------------------------------------------------
	-      Set render pass
	---------------------------------------------------------------------*/
	{
		core::Attachment colorAttachment = core::Attachment::RenderTarget(_pixelFormat);
		core::Attachment depthAttachment = core::Attachment::DepthStencil(_depthStencilFormat);

		// vulkanの場合, 初期RenderTargetはUnlnownである必要があるとのこと
		if (_apiVersion == APIVersion::Vulkan) { colorAttachment.InitialLayout = core::ResourceState::Common; }

		_renderPass = _device->CreateRenderPass(colorAttachment, depthAttachment);
		_renderPass->SetClearValue(clearColor, clearDepthColor);
	}

	/*-------------------------------------------------------------------
	-      set continue drawing render pass (for texture rendering)
	---------------------------------------------------------------------*/
	{
		core::Attachment colorAttachment = core::Attachment::RenderTarget(_pixelFormat, core::ResourceState::RenderTarget,
			core::ResourceState::Present, core::AttachmentLoad::Load);

		core::Attachment depthAttachment = core::Attachment::DepthStencil(_depthStencilFormat, core::ResourceState::Common, 
			core::ResourceState::DepthStencil,core::AttachmentLoad::Load);

		// vulkanの場合, 初期RenderTargetはUnlnownである必要があるとのこと
		if (_apiVersion == APIVersion::Vulkan) { colorAttachment.InitialLayout = core::ResourceState::Common; }

		_drawContinueRenderPass = _device->CreateRenderPass(colorAttachment, depthAttachment);
		_drawContinueRenderPass->SetClearValue(clearColor, clearDepthColor);
	}

	/*-------------------------------------------------------------------
	-      Create frame buffer
	---------------------------------------------------------------------*/
	_frameBuffers.resize(FRAME_BUFFER_COUNT);
	for (size_t i = 0; i < _frameBuffers.size(); ++i)
	{
		/*-------------------------------------------------------------------
		-      Create Depth Texture
		---------------------------------------------------------------------*/
		auto depthInfo = core::GPUTextureMetaData::DepthStencil( Screen::GetScreenWidth(), Screen::GetScreenHeight(), 
			_depthStencilFormat, clearDepthColor);

		if (_apiVersion == APIVersion::Vulkan) { depthInfo.State = core::ResourceState::Common; }

		const auto depthTexture = _device->CreateTexture(depthInfo);

		/*-------------------------------------------------------------------
		-      Create Frame Buffer
		---------------------------------------------------------------------*/
		_frameBuffers[i] = _device->CreateFrameBuffer(_renderPass, _swapchain->GetBuffer(i), depthTexture);
	}
}

#pragma endregion Set Up
#pragma endregion Private Function
