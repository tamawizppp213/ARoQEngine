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
#include "../Include/DirectX12CommandQueue.hpp"
#include "../Include/DirectX12CommandList.hpp"
#include "../Include/DirectX12EnumConverter.hpp"
#include "../Include/DirectX12Fence.hpp"
#include "../Include/DirectX12Device.hpp"
#include "../Include/DirectX12Debug.hpp"
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
RHICommandQueue::RHICommandQueue(const std::shared_ptr<rhi::core::RHIDevice>& device, core::CommandListType type, const std::wstring& name) : rhi::core::RHICommandQueue(device, type)
{
#ifdef _DEBUG
	assert(device);
	assert(type != core::CommandListType::Unknown);
#endif

	const auto dxDevice = static_cast<RHIDevice*>(device.get())->GetDevice();

	D3D12_COMMAND_LIST_TYPE dxCommandListType = EnumConverter::Convert(_commandListType);

	/*-------------------------------------------------------------------
	-                   Set up command queue info
	---------------------------------------------------------------------*/
	const D3D12_COMMAND_QUEUE_DESC cmdQDesc =
	{
		.Type     = dxCommandListType,                    // Enable to execute all command 
		.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,  // Command queue priority (今後変えるかも)
		.Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE,        // Default 
		.NodeMask = 0                                     // Single GPU
	};

	/*-------------------------------------------------------------------
	-                   Create command queue
	---------------------------------------------------------------------*/
	ThrowIfFailed(dxDevice->CreateCommandQueue(&cmdQDesc, IID_PPV_ARGS(&_commandQueue)));
	_commandQueue->SetName(name.c_str());
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
* 
*  @brief     Used to wait for another Command queue to complete execution. (in GPU)
*             FenceのWaitはCPU側も処理が止まりますが, CommandQueueのWaitは, 指定したValue以上の値になるまで, GPU内のみで処理を止めることになります. 
* 
*  @param[in] const std::shared_ptr<core::RHIFence>& fence
*  @param[in] std::uint64_t value
* 
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
* 
*  @brief     Update value when the Command Queue execution completes.
*             GPU内で処理が完結します.　
* 
*  @param[in] const std::shared_ptr<core::RHIFence>& fence
* 
*  @param[in] std::uint64_t value
* 
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
* 
*  @brief     Execute command list contents. normally set graphics, compute, transfer commandlist
* 
*  @param[in] const std::vector<std::shared_ptr<rhi::core::RHICommandList>>& commandLists
* 
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

#pragma region Property
void RHICommandQueue::SetName(const std::wstring& name)
{
	_commandQueue->SetName(name.c_str());
}

#pragma endregion Property