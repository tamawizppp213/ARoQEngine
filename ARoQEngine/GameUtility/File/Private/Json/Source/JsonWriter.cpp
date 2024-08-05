//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonWriter.cpp
///  @brief  temp
///  @author toide
///  @date   2024/07/31 1:36:05
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Private/Json/Include/JsonWriter.hpp"
#include "GameUtility/File/Private/Json/Include/JsonWritePolicy.hpp"
#include "Platform/Core/Include/CoreFileSystem.hpp"
#include "GameUtility/Base/Include/GUStringConverter.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu::file::json;
using namespace gu;
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
* @brief     貯めた文字列データをファイルに流し込みます.
* @param[in] const gu::tstring filePath
* @param[in] const bool useAsync
* @return    bool
*************************************************************************/
bool JsonWriter::Flush(const gu::tstring& filePath, const bool useAsync)
{
	const auto fileHandle = platform::core::IFileSystem::Get()->OpenWrite(filePath, false, false, useAsync);
	
	if (!fileHandle)
	{
		return false;
	}

	/*-------------------------------------------------------------------
	-            ファイルの全文字列を書き込む
	---------------------------------------------------------------------*/
	{
		const auto utf8 = StringConverter::ConvertTStringToUTF8(_data);
		
		if (!fileHandle->Write(utf8.CString(), sizeof(char8) * utf8.Size()))
		{
			return false;
		}

		_data.Clear();

		return true;
	}
}

/*!**********************************************************************
*  @brief     JsonObjectの書き込みを開始します
*  @param[in] void
*  @return    void
*************************************************************************/
void JsonWriter::WriteObjectStart()
{
	Check(EnableWriteWithoutKey());

	if (_previousToken != JsonToken::None)
	{
		WriteCommaIfNeeded();
	}

	if (_previousToken != JsonToken::None)
	{
		JsonWritePolicy::WriteLineTerminator(_data);
		JsonWritePolicy::WriteTab(_data, _indentLevel);
	}

	JsonWritePolicy::WriteChar(_data, '{');
	++_indentLevel;
	_stackStates.Push(JsonValueType::Object);
	_previousToken = JsonToken::CurlyOpen;
}

/*!**********************************************************************
*  @brief     JsonObjectの書き込みを開始します
*  @param[in] const gu::tstring& key : キー名
*  @return    void
*************************************************************************/
void JsonWriter::WriteObjectStart(const gu::tstring& key)
{
	Check(_stackStates.Top() == JsonValueType::Object);

	WriteKey(key);

	JsonWritePolicy::WriteLineTerminator(_data);
	JsonWritePolicy::WriteTab(_data, _indentLevel);
	JsonWritePolicy::WriteChar(_data, '{');
	++_indentLevel;
	_stackStates.Push(JsonValueType::Object);
	_previousToken = JsonToken::CurlyOpen;
}

/*!**********************************************************************
*  @brief     JsonObjectの書き込みを終了します.
*  @param[in] void
*  @return    void
*************************************************************************/
void JsonWriter::WriteObjectEnd()
{
	Check(_stackStates.Top() == JsonValueType::Object);

	JsonWritePolicy::WriteLineTerminator(_data);

	--_indentLevel;
	JsonWritePolicy::WriteTab(_data, _indentLevel);
	JsonWritePolicy::WriteChar(_data, '}');
	_stackStates.Pop();
	_previousToken = JsonToken::CurlyClose;
}

/*!**********************************************************************
*  @brief     JsonArrayの書き込みを開始します.
*  @param[in] void
*  @return    void
*************************************************************************/
void JsonWriter::WriteArrayStart()
{
	Check(EnableWriteWithoutKey());

	if (_previousToken != JsonToken::None)
	{
		WriteCommaIfNeeded();
	}

	if (_previousToken != JsonToken::None)
	{
		JsonWritePolicy::WriteLineTerminator(_data);
		JsonWritePolicy::WriteTab(_data, _indentLevel);
	}

	JsonWritePolicy::WriteChar(_data, '[');
	++_indentLevel;
	_stackStates.Push(JsonValueType::Array);
	_previousToken = JsonToken::SquareOpen;
}

/*!**********************************************************************
*  @brief     JsonArrayの書き込みを開始します.
*  @param[in] void
*  @return    void
*************************************************************************/
void JsonWriter::WriteArrayStart(const gu::tstring& key)
{
	Check(_stackStates.Top() == JsonValueType::Object);

	WriteKey(key);

	JsonWritePolicy::WriteSpace(_data);
	JsonWritePolicy::WriteChar(_data, '[');
	++_indentLevel;
	_stackStates.Push(JsonValueType::Array);
	_previousToken = JsonToken::SquareOpen;
}

/*!**********************************************************************
*  @brief     JsonArrayの書き込みを終了します.
*  @param[in] void
*  @return    void
*************************************************************************/
void JsonWriter::WriteArrayEnd()
{
	Check(_stackStates.Top() == JsonValueType::Array);

	JsonWritePolicy::WriteLineTerminator(_data);

	--_indentLevel;
	
	if (_previousToken == JsonToken::SquareClose || _previousToken == JsonToken::CurlyClose || _previousToken == JsonToken::String)
	{
		JsonWritePolicy::WriteLineTerminator(_data);
		JsonWritePolicy::WriteTab(_data, _indentLevel);
	}
	else if (_previousToken != JsonToken::SquareOpen)
	{
		JsonWritePolicy::WriteSpace(_data);
	}

	JsonWritePolicy::WriteChar(_data, ']');
	_stackStates.Pop();
	_previousToken = JsonToken::SquareClose;
}

/*!**********************************************************************
*  @brief     JsonArrayの書き込みを終了します.
*  @param[in] const gu::tstring& key : キー名
*  @return    void
*************************************************************************/
void JsonWriter::WriteValue(const gu::tstring& key, const gu::tstring& value)
{
	if (!key.IsEmpty())
	{
		WriteKey(key);
	}

	JsonWritePolicy::WriteSpace(_data);
	JsonWritePolicy::WriteChar(_data, '"');
	WriteEscapedString(value);
	JsonWritePolicy::WriteChar(_data, '"');
	_previousToken = JsonToken::String;
}

/*!**********************************************************************
*  @brief     文字列データを書き込みます
*  @param[in] const gu::tstring& key : キー名
*  @param[in] const bool value : バリュー名
*  @return    void
*************************************************************************/
void JsonWriter::WriteValue(const gu::tstring& key, const bool value)
{
	Check(_stackStates.Top() == JsonValueType::Object);

	if (!key.IsEmpty())
	{
		WriteKey(key);
	}

	JsonWritePolicy::WriteSpace(_data);
	JsonWritePolicy::WriteString(_data, value ? SP("true") : SP("false"));
	_previousToken = value ? JsonToken::True : JsonToken::False;
}

/*!**********************************************************************
*  @brief     文字列データを書き込みます
*  @param[in] const gu::tstring& key : キー名
*  @param[in] const float value : バリュー名
*  @return    void
*************************************************************************/
void JsonWriter::WriteValue(const gu::tstring& key, const float value)
{
	WriteNumberInternal(key, gu::tstring::FromNumber(value));
}

/*!**********************************************************************
*  @brief     文字列データを書き込みます
*  @param[in] const gu::tstring& key : キー名
*  @param[in] const double value : バリュー名
*  @return    void
*************************************************************************/
void JsonWriter::WriteValue(const gu::tstring& key, const double value)
{
	WriteNumberInternal(key, gu::tstring::FromNumber(value));
}

/*!**********************************************************************
*  @brief     文字列データを書き込みます
*  @param[in] const gu::tstring& key : キー名
*  @param[in] const bool value : バリュー名
*  @return    void
*************************************************************************/
void JsonWriter::WriteValue(const gu::tstring& key, const gu::int32 value)
{
	WriteNumberInternal(key, gu::tstring::FromNumber(value));
}


/*!**********************************************************************
*  @brief     文字列データを書き込みます
*  @param[in] const gu::tstring& key : キー名
*  @param[in] const gu::tstring& value : 値
*  @return    void
*************************************************************************/
void JsonWriter::WriteNumberInternal(const gu::tstring& key, const gu::tstring& number)
{
	Check(_stackStates.Top() == JsonValueType::Object);

	if (!key.IsEmpty())
	{
		WriteKey(key);
	}

	JsonWritePolicy::WriteSpace(_data);
	JsonWritePolicy::WriteString(_data, number);
	_previousToken = JsonToken::Number;

}

#pragma endregion Public Function

#pragma region Protected Function
/*!**********************************************************************
*  @brief     エスケープ文字があった場合の文字列書き込み
*  @param[in] const gu::tstring& 文字列
*  @return    void
*************************************************************************/
void JsonWriter::WriteEscapedString(const gu::tstring& value)
{
	for (gu::uint64 i = 0; i < value.Size(); ++i)
	{
		switch (value[i])
		{
		case '"':
			JsonWritePolicy::WriteString(_data, SP("\\\""));
			break;
		case '\\':
			JsonWritePolicy::WriteString(_data, SP("\\\\"));
			break;
		case '\b':
			JsonWritePolicy::WriteString(_data, SP("\\b"));
			break;
		case '\f':
			JsonWritePolicy::WriteString(_data, SP("\\f"));
			break;
		case '\n':
			JsonWritePolicy::WriteString(_data, SP("\\n"));
			break;
		case '\r':
			JsonWritePolicy::WriteString(_data, SP("\\r"));
			break;
		case '\t':
			JsonWritePolicy::WriteString(_data, SP("\\t"));
			break;
		default:
			JsonWritePolicy::WriteChar(_data, value[i]);
			break;
		}
	}
}

/*!**********************************************************************
*  @brief     キー名の書き込みを行います.
*  @param[in] void
*  @return    void
*************************************************************************/
void JsonWriter::WriteKey(const gu::tstring& key)
{
	WriteCommaIfNeeded();
	JsonWritePolicy::WriteLineTerminator(_data);
	JsonWritePolicy::WriteTab(_data, _indentLevel);
	JsonWritePolicy::WriteChar(_data, '"');
	JsonWritePolicy::WriteString(_data, key);
	JsonWritePolicy::WriteString(_data, SP("\":"));
	JsonWritePolicy::WriteSpace(_data);
}

void JsonWriter::WriteCommaIfNeeded()
{
	if (_previousToken != JsonToken::CurlyOpen && _previousToken != JsonToken::SquareOpen && _previousToken != JsonToken::Key)
	{
		JsonWritePolicy::WriteChar(_data, ',');
	}
}
#pragma endregion