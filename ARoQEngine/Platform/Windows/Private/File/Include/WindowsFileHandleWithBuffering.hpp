//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsFileHandleAsync.hpp
///  @brief  �t�@�C�������p�̔񓯊�IO
///  @author toide
///  @date   2024/05/06 14:29:06
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_FILE_HANDLE_ASYNC_HPP
#define WINDOWS_FILE_HANDLE_ASYNC_HPP

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
	*				  	 IFileHandleWithBuffering
	*************************************************************************//**
	/*  @brief  �t�@�C�������p�̔񓯊�IO@n
	*           �o�b�t�@��2����, ��͓ǂݍ��ݗp�ł�����̓V���A���C�Y�p�ł�
	*****************************************************************************/
	class IFileHandleWithBuffering : public core::file::IFileHandle
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
		*  @brief      �t�@�C���Ɏw��̃o�C�g���������������݂��s���܂�. 
		*  @attentioin ����͔񓯊����[�_�[�݂̂ŁA�������݂̓T�|�[�g���Ă��Ȃ��B
		*  @param[in] const void* ���ʂ��i�[����o�b�t�@�ł�. ������, readByte�ȏ�̃o�C�g���͊m�ۂ���K�v������܂�.
		*  @param[in] const gu::int64  �������݃o�C�g��
		*  @return    bool �t�@�C���ɏ������݂�����������true
		*************************************************************************/
		virtual bool Write([[maybe_unused]] const void* source, [[maybe_unused]] const gu::int64 writeByte) override
		{
			return false;
		}

		/*!**********************************************************************
		*  @brief     �w��o�C�g�T�C�Y�ɂȂ�悤�Ƀt�@�C����؂�l�߂܂�.
		*  @param[in] const gu::int64 �؂�l�߂�o�C�g��
		*  @return    bool �t�@�C���̐؂�l�߂�����������true
		*************************************************************************/
		virtual bool Truncate([[maybe_unused]] const gu::int64 requiredByteSize) override
		{
			return false;
		}

		/*!**********************************************************************
		*  @brief     �t�@�C�����L�����ǂ��� (�t�@�C���|�C���^�Ȃǂ����݂��邩�ǂ���)
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool IsValid() const
		{
			return _handle != NULL && _handle != INVALID_HANDLE_VALUE;
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
		IFileHandleWithBuffering() = default;

		virtual ~IFileHandleWithBuffering();

		/*! @brief ��{�ݒ�*/
		IFileHandleWithBuffering (const HANDLE handle, const gu::uint32 desiredAccess, const gu::uint32 shareMode, const gu::uint32 flags, const gu::int32 bufferSize = DEFAULT_BUFFER_SIZE);


		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property
		enum { DEFAULT_BUFFER_SIZE = 64 * 1024 };
		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function
		/*!**********************************************************************
		*  @brief     �t�@�C���̃n���h������܂�
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		bool Close();

		/*!**********************************************************************
		*  @brief     �񓯊��ǂݍ��݂̊����҂�
		*  @param[in] void
		*  @return    bool 
		*************************************************************************/
		bool WaitForAsyncRead();

		/*!**********************************************************************
		*  @brief     �񓯊��ǂݍ��݂̊J�n
		*  @param[in] const gu::int32 �ǂݍ��݃o�b�t�@�̃C���f�b�N�X
		*  @return    void
		*************************************************************************/
		void StartAsyncRead(const gu::int32 readBufferIndex);

		/*!**********************************************************************
		*  @brief     �X�g���[�~���O�o�b�t�@�̓ǂݍ��݂��J�n����
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		__forceinline void StartStreamBufferRead()
		{
			StartAsyncRead(_streamingBufferIndex);
		}

		/*!**********************************************************************
		*  @brief     �V���A���C�Y�o�b�t�@�̓ǂݍ��݂��J�n����
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		__forceinline void StartSerializeBufferRead()
		{
			StartAsyncRead(_serializeBufferIndex);
		}

		/*!**********************************************************************
		*  @brief     �o�b�t�@�C���f�b�N�X 0 �� 1 �̊ԂŁA�ǂݍ��݂ƃV���A���C�Y���s���o�b�t�@��؂�ւ��܂��B
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		__forceinline void SwapBuffers()
		{
			_streamingBufferIndex ^= 1;
			_serializeBufferIndex ^= 1;
			_serializePosition = 0;
		}

		/*!**********************************************************************
		*  @brief     �񓯊�IO�̏�Ԃ��`����X�e�[�g�������o�ϐ��ɃR�s�[���܂�.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		__forceinline void CopyOverlappedPosition()
		{
			ULARGE_INTEGER largeInteger = {};
			largeInteger.QuadPart    = _overlappedPosition;
			_overlappedIO.Offset     = largeInteger.LowPart;
			_overlappedIO.OffsetHigh = largeInteger.HighPart;
		}

		/*!**********************************************************************
		*  @brief     FileOffset�̍X�V
		*  @param[in] const gu::uint32 �ǂݍ��݃o�C�g��
		*  @return    void
		*************************************************************************/
		__forceinline void UpdateFileOffsetAfterRead(const gu::uint32 amountRead)
		{
			_hasReadOutstanding = false;
			_overlappedPosition += amountRead;
			CopyOverlappedPosition();
			if(_overlappedPosition >= gu::uint64(_size))
			{
				_isEOF = true;
			}
		}

		#pragma endregion 

		#pragma region Private Property
		/*! @brief �񓯊���Ԃ����肷��̂Ɏg����OverlappedIO*/
		OVERLAPPED _overlappedIO = {};

		/*! @brief File���i�[����Windows�n���h��*/
		HANDLE _handle = nullptr;

		/*! @brief �t�@�C���ƃo�b�t�@��g�ݍ��킹���ǂݏ����̑S�̈ʒu*/
		gu::int64 _position = 0;

		/*! @brief �t�@�C���̑S�̃T�C�Y*/
		gu::int64 _size = 0;

		/*! @brief OverlappedIO�\���̂ɂ�����t�@�C�����̑S�̈ʒu*/
		gu::uint64 _overlappedPosition = 0;

		/*! @brief �t�@�C���n���h�����I�[�v������Ƃ���Windows API�ɓn�����A��]����A�N�Z�X*/
		gu::uint32 _desiredAccess = 0;

		/*! @brief �t�@�C���n���h�����J���Ƃ���Windows API�ɓn����鋤�L���[�h*/
		gu::uint32 _shareMode = 0;

		/*! @brief �t�@�C���n���h�����J���Ƃ���Windows API�ɓn�����t���O*/
		gu::uint32 _flags = 0;

		/*! @brief �񓯊��œǂݍ��ނ��߂̃X�g���[�~���O�o�b�t�@�̌��݂̃C���f�b�N�X*/
		gu::int32 _streamingBufferIndex = 0;

		/*! @brief �V���A���C�Y����o�b�t�@�̌��݂̃C���f�b�N�X*/
		gu::int32 _serializeBufferIndex = 0;

		/*! @brief �V���A���C�Y�o�b�t�@�̈ʒu*/
		gu::int32 _serializePosition = 0;

		/*! @brief �񓯊��ǂݍ��݂��������̃o�b�t�@��ǐՂ��܂� (0 = create/seek ��̍ŏ��̓ǂݍ��݁A1 = �X�g���[�~���O�o�b�t�@)*/
		gu::int32 _currentAsyncReadBufferIndex = 0;

		/*! @brief �o�b�t�@�T�C�Y*/
		gu::int32 _bufferSize = 0;

		/*! @brief �t�@�C����񓯊��œǂݍ��ނ��߂�2�̃o�b�t�@*/
		gu::int8* _readBuffers[2] = { nullptr, nullptr };

		/*! @brief �t�@�C���̏I�[���ǂ����̃t���O. */
		bool _isEOF = false;

		/*! @brief �i�s���̓ǂݍ��݂̗L���𔻒肷��*/
		bool _hasReadOutstanding = false;
		#pragma endregion 

	};
}

#endif