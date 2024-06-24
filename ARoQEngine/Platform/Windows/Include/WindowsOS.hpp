//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsOS.hpp
///  @brief  OS情報をまとめたクラス. レジストリキーの取得などを行えます.
///  @author toide
///  @date   2024/06/23 23:42:51
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_OS_HPP
#define WINDOWS_OS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUString.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{
	/****************************************************************************
	*				  			   WindowsOS
	*************************************************************************//**
	/*  @brief   OS情報をまとめたクラス
	*****************************************************************************/
	class WindowsOS
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     レジストリキーを探します.
		*  @param[in] void* hKey ルートキー
		*  @param[in] const tstring& subKey サブキー
		*  @param[in] const tstring& valueName 値名
		*  @param[out] tstring& 出力文字列
		*  @return    bool キーが存在するかどうか
		*************************************************************************/
		static bool QueryRegistryKey(void* hKey, const gu::tstring& subKey, const gu::tstring& valueName, gu::tstring& outData);

		/*!**********************************************************************
		*  @brief     Visual studioのCommonToolsのパスを取得します (取得できない場合は空の文字列を返します.)
		*  @param[in] const gu::uint32 Visual Studioのメジャーバージョン (VS2022の場合は17)
		*  @return    tstring CommonToolsのパス
		*************************************************************************/
		static gu::tstring GetVisualStudioCommonToolsPath(const gu::uint32 majorVersion);

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

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