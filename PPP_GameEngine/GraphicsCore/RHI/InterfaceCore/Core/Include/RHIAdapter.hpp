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
#include "RHICommonState.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "GameUtility/Base/Include/GUString.hpp"

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
	class RHIDisplayAdapter : public NonCopyable
	{
	public:
		
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*---------------------------------------------------------------
				@brief :  return logical device shared pointer.
						  frame count is used for the command allocators
		-----------------------------------------------------------------*/
		virtual gu::SharedPointer<core::RHIDevice> CreateDevice() = 0;
		
		/*---------------------------------------------------------------
				@brief : Describe physical device name and spec(future work) 
		-----------------------------------------------------------------*/
		virtual void PrintInfo() = 0; // Todo : ç°å„ÇÕê´î\Ç»Ç«Ç‡ì¸ÇÍÇƒÇ®Ç´ÇΩÇ¢. 2022/09/07

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/*---------------------------------------------------------------
				@brief : Discrete GPU (true : dGPU, xGPU, false: iGPU)
		-----------------------------------------------------------------*/
		bool IsDiscreteGPU() const { return _isDiscreteGPU; }

		/*---------------------------------------------------------------
				@brief : Unified memory architecture (true: iGPU, false: dGPU, xGPU)
		-----------------------------------------------------------------*/
		bool IsUnifiedGPU() const { return !_isDiscreteGPU; }

		/*---------------------------------------------------------------
				@brief : Return Physical Device Name (GPU name)
		-----------------------------------------------------------------*/
		const gu::string& GetName() const { return _name; };

		/*---------------------------------------------------------------
				@brief :  Return vender ID. return 0 if no assignment
		-----------------------------------------------------------------*/
		const gu::uint32 GetVenderID() const { return _venderID; }
		
		/*---------------------------------------------------------------
				@brief :  Return Device ID. Return 0 if no assignment
		-----------------------------------------------------------------*/
		const gu::uint32 GetDeviceID() const { return _deviceID; } 
		
		/*---------------------------------------------------------------
				@brief :   Return RHIInstance Raw Pointer
		-----------------------------------------------------------------*/
		RHIInstance* GetInstance() const { return _instance.Get(); };
		
		/* @brief : Device vender check*/
		bool IsAdapterNVIDIA() const { Check(_venderID != 0); return _venderID == 0x10DE; }
		bool IsAdapterIntel()  const { Check(_venderID != 0); return _venderID == 0x8086;}
		bool IsAdapterAMD()    const { Check(_venderID != 0); return _venderID == 0x1002; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDisplayAdapter() = default;

		virtual ~RHIDisplayAdapter() { if (_instance) { _instance.Reset(); } }

		RHIDisplayAdapter(const gu::SharedPointer<RHIInstance>& instance) : _instance(instance)
		{
			Check(_instance);
		}; 

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		// @brief : Vender ID is able to specify company.  
		gu::uint32 _venderID = 0;

		// @brief : Device ID is the product id
		gu::uint32 _deviceID = 0;

		// @brief : true : dGPU, xGPU, false: iGPU
		bool _isDiscreteGPU = false;

		// @brief : Instance handler
		gu::SharedPointer<RHIInstance> _instance = nullptr;
		
		// @brief : adapter name
		gu::string _name = "";
	};
}

#endif