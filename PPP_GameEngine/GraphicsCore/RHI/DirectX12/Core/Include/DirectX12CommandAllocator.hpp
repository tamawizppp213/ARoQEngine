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
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
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
		/*---------------------------------------------------------------
		　　　　　@brief : コマンドアロケーターをリセットします (コマンドバッファの削除)
			   @note 　: あらゆるバインドされたコマンドリストはこの関数を呼ばれる前に閉じておく必要があります
						加えて, GPU上のコマンドが実行されるまで, この関数は呼んではなりません
		-----------------------------------------------------------------*/
		void CleanUp() override;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/*---------------------------------------------------------------
		　　　　　@brief : コマンドリストの種類を返します (Graphics, compute, copy)
		-----------------------------------------------------------------*/
		CommandAllocatorComPtr GetAllocator() { return _commandAllocator; }

		/*---------------------------------------------------------------
		　　　　　@brief : デバッグ用の表示名を設定します
		-----------------------------------------------------------------*/
		void SetName(const gu::tstring& name) override;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHICommandAllocator() = default;

		~RHICommandAllocator();

		explicit RHICommandAllocator(
			const gu::SharedPointer<rhi::core::RHIDevice>& device, 
			const core::CommandListType type,
			const gu::tstring& name);

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