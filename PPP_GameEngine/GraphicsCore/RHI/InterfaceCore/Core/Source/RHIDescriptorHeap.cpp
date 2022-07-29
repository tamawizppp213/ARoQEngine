//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUDescriptorHeap.hpp
///             @brief  GPU DescriptorHeap 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIDescriptorHeap.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
RHIDescriptorHeap::RHIDescriptorHeap( const std::shared_ptr<RHIDevice>& device, const DescriptorHeapType heapType, const size_t maxHeapSize)
	: _device(device)
{
	_heapTypeLists = std::vector<DescriptorHeapType>({ heapType });
	_maxDescriptorCounts = std::vector<size_t>({ maxHeapSize });
}

RHIDescriptorHeap::RHIDescriptorHeap(const std::shared_ptr<RHIDevice>& device, const std::vector<DescriptorHeapType>& heapTypeList, const std::vector<size_t>& maxDescriptorCounts)
	: _device(device), _heapTypeLists(heapTypeList), _maxDescriptorCounts(maxDescriptorCounts)
{

}

RHIDescriptorHeap::~RHIDescriptorHeap()
{
	_heapTypeLists.clear(); _heapTypeLists.shrink_to_fit();
	_maxDescriptorCounts.clear(); _heapTypeLists.shrink_to_fit();
}