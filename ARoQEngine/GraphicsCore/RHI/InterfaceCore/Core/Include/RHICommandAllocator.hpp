//////////////////////////////////////////////////////////////////////////////////
/// @file   RHICommandAllocator.hpp
/// @brief  このクラスは、コマンドリストのメモリを格納するために使用されます. @n 
///         コマンドリストのリセット関数が呼ばれると、メモリブロックを書き込み状態に移行することになります. @n
///         Cleanupを行った時点で, コマンドアロケータの中身を全て破棄します. その際, GPU命令は既に実行完了していないとエラーが出ます. 
/// @author Toide Yutaro
/// @date   2024_04_06
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_COMMAND_ALLOCATOR_HPP
#define RHI_COMMAND_ALLOCATOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
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
	****************************************************************************/
	/* @brief     コマンドリストのメモリを格納するために使用されます @n
	*             コマンドリストでアロケータにGPU命令を格納し, コマンドキューで命令を実行. 完了したらCleanupで中身を消去します.
	*****************************************************************************/
	class RHICommandAllocator : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief  コマンドアロケーターをリセットします (コマンドバッファの削除) @n
		*          あらゆるバインドされたコマンドリストはこの関数を呼ばれる前に閉じておく必要があります.@n
		*          加えて, GPU上のコマンドが実行されるまで, この関数は呼んではなりません
		*************************************************************************/
		virtual void CleanUp() = 0;

		#pragma endregion

		#pragma region Public Property
		
		/*!**********************************************************************
		*  @brief   コマンドリストの種類を返します (Graphics, compute, copy)
		*  @return  core::CommandList : コマンドリストの種類
		*************************************************************************/
		core::CommandListType GetCommandListType() const { return _commandListType; }

		/*!**********************************************************************
		*  @brief   デバッグ用の表示名を設定します
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) = 0;

		#pragma endregion
		
		#pragma region Public Constructor and Destructor

		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		/*! @brief デフォルトコンストラクタ*/
		RHICommandAllocator() = default;

		/*! @brief デストラクタ*/
		virtual ~RHICommandAllocator() = default;

		/*! @brief デバイスとコマンドリストの種類で生成するコンストラクタ*/
		explicit RHICommandAllocator(const gu::SharedPointer<RHIDevice>& device, const core::CommandListType commandListType)
			: _device(device), _commandListType(commandListType)
		{
			Checkf(_device, "device is nullptr.\n");
		};

		#pragma endregion 

		#pragma region Protected Property

		/*! @brief 論理デバイス*/
		gu::SharedPointer<RHIDevice> _device = nullptr;

		/*! @brief コマンドリストの種類*/
		CommandListType   _commandListType = core::CommandListType::Unknown;

		#pragma endregion
	};
}
#endif