//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsFileHandle.cpp
///  @brief  ファイル走査用の同期/非同期IO
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
/*! @brief 基本設定*/
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
*  @brief     現在の読み込み位置や書き込み位置を返します.
*  @param[in] void
*  @return    gu::int64 読み込み・書き込み位置, だめな場合は-1を返します.
*************************************************************************/
gu::int64 IFileHandle::Tell()
{
	Check(IsValid());
	return _position;
}

/*!**********************************************************************
*  @brief     ファイルのサイズを返します
*  @param[in] void
*  @return    gu::int64
*************************************************************************/
gu::int64 IFileHandle::Size()
{
	Check(IsValid());
	return _size;
}

/*!**********************************************************************
*  @brief     現在の読み込み位置や書き込み位置を新しい位置に変更します
*  @param[in] const gu::int64 新しい読み込み・書き込み位置
*  @return    bool 移動が行えた場合はtrueを返します.
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
*  @brief     現在の読み込み位置や書き込み位置を終端から遡って読み込みます.
*  @param[in] const gu::int64 終端からの差分位置 (必ず0以下の数に設定してください.)
*  @return    bool 移動が行えた場合はtrueを返します.
*************************************************************************/
bool IFileHandle::SeekFromEnd(const gu::int64 relativePositionFromEnd)
{
	Check(IsValid());
	Check(relativePositionFromEnd <= 0);

	return Seek(_size + relativePositionFromEnd);
}

/*!**********************************************************************
*  @brief        ファイルから指定のバイト数分だけdestinationのバッファに書き込みます.
*  @param[inout] gu::uint8* 結果を格納するバッファです. ただし, readByte以上のバイト数は確保する必要があります.
*  @param[in]    const gu::int64 読み込みバイト数
*  @return       bool 読み込み成功したらtrue
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
		-              ファイルの読み込み
		---------------------------------------------------------------------*/
		if (!ReadFile(_handle, destination, bytesToRead32, (::DWORD*)&numRead, &_overlappedIO))
		{
			const auto errorCode = GetLastError();
			if (errorCode != ERROR_IO_PENDING)
			{
				return false;
			}

			// 読み込み完了待ち
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
*  @brief     ファイルに指定のバイト数分だけ書き込みを行います.
*  @param[in] const gu::uint8* 結果を格納するバッファです. ただし, readByte以上のバイト数は確保する必要があります.
*  @param[in] const gu::int64  書き込みバイト数
*  @return    bool ファイルに書き込みが成功したらtrue
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
		-              ファイルの書き込み
		---------------------------------------------------------------------*/
		if (!WriteFile(_handle, source, bytesToWrite32, (::DWORD*)&numWrite, &_overlappedIO))
		{
			const auto errorCode = GetLastError();
			if (errorCode != ERROR_IO_PENDING)
			{
				return false;
			}

			// 書き込み完了待ち
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
*  @brief     指定バイトサイズになるようにファイルを切り詰めます.
*  @param[in] const gu::int64 切り詰めるバイト数
*  @return    bool ファイルの切り詰めが成功したらtrue
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
*  @brief     非同期IOのためのOverlapped構造体を作成する
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
*  @brief     同期IO操作のためのファイルポインタを更新します
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
*  @brief     ファイルのサイズを返します
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