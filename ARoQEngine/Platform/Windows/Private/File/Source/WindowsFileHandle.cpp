//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsFileHandle.cpp
///  @brief  �t�@�C�������p�̓���/�񓯊�IO
///  @author toide
///  @date   2024/05/06 14:32:26
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Windows/Private/File/Include/WindowsFileHandle.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "GameUtility/Memory/Include/GUMemory.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform::windows::file;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
/*! @brief ��{�ݒ�*/
IFileHandle::IFileHandle(const HANDLE handle, const gu::uint32 desiredAccess, const gu::uint32 shareMode, const gu::uint32 flags) :
	_handle(handle), _position(0), _size(0), _desiredAccess(desiredAccess), _shareMode(shareMode), _flags(flags)
{
	if (IsValid())
	{
		UpdateFileSize();
	}

	gu::Memory::Zero(&_overlappedIO, sizeof(OVERLAPPED));
}

IFileHandle::~IFileHandle()
{
	if (_handle)
	{
		CloseHandle(_handle);
		_handle = nullptr;
	}
}
#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     ���݂̓ǂݍ��݈ʒu�⏑�����݈ʒu��Ԃ��܂�.
*  @param[in] void
*  @return    gu::int64 �ǂݍ��݁E�������݈ʒu, ���߂ȏꍇ��-1��Ԃ��܂�.
*************************************************************************/
gu::int64 IFileHandle::Tell()
{
	Check(IsValid());
	return _position;
}

/*!**********************************************************************
*  @brief     �t�@�C���̃T�C�Y��Ԃ��܂�
*  @param[in] void
*  @return    gu::int64
*************************************************************************/
gu::int64 IFileHandle::Size()
{
	Check(IsValid());
	return _size;
}

/*!**********************************************************************
*  @brief     ���݂̓ǂݍ��݈ʒu�⏑�����݈ʒu��V�����ʒu�ɕύX���܂�
*  @param[in] const gu::int64 �V�����ǂݍ��݁E�������݈ʒu
*  @return    bool �ړ����s�����ꍇ��true��Ԃ��܂�.
*************************************************************************/
bool IFileHandle::Seek(const gu::int64 newPosition)
{
	Check(IsValid());
	Check(newPosition >= 0);

	_position = newPosition;
	UpdateOverlappedPosition();
	return true;
}

/*!**********************************************************************
*  @brief     ���݂̓ǂݍ��݈ʒu�⏑�����݈ʒu���I�[����k���ēǂݍ��݂܂�.
*  @param[in] const gu::int64 �I�[����̍����ʒu (�K��0�ȉ��̐��ɐݒ肵�Ă�������.)
*  @return    bool �ړ����s�����ꍇ��true��Ԃ��܂�.
*************************************************************************/
bool IFileHandle::SeekFromEnd(const gu::int64 relativePositionFromEnd)
{
	Check(IsValid());
	Check(relativePositionFromEnd <= 0);

	return Seek(_size + relativePositionFromEnd);
}

/*!**********************************************************************
*  @brief        �t�@�C������w��̃o�C�g��������destination�̃o�b�t�@�ɏ������݂܂�.
*  @param[inout] gu::uint8* ���ʂ��i�[����o�b�t�@�ł�. ������, readByte�ȏ�̃o�C�g���͊m�ۂ���K�v������܂�.
*  @param[in]    const gu::int64 �ǂݍ��݃o�C�g��
*  @return       bool �ǂݍ��ݐ���������true
*************************************************************************/
bool IFileHandle::Read(gu::uint8* destination, const gu::int64 initTotalByte)
{
	Check(IsValid());

	int64 totalNumRead = 0;
	int64 readByte     = initTotalByte;

	do
	{
		uint32 bytesToRead32 = (uint32)(readByte > (int64)MAX_UINT32 ? (int64)(MAX_UINT32) : readByte); // min
		uint32 numRead = 0;

		/*-------------------------------------------------------------------
		-              �t�@�C���̓ǂݍ���
		---------------------------------------------------------------------*/
		if (!ReadFile(_handle, destination, bytesToRead32, (::DWORD*)&numRead, &_overlappedIO))
		{
			const auto errorCode = GetLastError();
			if (errorCode != ERROR_IO_PENDING)
			{
				return false;
			}

			// �ǂݍ��݊����҂�
			numRead = 0;
			if (!GetOverlappedResult(_handle, &_overlappedIO, (::DWORD*)&numRead, true))
			{
				return false;
			}

			readByte    -= bytesToRead32;
			destination += bytesToRead32;
			totalNumRead += numRead;

			_position += numRead;
			UpdateOverlappedPosition();

			if (bytesToRead32 != numRead)
			{
				return false;
			}
		}
	} while (readByte > 0);

	return true;
}

/*!**********************************************************************
*  @brief     �t�@�C���Ɏw��̃o�C�g���������������݂��s���܂�.
*  @param[in] const gu::uint8* ���ʂ��i�[����o�b�t�@�ł�. ������, readByte�ȏ�̃o�C�g���͊m�ۂ���K�v������܂�.
*  @param[in] const gu::int64  �������݃o�C�g��
*  @return    bool �t�@�C���ɏ������݂�����������true
*************************************************************************/
bool IFileHandle::Write(const gu::uint8* source, const gu::int64 initTotalByte)
{
	Check(IsValid());

	int64 totalNumWrite = 0;
	int64 writeByte     = initTotalByte;

	do
	{
		uint32 bytesToWrite32 = (uint32)(writeByte > (int64)MAX_UINT32 ? (int64)(MAX_UINT32) : writeByte); // min
		uint32 numWrite = 0;

		/*-------------------------------------------------------------------
		-              �t�@�C���̏�������
		---------------------------------------------------------------------*/
		if (!WriteFile(_handle, source, bytesToWrite32, (::DWORD*)&numWrite, &_overlappedIO))
		{
			const auto errorCode = GetLastError();
			if (errorCode != ERROR_IO_PENDING)
			{
				return false;
			}

			// �������݊����҂�
			numWrite = 0;
			if (!GetOverlappedResult(_handle, &_overlappedIO, (::DWORD*)&numWrite, true))
			{
				return false;
			}

			writeByte    -= bytesToWrite32;
			source       += bytesToWrite32;
			totalNumWrite += numWrite;

			_position += numWrite;
			UpdateOverlappedPosition();
			_size = _position > _size ? _position : _size;

			if (bytesToWrite32 != numWrite)
			{
				return false;
			}
		}
	} while (writeByte > 0);

	return true;
}

/*!**********************************************************************
*  @brief     �w��o�C�g�T�C�Y�ɂȂ�悤�Ƀt�@�C����؂�l�߂܂�.
*  @param[in] const gu::int64 �؂�l�߂�o�C�g��
*  @return    bool �t�@�C���̐؂�l�߂�����������true
*************************************************************************/
bool IFileHandle::Truncate(const gu::int64 requiredByteSize)
{
	Check(IsValid());

	if (Seek(requiredByteSize) && UpdateNonOverlappedPosition() && SetEndOfFile(_handle) != 0)
	{
		UpdateFileSize();
		Check(IsValid());
		return true;
	}
	return false;
}

#pragma endregion Public Function

#pragma region Private Function
/*!**********************************************************************
*  @brief     �񓯊�IO�̂��߂�Overlapped�\���̂��쐬����
*  @param[in] void
*  @return    void
*************************************************************************/
void IFileHandle::UpdateOverlappedPosition()
{
	ULARGE_INTEGER largeInteger = {};
	largeInteger.QuadPart    = _position;
	_overlappedIO.Offset     = largeInteger.LowPart;
	_overlappedIO.OffsetHigh = largeInteger.HighPart;
}

/*!**********************************************************************
*  @brief     ����IO����̂��߂̃t�@�C���|�C���^���X�V���܂�
*  @param[in] void
*  @return    bool
*************************************************************************/
bool IFileHandle::UpdateNonOverlappedPosition()
{
	LARGE_INTEGER largeInteger = {};
	largeInteger.QuadPart = _position;
	return SetFilePointer(_handle, largeInteger.LowPart, &largeInteger.HighPart, FILE_BEGIN) != INVALID_SET_FILE_POINTER;
}

/*!**********************************************************************
*  @brief     �t�@�C���̃T�C�Y��Ԃ��܂�
*  @param[in] void
*  @return    gu::int64
*************************************************************************/
void IFileHandle::UpdateFileSize()
{
	LARGE_INTEGER largeInteger = {};
	
	bool success = GetFileSizeEx(_handle, &largeInteger);
	if (success)
	{
		_size = largeInteger.QuadPart;
	}
	else
	{
		_size = -1;
		OutputDebugStringW(L"GetFileSizeEx: Failed to get file size for handle!\n");
	}
}
#pragma endregion 