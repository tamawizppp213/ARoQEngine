//////////////////////////////////////////////////////////////////////////////////
///             @file   GUCommandLine.cpp
///             @brief  temp
///             @author toide
///             @date   2024/03/02 1:38:12
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/GUCommandLine.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu;

bool    CommandLine::_hasInitialized = false;
tstring CommandLine::_commandLine    = SP("");

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////

#pragma region Constructor and Destructor 

#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*                       BuildFromArgumentVector
****************************************************************************/
/* @brief     main�֐��Ŏg����argv, argc���g���ăR�}���h���C�����\�z���܂�.

*  @param[in] const int32   argumentCount  : �R�}���h���C�������̐�
*  @param[in] const wchar** argumentVector : �R�}���h���C���̕����z�� (argumentCount������) 
*
*  @return �@�@tstring
*****************************************************************************/
tstring CommandLine::BuildFromArgumentVector(const int32 argumentCount, const wchar** argumentVector)
{
	/*----------------------------------------------------------------------
	*  �ŏ��̎��s�t�@�C�����ɑ΂�������𖳎�����, �e������string�ɓn��
	*----------------------------------------------------------------------*/
	tstring result = SP("");
	for (int32 i = 1; i < argumentCount; ++i)
	{
		wstring argument(argumentVector[i]);

		// �󔒕������܂܂�ƃ_�u���N�I�[�e�[�V�������܂܂�Ă��Ȃ����argument�ɑ���
		if (argument.Contains(L" ", true) && !argument.Contains(L"\"", true))
		{
			const auto equalIndex = argument.Find(L"=");

			// equal�͍ŏ��Ɍ���邱�Ƃ͂Ȃ��̂�(��������������Ȃ�����)�A1�ȍ~��index�Œ���
			if (equalIndex > 0 && argument.Find(L" ") > equalIndex)
			{
				argument = argument.Left(equalIndex + 1) + L"\"" + argument.SubString(equalIndex + 1) + L"\"";
			}
			else
			{
				argument = wstring(L"\"") + argument + L"\"";
			}
		}

		// �R�}���h�����ɒǉ�����
		result.Reserve(result.Size() + argument.Size() + 1);
		for (int32 j = 0; j < argument.Size(); ++j)
		{
			result += static_cast<tchar>(argument[j]);
		}

		// �e�����̊Ԃɂ͕K���󔒂�ǉ�����
		if (i + 1 < argumentCount)
		{
			result += L" ";
		}
	}

	return result;
}

/****************************************************************************
*                       BuildFromArgumentVector
****************************************************************************/
/* @brief     main�֐��Ŏg����argv, argc���g���ăR�}���h���C�����\�z���܂�.

*  @param[in] const int32   argumentCount  : �R�}���h���C�������̐�
*  @param[in] const char** argumentVector : �R�}���h���C���̕����z�� (argumentCount������)
*
*  @return �@�@tstring
*****************************************************************************/
tstring CommandLine::BuildFromArgumentVector(const int32 argumentCount, const char** argumentVector)
{
	/*----------------------------------------------------------------------
	*  �ŏ��̎��s�t�@�C�����ɑ΂�������𖳎�����, �e������string�ɓn��
	*----------------------------------------------------------------------*/
	tstring result = SP("");
	for (int32 i = 1; i < argumentCount; ++i)
	{
		string argument(argumentVector[i]);

		// �󔒕������܂܂�ƃ_�u���N�I�[�e�[�V�������܂܂�Ă��Ȃ����argument�ɑ���
		if (argument.Contains(" ", true) && !argument.Contains("\"", true))
		{
			const auto equalIndex = argument.Find("=");

			// equal�͍ŏ��Ɍ���邱�Ƃ͂Ȃ��̂�(��������������Ȃ�����)�A1�ȍ~��index�Œ���
			if (equalIndex > 0 && argument.Find(" ") > equalIndex)
			{
				argument = argument.Left(equalIndex + 1) + "\"" + argument.SubString(equalIndex + 1) + "\"";
			}
			else
			{
				argument = string("\"") + argument + "\"";
			}
		}

		// �R�}���h�����ɒǉ�����
		result.Reserve(result.Size() + argument.Size() + 1);
		for (int32 j = 0; j < argument.Size(); ++j)
		{
			result += static_cast<tchar>(argument[j]);
		}

		// �e�����̊Ԃɂ͕K���󔒂�ǉ�����
		if (i + 1 < argumentCount)
		{
			result += SP(" ");
		}
	}

	return result;
}

void CommandLine::SetUp(const tchar* newCommandLine)
{
	if (_hasInitialized)
	{
		Reset(); 
	}

	_commandLine    = newCommandLine;
	_hasInitialized = true;
}
#pragma endregion Main Function