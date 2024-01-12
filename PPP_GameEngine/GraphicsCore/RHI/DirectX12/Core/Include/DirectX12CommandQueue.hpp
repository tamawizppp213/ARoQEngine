//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12CommandQueue.hpp
///             @brief  Send drawing commands to the GPU,
///                     provide methods for synchronous processing of drawing command execution
///                     There are three types : Graphics, Compute, Copy command queue.
///             @author Toide Yutaro
///             @date   2022_09_23
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
	*  @class     RHICommandQueue
	*  @brief     Send drawing commands to the GPU,
				  provide methods for synchronous processing of drawing command execution
				  There are three types : Graphics, Compute, Copy command queue.
	*****************************************************************************/
	class RHICommandQueue : public rhi::core::RHICommandQueue
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief :  ���̃R�}���h�L���[�Ƃ̎��s������ۏ؂��邽��, �ق��̃R�}���h�L���[�̎��s������҂�
		/*----------------------------------------------------------------------*/
		void Wait  (const gu::SharedPointer<core::RHIFence>& fence, const std::uint64_t value) override;
		
		/*----------------------------------------------------------------------
		*  @brief :  �R�}���h�L���[�̎��s������������A�t�F���X�̒l�ivalue�j���X�V����
		/*----------------------------------------------------------------------*/
		void Signal(const gu::SharedPointer<core::RHIFence>& fence, const std::uint64_t value) override;
		
		/*----------------------------------------------------------------------
		*  @brief :  �R�}���h���X�g�̓��e�����s����. 
		             �ʏ��set graphics, compute, transfer commandlist�B
		/*----------------------------------------------------------------------*/
		void Execute(const std::vector<gu::SharedPointer<rhi::core::RHICommandList>>& commandLists) override;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		CommandQueueComPtr GetCommandQueue() { return _commandQueue; }

		void SetName(const std::wstring& name) override;

		/*----------------------------------------------------------------------
		*  @brief :  �R�}���h�L���[����GPU�^�C���X�^���v��Hz�P�ʂŕԂ��܂�.
		/*----------------------------------------------------------------------*/
		gu::uint64 GetTimestampFrequency() override;

		/*----------------------------------------------------------------------
		*  @brief : GPU��CPU�̌v�����Ԃ�MicroSeconds�P�ʂŎ擾���܂�
		/*----------------------------------------------------------------------*/
		core::GPUTimingCalibrationTimestamp GetCalibrationTimestamp() override;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHICommandQueue() = default;
		
		~RHICommandQueue();
		
		explicit RHICommandQueue(const gu::SharedPointer<rhi::core::RHIDevice>& device, const core::CommandListType type, const std::wstring& name);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		CommandQueueComPtr _commandQueue = nullptr;
	};
}
#endif