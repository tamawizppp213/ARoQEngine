//////////////////////////////////////////////////////////////////////////////////
///  @file   RHICommandQueue.hpp
///  @brief  コマンドリストによって貯められた描画コマンドをまとめてGPU側に送信します.(Execute関数) @n
///          また, コマンドキュー間のGPU側の同期も行うことが可能です (Wait, Signal) @n
///  @author Toide Yutaro
///  @date   2024_04_06
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_COMMAND_QUEUE_HPP
#define RHI_COMMAND_QUEUE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
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
	/* @class     RHICommandQueue
	*  @brief     コマンドリストによって貯められた描画コマンドをまとめてGPU側に送信します.(Execute関数) @n
	*             また, コマンドキュー間のGPU側の同期も行うことが可能です (Wait, Signal) @n
    *                                         |
    *              set signal value (t=1)     | wait (t=1)-> execute gpu commands
	*			  --------------------------------------> t
	*****************************************************************************/
	class RHICommandQueue : public gu::NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     他のコマンドキューとの実行順序を保証するため, ほかのコマンドキューの実行完了を待つ@n
		*             Fenceが持つWaitはCPU側も処理が止まってしまいますが, @n
		*             CommandQueueのWaitは指定したValue以上の値になるまでGPU内でのみ処理を止めます.
		*  @param[in] 外部からGPUの同期を行うためのFence
		*  @param[in] GPUの待機処理を終了し, CommandQueueの実行を再開する時の値
		*************************************************************************/
		virtual void Wait   (const gu::SharedPointer<RHIFence>& fence, const gu::uint64 value) = 0;
		
		/*!**********************************************************************
		*  @brief     コマンドキューの実行が完了したら, フェンスの値(value)を更新する
		*  @param[in] 外部からGPUの同期を行うためのFence
		*  @param[in] コマンドキューの実行完了時に更新される値
		*************************************************************************/
		virtual void Signal (const gu::SharedPointer<RHIFence>& fence, const gu::uint64 value) = 0;
		
		/*!**********************************************************************
		*  @brief     コマンドリストに貯めた内容を実行する. 通常はset graphics, compute, transfer commandlist
		*  @param[in] GPUのコマンドを貯めたコマンドリスト
		*************************************************************************/
		virtual void Execute(const gu::DynamicArray<gu::SharedPointer<RHICommandList>>& commandLists) = 0;

	    #pragma endregion 
		
		#pragma region Public Property
		/*!**********************************************************************
		*  @brief    コマンドリストの種類
		*************************************************************************/
		core::CommandListType GetType() const { return _commandListType; }
		
		/*!**********************************************************************
		*  @brief     デバッグ名
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) = 0;

		/*!**********************************************************************
		*  @brief   コマンドキュー中のGPUタイムスタンプをHz単位で返します.
		*************************************************************************/
		virtual gu::uint64 GetTimestampFrequency() = 0;

		/*!**********************************************************************
		*  @brief   GPUとCPUの計測時間をMicroSeconds単位で取得します
		*************************************************************************/
		virtual GPUTimingCalibrationTimestamp GetCalibrationTimestamp() = 0;
		#pragma endregion
		
		#pragma region Public Constructor and Destructor
		#pragma endregion 
	protected:
		#pragma region Protected Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		RHICommandQueue() = default;
		
		/*! @brief デストラクタ*/
		virtual ~RHICommandQueue() = default;
		
		/*! @brief デバイスとコマンドキューの種類を指定して作成するコンストラクタ*/
		explicit RHICommandQueue(const gu::SharedPointer<RHIDevice>& device, const CommandListType type) :  _device(device), _commandListType(type) {};
		#pragma endregion 
		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		/*! @brief 論理デバイス*/
		gu::SharedPointer<RHIDevice> _device = nullptr;

		/*! @brief コマンドリストの種類*/
		CommandListType  _commandListType = CommandListType::Unknown;
	};
}
#endif