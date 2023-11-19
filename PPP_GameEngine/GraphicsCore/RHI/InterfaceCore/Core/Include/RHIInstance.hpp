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
#include "GameUtility/Base/Include/GUMemory.hpp"
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
	class RHIInstance : public NonCopyable
	{
	public:
		/* The CPU debugger and GPU debugger do not cause anything special when in Release mode.*/
	    /* The GPU debugger impacts on the frame rate. default : off*/
		static gu::SharedPointer<RHIInstance> CreateInstance(const core::APIVersion version, bool enableCPUDebugger = true, bool enableGPUDebugger = false, bool useGPUDebugBreak = false);
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* directX12 : (High) xGPU, dGPU iGPU (Low), vulkan : dGPU (not : first select gpu) */
		virtual gu::SharedPointer <RHIDisplayAdapter> SearchHighPerformanceAdapter() = 0;
		
		/* directX12 : (Low) iGPU, dGPU xGPU (High), vulkan : iGPU (not : first select gpu) */
		virtual gu::SharedPointer<RHIDisplayAdapter>  SearchMinimumPowerAdapter() = 0;
		
		/* return all available display adapter*/
		virtual std::vector<gu::SharedPointer<RHIDisplayAdapter>> EnumrateAdapters() = 0;
		
		/* OutputDebugString : adapter list*/
		virtual void LogAdapters() = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const char* GetEngineName() const { return EngineName; }

		bool UseGPUDebugBreak() const { return _useGPUDebugBreak; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		RHIInstance() = default;

		RHIInstance(bool enableCPUDebugger, bool enableGPUDebugger, bool useGPUDebugBreak) :
			_enableCPUDebugger(enableCPUDebugger),
			_enableGPUDebugger(enableGPUDebugger),
			_useGPUDebugBreak(useGPUDebugBreak)
		{
		};

		virtual ~RHIInstance() = default;

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		bool _enableCPUDebugger = false; // debug only
		bool _enableGPUDebugger = false; // debug only
		bool _useGPUDebugBreak  = false; // debug only
		const char* EngineName = "PPP_Engine";
	};
	
}

#endif