//////////////////////////////////////////////////////////////////////////////////
///  @file   CoreFileHandle.hpp
///  @brief  Fileを読み込むときに, 読み込み位置を登録するような処理が行えます.
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
	/*  @brief   ファイル走査用の同期/非同期IO
	*****************************************************************************/
	class IFileHandle
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     現在の読み込み位置や書き込み位置を返します. 
		*  @param[in] void
		*  @return    gu::int64 読み込み・書き込み位置, だめな場合は-1を返します.
		*************************************************************************/
		virtual gu::int64 Tell() = 0;

		/*!**********************************************************************
		*  @brief     現在の読み込み位置や書き込み位置を新しい位置に変更します
		*  @param[in] const gu::int64 新しい読み込み・書き込み位置
		*  @return    bool 移動が行えた場合はtrueを返します. 
		*************************************************************************/
		virtual bool Seek(const gu::int64 newPosition) = 0;

		/*!**********************************************************************
		*  @brief     現在の読み込み位置や書き込み位置を終端から遡って読み込みます. 
		*  @param[in] const gu::int64 終端からの差分位置 (必ず0以下の数に設定してください.)
		*  @return    bool 移動が行えた場合はtrueを返します.
		*************************************************************************/
		virtual bool SeekFromEnd(const gu::int64 relativePositionFromEnd = 0) = 0;

		/*!**********************************************************************
		*  @brief        ファイルから指定のバイト数分だけdestinationのバッファに書き込みます. 
		*  @param[inout] void* 結果を格納するバッファです. ただし, readByte以上のバイト数は確保する必要があります.  
		*  @param[in]    const gu::int64 読み込みバイト数
		*  @return       bool 読み込み成功したらtrue
		*************************************************************************/
		virtual bool Read(void* destination, const gu::int64 readByte) = 0;

		/*!**********************************************************************
		*  @brief     ファイルに指定のバイト数分だけ書き込みを行います. 
		*  @param[in] const void* 結果を格納するバッファです. ただし, readByte以上のバイト数は確保する必要があります.
		*  @param[in] const gu::int64  書き込みバイト数
		*  @return    bool ファイルに書き込みが成功したらtrue
		*************************************************************************/
		virtual bool Write(const void* source, const gu::int64 writeByte) = 0;

		/*!**********************************************************************
		*  @brief     指定バイトサイズになるようにファイルを切り詰めます.
		*  @param[in] const gu::int64 切り詰めるバイト数
		*  @return    bool ファイルの切り詰めが成功したらtrue
		*************************************************************************/
		virtual bool Truncate(const gu::int64 requiredByteSize) = 0;

		/*!**********************************************************************
		*  @brief     ファイルのサイズを返します
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
		/*! @brief デストラクタ*/
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