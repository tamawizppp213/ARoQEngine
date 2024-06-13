//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonValue.hpp
///  @brief  temp
///  @author toide
///  @date   2024/06/08 13:38:55
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JSON_VALUE_HPP
#define JSON_VALUE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "JsonObject.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::file::json
{
	/****************************************************************************
	*				  			   JsonValue
	*************************************************************************//**
	/*  @brief  Jsonの値の取得などを扱うクラス
	*****************************************************************************/
	struct JsonValue
	{
	public:
		#pragma region Public Function

		#pragma endregion 

		#pragma region Public Property

		#pragma region GetValue
		/*!**********************************************************************
		*  @brief     Jsonの種類を取得します.
		*  @param[in] void
		*  @return    JsonValueType
		*************************************************************************/
		JsonValueType GetValueType() const { return _type; }

		/*!**********************************************************************
		*  @brief     String型の値を取得します.
		*  @param[in] void
		*  @return    tstring : 文字列の結果
		*************************************************************************/
		virtual tstring GetString() const;

		/*!**********************************************************************
		*  @brief     JsonObject型の値を取得します.
		*  @param[in] void
		*  @return    JsonObject
		*************************************************************************/
		virtual SharedPointer<JsonObject> GetObject() { return nullptr; }

		/*!**********************************************************************
		*  @brief     JsonObject型の値を取得します.
		*  @param[in] void
		*  @return    JsonObject
		*************************************************************************/
		virtual const SharedPointer<JsonObject> GetObject() const { return nullptr; }

		/*!**********************************************************************
		*  @brief     Array型の値を取得します.
		*  @param[in] void
		*  @return    DynamicArray<SharedPointer<JsonValue>>
		*************************************************************************/
		virtual DynamicArray<SharedPointer<JsonValue>>& GetArray() { throw "failed to get array"; }

		/*!**********************************************************************
		*  @brief     Array型の値を取得します.
		*  @param[in] void
		*  @return    DynamicArray<SharedPointer<JsonValue>>
		*************************************************************************/
		virtual const DynamicArray<SharedPointer<JsonValue>>& GetArray() const { throw "failed to get array"; }

		/*!**********************************************************************
		*  @brief     boolの値を取得します
		*  @param[in] void
		*  @return    int8
		*************************************************************************/
		virtual bool GetBool() const { throw "failed to get bool"; }

		/*!**********************************************************************
		*  @brief     Int8の値を取得します
		*  @param[in] void
		*  @return    int8
		*************************************************************************/
		virtual int8 GetInt8() const;

		/*!**********************************************************************
		*  @brief     Int16の値を取得します
		*  @param[in] void
		*  @return    int16
		*************************************************************************/
		virtual int16 GetInt16() const;

		/*!**********************************************************************
		*  @brief     Int32の値を取得します
		*  @param[in] void
		*  @return    int32
		*************************************************************************/
		virtual int32 GetInt32() const;

		/*!**********************************************************************
		*  @brief     Int64の値を取得します
		*  @param[in] void
		*  @return    int64
		*************************************************************************/
		virtual int64 GetInt64() const;

		/*!**********************************************************************
		*  @brief     UInt8の値を取得します
		*  @param[in] void
		*  @return    uint8
		*************************************************************************/
		virtual uint8 GetUInt8() const;

		/*!**********************************************************************
		*  @brief     UInt16の値を取得します
		*  @param[in] void
		*  @return    uint16
		*************************************************************************/
		virtual uint16 GetUInt16() const;

		/*!**********************************************************************
		*  @brief     UInt32の値を取得します
		*  @param[in] void
		*  @return    uint32
		*************************************************************************/
		virtual uint32 GetUInt32() const;

		/*!**********************************************************************
		*  @brief     UInt64の値を取得します
		*  @param[in] void
		*  @return    uint64
		*************************************************************************/
		virtual uint64 GetUInt64() const;

		/*!**********************************************************************
		*  @brief     Floatの値を取得します
		*  @param[in] void
		*  @return    float
		*************************************************************************/
		virtual float GetFloat() const;

		/*!**********************************************************************
		*  @brief     Doubleの値を取得します
		*  @param[in] void
		*  @return    double
		*************************************************************************/
		virtual double GetDouble() const;

		#pragma endregion TryGetValue

		#pragma region Type Check
		/*!**********************************************************************
		*  @brief     Nullかどうかを返します.
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool IsNull() const { return _type == JsonValueType::Null; }

		/*!**********************************************************************
		*  @brief     Falseかどうかを返します
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool IsFalse() const { return _type == JsonValueType::False; }

		/*!**********************************************************************
		*  @brief     Trueかどうかを返します
		*  @param[in] void
		*  @return    bool
		* ************************************************************************/
		__forceinline bool IsTrue() const { return _type == JsonValueType::True; }

		/*!**********************************************************************
		*  @brief     Boolかどうかを返します
		*  @param[in] void
		*  @return    bool
		* ************************************************************************/
		__forceinline bool IsBool() const { return _type == JsonValueType::True || _type ==  JsonValueType::False; }

		/*!**********************************************************************
		*  @brief     Numberかどうかを返します
		*  @param[in] void
		*  @return    bool
		* ************************************************************************/
		__forceinline bool IsNumber() const { return _type == JsonValueType::Number; }

		/*!**********************************************************************
		*  @brief     Stringかどうかを返します
		*  @param[in] void
		*  @return    bool
		* ************************************************************************/
		__forceinline bool IsString() const { return HasAnyFlags(_type, JsonValueType::String); }

		/*!**********************************************************************
		*  @brief     Arrayかどうかを返します
		*  @param[in] void
		*  @return    bool
		* ************************************************************************/
		__forceinline bool IsArray() const { return HasAnyFlags(_type, JsonValueType::Array); }

		/*!**********************************************************************
		*  @brief     Objectかどうかを返します
		*  @param[in] void
		*  @return    bool
		* ************************************************************************/
		__forceinline bool IsObject() const { return HasAnyFlags(_type, JsonValueType::Object); }

		#pragma endregion Type Check

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		JsonValue() = default;

		/*! @brief デストラクタ*/
		virtual ~JsonValue() = default;

		/*! @brief Typeを指定する*/
		JsonValue(const JsonValueType type) : _type(type) {};
		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property
		/*! @brief Jsonの種類*/
		JsonValueType _type = JsonValueType::Null;

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Property
		

		#pragma endregion 

	};


}

#endif