//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsOS.cpp
///  @brief  temp
///  @author toide
///  @date   2024/06/24 0:25:47
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Windows/Include/WindowsOS.hpp"
#include "GameUtility/Base/Include/GUStringConverter.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform::windows;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     レジストリキーを探します.
*  @param[in] void* hKey ルートキー
*  @param[in] const tstring& subKey サブキー
*  @param[in] const tstring& valueName 値名
*  @param[out] tstring& 出力文字列
*  @return    bool キーが存在するかどうか
*************************************************************************/
bool WindowsOS::QueryRegistryKey(void* hKey, const gu::tstring& subKey, const gu::tstring& valueName, gu::tstring& outData)
{
	const HKEY hRootKey = reinterpret_cast<HKEY>(hKey);

	bool result = false;

	/*----------------------------------------------------------------------
	*         レジストリキーを探します
	*----------------------------------------------------------------------*/
	for (int32 i = 0; i < 2 && !result; ++i)
	{
		HKEY key = NULL;
		const uint32 registryFlags = i == 0 ? KEY_WOW64_32KEY : KEY_WOW64_64KEY;

		// キーが存在するかどうかを確認します
		if (RegOpenKeyEx(hRootKey, subKey.CString(), NULL, KEY_READ | registryFlags, &key) != ERROR_SUCCESS)
		{
			continue;
		}

		// 必要な文字列バッファの大きさを取得します.
		::DWORD bufferSize = 0;
		::DWORD valueType  = 0;
		if (RegQueryValueEx(key, valueName.CString(), NULL, &valueType, NULL, &bufferSize) != ERROR_SUCCESS || bufferSize == 0)
		{
			RegCloseKey(key);
			continue;
		}

		// レジストリキーの値を取得します
		switch (valueType)
		{
			case REG_DWORD:
			{
				DWORD value = 0;
				if (RegQueryValueEx(key, valueName.CString(), NULL, NULL, reinterpret_cast<BYTE*>(&value), &bufferSize) == ERROR_SUCCESS)
				{
					outData = tstring::FromNumber((uint32)value);
					result = true;
				}
				break;
			}
			case REG_QWORD:
			{
				int64 value = 0;
				if (RegQueryValueEx(key, valueName.CString(), NULL, NULL, reinterpret_cast<BYTE*>(&value), &bufferSize) == ERROR_SUCCESS)
				{
					outData = tstring::FromNumber((uint64)value);
					result = true;
				}
				break;
			}
			case REG_SZ:
			case REG_EXPAND_SZ:
			case REG_MULTI_SZ:
			{
				char* buffer = new char[bufferSize];

				if (RegQueryValueEx(key, valueName.CString(), NULL, NULL, reinterpret_cast<BYTE*>(buffer), &bufferSize) == ERROR_SUCCESS)
				{
					outData = StringConverter::ConvertStringToTString(string(buffer));
					result = true;
				}

				delete[] buffer;
				break;
			}
		}
		RegCloseKey(key);
	}

	return result;
}

/*!**********************************************************************
*  @brief     Visual studioのCommonToolsのパスを取得します (取得できない場合は空の文字列を返します.)
*  @param[in] const gu::uint32 Visual Studioのメジャーバージョン (VS2022の場合は17)
*  @return    tstring CommonToolsのパス
*************************************************************************/
gu::tstring WindowsOS::GetVisualStudioCommonToolsPath(const gu::uint32 majorVersion)
{
	/*----------------------------------------------------------------------
	*      Visual studioは2013以降のバージョンから使用可能です
	*----------------------------------------------------------------------*/
	return tstring();
}

#pragma endregion Public Function