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
	for (int32 i = 0; i < 2 && !result; ++i)
	{
		HKEY key = NULL;
		const uint32 registryFlags = i == 0 ? KEY_WOW64_32KEY : KEY_WOW64_64KEY;

		// �L�[�����݂��邩�ǂ������m�F���܂�
		if (RegOpenKeyEx(hRootKey, subKey.CString(), NULL, KEY_READ | registryFlags, &key) != ERROR_SUCCESS)
		{
			continue;
		}

		// �K�v�ȕ�����o�b�t�@�̑傫�����擾���܂�.
		::DWORD bufferSize = 0;
		::DWORD valueType  = 0;
		if (RegQueryValueEx(key, valueName.CString(), NULL, &valueType, NULL, &bufferSize) != ERROR_SUCCESS || bufferSize == 0)
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
*  @brief     Visual studio��CommonTools�̃p�X���擾���܂� (�擾�ł��Ȃ��ꍇ�͋�̕������Ԃ��܂�.)
*  @param[in] const gu::uint32 Visual Studio�̃��W���[�o�[�W���� (VS2022�̏ꍇ��17)
*  @return    tstring CommonTools�̃p�X
*************************************************************************/
gu::tstring WindowsOS::GetVisualStudioCommonToolsPath(const gu::uint32 majorVersion)
{
	/*----------------------------------------------------------------------
	*      Visual studio��2013�ȍ~�̃o�[�W��������g�p�\�ł�
	*----------------------------------------------------------------------*/
	return tstring();
}

#pragma endregion Public Function