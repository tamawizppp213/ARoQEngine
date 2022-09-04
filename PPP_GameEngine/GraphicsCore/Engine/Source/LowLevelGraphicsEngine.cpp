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
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandAllocator.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandQueue.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandList.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHISwapchain.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFence.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
void LowLevelGraphicsEngine::StartUp(APIVersion apiVersion, HWND hwnd, HINSTANCE hInstance)
{
	_hwnd = hwnd; _hInstance = hInstance;
	SelectDevice(apiVersion);             // directX12 or vulkan api
	CreateCommandObject();
	CreateSwapchain();
	CreateSyncObject();
	CreateDescriptorHeaps();
}
void LowLevelGraphicsEngine::BeginDrawFrame()
{
	_commandList->BeginRecording();
}
void LowLevelGraphicsEngine::EndDrawFrame()
{
	/*-------------------------------------------------------------------
	-      Finish recording commands list
	---------------------------------------------------------------------*/
	_commandList->EndRecording();
	/*-------------------------------------------------------------------
	-          Execute GPU Command
	---------------------------------------------------------------------*/
	std::vector<std::shared_ptr<rhi::core::RHICommandList>> commandLists;
	commandLists.push_back(_commandList);
	_commandQueue->Execute(commandLists);
	/*-------------------------------------------------------------------
	-          Flip Screen
	---------------------------------------------------------------------*/
	_swapchain->Present();
	_fences[_currentFrameIndex]->Signal(_commandQueue);
}
void LowLevelGraphicsEngine::OnResize()
{

}
void LowLevelGraphicsEngine::ShutDown()
{

}

#pragma region Private Function
#pragma region SetUp
/****************************************************************************
*                     SelectDevice
*************************************************************************//**
*  @fn        void LowLevelGraphicsEngine::SelectDevice(APIVersion version)
*  @brief     Select Device API (APIVersion : DirectX12 or Vulkan)
*  @param[in] APIVersion version
*  @return 　　void
*****************************************************************************/
void LowLevelGraphicsEngine::SelectDevice(APIVersion version)
{
	switch (version)
	{
		case APIVersion::DirectX12: { _device = std::make_shared<rhi::directX12::RHIDevice>(); std::cout << "DirectX12" << std::endl; break; }
		case APIVersion::Vulkan:    { _device = std::make_shared<rhi::vulkan::RHIDevice>();    std::cout << "Vulkan"    << std::endl; break; }
		default: { throw std::runtime_error("Unknown API."); break; }
	}
	_device->Create(_hwnd, _hInstance, _useHDR, _useRayTracing);
}
/****************************************************************************
*                     CreateCommandObject
*************************************************************************//**
*  @fn        void LowLevelGraphicsEngine::CreateCommandObject()
*  @brief     Create Command Object
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void LowLevelGraphicsEngine::CreateCommandObject()
{
	_commandQueue = _device->CreateCommandQueue();
	for (int i = 0; i < FRAME_BUFFER_COUNT; ++i)
	{
		_commandAllocator[i] = _device->CreateCommandAllocator();
	}
	_commandList = _device->CreateCommandList(_commandAllocator[_currentFrameIndex]);
}
/****************************************************************************
*                     CreateSwapchain
*************************************************************************//**
*  @fn        void LowLevelGraphicsEngine::CreateSwapchain()
*  @brief     Create Swapchain
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void LowLevelGraphicsEngine::CreateSwapchain()
{
	rhi::core::WindowInfo  windowInfo = { static_cast<size_t>(Screen::GetScreenWidth()), static_cast<size_t>(Screen::GetScreenHeight()), _hwnd };
	rhi::core::PixelFormat pixelFormat = _pixelFormat;
	_swapchain = _device->CreateSwapchain(_commandQueue, windowInfo, pixelFormat, FRAME_BUFFER_COUNT, VSYNC);
	
}
/****************************************************************************
*                     CreateSyncObject
*************************************************************************//**
*  @fn        void LowLevelGraphicsEngine::CreateSyncObject()
*  @brief     Create Syncronization
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void LowLevelGraphicsEngine::CreateSyncObject()
{
	for (int i = 0; i < FRAME_BUFFER_COUNT; ++i)
	{
		_fences[i] = _device->CreateFence();
	}
}
void LowLevelGraphicsEngine::CreateDescriptorHeaps()
{
	switch (_apiVersion)
	{
		case APIVersion::DirectX12:
		{
			_cbvSrvUavHeap = _device->CreateDescriptorHeap(core::DescriptorHeapType::CBV_SRV_UAV, CBV_DESC_COUNT + SRV_DESC_COUNT + UAV_DESC_COUNT);
			_rtvHeap       = _device->CreateDescriptorHeap(core::DescriptorHeapType::RTV, RTV_DESC_COUNT);
			_dsvHeap       = _device->CreateDescriptorHeap(core::DescriptorHeapType::DSV, DSV_DESC_COUNT);
			break;
		}
		case APIVersion::Vulkan:
		{
			std::vector<core::DescriptorHeapType> heapTypes = { core::DescriptorHeapType::CBV, core::DescriptorHeapType::SRV, core::DescriptorHeapType::UAV };
			std::vector<size_t>         maxDescriptorCounts = { CBV_DESC_COUNT, SRV_DESC_COUNT, UAV_DESC_COUNT };
			_cbvSrvUavHeap = _device->CreateDescriptorHeap(heapTypes, maxDescriptorCounts);
			_rtvHeap = nullptr;
			_dsvHeap = nullptr;
			break;
		}
	}
}
#pragma endregion Set Up
#pragma endregion Private Function
