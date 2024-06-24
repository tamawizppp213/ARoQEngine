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
bool JsonReader::Read(const gu::tstring& filePath, const bool useAsync)
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
	tstring    fileString = SP("");
	const auto fileSize   = fileHandle->Size();

	{
		DynamicArray<char8> byteArray(fileSize, true);
		
		fileHandle->Read(byteArray.Data(), sizeof(char) * fileSize);

		// 文字列メモリ負荷対策
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
*  @brief      次のトークンが見つかるまで文字列を進めます.
*  @param[in]  const tstring& jsonファイルを文字列化したもの
*  @param[out] JsonToken token : 次のトークン
*  @return     bool : 読み込みに成功したかどうか
*************************************************************************/
bool JsonReader::ParseNextToken(const tstring& json, JsonToken& token)
{
	for (uint64 i = _parseIndex; i < json.Size(); ++i)
	{
		// 文字列を1文字分進める
		++_parseIndex;

		// 終端文字の場合は終了
		if (json[i] == SP('\0'))
		{
			break;
		}

		// 改行文字の場合は文字数をリセット
		if(json[i] == SP('\n'))
		{
			++_lineCount;
			_characterCount = 0;
		}

		// 空白文字の場合はスキップ
		if (IsWhiteSpace(json[i]))
		{
			continue;
		}

		// トークンの種類を判定
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