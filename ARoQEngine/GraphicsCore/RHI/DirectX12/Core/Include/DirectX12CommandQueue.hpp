//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12RHICommandQueue.hpp
///  @brief  コマンドリストによって貯められた描画コマンドをまとめてGPU側に送信します.(Execute関数) @n
///          また, コマンドキュー間のGPU側の同期も行うことが可能です (Wait, Signal) @n
///  @author Toide Yutaro
///  @date   2024_04_06
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
	/* @class     RHICommandQueue
	*  @brief     コマンドリストによって貯められた描画コマンドをまとめてGPU側に送信します.(Execute関数) @n
	*             また, コマンドキュー間のGPU側の同期も行うことが可能です (Wait, Signal) @n
	*                                         |@n
	*              set signal value (t=1)     | wait (t=1)-> execute gpu commands @n
	*			  --------------------------------------> t
	*****************************************************************************/
	class RHICommandQueue : public rhi::core::RHICommandQueue
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     他のコマンドキューとの実行順序を保証するため, ほかのコマンドキューの実行完了を待つ@n
		*             Fenceが持つWaitはCPU側も処理が止まってしまいますが, @n
		*             CommandQueueのWaitは指定したValue以上の値になるまでGPU内でのみ処理を止めます.
		*  @param[in] const gu::SharedPointer<core::RHIFence> : 外部からGPUの同期を行うためのFence
		*  @param[in] const gu::uint64 : GPUの待機処理を終了し, CommandQueueの実行を再開する時の値
		*************************************************************************/
		void Wait  (const gu::SharedPointer<core::RHIFence>& fence, const gu::uint64 value) override;
		
		/*!**********************************************************************
		*  @brief     コマンドキューの実行が完了したら, フェンスの値(value)を更新する
		*  @param[in] const gu::SharedPointer<core::RHIFence>& 外部からGPUの同期を行うためのFence
		*  @param[in] const gu::uint64 コマンドキューの実行完了時に更新される値
		*************************************************************************/
		void Signal(const gu::SharedPointer<core::RHIFence>& fence, const gu::uint64 value) override;
		
		/*!**********************************************************************
		*  @brief     コマンドリストに貯めた内容を実行する. 通常はset graphics, compute, transfer commandlist
		*  @param[in] GPUのコマンドを貯めたコマンドリスト配列
		*************************************************************************/
		void Execute(const gu::DynamicArray<gu::SharedPointer<rhi::core::RHICommandList>>& commandLists) override;

		#pragma endregion

		#pragma region Public Member Variables
		/*!**********************************************************************
		*  @brief     DirectX12で使用するコマンドキュー
		*************************************************************************/
		CommandQueueComPtr GetCommandQueue() { return _commandQueue; }

		/*!**********************************************************************
		*  @brief     デバッグ名
		*************************************************************************/
		void SetName(const gu::tstring& name) override;

		/*!**********************************************************************
		*  @brief   コマンドキュー中のGPUタイムスタンプをHz単位で返します.
		*************************************************************************/
		gu::uint64 GetTimestampFrequency() override;

		/*!**********************************************************************
		*  @brief  GPUとCPUの計測時刻をMicroSeconds単位で取得します@n
		* 　　　　　　測定したい区間でそれぞれTimestampの結果を保存し, その差分を引けば計測時間を測定可能です
		*************************************************************************/
		core::GPUTimingCalibrationTimestamp GetCalibrationTimestamp() override;

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		RHICommandQueue() = default;
		
		/*! @brief デストラクタ*/
		~RHICommandQueue();
		
		/*! @brief デバイスとコマンドキューの種類を指定して作成するコンストラクタ*/
		explicit RHICommandQueue(const gu::SharedPointer<rhi::core::RHIDevice>& device, const core::CommandListType type, const gu::tstring& name);

		#pragma endregion 

	protected:
		#pragma region Protected Function
		
		#pragma endregion 
		
		#pragma region Protected Member Variables
		/*! @brief DirectX12で使用されるコマンドキューの生ポインタ*/
		CommandQueueComPtr _commandQueue = nullptr;

		/*! @brief */
		#pragma endregion 	
	};
}
#endif