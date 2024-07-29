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
#include "GameUtility/Base/Include/GUCharacter.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu::file::json;
using namespace platform::core;
using namespace gu;

namespace
{
	static constexpr JsonNotation TOKEN_TO_NOTATION_TABLE[] = 
	{ 
		JsonNotation::Error, JsonNotation::Error, JsonNotation::ObjectStart, 
		JsonNotation::ObjectEnd, JsonNotation::ArrayStart, JsonNotation::ArrayEnd, 
		JsonNotation::Error, JsonNotation::String, JsonNotation::Number, JsonNotation::Boolean,JsonNotation::Boolean, JsonNotation::Null
	};
}

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
		notation = JsonNotation::Error;
		return false;
	}

	const bool readAtEnd = ReadAtEnd();
	if (!_hasReadRootObject && readAtEnd)
	{
		return true;
	}

	if (_hasReadRootObject && !readAtEnd)
	{
		return true;
	}

	if (readAtEnd)
	{
		return false;
	}


	/*-------------------------------------------------------------------
	-            ��̕�����̏ꍇ�͓ǂݍ��ݎ��s
	---------------------------------------------------------------------*/
	bool readWasSuccess = false;
	_key.Clear();

	/*-------------------------------------------------------------------
	-            Json������̓ǂݍ���
	---------------------------------------------------------------------*/
	do
	{
		JsonValueType currentState = JsonValueType::Null;

		if (_parseStates.Size() > 0)
		{
			currentState = _parseStates.Top();
		}

		switch (currentState)
		{
			case JsonValueType::Array:
			{
				readWasSuccess = ReadNextArrayValue(_token);
				break;
			}
			case JsonValueType::Object:
			{
				readWasSuccess = ReadNextObjectValue(_token);
				break;
			}
			default:
			{
				readWasSuccess = ReadStart(_token);
				break;
			}
		}

	} while (readWasSuccess && _token == JsonToken::None);

	/*-------------------------------------------------------------------
	-            �ǂݍ��ݏI���Ή�
	---------------------------------------------------------------------*/
	notation = TOKEN_TO_NOTATION_TABLE[static_cast<int32>(_token)];
	_hasReadRootObject = _parseStates.IsEmpty();

	if(!readWasSuccess || notation == JsonNotation::Error)
	{
		return true;
	}

	if(_hasReadRootObject && !ReadAtEnd())
	{
		readWasSuccess = ParseWhiteSpace();
	}

	return readWasSuccess;
}

#pragma endregion Public Function

#pragma region Public Property

#pragma endregion Public Property

#pragma region Protected Function
/*!**********************************************************************
*  @brief     JsonToken���擾���܂�
*  @param[out]JsonToken&
*  @return    bool
*************************************************************************/
bool JsonReader::ReadStart(JsonToken& token)
{
	if (!ParseWhiteSpace())
	{
		return false;
	}

	token = JsonToken::None;

	if(!ParseNextToken(token))
	{
		return false;
	}

	if ((token != JsonToken::CurlyOpen) && (token != JsonToken::SquareOpen))
	{
		return false;
	}

	return true;
}

/*!**********************************************************************
*  @brief     JsonObject���擾���܂�
*  @param[out]JsonToken&
*  @return    bool
*************************************************************************/
bool JsonReader::ReadNextObjectValue(JsonToken& token)
{
	const bool commaExpected = token != JsonToken::CurlyOpen;
	token = JsonToken::None;

	/*-------------------------------------------------------------------
	-        1��ڂ�NextToken��, �L�[�����擾���邽�߂Ɏg�p���܂�.
	---------------------------------------------------------------------*/
	if (!ParseNextToken(token))
	{
		return false;
	}

	if(token == JsonToken::CurlyClose)
	{
		return true;
	}
	else
	{
		if (commaExpected)
		{
			if(token != JsonToken::Comma)
			{
				return false;
			}

			token = JsonToken::None;

			if(!ParseNextToken(token))
			{
				return false;
			}
			
		}

		if (token != JsonToken::String)
		{
			return false;
		}

		_key  = _stringValue;
		token = JsonToken::None;

		/*-------------------------------------------------------------------
		-        2��ڂ�NextToken��, �R�������擾���邽�߂Ɏg�p���܂�.
		---------------------------------------------------------------------*/
		if (!ParseNextToken(token))
		{
			return false;
		}

		if (token != JsonToken::Colon)
		{
			return false;
		}

		/*-------------------------------------------------------------------
		-        3��ڂ�NextToken��, �o�����[�����擾���邽�߂Ɏg�p���܂�.
		---------------------------------------------------------------------*/
		token = JsonToken::None;

		if (!ParseNextToken(token))
		{
			return false;
		}
	}
	
	return true;
}

/*!**********************************************************************
*  @brief     JsonArray���擾���܂�
*  @param[out]JsonToken&
*  @return    bool
*************************************************************************/
bool JsonReader::ReadNextArrayValue(JsonToken& token)
{
	const bool commaExpected = token != JsonToken::SquareOpen;

	token = JsonToken::None;

	if (!ParseNextToken(token))
	{
		return false;
	}

	if(token == JsonToken::SquareClose)
	{
		return true;
	}
	else
	{
		if (commaExpected)
		{
			if (token != JsonToken::Comma)
			{
				return false;
			}

			token = JsonToken::None;

			if (!ParseNextToken(token))
			{
				return false;
			}
		}
	}

	return true;
}

/*!**********************************************************************
*  @brief      ���̃g�[�N����������܂ŕ������i�߂܂�.
*  @param[in]  const tstring& json�t�@�C���𕶎��񉻂�������
*  @param[out] JsonToken token : ���̃g�[�N��
*  @return     bool : �ǂݍ��݂ɐ����������ǂ���
*************************************************************************/
bool JsonReader::ParseNextToken(JsonToken& token)
{
	while (!ReadAtEnd())
	{
		// �������1�������i�߂�
		if (!TryToNextIndex())
		{
			return false;
		}

		const auto character = _rawData[_parseIndex];

		// �󔒕����̏ꍇ�̓X�L�b�v
		if (Character::IsWhiteSpace(character))
		{
			continue;
		}

		// ���l�^�̓ǂݍ���
		if (IsNumber(character))
		{
			if (!ParseNumberToken())
			{
				return false;
			}

			token = JsonToken::Number;
			return true;
		}

		// �g�[�N���̎�ނ𔻒�
		switch (character)
		{
			case SP('{'):
			{
				token = JsonToken::CurlyOpen;
				_parseStates.Push(JsonValueType::Object);
				return true;
			}
			case SP('}'):
			{
				token = JsonToken::CurlyClose;
				if (!_parseStates.IsEmpty())
				{
					_parseStates.Pop();
					return true;
				}
				else
				{
					return false;
				}
			}
			case SP('['):
			{
				token = JsonToken::SquareOpen;
				_parseStates.Push(JsonValueType::Array);
				return true;
			}
			case SP(']'):
			{
				token = JsonToken::SquareClose;

				if (!_parseStates.IsEmpty())
				{
					_parseStates.Pop();
					return true;
				}
				else
				{
					return false;
				}
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
				if (!ParseStringToken())
				{
					return false;
				}
				token = JsonToken::String;
				return true;
			}
			case SP('t'):
			case SP('T'):
			case SP('f'):
			case SP('F'):
			case SP('n'):
			case SP('N'):
			{
				tstring value = character;

				while (_parseIndex < _rawData.Size())
				{
					++_parseIndex;

					if (Character::IsAlphabet(character))
					{
						value += character;
					}
					else
					{
						--_parseIndex;
						break;
					}
				}

				if(value == SP("true") || value == SP("True") || value == SP("TRUE"))
				{
					token = JsonToken::True;
					_boolValue = true;
					return true;
				}
				else if (value == SP("false") || value == SP("False") || value == SP("FALSE"))
				{
					token = JsonToken::False;
					_boolValue = false;
					return true;
				}
				else if (value == SP("null") || value == SP("Null") || value == SP("NULL"))
				{
					token = JsonToken::Null;
					return true;
				}
				else
				{
					return false;
				}

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
bool JsonReader::ParseNumberToken()
{
	/*-------------------------------------------------------------------
	-            �����񐔂̎擾����
	---------------------------------------------------------------------*/
	if (_parseIndex >= _rawData.Size())
	{
		return false;
	}

	tstring result       = SP("");
	result += _rawData[_parseIndex];
	int32   currentState = 0;
	bool    readStateError      = false;
	/*-------------------------------------------------------------------
	-            ������̎擾
	---------------------------------------------------------------------*/
	while (!ReadAtEnd())
	{
		// �������1�������i�߂�
		if (!TryToNextIndex())
		{
			return false;
		}

		const auto character = _rawData[_parseIndex];

		// �����ȊO�ł���ΏI��
		if (!IsNumber(character))
		{
			_parseIndex--;
			break;
		}

		switch (currentState)
		{
			case 0:
			{
				if (character == SP('-'))
				{
					currentState = 1;
				}
				else if (character == SP('0'))
				{
					currentState = 2;
				}
				else if(Character::IsNonZeroDigit(character))
				{
					currentState = 3;
				}
				else
				{
					readStateError = false;
				}
				break;
			}
			case 1:
			{
				if (character == SP('0'))
				{
					currentState = 2;
				}
				else if (Character::IsNonZeroDigit(character))
				{
					currentState = 3;
				}
				else
				{
					readStateError = true;
				}
				break;
			}
			case 2:
			{
				if (character == SP('.'))
				{
					currentState = 4;
				}
				else if (character == SP('e') || character == SP('E'))
				{
					currentState = 5;
				}
				else
				{
					currentState = 3;
				}
				break;
			}
			case 3:
			{
				if (Character::IsDigit(character))
				{
					currentState = 3;
				}
				else if (character == SP('.'))
				{
					currentState = 4;
				}
				else if (character == SP('e') || character == SP('E'))
				{
					currentState = 5;
				}
				else
				{
					readStateError = true;
				}
				break;
			}
			case 4:
			{
				if (Character::IsDigit(character))
				{
					currentState = 6;
				}
				else
				{
					readStateError = true;
				}
				break;
			}
			case 5:
			{
				if (character == SP('-') || character == SP('+'))
				{
					currentState = 7;
				}
				else if (Character::IsDigit(character))
				{
					currentState = 8;
				}
				else
				{
					readStateError = true;
				}
				break;
			}
			case 6:
			{
				if (Character::IsDigit(character))
				{
					currentState = 6;
				}
				else if (character == SP('e') || character == SP('E'))
				{
					currentState = 5;
				}
				else
				{
					readStateError = true;
				}
				break;
			}
			case 7:
			{
				if (Character::IsDigit(character))
				{
					currentState = 8;
				}
				else
				{
					readStateError = true;
				}
				break;
			}
			case 8:
			{
				if (Character::IsDigit(character))
				{
					currentState = 8;
				}
				else
				{
					readStateError = true;
				}
				break;
			}
			default:
			{
				return false;
			}
		}

		if (readStateError)
		{
			break;
		}

		result += character;
	}

	if(!readStateError && ((currentState == 2) || (currentState == 3) || (currentState == 6) || currentState == 8))
	{
		_stringValue = result;
		_numberValue = result.ToDouble();
		return true;
	}

	return false;
}

/*!**********************************************************************
*  @brief      ������^��ǂݎ��܂�.
*  @param[in]  const tchar ������
*  @return     bool : �ǂݍ��݂ɐ����������ǂ���
*************************************************************************/
bool JsonReader::ParseStringToken()
{
	_stringValue.Clear();

	while (!ReadAtEnd())
	{
		if (!TryToNextIndex())
		{
			return false;
		}

		/*-------------------------------------------------------------------
		-        ������"�ł����, ������̏I�[�Ƃ݂Ȃ��ďI�� (���O�ɕ�����̐擪��ParseNextToken�Ŏ擾�ς݂ł�.)
		---------------------------------------------------------------------*/
		if (_rawData[_parseIndex] == SP('\"'))
		{
			break;
		}

		/*-------------------------------------------------------------------
		-       �G�X�P�[�v�������܂܂�Ă���ꍇ, ���̓��e�����Ƃɏ��������肵�܂�.
		---------------------------------------------------------------------*/
		if(_rawData[_parseIndex] == SP('\\'))
		{
			if (!TryToNextIndex())
			{
				return false;
			}

			switch (_rawData[_parseIndex])
			{
				case SP('\"'):
				{
					_stringValue += SP('\"');
					break;
				}
				case SP('\\'):
				{
					_stringValue += SP('\\');
					break;
				}
				case SP('/'):
				{
					_stringValue += SP('/');
					break;
				}
				case SP('b'):
				{
					_stringValue += SP('\b');
					break;
				}
				case SP('f'):
				{
					_stringValue += SP('\f');
					break;
				}
				case SP('n'):
				{
					_stringValue += SP('\n');
					break;
				}
				case SP('r'):
				{
					_stringValue += SP('\r');
					break;
				}
				case SP('t'):
				{
					_stringValue += SP('\t');
					break;
				}
				// hexadecimals�͖��Ή�
				default:
				{
					return false;
				}
			}
		}
		else
		{
			_stringValue += _rawData[_parseIndex];
		}
	}
	return true;
}

/*!**********************************************************************
*  @brief      �󔒕����񂪑����܂Ői�݂܂�
*  @param[in]  const tchar ������
*  @return     bool : �ǂݍ��݂ɐ����������ǂ���
*************************************************************************/
bool JsonReader::ParseWhiteSpace()
{
	while (!ReadAtEnd())
	{
		if (!TryToNextIndex())
		{
			return false;
		}

		if(!Character::IsWhiteSpace(_rawData[_parseIndex]))
		{
			--_parseIndex;
			break;
		}
	}

	return true;
}
#pragma endregion Protected Function