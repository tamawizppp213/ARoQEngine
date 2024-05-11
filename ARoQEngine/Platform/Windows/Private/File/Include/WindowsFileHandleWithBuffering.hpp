//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsFileHandleAsync.hpp
///  @brief  ファイル走査用の非同期IO
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
	/*  @brief  ファイル走査用の非同期IO@n
	*           バッファは2つあり, 一つは読み込み用でもう一つはシリアライズ用です
	*****************************************************************************/
	class IFileHandleWithBuffering : public core::file::IFileHandle
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     現在の読み込み位置や書き込み位置を返します. 
		*  @param[in] void
		*  @return    gu::int64 読み込み・書き込み位置, だめな場合は-1を返します.
		*************************************************************************/
		virtual gu::int64 Tell() override;

		/*!**********************************************************************
		*  @brief     現在の読み込み位置や書き込み位置を新しい位置に変更します
		*  @param[in] const gu::int64 新しい読み込み・書き込み位置
		*  @return    bool 移動が行えた場合はtrueを返します. 
		*************************************************************************/
		virtual bool Seek(const gu::int64 newPosition) override;

		/*!**********************************************************************
		*  @brief     現在の読み込み位置や書き込み位置を終端から遡って読み込みます. 
		*  @param[in] const gu::int64 終端からの差分位置 (必ず0以下の数に設定してください.)
		*  @return    bool 移動が行えた場合はtrueを返します.
		*************************************************************************/
		virtual bool SeekFromEnd(const gu::int64 relativePositionFromEnd = 0) override;

		/*!**********************************************************************
		*  @brief        ファイルから指定のバイト数分だけdestinationのバッファに書き込みます. 
		*  @param[inout] void* 結果を格納するバッファです. ただし, readByte以上のバイト数は確保する必要があります.  
		*  @param[in]    const gu::int64 読み込みバイト数
		*  @return       bool 読み込み成功したらtrue
		*************************************************************************/
		virtual bool Read(void* destination, const gu::int64 readByte) override;

		/*!**********************************************************************
		*  @brief      ファイルに指定のバイト数分だけ書き込みを行います. 
		*  @attentioin これは非同期リーダーのみで、書き込みはサポートしていない。
		*  @param[in] const void* 結果を格納するバッファです. ただし, readByte以上のバイト数は確保する必要があります.
		*  @param[in] const gu::int64  書き込みバイト数
		*  @return    bool ファイルに書き込みが成功したらtrue
		*************************************************************************/
		virtual bool Write([[maybe_unused]] const void* source, [[maybe_unused]] const gu::int64 writeByte) override
		{
			return false;
		}

		/*!**********************************************************************
		*  @brief     指定バイトサイズになるようにファイルを切り詰めます.
		*  @param[in] const gu::int64 切り詰めるバイト数
		*  @return    bool ファイルの切り詰めが成功したらtrue
		*************************************************************************/
		virtual bool Truncate([[maybe_unused]] const gu::int64 requiredByteSize) override
		{
			return false;
		}

		/*!**********************************************************************
		*  @brief     ファイルが有効かどうか (ファイルポインタなどが存在するかどうか)
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool IsValid() const
		{
			return _handle != NULL && _handle != INVALID_HANDLE_VALUE;
		}

		/*!**********************************************************************
		*  @brief     ファイルのサイズを返します
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
		/*! @brief デフォルトコンストラクタ*/
		IFileHandleWithBuffering() = default;

		virtual ~IFileHandleWithBuffering();

		/*! @brief 基本設定*/
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
		*  @brief     ファイルのハンドルを閉じます
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		bool Close();

		/*!**********************************************************************
		*  @brief     非同期読み込みの完了待ち
		*  @param[in] void
		*  @return    bool 
		*************************************************************************/
		bool WaitForAsyncRead();

		/*!**********************************************************************
		*  @brief     非同期読み込みの開始
		*  @param[in] const gu::int32 読み込みバッファのインデックス
		*  @return    void
		*************************************************************************/
		void StartAsyncRead(const gu::int32 readBufferIndex);

		/*!**********************************************************************
		*  @brief     ストリーミングバッファの読み込みを開始する
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		__forceinline void StartStreamBufferRead()
		{
			StartAsyncRead(_streamingBufferIndex);
		}

		/*!**********************************************************************
		*  @brief     シリアライズバッファの読み込みを開始する
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		__forceinline void StartSerializeBufferRead()
		{
			StartAsyncRead(_serializeBufferIndex);
		}

		/*!**********************************************************************
		*  @brief     バッファインデックス 0 と 1 の間で、読み込みとシリアライズを行うバッファを切り替えます。
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
		*  @brief     非同期IOの状態を定義するステートをメンバ変数にコピーします.
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
		*  @brief     FileOffsetの更新
		*  @param[in] const gu::uint32 読み込みバイト数
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
		/*! @brief 非同期状態を決定するのに使われるOverlappedIO*/
		OVERLAPPED _overlappedIO = {};

		/*! @brief Fileを格納するWindowsハンドル*/
		HANDLE _handle = nullptr;

		/*! @brief ファイルとバッファを組み合わせた読み書きの全体位置*/
		gu::int64 _position = 0;

		/*! @brief ファイルの全体サイズ*/
		gu::int64 _size = 0;

		/*! @brief OverlappedIO構造体におけるファイル内の全体位置*/
		gu::uint64 _overlappedPosition = 0;

		/*! @brief ファイルハンドルをオープンするときにWindows APIに渡される、希望するアクセス*/
		gu::uint32 _desiredAccess = 0;

		/*! @brief ファイルハンドルを開くときにWindows APIに渡される共有モード*/
		gu::uint32 _shareMode = 0;

		/*! @brief ファイルハンドルを開くときにWindows APIに渡されるフラグ*/
		gu::uint32 _flags = 0;

		/*! @brief 非同期で読み込むためのストリーミングバッファの現在のインデックス*/
		gu::int32 _streamingBufferIndex = 0;

		/*! @brief シリアライズするバッファの現在のインデックス*/
		gu::int32 _serializeBufferIndex = 0;

		/*! @brief シリアライズバッファの位置*/
		gu::int32 _serializePosition = 0;

		/*! @brief 非同期読み込みが未処理のバッファを追跡します (0 = create/seek 後の最初の読み込み、1 = ストリーミングバッファ)*/
		gu::int32 _currentAsyncReadBufferIndex = 0;

		/*! @brief バッファサイズ*/
		gu::int32 _bufferSize = 0;

		/*! @brief ファイルを非同期で読み込むための2つのバッファ*/
		gu::int8* _readBuffers[2] = { nullptr, nullptr };

		/*! @brief ファイルの終端かどうかのフラグ. */
		bool _isEOF = false;

		/*! @brief 進行中の読み込みの有無を判定する*/
		bool _hasReadOutstanding = false;
		#pragma endregion 

	};
}

#endif