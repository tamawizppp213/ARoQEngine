//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanCommandQueue.hpp
///             @brief  Send drawing commands to the GPU,
///                     provide methods for synchronous processing of drawing command execution
///                     There are three types : Graphics, Compute, Copy command queue.
///             @author Toide Yutaro
///             @date   2022_09_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_COMMAND_QUEUE_HPP
#define VULKAN_COMMAND_QUEUE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandQueue.hpp"
#include <vulkan/vulkan.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Fence
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	/****************************************************************************
	*				  			RHICommandQueue
	*************************************************************************//**
	*  @class     RHICommandQueue
	*  @brief     Send drawing commands to the GPU,
				  provide methods for synchronous processing of drawing command execution
				  There are three types : Graphics, Compute, Copy command queue.
	*****************************************************************************/
	class RHICommandQueue : public rhi::core::RHICommandQueue
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @ brief : Used to wait for another Command queue to complete execution. (in GPU)*/
		void Wait(const gu::SharedPointer<core::RHIFence>& fence, const std::uint64_t value) override;
		
		/* @ brief : Update the fence value (value) when the Command Queue execution completes.*/
		void Signal(const gu::SharedPointer<core::RHIFence>& fence, const std::uint64_t value) override;
		
		/* @brief : Execute command list contents. normally set graphics, compute, transfer commandlist */
		void Execute(const std::vector<gu::SharedPointer<rhi::core::RHICommandList>>& commandLists) override;
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @brief : Return VkQueue.*/
		VkQueue GetQueue() const noexcept { return _queue; }
		
		/* @brief : Return the index which specifies the queue type.*/
		gu::uint32 GetQueueFamilyIndex() const noexcept { return _queueFamilyIndex; }
		
		/* @brief : Return the queue array index in the queue family.*/
		gu::uint32 GetQueueIndex() const noexcept { return _queueIndex; }
		
		void SetName(const gu::tstring& name) override;

		/*----------------------------------------------------------------------
		*  @brief :  コマンドキュー中のGPUタイムスタンプをHz単位で返します.
		/*----------------------------------------------------------------------*/
		gu::uint64 GetTimestampFrequency() override { return 0; }

		/*----------------------------------------------------------------------
		*  @brief :  CPUとGPUの計測時間をMicroseconds単位で取得します
		/*----------------------------------------------------------------------*/
		core::GPUTimingCalibrationTimestamp RHICommandQueue::GetCalibrationTimestamp()
		{
			return core::GPUTimingCalibrationTimestamp();
		}
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHICommandQueue() = default;
		
		explicit RHICommandQueue(const gu::SharedPointer<rhi::core::RHIDevice>& device, const core::CommandListType type, const std::uint32_t queueFamilyIndex, const std::uint32_t queueIndex = 0, const gu::tstring& name = SP(""));
		
		~RHICommandQueue();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkQueue       _queue            = nullptr;
		std::uint32_t _queueFamilyIndex = 0; // CommandQueueの種類ごとに振られるGroupID
		std::uint32_t _queueIndex       = 0; // QueueFamily中, CommandQueueの配列Index
	};
}
#endif