//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonValueString.cpp
///  @brief  String型のJson値を扱うクラス
///  @author toide
///  @date   2024/06/09 1:35:22
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Private/Json/Include/JsonValueString.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "GameUtility/Base/Include/GUParse.hpp"

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

#pragma region GetValue
/*!**********************************************************************
*  @brief     String型の値を取得します.
*  @param[in] void
*  @return    tstring : 文字列の結果
*************************************************************************/
tstring JsonValueString::GetString() const
{
	Checkf(_type == JsonValueType::String, "Json type is not String");
	return _value;
}

/*!**********************************************************************
*  @brief     boolの値を取得します
*  @param[in] void
*  @return    int8
*************************************************************************/
bool JsonValueString::GetBool() const
{
	Checkf(_type == JsonValueType::String, "Json type is not String");
	
	bool result = false;
	Parse::Bool(_value, _value, result); 
	return result;
}

/*!**********************************************************************
*  @brief     Int8の値を取得します
*  @param[in] void
*  @return    int8
*************************************************************************/
int8 JsonValueString::GetInt8() const
{
	Checkf(_type == JsonValueType::String, "Json type is not String");
	return _value.ToInt8();
}

/*!**********************************************************************
*  @brief     Int16の値を取得します
*  @param[in] void
*  @return    int16
*************************************************************************/
int16 JsonValueString::GetInt16() const
{
	Checkf(_type == JsonValueType::String, "Json type is not String");
	return _value.ToInt16();
}

/*!**********************************************************************
*  @brief     Int32の値を取得します
*  @param[in] void
*  @return    int32
*************************************************************************/
int32 JsonValueString::GetInt32() const
{
	Checkf(_type == JsonValueType::String, "Json type is not String");
	return _value.ToInt32();
}

/*!**********************************************************************
*  @brief     Int64の値を取得します
*  @param[in] void
*  @return    int64
*************************************************************************/
int64 JsonValueString::GetInt64() const
{
	Checkf(_type == JsonValueType::String, "Json type is not String");
	return _value.ToInt64();
}

/*!**********************************************************************
*  @brief     UInt8の値を取得します
*  @param[in] void
*  @return    uint8
*************************************************************************/
uint8 JsonValueString::GetUInt8() const
{
	Checkf(_type == JsonValueType::String, "Json type is not String");
	return _value.ToUInt8();
}

/*!**********************************************************************
*  @brief     UInt16の値を取得します
*  @param[in] void
*  @return    uint16
*************************************************************************/
uint16 JsonValueString::GetUInt16() const
{
	Checkf(_type == JsonValueType::String, "Json type is not String");
	return _value.ToUInt16();
}

/*!**********************************************************************
*  @brief     UInt32の値を取得します
*  @param[in] void
*  @return    uint32
*************************************************************************/
uint32 JsonValueString::GetUInt32() const
{
	Checkf(_type == JsonValueType::String, "Json type is not String");
	return _value.ToUInt32();
}

/*!**********************************************************************
*  @brief     UInt64の値を取得します
*  @param[in] void
*  @return    uint64
*************************************************************************/
uint64 JsonValueString::GetUInt64() const
{
	Checkf(_type == JsonValueType::String, "Json type is not String");
	return _value.ToUInt64();
}

/*!**********************************************************************
*  @brief     Floatの値を取得します
*  @param[in] void
*  @return    float
*************************************************************************/
float JsonValueString::GetFloat() const
{
	Checkf(_type == JsonValueType::String, "Json type is not String");
	throw "Not implemented";
}

/*!**********************************************************************
*  @brief     Doubleの値を取得します
*  @param[in] void
*  @return    double
*************************************************************************/
double JsonValueString::GetDouble() const
{
	Checkf(_type == JsonValueType::String, "Json type is not String");
	throw "Not implemented";
}
#pragma endregion GetValue