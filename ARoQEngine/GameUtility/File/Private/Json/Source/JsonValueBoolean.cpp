//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonValueBoolean.cpp
///  @brief  Boolean型のJson値を扱うクラス
///  @author toide
///  @date   2024/06/09 1:35:22
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Private/Json/Include/JsonValueBoolean.hpp"
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
tstring JsonBoolean::GetString() const
{
	return _value ? SP("true") : SP("false");
}
/*!**********************************************************************
*  @brief     boolの値を取得します
*  @param[in] void
*  @return    int8
*************************************************************************/
bool JsonBoolean::GetBool() const
{
	Checkf(_type == JsonValueType::True || _type == JsonValueType::False, "Json type is not boolean");
	return _value;
}

/*!**********************************************************************
*  @brief     Int8の値を取得します
*  @param[in] void
*  @return    int8
*************************************************************************/
int8 JsonBoolean::GetInt8() const
{
	Checkf(_type == JsonValueType::True || _type == JsonValueType::False, "Json type is not boolean");
	return static_cast<int8>(_value);
}

/*!**********************************************************************
*  @brief     Int16の値を取得します
*  @param[in] void
*  @return    int16
*************************************************************************/
int16 JsonBoolean::GetInt16() const
{
	Checkf(_type == JsonValueType::True || _type == JsonValueType::False, "Json type is not boolean");
	return static_cast<int16>(_value);
}

/*!**********************************************************************
*  @brief     Int32の値を取得します
*  @param[in] void
*  @return    int32
*************************************************************************/
int32 JsonBoolean::GetInt32() const
{
	Checkf(_type == JsonValueType::True || _type == JsonValueType::False, "Json type is not boolean");
	return static_cast<int32>(_value);
}

/*!**********************************************************************
*  @brief     Int64の値を取得します
*  @param[in] void
*  @return    int64
*************************************************************************/
int64 JsonBoolean::GetInt64() const
{
	Checkf(_type == JsonValueType::True || _type == JsonValueType::False, "Json type is not boolean");
	return static_cast<int64>(_value);
}

/*!**********************************************************************
*  @brief     UInt8の値を取得します
*  @param[in] void
*  @return    uint8
*************************************************************************/
uint8 JsonBoolean::GetUInt8() const
{
	Checkf(_type == JsonValueType::True || _type == JsonValueType::False, "Json type is not boolean");
	return static_cast<uint8>(_value);
}

/*!**********************************************************************
*  @brief     UInt16の値を取得します
*  @param[in] void
*  @return    uint16
*************************************************************************/
uint16 JsonBoolean::GetUInt16() const
{
	Checkf(_type == JsonValueType::True || _type == JsonValueType::False, "Json type is not boolean");
	return static_cast<uint16>(_value);
}

/*!**********************************************************************
*  @brief     UInt32の値を取得します
*  @param[in] void
*  @return    uint32
*************************************************************************/
uint32 JsonBoolean::GetUInt32() const
{
	Checkf(_type == JsonValueType::True || _type == JsonValueType::False, "Json type is not boolean");
	return static_cast<uint32>(_value);
}

/*!**********************************************************************
*  @brief     UInt64の値を取得します
*  @param[in] void
*  @return    uint64
*************************************************************************/
uint64 JsonBoolean::GetUInt64() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	return static_cast<uint64>(_value);
}

/*!**********************************************************************
*  @brief     Floatの値を取得します
*  @param[in] void
*  @return    float
*************************************************************************/
float JsonBoolean::GetFloat() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	return static_cast<float>(_value);

}

/*!**********************************************************************
*  @brief     Doubleの値を取得します
*  @param[in] void
*  @return    double
*************************************************************************/
double JsonBoolean::GetDouble() const
{
	Checkf(_type == JsonValueType::Number, "Json type is not number");
	return static_cast<double>(_value);
}
#pragma endregion GetValue