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
*  @brief     Jsonファイルを最後まで読み込みます.
*  @param[in] const gu::tstring& filePath : 読み込みたいJsonファイルのパス
*  @param[in] const bool 非同期読み込みを行うか
*  @return    bool : 読み込みに成功したかどうか
*************************************************************************/
bool JsonReader::Initialize(const gu::tstring& filePath, const bool useAsync)
{
	/*-------------------------------------------------------------------
	-            ファイルハンドルを取得
	---------------------------------------------------------------------*/
	const auto fileHandle = IFileSystem::Get()->OpenRead(filePath.CString(), true, useAsync);

	if (!fileHandle)
	{
		return false;
	}

	/*-------------------------------------------------------------------
	-            ファイルの全文字列を取得する
	---------------------------------------------------------------------*/
	const auto fileSize   = fileHandle->Size();

	{
		DynamicArray<char8> byteArray(fileSize, true);
		
		fileHandle->Read(byteArray.Data(), sizeof(char) * fileSize);

		// 文字列メモリ負荷対策
		u8string utf8String(byteArray.Data(), fileSize);

		_rawData = StringConverter::ConvertUTF8ToTString(utf8String);
	}

	return true;

}

/*!**********************************************************************
*  @brief     次の値を読み込みます
*  @param[out]JsonNotation&
*  @return    bool : 読み込みに成功したかどうか
*************************************************************************/
bool JsonReader::ReadNext(JsonNotation& notation)
{
	/*-------------------------------------------------------------------
	-            空の文字列の場合は読み込み失敗
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
	-            空の文字列の場合は読み込み失敗
	---------------------------------------------------------------------*/
	bool readWasSuccess = false;
	_key.Clear();

	/*-------------------------------------------------------------------
	-            Json文字列の読み込み
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
	-            読み込み終了対応
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
*  @brief     JsonTokenを取得します
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
*  @brief     JsonObjectを取得します
*  @param[out]JsonToken&
*  @return    bool
*************************************************************************/
bool JsonReader::ReadNextObjectValue(JsonToken& token)
{
	const bool commaExpected = token != JsonToken::CurlyOpen;
	token = JsonToken::None;

	/*-------------------------------------------------------------------
	-        1回目のNextTokenは, キー名を取得するために使用します.
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
		-        2回目のNextTokenは, コロンを取得するために使用します.
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
		-        3回目のNextTokenは, バリュー名を取得するために使用します.
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
*  @brief     JsonArrayを取得します
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
*  @brief      次のトークンが見つかるまで文字列を進めます.
*  @param[in]  const tstring& jsonファイルを文字列化したもの
*  @param[out] JsonToken token : 次のトークン
*  @return     bool : 読み込みに成功したかどうか
*************************************************************************/
bool JsonReader::ParseNextToken(JsonToken& token)
{
	while (!ReadAtEnd())
	{
		// 文字列を1文字分進める
		if (!TryToNextIndex())
		{
			return false;
		}

		const auto character = _rawData[_parseIndex];

		// 空白文字の場合はスキップ
		if (Character::IsWhiteSpace(character))
		{
			continue;
		}

		// 数値型の読み込み
		if (IsNumber(character))
		{
			if (!ParseNumberToken())
			{
				return false;
			}

			token = JsonToken::Number;
			return true;
		}

		// トークンの種類を判定
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
*  @brief      数値型を読み取ります.
*  @param[in]  const tchar 文字列
*  @return     bool : 読み込みに成功したかどうか
*************************************************************************/
bool JsonReader::ParseNumberToken()
{
	/*-------------------------------------------------------------------
	-            文字列数の取得準備
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
	-            文字列の取得
	---------------------------------------------------------------------*/
	while (!ReadAtEnd())
	{
		// 文字列を1文字分進める
		if (!TryToNextIndex())
		{
			return false;
		}

		const auto character = _rawData[_parseIndex];

		// 数字以外であれば終了
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
*  @brief      文字列型を読み取ります.
*  @param[in]  const tchar 文字列
*  @return     bool : 読み込みに成功したかどうか
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
		-        文字が"であれば, 文字列の終端とみなして終了 (事前に文字列の先頭はParseNextTokenで取得済みです.)
		---------------------------------------------------------------------*/
		if (_rawData[_parseIndex] == SP('\"'))
		{
			break;
		}

		/*-------------------------------------------------------------------
		-       エスケープ文字が含まれている場合, 次の内容をもとに処理を決定します.
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
				// hexadecimalsは未対応
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
*  @brief      空白文字列が続くまで進みます
*  @param[in]  const tchar 文字列
*  @return     bool : 読み込みに成功したかどうか
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