//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsFileHandle.hpp
///  @brief  ファイル走査用の同期/非同期IO
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
	/*  @brief  ファイル走査用の同期/非同期IO
	*****************************************************************************/
	class IFileHandle : public core::file::IFileHandle
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
		*  @brief     ファイルに指定のバイト数分だけ書き込みを行います. 
		*  @param[in] const void* 結果を格納するバッファです. ただし, readByte以上のバイト数は確保する必要があります.
		*  @param[in] const gu::int64  書き込みバイト数
		*  @return    bool ファイルに書き込みが成功したらtrue
		*************************************************************************/
		virtual bool Write(const void* source, const gu::int64 writeByte) override;

		/*!**********************************************************************
		*  @brief     指定バイトサイズになるようにファイルを切り詰めます.
		*  @param[in] const gu::int64 切り詰めるバイト数
		*  @return    bool ファイルの切り詰めが成功したらtrue
		*************************************************************************/
		virtual bool Truncate(const gu::int64 requiredByteSize) override;

		/*!**********************************************************************
		*  @brief     ファイルが有効かどうか (ファイルポインタなどが存在するかどうか)
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool IsValid() const
		{
			return _handle != NULL && _handle != INVALID_HANDLE_VALUE && _size != -1;
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
		IFileHandle() = default;

		virtual ~IFileHandle();

		/*! @brief 基本設定*/
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
		*  @brief     非同期IOのためのOverlapped構造体を作成する
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void UpdateOverlappedPosition();

		/*!**********************************************************************
		*  @brief     同期IO操作のためのファイルポインタを更新します
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		bool UpdateNonOverlappedPosition();

		/*!**********************************************************************
		*  @brief     ファイルサイズを更新します
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void UpdateFileSize();
		#pragma endregion 

		#pragma region Private Property
		/*! @brief 非同期状態を決定するのに使われるOverlappedIO*/
		OVERLAPPED _overlappedIO = {};

		/*! @brief Fileを格納するWindowsハンドル*/
		HANDLE _handle = nullptr;

		/*! @brief ファイルの読み書き位置*/
		gu::int64 _position = 0;

		/*! @brief ファイルの全体サイズ*/
		gu::int64 _size = 0;

		/*! @brief ファイルハンドルをオープンするときにWindows APIに渡される、希望するアクセス*/
		gu::uint32 _desiredAccess = 0;

		/*! @brief ファイルハンドルを開くときにWindows APIに渡される共有モード*/
		gu::uint32 _shareMode = 0;

		/*! @brief ファイルハンドルを開くときにWindows APIに渡されるフラグ*/
		gu::uint32 _flags = 0;
		#pragma endregion 

	};
}

#endif