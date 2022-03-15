//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12ResourceAllocator.hpp
///             @brief  DirectX12Resource Allocator
///             @author Toide Yutaro
///             @date   2022_03_14
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_RESOURCE_ALLOCATOR_HPP
#define DIRECTX12_RESOURCE_ALLOCATOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "DirectX12BaseStruct.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  			ResourceAllocator
*************************************************************************//**
*  @class     ResourceAllocator
*  @brief     Resource allocate
*****************************************************************************/
class ResourceAllocator
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	inline CPU_DESC_HANDLER GetCPUDescHandler(UINT offsetIndex = 0) const
	{
		if (offsetIndex > _maxDescriptorCount)
		{
			MessageBox(NULL, L"The number of IDs has exceeded the expected number. ", L"Warning", MB_ICONWARNING);
		}

		return CPU_DESC_HANDLER(_cpuHeapPtr, offsetIndex, _descriptorSize);
	}
	inline GPU_DESC_HANDLER GetGPUDescHandler(UINT offsetIndex = 0) const
	{
		if (offsetIndex > _maxDescriptorCount)
		{
			MessageBox(NULL, L"The number of IDs has exceeded the expected number. ", L"Warning", MB_ICONWARNING);
		}
		return GPU_DESC_HANDLER(_gpuHeapPtr, offsetIndex, _descriptorSize);
	}

	inline UINT IssueID()
	{
		if (_currentID + 1 > _maxDescriptorCount) { MessageBox(NULL, L"The number of IDs has exceeded the expected number. ", L"Warning", MB_ICONWARNING); }
		
		_currentID++; return _currentID; 
	}
	inline void ResetID()
	{
		_currentID = INVALID_ID;
	}
	inline UINT GetHeapSize()  const { return _maxDescriptorCount * _descriptorSize; }
	inline void SetResourceAllocator(UINT maxDescriptorCount, UINT descriptorSize, D3D12_CPU_DESCRIPTOR_HANDLE cpuHeapPtr, D3D12_GPU_DESCRIPTOR_HANDLE gpuHeapPtr)
	{
		_maxDescriptorCount  = maxDescriptorCount;
		_descriptorSize      = descriptorSize;
		_cpuHeapPtr          = cpuHeapPtr;
		_gpuHeapPtr          = gpuHeapPtr;
		_currentID           = INVALID_ID;
	}
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	ResourceAllocator()                                    = default;
	ResourceAllocator(const ResourceAllocator&)            = delete;
	ResourceAllocator& operator=(const ResourceAllocator&) = delete;
	ResourceAllocator(ResourceAllocator&&)                 = delete;
	ResourceAllocator& operator=(ResourceAllocator&&)      = delete;
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/

	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	D3D12_CPU_DESCRIPTOR_HANDLE _cpuHeapPtr         = D3D12_CPU_DESCRIPTOR_HANDLE();
	D3D12_GPU_DESCRIPTOR_HANDLE _gpuHeapPtr         = D3D12_GPU_DESCRIPTOR_HANDLE();
	UINT                        _descriptorSize     = 0;
	UINT                        _maxDescriptorCount = 0;
	int                         _currentID = INVALID_ID;
	static constexpr int INVALID_ID = -1;
};

#endif