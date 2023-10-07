//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIInstance.hpp
///             @brief  Select device api (このエンジンを使用時最初に呼び出す.) 
///             @author Toide Yutaro
///             @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_RHI_INSTANCE_HPP
#define DIRECTX12_RHI_INSTANCE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIInstance.hpp"
#include "DirectX12Core.hpp"
#include <dxgi1_6.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{
	/****************************************************************************
	*				  			RHIInstance
	*************************************************************************//**
	*  @class     RHIInstance
	*  @brief     Select device api and select display adapter
	*****************************************************************************/
	class RHIInstance : public core::RHIInstance
	{
	public:

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* directX12 : (High) xGPU, dGPU iGPU (Low) selected*/
		std::shared_ptr<core::RHIDisplayAdapter> SearchHighPerformanceAdapter() override;
		
		/* directX12 : (Low) iGPU, dGPU xGPU (High)*/
		std::shared_ptr<core::RHIDisplayAdapter> SearchMinimumPowerAdapter ()override;
		
		/* return all available display adapter*/
		std::vector<std::shared_ptr<core::RHIDisplayAdapter>> EnumrateAdapters() override;
		
		/* OutputDebugString : adapter list*/
		void LogAdapters() override;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		FactoryComPtr GetFactory() const noexcept { return _factory; }

		/*IDREDSettings*  GetDREDSettings () const noexcept { return _useDRED ? _dredSettings.Get() : nullptr; }
		IDREDSettings1* GetDREDSettings1() const noexcept { return _useDREDContext ? _dredSettings1.Get() : nullptr; }*/
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIInstance() = default;

		RHIInstance(bool enableCPUDebugger, bool enableGPUDebugger, bool useGPUBreakPoint); 
		
		~RHIInstance();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		/* @brief : CPU debugger*/
		void EnabledDebugLayer();            // debug mode only use

		/* @brief : GPU debugger*/
		void EnabledShaderBasedValidation(); // It has a significant impact on the frame rate.
		
		/* @brief : Acquire the debug information when the Device is removed.*/
		void EnabledGPUClashDebuggingModes(); 

		/* @brief : Select High performance or minimum power ( (High) xGPU, dGPU iGPU (Low))*/
		std::shared_ptr<core::RHIDisplayAdapter> SearchAdapter(const DXGI_GPU_PREFERENCE preference);
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		FactoryComPtr _factory = nullptr;

		bool _useDRED        = false;
		bool _useDREDContext = false;
		bool _useLightWeightDRED = false;
	};
}

#endif