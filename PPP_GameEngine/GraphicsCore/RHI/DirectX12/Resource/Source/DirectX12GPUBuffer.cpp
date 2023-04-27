//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPUBuffer.hpp
///             @brief  GPU Buffer 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DirectX12GPUBuffer.hpp"
#include "../../Core/Include/DirectX12Debug.hpp"
#include "../../Core/Include/DirectX12EnumConverter.hpp"
#include "../../Core/Include/DirectX12Device.hpp"
#include "../../Core/Include/DirectX12CommandList.hpp"
#include "../../Core/Include/DirectX12BaseStruct.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUBuffer::GPUBuffer(const std::shared_ptr<core::RHIDevice>& device, const core::GPUBufferMetaData& metaData, const std::wstring& name)
	:core::GPUBuffer(device, metaData, name)
{
	/*-------------------------------------------------------------------
	-           Set heap property
	---------------------------------------------------------------------*/
	D3D12_HEAP_PROPERTIES heapProp
	{
		.Type                 = EnumConverter::Convert(_metaData.HeapType), 
		.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN,
		.CreationNodeMask     = 1,
		.VisibleNodeMask      = 1
	};

	/*-------------------------------------------------------------------
	-           Set resource desc
	---------------------------------------------------------------------*/
	const D3D12_RESOURCE_DESC resourceDesc = 
	{
		.Dimension          = D3D12_RESOURCE_DIMENSION_BUFFER,
		.Alignment          = 0,
		.Width              = static_cast<UINT64>(GetTotalByteSize()),
		.Height             = 1, // For 1D buffer
		.DepthOrArraySize   = 1, // For 1D buffer
		.MipLevels          = 1,
		.Format             = DXGI_FORMAT_UNKNOWN,
		.SampleDesc         = {1, 0},
		.Layout             = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		.Flags              = EnumConverter::Convert(metaData.ResourceUsage)
	};

	/*-------------------------------------------------------------------
	-           Create
	---------------------------------------------------------------------*/
	auto dxDevice = static_cast<rhi::directX12::RHIDevice*>(_device.get())->GetDevice();
	ThrowIfFailed(dxDevice->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDesc,
		EnumConverter::Convert(metaData.State),                                          // Generic Read
		nullptr,
		IID_PPV_ARGS(&_resource)));

	_resource->SetName(name.c_str());
}

GPUBuffer::~GPUBuffer()
{
	if (_intermediateBuffer) { _intermediateBuffer.Reset(); }
	if (_resource)           { _resource.Reset(); }
}
#pragma region Public Function
/****************************************************************************
*                     CopyStart
*************************************************************************//**
*  @fn        void GPUBuffer::CopyStart()
* 
*  @brief     Call Map Function
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void GPUBuffer::CopyStart()
{
#if _DEBUG
	assert(_metaData.IsCPUAccessible());
#endif
	ThrowIfFailed(_resource->Map(0, nullptr, reinterpret_cast<void**>(&_mappedData)));
}
/****************************************************************************
*                     CopyData
*************************************************************************//**
*  @fn        void GPUBuffer::CopyData()
* 
*  @brief     GPU copy to one element
* 
*  @param[in] const void* dataPtr
* 
*  @param[in] const size_t elementIndex
* 
*  @return 　　void
*****************************************************************************/
void GPUBuffer::CopyData(const void* data, const size_t elementIndex)
{
#ifdef _DEBUG
	assert(elementIndex <= _metaData.Count);
#endif
	std::memcpy(&_mappedData[elementIndex * _metaData.Stride], data, _metaData.Stride);
}
/****************************************************************************
*                     CopyTotalData
*************************************************************************//**
*  @fn        void GPUBuffer::CopyData()
* 
*  @brief     GPU copy the specified range
* 
*  @param[in] void*  dataPtr
* 
*  @param[in] size_t dataLength
* 
*  @param[in] size_t indexOffset (default 0)
* 
*  @return 　　void
*****************************************************************************/
void GPUBuffer::CopyTotalData(const void* data, const size_t dataLength, const size_t indexOffset)
{
#if _DEBUG
	assert(dataLength + indexOffset <= _metaData.Count);
#endif
	std::memcpy(&_mappedData[indexOffset * _metaData.Stride], data, _metaData.Stride * (size_t)dataLength);
}
/****************************************************************************
*                     CopyEnd
*************************************************************************//**
*  @fn        void GPUBuffer::CopyEnd()
* 
*  @brief     Call UnMap Function
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void GPUBuffer::CopyEnd()
{
	_resource->Unmap(0, nullptr);
}

void GPUBuffer::SetName(const std::wstring& name)
{
	ThrowIfFailed(_resource->SetName(name.c_str()));
}

void GPUBuffer::Pack(const void* data, const std::shared_ptr<core::RHICommandList>& copyCommandList)
{

	auto dxDevice = static_cast<rhi::directX12::RHIDevice*>(_device.get())->GetDevice();
	if ((!_metaData.IsCPUAccessible()))
	{
#if _DEBUG
		assert(copyCommandList->GetType() == core::CommandListType::Copy);
#endif

		/*-------------------------------------------------------------------
		-          Copy CPU memory data into our dafault buffer,
		-          we need to create an intermediate upload heap
		---------------------------------------------------------------------*/
		auto uploadHeapProp   = HEAP_PROPERTY(D3D12_HEAP_TYPE_UPLOAD);
		auto uploadTempBuffer = RESOURCE_DESC::Buffer(GetTotalByteSize());
		// CreateCommittedResource -> Heap確保 + 実際にGPUにメモリを書き込むを両方やってくれる.
		ThrowIfFailed(dxDevice->CreateCommittedResource(
			&uploadHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&uploadTempBuffer,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(_intermediateBuffer.GetAddressOf())));
		_intermediateBuffer->SetName(L"Intermediate Buffer");

		/*-------------------------------------------------------------------
		-      Describe the data we want to copy into the default buffer.
		---------------------------------------------------------------------*/
		D3D12_SUBRESOURCE_DATA subResourceData = {};
		subResourceData.pData      = data;
		subResourceData.RowPitch   = GetTotalByteSize();
		subResourceData.SlicePitch = subResourceData.RowPitch;

		/*-------------------------------------------------------------------
		-      Schedule to copy the data to the default buffer resource
		---------------------------------------------------------------------*/
		const auto dxCommandList = std::static_pointer_cast<directX12::RHICommandList>(copyCommandList)->GetCommandList();
		const auto beforeState = EnumConverter::Convert(_metaData.State);
		const auto before      = BARRIER::Transition(_resource.Get(), beforeState, D3D12_RESOURCE_STATE_COPY_DEST);
		
		dxCommandList->ResourceBarrier(1, &before);
		UpdateSubresources<1>(dxCommandList.Get(), _resource.Get(), _intermediateBuffer.Get(), 0, 0, 1, &subResourceData);
		
		auto after = BARRIER::Transition(_resource.Get(),  D3D12_RESOURCE_STATE_COPY_DEST, beforeState);
		dxCommandList->ResourceBarrier(1, &after);
	}
	else if (_metaData.HeapType == core::MemoryHeap::Upload || _metaData.HeapType == core::MemoryHeap::Readback)
	{
		Update(data, GetElementCount());
	}
	else
	{
		throw std::runtime_error("Unknown memory heap type");
	}
}
#pragma endregion Public Function