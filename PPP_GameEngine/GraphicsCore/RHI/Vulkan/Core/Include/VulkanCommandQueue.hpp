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
	*				  			TemplateClass
	*************************************************************************//**
	*  @class     TemplateClass
	*  @brief     temp
	*****************************************************************************/
	class RHICommandQueue : public rhi::core::RHICommandQueue
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Execute(const std::vector<std::shared_ptr<rhi::core::RHICommandList>>& commandLists) override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkQueue GetQueue() const noexcept { return _queue; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHICommandQueue() = default;
		explicit RHICommandQueue(const std::shared_ptr<rhi::core::RHIDevice>& device);
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