//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPUBuffer.hpp
///             @brief  GPU Buffer 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUBuffer::GPUBuffer(const std::shared_ptr<core::RHIDevice>& device, std::uint32_t stride, std::uint32_t count, core::BufferType bufferType)
	:core::GPUBuffer(device, stride, count, bufferType)
{
	/*-------------------------------------------------------------------
	-           Set heap property
	---------------------------------------------------------------------*/
	D3D12_HEAP_PROPERTIES heapProp = {};
	//heapProp.Type = EnumConverter::Convert(_memoryHeap);
	heapProp.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask     = 1;
	heapProp.VisibleNodeMask      = 1;
	/*-------------------------------------------------------------------
	-           Set resource desc
	---------------------------------------------------------------------*/
	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension          = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment          = 0;
	resourceDesc.Width              = static_cast<UINT64>(GetTotalByteSize());
	resourceDesc.Height             = 1;
	resourceDesc.DepthOrArraySize   = 1;
	resourceDesc.MipLevels          = 1;
	resourceDesc.Format             = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count   = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout             = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags              = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	/*-------------------------------------------------------------------
	-           Create
	---------------------------------------------------------------------*/
	auto dxDevice = static_cast<rhi::directX12::RHIDevice*>(_device.get())->GetDevice();
	ThrowIfFailed(dxDevice->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,                                          // Generic Read
		nullptr,
		IID_PPV_ARGS(&_resource)));
}

#pragma region Public Function
void GPUBuffer::CopyStart()
{
	ThrowIfFailed(_resource->Map(0, nullptr, reinterpret_cast<void**>(&_mappedData)));
}
void GPUBuffer::CopyData(int elementIndex, const void* data)
{
	std::memcpy(&_mappedData[elementIndex * _stride], data, _stride);
}
void GPUBuffer::CopyTotalData(const void* data, int dataLength)
{
	std::memcpy(&_mappedData[0], data, _stride * (size_t)dataLength);
}
void GPUBuffer::CopyEnd()
{
	_resource->Unmap(0, nullptr);
}
#pragma endregion Public Function