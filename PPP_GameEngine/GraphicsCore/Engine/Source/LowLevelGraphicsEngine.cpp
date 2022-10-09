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
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFence.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIDescriptorHeap.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Destructor
LowLevelGraphicsEngine::~LowLevelGraphicsEngine()
{
	if (_dsvHeap)              { _dsvHeap             .reset(); }
	if (_rtvHeap)              { _rtvHeap             .reset(); }
	if (_csvSrvUavHeap)        { _csvSrvUavHeap       .reset(); }
	if (_renderPass)           { _renderPass          .reset(); }
	if (_swapchain)            { _swapchain           .reset(); }
	if (_fence)                { _fence               .reset(); }
	if (_graphicsCommandQueue) { _graphicsCommandQueue.reset(); }
	if (_computeCommandQueue)  { _computeCommandQueue .reset(); }
	_device->Destroy();
	if (_device)    { _device.reset(); }
	if (_adapter)   { _adapter.reset(); }
	if (_instance)  { _instance.reset(); }
}
#pragma endregion Destructor
void LowLevelGraphicsEngine::StartUp(APIVersion apiVersion, HWND hwnd, HINSTANCE hInstance)
{
	_hwnd = hwnd; _hInstance = hInstance; _apiVersion = apiVersion;
	/*-------------------------------------------------------------------
	-      Select proper physical device 
	---------------------------------------------------------------------*/
	_instance = rhi::core::RHIInstance::CreateInstance(APIVersion::Vulkan, true, false);
	_instance->LogAdapters();
	_adapter = _instance->SearchHighPerformanceAdapter();
	/*-------------------------------------------------------------------
	-      Create device resources
	---------------------------------------------------------------------*/
	_device = _adapter->CreateDevice(FRAME_BUFFER_COUNT);
	_graphicsCommandQueue     = _device->GetCommandQueue    (CommandListType::Graphics);
	_computeCommandQueue      = _device->GetCommandQueue    (CommandListType::Compute);
	_fence                    = _device->CreateFence();
	
	core::WindowInfo windowInfo = core::WindowInfo(Screen::GetScreenWidth(), Screen::GetScreenHeight(), _hwnd, _hInstance);
	_swapchain = _device->CreateSwapchain(
		_graphicsCommandQueue, windowInfo, 
		core::PixelFormat::R16G16B16A16_FLOAT, 
		FRAME_BUFFER_COUNT, VSYNC, false);
	SetUpRenderPass();
	SetUpHeap();

}
void LowLevelGraphicsEngine::BeginDrawFrame()
{
	//_commandList->BeginRecording();
}
void LowLevelGraphicsEngine::EndDrawFrame()
{
	///*-------------------------------------------------------------------
	//-      Finish recording commands list
	//---------------------------------------------------------------------*/
	//_commandList->EndRecording();
	///*-------------------------------------------------------------------
	//-          Execute GPU Command
	//---------------------------------------------------------------------*/
	//std::vector<std::shared_ptr<rhi::core::RHICommandList>> commandLists;
	//commandLists.push_back(_commandList);
	//_commandQueue->Execute(commandLists);
	///*-------------------------------------------------------------------
	//-          Flip Screen
	//---------------------------------------------------------------------*/
	//_swapchain->Present();
	////_fences[_currentFrameIndex]->Signal(_commandQueue);
}
void LowLevelGraphicsEngine::OnResize()
{

}
void LowLevelGraphicsEngine::ShutDown()
{

}

#pragma region Private Function
#pragma region SetUp
void LowLevelGraphicsEngine::SetUpRenderPass()
{
	core::Attachment colorAttachment = {};
	colorAttachment.RenderTarget(_pixelFormat);
	_renderPass = _device->CreateRenderPass(colorAttachment, std::nullopt);
}

void LowLevelGraphicsEngine::SetUpHeap()
{
	std::map<core::DescriptorHeapType, size_t> heapInfoList;
	heapInfoList[core::DescriptorHeapType::CBV] = CBV_DESC_COUNT;
	heapInfoList[core::DescriptorHeapType::SRV] = SRV_DESC_COUNT;
	heapInfoList[core::DescriptorHeapType::UAV] = UAV_DESC_COUNT;
	_csvSrvUavHeap = _device->CreateDescriptorHeap(heapInfoList);
	if (_apiVersion == APIVersion::DirectX12)
	{
		_rtvHeap = _device->CreateDescriptorHeap(core::DescriptorHeapType::RTV, RTV_DESC_COUNT);
		_dsvHeap = _device->CreateDescriptorHeap(core::DescriptorHeapType::DSV, DSV_DESC_COUNT);
	}
}
#pragma endregion Set Up
#pragma endregion Private Function
