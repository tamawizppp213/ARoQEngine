//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonObject.hpp
///  @brief  本クラスは, Jsonのオブジェクトを扱うクラスです.@n
///          Jsonのオブジェクトとは, {"key":"value"}のような形式のデータです.@n
///  @author toide
///  @date   2024/06/08 18:02:51
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JSON_OBJECT_HPP
#define JSON_OBJECT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "JsonType.hpp"
#include "GameUtility/Container/Include/GUHashMap.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu::file::json
{
	struct JsonValue;

	/****************************************************************************
	*				  			   JsonObject
	*************************************************************************//**
	/*  @class     JsonObject
	*   @brief     temp
	*****************************************************************************/
	struct JsonObject
	{
	private:
		using ObjectMap = HashMap<tstring, SharedPointer<JsonValue>>;

	public:
		#pragma region Public Function

		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     生のHashMapを返します.
		*  @param[in] void
		*  @return    HashMap<tstring, SharedPointer<JsonValue>>
		*************************************************************************/
		ObjectMap& Data() { return _values; }

		/*!**********************************************************************
		*  @brief     生のHashMapをconst参照付きで返します.
		*  @param[in] void
		*  @return    HashMap<tstring, SharedPointer<JsonValue>>
		*************************************************************************/
		const ObjectMap& Data() const { return _values; }

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

		#pragma endregion 

		#pragma region Private Property
		/*! @brief オブジェクトを格納するキーバリューペア*/
		ObjectMap _values = {};
		#pragma endregion 

	};

}
#endif