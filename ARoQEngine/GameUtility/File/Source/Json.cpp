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
*  @brief     シリアライズを行います.　シリアライズとはオブジェクトをJson形式の文字列に変換することです.
*  @param[in] const gu::SharedPointer<JsonWriter>& jsonを書き込むクラス
*  @param[in] const gu::SharedPointer<JsonObject>& シリアライズしたいオブジェクト
*  @return    bool 成功したかどうか
*************************************************************************/
bool JsonSerializer::Serialize(const gu::SharedPointer<JsonWriter>& writer, const gu::SharedPointer<Element>& startElement)
{
	return false;
}


/*!**********************************************************************
*  @brief     デシリアライズを行います.　デシリアライズとはJson形式の文字列をオブジェクトに変換することです.
*  @param[in] const gu::SharedPointer<JsonReader>& jsonを読み込むクラス
*  @param[in] const gu::DynamicArray<gu::SharedPointer<JsonValue>>& デシリアライズしたいオブジェクト
*  @return    bool 成功したかどうか
*************************************************************************/
bool JsonSerializer::Deserialize(const gu::SharedPointer<JsonReader>& reader, State& state)
{
	/*-------------------------------------------------------------------
	-          Stateの準備
	---------------------------------------------------------------------*/
	// ObjectやArrayといった複数の要素を一時的に貯めておく
	DynamicArray<SharedPointer<State>> stackState = {};

	// 現在の読み取り情報
	SharedPointer<State> currentState = nullptr;

	// 現在の読み取り値
	SharedPointer<JsonValue> newValue = nullptr;

	// 読み取っているNotation
	JsonNotation notation = JsonNotation::None;

	/*-------------------------------------------------------------------
	-         読み取り
	

	return false;
}
#pragma endregion Private Function