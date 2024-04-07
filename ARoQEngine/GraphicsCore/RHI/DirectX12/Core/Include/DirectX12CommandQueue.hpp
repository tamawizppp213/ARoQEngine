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
	*                                         |@n
	*              set signal value (t=1)     | wait (t=1)-> execute gpu commands @n
	*			  --------------------------------------> t
	*****************************************************************************/
	class RHICommandQueue : public rhi::core::RHICommandQueue
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     ���̃R�}���h�L���[�Ƃ̎��s������ۏ؂��邽��, �ق��̃R�}���h�L���[�̎��s������҂�@n
		*             Fence������Wait��CPU�����������~�܂��Ă��܂��܂���, @n
		*             CommandQueue��Wait�͎w�肵��Value�ȏ�̒l�ɂȂ�܂�GPU���ł̂ݏ������~�߂܂�.
		*  @param[in] const gu::SharedPointer<core::RHIFence> : �O������GPU�̓������s�����߂�Fence
		*  @param[in] const gu::uint64 : GPU�̑ҋ@�������I����, CommandQueue�̎��s���ĊJ���鎞�̒l
		*************************************************************************/
		void Wait  (const gu::SharedPointer<core::RHIFence>& fence, const gu::uint64 value) override;
		
		/*!**********************************************************************
		*  @brief     �R�}���h�L���[�̎��s������������, �t�F���X�̒l(value)���X�V����
		*  @param[in] const gu::SharedPointer<core::RHIFence>& �O������GPU�̓������s�����߂�Fence
		*  @param[in] const gu::uint64 �R�}���h�L���[�̎��s�������ɍX�V�����l
		*************************************************************************/
		void Signal(const gu::SharedPointer<core::RHIFence>& fence, const gu::uint64 value) override;
		
		/*!**********************************************************************
		*  @brief     �R�}���h���X�g�ɒ��߂����e�����s����. �ʏ��set graphics, compute, transfer commandlist
		*  @param[in] GPU�̃R�}���h�𒙂߂��R�}���h���X�g�z��
		*************************************************************************/
		void Execute(const gu::DynamicArray<gu::SharedPointer<rhi::core::RHICommandList>>& commandLists) override;

		#pragma endregion

		#pragma region Public Member Variables
		/*!**********************************************************************
		*  @brief     DirectX12�Ŏg�p����R�}���h�L���[
		*************************************************************************/
		CommandQueueComPtr GetCommandQueue() { return _commandQueue; }

		/*!**********************************************************************
		*  @brief     �f�o�b�O��
		*************************************************************************/
		void SetName(const gu::tstring& name) override;

		/*!**********************************************************************
		*  @brief   �R�}���h�L���[����GPU�^�C���X�^���v��Hz�P�ʂŕԂ��܂�.
		*************************************************************************/
		gu::uint64 GetTimestampFrequency() override;

		/*!**********************************************************************
		*  @brief  GPU��CPU�̌v��������MicroSeconds�P�ʂŎ擾���܂�@n
		* �@�@�@�@�@�@���肵������Ԃł��ꂼ��Timestamp�̌��ʂ�ۑ���, ���̍����������Όv�����Ԃ𑪒�\�ł�
		*************************************************************************/
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

		/*! @brief */
		#pragma endregion 	
	};
}
#endif