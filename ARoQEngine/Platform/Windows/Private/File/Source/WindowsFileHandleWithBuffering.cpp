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

/*! @brief 基本設定*/
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
*  @brief     現在の読み込み位置や書き込み位置を返します.
*  @param[in] void
*  @return    gu::int64 読み込み・書き込み位置, だめな場合は-1を返します.
*************************************************************************/
gu::int64 IFileHandleWithBuffering::Tell()
{
	Check(IsValid());
	return _position;
}

/*!**********************************************************************
*  @brief     現在の読み込み位置や書き込み位置を新しい位置に変更します
*  @param[in] const gu::int64 新しい読み込み・書き込み位置
*  @return    bool 移動が行えた場合はtrueを返します.
*************************************************************************/
bool IFileHandleWithBuffering::Seek(const gu::int64 newPosition)
{
	Check(IsValid());
	Check(0 <= newPosition && newPosition <= _size);

	// ファイルポインタの差分を取得
	const gu::int64 delta = newPosition - _position;
	if (delta == 0) 
	{
		return true;
	}

	// どんな場合でも現在の非同期読み込みが終了するまで待つ
	if(!WaitForAsyncRead())
	{
		return false;
	}

	_position = newPosition;

	// 要求された位置が現在のシリアライズ・バッファ内にない場合、読み込み処理をすべてやり直す必要がある
	const auto absDelta = delta >= 0 ? delta : -delta;
	const auto withinSerializeBuffer = (delta < 0 && (_serializePosition - absDelta) >= 0) || 
	               	                   (delta > 0 && (_serializePosition + absDelta) <= _bufferSize);

	if (withinSerializeBuffer)
	{
		_serializePosition += (gu::int32)delta;
	}
	else
	{
		//  EOFトラッキングをリセットし、必要であれば読み取りハンドルに設定させる
		_isEOF = false;

		// バッファ内ではないので、シリアライズバッファで新たに非同期読み込みを開始する
		_overlappedPosition = newPosition;
		CopyOverlappedPosition();
		_currentAsyncReadBufferIndex = _serializeBufferIndex;
		_serializePosition = 0;
		StartSerializeBufferRead();
	}
	return true;
}

/*!**********************************************************************
*  @brief     現在の読み込み位置や書き込み位置を終端から遡って読み込みます.
*  @param[in] const gu::int64 終端からの差分位置 (必ず0以下の数に設定してください.)
*  @return    bool 移動が行えた場合はtrueを返します.
*************************************************************************/
bool IFileHandleWithBuffering::SeekFromEnd(const gu::int64 relativePositionFromEnd)
{
	Check(IsValid());
	Check(relativePositionFromEnd <= 0);
	return Seek(_size + relativePositionFromEnd);
}

/*!**********************************************************************
*  @brief        ファイルから指定のバイト数分だけdestinationのバッファに書き込みます.
*  @param[inout] void* 結果を格納するバッファです. ただし, readByte以上のバイト数は確保する必要があります.
*  @param[in]    const gu::int64 読み込みバイト数
*  @return       bool 読み込み成功したらtrue
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
		// シリアライズ・バッファから読み込めるバイト数を調べる
		const gu::int64 numToCopy = readByte > (gu::int64)_bufferSize - _serializePosition ? (gu::int64)_bufferSize - _serializePosition : readByte;

		// ファイルの最後を過ぎて読み込むため失敗
		if (_position + numToCopy > _size)
		{
			return false;
		}

		// シリアライズ・バッファの終端にいるかどうかを確認する
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
*  @brief     ファイルのサイズを返します
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
*  @brief     ファイルのハンドルを閉じます
*  @param[in] void
*  @return    bool
*************************************************************************/
bool IFileHandleWithBuffering::Close()
{
	/*----------------------------------------------------------------------
	*   ファイルハンドルが既に無ければそのまま終了
	*----------------------------------------------------------------------*/
	if (_handle == nullptr) { return true; }

	/*----------------------------------------------------------------------
	*   ファイルハンドルが存在する場合はハンドルを閉じる
	*----------------------------------------------------------------------*/
	if (!CloseHandle(_handle))
	{
		return false;
	}
	_handle = nullptr;
	return true;
}

/*!**********************************************************************
*  @brief     非同期読み込みの完了待ち
*  @param[in] void
*  @return    bool
*************************************************************************/
bool IFileHandleWithBuffering::WaitForAsyncRead()
{
	/*----------------------------------------------------------------------
	*   ファイルハンドルが既にEOFに到達している場合はそのまま終了
	*----------------------------------------------------------------------*/
	if(_isEOF || !_hasReadOutstanding)
	{
		return true;
	}

	/*----------------------------------------------------------------------
	*   非同期操作の結果を取得して成功すれば, ファイルオフセットを更新する
	*----------------------------------------------------------------------*/
	gu::uint32 readByte = 0;
	if (GetOverlappedResult(_handle, &_overlappedIO, (::DWORD*)&readByte, true) != false)
	{
		UpdateFileOffsetAfterRead(readByte);
		return true;
	}
	/*----------------------------------------------------------------------
	*   EOFに到達している場合はそのまま終了
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
*  @brief     非同期読み込みの完了待ち
*  @param[in] const gu::int32 読み込みバッファのインデックス
*  @return    void
*************************************************************************/
void IFileHandleWithBuffering::StartAsyncRead(const gu::int32 readBufferIndex)
{
	/*----------------------------------------------------------------------
	*   ファイルハンドルが既にEOFに到達している場合はそのまま終了
	*----------------------------------------------------------------------*/
	if (_isEOF)
	{
		return;
	}

	// 進行中の読み込みがあることを示しておく
	_hasReadOutstanding = true;

	_currentAsyncReadBufferIndex = readBufferIndex;

	/*----------------------------------------------------------------------
	*   非同期読み込みの実行
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