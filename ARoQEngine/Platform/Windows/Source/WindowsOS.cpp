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
	// 32bitと64bitの両方を探します
	for (int32 i = 0; i < 2 && !result; ++i)
	{
		HKEY key = NULL;
		const uint32 registryFlags = i == 0 ? KEY_WOW64_32KEY : KEY_WOW64_64KEY;

		// キーが存在するかどうかを確認します
		if (RegOpenKeyExW(hRootKey, subKey.CString(), NULL, KEY_READ | registryFlags, &key) != ERROR_SUCCESS)
		{
			continue;
		}

		// 必要な文字列バッファの大きさを取得します.
		::DWORD bufferSize = 0;
		::DWORD valueType  = 0;
		const auto valueRegQuery = RegQueryValueExW(key, valueName.CString(), NULL, &valueType, NULL, &bufferSize);
		if (RegQueryValueExW(key, valueName.CString(), NULL, &valueType, NULL, &bufferSize) != ERROR_SUCCESS || bufferSize == 0)
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
				if (RegQueryValueExW(key, valueName.CString(), NULL, NULL, reinterpret_cast<BYTE*>(&value), &bufferSize) == ERROR_SUCCESS)
				{
					outData = tstring::FromNumber((uint32)value);
					result = true;
				}
				break;
			}
			case REG_QWORD:
			{
				int64 value = 0;
				if (RegQueryValueExW(key, valueName.CString(), NULL, NULL, reinterpret_cast<BYTE*>(&value), &bufferSize) == ERROR_SUCCESS)
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
				outData = tstring(bufferSize / sizeof(wchar_t), true);

				if (RegQueryValueExW(key, valueName.CString(), NULL, NULL, (LPBYTE)(outData.CString()), &bufferSize) == ERROR_SUCCESS)
				{
					result = true;
				}

				break;
			}
		}
		RegCloseKey(key);
	}

	return result;
}

/*!**********************************************************************
*  @brief     Registryキーに対応する値を取得します. この関数はUserのCurrentUserに存在するキーのみを探します.
*  @param[in] const gu::tstring& キーが存在する場所
*  @param[in] const gu::tstring& キー名
*  @param[out]tstring& キーの値
*  @return    bool 成功したかどうか
*************************************************************************/
bool WindowsOS::GetRegistryKeyValue(const gu::tstring& keyPlace, const gu::tstring& valueName, gu::tstring& outValue)
{
	const auto subKey = tstring(SP("Software\\")) + keyPlace;
	return QueryRegistryKey(HKEY_CURRENT_USER, subKey, valueName, outValue);
}

/*!**********************************************************************
*  @brief     Registryキーに対応する値を設定します. この関数はUserのCurrentUserに存在するキーのみを探します.
*  @param[in] const gu::tstring& キーが存在する場所
*  @param[in] const gu::tstring& キー名
*  @param[in] tstring& キーの値
*  @return    bool 成功したかどうか
*************************************************************************/
bool WindowsOS::SetRegistryKeyValue(const gu::tstring& keyPlace, const gu::tstring& valueName, const gu::tstring& value)
{
	const auto subKey = tstring(SP("Software\\")) + keyPlace;

	HKEY key = NULL;
	if (RegCreateKeyExW(HKEY_CURRENT_USER, subKey.CString(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, NULL) != ERROR_SUCCESS)
	{
		return false;
	}

	const auto result = RegSetValueExW(key, valueName.CString(), 0, REG_SZ, reinterpret_cast<const BYTE*>(value.CString()), (value.Size() + 1) * sizeof(wchar)) == ERROR_SUCCESS;
	RegCloseKey(key);

	return result;
}

/*!**********************************************************************
*  @brief     Registryキーに対応する値を削除します. この関数はUserのCurrentUserに存在するキーのみを探します.
*  @param[in] const gu::tstring& キーが存在する場所
*  @param[in] const gu::tstring& キー名
*  @return    bool 成功したかどうか
*************************************************************************/
bool WindowsOS::DeleteRegistryKeyValue(const gu::tstring& keyPlace, const gu::tstring& valueName)
{
	const auto subKey = tstring(SP("Software\\")) + keyPlace;

	HKEY key = NULL;
	if (RegOpenKeyExW(HKEY_CURRENT_USER, subKey.CString(), 0, KEY_WRITE | KEY_READ, &key) != ERROR_SUCCESS)
	{
		return false;
	}

	/*----------------------------------------------------------------------
	*        値を削除
	*----------------------------------------------------------------------*/
	if (RegDeleteValueW(key, valueName.CString()) != ERROR_SUCCESS)
	{
		RegCloseKey(key);
		return false;	
	}

	/*----------------------------------------------------------------------
	*     対象となるkeyPlaceにおいて, ほかのキーが存在しない場合はkeyPlace自体を削除します
	*----------------------------------------------------------------------*/
	// 開いているキーのサブキーの数を取得します
	tchar   checkKeyName[256]  = { 0 };
	::DWORD checkKeyNameLength = sizeof(checkKeyName) / sizeof(checkKeyName[0]);
	HRESULT enumResult         = RegEnumKeyExW(key, 0, checkKeyName, &checkKeyNameLength, NULL, NULL, NULL, NULL);
	const auto zeroSubKey      = enumResult != ERROR_SUCCESS;

	// オープンキーの残りの値を問い合わせる
	wchar checkValueName[256]  = { 0 };
	::DWORD checkValueNameSize = sizeof(checkValueName) / sizeof(checkValueName[0]);
	HRESULT enumValueResult    = RegEnumValueW(key, 0, checkValueName, &checkValueNameSize, NULL, NULL, NULL, NULL);
	const auto zeroValue       = enumValueResult != ERROR_SUCCESS;

	RegCloseKey(key);

	// キーが存在しない場合は削除します
	if(zeroSubKey && zeroValue)
	{
		::RegDeleteKeyW(HKEY_CURRENT_USER, subKey.CString());
	}

	return true;
}

/*!**********************************************************************
*  @brief     Registryキー自体を削除します. この関数はUserのCurrentUserに存在するキーのみを探します.
*  @param[in] const gu::tstring& キーが存在する場所
*  @param[in] const bool 下層のキーも削除するか
*  @return    bool 成功したかどうか
*************************************************************************/
bool WindowsOS::DeleteRegistryKey(const gu::tstring& keyPlace, const bool isRecursive)
{
	const auto subKey = tstring(SP("Software\\")) + keyPlace;

	if (isRecursive)
	{
		return RegDeleteTreeW(HKEY_CURRENT_USER, subKey.CString()) == ERROR_SUCCESS;
	}
	else
	{
		return RegDeleteKeyW(HKEY_CURRENT_USER, subKey.CString()) == ERROR_SUCCESS;
	}
}

/*!**********************************************************************
*  @brief     OSのIDを取得します
*  @param[in] void
*  @return    tstring ID名
*************************************************************************/
gu::tstring WindowsOS::GetOperatingSystemID()
{
	tstring result = SP("");

	QueryRegistryKey(HKEY_LOCAL_MACHINE, SP("Software\\Microsoft\\Cryptography"), SP("MachineGuid"), result);
	
	return result;
}

/*!**********************************************************************
*  @brief     最後のエラーコードを取得します
*  @param[in] void
*  @return    gu::uint32 エラーコード
*************************************************************************/
gu::uint32 WindowsOS::GetLastError()
{
	return (uint32)::GetLastError();
}

/*!**********************************************************************
*  @brief     最後のエラーコードを設定します
*  @param[in] void
*  @return    const gu::uint32 エラーコード
*************************************************************************/
void WindowsOS::SetLastError(const gu::uint32 errorCode)
{
	::SetLastError((DWORD)errorCode);
}

/*!**********************************************************************
*  @brief     COMライブラリを初期化します
*  @param[in] void
*  @return    bool
*************************************************************************/
bool WindowsOS::CoInitialize(const bool useMultiThread)
{
	HRESULT hResult = ::CoInitializeEx(nullptr, useMultiThread ? COINIT_MULTITHREADED : COINIT_APARTMENTTHREADED);

	// S_FALSEについては既に初期化が行われているために成功判定とみなします. 
	return hResult == S_OK || hResult == S_FALSE;
}

/*!**********************************************************************
*  @brief     COMライブラリを終了します
*  @param[in] void
*  @return    void
*************************************************************************/
void WindowsOS::CoUninitialize()
{
	::CoUninitialize();
}

/*!**********************************************************************
*  @brief     指定されたファイル名のDLLをロードし, そのハンドルを返します. @n
*             https://qiita.com/tera1707/items/727e7ad4dc9f3205c137
*  @param[in] const tstring& fiieName : ロードしたいDLLのファイル名
*  @return    void* : ロードしたDLLのハンドル
*************************************************************************/
void* WindowsOS::GetDLLHandle(const gu::tstring& fileName)
{
	return ::LoadLibraryW(fileName.CString());
}

/*!**********************************************************************
*  @brief     指定されたファイル名のDLLをメモリから解放します
*  @param[in] void* handle : ロードしたDLLのハンドル
*  @return    bool 結果
*************************************************************************/
bool WindowsOS::FreeDLLHandle(void* handle)
{
	return !!::FreeLibrary((HMODULE)handle);
}

#pragma endregion Public Function