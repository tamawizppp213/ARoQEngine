//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12CommandQueue.hpp
///             @brief  Send drawing commands to the GPU,
///                     provide methods for synchronous processing of drawing command execution
///                     There are three types : Graphics, Compute, Copy command queue.
///             @author Toide Yutaro
///             @date   2022_09_23
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
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
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
		/* @ brief : Used to wait for another Command queue to complete execution. (in GPU)
		             他のコマンドキューとの実行順序を保証するために使用する.*/
		void Wait  (const std::shared_ptr<core::RHIFence>& fence, const std::uint64_t value) override;
		
		/* @ brief : Update the fence value (value) when the submitted Command Queue execution completes.*/
		void Signal(const std::shared_ptr<core::RHIFence>& fence, const std::uint64_t value) override;
		
		/* @brief : Execute command list contents. normally set graphics, compute, transfer commandlist */
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