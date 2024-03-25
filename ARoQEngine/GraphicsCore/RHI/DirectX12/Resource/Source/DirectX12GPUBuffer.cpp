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
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "GameUtility/Memory/Include/GUMemory.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUBuffer::GPUBuffer(const gu::SharedPointer<core::RHIDevice>& device, const core::GPUBufferMetaData& metaData, const gu::tstring& name)
	:core::GPUBuffer(device, metaData, name)
{
	using enum core::ResourceUsage;

	const auto rhiDevice = static_cast<directX12::RHIDevice*>(device.Get());

	/*-------------------------------------------------------------------
	-           Set heap property
	---------------------------------------------------------------------*/
	const D3D12_HEAP_PROPERTIES heapProp
	{
		.Type                 = EnumConverter::Convert(_metaData.HeapType), 
		.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN,
		.CreationNodeMask     = rhiDevice->GetGPUMask().Value(),
		.VisibleNodeMask      = rhiDevice->GetGPUMask().Value()
	};

	/*-------------------------------------------------------------------
	-           Set resource desc
	---------------------------------------------------------------------*/
	const auto usage     = metaData.ResourceUsage;
	const auto isDynamic = gu::HasAnyFlags(usage, core::ResourceUsage::AnyDynamic);
	Check(isDynamic ? _metaData.HeapType != core::MemoryHeap::Default : true);

	const D3D12_RESOURCE_DESC resourceDesc = 
	{
		.Dimension          = D3D12_RESOURCE_DIMENSION_BUFFER,
		.Alignment          = metaData.Stride > 0 && 
		                      (gu::HasAnyFlags(usage, StructuredBuffer) ||
							  !gu::HasAnyFlags(usage, ByteAddress | DrawIndirect)) ? (UINT64)0: (UINT64)4,
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
	const auto dxDevice = static_cast<rhi::directX12::RHIDevice*>(_device.Get());
	ThrowIfFailed( dxDevice->CreateCommittedResource(_resource, resourceDesc, heapProp, EnumConverter::Convert(metaData.State)));

	_resource->SetName(name.CString());
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
	Check(_metaData.IsCPUAccessible());

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
	Check(elementIndex <= _metaData.Count);
	gu::Memory::Copy(&_mappedData[elementIndex * _metaData.Stride], data, _metaData.Stride);
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
	Check(dataLength + indexOffset <= _metaData.Count);
	gu::Memory::Copy(&_mappedData[indexOffset * _metaData.Stride], data, _metaData.Stride * (size_t)dataLength);
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

void GPUBuffer::SetName(const gu::tstring& name)
{
	ThrowIfFailed(_resource->SetName(name.CString()));
}

void GPUBuffer::Pack(const void* data, const gu::SharedPointer<core::RHICommandList>& copyCommandList)
{
	const auto rhiDevice = static_cast<rhi::directX12::RHIDevice*>(_device.Get());
	const auto dxDevice  = rhiDevice->GetDevice();
	if ((!_metaData.IsCPUAccessible()))
	{
		Check(copyCommandList->GetType() == core::CommandListType::Copy);

		/*-------------------------------------------------------------------
		-          Copy CPU memory data into our default buffer,
		-          we need to create an intermediate upload heap
		---------------------------------------------------------------------*/
		const D3D12_HEAP_PROPERTIES uploadHeapProp
		{
			.Type                 = D3D12_HEAP_TYPE_UPLOAD,
			.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
			.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN,
			.CreationNodeMask     = rhiDevice->GetGPUMask().Value(),
			.VisibleNodeMask      = rhiDevice->GetGPUMask().Value()
		};

		const auto uploadTempBuffer = RESOURCE_DESC::Buffer(GetTotalByteSize());

		// CreateCommittedResource -> Heap確保 + 実際にGPUにメモリを書き込むを両方やってくれる.
		ThrowIfFailed(rhiDevice->CreateCommittedResource
		(
			_intermediateBuffer, uploadTempBuffer, uploadHeapProp, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr)
		);

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
		const auto dxCommandList = gu::StaticPointerCast<directX12::RHICommandList>(copyCommandList)->GetCommandList();
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