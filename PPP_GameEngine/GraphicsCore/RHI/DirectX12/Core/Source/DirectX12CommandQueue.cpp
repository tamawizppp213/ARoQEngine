//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12CommandQueue.cpp
///             @brief  Command queue 
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandQueue.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandList.hpp"
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
RHICommandQueue::RHICommandQueue(const std::shared_ptr<rhi::core::RHIDevice>& device) : rhi::core::RHICommandQueue(device)
{
	const auto dxDevice = static_cast<RHIDevice*>(_device.get())->GetDevice();

	/*-------------------------------------------------------------------
	-                   Set up command queue info
	---------------------------------------------------------------------*/
	D3D12_COMMAND_QUEUE_DESC cmdQDesc = {};
	cmdQDesc.NodeMask = 0;                                   // Single GPU
	cmdQDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL; // Command queue priority
	cmdQDesc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;      // Enable to execute all command 
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
*							Execute
*************************************************************************//**
*  @fn        void RHICommandQueue::Execute(const std::vector<std::shared_ptr<rhi::core::RHICommandList>>& commandLists)
*  @brief     Execute command queue 
*  @param[in] const std::vector<std::shared_ptr<rhi::core::RHICommandList>>& commandLists
*  @return Å@Å@void
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
	_commandQueue->ExecuteCommandLists(static_cast<UINT>(dxCommandLists.size()), dxCommandLists.data());
}
#pragma endregion Execute