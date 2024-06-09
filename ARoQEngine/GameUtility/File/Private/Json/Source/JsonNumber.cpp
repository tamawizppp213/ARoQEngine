//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonNumber.cpp
///  @brief  temp
///  @author toide
///  @date   2024/06/08 22:50:24
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Private/Json/Include/JsonNumber.hpp"
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
tstring JsonNumber::GetString() const
{
	return gu::tstring::FromNumber(_value);
}

/*!**********************************************************************
*  @brief     Int8の値を取得します
*  @param[in] void
*  @return    int8
*************************************************************************/
int8 JsonNumber::GetInt8() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	Checkf(MIN_INT8 <= _value && _value <= MAX_INT8, "Json value is out of range");
	return static_cast<int8>(_value);
}

/*!**********************************************************************
*  @brief     Int16の値を取得します
*  @param[in] void
*  @return    int16
*************************************************************************/
int16 JsonNumber::GetInt16() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	Checkf(MIN_INT16 <= _value && _value <= MAX_INT16, "Json value is out of range");
	return static_cast<int16>(_value);
}

/*!**********************************************************************
*  @brief     Int32の値を取得します
*  @param[in] void
*  @return    int32
*************************************************************************/
int32 JsonNumber::GetInt32() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	Checkf(MIN_INT32 <= _value && _value <= MAX_INT32, "Json value is out of range");
	return static_cast<int32>(_value);
}

/*!**********************************************************************
*  @brief     Int64の値を取得します
*  @param[in] void
*  @return    int64
*************************************************************************/
int64 JsonNumber::GetInt64() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	Checkf(MIN_INT64 <= _value && _value <= MAX_INT64, "Json value is out of range");
	return static_cast<int64>(_value);
}

/*!**********************************************************************
*  @brief     UInt8の値を取得します
*  @param[in] void
*  @return    uint8
*************************************************************************/
uint8 JsonNumber::GetUInt8() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	Checkf(MIN_UINT8 <= _value && _value <= MAX_UINT8, "Json value is out of range");
	return static_cast<uint8>(_value);
}

/*!**********************************************************************
*  @brief     UInt16の値を取得します
*  @param[in] void
*  @return    uint16
*************************************************************************/
uint16 JsonNumber::GetUInt16() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	Checkf(MIN_UINT16 <= _value && _value <= MAX_UINT16, "Json value is out of range");
	return static_cast<uint16>(_value);
}

/*!**********************************************************************
*  @brief     UInt32の値を取得します
*  @param[in] void
*  @return    uint32
*************************************************************************/
uint32 JsonNumber::GetUInt32() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	Checkf(MIN_UINT32 <= _value && _value <= MAX_UINT32, "Json value is out of range");
	return static_cast<uint32>(_value);
}

/*!**********************************************************************
*  @brief     UInt64の値を取得します
*  @param[in] void
*  @return    uint64
*************************************************************************/
uint64 JsonNumber::GetUInt64() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	Checkf(MIN_UINT64 <= _value && _value <= MAX_UINT64, "Json value is out of range");
	return static_cast<uint64>(_value);
}

/*!**********************************************************************
*  @brief     Floatの値を取得します
*  @param[in] void
*  @return    float
*************************************************************************/
float JsonNumber::GetFloat() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	Checkf(MIN_FLOAT32 <= _value && _value <= MAX_FLOAT32, "Json value is out of range");
	return static_cast<float>(_value);

}

/*!**********************************************************************
*  @brief     Doubleの値を取得します
*  @param[in] void
*  @return    double
*************************************************************************/
double JsonNumber::GetDouble() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	Checkf(MIN_DOUBLE64 <= _value && _value <= MAX_DOUBLE64, "Json value is out of range");
	return static_cast<double>(_value);
}
#pragma endregion GetValue