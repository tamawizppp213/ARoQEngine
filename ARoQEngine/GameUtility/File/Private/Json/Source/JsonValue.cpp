//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonValue.cpp
///  @brief  temp
///  @author toide
///  @date   2024/06/08 22:10:04
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Private/Json/Include/JsonValue.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"

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
#pragma region TryGetValue
/*!**********************************************************************
*  @brief     String型の値を取得します.
*  @param[in] void
*  @return    tstring : 文字列の結果
*************************************************************************/
tstring JsonValue::GetString() const
{
	return tstring();
}

/*!**********************************************************************
*  @brief     Int8の値を取得します
*  @param[in] void
*  @return    int8
*************************************************************************/
int8 JsonValue::GetInt8() const
{
	throw "Json type is not number";
}

/*!**********************************************************************
*  @brief     Int16の値を取得します
*  @param[in] void
*  @return    int16
*************************************************************************/
int16 JsonValue::GetInt16() const
{
	throw "Json type is not number";
}

/*!**********************************************************************
*  @brief     Int32の値を取得します
*  @param[in] void
*  @return    int32
*************************************************************************/
int32 JsonValue::GetInt32() const
{
	throw "Json type is not number";
}

/*!**********************************************************************
*  @brief     Int64の値を取得します
*  @param[in] void
*  @return    int64
*************************************************************************/
int64 JsonValue::GetInt64() const
{
	throw "Json type is not number";
}

/*!**********************************************************************
*  @brief     UInt8の値を取得します
*  @param[in] void
*  @return    uint8
*************************************************************************/
uint8 JsonValue::GetUInt8() const
{
	throw "Json type is not number";
}

/*!**********************************************************************
*  @brief     UInt16の値を取得します
*  @param[in] void
*  @return    uint16
*************************************************************************/
uint16 JsonValue::GetUInt16() const 
{
	throw "Json type is not number";
}

/*!**********************************************************************
*  @brief     UInt32の値を取得します
*  @param[in] void
*  @return    uint32
*************************************************************************/
uint32 JsonValue::GetUInt32() const
{
	throw "Json type is not number";
}

/*!**********************************************************************
*  @brief     UInt64の値を取得します
*  @param[in] void
*  @return    uint64
*************************************************************************/
uint64 JsonValue::GetUInt64() const
{
	throw "Json type is not number";
}

#pragma endregion TryGetValue
#pragma endregion Public Function