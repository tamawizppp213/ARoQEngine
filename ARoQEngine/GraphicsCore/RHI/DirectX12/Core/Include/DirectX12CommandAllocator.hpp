//////////////////////////////////////////////////////////////////////////////////
/// @file   DirectX12CommandAllocator.hpp
/// @brief  ���̃N���X�́A�R�}���h���X�g�̃��������i�[���邽�߂Ɏg�p����܂�. @n 
///         �R�}���h���X�g�̃��Z�b�g�֐����Ă΂��ƁA�������u���b�N���������ݏ�ԂɈڍs���邱�ƂɂȂ�܂�. @n
///         Cleanup���s�������_��, �R�}���h�A���P�[�^�̒��g��S�Ĕj�����܂�. ���̍�, GPU���߂͊��Ɏ��s�������Ă��Ȃ��ƃG���[���o�܂�. 
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
	*  @brief     �R�}���h���X�g�̃��������i�[���邽�߂Ɏg�p����܂� @n
	*             �R�}���h���X�g�ŃA���P�[�^��GPU���߂��i�[��, �R�}���h�L���[�Ŗ��߂����s. ����������Cleanup�Œ��g���������܂�.
	*****************************************************************************/
	class RHICommandAllocator : public rhi::core::RHICommandAllocator
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief  �R�}���h�A���P�[�^�[�����Z�b�g���܂� (�R�}���h�o�b�t�@�̍폜) @n
		*          ������o�C���h���ꂽ�R�}���h���X�g�͂��̊֐����Ă΂��O�ɕ��Ă����K�v������܂�.@n
		*          ������, GPU��̃R�}���h�����s�����܂�, ���̊֐��͌Ă�ł͂Ȃ�܂���
		*************************************************************************/
		virtual void CleanUp() override;

		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief   DirectX12�Ŏg�p����R�}���h�A���P�[�^�ł�
		*************************************************************************/
		CommandAllocatorComPtr GetAllocator() { return _commandAllocator; }

		/*!**********************************************************************
		*  @brief   �f�o�b�O�p�̕\������ݒ肵�܂�
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) override;
		#pragma endregion
		
		#pragma region Public Constructor and Destructor

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		RHICommandAllocator() = default;

		/*! @brief �f�X�g���N�^*/
		~RHICommandAllocator();

		/*! @brief �f�o�C�X�ƃR�}���h���X�g�̎�ނŐ�������R���X�g���N�^*/
		explicit RHICommandAllocator(
			const gu::SharedPointer<rhi::core::RHIDevice>& device,
			const core::CommandListType type,
			const gu::tstring& name);
		#pragma endregion 

	protected:
		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
		/*! @brief  DirectX12�Ŏg�p����R�}���h�A���P�[�^�ł�*/
		CommandAllocatorComPtr _commandAllocator = nullptr;

		#pragma endregion
	};
}
#endif