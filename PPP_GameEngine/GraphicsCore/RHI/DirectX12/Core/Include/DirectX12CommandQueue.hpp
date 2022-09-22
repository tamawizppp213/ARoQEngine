//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12CommandQueue.hpp
///             @brief  CommandQueue. This class is used to execute commandLists.
///             @author Toide Yutaro
///             @date   2022_06_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_COMMAND_QUEUE_HPP
#define DIRECTX12_COMMAND_QUEUE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandQueue.hpp"
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
	class RHICommandQueue : public rhi::core::RHICommandQueue
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @ brief : Wait until the specified Fence value is reached.*/
		void Wait  (const std::shared_ptr<core::RHIFence>& fence, const std::uint64_t value) override;
		/* @ brief : Update the fence value (value) when the Command Queue execution completes.*/
		void Signal(const std::shared_ptr<core::RHIFence>& fence, const std::uint64_t value) override;
		// @ brief : Execute command lists.
		void Execute(const std::vector<std::shared_ptr<rhi::core::RHICommandList>>& commandLists) override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		CommandQueueComPtr GetCommandQueue() { return _commandQueue; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHICommandQueue() = default;
		~RHICommandQueue();
		explicit RHICommandQueue(const std::shared_ptr<rhi::core::RHIDevice>& device, const core::CommandListType type);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		CommandQueueComPtr _commandQueue = nullptr;
	};
}
#endif