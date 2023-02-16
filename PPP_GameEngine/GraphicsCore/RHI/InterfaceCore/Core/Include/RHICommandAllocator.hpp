//////////////////////////////////////////////////////////////////////////////////
///             @file   RHICommandAllocator.hpp
///             @brief  CommandAllocator
///                     This class is used to store the command list memory.
///                     Every time a command list is reset, it will allocate a block of memory.
///                     Even if a command list is reset, these memories aren't lost. 
///                     When the Reset function in this class is called, these memories cleans up. 
///                     To acieve the maximum frame rate, you should create each command list one by one.
///             @author Toide Yutaro
///             @date   2022_06_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_COMMAND_ALLOCATOR_HPP
#define RHI_COMMAND_ALLOCATOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <memory>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Allocator class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHICommandList;
	class RHIDevice;
	/****************************************************************************
	*				  			RHICommandAllocator
	*************************************************************************//**
	*  @class     RHICommandAllocator
	*  @brief     Allocation of memory used by command list
	*****************************************************************************/
	class RHICommandAllocator : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Reset command allocator (clear command list buffer)*/
		virtual void CleanUp() = 0;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		core::CommandListType GetCommandListType() const { return _commandListType; }

		/* @brief : For Debugging */
		virtual void SetName(const std::wstring& name) = 0;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		RHICommandAllocator () = default;

		virtual ~RHICommandAllocator() = default;

		explicit RHICommandAllocator(const std::shared_ptr<RHIDevice>& device, const core::CommandListType commandListType) 
			: _device(device),  _commandListType(commandListType) { };
		
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice> _device = nullptr;

		CommandListType   _commandListType = core::CommandListType::Unknown;
	};
}
#endif