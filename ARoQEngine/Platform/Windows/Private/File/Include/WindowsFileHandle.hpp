//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsFileHandle.hpp
///  @brief  �t�@�C�������p�̓���/�񓯊�IO
///  @author toide
///  @date   2024/05/06 14:29:06
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_FILE_HANDLE_SYNC_HPP
#define WINDOWS_FILE_HANDLE_SYNC_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Core/Private/File/Include/CoreFileHandle.hpp"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows::file
{
	/****************************************************************************
	*				  	 IFileHandle
	****************************************************************************/
	/*  @brief  �t�@�C�������p�̓���/�񓯊�IO
	*****************************************************************************/
	class IFileHandle : public core::file::IFileHandle
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     ���݂̓ǂݍ��݈ʒu�⏑�����݈ʒu��Ԃ��܂�. 
		*  @param[in] void
		*  @return    gu::int64 �ǂݍ��݁E�������݈ʒu, ���߂ȏꍇ��-1��Ԃ��܂�.
		*************************************************************************/
		virtual gu::int64 Tell() override;

		/*!**********************************************************************
		*  @brief     ���݂̓ǂݍ��݈ʒu�⏑�����݈ʒu��V�����ʒu�ɕύX���܂�
		*  @param[in] const gu::int64 �V�����ǂݍ��݁E�������݈ʒu
		*  @return    bool �ړ����s�����ꍇ��true��Ԃ��܂�. 
		*************************************************************************/
		virtual bool Seek(const gu::int64 newPosition) override;

		/*!**********************************************************************
		*  @brief     ���݂̓ǂݍ��݈ʒu�⏑�����݈ʒu���I�[����k���ēǂݍ��݂܂�. 
		*  @param[in] const gu::int64 �I�[����̍����ʒu (�K��0�ȉ��̐��ɐݒ肵�Ă�������.)
		*  @return    bool �ړ����s�����ꍇ��true��Ԃ��܂�.
		*************************************************************************/
		virtual bool SeekFromEnd(const gu::int64 relativePositionFromEnd = 0) override;

		/*!**********************************************************************
		*  @brief        �t�@�C������w��̃o�C�g��������destination�̃o�b�t�@�ɏ������݂܂�. 
		*  @param[inout] void* ���ʂ��i�[����o�b�t�@�ł�. ������, readByte�ȏ�̃o�C�g���͊m�ۂ���K�v������܂�.  
		*  @param[in]    const gu::int64 �ǂݍ��݃o�C�g��
		*  @return       bool �ǂݍ��ݐ���������true
		*************************************************************************/
		virtual bool Read(void* destination, const gu::int64 readByte) override;

		/*!**********************************************************************
		*  @brief     �t�@�C���Ɏw��̃o�C�g���������������݂��s���܂�. 
		*  @param[in] const void* ���ʂ��i�[����o�b�t�@�ł�. ������, readByte�ȏ�̃o�C�g���͊m�ۂ���K�v������܂�.
		*  @param[in] const gu::int64  �������݃o�C�g��
		*  @return    bool �t�@�C���ɏ������݂�����������true
		*************************************************************************/
		virtual bool Write(const void* source, const gu::int64 writeByte) override;

		/*!**********************************************************************
		*  @brief     �w��o�C�g�T�C�Y�ɂȂ�悤�Ƀt�@�C����؂�l�߂܂�.
		*  @param[in] const gu::int64 �؂�l�߂�o�C�g��
		*  @return    bool �t�@�C���̐؂�l�߂�����������true
		*************************************************************************/
		virtual bool Truncate(const gu::int64 requiredByteSize) override;

		/*!**********************************************************************
		*  @brief     �t�@�C�����L�����ǂ��� (�t�@�C���|�C���^�Ȃǂ����݂��邩�ǂ���)
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool IsValid() const
		{
			return _handle != NULL && _handle != INVALID_HANDLE_VALUE && _size != -1;
		}

		/*!**********************************************************************
		*  @brief     �t�@�C���̃T�C�Y��Ԃ��܂�
		*  @param[in] void
		*  @return    gu::int64
		*************************************************************************/
		virtual gu::int64 Size() override;

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		IFileHandle() = default;

		virtual ~IFileHandle();

		/*! @brief ��{�ݒ�*/
		IFileHandle(const HANDLE handle, const gu::uint32 desiredAccess, const gu::uint32 shareMode, const gu::uint32 flags);


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
		/*!**********************************************************************
		*  @brief     �񓯊�IO�̂��߂�Overlapped�\���̂��쐬����
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void UpdateOverlappedPosition();

		/*!**********************************************************************
		*  @brief     ����IO����̂��߂̃t�@�C���|�C���^���X�V���܂�
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		bool UpdateNonOverlappedPosition();

		/*!**********************************************************************
		*  @brief     �t�@�C���T�C�Y���X�V���܂�
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void UpdateFileSize();
		#pragma endregion 

		#pragma region Private Property
		/*! @brief �񓯊���Ԃ����肷��̂Ɏg����OverlappedIO*/
		OVERLAPPED _overlappedIO = {};

		/*! @brief File���i�[����Windows�n���h��*/
		HANDLE _handle = nullptr;

		/*! @brief �t�@�C���̓ǂݏ����ʒu*/
		gu::int64 _position = 0;

		/*! @brief �t�@�C���̑S�̃T�C�Y*/
		gu::int64 _size = 0;

		/*! @brief �t�@�C���n���h�����I�[�v������Ƃ���Windows API�ɓn�����A��]����A�N�Z�X*/
		gu::uint32 _desiredAccess = 0;

		/*! @brief �t�@�C���n���h�����J���Ƃ���Windows API�ɓn����鋤�L���[�h*/
		gu::uint32 _shareMode = 0;

		/*! @brief �t�@�C���n���h�����J���Ƃ���Windows API�ɓn�����t���O*/
		gu::uint32 _flags = 0;
		#pragma endregion 

	};
}

#endif