//////////////////////////////////////////////////////////////////////////////////
///  @file   Json.cpp
///  @brief  Json�̃V���A���C�Y��f�V���A���C�Y���s���N���X
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
*  @brief      �f�V���A���C�Y���s���܂�.�@�f�V���A���C�Y�Ƃ�Json�`���̕�������I�u�W�F�N�g�ɕϊ����邱�Ƃł�.
*  @param[in]  const gu::SharedPointer<JsonReader>& json��ǂݍ��ރN���X
*  @param[out] gu::SharedPointer<JsonObject> �f�V���A���C�Y�������I�u�W�F�N�g
*  @param[in]  const ReadFlags flags = ReadFlags::None
*  @param[in]  const bool readAsync = false
*  @return     bool �����������ǂ���
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
*  @brief     �f�V���A���C�Y���s���܂�.�@�f�V���A���C�Y�Ƃ�Json�`���̕�������I�u�W�F�N�g�ɕϊ����邱�Ƃł�.
*  @param[in] const gu::SharedPointer<JsonReader>& json��ǂݍ��ރN���X
*  @param[in] gu::DynamicArray<gu::SharedPointer<JsonValue>>& �f�V���A���C�Y�������I�u�W�F�N�g
*  @param[in] const ReadFlags flags = ReadFlags::None
*  @param[in] const bool readAsync = false
*  @return    bool �����������ǂ���
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

	return true;
}

/*!**********************************************************************
*  @brief      �f�V���A���C�Y���s���܂�.�@�f�V���A���C�Y�Ƃ�Json�`���̕�������I�u�W�F�N�g�ɕϊ����邱�Ƃł�.
*  @param[in]  const gu::tstring& �t�@�C���p�X
*  @param[out] gu::SharedPointer<JsonValue>& Json��͂��邽�߂̃f�[�^�\��
*  @param[in]  const  ReadFlags �t���O
*  @return     bool   �����������ǂ���
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
*  @brief     �f�V���A���C�Y���s���܂�.�@�f�V���A���C�Y�Ƃ�Json�`���̕�������I�u�W�F�N�g�ɕϊ����邱�Ƃł�.
*  @param[in] const  gu::tstring �t�@�C���p�X
*  @param[out]State& Json��͂��邽�߂̃f�[�^�\��
*  @param[in] const  ReadFlags �t���O
*  @return    bool   �����������ǂ���
*************************************************************************/
bool Json::Deserialize(const gu::tstring& filePath, State& outStackState, const ReadFlags flags, const bool useAsync)
{
	/*-------------------------------------------------------------------
	-          State�̏���
	---------------------------------------------------------------------*/
	// Json��ǂݎ��
	const auto reader = gu::MakeShared<JsonReader>(filePath, useAsync);

	// Object��Array�Ƃ����������̗v�f���ꎞ�I�ɒ��߂Ă���
	Stack<SharedPointer<State>> stackState = {};

	// ���݂̓ǂݎ����
	SharedPointer<State> currentState = nullptr;

	// ���݂̓ǂݎ��l
	SharedPointer<JsonValue> newValue = nullptr;

	// �ǂݎ���Ă���Notation
	JsonNotation notation = JsonNotation::None;

	while (reader->ReadNext(notation))
	{
		tstring key = reader->GetKey();
		newValue.Reset();

		switch (notation)
		{
			case JsonNotation::ObjectStart:
			{
				// Object�̊J�n
				if (currentState)
				{
					// Object�̒���Object������ꍇ
					stackState.Push(currentState);
				}

				// �V����Object���쐬
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
				// Array�̊J�n
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
					newValue = MakeShared<JsonValueNumberString>(reader->GetValueAsNumberString());
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

bool Json::Serialize(const gu::tstring& filePath, const gu::SharedPointer<Element>& startElement, const bool writeAsync)
{
	gu::Stack<gu::SharedPointer<Element>> stackElements = {};
	stackElements.Push(startElement);

	/*-------------------------------------------------------------------
	-          Json����������
	---------------------------------------------------------------------*/
	// Json�������݃N���X
	const auto writer = gu::MakeShared<JsonWriter>();

	while (stackElements.Size() > 0)
	{
		const auto element = *(&stackElements.Top());
		stackElements.Pop();

		const bool writeValueOnly = !element->IsKeyValue || (element->Key.IsEmpty() && writer->GetCurrentValueType() != JsonValueType::Object);

		/*-------------------------------------------------------------------
		-          �eJsonValueType�ɂ��������ď�������
		---------------------------------------------------------------------*/
		switch (element->Value->GetValueType())
		{
			case JsonValueType::Number:
			{
				if (writeValueOnly)
				{
					if (element->Value->UseStringRepresentation())
					{
						writer->WriteValue(SP(""), StaticPointerCast<JsonValueNumberString>(element->Value)->GetString());
					}
					else
					{
						writer->WriteValue(SP(""), StaticPointerCast<JsonValueNumber>(element->Value)->GetDouble());
					}
				}
				else 
				{
					if (element->Value->UseStringRepresentation())
					{
						writer->WriteValue(element->Key, StaticPointerCast<JsonValueNumberString>(element->Value)->GetString());
					}
					else
					{
						writer->WriteValue(element->Key, StaticPointerCast<JsonValueNumber>(element->Value)->GetDouble());
					}
				}
				break;
			}
			case JsonValueType::String:
			{
				if(writeValueOnly)
				{
					writer->WriteValue(SP(""), StaticPointerCast<JsonValueString>(element->Value)->GetString());
				}
				else
				{
					writer->WriteValue(element->Key, StaticPointerCast<JsonValueString>(element->Value)->GetString());
				}
				break;
			}
			case JsonValueType::True:
			case JsonValueType::False:
			{
				if (writeValueOnly)
				{
					writer->WriteValue(SP(""), StaticPointerCast<JsonValueBoolean>(element->Value)->GetBool());
				}
				else
				{
					writer->WriteValue(element->Key, StaticPointerCast<JsonValueBoolean>(element->Value)->GetBool());
				}
				break;
			}
			case JsonValueType::Array:
			{
				if (element->IsExecuted)
				{
					writer->WriteArrayEnd();
				}
				else
				{
					element->IsExecuted = true;

					stackElements.Push(element);

					if(writeValueOnly)
					{
						writer->WriteArrayStart();
					}
					else
					{
						writer->WriteArrayStart(element->Key);
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
					writer->WriteObjectEnd();
				}
				else
				{
					element->IsExecuted = true;

					const auto& objects = StaticPointerCast<JsonValueObject>(element->Value)->GetObject();

					stackElements.Push(element);

					if (writeValueOnly)
					{
						writer->WriteObjectStart();
					}
					else
					{
						writer->WriteObjectStart(element->Key);
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
				writer->WriteValue(element->Key, SP("null"));
				break;
			}
			default:
			{
				break;
			}
		}
	}

	return writer->Flush(filePath, writeAsync);
}
#pragma endregion Private Function