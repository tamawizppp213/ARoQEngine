//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsAtomic.cpp
///  @brief  temp
///  @author toide
///  @date   2024/06/22 19:56:34
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Windows/Include/WindowsAtomic.hpp"
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
*  @brief     int8�^���A�g�~�b�N�ɃC���N�������g���܂�.
*  @param[in] volatile gu::int8* value : �C���N�������g�������l
*  @return    gu::int8 : �C���N�������g��̒l
*************************************************************************/
gu::int8 CoreAtomic::Increment(volatile gu::int8* value)
{
	return ::_InterlockedExchange8((char*)value, 1) + 1;
}

/*!**********************************************************************
*  @brief     int16�^���A�g�~�b�N�ɃC���N�������g���܂�.
*  @param[in] volatile gu::int16* value : �C���N�������g�������l
*  @return    gu::int16 : �C���N�������g��̒l
*************************************************************************/
gu::int16 CoreAtomic::Increment(volatile gu::int16* value)
{
	return ::_InterlockedIncrement16((short*)value);
}

/*!**********************************************************************
*  @brief     int32�^���A�g�~�b�N�ɃC���N�������g���܂�.
*  @param[in] volatile gu::int32* value : �C���N�������g�������l
*  @return    gu::int32 : �C���N�������g��̒l
*************************************************************************/
gu::int32 CoreAtomic::Increment(volatile gu::int32* value)
{
	return ::_InterlockedIncrement((long*)value);
}

/*!**********************************************************************
*  @brief     int64�^���A�g�~�b�N�ɃC���N�������g���܂�.
*  @param[in] volatile gu::int64* value : �C���N�������g�������l
*  @return    gu::int64 : �C���N�������g��̒l
*************************************************************************/
gu::int64 CoreAtomic::Increment(volatile gu::int64* value)
{
	return ::_InterlockedIncrement64((long long*)value);
}

/*!**********************************************************************
*  @brief     int8�^���A�g�~�b�N�Ƀf�N�������g���܂�.
*  @param[in] volatile gu::int8* value : �f�N�������g�������l
*  @return    gu::int8 : �f�N�������g��̒l
* ************************************************************************/
gu::int8 CoreAtomic::Decrement(volatile gu::int8* value)
{
	return ::_InterlockedExchangeAdd8((char*)value, -1) - 1;
}

/*!**********************************************************************
*  @brief     int16�^���A�g�~�b�N�Ƀf�N�������g���܂�.
*  @param[in] volatile gu::int16* value : �f�N�������g�������l
*  @return    gu::int16 : �f�N�������g��̒l
* ************************************************************************/
gu::int16 CoreAtomic::Decrement(volatile gu::int16* value)
{
	return ::_InterlockedDecrement16((short*)value);
}

/*!**********************************************************************
*  @brief     int32�^���A�g�~�b�N�Ƀf�N�������g���܂�.
*  @param[in] volatile gu::int32* value : �f�N�������g�������l
*  @return    gu::int32 : �f�N�������g��̒l
* ************************************************************************/
gu::int32 CoreAtomic::Decrement(volatile gu::int32* value)
{
	return ::_InterlockedDecrement((long*)value);
}

/*!**********************************************************************
*  @brief     int64�^���A�g�~�b�N�Ƀf�N�������g���܂�.
*  @param[in] volatile gu::int64* value : �f�N�������g�������l
*  @return    gu::int64 : �f�N�������g��̒l
* ************************************************************************/
gu::int64 CoreAtomic::Decrement(volatile gu::int64* value)
{
	return ::_InterlockedDecrement64((long long*)value);
}

/*!**********************************************************************
*  @brief     int8�^���A�g�~�b�N�ɉ��Z���܂�.
*  @param[in] volatile gu::int8* value : ���Z�������l
*  @param[in] gu::int8 addValue : ���Z�������l
*  @return    gu::int8 : ���Z��̒l
* ************************************************************************/
gu::int8 CoreAtomic::Add(volatile gu::int8* value, gu::int8 addValue)
{
	return ::_InterlockedExchangeAdd8((char*)value, addValue);
}

/*!**********************************************************************
*  @brief     int16�^���A�g�~�b�N�ɉ��Z���܂�.
*  @param[in] volatile gu::int16* value : ���Z�������l
*  @param[in] gu::int16 addValue : ���Z�������l
*  @return    gu::int16 : ���Z��̒l
* ************************************************************************/
gu::int16 CoreAtomic::Add(volatile gu::int16* value, gu::int16 addValue)
{
	return ::_InterlockedExchangeAdd16((short*)value, addValue);
}

/*!**********************************************************************
*  @brief     int32�^���A�g�~�b�N�ɉ��Z���܂�.
*  @param[in] volatile gu::int32* value : ���Z�������l
*  @param[in] gu::int32 addValue : ���Z�������l
*  @return    gu::int32 : ���Z��̒l
* ************************************************************************/
gu::int32 CoreAtomic::Add(volatile gu::int32* value, gu::int32 addValue)
{
	return ::_InterlockedExchangeAdd((long*)value, addValue);
}

/*!**********************************************************************
*  @brief     int64�^���A�g�~�b�N�ɉ��Z���܂�.
*  @param[in] volatile gu::int64* value : ���Z�������l
*  @param[in] gu::int64 addValue : ���Z�������l
*  @return    gu::int64 : ���Z��̒l
* ************************************************************************/
gu::int64 CoreAtomic::Add(volatile gu::int64* value, gu::int64 addValue)
{
	return ::_InterlockedExchangeAdd64((long long*)value, addValue);
}

/*!**********************************************************************
*  @brief     int8�^���A�g�~�b�N�Ɍ��Z���܂�.
*  @param[in] volatile gu::int8* value : ���Z�������l
*  @param[in] gu::int8 subValue : ���Z�������l
*  @return    gu::int8 : ���Z��̒l
* ************************************************************************/
gu::int8 CoreAtomic::Subtract(volatile gu::int8* value, gu::int8 subValue)
{
	return ::_InterlockedExchangeAdd8((char*)value, -subValue);
}

/*!**********************************************************************
*  @brief     int16�^���A�g�~�b�N�Ɍ��Z���܂�.
*  @param[in] volatile gu::int16* value : ���Z�������l
*  @param[in] gu::int16 subValue : ���Z�������l
*  @return    gu::int16 : ���Z��̒l
* ************************************************************************/
gu::int16 CoreAtomic::Subtract(volatile gu::int16* value, gu::int16 subValue)
{
	return ::_InterlockedExchangeAdd16((short*)value, -subValue);
}

/*!**********************************************************************
*  @brief     int32�^���A�g�~�b�N�Ɍ��Z���܂�.
*  @param[in] volatile gu::int32* value : ���Z�������l
*  @param[in] gu::int32 subValue : ���Z�������l
*  @return    gu::int32 : ���Z��̒l
* ************************************************************************/
gu::int32 CoreAtomic::Subtract(volatile gu::int32* value, gu::int32 subValue)
{
	return ::_InterlockedExchangeAdd((long*)value, -subValue);
}

/*!**********************************************************************
*  @brief     int64�^���A�g�~�b�N�Ɍ��Z���܂�.
* @param[in] volatile gu::int64* value : ���Z�������l
* @param[in] gu::int64 subValue : ���Z�������l
* @return    gu::int64 : ���Z��̒l
* ************************************************************************/
gu::int64 CoreAtomic::Subtract(volatile gu::int64* value, gu::int64 subValue)
{
	return ::_InterlockedExchangeAdd64((long long*)value, -subValue);
}

/*!**********************************************************************
*  @brief        int8�^���A�g�~�b�N�ɔ�r���Č������܂�.
*  @param[inout] volatile gu::int8* value : ��r�������l
*  @param[in]    gu::int8 exchangeValue : �����������l
*  @param[in]    gu::int8 comparandValue : ��r�������l
*  @return       gu::int8 : �����O�̒l
* ************************************************************************/
gu::int8 CoreAtomic::CompareExchange(volatile gu::int8* value, gu::int8 exchangeValue, gu::int8 comparandValue)
{
	return ::_InterlockedCompareExchange8((char*)value, exchangeValue, comparandValue);
}

/*!**********************************************************************
*  @brief        int16�^���A�g�~�b�N�ɔ�r���Č������܂�.
*  @param[inout] volatile gu::int16* value : ��r�������l
*  @param[in]    gu::int16 exchangeValue : �����������l
*  @param[in]    gu::int16 comparandValue : ��r�������l
*  @return       gu::int16 : �����O�̒l
* ************************************************************************/
gu::int16 CoreAtomic::CompareExchange(volatile gu::int16* value, gu::int16 exchangeValue, gu::int16 comparandValue)
{
	return ::_InterlockedCompareExchange16((short*)value, exchangeValue, comparandValue);
}

/*!**********************************************************************
*  @brief        int32�^���A�g�~�b�N�ɔ�r���Č������܂�.
*  @param[inout] volatile gu::int32* value : ��r�������l
*  @param[in]    gu::int32 exchangeValue : �����������l
*  @param[in]    gu::int32 comparandValue : ��r�������l
*  @return       gu::int32 : �����O�̒l
* ************************************************************************/
gu::int32 CoreAtomic::CompareExchange(volatile gu::int32* value, gu::int32 exchangeValue, gu::int32 comparandValue)
{
	return ::_InterlockedCompareExchange((long*)value, exchangeValue, comparandValue);
}

/*!**********************************************************************
*  @brief        int64�^���A�g�~�b�N�ɔ�r���Č������܂�.
*  @param[inout] volatile gu::int64* value : ��r�������l
*  @param[in]    gu::int64 exchangeValue : �����������l
*  @param[in]    gu::int64 comparandValue : ��r�������l
*  @return       gu::int64 : �����O�̒l
* ************************************************************************/
gu::int64 CoreAtomic::CompareExchange(volatile gu::int64* value, gu::int64 exchangeValue, gu::int64 comparandValue)
{
	return ::_InterlockedCompareExchange64((long long*)value, exchangeValue, comparandValue);
}

/*!**********************************************************************
*  @brief        int8�^���A�g�~�b�N�ɔ�r���Č������܂�.
*  @param[inout] volatile gu::int8* value : ��r�������l
*  @param[in]    gu::int8 exchangeValue : �����������l
*  @return       gu::int8 : �����O�̒l
* ************************************************************************/
gu::int8 CoreAtomic::Exchange(volatile gu::int8* value, gu::int8 exchangeValue)
{
	return ::_InterlockedExchange8((char*)value, exchangeValue);
}

/*!**********************************************************************
*  @brief        int16�^���A�g�~�b�N�ɔ�r���Č������܂�.
*  @param[inout] volatile gu::int16* value : ��r�������l
*  @param[in]    gu::int16 exchangeValue : �����������l
*  @return       gu::int16 : �����O�̒l
* ************************************************************************/
gu::int16 CoreAtomic::Exchange(volatile gu::int16* value, gu::int16 exchangeValue)
{
	return ::_InterlockedExchange16((short*)value, exchangeValue);

}

/*!**********************************************************************
*  @brief        int32�^���A�g�~�b�N�ɔ�r���Č������܂�.
*  @param[inout] volatile gu::int32* value : ��r�������l
*  @param[in]    gu::int32 exchangeValue : �����������l
*  @return       gu::int32 : �����O�̒l
* ************************************************************************/
gu::int32 CoreAtomic::Exchange(volatile gu::int32* value, gu::int32 exchangeValue)
{
	return ::_InterlockedExchange((long*)value, exchangeValue);
}

/*!**********************************************************************
*  @brief        int64�^���A�g�~�b�N�ɔ�r���Č������܂�.
*  @param[inout] volatile gu::int64* value : ��r�������l
*  @param[in]    gu::int64 exchangeValue : �����������l
*  @return       gu::int64 : �����O�̒l
* ************************************************************************/
gu::int64 CoreAtomic::Exchange(volatile gu::int64* value, gu::int64 exchangeValue)
{
	return ::_InterlockedExchange64((long long*)value, exchangeValue);
}

/*!**********************************************************************
*  @brief        int8�^���A�g�~�b�N��And���Z���܂�.
*  @param[inout] volatile gu::int8* destination : ���͒l��And���Z�̌���
*  @param[in]    gu::int8 value : And���Z�������l
*  @return       gu::int8 : ���X�̓��͒l
* ************************************************************************/
gu::int8 CoreAtomic::And(volatile gu::int8* destination, gu::int8 value)
{
	return ::_InterlockedAnd8((volatile char*)destination, value);
}

/*!**********************************************************************
*  @brief        int16�^���A�g�~�b�N��And���Z���܂�.
*  @param[inout] volatile gu::int16* destination : ���͒l��And���Z�̌���
*  @param[in]    gu::int16 value : And���Z�������l
*  @return       gu::int16 : ���X�̓��͒l
* ************************************************************************/
gu::int16 CoreAtomic::And(volatile gu::int16* destination, gu::int16 value)
{
	return ::_InterlockedAnd16((volatile short*)destination, value);
}

/*!**********************************************************************
*  @brief        int32�^���A�g�~�b�N��And���Z���܂�.
*  @param[inout] volatile gu::int32* destination : ���͒l��And���Z�̌���
*  @param[in]    gu::int32 value : And���Z�������l
*  @return       gu::int32 : ���X�̓��͒l
* ************************************************************************/
gu::int32 CoreAtomic::And(volatile gu::int32* destination, gu::int32 value)
{
	return ::_InterlockedAnd((volatile long*)destination, value);
}

/*!**********************************************************************
*  @brief        int64�^���A�g�~�b�N��And���Z���܂�.
*  @param[inout] volatile gu::int64* destination : ���͒l��And���Z�̌���
*  @param[in]    gu::int64 value : And���Z�������l
*  @return       gu::int64 : ���X�̓��͒l
* ************************************************************************/
gu::int64 CoreAtomic::And(volatile gu::int64* destination, gu::int64 value)
{
	return ::_InterlockedAnd64((volatile long long*)destination, value);
}

/*!**********************************************************************
*  @brief        int8�^���A�g�~�b�N��Or���Z���܂�.
*  @param[inout] volatile gu::int8* destination : ���͒l��Or���Z�̌���
*  @param[in]    gu::int8 value : Or���Z�������l
*  @return       gu::int8 : ���X�̓��͒l
* ************************************************************************/
gu::int8 CoreAtomic::Or(volatile gu::int8* destination, gu::int8 value)
{
	return ::_InterlockedOr8((volatile char*)destination, value);
}

/*!**********************************************************************
*  @brief        int16�^���A�g�~�b�N��Or���Z���܂�.
*  @param[inout] volatile gu::int16* destination : ���͒l��Or���Z�̌���
*  @param[in]    gu::int16 value : Or���Z�������l
*  @return       gu::int16 : ���X�̓��͒l
* ************************************************************************/
gu::int16 CoreAtomic::Or(volatile gu::int16* destination, gu::int16 value)
{
	return ::_InterlockedOr16((volatile short*)destination, value);
}

/*!**********************************************************************
*  @brief        int32�^���A�g�~�b�N��Or���Z���܂�.
*  @param[inout] volatile gu::int32* destination : ���͒l��Or���Z�̌���
*  @param[in]    gu::int32 value : Or���Z�������l
*  @return       gu::int32 : ���X�̓��͒l
* ************************************************************************/
gu::int32 CoreAtomic::Or(volatile gu::int32* destination, gu::int32 value)
{
	return ::_InterlockedOr((volatile long*)destination, value);
}

/*!**********************************************************************
*  @brief        int64�^���A�g�~�b�N��Or���Z���܂�.
*  @param[inout] volatile gu::int64* destination : ���͒l��Or���Z�̌���
*  @param[in]    gu::int64 value : Or���Z�������l
*  @return       gu::int64 : ���X�̓��͒l
* ************************************************************************/
gu::int64 CoreAtomic::Or(volatile gu::int64* destination, gu::int64 value)
{
	return ::_InterlockedOr64((volatile long long*)destination, value);
}

/*!**********************************************************************
*  @brief        int8�^���A�g�~�b�N��Xor���Z���܂�.
*  @param[inout] volatile gu::int8* destination : ���͒l��Xor���Z�̌���
*  @param[in]    gu::int8 value : Xor���Z�������l
*  @return       gu::int8 : ���X�̓��͒l
* ************************************************************************/
gu::int8 CoreAtomic::Xor(volatile gu::int8* destination, gu::int8 value)
{
	return ::_InterlockedXor8((volatile char*)destination, value);
}

/*!**********************************************************************
*  @brief        int16�^���A�g�~�b�N��Xor���Z���܂�.
*  @param[inout] volatile gu::int16* destination : ���͒l��Xor���Z�̌���
*  @param[in]    gu::int16 value : Xor���Z�������l
*  @return       gu::int16 : ���X�̓��͒l
* ************************************************************************/
gu::int16 CoreAtomic::Xor(volatile gu::int16* destination, gu::int16 value)
{
	return ::_InterlockedXor16((volatile short*)destination, value);
}

/*!**********************************************************************
*  @brief        int32�^���A�g�~�b�N��Xor���Z���܂�.
*  @param[inout] volatile gu::int32* destination : ���͒l��Xor���Z�̌���
*  @param[in]    gu::int32 value : Xor���Z�������l
*  @return       gu::int32 : ���X�̓��͒l
* ************************************************************************/
gu::int32 CoreAtomic::Xor(volatile gu::int32* destination, gu::int32 value)
{
	return ::_InterlockedXor((volatile long*)destination, value);
}

/*!**********************************************************************
*  @brief        int64�^���A�g�~�b�N��Xor���Z���܂�.
*  @param[inout] volatile gu::int64* destination : ���͒l��Xor���Z�̌���
*  @param[in]    gu::int64 value : Xor���Z�������l
*  @return       gu::int64 : ���X�̓��͒l
* ************************************************************************/
gu::int64 CoreAtomic::Xor(volatile gu::int64* destination, gu::int64 value)
{
	return ::_InterlockedXor64((volatile long long*)destination, value);
}
#pragma endregion Public Function