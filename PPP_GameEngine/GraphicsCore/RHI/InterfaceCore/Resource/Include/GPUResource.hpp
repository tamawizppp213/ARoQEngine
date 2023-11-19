//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUResource.hpp
///             @brief  GPU Resource 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_RESOURCE_HPP
#define GPU_RESOURCE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUMemory.hpp"
#include <string>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::core
{
	class RHIDevice;
	/****************************************************************************
	*				  			GPUResource
	*************************************************************************//**
	*  @class     GPUResource 
	*  @brief     Resource (å„Ç≈NoncopyableÇ…ïœçXÇ∑ÇÈ)
	*****************************************************************************/
	class GPUResource 
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual void TransitionResourceState(const core::ResourceState after) = 0;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		virtual void SetName(const std::wstring& name) = 0;

		virtual core::ResourceState GetResourceState() const noexcept = 0;
		
		bool IsTexture() const { return _isTexture; }

		bool IsBuffer() const { return !_isTexture; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUResource() = default;

		virtual ~GPUResource()
		{
			if (_device) { _device.Reset(); }
		}

		explicit GPUResource(const gu::SharedPointer<RHIDevice>& device) : _device(device) {};
		
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<RHIDevice> _device = nullptr;

		bool _isTexture = true;
	}; 
}


#endif