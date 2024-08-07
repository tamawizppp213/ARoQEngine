//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonValueArray.cpp
///  @brief  配列型のJsonの値を扱うクラス
///  @author toide
///  @date   2024/06/09 23:12:53
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Private/Json/Include/JsonValueArray.hpp"
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
/*!**********************************************************************
*  @brief     Array型の値を取得します.
*  @param[in] void
*  @return    DynamicArray<SharedPointer<JsonValue>>
*************************************************************************/
DynamicArray<SharedPointer<JsonValue>>& JsonValueArray::GetArray()
{
	Checkf(_type == JsonValueType::Array, "Json type is not array type");

	return _array;
}

/*!**********************************************************************
*  @brief     Array型の値を取得します.
*  @param[in] void
*  @return    DynamicArray<SharedPointer<JsonValue>>
*************************************************************************/
const DynamicArray<SharedPointer<JsonValue>>& JsonValueArray::GetArray() const
{
	Checkf(_type == JsonValueType::Array, "Json type is not array type");

	return _array;
}

#pragma endregion Public Function