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
		*  @brief     Jsonファイルを読み込みます.
		*  @param[in] const gu::tstring& ファイルパス
		*  @param[in] const bool 非同期で読み込みを行うか
		*  @return    bool 成功したかどうか
		*************************************************************************/
		bool Load(const gu::tstring& filePath, const bool useAsync);

		/*!**********************************************************************
		*  @brief     Jsonファイルを読み込みます.
		*  @param[in] const gu::tstring& ファイルパス
		*  @param[in] const bool 非同期で読み込みを行うか
		*  @return    bool 成功したかどうか
		*************************************************************************/
		bool Save(const gu::tstring& filePath, const bool writeAsync);

		/*!**********************************************************************
		*  @brief     シリアライズを行います.　シリアライズとはオブジェクトをJson形式の文字列に変換することです.
		*  @param[in] const gu::SharedPointer<JsonValue>& シリアライズしたいオブジェクト
		*  @return    bool 成功したかどうか
		*************************************************************************/
		__forceinline bool Serialize(const gu::SharedPointer<json::JsonValue>& value)
		{
			return Serialize(gu::MakeShared<Element>(value));
		}

		/*!**********************************************************************
		*  @brief     シリアライズを行います.　シリアライズとはオブジェクトをJson形式の文字列に変換することです.
		*  @param[in] const gu::SharedPointer<JsonObject>& シリアライズしたいオブジェクト
		*  @return    bool 成功したかどうか
		*************************************************************************/
		bool Serialize(const gu::SharedPointer<json::JsonObject>& object)
		{
			return Serialize(gu::MakeShared<Element>(object));
		}

		/*!**********************************************************************
		*  @brief     シリアライズを行います.　シリアライズとはオブジェクトをJson形式の文字列に変換することです.
		*  @param[in] const gu::SharedPointer<JsonObject>& シリアライズしたいオブジェクト
		*  @return    bool 成功したかどうか
		*************************************************************************/
		bool Serialize(const gu::DynamicArray<gu::SharedPointer<json::JsonValue>>& object)
		{
			return Serialize(gu::MakeShared<Element>(object));
		}

		/*!**********************************************************************
		*  @brief      デシリアライズを行います.　デシリアライズとはJson形式の文字列をオブジェクトに変換することです.
		*  @param[out] gu::SharedPointer<JsonObject> デシリアライズしたいオブジェクト
		*  @param[in]  const ReadFlags flags = ReadFlags::None
		*  @return     bool 成功したかどうか
		*************************************************************************/
		bool Deserialize(gu::SharedPointer<json::JsonObject>& object, const ReadFlags flags = ReadFlags::None);

		/*!**********************************************************************
		*  @brief     デシリアライズを行います.　デシリアライズとはJson形式の文字列をオブジェクトに変換することです.
		*  @param[out]gu::DynamicArray<gu::SharedPointer<JsonValue>>& デシリアライズしたいオブジェクト
		*  @param[in] const ReadFlags flags = ReadFlags::None
		*  @return    bool 成功したかどうか
		*************************************************************************/
		bool Deserialize(gu::DynamicArray<gu::SharedPointer<json::JsonValue>>& outArray, const ReadFlags flags = ReadFlags::None);

		/*!**********************************************************************
		*  @brief      デシリアライズを行います.　デシリアライズとはJson形式の文字列をオブジェクトに変換することです.
		*  @param[out] gu::SharedPointer<JsonValue>& Json解析するためのデータ構造
		*  @param[in]  const  ReadFlags フラグ
		*  @return     bool   成功したかどうか
		*************************************************************************/
		bool Deserialize(gu::SharedPointer<json::JsonValue>& value, const ReadFlags flags = ReadFlags::None);


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
		/*! @brief Jsonの読み取りポインタ*/
		gu::SharedPointer<json::JsonReader> _reader = nullptr;

		/*! @brief Jsonの書き込みポインタ*/
		gu::SharedPointer<json::JsonWriter> _writer = nullptr;
		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor
		#pragma endregion

		#pragma region Private Function

		/*!**********************************************************************
		*  @brief     シリアライズを行います.　シリアライズとはオブジェクトをJson形式の文字列に変換することです.
		*  @param[in] const gu::SharedPointer<Element>& 要素
		*  @return    bool 成功したかどうか
		*************************************************************************/
		bool Serialize(const gu::SharedPointer<Element>& startElement);

		/*!**********************************************************************
		*  @brief     デシリアライズを行います.　デシリアライズとはJson形式の文字列をオブジェクトに変換することです.
		*  @param[out]State& Json解析するためのデータ構造
		*  @param[in] const  ReadFlags フラグ
		*  @return    bool   成功したかどうか
		*************************************************************************/
		bool Deserialize(State& outStackState, const ReadFlags flags = ReadFlags::None);

		#pragma endregion 

		#pragma region Private Property

		#pragma endregion 

	};

}
#endif