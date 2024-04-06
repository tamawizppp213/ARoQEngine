//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12RHICommandQueue.hpp
///  @brief  �R�}���h���X�g�ɂ���Ē��߂�ꂽ�`��R�}���h���܂Ƃ߂�GPU���ɑ��M���܂�.(Execute�֐�) @n
///          �܂�, �R�}���h�L���[�Ԃ�GPU���̓������s�����Ƃ��\�ł� (Wait, Signal) @n
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
	*  @brief     �R�}���h���X�g�ɂ���Ē��߂�ꂽ�`��R�}���h���܂Ƃ߂�GPU���ɑ��M���܂�.(Execute�֐�) @n
	*             �܂�, �R�}���h�L���[�Ԃ�GPU���̓������s�����Ƃ��\�ł� (Wait, Signal) @n
	*                                         |
	*              set signal value (t=1)     | wait (t=1)-> execute gpu commands
	*			  --------------------------------------> t
	*****************************************************************************/
	class RHICommandQueue : public rhi::core::RHICommandQueue
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief  ���̃R�}���h�L���[�Ƃ̎��s������ۏ؂��邽��, �ق��̃R�}���h�L���[�̎��s������҂�
		*************************************************************************/
		void Wait  (const gu::SharedPointer<core::RHIFence>& fence, const gu::uint64 value) override;
		
		/*----------------------------------------------------------------------
		*  @brief :  �R�}���h�L���[�̎��s������������A�t�F���X�̒l�ivalue�j���X�V����
		/*----------------------------------------------------------------------*/
		void Signal(const gu::SharedPointer<core::RHIFence>& fence, const gu::uint64 value) override;
		
		/*----------------------------------------------------------------------
		*  @brief :  �R�}���h���X�g�̓��e�����s����. 
		             �ʏ��set graphics, compute, transfer commandlist�B
		/*----------------------------------------------------------------------*/
		void Execute(const gu::DynamicArray<gu::SharedPointer<rhi::core::RHICommandList>>& commandLists) override;

		#pragma endregion

		#pragma region Public Member Variables
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		CommandQueueComPtr GetCommandQueue() { return _commandQueue; }

		void SetName(const gu::tstring& name) override;

		/*----------------------------------------------------------------------
		*  @brief :  �R�}���h�L���[����GPU�^�C���X�^���v��Hz�P�ʂŕԂ��܂�.
		/*----------------------------------------------------------------------*/
		gu::uint64 GetTimestampFrequency() override;

		/*----------------------------------------------------------------------
		*  @brief : GPU��CPU�̌v�����Ԃ�MicroSeconds�P�ʂŎ擾���܂�
		/*----------------------------------------------------------------------*/
		core::GPUTimingCalibrationTimestamp GetCalibrationTimestamp() override;

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		RHICommandQueue() = default;
		
		/*! @brief �f�X�g���N�^*/
		~RHICommandQueue();
		
		/*! @brief �f�o�C�X�ƃR�}���h�L���[�̎�ނ��w�肵�č쐬����R���X�g���N�^*/
		explicit RHICommandQueue(const gu::SharedPointer<rhi::core::RHIDevice>& device, const core::CommandListType type, const gu::tstring& name);

		#pragma endregion 

	protected:
		#pragma region Protected Function
		
		#pragma endregion 
		
		#pragma region Protected Member Variables
		/*! @brief DirectX12�Ŏg�p�����R�}���h�L���[�̐��|�C���^*/
		CommandQueueComPtr _commandQueue = nullptr;
		#pragma endregion 	
	};
}
#endif