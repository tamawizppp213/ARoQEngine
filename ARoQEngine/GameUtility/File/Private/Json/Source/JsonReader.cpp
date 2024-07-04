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
bool JsonReader::Initialize(const gu::tstring& filePath, const bool useAsync)
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
	const auto fileSize   = fileHandle->Size();

	{
		DynamicArray<char8> byteArray(fileSize, true);
		
		fileHandle->Read(byteArray.Data(), sizeof(char) * fileSize);

		// �����񃁃������ב΍�
		u8string utf8String(byteArray.Data(), fileSize);

		_rawData = StringConverter::ConvertUTF8ToTString(utf8String);
	}

	return true;

}

/*!**********************************************************************
*  @brief     ���̒l��ǂݍ��݂܂�
*  @param[out]JsonNotation&
*  @return    bool : �ǂݍ��݂ɐ����������ǂ���
*************************************************************************/
bool JsonReader::ReadNext(JsonNotation& notation)
{
	/*-------------------------------------------------------------------
	-            ��̕�����̏ꍇ�͓ǂݍ��ݎ��s
	---------------------------------------------------------------------*/
	if(_rawData.IsEmpty())
	{
		return false;
	}

	/*-------------------------------------------------------------------
	-            ��̕�����̏ꍇ�͓ǂݍ��ݎ��s
	---------------------------------------------------------------------*/
	bool readWasSuccess = false;
	_key.Clear();

	do
	{
		JsonValueType currentState = JsonValueType::Null;



	} while (readWasSuccess && _token == JsonToken::None);
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
	while (true)
	{
		// �������1�������i�߂�
		++_parseIndex;

		// �I�[�����̏ꍇ�͏I��
		if (json[_parseIndex] == SP('\0'))
		{
			break;
		}

		// ���s�����̏ꍇ�͕����������Z�b�g
		if(json[_parseIndex] == SP('\n'))
		{
			
		}

		// �󔒕����̏ꍇ�̓X�L�b�v
		if (IsWhiteSpace(json[_parseIndex]))
		{
			continue;
		}

		// ���l�^�̓ǂݍ���
		if (!IsNumber(json[_parseIndex]))
		{

		}

		// �g�[�N���̎�ނ𔻒�
		switch (json[_parseIndex])
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

/*!**********************************************************************
*  @brief      ���l�^��ǂݎ��܂�.
*  @param[in]  const tchar ������
*  @return     bool : �ǂݍ��݂ɐ����������ǂ���
*************************************************************************/
bool JsonReader::ParseNumberToken(const tstring& json)
{
	/*-------------------------------------------------------------------
	-            �����񐔂̎擾
	---------------------------------------------------------------------*/
	const tchar* startIndex = &json[_parseIndex];
	uint64 characterCount = 0;

	while (true)
	{
		// �I�[�����̏ꍇ�͏I��
		if (json[_parseIndex] == SP('\0'))
		{
			break;
		}

		if (!IsNumber(json[_parseIndex]))
		{
			++_parseIndex;
			break;
		}

		// �������1�������i�߂�
		++characterCount;
		++_parseIndex;
	}

	/*-------------------------------------------------------------------
	-            ���l�f�[�^�̓ǂݍ���
	---------------------------------------------------------------------*/
	tstring numberString(startIndex, characterCount);


	return true;
}
#pragma endregion Protected Function