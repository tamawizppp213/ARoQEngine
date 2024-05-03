//////////////////////////////////////////////////////////////////////////////////
///  @file   RHICommandQueue.hpp
///  @brief  �R�}���h���X�g�ɂ���Ē��߂�ꂽ�`��R�}���h���܂Ƃ߂�GPU���ɑ��M���܂�.(Execute�֐�) @n
///          �܂�, �R�}���h�L���[�Ԃ�GPU���̓������s�����Ƃ��\�ł� (Wait, Signal) @n
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
	*  @brief     �R�}���h���X�g�ɂ���Ē��߂�ꂽ�`��R�}���h���܂Ƃ߂�GPU���ɑ��M���܂�.(Execute�֐�) @n
	*             �܂�, �R�}���h�L���[�Ԃ�GPU���̓������s�����Ƃ��\�ł� (Wait, Signal) @n
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
		*  @brief     ���̃R�}���h�L���[�Ƃ̎��s������ۏ؂��邽��, �ق��̃R�}���h�L���[�̎��s������҂�@n
		*             Fence������Wait��CPU�����������~�܂��Ă��܂��܂���, @n
		*             CommandQueue��Wait�͎w�肵��Value�ȏ�̒l�ɂȂ�܂�GPU���ł̂ݏ������~�߂܂�.
		*  @param[in] �O������GPU�̓������s�����߂�Fence
		*  @param[in] GPU�̑ҋ@�������I����, CommandQueue�̎��s���ĊJ���鎞�̒l
		*************************************************************************/
		virtual void Wait   (const gu::SharedPointer<RHIFence>& fence, const gu::uint64 value) = 0;
		
		/*!**********************************************************************
		*  @brief     �R�}���h�L���[�̎��s������������, �t�F���X�̒l(value)���X�V����
		*  @param[in] �O������GPU�̓������s�����߂�Fence
		*  @param[in] �R�}���h�L���[�̎��s�������ɍX�V�����l
		*************************************************************************/
		virtual void Signal (const gu::SharedPointer<RHIFence>& fence, const gu::uint64 value) = 0;
		
		/*!**********************************************************************
		*  @brief     �R�}���h���X�g�ɒ��߂����e�����s����. �ʏ��set graphics, compute, transfer commandlist
		*  @param[in] GPU�̃R�}���h�𒙂߂��R�}���h���X�g
		*************************************************************************/
		virtual void Execute(const gu::DynamicArray<gu::SharedPointer<RHICommandList>>& commandLists) = 0;

	    #pragma endregion 
		
		#pragma region Public Property
		/*!**********************************************************************
		*  @brief    �R�}���h���X�g�̎��
		*************************************************************************/
		core::CommandListType GetType() const { return _commandListType; }
		
		/*!**********************************************************************
		*  @brief     �f�o�b�O��
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) = 0;

		/*!**********************************************************************
		*  @brief   �R�}���h�L���[����GPU�^�C���X�^���v��Hz�P�ʂŕԂ��܂�.
		*************************************************************************/
		virtual gu::uint64 GetTimestampFrequency() = 0;

		/*!**********************************************************************
		*  @brief   GPU��CPU�̌v�����Ԃ�MicroSeconds�P�ʂŎ擾���܂�
		*************************************************************************/
		virtual GPUTimingCalibrationTimestamp GetCalibrationTimestamp() = 0;
		#pragma endregion
		
		#pragma region Public Constructor and Destructor
		#pragma endregion 
	protected:
		#pragma region Protected Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		RHICommandQueue() = default;
		
		/*! @brief �f�X�g���N�^*/
		virtual ~RHICommandQueue() = default;
		
		/*! @brief �f�o�C�X�ƃR�}���h�L���[�̎�ނ��w�肵�č쐬����R���X�g���N�^*/
		explicit RHICommandQueue(const gu::SharedPointer<RHIDevice>& device, const CommandListType type) :  _device(device), _commandListType(type) {};
		#pragma endregion 
		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		/*! @brief �_���f�o�C�X*/
		gu::SharedPointer<RHIDevice> _device = nullptr;

		/*! @brief �R�}���h���X�g�̎��*/
		CommandListType  _commandListType = CommandListType::Unknown;
	};
}
#endif