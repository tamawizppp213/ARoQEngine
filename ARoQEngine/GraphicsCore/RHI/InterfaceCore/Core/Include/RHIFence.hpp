//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIFence.hpp
///             @brief  CPU-GPU synchronization
///             @author Toide Yutaro
///             @date   2022_06_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_FENCE_HPP
#define RHI_FENCE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                            Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	class RHICommandQueue;
	/****************************************************************************
	*				  			RHIFence
	*************************************************************************//**
	*  @class     RHIFence
	*  @brief     CPU-GPU synchronization
	*****************************************************************************/
	class RHIFence : public gu::NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief: Set fence value from CPU side. (in case RHICommandQueue::Signal -> Set fence value from GPU side)*/
		virtual void Signal(const std::uint64_t value)  = 0;
		
		/* @brief:  Detect the completion of GPU processing on the CPU side. If not, it waits.*/
		virtual void Wait  (const std::uint64_t value)  = 0;
		
		/* @brief:  Return current fence value*/
		virtual std::uint64_t GetCompletedValue() = 0;
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		virtual void SetName(const gu::tstring& name) = 0;

	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIFence() = default;

		explicit RHIFence(const gu::SharedPointer<RHIDevice>& device) { _device = device; }
		
		virtual ~RHIFence() { if (_device) { _device.Reset(); } }
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<RHIDevice> _device = nullptr;
	};
}
#endif