//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonValueNull.hpp
///  @brief  何もないJsonの値を扱うクラス
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
	*				  			JsonValueNull
	*************************************************************************//**
	/*  @brief  何もないJsonの値を扱うクラス
	*****************************************************************************/
	struct JsonValueNull : public JsonValue
	{
	public:
		#pragma region Public Function

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		/*! @brief デフォルトコンストラクタ*/
		JsonValueNull() : JsonValue(JsonValueType::Null) {}

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


		#pragma endregion 
	};
}
#endif