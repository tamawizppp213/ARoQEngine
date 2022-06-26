//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12CommandList.cpp
///             @brief  CommandList 
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandList.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandAllocator.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include <d3d12.h>
#include <dxgi1_6.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace Microsoft::WRL;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
RHICommandList::~RHICommandList()
{
	if (_commandList) { _commandList.Reset(); }
}

RHICommandList::RHICommandList(const std::shared_ptr<rhi::core::RHIDevice>& device, const std::shared_ptr<rhi::core::RHICommandAllocator>& commandAllocator) : 
	rhi::core::RHICommandList(device, commandAllocator)
{
	const auto dxDevice    = static_cast<RHIDevice*>(_device.get())->GetDevice();
	const auto dxAllocator = static_cast<RHICommandAllocator*>(_commandAllocator.get())->GetAllocator();

	ThrowIfFailed(dxDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		dxAllocator.Get(), // Associated command allocator
		nullptr,                                     // Initial PipeLine State Object
		IID_PPV_ARGS(_commandList.GetAddressOf())));
	_commandList->SetName(L"DirectX12::CommandList");
	_commandList->Close();
}

