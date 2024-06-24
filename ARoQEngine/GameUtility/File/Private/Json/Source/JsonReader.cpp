//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonReader.cpp
///  @brief  temp
///  @author toide
///  @date   2024/06/15 1:19:49
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Private/Json/Include/JsonReader.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "Platform/Core/Include/CoreFileSystem.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Base/Include/GUStringConverter.hpp"
#include "GameUtility/Base/Include/GUCharacterCodeConverter.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu::file::json;
using namespace platform::core;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     Json�t�@�C�����Ō�܂œǂݍ��݂܂�.
*  @param[in] const gu::tstring& filePath : �ǂݍ��݂���Json�t�@�C���̃p�X
*  @param[in] const bool �񓯊��ǂݍ��݂��s����
*  @return    bool : �ǂݍ��݂ɐ����������ǂ���
*************************************************************************/
bool JsonReader::Read(const gu::tstring& filePath, const bool useAsync)
{
	/*-------------------------------------------------------------------
	-            �t�@�C���n���h�����擾
	---------------------------------------------------------------------*/
	const auto fileHandle = IFileSystem::Get()->OpenRead(filePath.CString(), true, useAsync);

	if (!fileHandle)
	{
		return false;
	}

	/*-------------------------------------------------------------------
	-            �t�@�C���̑S��������擾����
	---------------------------------------------------------------------*/
	tstring    fileString = SP("");
	const auto fileSize   = fileHandle->Size();

	{
		DynamicArray<char8> byteArray(fileSize, true);
		
		fileHandle->Read(byteArray.Data(), sizeof(char) * fileSize);

		// �����񃁃������ב΍�
		u8string utf8String(byteArray.Data(), fileSize);

		fileString = StringConverter::ConvertUTF8ToTString(utf8String);
	}

	

	return true;

}
#pragma endregion Public Function

#pragma region Public Property

#pragma endregion Public Property

#pragma region Protected Function
/*!**********************************************************************
*  @brief      ���̃g�[�N����������܂ŕ������i�߂܂�.
*  @param[in]  const tstring& json�t�@�C���𕶎��񉻂�������
*  @param[out] JsonToken token : ���̃g�[�N��
*  @return     bool : �ǂݍ��݂ɐ����������ǂ���
*************************************************************************/
bool JsonReader::ParseNextToken(const tstring& json, JsonToken& token)
{
	for (uint64 i = _parseIndex; i < json.Size(); ++i)
	{
		// �������1�������i�߂�
		++_parseIndex;

		// �I�[�����̏ꍇ�͏I��
		if (json[i] == SP('\0'))
		{
			break;
		}

		// ���s�����̏ꍇ�͕����������Z�b�g
		if(json[i] == SP('\n'))
		{
			++_lineCount;
			_characterCount = 0;
		}

		// �󔒕����̏ꍇ�̓X�L�b�v
		if (IsWhiteSpace(json[i]))
		{
			continue;
		}

		// �g�[�N���̎�ނ𔻒�
		switch (json[i])
		{
			case SP('{'):
			{
				token = JsonToken::CurlyOpen;
				return true;
			}
			case SP('}'):
			{
				token = JsonToken::CurlyClose;
				return true;
			}
			case SP('['):
			{
				token = JsonToken::SquareOpen;
				return true;
			}
			case SP(']'):
			{
				token = JsonToken::SquareClose;
				return true;
			}
			case SP(':'):
			{
				token = JsonToken::Colon;
				return true;
			}
			case SP(','):
			{
				token = JsonToken::Comma;
				return true;
			}
			case SP('\"'):
			{
				token = JsonToken::String;
				return true;
			}
			case SP('t'):
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
	}

	return true;
}
#pragma endregion Protected Function