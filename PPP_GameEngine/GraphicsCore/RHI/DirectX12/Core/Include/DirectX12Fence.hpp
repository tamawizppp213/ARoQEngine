//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12Fence.hpp
///             @brief  CPU-GPU synchronization
///             @author Toide Yutaro
///             @date   2022_06_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_FENCE_HPP
#define DIRECTX12_FENCE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFence.hpp"
#include "DirectX12Core.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{
	/****************************************************************************
	*				  			RHIFence
	*************************************************************************//**
	*  @class     RHIFence
	*  @brief     CPU-GPU synchronization
	*****************************************************************************/
	class RHIFence : public rhi::core::RHIFence
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief: Set fence value from CPU side. 
		(in case RHICommandQueue::Signal -> Set fence value from GPU side)*/
		void Signal(const std::uint64_t value) override ;
		
		/* @brief:  Detect the completion of GPU processing on the CPU side. If not, it waits.*/
		void Wait  (const std::uint64_t value) override;
		
		/* @brief:  Return current fence value*/
		std::uint64_t GetCompletedValue() override;
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		FenceComPtr GetFence() const noexcept { return _fence; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIFence() = default;

		~RHIFence();

		explicit RHIFence(const std::shared_ptr<rhi::core::RHIDevice>& device, const std::uint64_t initialValue = 0);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		FenceComPtr _fence = nullptr;
	};
}
#endif