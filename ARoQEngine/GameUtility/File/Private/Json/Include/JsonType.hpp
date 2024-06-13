//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonType.hpp
///  @brief  temp
///  @author toide
///  @date   2024/06/08 13:43:20
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JSON_TYPE_HPP
#define JSON_TYPE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Base/Include/GUEnumClassFlags.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::file::json
{
	/****************************************************************************
	*				  		JsonValueType
	****************************************************************************/
	/*  @brief 　Jsonの値の種類
	*****************************************************************************/
	enum class JsonValueType : gu::uint16
	{
		Null    = 0, //!< 未設定
		False   = 1, //!< false
		True    = 2, //!< true
		Object  = 3, //!< オブジェクト型
		Array   = 4, //!< 配列
		String  = 5, //!< 文字列
		Number  = 6
	};
}

#endif