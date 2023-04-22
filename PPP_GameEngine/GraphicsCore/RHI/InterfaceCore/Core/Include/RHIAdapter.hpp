//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIAdapter.hpp
///             @brief  Physical Device (adapter), Describe gpu information 
///             @author Toide Yutaro
///             @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_ADAPTER_HPP
#define RHI_ADAPTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "RHICommonState.hpp"
#include <memory>
#include <string>
#include <cassert>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	class RHIInstance;
	/****************************************************************************
	*				  			RHIDisplayAdapter
	*************************************************************************//**
	*  @class     RHIDisplayAdapter
	*  @brief     Physical Device (adapter), Describe gpu information
	*****************************************************************************/
	class RHIDisplayAdapter : public NonCopyable, public std::enable_shared_from_this<RHIDisplayAdapter>
	{
	public:
		
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* return logical device shared pointer. frame count is used for the command allocators*/
		virtual std::shared_ptr<core::RHIDevice> CreateDevice() = 0;
		
		/* Describe physical device name and spec(future work) */
		virtual void PrintInfo() = 0; // Todo : ç°å„ÇÕê´î\Ç»Ç«Ç‡ì¸ÇÍÇƒÇ®Ç´ÇΩÇ¢. 2022/09/07

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @brief : Discrete GPU (true : dGPU, xGPU, false: iGPU)*/
		bool IsDiscreteGPU() const { return _isDiscreteGPU; }

		/* @brief : Unified memory architecture (true: iGPU, false: dGPU, xGPU)*/
		bool IsUnifiedGPU() const { return !_isDiscreteGPU; }

		/* @brief : Return Physical Device Name (GPU name)*/
		const std::string&  GetName    () const { return _name; };

		/* @brief : Return vender ID*/
		const std::uint32_t GetVenderID() const { return _venderID; } // Return 0 if no assignment
		
		/* @brief : Return Device ID*/
		const std::uint32_t GetDeviceID() const { return _deviceID; } // Return 0 if no assignment
		
		// @brief : Return RHIInstance Raw Pointer
		RHIInstance* GetInstance() const { return _instance.get(); };
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDisplayAdapter() = default;

		virtual ~RHIDisplayAdapter() { if (_instance) { _instance.reset(); } }

		RHIDisplayAdapter(const std::shared_ptr<RHIInstance>& instance) : _instance(instance)
		{
			assert(_instance);
		}; 

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::string   _name     = "";
		std::uint32_t _venderID = 0;
		std::uint32_t _deviceID = 0;
		bool _isDiscreteGPU = false;
		std::shared_ptr<RHIInstance> _instance = nullptr;
	};
}

#endif