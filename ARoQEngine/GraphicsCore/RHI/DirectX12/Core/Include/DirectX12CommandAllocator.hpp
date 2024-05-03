//////////////////////////////////////////////////////////////////////////////////
/// @file   DirectX12CommandAllocator.hpp
/// @brief  このクラスは、コマンドリストのメモリを格納するために使用されます. @n 
///         コマンドリストのリセット関数が呼ばれると、メモリブロックを書き込み状態に移行することになります. @n
///         Cleanupを行った時点で, コマンドアロケータの中身を全て破棄します. その際, GPU命令は既に実行完了していないとエラーが出ます. 
/// @author Toide Yutaro
/// @date   2024_04_06
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_COMMAND_ALLOCATOR_HPP
#define DIRECTX12_COMMAND_ALLOCATOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandAllocator.hpp"
#include "DirectX12Core.hpp"

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
	/* @class     RHICommandAllocator
	*  @brief     コマンドリストのメモリを格納するために使用されます @n
	*             コマンドリストでアロケータにGPU命令を格納し, コマンドキューで命令を実行. 完了したらCleanupで中身を消去します.
	*****************************************************************************/
	class RHICommandAllocator : public rhi::core::RHICommandAllocator
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief  コマンドアロケーターをリセットします (コマンドバッファの削除) @n
		*          あらゆるバインドされたコマンドリストはこの関数を呼ばれる前に閉じておく必要があります.@n
		*          加えて, GPU上のコマンドが実行されるまで, この関数は呼んではなりません
		*************************************************************************/
		virtual void CleanUp() override;

		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief   DirectX12で使用するコマンドアロケータです
		*************************************************************************/
		CommandAllocatorComPtr GetAllocator() { return _commandAllocator; }

		/*!**********************************************************************
		*  @brief   デバッグ用の表示名を設定します
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) override;
		#pragma endregion
		
		#pragma region Public Constructor and Destructor

		/*! @brief デフォルトコンストラクタ*/
		RHICommandAllocator() = default;

		/*! @brief デストラクタ*/
		~RHICommandAllocator();

		/*! @brief デバイスとコマンドリストの種類で生成するコンストラクタ*/
		explicit RHICommandAllocator(
			const gu::SharedPointer<rhi::core::RHIDevice>& device,
			const core::CommandListType type,
			const gu::tstring& name);
		#pragma endregion 

	protected:
		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
		/*! @brief  DirectX12で使用するコマンドアロケータです*/
		CommandAllocatorComPtr _commandAllocator = nullptr;

		#pragma endregion
	};
}
#endif