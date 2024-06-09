//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonValueBoolean.hpp
///  @brief  Boolean型のJson値を扱うクラス
///  @author toide
///  @date   2024/06/09 0:47:13
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JSON_BOOLEAN_HPP
#define JSON_BOOLEAN_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "JsonValue.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::file::json
{
	/****************************************************************************
	*				  			   JsonBoolean
	*************************************************************************//**
	/*  @brief  true/falseのJsonの値を扱うクラス
	*****************************************************************************/
	struct JsonValueBoolean : public JsonValue
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     String型の値を取得します.
		*  @param[in] void
		*  @return    tstring : 文字列の結果
		*************************************************************************/
		virtual tstring GetString() const override;

		/*!**********************************************************************
		*  @brief     boolの値を取得します
		*  @param[in] void
		*  @return    int8
		*************************************************************************/
		virtual bool GetBool() const override;

		/*!**********************************************************************
		*  @brief     Int8の値を取得します
		*  @param[in] void
		*  @return    int8
		*************************************************************************/
		virtual int8 GetInt8() const override;

		/*!**********************************************************************
		*  @brief     Int16の値を取得します
		*  @param[in] void
		*  @return    int16
		*************************************************************************/
		virtual int16 GetInt16() const override;

		/*!**********************************************************************
		*  @brief     Int32の値を取得します
		*  @param[in] void
		*  @return    int32
		*************************************************************************/
		virtual int32 GetInt32() const override;

		/*!**********************************************************************
		*  @brief     Int64の値を取得します
		*  @param[in] void
		*  @return    int64
		*************************************************************************/
		virtual int64 GetInt64() const override;

		/*!**********************************************************************
		*  @brief     UInt8の値を取得します
		*  @param[in] void
		*  @return    uint8
		*************************************************************************/
		virtual uint8 GetUInt8() const override;

		/*!**********************************************************************
		*  @brief     UInt16の値を取得します
		*  @param[in] void
		*  @return    uint16
		*************************************************************************/
		virtual uint16 GetUInt16() const override;

		/*!**********************************************************************
		*  @brief     UInt32の値を取得します
		*  @param[in] void
		*  @return    uint32
		*************************************************************************/
		virtual uint32 GetUInt32() const override;

		/*!**********************************************************************
		*  @brief     UInt64の値を取得します
		*  @param[in] void
		*  @return    uint64
		*************************************************************************/
		virtual uint64 GetUInt64() const override;

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

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		/*! @brief デフォルトコンストラクタ*/
		JsonValueBoolean() : JsonValue(JsonValueType::False) {}

		/*! @brief bool値で初期化*/
		JsonValueBoolean(const bool value) : JsonValue(value ? JsonValueType::True : JsonValueType::False), _value(value) {}

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

		#pragma endregion 

		#pragma region Private Property

		/*! @brief 保存するbool値*/
		bool _value = false;

		#pragma endregion 
	};
}
#endif