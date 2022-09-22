//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanCommandQueue.hpp
///             @brief  Command Queue
///             @author Toide Yutaro
///             @date   2022_06_23
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
	*  @brief     CommandQueue (for executing GPU command)
	*****************************************************************************/
	class RHICommandQueue : public rhi::core::RHICommandQueue
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @ brief : Wait until the specified Fence value is reached.*/
		void Wait(const std::shared_ptr<core::RHIFence>& fence, const std::uint64_t value) override;
		/* @ brief : Update the fence value (value) when the Command Queue execution completes.*/
		void Signal(const std::shared_ptr<core::RHIFence>& fence, const std::uint64_t value) override;
		// Execute command lists
		void Execute(const std::vector<std::shared_ptr<rhi::core::RHICommandList>>& commandLists) override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkQueue GetQueue() const noexcept { return _queue; }
		std::uint32_t GetQueueFamilyIndex() const noexcept { return _queueFamilyIndex; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHICommandQueue() = default;
		explicit RHICommandQueue(const std::shared_ptr<rhi::core::RHIDevice>& device, const core::CommandListType type, const std::uint32_t queueFamilyIndex);
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
	};
}
#endif