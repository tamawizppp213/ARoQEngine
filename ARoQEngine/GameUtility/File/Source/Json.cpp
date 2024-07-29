//////////////////////////////////////////////////////////////////////////////////
///  @file   Json.cpp
///  @brief  Jsonのシリアライズやデシリアライズを行うクラス
///  @author toide
///  @date   2024/06/29 17:57:17
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Include/Json.hpp"
#include "GameUtility/File/Private/Json/Include/JsonReader.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueObject.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueArray.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueBoolean.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueNumber.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueString.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueNull.hpp"
#include "GameUtility/Container/Include/GUStack.hpp"
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

#pragma endregion Public Function

#pragma region Private Function
///*!**********************************************************************
//*  @brief     シリアライズを行います.　シリアライズとはオブジェクトをJson形式の文字列に変換することです.
//*  @param[in] const gu::SharedPointer<JsonWriter>& jsonを書き込むクラス
//*  @param[in] const gu::SharedPointer<JsonObject>& シリアライズしたいオブジェクト
//*  @return    bool 成功したかどうか
//*************************************************************************/
//bool JsonSerializer::Serialize(const gu::SharedPointer<JsonWriter>& writer, const gu::SharedPointer<Element>& startElement)
//{
//	return false;
//}

/*!**********************************************************************
*  @brief      デシリアライズを行います.　デシリアライズとはJson形式の文字列をオブジェクトに変換することです.
*  @param[in]  const gu::SharedPointer<JsonReader>& jsonを読み込むクラス
*  @param[out] gu::SharedPointer<JsonObject> デシリアライズしたいオブジェクト
*  @param[in]  const ReadFlags flags = ReadFlags::None
*  @param[in]  const bool readAsync = false
*  @return     bool 成功したかどうか
*************************************************************************/
bool Json::Deserialize(const gu::tstring& filePath, gu::SharedPointer<JsonObject>& object, const ReadFlags flags, const bool readAsync)
{
	State stackState = {};
	if(!Deserialize(filePath, stackState, flags, readAsync))
	{
		return false;
	}

	if(stackState.Type != JsonValueType::Object)
	{
		return false;
	}

	object = stackState.Object;

	return true;

}

/*!**********************************************************************
*  @brief     デシリアライズを行います.　デシリアライズとはJson形式の文字列をオブジェクトに変換することです.
*  @param[in] const gu::SharedPointer<JsonReader>& jsonを読み込むクラス
*  @param[in] gu::DynamicArray<gu::SharedPointer<JsonValue>>& デシリアライズしたいオブジェクト
*  @param[in] const ReadFlags flags = ReadFlags::None
*  @param[in] const bool readAsync = false
*  @return    bool 成功したかどうか
*************************************************************************/
bool Json::Deserialize(const gu::tstring& filePath, gu::DynamicArray<gu::SharedPointer<JsonValue>>& outArray, const ReadFlags flags, const bool readAsync)
{
	State stackState = {};
	if(!Deserialize(filePath, stackState, flags, readAsync))
	{
		return false;
	}

	if(stackState.Type != JsonValueType::Array)
	{
		return false;
	}

	outArray = stackState.Array;

}

/*!**********************************************************************
*  @brief      デシリアライズを行います.　デシリアライズとはJson形式の文字列をオブジェクトに変換することです.
*  @param[in]  const gu::tstring& ファイルパス
*  @param[out] gu::SharedPointer<JsonValue>& Json解析するためのデータ構造
*  @param[in]  const  ReadFlags フラグ
*  @return     bool   成功したかどうか
*************************************************************************/
bool Json::Deserialize(const gu::tstring& filePath, gu::SharedPointer<JsonValue>& value, const ReadFlags flags, const bool readAsync)
{
	State stackState = {};
	if(!Deserialize(filePath, stackState, flags, readAsync))
	{
		return false;
	}

	switch(stackState.Type)
	{
		case JsonValueType::Object:
		{
			if(!stackState.Object)
			{
				return false;
			}
			value = MakeShared<JsonValueObject>(stackState.Object);
			break;
		}
		case JsonValueType::Array:
		{
			value = MakeShared<JsonValueArray>(stackState.Array);
			break;
		}
		default:
		{
			return false;
		}
	}

	return true;
}

/*!**********************************************************************
*  @brief     デシリアライズを行います.　デシリアライズとはJson形式の文字列をオブジェクトに変換することです.
*  @param[in] const  gu::tstring ファイルパス
*  @param[out]State& Json解析するためのデータ構造
*  @param[in] const  ReadFlags フラグ
*  @return    bool   成功したかどうか
*************************************************************************/
bool Json::Deserialize(const gu::tstring& filePath, State& outStackState, const ReadFlags flags, const bool useAsync)
{
	/*-------------------------------------------------------------------
	-          Stateの準備
	---------------------------------------------------------------------*/
	// Jsonを読み取る
	const auto reader = gu::MakeShared<JsonReader>(filePath, useAsync);

	// ObjectやArrayといった複数の要素を一時的に貯めておく
	Stack<SharedPointer<State>> stackState = {};

	// 現在の読み取り情報
	SharedPointer<State> currentState = nullptr;

	// 現在の読み取り値
	SharedPointer<JsonValue> newValue = nullptr;

	// 読み取っているNotation
	JsonNotation notation = JsonNotation::None;

	while (reader->ReadNext(notation))
	{
		tstring key = reader->GetKey();
		newValue.Reset();

		switch (notation)
		{
			case JsonNotation::ObjectStart:
			{
				// Objectの開始
				if (currentState)
				{
					// Objectの中にObjectがある場合
					stackState.Push(currentState);
				}

				// 新しいObjectを作成
				currentState         = gu::MakeShared<State>();
				currentState->Type   = JsonValueType::Object;
				currentState->Key    = key;
				currentState->Object = gu::MakeShared<JsonObject>();
				break;
			}
			case JsonNotation::ObjectEnd:
			{
				if (stackState.Size() > 0)
				{
					key          = currentState->Key;
					newValue     = MakeShared<JsonValueObject>(currentState->Object);
					currentState = stackState.Top();
					stackState.Pop();
				}
				break;
			}
			case JsonNotation::ArrayStart:
			{
				// Arrayの開始
				if (currentState)
				{
					stackState.Push(currentState);
				}

				currentState         = gu::MakeShared<State>();
				currentState->Type   = JsonValueType::Array;
				currentState->Key    = key;
				break;
			}
			case JsonNotation::ArrayEnd:
			{
				if (stackState.Size() > 0)
				{
					key          = currentState->Key;
					newValue     = MakeShared<JsonValueArray>(currentState->Array);
					currentState = stackState.Top();
					stackState.Pop();
				}
				break;
			}
			case JsonNotation::Boolean:
			{
				newValue = MakeShared<JsonValueBoolean>(reader->GetValueAsBoolean());
				break;
			}
			case JsonNotation::Number:
			{
				if (flags == ReadFlags::StoreNumberAsString)
				{
					newValue = MakeShared<JsonValueString>(reader->GetValueAsNumberString());
				}
				else
				{
					newValue = MakeShared<JsonValueNumber>(reader->GetValueAsNumber());
				}
				break;
			}
			case JsonNotation::String:
			{
				newValue = MakeShared<JsonValueString>(reader->GetValueAsString());
				break;
			}
			case JsonNotation::Null:
			{
				newValue = MakeShared<JsonValueNull>();
				break;
			}
			case JsonNotation::Error:
			{
				return false;
			}
		}

		if (newValue && currentState)
		{
			if (currentState->Type == JsonValueType::Object)
			{
				currentState->Object->AddMember(key, newValue);
			}
			else
			{
				currentState->Array.Push(newValue);
			}
		}
	}

	if (!currentState)
	{
		return false;
	}

	outStackState = *currentState;

	return true;
}
#pragma endregion Private Function