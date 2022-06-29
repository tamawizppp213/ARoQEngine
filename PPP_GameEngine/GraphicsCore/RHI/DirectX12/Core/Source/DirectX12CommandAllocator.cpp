//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12CommandAllocator.cpp
///             @brief  Command Allocator
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandAllocator.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include <d3d12.h>
#include <dxgi1_6.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::directX12;
using namespace rhi;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
RHICommandAllocator::~RHICommandAllocator()
{
	if (_commandAllocator) { _commandAllocator.Reset(); }
}

RHICommandAllocator::RHICommandAllocator(const std::shared_ptr<rhi::core::RHIDevice>& device) : rhi::core::RHICommandAllocator(device)
{
	const auto dxDevice = static_cast<rhi::directX12::RHIDevice*>(_device.get())->GetDevice();

	ThrowIfFailed(dxDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,              // Enable to execute all command 
		IID_PPV_ARGS(&_commandAllocator)));
	_commandAllocator->SetName(L"DirectX12::CommandAllocator");
}

void RHICommandAllocator::Reset()
{
	ThrowIfFailed(_commandAllocator->Reset());
}