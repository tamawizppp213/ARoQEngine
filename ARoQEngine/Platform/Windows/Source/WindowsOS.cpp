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
*  @brief     ���W�X�g���L�[��T���܂�.
*  @param[in] void* hKey ���[�g�L�[
*  @param[in] const tstring& subKey �T�u�L�[
*  @param[in] const tstring& valueName �l��
*  @param[out] tstring& �o�͕�����
*  @return    bool �L�[�����݂��邩�ǂ���
*************************************************************************/
bool WindowsOS::QueryRegistryKey(void* hKey, const gu::tstring& subKey, const gu::tstring& valueName, gu::tstring& outData)
{
	const HKEY hRootKey = reinterpret_cast<HKEY>(hKey);

	bool result = false;

	/*----------------------------------------------------------------------
	*         ���W�X�g���L�[��T���܂�
	*----------------------------------------------------------------------*/
	// 32bit��64bit�̗�����T���܂�
	for (int32 i = 0; i < 2 && !result; ++i)
	{
		HKEY key = NULL;
		const uint32 registryFlags = i == 0 ? KEY_WOW64_32KEY : KEY_WOW64_64KEY;

		// �L�[�����݂��邩�ǂ������m�F���܂�
		if (RegOpenKeyExW(hRootKey, subKey.CString(), NULL, KEY_READ | registryFlags, &key) != ERROR_SUCCESS)
		{
			continue;
		}

		// �K�v�ȕ�����o�b�t�@�̑傫�����擾���܂�.
		::DWORD bufferSize = 0;
		::DWORD valueType  = 0;
		const auto valueRegQuery = RegQueryValueExW(key, valueName.CString(), NULL, &valueType, NULL, &bufferSize);
		if (RegQueryValueExW(key, valueName.CString(), NULL, &valueType, NULL, &bufferSize) != ERROR_SUCCESS || bufferSize == 0)
		{
			RegCloseKey(key);
			continue;
		}

		// ���W�X�g���L�[�̒l���擾���܂�
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
*  @brief     Registry�L�[�ɑΉ�����l���擾���܂�. ���̊֐���User��CurrentUser�ɑ��݂���L�[�݂̂�T���܂�.
*  @param[in] const gu::tstring& �L�[�����݂���ꏊ
*  @param[in] const gu::tstring& �L�[��
*  @param[out]tstring& �L�[�̒l
*  @return    bool �����������ǂ���
*************************************************************************/
bool WindowsOS::GetRegistryKeyValue(const gu::tstring& keyPlace, const gu::tstring& valueName, gu::tstring& outValue)
{
	const auto subKey = tstring(SP("Software\\")) + keyPlace;
	return QueryRegistryKey(HKEY_CURRENT_USER, subKey, valueName, outValue);
}

/*!**********************************************************************
*  @brief     Registry�L�[�ɑΉ�����l��ݒ肵�܂�. ���̊֐���User��CurrentUser�ɑ��݂���L�[�݂̂�T���܂�.
*  @param[in] const gu::tstring& �L�[�����݂���ꏊ
*  @param[in] const gu::tstring& �L�[��
*  @param[in] tstring& �L�[�̒l
*  @return    bool �����������ǂ���
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
*  @brief     Registry�L�[�ɑΉ�����l���폜���܂�. ���̊֐���User��CurrentUser�ɑ��݂���L�[�݂̂�T���܂�.
*  @param[in] const gu::tstring& �L�[�����݂���ꏊ
*  @param[in] const gu::tstring& �L�[��
*  @return    bool �����������ǂ���
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
	*        �l���폜
	*----------------------------------------------------------------------*/
	if (RegDeleteValueW(key, valueName.CString()) != ERROR_SUCCESS)
	{
		RegCloseKey(key);
		return false;	
	}

	/*----------------------------------------------------------------------
	*     �ΏۂƂȂ�keyPlace�ɂ�����, �ق��̃L�[�����݂��Ȃ��ꍇ��keyPlace���̂��폜���܂�
	*----------------------------------------------------------------------*/
	// �J���Ă���L�[�̃T�u�L�[�̐����擾���܂�
	tchar   checkKeyName[256]  = { 0 };
	::DWORD checkKeyNameLength = sizeof(checkKeyName) / sizeof(checkKeyName[0]);
	HRESULT enumResult         = RegEnumKeyExW(key, 0, checkKeyName, &checkKeyNameLength, NULL, NULL, NULL, NULL);
	const auto zeroSubKey      = enumResult != ERROR_SUCCESS;

	// �I�[�v���L�[�̎c��̒l��₢���킹��
	wchar checkValueName[256]  = { 0 };
	::DWORD checkValueNameSize = sizeof(checkValueName) / sizeof(checkValueName[0]);
	HRESULT enumValueResult    = RegEnumValueW(key, 0, checkValueName, &checkValueNameSize, NULL, NULL, NULL, NULL);
	const auto zeroValue       = enumValueResult != ERROR_SUCCESS;

	RegCloseKey(key);

	// �L�[�����݂��Ȃ��ꍇ�͍폜���܂�
	if(zeroSubKey && zeroValue)
	{
		::RegDeleteKeyW(HKEY_CURRENT_USER, subKey.CString());
	}

	return true;
}

/*!**********************************************************************
*  @brief     Registry�L�[���̂��폜���܂�. ���̊֐���User��CurrentUser�ɑ��݂���L�[�݂̂�T���܂�.
*  @param[in] const gu::tstring& �L�[�����݂���ꏊ
*  @param[in] const bool ���w�̃L�[���폜���邩
*  @return    bool �����������ǂ���
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
*  @brief     OS��ID���擾���܂�
*  @param[in] void
*  @return    tstring ID��
*************************************************************************/
gu::tstring WindowsOS::GetOperatingSystemID()
{
	tstring result = SP("");

	QueryRegistryKey(HKEY_LOCAL_MACHINE, SP("Software\\Microsoft\\Cryptography"), SP("MachineGuid"), result);
	
	return result;
}

/*!**********************************************************************
*  @brief     �Ō�̃G���[�R�[�h���擾���܂�
*  @param[in] void
*  @return    gu::uint32 �G���[�R�[�h
*************************************************************************/
gu::uint32 WindowsOS::GetLastError()
{
	return (uint32)::GetLastError();
}

/*!**********************************************************************
*  @brief     �Ō�̃G���[�R�[�h��ݒ肵�܂�
*  @param[in] void
*  @return    const gu::uint32 �G���[�R�[�h
*************************************************************************/
void WindowsOS::SetLastError(const gu::uint32 errorCode)
{
	::SetLastError((DWORD)errorCode);
}

/*!**********************************************************************
*  @brief     COM���C�u���������������܂�
*  @param[in] void
*  @return    bool
*************************************************************************/
bool WindowsOS::CoInitialize(const bool useMultiThread)
{
	HRESULT hResult = ::CoInitializeEx(nullptr, useMultiThread ? COINIT_MULTITHREADED : COINIT_APARTMENTTHREADED);

	// S_FALSE�ɂ��Ă͊��ɏ��������s���Ă��邽�߂ɐ�������Ƃ݂Ȃ��܂�. 
	return hResult == S_OK || hResult == S_FALSE;
}

/*!**********************************************************************
*  @brief     COM���C�u�������I�����܂�
*  @param[in] void
*  @return    void
*************************************************************************/
void WindowsOS::CoUninitialize()
{
	::CoUninitialize();
}

/*!**********************************************************************
*  @brief     �w�肳�ꂽ�t�@�C������DLL�����[�h��, ���̃n���h����Ԃ��܂�. @n
*             https://qiita.com/tera1707/items/727e7ad4dc9f3205c137
*  @param[in] const tstring& fiieName : ���[�h������DLL�̃t�@�C����
*  @return    void* : ���[�h����DLL�̃n���h��
*************************************************************************/
void* WindowsOS::GetDLLHandle(const gu::tstring& fileName)
{
	return ::LoadLibraryW(fileName.CString());
}

/*!**********************************************************************
*  @brief     �w�肳�ꂽ�t�@�C������DLL�����������������܂�
*  @param[in] void* handle : ���[�h����DLL�̃n���h��
*  @return    bool ����
*************************************************************************/
bool WindowsOS::FreeDLLHandle(void* handle)
{
	return !!::FreeLibrary((HMODULE)handle);
}

#pragma endregion Public Function