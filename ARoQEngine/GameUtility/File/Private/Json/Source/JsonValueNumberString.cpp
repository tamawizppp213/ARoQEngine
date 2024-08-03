//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonValueNumber.cpp
///  @brief  数値型のJsonの値を扱うクラス
///  @author toide
///  @date   2024/06/08 22:50:24
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Private/Json/Include/JsonValueNumberString.hpp"
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

#pragma region GetValue
/*!**********************************************************************
*  @brief     String型の値を取得します.
*  @param[in] void
*  @return    tstring : 文字列の結果
*************************************************************************/
tstring JsonValueNumberString::GetString() const
{
	return _value;
}

/*!**********************************************************************
*  @brief     Int8の値を取得します
*  @param[in] void
*  @return    int8
*************************************************************************/
int8 JsonValueNumberString::GetInt8() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");

	return _value.ToInt8();
}

/*!**********************************************************************
*  @brief     Int16の値を取得します
*  @param[in] void
*  @return    int16
*************************************************************************/
int16 JsonValueNumberString::GetInt16() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	return _value.ToInt16();
}

/*!**********************************************************************
*  @brief     Int32の値を取得します
*  @param[in] void
*  @return    int32
*************************************************************************/
int32 JsonValueNumberString::GetInt32() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	return _value.ToInt32();
}

/*!**********************************************************************
*  @brief     Int64の値を取得します
*  @param[in] void
*  @return    int64
*************************************************************************/
int64 JsonValueNumberString::GetInt64() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	return _value.ToInt64();
}

/*!**********************************************************************
*  @brief     UInt8の値を取得します
*  @param[in] void
*  @return    uint8
*************************************************************************/
uint8 JsonValueNumberString::GetUInt8() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	return _value.ToUInt8();
}

/*!**********************************************************************
*  @brief     UInt16の値を取得します
*  @param[in] void
*  @return    uint16
*************************************************************************/
uint16 JsonValueNumberString::GetUInt16() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	return _value.ToUInt16();
}

/*!**********************************************************************
*  @brief     UInt32の値を取得します
*  @param[in] void
*  @return    uint32
*************************************************************************/
uint32 JsonValueNumberString::GetUInt32() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	return _value.ToUInt32();
}

/*!**********************************************************************
*  @brief     UInt64の値を取得します
*  @param[in] void
*  @return    uint64
*************************************************************************/
uint64 JsonValueNumberString::GetUInt64() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	return _value.ToUInt64();
}

/*!**********************************************************************
*  @brief     Floatの値を取得します
*  @param[in] void
*  @return    float
*************************************************************************/
float JsonValueNumberString::GetFloat() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	return _value.ToFloat();
}

/*!**********************************************************************
*  @brief     Doubleの値を取得します
*  @param[in] void
*  @return    double
*************************************************************************/
double JsonValueNumberString::GetDouble() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	return _value.ToDouble();
}
#pragma endregion GetValue