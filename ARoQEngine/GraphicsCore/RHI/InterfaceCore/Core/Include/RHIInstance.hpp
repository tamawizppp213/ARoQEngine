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
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "RHICommonState.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
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
	class RHIInstance : public gu::NonCopyable
	{
	public:
		/*----------------------------------------------------------------------
		*  @brief : CPU DebuggerとGPU DebuggerはReleaseモードでは何も起こしません. 
		* 
		*           GPU Debuggerはフレームレートに影響を及ぼすことから, デフォルトはoffとなっています. 
		* 
		*----------------------------------------------------------------------*/
		static gu::SharedPointer<RHIInstance> CreateInstance(const core::APIVersion version, const bool enableCPUDebugger = true, bool enableGPUDebugger = false, const bool useGPUDebugBreak = false);
		
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* directX12 : (High) xGPU, dGPU iGPU (Low), vulkan : dGPU (not : first select gpu) */
		virtual gu::SharedPointer <RHIDisplayAdapter> SearchHighPerformanceAdapter() = 0;
		
		/* directX12 : (Low) iGPU, dGPU xGPU (High), vulkan : iGPU (not : first select gpu) */
		virtual gu::SharedPointer<RHIDisplayAdapter>  SearchMinimumPowerAdapter() = 0;
		
		/* return all available display adapter*/
		virtual gu::DynamicArray<gu::SharedPointer<RHIDisplayAdapter>> EnumrateAdapters() = 0;
		
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