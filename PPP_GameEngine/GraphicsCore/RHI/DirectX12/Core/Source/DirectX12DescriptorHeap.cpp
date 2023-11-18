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
#pragma region Constructor and Destructor
RHIDescriptorHeap::RHIDescriptorHeap(const gu::SharedPointer<core::RHIDevice>& device)
	: core::RHIDescriptorHeap(device)
{

}

RHIDescriptorHeap::~RHIDescriptorHeap()
{
	if (!_resourceAllocators.empty()) { _resourceAllocators.clear(); }
	if (_descriptorHeap)              { _descriptorHeap.Reset(); }
	_descriptorByteSize = 0;
}

#pragma endregion Constructor and Destructor
#pragma region Public Function 
/****************************************************************************
*                     Allocate
*************************************************************************//**
*  @fn        RHIDescriptorHeap::DescriptorID RHIDescriptorHeap::Allocate(const core::DescriptorHeapType heapType, const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout)
*
*  @brief     Allocate view.Return descriptor index(only use resourceLayout in vulkan api
*
*  @param[in] const core::DescriptorHeapType heapType
*  @param[in] const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout
*
*  @return 　　DescriptorID
*****************************************************************************/
RHIDescriptorHeap::DescriptorID RHIDescriptorHeap::Allocate(const core::DescriptorHeapType heapType, const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout)
{
	/*-------------------------------------------------------------------
	-			     Check heap type
	---------------------------------------------------------------------*/
	if (!_heapInfo.contains(heapType)) { throw std::runtime_error("Not include heap type"); }

	/*-------------------------------------------------------------------
	-			     Issue ID
	---------------------------------------------------------------------*/
	auto& resourceAllocator = _resourceAllocators[heapType];
	return resourceAllocator.IssueID(); // resource view index in each heap.
}

/****************************************************************************
*                     Free
*************************************************************************//**
*  @fn        void RHIDescriptorHeap::Free(const core::DescriptorHeapType heapType, const DescriptorID offsetIndex)
*
*  @brief     Free Resource allocator and heap
*
*  @param[in] const core::DescriptorHeapType heapType
*  @param[in] const DescriptorID offsetID
*
*  @return 　　void
*****************************************************************************/
void RHIDescriptorHeap::Free(const core::DescriptorHeapType heapType, const DescriptorID offsetIndex)
{
	/*-------------------------------------------------------------------
	-			     Check heap type
	---------------------------------------------------------------------*/
	if (!_heapInfo.contains(heapType)) { return; }

	/*-------------------------------------------------------------------
	-			     Free ID
	---------------------------------------------------------------------*/
	_resourceAllocators[heapType].FreeID(offsetIndex); // resource view index in each heap.s
}

/****************************************************************************
*                     Resize
*************************************************************************//**
*  @fn        void RHIDescriptorHeap::Resize(const core::DescriptorHeapType type, const size_t viewCount)
* 
*  @brief     Resize max view count size heap
* 
*  @param[in] const core::DescriptorHeapType type
* 
*  @param[in] const size_t viewCount
* 
*  @return 　　void
*****************************************************************************/
void RHIDescriptorHeap::Resize(const core::DescriptorHeapType type, const size_t viewCount)
{
	/*-------------------------------------------------------------------
	-			     Check max heap size
	---------------------------------------------------------------------*/
	if (_heapInfo.contains(type) && _heapInfo.at(type) > viewCount) { return; }

	std::map<core::DescriptorHeapType, MaxDescriptorSize> heapInfo;
	heapInfo[type] = viewCount;

	/*-------------------------------------------------------------------
	-			    Resize Heap
	---------------------------------------------------------------------*/
	Resize(heapInfo);
}
/****************************************************************************
*                     Resize
*************************************************************************//**
*  @fn        void RHIDescriptorHeap::Resize(const core::DescriptorHeapType type, const size_t viewCount)
* 
*  @brief     Resize max view count size heap
* 
*  @param[in] const std::map<core::DescriptorHeapType, MaxDescriptorSize>& heapInfo
* 
*  @return 　　void
*****************************************************************************/
void RHIDescriptorHeap::Resize(const std::map<core::DescriptorHeapType, MaxDescriptorSize>& heapInfos)
{
	const auto dxDevice = gu::StaticPointerCast<RHIDevice>(_device)->GetDevice();

	/*-------------------------------------------------------------------
	-	Count total descriptor size and Check Heap Type (CBV or SRV or UAV)
	---------------------------------------------------------------------*/
	if (!CheckCorrectViewConbination(heapInfos)) { throw std::runtime_error("wrong conbination view"); }

	size_t totalDescriptorCount = 0;

	for (const auto& desc : heapInfos) 
	{ 
		// count total descriptor size
		totalDescriptorCount += desc.second; 

		// check max heap size
		if (_heapInfo.find(desc.first) == _heapInfo.end())   { continue; }
		if (_heapInfo.at(desc.first) > desc.second)          { return; }
	}

	/*-------------------------------------------------------------------
	-			     Count total descriptor size
	---------------------------------------------------------------------*/
	const D3D12_DESCRIPTOR_HEAP_TYPE heapType = EnumConverter::Convert(heapInfos.begin()->first);
	
	const D3D12_DESCRIPTOR_HEAP_DESC heapDesc =
	{
		.Type           = heapType,
		.NumDescriptors = static_cast<UINT>(totalDescriptorCount),
		.Flags          = heapType == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV || heapType == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER  ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		.NodeMask       = 0
	};

	/*-------------------------------------------------------------------
	-			     Create heap desc
	---------------------------------------------------------------------*/
	DescriptorHeapComPtr heap = nullptr;
	ThrowIfFailed(dxDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(heap.GetAddressOf())));
	heap->SetName(L"DirectX12::DescriptorHeap");

	/*-------------------------------------------------------------------
	-			 Reallocate (in case existing descriptor heap)
	---------------------------------------------------------------------*/
	if (_totalHeapCount > 0 && _descriptorHeap != nullptr)
	{
		size_t pointer = 0;
		for (const auto& heapInfo : heapInfos)
		{
			// copy already created descriptor view
			dxDevice->CopyDescriptorsSimple(
				_resourceAllocators[heapInfo.first].GetMaxDescriptorCount(),                           // copy   descriptor count 
				D3D12_CPU_DESCRIPTOR_HANDLE(heap->GetCPUDescriptorHandleForHeapStart().ptr + pointer), // dest   descriptor range 
				_resourceAllocators[heapInfo.first].GetCPUDescHandler(),                               // source descriptor range
				heapType);     

			// Proceed cpu and gpu pointer 
			pointer += _descriptorByteSize * heapInfo.second;
		}
	}

	/*-------------------------------------------------------------------
	-			 Set heap configuration
	---------------------------------------------------------------------*/
	_totalHeapCount     = totalDescriptorCount;
	_descriptorByteSize = dxDevice->GetDescriptorHandleIncrementSize(heapType);
	_descriptorHeap     = heap;

	{
		size_t pointer = 0;
		for (const auto& heapInfo : heapInfos)
		{
			_resourceAllocators[heapInfo.first].SetResourceAllocator
			(
				static_cast<std::uint32_t>(heapInfo.second),            // max descriptor count
				static_cast<std::uint32_t>(_descriptorByteSize),        // one descriptor byte size
				D3D12_CPU_DESCRIPTOR_HANDLE(_descriptorHeap->GetCPUDescriptorHandleForHeapStart().ptr + pointer), // cpu start pointer
				heapInfo.first == core::DescriptorHeapType::RTV || heapInfo.first == core::DescriptorHeapType::DSV ?
				D3D12_GPU_DESCRIPTOR_HANDLE() : 
				D3D12_GPU_DESCRIPTOR_HANDLE(_descriptorHeap->GetGPUDescriptorHandleForHeapStart().ptr + pointer) // gpu start pointer
			);

			_heapInfo[heapInfo.first] = heapInfo.second;

			pointer += _descriptorByteSize * heapInfo.second;
		}
	}
}

/****************************************************************************
*                     Reset
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::ReportLiveObjects()
* 
*  @brief     Reset (ResetFlag: All -> Heap Count 0, or OnlyOffset)
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void RHIDescriptorHeap::Reset(const ResetFlag flag)
{
	for (auto& resourceAllocator : _resourceAllocators)
	{
		resourceAllocator.second.ResetID();
	}

	if (flag == ResetFlag::All && _descriptorHeap != nullptr) 
	{
		_descriptorHeap.Reset(); 
		_totalHeapCount     = 0;
		_descriptorByteSize = 0;

	}
}
#pragma endregion Public Function
#pragma region Private Function
bool RHIDescriptorHeap::CheckCorrectViewConbination(const std::map<core::DescriptorHeapType, MaxDescriptorSize>& heapInfos)
{
	bool foundSingleView = false;

	for (const auto& heapInfo : heapInfos)
	{
		if (heapInfo.first == core::DescriptorHeapType::RTV)     { foundSingleView = true; break; }
		if (heapInfo.first == core::DescriptorHeapType::DSV)     { foundSingleView = true; break; }
		if (heapInfo.first == core::DescriptorHeapType::SAMPLER) { foundSingleView = true; break; }
	}

	if (foundSingleView) { return heapInfos.size() == 1 ? true : false; }
	else                 { return true; }
}
#pragma endregion Private Function