//////////////////////////////////////////////////////////////////////////////////
/// @file   RHICommandAllocator.hpp
/// @brief  ���̃N���X�́A�R�}���h���X�g�̃��������i�[���邽�߂Ɏg�p����܂�. @n 
///         �R�}���h���X�g�̃��Z�b�g�֐����Ă΂��ƁA�������u���b�N���������ݏ�ԂɈڍs���邱�ƂɂȂ�܂�. @n
///         Cleanup���s�������_��, �R�}���h�A���P�[�^�̒��g��S�Ĕj�����܂�. ���̍�, GPU���߂͊��Ɏ��s�������Ă��Ȃ��ƃG���[���o�܂�. 
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
	/* @brief     �R�}���h���X�g�̃��������i�[���邽�߂Ɏg�p����܂� @n
	*             �R�}���h���X�g�ŃA���P�[�^��GPU���߂��i�[��, �R�}���h�L���[�Ŗ��߂����s. ����������Cleanup�Œ��g���������܂�.
	*****************************************************************************/
	class RHICommandAllocator : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief  �R�}���h�A���P�[�^�[�����Z�b�g���܂� (�R�}���h�o�b�t�@�̍폜) @n
		*          ������o�C���h���ꂽ�R�}���h���X�g�͂��̊֐����Ă΂��O�ɕ��Ă����K�v������܂�.@n
		*          ������, GPU��̃R�}���h�����s�����܂�, ���̊֐��͌Ă�ł͂Ȃ�܂���
		*************************************************************************/
		virtual void CleanUp() = 0;

		#pragma endregion

		#pragma region Public Property
		
		/*!**********************************************************************
		*  @brief   �R�}���h���X�g�̎�ނ�Ԃ��܂� (Graphics, compute, copy)
		*  @return  core::CommandList : �R�}���h���X�g�̎��
		*************************************************************************/
		core::CommandListType GetCommandListType() const { return _commandListType; }

		/*!**********************************************************************
		*  @brief   �f�o�b�O�p�̕\������ݒ肵�܂�
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) = 0;

		#pragma endregion
		
		#pragma region Public Constructor and Destructor

		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		RHICommandAllocator() = default;

		/*! @brief �f�X�g���N�^*/
		virtual ~RHICommandAllocator() = default;

		/*! @brief �f�o�C�X�ƃR�}���h���X�g�̎�ނŐ�������R���X�g���N�^*/
		explicit RHICommandAllocator(const gu::SharedPointer<RHIDevice>& device, const core::CommandListType commandListType)
			: _device(device), _commandListType(commandListType)
		{
			Checkf(_device, "device is nullptr.\n");
		};

		#pragma endregion 

		#pragma region Protected Property

		/*! @brief �_���f�o�C�X*/
		gu::SharedPointer<RHIDevice> _device = nullptr;

		/*! @brief �R�}���h���X�g�̎��*/
		CommandListType   _commandListType = core::CommandListType::Unknown;

		#pragma endregion
	};
}
#endif