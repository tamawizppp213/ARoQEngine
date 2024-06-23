//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsAtomic.cpp
///  @brief  temp
///  @author toide
///  @date   2024/06/22 19:56:34
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Windows/Include/WindowsAtomicOperator.hpp"
#include <intrin.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform::windows;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     メモリバリアを設定します
*  @param[in] void
*  @return    void
*************************************************************************/
void CoreAtomicOperator::ReadWriteBarrier()
{
	::_ReadWriteBarrier();
}

/*!**********************************************************************
*  @brief     Readバリアを設定します
*  @param[in] void
*  @return    void
*************************************************************************/
void CoreAtomicOperator::ReadBarrier()
{
	::_ReadBarrier();
}

/*!**********************************************************************
*  @brief     Readバリアを設定します
*  @param[in] void
*  @return    void
*************************************************************************/
void CoreAtomicOperator::WriteBarrier()
{
	::_WriteBarrier();
}

/*!**********************************************************************
*  @brief     int8型をアトミックにインクリメントします.
*  @param[in] volatile gu::int8* value : インクリメントしたい値
*  @return    gu::int8 : インクリメント後の値
*************************************************************************/
gu::int8 CoreAtomicOperator::Increment(volatile gu::int8* value)
{
	return ::_InterlockedExchange8((char*)value, 1) + 1;
}

/*!**********************************************************************
*  @brief     int16型をアトミックにインクリメントします.
*  @param[in] volatile gu::int16* value : インクリメントしたい値
*  @return    gu::int16 : インクリメント後の値
*************************************************************************/
gu::int16 CoreAtomicOperator::Increment(volatile gu::int16* value)
{
	return ::_InterlockedIncrement16((short*)value);
}

/*!**********************************************************************
*  @brief     int32型をアトミックにインクリメントします.
*  @param[in] volatile gu::int32* value : インクリメントしたい値
*  @return    gu::int32 : インクリメント後の値
*************************************************************************/
gu::int32 CoreAtomicOperator::Increment(volatile gu::int32* value)
{
	return ::_InterlockedIncrement((long*)value);
}

/*!**********************************************************************
*  @brief     int64型をアトミックにインクリメントします.
*  @param[in] volatile gu::int64* value : インクリメントしたい値
*  @return    gu::int64 : インクリメント後の値
*************************************************************************/
gu::int64 CoreAtomicOperator::Increment(volatile gu::int64* value)
{
	return ::_InterlockedIncrement64((long long*)value);
}

/*!**********************************************************************
*  @brief     int8型をアトミックにデクリメントします.
*  @param[in] volatile gu::int8* value : デクリメントしたい値
*  @return    gu::int8 : デクリメント後の値
* ************************************************************************/
gu::int8 CoreAtomicOperator::Decrement(volatile gu::int8* value)
{
	return ::_InterlockedExchangeAdd8((char*)value, -1) - 1;
}

/*!**********************************************************************
*  @brief     int16型をアトミックにデクリメントします.
*  @param[in] volatile gu::int16* value : デクリメントしたい値
*  @return    gu::int16 : デクリメント後の値
* ************************************************************************/
gu::int16 CoreAtomicOperator::Decrement(volatile gu::int16* value)
{
	return ::_InterlockedDecrement16((short*)value);
}

/*!**********************************************************************
*  @brief     int32型をアトミックにデクリメントします.
*  @param[in] volatile gu::int32* value : デクリメントしたい値
*  @return    gu::int32 : デクリメント後の値
* ************************************************************************/
gu::int32 CoreAtomicOperator::Decrement(volatile gu::int32* value)
{
	return ::_InterlockedDecrement((long*)value);
}

/*!**********************************************************************
*  @brief     int64型をアトミックにデクリメントします.
*  @param[in] volatile gu::int64* value : デクリメントしたい値
*  @return    gu::int64 : デクリメント後の値
* ************************************************************************/
gu::int64 CoreAtomicOperator::Decrement(volatile gu::int64* value)
{
	return ::_InterlockedDecrement64((long long*)value);
}

/*!**********************************************************************
*  @brief     int8型をアトミックに加算します.
*  @param[in] volatile gu::int8* value : 加算したい値
*  @param[in] gu::int8 addValue : 加算したい値
*  @return    gu::int8 : 加算後の値
* ************************************************************************/
gu::int8 CoreAtomicOperator::Add(volatile gu::int8* value, gu::int8 addValue)
{
	return ::_InterlockedExchangeAdd8((char*)value, addValue) + addValue;
}

/*!**********************************************************************
*  @brief     int16型をアトミックに加算します.
*  @param[in] volatile gu::int16* value : 加算したい値
*  @param[in] gu::int16 addValue : 加算したい値
*  @return    gu::int16 : 加算後の値
* ************************************************************************/
gu::int16 CoreAtomicOperator::Add(volatile gu::int16* value, gu::int16 addValue)
{
	return ::_InterlockedExchangeAdd16((short*)value, addValue) + addValue;
}

/*!**********************************************************************
*  @brief     int32型をアトミックに加算します.
*  @param[in] volatile gu::int32* value : 加算したい値
*  @param[in] gu::int32 addValue : 加算したい値
*  @return    gu::int32 : 加算後の値
* ************************************************************************/
gu::int32 CoreAtomicOperator::Add(volatile gu::int32* value, gu::int32 addValue)
{
	return ::_InterlockedExchangeAdd((long*)value, addValue) + addValue;
}

/*!**********************************************************************
*  @brief     int64型をアトミックに加算します.
*  @param[in] volatile gu::int64* value : 加算したい値
*  @param[in] gu::int64 addValue : 加算したい値
*  @return    gu::int64 : 加算後の値
* ************************************************************************/
gu::int64 CoreAtomicOperator::Add(volatile gu::int64* value, gu::int64 addValue)
{
	return ::_InterlockedExchangeAdd64((long long*)value, addValue) + addValue;
}

/*!**********************************************************************
*  @brief     int8型をアトミックに減算します.
*  @param[in] volatile gu::int8* value : 減算したい値
*  @param[in] gu::int8 subValue : 減算したい値
*  @return    gu::int8 : 減算後の値
* ************************************************************************/
gu::int8 CoreAtomicOperator::Subtract(volatile gu::int8* value, gu::int8 subValue)
{
	return ::_InterlockedExchangeAdd8((char*)value, -subValue) - subValue;
}

/*!**********************************************************************
*  @brief     int16型をアトミックに減算します.
*  @param[in] volatile gu::int16* value : 減算したい値
*  @param[in] gu::int16 subValue : 減算したい値
*  @return    gu::int16 : 減算後の値
* ************************************************************************/
gu::int16 CoreAtomicOperator::Subtract(volatile gu::int16* value, gu::int16 subValue)
{
	return ::_InterlockedExchangeAdd16((short*)value, -subValue) - subValue;
}

/*!**********************************************************************
*  @brief     int32型をアトミックに減算します.
*  @param[in] volatile gu::int32* value : 減算したい値
*  @param[in] gu::int32 subValue : 減算したい値
*  @return    gu::int32 : 減算後の値
* ************************************************************************/
gu::int32 CoreAtomicOperator::Subtract(volatile gu::int32* value, gu::int32 subValue)
{
	return ::_InterlockedExchangeAdd((long*)value, -subValue) - subValue;
}

/*!**********************************************************************
*  @brief     int64型をアトミックに減算します.
* @param[in] volatile gu::int64* value : 減算したい値
* @param[in] gu::int64 subValue : 減算したい値
* @return    gu::int64 : 減算後の値
* ************************************************************************/
gu::int64 CoreAtomicOperator::Subtract(volatile gu::int64* value, gu::int64 subValue)
{
	return ::_InterlockedExchangeAdd64((long long*)value, -subValue) - subValue;
}

/*!**********************************************************************
*  @brief        int8型をアトミックに比較して交換します.
*  @param[inout] volatile gu::int8* value : 比較したい値
*  @param[in]    gu::int8 exchangeValue : 交換したい値
*  @param[in]    gu::int8 comparandValue : 比較したい値
*  @return       gu::int8 : 交換前の値
* ************************************************************************/
gu::int8 CoreAtomicOperator::CompareExchange(volatile gu::int8* value, gu::int8 exchangeValue, gu::int8 comparandValue)
{
	return ::_InterlockedCompareExchange8((char*)value, exchangeValue, comparandValue);
}

/*!**********************************************************************
*  @brief        int16型をアトミックに比較して交換します.
*  @param[inout] volatile gu::int16* value : 比較したい値
*  @param[in]    gu::int16 exchangeValue : 交換したい値
*  @param[in]    gu::int16 comparandValue : 比較したい値
*  @return       gu::int16 : 交換前の値
* ************************************************************************/
gu::int16 CoreAtomicOperator::CompareExchange(volatile gu::int16* value, gu::int16 exchangeValue, gu::int16 comparandValue)
{
	return ::_InterlockedCompareExchange16((short*)value, exchangeValue, comparandValue);
}

/*!**********************************************************************
*  @brief        int32型をアトミックに比較して交換します.
*  @param[inout] volatile gu::int32* value : 比較したい値
*  @param[in]    gu::int32 exchangeValue : 交換したい値
*  @param[in]    gu::int32 comparandValue : 比較したい値
*  @return       gu::int32 : 交換前の値
* ************************************************************************/
gu::int32 CoreAtomicOperator::CompareExchange(volatile gu::int32* value, gu::int32 exchangeValue, gu::int32 comparandValue)
{
	return ::_InterlockedCompareExchange((long*)value, exchangeValue, comparandValue);
}

/*!**********************************************************************
*  @brief        int64型をアトミックに比較して交換します.
*  @param[inout] volatile gu::int64* value : 比較したい値
*  @param[in]    gu::int64 exchangeValue : 交換したい値
*  @param[in]    gu::int64 comparandValue : 比較したい値
*  @return       gu::int64 : 交換前の値
* ************************************************************************/
gu::int64 CoreAtomicOperator::CompareExchange(volatile gu::int64* value, gu::int64 exchangeValue, gu::int64 comparandValue)
{
	return ::_InterlockedCompareExchange64((long long*)value, exchangeValue, comparandValue);
}

/*!**********************************************************************
*  @brief        int8型をアトミックに比較して交換します.
*  @param[inout] volatile gu::int8* value : 比較したい値
*  @param[in]    gu::int8 exchangeValue : 交換したい値
*  @return       gu::int8 : 交換前の値
* ************************************************************************/
gu::int8 CoreAtomicOperator::Exchange(volatile gu::int8* value, gu::int8 exchangeValue)
{
	return ::_InterlockedExchange8((char*)value, exchangeValue);
}

/*!**********************************************************************
*  @brief        int16型をアトミックに比較して交換します.
*  @param[inout] volatile gu::int16* value : 比較したい値
*  @param[in]    gu::int16 exchangeValue : 交換したい値
*  @return       gu::int16 : 交換前の値
* ************************************************************************/
gu::int16 CoreAtomicOperator::Exchange(volatile gu::int16* value, gu::int16 exchangeValue)
{
	return ::_InterlockedExchange16((short*)value, exchangeValue);

}

/*!**********************************************************************
*  @brief        int32型をアトミックに比較して交換します.
*  @param[inout] volatile gu::int32* value : 比較したい値
*  @param[in]    gu::int32 exchangeValue : 交換したい値
*  @return       gu::int32 : 交換前の値
* ************************************************************************/
gu::int32 CoreAtomicOperator::Exchange(volatile gu::int32* value, gu::int32 exchangeValue)
{
	return ::_InterlockedExchange((long*)value, exchangeValue);
}

/*!**********************************************************************
*  @brief        int64型をアトミックに比較して交換します.
*  @param[inout] volatile gu::int64* value : 比較したい値
*  @param[in]    gu::int64 exchangeValue : 交換したい値
*  @return       gu::int64 : 交換前の値
* ************************************************************************/
gu::int64 CoreAtomicOperator::Exchange(volatile gu::int64* value, gu::int64 exchangeValue)
{
	return ::_InterlockedExchange64((long long*)value, exchangeValue);
}

/*!**********************************************************************
*  @brief        int8型をアトミックにAnd演算します.
*  @param[inout] volatile gu::int8* destination : 入力値とAnd演算の結果
*  @param[in]    gu::int8 value : And演算したい値
*  @return       gu::int8 : 元々の入力値
* ************************************************************************/
gu::int8 CoreAtomicOperator::And(volatile gu::int8* destination, gu::int8 value)
{
	return ::_InterlockedAnd8((volatile char*)destination, value);
}

/*!**********************************************************************
*  @brief        int16型をアトミックにAnd演算します.
*  @param[inout] volatile gu::int16* destination : 入力値とAnd演算の結果
*  @param[in]    gu::int16 value : And演算したい値
*  @return       gu::int16 : 元々の入力値
* ************************************************************************/
gu::int16 CoreAtomicOperator::And(volatile gu::int16* destination, gu::int16 value)
{
	return ::_InterlockedAnd16((volatile short*)destination, value);
}

/*!**********************************************************************
*  @brief        int32型をアトミックにAnd演算します.
*  @param[inout] volatile gu::int32* destination : 入力値とAnd演算の結果
*  @param[in]    gu::int32 value : And演算したい値
*  @return       gu::int32 : 元々の入力値
* ************************************************************************/
gu::int32 CoreAtomicOperator::And(volatile gu::int32* destination, gu::int32 value)
{
	return ::_InterlockedAnd((volatile long*)destination, value);
}

/*!**********************************************************************
*  @brief        int64型をアトミックにAnd演算します.
*  @param[inout] volatile gu::int64* destination : 入力値とAnd演算の結果
*  @param[in]    gu::int64 value : And演算したい値
*  @return       gu::int64 : 元々の入力値
* ************************************************************************/
gu::int64 CoreAtomicOperator::And(volatile gu::int64* destination, gu::int64 value)
{
	return ::_InterlockedAnd64((volatile long long*)destination, value);
}

/*!**********************************************************************
*  @brief        int8型をアトミックにOr演算します.
*  @param[inout] volatile gu::int8* destination : 入力値とOr演算の結果
*  @param[in]    gu::int8 value : Or演算したい値
*  @return       gu::int8 : 元々の入力値
* ************************************************************************/
gu::int8 CoreAtomicOperator::Or(volatile gu::int8* destination, gu::int8 value)
{
	return ::_InterlockedOr8((volatile char*)destination, value);
}

/*!**********************************************************************
*  @brief        int16型をアトミックにOr演算します.
*  @param[inout] volatile gu::int16* destination : 入力値とOr演算の結果
*  @param[in]    gu::int16 value : Or演算したい値
*  @return       gu::int16 : 元々の入力値
* ************************************************************************/
gu::int16 CoreAtomicOperator::Or(volatile gu::int16* destination, gu::int16 value)
{
	return ::_InterlockedOr16((volatile short*)destination, value);
}

/*!**********************************************************************
*  @brief        int32型をアトミックにOr演算します.
*  @param[inout] volatile gu::int32* destination : 入力値とOr演算の結果
*  @param[in]    gu::int32 value : Or演算したい値
*  @return       gu::int32 : 元々の入力値
* ************************************************************************/
gu::int32 CoreAtomicOperator::Or(volatile gu::int32* destination, gu::int32 value)
{
	return ::_InterlockedOr((volatile long*)destination, value);
}

/*!**********************************************************************
*  @brief        int64型をアトミックにOr演算します.
*  @param[inout] volatile gu::int64* destination : 入力値とOr演算の結果
*  @param[in]    gu::int64 value : Or演算したい値
*  @return       gu::int64 : 元々の入力値
* ************************************************************************/
gu::int64 CoreAtomicOperator::Or(volatile gu::int64* destination, gu::int64 value)
{
	return ::_InterlockedOr64((volatile long long*)destination, value);
}

/*!**********************************************************************
*  @brief        int8型をアトミックにXor演算します.
*  @param[inout] volatile gu::int8* destination : 入力値とXor演算の結果
*  @param[in]    gu::int8 value : Xor演算したい値
*  @return       gu::int8 : 元々の入力値
* ************************************************************************/
gu::int8 CoreAtomicOperator::Xor(volatile gu::int8* destination, gu::int8 value)
{
	return ::_InterlockedXor8((volatile char*)destination, value);
}

/*!**********************************************************************
*  @brief        int16型をアトミックにXor演算します.
*  @param[inout] volatile gu::int16* destination : 入力値とXor演算の結果
*  @param[in]    gu::int16 value : Xor演算したい値
*  @return       gu::int16 : 元々の入力値
* ************************************************************************/
gu::int16 CoreAtomicOperator::Xor(volatile gu::int16* destination, gu::int16 value)
{
	return ::_InterlockedXor16((volatile short*)destination, value);
}

/*!**********************************************************************
*  @brief        int32型をアトミックにXor演算します.
*  @param[inout] volatile gu::int32* destination : 入力値とXor演算の結果
*  @param[in]    gu::int32 value : Xor演算したい値
*  @return       gu::int32 : 元々の入力値
* ************************************************************************/
gu::int32 CoreAtomicOperator::Xor(volatile gu::int32* destination, gu::int32 value)
{
	return ::_InterlockedXor((volatile long*)destination, value);
}

/*!**********************************************************************
*  @brief        int64型をアトミックにXor演算します.
*  @param[inout] volatile gu::int64* destination : 入力値とXor演算の結果
*  @param[in]    gu::int64 value : Xor演算したい値
*  @return       gu::int64 : 元々の入力値
* ************************************************************************/
gu::int64 CoreAtomicOperator::Xor(volatile gu::int64* destination, gu::int64 value)
{
	return ::_InterlockedXor64((volatile long long*)destination, value);
}
#pragma endregion Public Function