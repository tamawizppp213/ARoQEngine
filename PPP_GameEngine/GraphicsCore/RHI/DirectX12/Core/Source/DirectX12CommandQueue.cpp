//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12CommandQueue.hpp
///             @brief  Send drawing commands to the GPU,
///                     provide methods for synchronous processing of drawing command execution
///                     There are three types : Graphics, Compute, Copy command queue.
///             @author Toide Yutaro
///             @date   2022_09_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandQueue.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandList.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Fence.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace Microsoft::WRL;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHICommandQueue::RHICommandQueue(const std::shared_ptr<rhi::core::RHIDevice>& device, core::CommandListType type) : rhi::core::RHICommandQueue(type)
{
	const auto dxDevice = static_cast<RHIDevice*>(device.get())->GetDevice();

	D3D12_COMMAND_LIST_TYPE dxCommandListType = EnumConverter::Convert(_commandListType);

	/*-------------------------------------------------------------------
	-                   Set up command queue info
	---------------------------------------------------------------------*/
	D3D12_COMMAND_QUEUE_DESC cmdQDesc = {};
	cmdQDesc.NodeMask = 0;                                   // Single GPU
	cmdQDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL; // Command queue priority (今後変えるかも)
	cmdQDesc.Type     = dxCommandListType;                   // Enable to execute all command 
	cmdQDesc.Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE;       // Default 

	/*-------------------------------------------------------------------
	-                   Create command queue
	---------------------------------------------------------------------*/
	ThrowIfFailed(dxDevice->CreateCommandQueue(&cmdQDesc, IID_PPV_ARGS(&_commandQueue)));
	_commandQueue->SetName(L"DirectX12::CommandQueue");
}

RHICommandQueue::~RHICommandQueue()
{
	if (_commandQueue) { _commandQueue.Reset(); }
}
#pragma endregion Constructor and Destructor
#pragma region Execute Function
/****************************************************************************
*							Wait
*************************************************************************//**
*  @fn        void RHICommandQueue::Wait(const std::shared_ptr<core::RHIFence>& fence, std::uint64_t value)
*  @brief     Used to wait for another Command queue to complete execution. (in GPU)
*  @param[in] const std::shared_ptr<core::RHIFence>& fence
*  @param[in] std::uint64_t value
*  @return 　　void
*****************************************************************************/
void RHICommandQueue::Wait(const std::shared_ptr<core::RHIFence>& fence, std::uint64_t value)
{
	FenceComPtr dxFence = static_pointer_cast<RHIFence>(fence)->GetFence();
	ThrowIfFailed(_commandQueue->Wait(dxFence.Get(), value));
}
/****************************************************************************
*							Signal
*************************************************************************//**
*  @fn        void RHICommandQueue::Signal(const std::shared_ptr<core::RHIFence>& fence, std::uint64_t value)
*  @brief     Update the fence value (value) when the Command Queue execution completes.
*  @param[in] const std::shared_ptr<core::RHIFence>& fence
*  @param[in] std::uint64_t value
*  @return 　　void
*****************************************************************************/
void RHICommandQueue::Signal(const std::shared_ptr<core::RHIFence>& fence, std::uint64_t value)
{
	FenceComPtr dxFence = static_pointer_cast<RHIFence>(fence)->GetFence();
	ThrowIfFailed(_commandQueue->Signal(dxFence.Get(), value));
}
/****************************************************************************
*							Execute
*************************************************************************//**
*  @fn        void RHICommandQueue::Execute(const std::vector<std::shared_ptr<rhi::core::RHICommandList>>& commandLists)
*  @brief     Execute command list contents. normally set graphics, compute, transfer commandlist
*  @param[in] const std::vector<std::shared_ptr<rhi::core::RHICommandList>>& commandLists
*  @return 　　void
*****************************************************************************/
void RHICommandQueue::Execute(const std::vector<std::shared_ptr<rhi::core::RHICommandList>>& commandLists)
{
	if (commandLists.empty()) { return; }

	/*-------------------------------------------------------------------
	-           Push back command list pointer 
	-      to use directX12 commandlist (ID3D12CommandList pointer)
	---------------------------------------------------------------------*/
	std::vector<ID3D12CommandList*> dxCommandLists;
	for (auto& list : commandLists)
	{
		dxCommandLists.push_back(static_pointer_cast<rhi::directX12::RHICommandList>(list)->GetCommandList().Get());
	}
	/*-------------------------------------------------------------------
	-                   Execute command lists
	---------------------------------------------------------------------*/
	if (dxCommandLists.empty()) { return; }
	_commandQueue->ExecuteCommandLists(static_cast<UINT>(dxCommandLists.size()), dxCommandLists.data());
}
#pragma endregion Execute