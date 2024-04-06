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
		/*----------------------------------------------------------------------
		*  @brief : Used to wait for another Command queue to complete execution. (in GPU)
		/*----------------------------------------------------------------------*/
		virtual void Wait   (const gu::SharedPointer<RHIFence>& fence, const gu::uint64 value) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :  Update the fence value (value) when the Command Queue execution completes.
		/*----------------------------------------------------------------------*/
		virtual void Signal (const gu::SharedPointer<RHIFence>& fence, const gu::uint64 value) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :  Execute command list contents. normally set graphics, compute, transfer commandlist
		             All CommandLists to be assigned must be Closed.
		/*----------------------------------------------------------------------*/
		virtual void Execute(const gu::DynamicArray<gu::SharedPointer<RHICommandList>>& commandLists) = 0;

	#pragma endregion 
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		core::CommandListType GetType() const { return _commandListType; }
		
		virtual void SetName(const gu::tstring& name) = 0;

		/*----------------------------------------------------------------------
		*  @brief :  Return the gpu timestamp frequency [Hz] of the command queue
		/*----------------------------------------------------------------------*/
		virtual gu::uint64 GetTimestampFrequency() = 0;

		/*----------------------------------------------------------------------
		*  @brief :  Return the gpu timestamp frequency [Hz] of the command queue
		/*----------------------------------------------------------------------*/
		virtual GPUTimingCalibrationTimestamp GetCalibrationTimestamp() = 0;

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