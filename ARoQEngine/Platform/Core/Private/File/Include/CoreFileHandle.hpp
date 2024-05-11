//////////////////////////////////////////////////////////////////////////////////
///  @file   CoreFileHandle.hpp
///  @brief  File��ǂݍ��ނƂ���, �ǂݍ��݈ʒu��o�^����悤�ȏ������s���܂�.
///  @author toide
///  @date   2024/05/06 13:36:30
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CORE_FILE_HANDLE_HPP
#define CORE_FILE_HANDLE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::core::file
{
	/****************************************************************************
	*				  			   CoreFileHandle
	*************************************************************************//**
	/*  @brief   �t�@�C�������p�̓���/�񓯊�IO
	*****************************************************************************/
	class IFileHandle
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     ���݂̓ǂݍ��݈ʒu�⏑�����݈ʒu��Ԃ��܂�. 
		*  @param[in] void
		*  @return    gu::int64 �ǂݍ��݁E�������݈ʒu, ���߂ȏꍇ��-1��Ԃ��܂�.
		*************************************************************************/
		virtual gu::int64 Tell() = 0;

		/*!**********************************************************************
		*  @brief     ���݂̓ǂݍ��݈ʒu�⏑�����݈ʒu��V�����ʒu�ɕύX���܂�
		*  @param[in] const gu::int64 �V�����ǂݍ��݁E�������݈ʒu
		*  @return    bool �ړ����s�����ꍇ��true��Ԃ��܂�. 
		*************************************************************************/
		virtual bool Seek(const gu::int64 newPosition) = 0;

		/*!**********************************************************************
		*  @brief     ���݂̓ǂݍ��݈ʒu�⏑�����݈ʒu���I�[����k���ēǂݍ��݂܂�. 
		*  @param[in] const gu::int64 �I�[����̍����ʒu (�K��0�ȉ��̐��ɐݒ肵�Ă�������.)
		*  @return    bool �ړ����s�����ꍇ��true��Ԃ��܂�.
		*************************************************************************/
		virtual bool SeekFromEnd(const gu::int64 relativePositionFromEnd = 0) = 0;

		/*!**********************************************************************
		*  @brief        �t�@�C������w��̃o�C�g��������destination�̃o�b�t�@�ɏ������݂܂�. 
		*  @param[inout] void* ���ʂ��i�[����o�b�t�@�ł�. ������, readByte�ȏ�̃o�C�g���͊m�ۂ���K�v������܂�.  
		*  @param[in]    const gu::int64 �ǂݍ��݃o�C�g��
		*  @return       bool �ǂݍ��ݐ���������true
		*************************************************************************/
		virtual bool Read(void* destination, const gu::int64 readByte) = 0;

		/*!**********************************************************************
		*  @brief     �t�@�C���Ɏw��̃o�C�g���������������݂��s���܂�. 
		*  @param[in] const void* ���ʂ��i�[����o�b�t�@�ł�. ������, readByte�ȏ�̃o�C�g���͊m�ۂ���K�v������܂�.
		*  @param[in] const gu::int64  �������݃o�C�g��
		*  @return    bool �t�@�C���ɏ������݂�����������true
		*************************************************************************/
		virtual bool Write(const void* source, const gu::int64 writeByte) = 0;

		/*!**********************************************************************
		*  @brief     �w��o�C�g�T�C�Y�ɂȂ�悤�Ƀt�@�C����؂�l�߂܂�.
		*  @param[in] const gu::int64 �؂�l�߂�o�C�g��
		*  @return    bool �t�@�C���̐؂�l�߂�����������true
		*************************************************************************/
		virtual bool Truncate(const gu::int64 requiredByteSize) = 0;

		/*!**********************************************************************
		*  @brief     �t�@�C���̃T�C�Y��Ԃ��܂�
		*  @param[in] void
		*  @return    gu::int64
		*************************************************************************/
		virtual gu::int64 Size()
		{
			gu::int64 current = Tell();
			SeekFromEnd();
			gu::int64 result  = Tell();
			Seek(current);
			return result;
		}


		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�X�g���N�^*/
		virtual ~IFileHandle() {};
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Property

		#pragma endregion 

	};

}
#endif