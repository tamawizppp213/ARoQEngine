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
namespace gu::file::json
{
	class JsonReader;
	class JsonWriter;

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
			JsonValueType Type = JsonValueType::Null;

			/*! @brief キー名*/
			gu::tstring Key = SP("");

			/*! @brief Json配列*/
			gu::DynamicArray<gu::SharedPointer<JsonValue>> Array = {};

			/*! @brief Jsonオブジェクト*/
			gu::SharedPointer<JsonObject> Object = nullptr;
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
			gu::SharedPointer<JsonValue> Value = nullptr;

			/*! @brief 要素が実行されたか*/
			bool IsExecuted = false;

			/*! @brief 要素がキー/値のペアであるかどうか*/
			bool IsKeyValue = false;

			Element(const gu::SharedPointer<JsonValue>& value) : Value(value) {}
			Element(const gu::SharedPointer<JsonObject>& object);
			Element(const gu::DynamicArray<gu::SharedPointer<JsonValue>>& array);
			Element(const gu::tstring& key, const gu::SharedPointer<JsonValue>& value) : Key(key), Value(value), IsKeyValue(true) {}
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
		static bool Serialize(const gu::SharedPointer<JsonWriter>& writer, const gu::SharedPointer<JsonValue>& value)
		{
			return false;
		}

		/*!**********************************************************************
		*  @brief     シリアライズを行います.　シリアライズとはオブジェクトをJson形式の文字列に変換することです.
		*  @param[in] const gu::SharedPointer<JsonWriter>& jsonを書き込むクラス
		*  @param[in] const gu::SharedPointer<JsonObject>& シリアライズしたいオブジェクト
		*  @return    bool 成功したかどうか
		*************************************************************************/
		static bool Serialize(const gu::SharedPointer<JsonWriter>& writer, const gu::SharedPointer<JsonObject>& object)
		{
			return false;
		}

		/*!**********************************************************************
		*  @brief     シリアライズを行います.　シリアライズとはオブジェクトをJson形式の文字列に変換することです.
		*  @param[in] const gu::SharedPointer<JsonWriter>& jsonを書き込むクラス
		*  @param[in] const gu::SharedPointer<JsonObject>& シリアライズしたいオブジェクト
		*  @return    bool 成功したかどうか
		*************************************************************************/
		static bool Serialize(const gu::SharedPointer<JsonWriter>& writer, const gu::DynamicArray<gu::SharedPointer<JsonValue>>& object)
		{
			return false;
		}

		/*!**********************************************************************
		*  @brief     デシリアライズを行います.　デシリアライズとはJson形式の文字列をオブジェクトに変換することです.
		*  @param[in] const gu::SharedPointer<JsonReader>& jsonを読み込むクラス
		*  @param[in] const gu::SharedPointer<JsonValue>& デシリアライズしたいオブジェクト
		*  @return    bool 成功したかどうか
		*************************************************************************/
		bool Deserialize(const gu::SharedPointer<JsonValue>& value)
		{
			return false;
		}

		/*!**********************************************************************
		*  @brief      デシリアライズを行います.　デシリアライズとはJson形式の文字列をオブジェクトに変換することです.
		*  @param[in]  const gu::SharedPointer<JsonReader>& jsonを読み込むクラス
		*  @param[out] gu::SharedPointer<JsonObject> デシリアライズしたいオブジェクト
		*  @param[in]  const ReadFlags flags = ReadFlags::None
		*  @param[in]  const bool readAsync = false
		*  @return     bool 成功したかどうか
		*************************************************************************/
		bool Deserialize(const gu::tstring& filePath, gu::SharedPointer<JsonObject>& object, const ReadFlags flags = ReadFlags::None, const bool readAsync = false);

		/*!**********************************************************************
		*  @brief     デシリアライズを行います.　デシリアライズとはJson形式の文字列をオブジェクトに変換することです.
		*  @param[in] const gu::SharedPointer<JsonReader>& jsonを読み込むクラス
		*  @param[out]gu::DynamicArray<gu::SharedPointer<JsonValue>>& デシリアライズしたいオブジェクト
		*  @param[in] const ReadFlags flags = ReadFlags::None
		*  @param[in] const bool readAsync = false
		*  @return    bool 成功したかどうか
		*************************************************************************/
		bool Deserialize(const gu::tstring& filePath, gu::DynamicArray<gu::SharedPointer<JsonValue>>& outArray, const ReadFlags flags = ReadFlags::None, const bool readAsync = false);

		/*!**********************************************************************
		*  @brief      デシリアライズを行います.　デシリアライズとはJson形式の文字列をオブジェクトに変換することです.
		*  @param[in]  const gu::tstring& ファイルパス
		*  @param[out] gu::SharedPointer<JsonValue>& Json解析するためのデータ構造
		*  @param[in]  const  ReadFlags フラグ
		*  @return     bool   成功したかどうか
		*************************************************************************/
		bool Deserialize(const gu::tstring& filePath, gu::SharedPointer<JsonValue>& value, const ReadFlags flags = ReadFlags::None, const bool readAsync = false);


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
		/*! @brief JsonReader*/
		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor
		#pragma endregion

		#pragma region Private Function

		/*!**********************************************************************
		*  @brief     シリアライズを行います.　シリアライズとはオブジェクトをJson形式の文字列に変換することです.
		*  @param[in] const gu::SharedPointer<JsonWriter>& jsonを書き込むクラス
		*  @param[in] const gu::SharedPointer<JsonObject>& シリアライズしたいオブジェクト
		*  @return    bool 成功したかどうか
		*************************************************************************/
		bool Serialize(const gu::SharedPointer<JsonWriter>& writer, const gu::SharedPointer<Element>& startElement);


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