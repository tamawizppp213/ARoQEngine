//////////////////////////////////////////////////////////////////////////////////
///             @file   TemplateText.hpp
///             @brief  TemplateText
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_RESOURCE_HPP
#define DIRECTX12_GPU_RESOURCE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "DirectX12Core.hpp"
#include <d3d12.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			GPUResource
*************************************************************************//**
*  @class     GPUResource
*  @brief     GPUResource
*****************************************************************************/
class GPUResource
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	virtual void Destroy()
	{
		_resource   = nullptr;
	}
	virtual void TransitionState(D3D12_RESOURCE_STATES after)
	{
		if (_usageState != after) { _usageState = after; }
	}
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	      Resource* GetResource()       { return _resource.Get(); }
	const Resource* GetResource() const { return _resource.Get(); }
	Resource**      GetAddressOf()      { return _resource.GetAddressOf(); }
	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() { return _resource->GetGPUVirtualAddress(); }
	D3D12_RESOURCE_STATES GetUsageState(){ return _usageState; }
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	GPUResource() :_usageState(D3D12_RESOURCE_STATE_COMMON) {};
	GPUResource(Resource* resource, D3D12_RESOURCE_STATES currentState) : _usageState(currentState),_resource(resource) {};
	~GPUResource() { Destroy(); }

	      Resource* operator->()       { return _resource.Get(); }
	const Resource* operator->() const { return _resource.Get(); }
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/

	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	ResourceComPtr            _resource = nullptr;
	D3D12_RESOURCE_STATES     _usageState;
};
#endif