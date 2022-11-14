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
		virtual std::shared_ptr<core::RHIDevice> CreateDevice(const std::uint32_t frameCount) = 0;
		/* Describe physical device name and spec(future work) */
		virtual void PrintInfo() = 0; // Todo : ç°å„ÇÕê´î\Ç»Ç«Ç‡ì¸ÇÍÇƒÇ®Ç´ÇΩÇ¢. 2022/09/07
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		bool              IsDiscreteGPU() const { return _isDiscreteGPU; }
		const std::string&  GetName    () const { return _name; };
		const std::uint32_t GetVenderID() const { return _venderID; } // Return 0 if no assignment
		const std::uint32_t GetDeviceID() const { return _deviceID; } // Return 0 if no assignment
		RHIInstance* GetInstance() const { return _instance.get(); };
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDisplayAdapter() = default;
		virtual ~RHIDisplayAdapter() { if (_instance) { _instance.reset(); } }
		RHIDisplayAdapter(const std::shared_ptr<RHIInstance>& instance) : _instance(instance){}; 
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