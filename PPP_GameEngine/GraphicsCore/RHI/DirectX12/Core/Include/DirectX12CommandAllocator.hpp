//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12CommandAllocator.hpp
///             @brief  DirectX12CommandAllocator
///                     This class is used to store the command list memory.
///                     Every time a command list is reset, it will allocate a block of memory.
///                     Even if a command list is reset, these memories aren't lost. 
///                     When the Reset function in this class is called, these memories cleans up. 
///                     To acieve the maximum frame rate, you should create each command list one by one.
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_COMMAND_ALLOCATOR_HPP
#define DIRECTX12_COMMAND_ALLOCATOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandAllocator.hpp"
#include "DirectX12Core.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                          CommandAllocatorClass
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{
	/****************************************************************************
	*				  			RHICommandAllocator
	*************************************************************************//**
	*  @class     RHICommandAllocator
	*  @brief     Command allocator
	*****************************************************************************/
	class RHICommandAllocator : public rhi::core::RHICommandAllocator
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Clean up the command pool. 
		   @note : All binded command lists must be closed before calling this function.
		           In addition, until command execution in GPU, this function mustn't be called.*/
		void CleanUp() override;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		CommandAllocatorComPtr GetAllocator() { return _commandAllocator; }

		void SetName(const std::wstring& name) override;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHICommandAllocator() = default;

		~RHICommandAllocator();

		explicit RHICommandAllocator(
			const std::shared_ptr<rhi::core::RHIDevice>& device, 
			const core::CommandListType type,
			const std::wstring& name);

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		CommandAllocatorComPtr _commandAllocator = nullptr;
	};
}
#endif