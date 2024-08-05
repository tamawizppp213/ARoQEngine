//////////////////////////////////////////////////////////////////////////////////
///  @file   Json.hpp
///  @brief  Jsonのシリアライズやデシリアライズを行うクラス
///  @author toide
///  @date   2024/06/29 14:23:49
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JSON_HPP
#define JSON_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Private/Json/Include/JsonCommon.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueObject.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueArray.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Base/Include/GUString.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::file
{
	namespace json
	{
		class JsonReader;
		class JsonWriter;
	}

	/****************************************************************************
	*				  			   Json
	****************************************************************************/
	/*  @brief   Jsonのシリアライズやデシリアライズを行うクラス
	*****************************************************************************/
	class Json
	{
	private:
		/****************************************************************************
		*				  			  State
		****************************************************************************/
		/*  @brief   Json解析するためのデータ構造
		*****************************************************************************/
		struct State
		{
			/*! @brief Jsonの値の種類*/
			json::JsonValueType Type = json::JsonValueType::Null;

			/*! @brief キー名*/
			gu::tstring Key = SP("");

			/*! @brief Json配列*/
			gu::DynamicArray<gu::SharedPointer<json::JsonValue>> Array = {};

			/*! @brief Jsonオブジェクト*/
			gu::SharedPointer<json::JsonObject> Object = nullptr;
		};

		/****************************************************************************
		*				  			 Element
		****************************************************************************/
		/*  @brief   Jsonの要素
		*****************************************************************************/
		struct Element
		{
			/*! @brief キー名*/
			gu::tstring Key = SP("");

			/*! @brief 値*/
			gu::SharedPointer<json::JsonValue> Value = nullptr;

			/*! @brief 要素が実行されたか*/
			bool IsExecuted = false;

			/*! @brief 要素がキー/値のペアであるかどうか*/
			bool IsKeyValue = false;

			Element(const gu::SharedPointer<json::JsonValue>& value) : Value(value) {}
			Element(const gu::SharedPointer<json::JsonObject>& object) : Value(gu::MakeShared<json::JsonValueObject>(object)) {};
			Element(const gu::DynamicArray<gu::SharedPointer<json::JsonValue>>& arrayData) :
				Value(gu::MakeShared<json::JsonValueArray>(arrayData)) {};
			Element(const gu::tstring& key, const gu::SharedPointer<json::JsonValue>& value) : Key(key), Value(value), IsKeyValue(true) {}
		};

	public:
		#pragma region Public Function
		enum class ReadFlags
		{
			None                = 0,
			StoreNumberAsString = 1
		};

		/*!**********************************************************************
		*  @brief     シリアライズを行います.　シリアライズとはオブジェクトをJson形式の文字列に変換することです.
		*  @param[in] const gu::SharedPointer<JsonWriter>& jsonを書き込むクラス
		*  @param[in] const gu::SharedPointer<JsonValue>& シリアライズしたいオブジェクト
		*  @return    bool 成功したかどうか
		*************************************************************************/
		__forceinline bool Serialize(const gu::tstring& filePath, const gu::SharedPointer<json::JsonValue>& value, const bool writeAsync = false)
		{
			return Serialize(filePath, gu::MakeShared<Element>(value), writeAsync);
		}

		/*!**********************************************************************
		*  @brief     シリアライズを行います.　シリアライズとはオブジェクトをJson形式の文字列に変換することです.
		*  @param[in] const gu::SharedPointer<JsonWriter>& jsonを書き込むクラス
		*  @param[in] const gu::SharedPointer<JsonObject>& シリアライズしたいオブジェクト
		*  @return    bool 成功したかどうか
		*************************************************************************/
		bool Serialize(const gu::tstring& filePath, const gu::SharedPointer<json::JsonObject>& object, const bool writeAsync = false)
		{
			return Serialize(filePath, gu::MakeShared<Element>(object), writeAsync);
		}

		/*!**********************************************************************
		*  @brief     シリアライズを行います.　シリアライズとはオブジェクトをJson形式の文字列に変換することです.
		*  @param[in] const gu::SharedPointer<JsonWriter>& jsonを書き込むクラス
		*  @param[in] const gu::SharedPointer<JsonObject>& シリアライズしたいオブジェクト
		*  @return    bool 成功したかどうか
		*************************************************************************/
		bool Serialize(const gu::tstring& filePath, const gu::DynamicArray<gu::SharedPointer<json::JsonValue>>& object, const bool writeAsync = false)
		{
			return Serialize(filePath, gu::MakeShared<Element>(object), writeAsync);
		}

		/*!**********************************************************************
		*  @brief      デシリアライズを行います.　デシリアライズとはJson形式の文字列をオブジェクトに変換することです.
		*  @param[in]  const gu::SharedPointer<JsonReader>& jsonを読み込むクラス
		*  @param[out] gu::SharedPointer<JsonObject> デシリアライズしたいオブジェクト
		*  @param[in]  const ReadFlags flags = ReadFlags::None
		*  @param[in]  const bool readAsync = false
		*  @return     bool 成功したかどうか
		*************************************************************************/
		bool Deserialize(const gu::tstring& filePath, gu::SharedPointer<json::JsonObject>& object, const ReadFlags flags = ReadFlags::None, const bool readAsync = false);

		/*!**********************************************************************
		*  @brief     デシリアライズを行います.　デシリアライズとはJson形式の文字列をオブジェクトに変換することです.
		*  @param[in] const gu::SharedPointer<JsonReader>& jsonを読み込むクラス
		*  @param[out]gu::DynamicArray<gu::SharedPointer<JsonValue>>& デシリアライズしたいオブジェクト
		*  @param[in] const ReadFlags flags = ReadFlags::None
		*  @param[in] const bool readAsync = false
		*  @return    bool 成功したかどうか
		*************************************************************************/
		bool Deserialize(const gu::tstring& filePath, gu::DynamicArray<gu::SharedPointer<json::JsonValue>>& outArray, const ReadFlags flags = ReadFlags::None, const bool readAsync = false);

		/*!**********************************************************************
		*  @brief      デシリアライズを行います.　デシリアライズとはJson形式の文字列をオブジェクトに変換することです.
		*  @param[in]  const gu::tstring& ファイルパス
		*  @param[out] gu::SharedPointer<JsonValue>& Json解析するためのデータ構造
		*  @param[in]  const  ReadFlags フラグ
		*  @return     bool   成功したかどうか
		*************************************************************************/
		bool Deserialize(const gu::tstring& filePath, gu::SharedPointer<json::JsonValue>& value, const ReadFlags flags = ReadFlags::None, const bool readAsync = false);


		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor
		#pragma endregion

		#pragma region Private Function

		/*!**********************************************************************
		*  @brief     シリアライズを行います.　シリアライズとはオブジェクトをJson形式の文字列に変換することです.
		*  @param[in] const gu::SharedPointer<JsonWriter>& jsonを書き込むクラス
		*  @param[in] const gu::SharedPointer<JsonObject>& シリアライズしたいオブジェクト
		*  @param[in]
		*  @return    bool 成功したかどうか
		*************************************************************************/
		bool Serialize(const gu::tstring& filePath, const gu::SharedPointer<Element>& startElement, const bool writeAsync);

		/*!**********************************************************************
		*  @brief     デシリアライズを行います.　デシリアライズとはJson形式の文字列をオブジェクトに変換することです.
		*  @param[in] const  gu::tstring ファイルパス
		*  @param[out]State& Json解析するためのデータ構造
		*  @param[in] const  ReadFlags フラグ
		*  @return    bool   成功したかどうか
		*************************************************************************/
		bool Deserialize(const gu::tstring& filePath, State& outStackState, const ReadFlags flags = ReadFlags::None, const bool readAsync = false);

		#pragma endregion 

		#pragma region Private Property

		#pragma endregion 

	};

}
#endif