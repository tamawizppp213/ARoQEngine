//////////////////////////////////////////////////////////////////////////////////
//              @file   GraphicsCoreEngine.cpp
///             @brief  Graphics Low Level Engine
///             @author Toide Yutaro
///             @date   -
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/Engine/Include/GraphicsCoreEngine.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12GraphicsDevice.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12CommandContext.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12Debug.hpp"
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Graphics Start Up and Shut Down 
/****************************************************************************
*							StartUp
*************************************************************************//**
*  @fn        void GraphicsCoreEngine::StartUp(HWND hwnd)
*  @brief     Start up grahic engine
*  @param[in] HWND hwnd
*  @return 　　void
*****************************************************************************/
void GraphicsCoreEngine::StartUp(HWND hwnd)
{
	if (_hasInitialized) { return; }

	/*-------------------------------------------------------------------
	-            Initialize GraphicsDevice DirectX12
	---------------------------------------------------------------------*/
	_device = std::make_unique<GraphicsDeviceDirectX12>();
	_device->Initialize(hwnd);
	/*-------------------------------------------------------------------
	-            Initialize CommandContext
	---------------------------------------------------------------------*/
	_commandContext = std::make_unique<CommandContext>();
	_commandContext->Initialize(_device.get());
	/*-------------------------------------------------------------------
	-            Complete Initialize
	---------------------------------------------------------------------*/
	_hasInitialized = true;
}
/****************************************************************************
*							OnResize
*************************************************************************//**
*  @fn        void GraphicsCoreEngine::OnResize()
*  @brief     Callback on resize screen window size
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsCoreEngine::OnResize()
{
	_device->OnResize();
}
/****************************************************************************
*							ShutDown
*************************************************************************//**
*  @fn        void GraphicsCoreEngine::ShutDown()
*  @brief     Shut down graphic engine
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsCoreEngine::ShutDown()
{
	_device->Finalize();
	_commandContext.reset();
	_device.reset();
}
#pragma endregion Graphics Start Up and Shut Down 
#pragma region Each Render Scene Command Interface
void GraphicsCoreEngine::OnInitializeRenderScene()
{
	_device->CompleteInitialize();
}
void GraphicsCoreEngine::BeginDrawFrame()
{
	_device->ClearScreen();
}
void GraphicsCoreEngine::EndDrawFrame()
{
	_device->CompleteRendering();
}
void GraphicsCoreEngine::OnTerminateRenderScene()
{
	_device->OnTerminateRenderScene();
}
void GraphicsCoreEngine::WaitNextFrame()
{
	_device->FlushCommandQueue();
}
#pragma endregion Each Render Scene Command Interface
#pragma region Create Function

UINT GraphicsCoreEngine::CreateShaderResourceView(GPUResource* resource, const D3D12_SHADER_RESOURCE_VIEW_DESC* desc)
{
	UINT id = _device->IssueViewID(HeapFlag::SRV);
	_device->GetDevice()->CreateShaderResourceView(resource->GetResource(), desc, _device->GetCPUResourceView(HeapFlag::SRV, id));
	return id;
}
void GraphicsCoreEngine::CreateShaderResourceView(GPUResource* resource, const D3D12_SHADER_RESOURCE_VIEW_DESC* desc, D3D12_CPU_DESCRIPTOR_HANDLE destDescriptor)
{
	_device->GetDevice()->CreateShaderResourceView(resource->GetResource(), desc, destDescriptor);
}
void GraphicsCoreEngine::CreateCommittedResource(const D3D12_HEAP_PROPERTIES* heapProperties, D3D12_HEAP_FLAGS heapFlags, const D3D12_RESOURCE_DESC* descriptor, D3D12_RESOURCE_STATES initialResourceState, const D3D12_CLEAR_VALUE* optimizedClearValue, const IID& riidResource, void** resource)
{
	ThrowIfFailed(_device->GetDevice()->CreateCommittedResource(heapProperties, heapFlags, descriptor, initialResourceState, optimizedClearValue, riidResource, resource));
}
#pragma endregion Create Function

void GraphicsCoreEngine::ExecuteCommandContext()
{
	_commandContext->Close();
	ID3D12CommandList* commandList[] = { _commandContext.get()->GetCommandList()};
	_device->GetCommandQueue()->ExecuteCommandLists(_countof(commandList), commandList);
}
void GraphicsCoreEngine::ResetCommandList()
{
	_device->ResetCommandList();
}

#pragma region Graphics Engine Property
INT GraphicsCoreEngine::GetCurrentFrameIndex()
{
	return _device->GetCurrentFrameIndex();
}
INT GraphicsCoreEngine::GetNextFrameBufferIndex()
{
	return _device->GetCurrentBackBufferIndex();
}
#pragma endregion Graphics Engine Property