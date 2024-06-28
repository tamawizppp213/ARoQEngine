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
		*  @param[out] tstring& キーの値
		*  @return    bool キーが存在するかどうか
		*************************************************************************/
		static bool QueryRegistryKey(void* hKey, const gu::tstring& subKey, const gu::tstring& valueName, gu::tstring& outData);

		/*!**********************************************************************
		*  @brief     Registryキーに対応する値を取得します. この関数はUserのCurrentUserに存在するキーのみを探します.
		*  @param[in] const gu::tstring& キーが存在する場所
		*  @param[in] const gu::tstring& キー名
		*  @param[out]tstring& キーの値
		*  @return    bool 成功したかどうか
		*************************************************************************/
		static bool GetRegistryKeyValue(const gu::tstring& keyPlace, const gu::tstring& valueName, gu::tstring& outValue);

		/*!**********************************************************************
		*  @brief     Registryキーに対応する値を設定します. この関数はUserのCurrentUserに存在するキーのみを探します.
		*  @param[in] const gu::tstring& キーが存在する場所
		*  @param[in] const gu::tstring& キー名
		*  @param[in] tstring& キーの値
		*  @return    bool 成功したかどうか
		*************************************************************************/
		static bool SetRegistryKeyValue(const gu::tstring& keyPlace, const gu::tstring& valueName, const gu::tstring& value);

		/*!**********************************************************************
		*  @brief     Registryキーに対応する値を削除します. この関数はUserのCurrentUserに存在するキーのみを探します.
		*  @param[in] const gu::tstring& キーが存在する場所
		*  @param[in] const gu::tstring& キー名
		*  @return    bool 成功したかどうか
		*************************************************************************/
		static bool DeleteRegistryKeyValue(const gu::tstring& keyPlace, const gu::tstring& valueName);

		/*!**********************************************************************
		*  @brief     Registryキー自体を削除します. この関数はUserのCurrentUserに存在するキーのみを探します.
		*  @param[in] const gu::tstring& キーが存在する場所
		*  @param[in] const bool 下層のキーも削除するか
		*  @return    bool 成功したかどうか
		*************************************************************************/
		static bool DeleteRegistryKey(const gu::tstring& keyPlace, const bool isRecursive);

		/*!**********************************************************************
		*  @brief     OSのIDを取得します
		*  @param[in] void
		*  @return    tstring ID名
		*************************************************************************/
		static gu::tstring GetOperatingSystemID();

		/*!**********************************************************************
		*  @brief     最後のエラーコードを取得します
		*  @param[in] void
		*  @return    gu::uint32 エラーコード
		*************************************************************************/
		static gu::uint32 GetLastError();

		/*!**********************************************************************
		*  @brief     最後のエラーコードを設定します
		*  @param[in] void
		*  @return    const gu::uint32 エラーコード
		*************************************************************************/
		static void SetLastError(const gu::uint32 errorCode);

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