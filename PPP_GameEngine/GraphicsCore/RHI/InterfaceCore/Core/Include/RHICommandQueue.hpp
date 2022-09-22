//////////////////////////////////////////////////////////////////////////////////
///             @file   RHICommandQueue.hpp
///             @brief  CommandQueue
///             @author Toide Yutaro
///             @date   2022_06_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_COMMAND_QUEUE_HPP
#define RHI_COMMAND_QUEUE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <memory>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	class RHICommandList;
	class RHIFence;
	/****************************************************************************
	*				  			RHIFence
	*************************************************************************//**
	*  @class     RHIFence
	*  @brief     CPU-GPU synchronization
	*****************************************************************************/
	class RHICommandQueue : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual void Wait   (const std::shared_ptr<RHIFence>& fence, const std::uint64_t value) = 0;
		virtual void Signal (const std::shared_ptr<RHIFence>& fence, const std::uint64_t value) = 0;
		virtual void Execute(const std::vector<std::shared_ptr<RHICommandList>>& commandLists) = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		RHICommandQueue() = default;
		virtual ~RHICommandQueue() = default;
		explicit RHICommandQueue(const CommandListType type) :  _commandListType(type) {};
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		CommandListType  _commandListType = CommandListType::Unknown;
	};
}
#endif