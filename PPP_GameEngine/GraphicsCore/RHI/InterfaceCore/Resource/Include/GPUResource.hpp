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
#include <memory>
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
		std::wstring GetName() const noexcept { return _name; };

		virtual void SetName(const std::wstring& name) = 0;

		virtual core::ResourceState GetResourceState() const noexcept = 0;
		
		bool IsTexture() const { return _isTexture; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUResource() = default;

		~GPUResource() = default;

		explicit GPUResource(const std::shared_ptr<RHIDevice>& device, const std::wstring& name = L"") : _device(device), _name(name) {};
		
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice> _device = nullptr;

		std::wstring _name = L"";

		bool _isTexture = true;
	}; 
}


#endif