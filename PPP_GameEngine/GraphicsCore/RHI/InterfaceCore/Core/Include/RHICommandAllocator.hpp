//////////////////////////////////////////////////////////////////////////////////
///             @file   RHICommandAllocator.hpp
///             @brief  CommandAllocator
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
		/* @brief : Reset command allocator*/
		virtual void Reset() = 0;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		core::CommandListType GetCommandListType() const { return _commandListType; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		RHICommandAllocator () = default;

		~RHICommandAllocator() = default;

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