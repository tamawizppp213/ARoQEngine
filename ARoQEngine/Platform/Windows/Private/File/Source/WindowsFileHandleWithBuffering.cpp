//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsFileHandleAsync.cpp
///  @brief  temp
///  @author toide
///  @date   2024/05/06 23:54:30
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Windows/Private/File/Include/WindowsFileHandleWithBuffering.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "GameUtility/Memory/Include/GUMemory.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform::windows::file;

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
IFileHandleWithBuffering::~IFileHandleWithBuffering()
{
	WaitForAsyncRead();
	Close();
	gu::Memory::Free(&_readBuffers[0]);
	gu::Memory::Free(&_readBuffers[1]);
}

/*! @brief ��{�ݒ�*/
IFileHandleWithBuffering::IFileHandleWithBuffering(const HANDLE handle, const gu::uint32 desiredAccess, const gu::uint32 shareMode, const gu::uint32 flags, const gu::int32 bufferSize)
	: _handle(handle), _position(0), _overlappedPosition(0), _bufferSize(bufferSize), _serializeBufferIndex(0), _flags(flags),
	_streamingBufferIndex(1), _serializePosition(0), _currentAsyncReadBufferIndex(0),
	_desiredAccess(desiredAccess), _shareMode(shareMode), _isEOF(false), _hasReadOutstanding(false)
{
	LARGE_INTEGER largeInteger = {};
	GetFileSizeEx(_handle, &largeInteger);
	_size = largeInteger.QuadPart;

	_readBuffers[0] = (gu::int8*)gu::Memory::Allocate(_bufferSize);
	_readBuffers[1] = (gu::int8*)gu::Memory::Allocate(_bufferSize);

	gu::Memory::Zero(&_overlappedIO, sizeof(OVERLAPPED));
	StartSerializeBufferRead();
}
#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     ���݂̓ǂݍ��݈ʒu�⏑�����݈ʒu��Ԃ��܂�.
*  @param[in] void
*  @return    gu::int64 �ǂݍ��݁E�������݈ʒu, ���߂ȏꍇ��-1��Ԃ��܂�.
*************************************************************************/
gu::int64 IFileHandleWithBuffering::Tell()
{
	Check(IsValid());
	return _position;
}

/*!**********************************************************************
*  @brief     ���݂̓ǂݍ��݈ʒu�⏑�����݈ʒu��V�����ʒu�ɕύX���܂�
*  @param[in] const gu::int64 �V�����ǂݍ��݁E�������݈ʒu
*  @return    bool �ړ����s�����ꍇ��true��Ԃ��܂�.
*************************************************************************/
bool IFileHandleWithBuffering::Seek(const gu::int64 newPosition)
{
	Check(IsValid());
	Check(0 <= newPosition && newPosition <= _size);

	// �t�@�C���|�C���^�̍������擾
	const gu::int64 delta = newPosition - _position;
	if (delta == 0) 
	{
		return true;
	}

	// �ǂ�ȏꍇ�ł����݂̔񓯊��ǂݍ��݂��I������܂ő҂�
	if(!WaitForAsyncRead())
	{
		return false;
	}

	_position = newPosition;

	// �v�����ꂽ�ʒu�����݂̃V���A���C�Y�E�o�b�t�@���ɂȂ��ꍇ�A�ǂݍ��ݏ��������ׂĂ�蒼���K�v������
	const auto absDelta = delta >= 0 ? delta : -delta;
	const auto withinSerializeBuffer = (delta < 0 && (_serializePosition - absDelta) >= 0) || 
	               	                   (delta > 0 && (_serializePosition + absDelta) <= _bufferSize);

	if (withinSerializeBuffer)
	{
		_serializePosition += (gu::int32)delta;
	}
	else
	{
		//  EOF�g���b�L���O�����Z�b�g���A�K�v�ł���Γǂݎ��n���h���ɐݒ肳����
		_isEOF = false;

		// �o�b�t�@���ł͂Ȃ��̂ŁA�V���A���C�Y�o�b�t�@�ŐV���ɔ񓯊��ǂݍ��݂��J�n����
		_overlappedPosition = newPosition;
		CopyOverlappedPosition();
		_currentAsyncReadBufferIndex = _serializeBufferIndex;
		_serializePosition = 0;
		StartSerializeBufferRead();
	}
	return true;
}

/*!**********************************************************************
*  @brief     ���݂̓ǂݍ��݈ʒu�⏑�����݈ʒu���I�[����k���ēǂݍ��݂܂�.
*  @param[in] const gu::int64 �I�[����̍����ʒu (�K��0�ȉ��̐��ɐݒ肵�Ă�������.)
*  @return    bool �ړ����s�����ꍇ��true��Ԃ��܂�.
*************************************************************************/
bool IFileHandleWithBuffering::SeekFromEnd(const gu::int64 relativePositionFromEnd)
{
	Check(IsValid());
	Check(relativePositionFromEnd <= 0);
	return Seek(_size + relativePositionFromEnd);
}

/*!**********************************************************************
*  @brief        �t�@�C������w��̃o�C�g��������destination�̃o�b�t�@�ɏ������݂܂�.
*  @param[inout] void* ���ʂ��i�[����o�b�t�@�ł�. ������, readByte�ȏ�̃o�C�g���͊m�ۂ���K�v������܂�.
*  @param[in]    const gu::int64 �ǂݍ��݃o�C�g��
*  @return       bool �ǂݍ��ݐ���������true
*************************************************************************/
bool IFileHandleWithBuffering::Read(void* destination, const gu::int64 initReadByte)
{
	Check(IsValid());

	if (initReadByte <= 0)
	{
		return false;
	}

	if (_currentAsyncReadBufferIndex == _serializeBufferIndex)
	{
		if (!WaitForAsyncRead())
		{
			return false;
		}
		StartStreamBufferRead();
	}

	Check(destination != nullptr);

	auto readByte = initReadByte;
	while(readByte > 0)
	{
		// �V���A���C�Y�E�o�b�t�@����ǂݍ��߂�o�C�g���𒲂ׂ�
		const gu::int64 numToCopy = readByte > (gu::int64)_bufferSize - _serializePosition ? (gu::int64)_bufferSize - _serializePosition : readByte;

		// �t�@�C���̍Ō���߂��ēǂݍ��ނ��ߎ��s
		if (_position + numToCopy > _size)
		{
			return false;
		}

		// �V���A���C�Y�E�o�b�t�@�̏I�[�ɂ��邩�ǂ������m�F����
		if (numToCopy > 0)
		{
			gu::Memory::Copy(destination, &_readBuffers[_serializeBufferIndex][_serializePosition], numToCopy);

			_serializePosition += (gu::int32)numToCopy;
			Check(_serializePosition <= _bufferSize);
			_position += numToCopy;
			Check(_position <= _size);

			readByte -= numToCopy;

			destination = (gu::uint8*)destination + numToCopy;
		}
		else
		{
			if (!WaitForAsyncRead())
			{
				return false;
			}
			SwapBuffers();
			StartStreamBufferRead();
		}
	}
	return true;
}

/*!**********************************************************************
*  @brief     �t�@�C���̃T�C�Y��Ԃ��܂�
*  @param[in] void
*  @return    gu::int64
*************************************************************************/
gu::int64 IFileHandleWithBuffering::Size()
{
	Check(IsValid());
	return _size;
}
#pragma endregion Public Function

#pragma region Private Function
/*!**********************************************************************
*  @brief     �t�@�C���̃n���h������܂�
*  @param[in] void
*  @return    bool
*************************************************************************/
bool IFileHandleWithBuffering::Close()
{
	/*----------------------------------------------------------------------
	*   �t�@�C���n���h�������ɖ�����΂��̂܂܏I��
	*----------------------------------------------------------------------*/
	if (_handle == nullptr) { return true; }

	/*----------------------------------------------------------------------
	*   �t�@�C���n���h�������݂���ꍇ�̓n���h�������
	*----------------------------------------------------------------------*/
	if (!CloseHandle(_handle))
	{
		return false;
	}
	_handle = nullptr;
	return true;
}

/*!**********************************************************************
*  @brief     �񓯊��ǂݍ��݂̊����҂�
*  @param[in] void
*  @return    bool
*************************************************************************/
bool IFileHandleWithBuffering::WaitForAsyncRead()
{
	/*----------------------------------------------------------------------
	*   �t�@�C���n���h��������EOF�ɓ��B���Ă���ꍇ�͂��̂܂܏I��
	*----------------------------------------------------------------------*/
	if(_isEOF || !_hasReadOutstanding)
	{
		return true;
	}

	/*----------------------------------------------------------------------
	*   �񓯊�����̌��ʂ��擾���Đ��������, �t�@�C���I�t�Z�b�g���X�V����
	*----------------------------------------------------------------------*/
	gu::uint32 readByte = 0;
	if (GetOverlappedResult(_handle, &_overlappedIO, (::DWORD*)&readByte, true) != false)
	{
		UpdateFileOffsetAfterRead(readByte);
		return true;
	}
	/*----------------------------------------------------------------------
	*   EOF�ɓ��B���Ă���ꍇ�͂��̂܂܏I��
	*----------------------------------------------------------------------*/
	else if(GetLastError() == ERROR_HANDLE_EOF)
	{
		_isEOF = true;
		return true;
	}
	else
	{
		return false;
	}
}

/*!**********************************************************************
*  @brief     �񓯊��ǂݍ��݂̊����҂�
*  @param[in] const gu::int32 �ǂݍ��݃o�b�t�@�̃C���f�b�N�X
*  @return    void
*************************************************************************/
void IFileHandleWithBuffering::StartAsyncRead(const gu::int32 readBufferIndex)
{
	/*----------------------------------------------------------------------
	*   �t�@�C���n���h��������EOF�ɓ��B���Ă���ꍇ�͂��̂܂܏I��
	*----------------------------------------------------------------------*/
	if (_isEOF)
	{
		return;
	}

	// �i�s���̓ǂݍ��݂����邱�Ƃ������Ă���
	_hasReadOutstanding = true;

	_currentAsyncReadBufferIndex = readBufferIndex;

	/*----------------------------------------------------------------------
	*   �񓯊��ǂݍ��݂̎��s
	*----------------------------------------------------------------------*/
	gu::uint32 readByte = 0;
	if (!ReadFile(_handle, _readBuffers[readBufferIndex], _bufferSize, (::DWORD*)&readByte, &_overlappedIO))
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			_isEOF = true;
			_hasReadOutstanding = false;
		}
	}
	else
	{
		UpdateFileOffsetAfterRead(readByte);
	}
}

#pragma endregion Private Function