//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12ResourceLayout.cpp
///             @brief  Resource Layout
///             @author Toide Yutaro
///             @date   2022_08_09
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DirectX12Memory.hpp"
#include "../Include/DirectX12Device.hpp"
#include "../Include/DirectX12Debug.hpp"
#include "../Include/DirectX12EnumConverter.hpp"
#include <d3d12.h>
#include <stdexcept>
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
RHIMemory::RHIMemory(const std::shared_ptr<core::RHIDevice>& device, const core::MemoryHeap heapType, const std::uint64_t memorySize, std::uint32_t memoryTypeBits)
	: core::RHIMemory(device, heapType, memorySize)
{
	const auto dxDevice = std::static_pointer_cast<directX12::RHIDevice>(device)->GetDevice();
	const D3D12_HEAP_DESC desc =
	{
		.SizeInBytes = memorySize,
		.Properties  = EnumConverter::Convert(heapType),
		.Alignment   = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT,
		.Flags       = D3D12_HEAP_FLAG_NONE
	};

	ThrowIfFailed(dxDevice->CreateHeap(&desc, IID_PPV_ARGS(&_heap)));
	
}

#pragma endregion Constructor and Destructor