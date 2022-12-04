//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIInstance.hpp
///             @brief  Select device api (このエンジンを使用時最初に呼び出す.) 
///             @author Toide Yutaro
///             @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_INSTANCE_HPP
#define RHI_INSTANCE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "RHICommonState.hpp"
#include <memory>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDisplayAdapter;
	/****************************************************************************
	*				  			RHIInstance
	*************************************************************************//**
	*  @class     RHIInstance
	*  @brief     Select device api 
	*****************************************************************************/
	class RHIInstance : public NonCopyable, public std::enable_shared_from_this<RHIInstance>
	{
	public:
		/* The CPU debugger and GPU debugger do not cause anything special when in Release mode.*/
	    /* The GPU debugger impacts on the frame rate. default : off*/
		static std::shared_ptr<RHIInstance> CreateInstance(const core::APIVersion version, bool enableCPUDebugger = true, bool enableGPUDebugger = false);
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* directX12 : (High) xGPU, dGPU iGPU (Low), vulkan : dGPU (not : first select gpu) */
		virtual std::shared_ptr <RHIDisplayAdapter> SearchHighPerformanceAdapter() = 0;
		/* directX12 : (Low) iGPU, dGPU xGPU (High), vulkan : iGPU (not : first select gpu) */
		virtual std::shared_ptr<RHIDisplayAdapter>  SearchMinimumPowerAdapter() = 0;
		/* return all available display adapter*/
		virtual std::vector<std::shared_ptr<RHIDisplayAdapter>> EnumrateAdapters() = 0;
		/* OutputDebugString : adapter list*/
		virtual void LogAdapters() = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		RHIInstance() = default;
		RHIInstance(bool enableCPUDebugger, bool enableGPUDebugger) :
			_enableCPUDebugger(enableCPUDebugger),
			_enableGPUDebugger(enableGPUDebugger)
		{
		};
		virtual ~RHIInstance() = default;

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		bool _enableCPUDebugger = false; // debug only
		bool _enableGPUDebugger = false; // debug only
		const char* EngineName = "PPP_Engine";
	};
	
}

#endif