//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonValueObject.hpp
///  @brief  Object型のJsonの値を扱うクラス
///  @author toide
///  @date   2024/06/09 0:47:13
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JSON_NULL_HPP
#define JSON_NULL_HPP

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
	*				  			   JsonValueObject
	*************************************************************************//**
	/*  @brief  Object型のJsonの値を扱うクラス
	*****************************************************************************/
	struct JsonValueObject : public JsonValue
	{
	public:
		#pragma region Public Function

		/*!**********************************************************************
		*  @brief     JsonObject型の値を取得します.
		*  @param[in] void
		*  @return    JsonObject
		*************************************************************************/
		virtual SharedPointer<JsonObject> GetObject() override;

		/*!**********************************************************************
		*  @brief     JsonObject型の値を取得します.
		*  @param[in] void
		*  @return    JsonObject
		*************************************************************************/
		virtual const SharedPointer<JsonObject> GetObject() const override;

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		/*! @brief デフォルトコンストラクタ*/
		JsonValueObject() : JsonValue(JsonValueType::Object) {}

		/*! @brief オブジェクト型を使って初期化*/
		JsonValueObject(SharedPointer<JsonObject>& object) : JsonValue(JsonValueType::Object), _object(object) {}

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
		/*! @brief 保存するオブジェクト*/
		SharedPointer<JsonObject> _object = nullptr;

		#pragma endregion 
	};
}
#endif