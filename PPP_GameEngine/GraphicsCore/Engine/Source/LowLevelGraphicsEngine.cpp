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
void LowLevelGraphicsEngine::StartUp(APIVersion apiVersion, HWND hwnd, HINSTANCE hInstance)
{
	_hwnd = hwnd; _hInstance = hInstance; _apiVersion = apiVersion;
	/*-------------------------------------------------------------------
	-      Select proper physical device 
	---------------------------------------------------------------------*/
	_instance = rhi::core::RHIInstance::CreateInstance(apiVersion, true, false);
	_instance->LogAdapters();
	_adapter = _instance->SearchHighPerformanceAdapter();
	/*-------------------------------------------------------------------
	-      Create device resources
	---------------------------------------------------------------------*/
	_device                = _adapter->CreateDevice(FRAME_BUFFER_COUNT);
	_commandQueues[CommandListType::Graphics] = _device->GetCommandQueue(CommandListType::Graphics);
	_commandQueues[CommandListType::Compute]  = _device->GetCommandQueue    (CommandListType::Compute);
	_commandQueues[CommandListType::Copy]     = _device->GetCommandQueue    (CommandListType::Copy);
	_fence                 = _device->CreateFence();
	/*-------------------------------------------------------------------
	-      Set up swapchain
	---------------------------------------------------------------------*/
	core::WindowInfo windowInfo = core::WindowInfo(Screen::GetScreenWidth(), Screen::GetScreenHeight(), _hwnd, _hInstance);

	_swapchain = _device->CreateSwapchain( _commandQueues[CommandListType::Graphics], windowInfo,
		_pixelFormat,  FRAME_BUFFER_COUNT, VSYNC, false);
	_pixelFormat = _swapchain->GetPixelFormat(); // sdrの場合は修正する/

	SetUpHeap();
	SetUpRenderResource();

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
*  @brief     The first call to the Draw function generates the back buffer image and executes the Default render pass.
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void LowLevelGraphicsEngine::BeginDrawFrame()
{
	_currentFrameIndex = _swapchain->PrepareNextImage(_fence, ++_fenceValue);
	/*-------------------------------------------------------------------
	-      GPU Command Wait
	---------------------------------------------------------------------*/
	_commandQueues[core::CommandListType::Graphics]->Wait(_fence, _fenceValue);
	_fence->Wait(_fenceValues[_currentFrameIndex]);
	/*-------------------------------------------------------------------
	-      Start Recording Command List 
	---------------------------------------------------------------------*/
	const auto& graphicsCommandList = _commandLists[_currentFrameIndex][core::CommandListType::Graphics];
	graphicsCommandList->BeginRecording();
	graphicsCommandList->BeginRenderPass(_renderPass, _frameBuffers[_currentFrameIndex]);
	
}
/****************************************************************************
*                     EndDrawFrame
*************************************************************************//**
*  @fn        void LowLevelGraphicsEngine::EndDrawFrame()
*  @brief     Call at the end of the Draw function to execute the command list and Flip the Swapchain. 
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void LowLevelGraphicsEngine::EndDrawFrame()
{
	/*-------------------------------------------------------------------
	-      Finish recording commands list
	---------------------------------------------------------------------*/
	const auto& graphicsCommandList = _commandLists[_currentFrameIndex][core::CommandListType::Graphics];
	graphicsCommandList->EndRenderPass();
	graphicsCommandList->EndRecording();

	/*-------------------------------------------------------------------
	-          Execute GPU Command
	---------------------------------------------------------------------*/
	std::vector<std::shared_ptr<rhi::core::RHICommandList>> commandLists;
	commandLists.push_back(graphicsCommandList);
	_commandQueues[core::CommandListType::Graphics]->Execute({graphicsCommandList});
	_commandQueues[core::CommandListType::Graphics]->Signal(_fence, _fenceValues[_currentFrameIndex] = ++_fenceValue);
	/*-------------------------------------------------------------------
	-          Flip Screen
	---------------------------------------------------------------------*/
	_swapchain->Present(_fence, _fenceValues[_currentFrameIndex]);
}

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
void LowLevelGraphicsEngine::ShutDown()
{
	if (_hasCalledShutDown) { return; }

	// finish all render command queue
	_commandQueues[CommandListType::Graphics]->Signal(_fence, _fenceValues[_currentFrameIndex]);
	_commandQueues[CommandListType::Compute] ->Signal(_fence, _fenceValues[_currentFrameIndex]);
	_commandQueues[CommandListType::Copy]    ->Signal(_fence, _fenceValues[_currentFrameIndex]);
	_fence->Wait(_fenceValue);
	
	if (_swapchain) { _swapchain.reset(); }
	_frameBuffers.clear(); _frameBuffers.shrink_to_fit();
	if (_renderPass) { _renderPass.reset(); }
	_commandLists.clear(); _commandLists.shrink_to_fit();
	
	if (_fence)                { _fence               .reset(); }
	if (!_commandQueues.empty()) { _commandQueues.clear(); }
	_device->Destroy();
	if (_device)    { _device.reset(); }
	if (_adapter)   { _adapter.reset(); }
	if (_instance)  { _instance.reset(); }

	_hasCalledShutDown = true;
}

#pragma region Private Function
#pragma region SetUp
/****************************************************************************
*                     SetUpHeap
*************************************************************************//**
*  @fn        void LowLevelGraphicsEngine::SetUpHeap()
*  @brief     Prepare Logical Device's Default Heap. (Each size is defined by this class static variables X_DESC_COUNT)
*  @param[in] void
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
*  @brief     Prepare render pass and frame buffer
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void LowLevelGraphicsEngine::SetUpRenderResource()
{
	// set render pass
	core::Attachment colorAttachment = core::Attachment::RenderTarget(_pixelFormat);
	core::Attachment depthAttachment = core::Attachment::DepthStencil(_depthStencilFormat);

	// vulkanの場合, 初期RenderTargetはUnlnownである必要があるとのこと
	if (_apiVersion == APIVersion::Vulkan) { colorAttachment.InitialLayout = core::ResourceState::Common; }
	_renderPass = _device->CreateRenderPass(colorAttachment, depthAttachment);
	_renderPass->SetClearValue(core::ClearValue(0.0f, 0.3f, 0.3f, 1.0f), core::ClearValue(0.0f, 0.0f, 0.0f, 1.0f));

	// resize 
	_frameBuffers.resize(FRAME_BUFFER_COUNT);
	for (size_t i = 0; i < _frameBuffers.size(); ++i)
	{
		auto depthInfo    = core::GPUTextureMetaData::DepthStencil(
			Screen::GetScreenWidth(), Screen::GetScreenHeight(), 
			_depthStencilFormat, core::ClearValue(0.0f, 0.0f, 0.0f, 1.0f));

		if (_apiVersion == APIVersion::Vulkan) { depthInfo.State = core::ResourceState::Common; }

		const auto depthTexture = _device->CreateTexture(depthInfo);
		// set frameBuffer
		_frameBuffers[i] = _device->CreateFrameBuffer(_renderPass, _swapchain->GetBuffer(i), depthTexture);
	}
}

#pragma endregion Set Up
#pragma endregion Private Function
