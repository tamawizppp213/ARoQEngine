//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12Query.cpp
///             @brief  temp
///             @author toide
///             @date   2024/01/04 2:55:34
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DirectX12Query.hpp"
#include "../Include/DirectX12Device.hpp"
#include "../Include/DirectX12Debug.hpp"
#include "../Include/DirectX12EnumConverter.hpp"
#include "../../Resource/Include/DirectX12GPUBuffer.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::directX12;
using namespace rhi;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 
RHIQuery::RHIQuery(const SharedPointer<core::RHIDevice>& device, 
	const core::QueryHeapType heapType) : core::RHIQuery(device, heapType), _queryAllocator(QueryAllocator(MAX_HEAP_BYTE_SIZE / GetResultStrideByteSize()))
{
	const auto rhiDevice = static_cast<rhi::directX12::RHIDevice*>(_device.Get());
	const auto dxDevice = rhiDevice->GetDevice();

	const auto count    = MAX_HEAP_BYTE_SIZE / GetResultStrideByteSize();

	/*-------------------------------------------------------------------
	-                  Query heapの作成
	---------------------------------------------------------------------*/
	D3D12_QUERY_HEAP_DESC queryHeapDesc =
	{
		.Type     = EnumConverter::Convert(heapType),
		.Count    = count,
		.NodeMask = device->GetGPUMask().Value()
	};

	ThrowIfFailed(dxDevice->CreateQueryHeap(&queryHeapDesc, IID_PPV_ARGS(_queryHeap.GetAddressOf())));
	_queryHeap->SetName(PrepareQueryHeapName());

	/*-------------------------------------------------------------------
	-                  Query heapの種類設定
	---------------------------------------------------------------------*/
	_dxQueryHeapType = EnumConverter::Convert (_queryHeapType);
	_dxQueryType     = EnumConverter::Convert1(_queryHeapType);

	/*-------------------------------------------------------------------
	-                  Readback bufferの作成
	---------------------------------------------------------------------*/
	auto metaData = rhi::core::GPUBufferMetaData::UploadBuffer
		            ( GetResultStrideByteSize(), count, rhi::core::MemoryHeap::Readback, nullptr );
	metaData.State = core::ResourceState::CopyDestination;

	_resultBuffer = rhiDevice->CreateBuffer(metaData, L"ReadbackBuffer");
	_resultBuffer->SetName(PrepareResultBufferName());

	/*-------------------------------------------------------------------
	-                  Readback bufferのmap
	---------------------------------------------------------------------*/
	// read back heap自体は永続的にマップされることを許容しているため, 今回は一度だけマップを行う対応とする
	_resultBuffer->Map();
}

RHIQuery::~RHIQuery()
{
	if (_resultBuffer)
	{
		_resultBuffer->Unmap();
		_resultBuffer.Reset();
	}
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*                     AllocatorID
*************************************************************************//**
*  @fn        RHIQuery::DescriptorID RHIQuery::AllocateID()
*
*  @brief     Query用の確保されていない領域のIndexを返します.
*             Heapが満杯になったら再確保せずに最初の状態に書き込みます. 
*             (書き込めなかったらまた修正します)
*
*  @param[in] void
*
*  @return    gu::uint32 descriptor id
*****************************************************************************/
core::QueryResultLocation RHIQuery::Allocate()
{
	return core::QueryResultLocation
	(
		SharedFromThis(),
		_queryAllocator.IssueID(),
		_queryHeapType
	);
}

/****************************************************************************
*                     FreeID
*************************************************************************//**
*  @fn        void RHIQuery::FreeID(const RHIQuery::DescriptorID offsetIndex)
*
*  @brief     Query用の指定されたIndexを解放します.
*
*  @param[in] const DescriptorID offsetIndex
*
*  @return    void
*****************************************************************************/
void RHIQuery::Free(core::QueryResultLocation& location)
{
	_queryAllocator.FreeID(location.QueryID);
	location.Heap   = nullptr;
}

/****************************************************************************
*                     GetResultStrideByteSize
*************************************************************************//**
*  @fn        gu::uint32 RHIQuery::GetResultStrideByteSize() const
*
*  @brief     結果を取得するバッファのstride sizeを返します
*
*  @param[in] void
*
*  @return    gu::uint32
*****************************************************************************/
gu::uint32 RHIQuery::GetResultStrideByteSize() const
{
	switch (_dxQueryType)
	{
		case D3D12_QUERY_TYPE_OCCLUSION:
		case D3D12_QUERY_TYPE_TIMESTAMP:
		{
			return sizeof(uint64);
		}
		case D3D12_QUERY_TYPE_PIPELINE_STATISTICS:
		{
			return sizeof(D3D12_QUERY_DATA_PIPELINE_STATISTICS);
		}
		default:
		{
			throw std::runtime_error("not supported byte size in the RHIQuery");
		}
	}
}
#pragma endregion Main Function

#pragma region Setup Function
/****************************************************************************
*                     PrepareQueryHeapName
*************************************************************************//**
*  @fn        const wchar* RHIQuery::PrepareQueryHeapName()
*
*  @brief     クエリ結果を保持するquery heapの名前を取得します
*
*  @param[in] void
*
*  @return    const wchar*
*****************************************************************************/
const wchar* RHIQuery::PrepareQueryHeapName()
{
	switch (_dxQueryHeapType)
	{
		case D3D12_QUERY_HEAP_TYPE_OCCLUSION           : { return L"Occulusion Query Heap"; }
		case D3D12_QUERY_HEAP_TYPE_TIMESTAMP           : { return L"Timestamp Query Heap"; }
		case D3D12_QUERY_HEAP_TYPE_COPY_QUEUE_TIMESTAMP: { return L"Timestamp Query Heap (Copy)"; }
		case D3D12_QUERY_HEAP_TYPE_PIPELINE_STATISTICS : { return L"Pipeline Statistics Query Heap (Copy)"; }
		default:
		{
			throw std::runtime_error("Not supported query heap name");
		}
	}
}

/****************************************************************************
*                     PrepareResultBufferName
*************************************************************************//**
*  @fn        const wchar* RHIQuery::PrepareResultBufferName()
*
*  @brief     クエリ結果を取得するReadBackBufferの名前を取得します
*
*  @param[in] void
*
*  @return    const wchar*
*****************************************************************************/
const wchar* RHIQuery::PrepareResultBufferName()
{
	switch (_dxQueryHeapType)
	{
		case D3D12_QUERY_HEAP_TYPE_OCCLUSION           : { return L"Occulusion Query Heap Result Buffer"; }
		case D3D12_QUERY_HEAP_TYPE_TIMESTAMP           : { return L"Timestamp Query Heap Result Buffer"; }
		case D3D12_QUERY_HEAP_TYPE_COPY_QUEUE_TIMESTAMP: { return L"Timestamp Query Heap Result Buffer(Copy)"; }
		case D3D12_QUERY_HEAP_TYPE_PIPELINE_STATISTICS : { return L"Pipeline Statistics Query Heap Result Buffer(Copy)"; }
		default:
		{
			throw std::runtime_error("Not supported query heap name");
		}
	}
}
#pragma endregion Setup Function