//////////////////////////////////////////////////////////////////////////////////
///             @file   RHICommandQueue.hpp
///             @brief  Send drawing commands to the GPU, 
///                     provide methods for synchronous processing of drawing command execution
///             @author Toide Yutaro
///             @date   2022_09_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_COMMAND_QUEUE_HPP
#define RHI_COMMAND_QUEUE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GameUtility/Base/Include/GUMemory.hpp"
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	class RHICommandList;
	class RHIFence;
	/****************************************************************************
	*				  			RHICommandQueue
	*************************************************************************//**
	*  @class     RHICommandQueue
	*  @brief     Send drawing commands to the GPU, 
	              provide methods for synchronous processing of drawing command execution
                                             |
                  set signal value (t=1)     | wait (t=1)-> execute gpu commands
				  --------------------------------------> t
	*****************************************************************************/
	class RHICommandQueue : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Used to wait for another Command queue to complete execution. (in GPU)*/
		virtual void Wait   (const gu::SharedPointer<RHIFence>& fence, const std::uint64_t value) = 0;
		
		/* @brief : Update the fence value (value) when the Command Queue execution completes.*/
		virtual void Signal (const gu::SharedPointer<RHIFence>& fence, const std::uint64_t value) = 0;
		
		/* @brief : Execute command list contents. normally set graphics, compute, transfer commandlist
		            All CommandLists to be assigned must be Closed.*/ 
		virtual void Execute(const std::vector<gu::SharedPointer<RHICommandList>>& commandLists) = 0;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		core::CommandListType GetType() const { return _commandListType; }
		
		virtual void SetName(const std::wstring& name) = 0;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		RHICommandQueue() = default;
		
		virtual ~RHICommandQueue() = default;
		
		explicit RHICommandQueue(const gu::SharedPointer<RHIDevice>& device, const CommandListType type) :  _device(device), _commandListType(type) {};
		
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		CommandListType  _commandListType = CommandListType::Unknown;

		gu::SharedPointer<RHIDevice> _device = nullptr;
	};
}
#endif