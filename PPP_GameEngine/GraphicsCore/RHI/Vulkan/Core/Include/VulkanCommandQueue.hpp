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
		void Wait(const std::shared_ptr<core::RHIFence>& fence, const std::uint64_t value) override;
		/* @ brief : Update the fence value (value) when the Command Queue execution completes.*/
		void Signal(const std::shared_ptr<core::RHIFence>& fence, const std::uint64_t value) override;
		/* @brief : Execute command list contents. normally set graphics, compute, transfer commandlist */
		void Execute(const std::vector<std::shared_ptr<rhi::core::RHICommandList>>& commandLists) override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkQueue GetQueue() const noexcept { return _queue; }
		std::uint32_t GetQueueFamilyIndex() const noexcept { return _queueFamilyIndex; }
		std::uint32_t GetQueueIndex() const noexcept { return _queueIndex; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHICommandQueue() = default;
		explicit RHICommandQueue(const std::shared_ptr<rhi::core::RHIDevice>& device, const core::CommandListType type, const std::uint32_t queueFamilyIndex, const std::uint32_t queueIndex = 0);
		~RHICommandQueue();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkQueue       _queue = nullptr;
		std::uint32_t _queueFamilyIndex = 0;
		std::uint32_t _queueIndex = 0;
	};
}
#endif