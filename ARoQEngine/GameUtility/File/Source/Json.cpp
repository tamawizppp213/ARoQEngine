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
#include "GameUtility/File/Private/Json/Include/JsonWriter.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueObject.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueArray.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueBoolean.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueNumber.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueNumberString.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueString.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueNull.hpp"
#include "GameUtility/Container/Include/GUStack.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu::file;
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
/*!**********************************************************************
*  @brief     Jsonファイルを読み込みます.
*  @param[in] const gu::tstring& ファイルパス
*  @param[in] const bool 非同期で読み込みを行うか
*  @return    bool 成功したかどうか
*************************************************************************/
bool Json::Load(const gu::tstring& filePath, const bool useAsync)
{
	_reader = gu::MakeShared<JsonReader>(filePath, useAsync);
	return _reader->Load(filePath, useAsync);
}

/*!**********************************************************************
*  @brief     Jsonファイルを読み込みます.
*  @param[in] const gu::tstring& ファイルパス
*  @param[in] const bool 非同期で読み込みを行うか
*  @return    bool 成功したかどうか
*************************************************************************/
bool Json::Save(const gu::tstring& filePath, const bool writeAsync)
{
	if (!_writer)
	{
		return false;
	}

	return _writer->Flush(filePath, writeAsync);
}

/*!**********************************************************************
*  @brief      デシリアライズを行います.　デシリアライズとはJson形式の文字列をオブジェクトに変換することです.
*  @param[in]  const gu::SharedPointer<JsonReader>& jsonを読み込むクラス
*  @param[out] gu::SharedPointer<JsonObject> デシリアライズしたいオブジェクト
*  @param[in]  const ReadFlags flags = ReadFlags::None
*  @param[in]  const bool readAsync = false
*  @return     bool 成功したかどうか
*************************************************************************/
bool Json::Deserialize(gu::SharedPointer<JsonObject>& object, const ReadFlags flags)
{
	State stackState = {};
	if(!Deserialize(stackState, flags))
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
*  @param[in] gu::DynamicArray<gu::SharedPointer<JsonValue>>& デシリアライズしたいオブジェクト
*  @param[in] const ReadFlags flags = ReadFlags::None
*  @return    bool 成功したかどうか
*************************************************************************/
bool Json::Deserialize(gu::DynamicArray<gu::SharedPointer<JsonValue>>& outArray, const ReadFlags flags)
{
	State stackState = {};
	if(!Deserialize(stackState, flags))
	{
		return false;
	}

	if(stackState.Type != JsonValueType::Array)
	{
		return false;
	}

	outArray = stackState.Array;

	return true;
}

/*!**********************************************************************
*  @brief      デシリアライズを行います.　デシリアライズとはJson形式の文字列をオブジェクトに変換することです.
*  @param[out] gu::SharedPointer<JsonValue>& Json解析するためのデータ構造
*  @param[in]  const  ReadFlags フラグ
*  @return     bool   成功したかどうか
*************************************************************************/
bool Json::Deserialize(gu::SharedPointer<JsonValue>& value, const ReadFlags flags)
{
	State stackState = {};
	if(!Deserialize(stackState, flags))
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
*  @param[out]State& Json解析するためのデータ構造
*  @param[in] const  ReadFlags フラグ
*  @return    bool   成功したかどうか
*************************************************************************/
bool Json::Deserialize(State& outStackState, const ReadFlags flags)
{
	/*-------------------------------------------------------------------
	-          Stateの準備
	---------------------------------------------------------------------*/
	if (!_reader)
	{
		_RPT0(_CRT_WARN, "JsonReader is not initialized.\n Please call the Load function \n");
		return false;
	}

	// ObjectやArrayといった複数の要素を一時的に貯めておく
	Stack<SharedPointer<State>> stackState = {};

	// 現在の読み取り情報
	SharedPointer<State> currentState = nullptr;

	// 現在の読み取り値
	SharedPointer<JsonValue> newValue = nullptr;

	// 読み取っているNotation
	JsonNotation notation = JsonNotation::None;

	while (_reader->ReadNext(notation))
	{
		tstring key = _reader->GetKey();
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
				newValue = MakeShared<JsonValueBoolean>(_reader->GetValueAsBoolean());
				break;
			}
			case JsonNotation::Number:
			{
				if (flags == ReadFlags::StoreNumberAsString)
				{
					newValue = MakeShared<JsonValueNumberString>(_reader->GetValueAsNumberString());
				}
				else
				{
					newValue = MakeShared<JsonValueNumber>(_reader->GetValueAsNumber());
				}
				break;
			}
			case JsonNotation::String:
			{
				newValue = MakeShared<JsonValueString>(_reader->GetValueAsString());
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

bool Json::Serialize(const gu::SharedPointer<Element>& startElement)
{
	if (!_writer)
	{
		_writer = gu::MakeShared<JsonWriter>();
	}

	gu::Stack<gu::SharedPointer<Element>> stackElements = {};
	stackElements.Push(startElement);

	/*-------------------------------------------------------------------
	-          Jsonを書き込む
	---------------------------------------------------------------------*/
	while (stackElements.Size() > 0)
	{
		const gu::SharedPointer<Element> element = *(&stackElements.Top());
		stackElements.Pop();

		const bool writeValueOnly = !element->IsKeyValue || (element->Key.IsEmpty() && _writer->GetCurrentValueType() != JsonValueType::Object);

		/*-------------------------------------------------------------------
		-          各JsonValueTypeにしたがって書き込む
		---------------------------------------------------------------------*/
		switch (element->Value->GetValueType())
		{
			case JsonValueType::Number:
			{
				if (writeValueOnly)
				{
					if (element->Value->UseStringRepresentation())
					{
						_writer->WriteValue(SP(""), StaticPointerCast<JsonValueNumberString>(element->Value)->GetString());
					}
					else
					{
						_writer->WriteValue(SP(""), StaticPointerCast<JsonValueNumber>(element->Value)->GetDouble());
					}
				}
				else 
				{
					if (element->Value->UseStringRepresentation())
					{
						_writer->WriteValue(element->Key, StaticPointerCast<JsonValueNumberString>(element->Value)->GetString());
					}
					else
					{
						_writer->WriteValue(element->Key, StaticPointerCast<JsonValueNumber>(element->Value)->GetDouble());
					}
				}
				break;
			}
			case JsonValueType::String:
			{
				if(writeValueOnly)
				{
					_writer->WriteValue(SP(""), StaticPointerCast<JsonValueString>(element->Value)->GetString());
				}
				else
				{
					_writer->WriteValue(element->Key, StaticPointerCast<JsonValueString>(element->Value)->GetString());
				}
				break;
			}
			case JsonValueType::True:
			case JsonValueType::False:
			{
				if (writeValueOnly)
				{
					_writer->WriteValue(SP(""), StaticPointerCast<JsonValueBoolean>(element->Value)->GetBool());
				}
				else
				{
					_writer->WriteValue(element->Key, StaticPointerCast<JsonValueBoolean>(element->Value)->GetBool());
				}
				break;
			}
			case JsonValueType::Array:
			{
				if (element->IsExecuted)
				{
					_writer->WriteArrayEnd();
				}
				else
				{
					element->IsExecuted = true;

					stackElements.Push(element);

					if(writeValueOnly)
					{
						_writer->WriteArrayStart();
					}
					else
					{
						_writer->WriteArrayStart(element->Key);
					}

					const auto& values = StaticPointerCast<JsonValueArray>(element->Value)->GetArray();
					for (int64 i = static_cast<int64>(values.Size()) - 1; i >= 0; --i)
					{
						stackElements.Push(MakeShared<Element>(values[i]));
					}
				}
				break;
			}
			case JsonValueType::Object:
			{
				if (element->IsExecuted)
				{
					_writer->WriteObjectEnd();
				}
				else
				{
					element->IsExecuted = true;

					const auto& objects = StaticPointerCast<JsonValueObject>(element->Value)->GetObject();

					stackElements.Push(element);

					if (writeValueOnly)
					{
						_writer->WriteObjectStart();
					}
					else
					{
						_writer->WriteObjectStart(element->Key);
					}
								
					DynamicArray<Pair<tstring, SharedPointer<JsonValue>>*> tempPairs = {};
					for (auto& keyValue : objects->Data())
					{
						tempPairs.Push(&keyValue);
					}

					for (int64 i = static_cast<int64>(tempPairs.Size()) - 1; i >= 0; --i)
					{
						stackElements.Push(MakeShared<Element>(tempPairs[i]->Key, tempPairs[i]->Value));
					}
				}
				break;
			}
			case JsonValueType::Null:
			{
				_writer->WriteValue(element->Key, SP("null"));
				break;
			}
			default:
			{
				return false;
			}
		}
	}

	return true;
}
#pragma endregion Private Function