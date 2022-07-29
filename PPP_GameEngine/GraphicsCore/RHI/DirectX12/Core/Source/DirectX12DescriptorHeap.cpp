//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIDescriptorHeap.hpp
///             @brief  EHI DescriptorHeap 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12DescriptorHeap.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include <d3d12.h>
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
RHIDescriptorHeap::RHIDescriptorHeap(const std::shared_ptr<core::RHIDevice>& device, const core::DescriptorHeapType heapType, const size_t maxDescriptorCount)
	: core::RHIDescriptorHeap(device, heapType, maxDescriptorCount)
{
	const auto dxDevice = std::static_pointer_cast<RHIDevice>(_device)->GetDevice();
	/*-------------------------------------------------------------------
	-			     Describe heap desc
	---------------------------------------------------------------------*/
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = static_cast<UINT>(maxDescriptorCount);
	heapDesc.Type           = EnumConverter::Convert(heapType);
	heapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NodeMask       = 0;
	ThrowIfFailed(dxDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(_descriptorHeap.GetAddressOf())));
	_descriptorHeap->SetName(L"DirectX12::DescriptorHeap");
	/*-------------------------------------------------------------------
	-			     Acquire Descriptor Size
	---------------------------------------------------------------------*/
	_descriptorSize = dxDevice->GetDescriptorHandleIncrementSize(EnumConverter::Convert(heapType));
}

RHIDescriptorHeap::RHIDescriptorHeap(const std::shared_ptr<core::RHIDevice>& device, const std::vector<core::DescriptorHeapType>& heapTypes, const std::vector<size_t>& maxDescriptorCounts)
	: core::RHIDescriptorHeap(device, heapTypes, maxDescriptorCounts)
{
	const auto dxDevice = std::static_pointer_cast<RHIDevice>(_device)->GetDevice();
	/*-------------------------------------------------------------------
	-			     Describe heap desc
	---------------------------------------------------------------------*/
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = static_cast<UINT>(maxDescriptorCounts[0]);
	heapDesc.Type           = EnumConverter::Convert(heapTypes[0]);
	heapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NodeMask       = 0;
	ThrowIfFailed(dxDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(_descriptorHeap.GetAddressOf())));
	_descriptorHeap->SetName(L"DirectX12::DescriptorHeap");
	/*-------------------------------------------------------------------
	-			     Acquire Descriptor Size
	---------------------------------------------------------------------*/
	_descriptorSize = dxDevice->GetDescriptorHandleIncrementSize(EnumConverter::Convert(heapTypes[0]));
}

RHIDescriptorHeap::~RHIDescriptorHeap()
{
	_descriptorHeap.Reset();
	_descriptorSize = 0;
}