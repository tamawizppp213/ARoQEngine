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
		return false;
	}

	/*-------------------------------------------------------------------
	-            空の文字列の場合は読み込み失敗
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
*  @brief      次のトークンが見つかるまで文字列を進めます.
*  @param[in]  const tstring& jsonファイルを文字列化したもの
*  @param[out] JsonToken token : 次のトークン
*  @return     bool : 読み込みに成功したかどうか
*************************************************************************/
bool JsonReader::ParseNextToken(const tstring& json, JsonToken& token)
{
	while (true)
	{
		// 文字列を1文字分進める
		++_parseIndex;

		// 終端文字の場合は終了
		if (json[_parseIndex] == SP('\0'))
		{
			break;
		}

		// 改行文字の場合は文字数をリセット
		if(json[_parseIndex] == SP('\n'))
		{
			
		}

		// 空白文字の場合はスキップ
		if (IsWhiteSpace(json[_parseIndex]))
		{
			continue;
		}

		// 数値型の読み込み
		if (!IsNumber(json[_parseIndex]))
		{

		}

		// トークンの種類を判定
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
*  @brief      数値型を読み取ります.
*  @param[in]  const tchar 文字列
*  @return     bool : 読み込みに成功したかどうか
*************************************************************************/
bool JsonReader::ParseNumberToken(const tstring& json)
{
	/*-------------------------------------------------------------------
	-            文字列数の取得
	---------------------------------------------------------------------*/
	const tchar* startIndex = &json[_parseIndex];
	uint64 characterCount = 0;

	while (true)
	{
		// 終端文字の場合は終了
		if (json[_parseIndex] == SP('\0'))
		{
			break;
		}

		if (!IsNumber(json[_parseIndex]))
		{
			++_parseIndex;
			break;
		}

		// 文字列を1文字分進める
		++characterCount;
		++_parseIndex;
	}

	/*-------------------------------------------------------------------
	-            数値データの読み込み
	---------------------------------------------------------------------*/
	tstring numberString(startIndex, characterCount);


	return true;
}
#pragma endregion Protected Function