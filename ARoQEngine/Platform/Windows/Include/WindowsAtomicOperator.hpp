//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsAtomics.hpp
///  @brief  実行順序を保証するためのアトミック操作を提供します.@n
///  @author toide
///  @date   2024/06/22 12:39:28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_ATOMIC_HPP
#define WINDOWS_ATOMIC_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{

	/****************************************************************************
	*				  			   WindowsAtomics
	*************************************************************************//**
	/*  @brief  実行順序を保証するためのアトミック操作を提供します.@n
	*****************************************************************************/
	class CoreAtomicOperator : public gu::NonCopyAndMove
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     Read/Writeバリアを設定します
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		static void ReadWriteBarrier();

		/*!**********************************************************************
		*  @brief     Readバリアを設定します
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		static void ReadBarrier();

		/*!**********************************************************************
		*  @brief     Readバリアを設定します
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		static void WriteBarrier();


		/*!**********************************************************************
		*  @brief     int8型をアトミックにインクリメントします.
		*  @param[in] volatile gu::int8* value : インクリメントしたい値
		*  @return    gu::int8 : インクリメント後の値
		*************************************************************************/
		static gu::int8 Increment(volatile gu::int8* value);

		/*!**********************************************************************
		*  @brief     int16型をアトミックにインクリメントします.
		*  @param[in] volatile gu::int16* value : インクリメントしたい値
		*  @return    gu::int16 : インクリメント後の値
		*************************************************************************/
		static gu::int16 Increment(volatile gu::int16* value);

		/*!**********************************************************************
		*  @brief     int32型をアトミックにインクリメントします.
		*  @param[in] volatile gu::int32* value : インクリメントしたい値
		*  @return    gu::int32 : インクリメント後の値
		*************************************************************************/
		static gu::int32 Increment(volatile gu::int32* value);

		/*!**********************************************************************
		*  @brief     int64型をアトミックにインクリメントします.
		*  @param[in] volatile gu::int64* value : インクリメントしたい値
		*  @return    gu::int64 : インクリメント後の値
		*************************************************************************/
		static gu::int64 Increment(volatile gu::int64* value);

		/*!**********************************************************************
		*  @brief     int8型をアトミックにデクリメントします.
		*  @param[in] volatile gu::int8* value : デクリメントしたい値
		*  @return    gu::int8 : デクリメント後の値
		* ************************************************************************/
		static gu::int8 Decrement(volatile gu::int8* value);

		/*!**********************************************************************
		*  @brief     int16型をアトミックにデクリメントします.
		*  @param[in] volatile gu::int16* value : デクリメントしたい値
		*  @return    gu::int16 : デクリメント後の値
		* ************************************************************************/
		static gu::int16 Decrement(volatile gu::int16* value);

		/*!**********************************************************************
		*  @brief     int32型をアトミックにデクリメントします.
		*  @param[in] volatile gu::int32* value : デクリメントしたい値
		*  @return    gu::int32 : デクリメント後の値
		* ************************************************************************/
		static gu::int32 Decrement(volatile gu::int32* value);

		/*!**********************************************************************
		*  @brief     int64型をアトミックにデクリメントします.
		*  @param[in] volatile gu::int64* value : デクリメントしたい値
		*  @return    gu::int64 : デクリメント後の値
		* ************************************************************************/
		static gu::int64 Decrement(volatile gu::int64* value);

		/*!**********************************************************************
		*  @brief     int8型をアトミックに加算します.
		*  @param[in] volatile gu::int8* value : 加算したい値
		*  @param[in] gu::int8 addValue : 加算したい値
		*  @return    gu::int8 : 加算後の値
		* ************************************************************************/
		static gu::int8 Add(volatile gu::int8* value, gu::int8 addValue);
		
		/*!**********************************************************************
		*  @brief     int16型をアトミックに加算します.
		*  @param[in] volatile gu::int16* value : 加算したい値
		*  @param[in] gu::int16 addValue : 加算したい値
		*  @return    gu::int16 : 加算後の値
		* ************************************************************************/
		static gu::int16 Add(volatile gu::int16* value, gu::int16 addValue);

		/*!**********************************************************************
		*  @brief     int32型をアトミックに加算します.
		*  @param[in] volatile gu::int32* value : 加算したい値
		*  @param[in] gu::int32 addValue : 加算したい値
		*  @return    gu::int32 : 加算後の値
		* ************************************************************************/
		static gu::int32 Add(volatile gu::int32* value, gu::int32 addValue);

		/*!**********************************************************************
		*  @brief     int64型をアトミックに加算します.
		*  @param[in] volatile gu::int64* value : 加算したい値
		*  @param[in] gu::int64 addValue : 加算したい値
		*  @return    gu::int64 : 加算後の値
		* ************************************************************************/
		static gu::int64 Add(volatile gu::int64* value, gu::int64 addValue);

		/*!**********************************************************************
		*  @brief     int8型をアトミックに減算します.
		*  @param[in] volatile gu::int8* value : 減算したい値
		*  @param[in] gu::int8 subValue : 減算したい値
		*  @return    gu::int8 : 減算後の値
		* ************************************************************************/
		static gu::int8 Subtract(volatile gu::int8* value, gu::int8 subValue);

		/*!**********************************************************************
		*  @brief     int16型をアトミックに減算します.
		*  @param[in] volatile gu::int16* value : 減算したい値
		*  @param[in] gu::int16 subValue : 減算したい値
		*  @return    gu::int16 : 減算後の値
		* ************************************************************************/
		static gu::int16 Subtract(volatile gu::int16* value, gu::int16 subValue);

		/*!**********************************************************************
		*  @brief     int32型をアトミックに減算します.
		*  @param[in] volatile gu::int32* value : 減算したい値
		*  @param[in] gu::int32 subValue : 減算したい値
		*  @return    gu::int32 : 減算後の値
		* ************************************************************************/
		static gu::int32 Subtract(volatile gu::int32* value, gu::int32 subValue);

		/*!**********************************************************************
		*  @brief     int64型をアトミックに減算します.
		* @param[in] volatile gu::int64* value : 減算したい値
		* @param[in] gu::int64 subValue : 減算したい値
		* @return    gu::int64 : 減算後の値
		* ************************************************************************/
		static gu::int64 Subtract(volatile gu::int64* value, gu::int64 subValue);

		/*!**********************************************************************
		*  @brief        int8型をアトミックに比較して交換します.
		*  @param[inout] volatile gu::int8* value : 比較したい値
		*  @param[in]    gu::int8 exchangeValue : 交換したい値
		*  @param[in]    gu::int8 comparandValue : 比較したい値
		*  @return       gu::int8 : 交換前の値
		* ************************************************************************/
		static gu::int8 CompareExchange(volatile gu::int8* value, gu::int8 exchangeValue, gu::int8 comparandValue);

		/*!**********************************************************************
		*  @brief        int16型をアトミックに比較して交換します.
		*  @param[inout] volatile gu::int16* value : 比較したい値
		*  @param[in]    gu::int16 exchangeValue : 交換したい値
		*  @param[in]    gu::int16 comparandValue : 比較したい値
		*  @return       gu::int16 : 交換前の値
		* ************************************************************************/
		static gu::int16 CompareExchange(volatile gu::int16* value, gu::int16 exchangeValue, gu::int16 comparandValue);

		/*!**********************************************************************
		*  @brief        int32型をアトミックに比較して交換します.
		*  @param[inout] volatile gu::int32* value : 比較したい値
		*  @param[in]    gu::int32 exchangeValue : 交換したい値
		*  @param[in]    gu::int32 comparandValue : 比較したい値
		*  @return       gu::int32 : 交換前の値
		* ************************************************************************/
		static gu::int32 CompareExchange(volatile gu::int32* value, gu::int32 exchangeValue, gu::int32 comparandValue);

		/*!**********************************************************************
		*  @brief        int64型をアトミックに比較して交換します.
		*  @param[inout] volatile gu::int64* value : 比較したい値
		*  @param[in]    gu::int64 exchangeValue : 交換したい値
		*  @param[in]    gu::int64 comparandValue : 比較したい値
		*  @return       gu::int64 : 交換前の値
		* ************************************************************************/
		static gu::int64 CompareExchange(volatile gu::int64* value, gu::int64 exchangeValue, gu::int64 comparandValue);

		/*!**********************************************************************
		*  @brief        int8型をアトミックに比較して交換します.
		*  @param[inout] volatile gu::int8* value : 比較したい値
		*  @param[in]    gu::int8 exchangeValue : 交換したい値
		*  @return       gu::int8 : 交換前の値
		* ************************************************************************/
		static gu::int8 Exchange(volatile gu::int8* value, gu::int8 exchangeValue);

		/*!**********************************************************************
		*  @brief        int16型をアトミックに比較して交換します.
		*  @param[inout] volatile gu::int16* value : 比較したい値
		*  @param[in]    gu::int16 exchangeValue : 交換したい値
		*  @return       gu::int16 : 交換前の値
		* ************************************************************************/
		static gu::int16 Exchange(volatile gu::int16* value, gu::int16 exchangeValue);

		/*!**********************************************************************
		*  @brief        int32型をアトミックに比較して交換します.
		*  @param[inout] volatile gu::int32* value : 比較したい値
		*  @param[in]    gu::int32 exchangeValue : 交換したい値
		*  @return       gu::int32 : 交換前の値
		* ************************************************************************/
		static gu::int32 Exchange(volatile gu::int32* value, gu::int32 exchangeValue);

		/*!**********************************************************************
		*  @brief        int64型をアトミックに比較して交換します.
		*  @param[inout] volatile gu::int64* value : 比較したい値
		*  @param[in]    gu::int64 exchangeValue : 交換したい値
		*  @return       gu::int64 : 交換前の値
		* ************************************************************************/
		static gu::int64 Exchange(volatile gu::int64* value, gu::int64 exchangeValue);

		/*!**********************************************************************
		*  @brief        int8型をアトミックにAnd演算します.
		*  @param[inout] volatile gu::int8* destination : 入力値とAnd演算の結果
		*  @param[in]    gu::int8 value : And演算したい値
		*  @return       gu::int8 : 元々の入力値
		* ************************************************************************/
		static gu::int8 And(volatile gu::int8* destination, gu::int8 value);

		/*!**********************************************************************
		*  @brief        int16型をアトミックにAnd演算します.
		*  @param[inout] volatile gu::int16* destination : 入力値とAnd演算の結果
		*  @param[in]    gu::int16 value : And演算したい値
		*  @return       gu::int16 : 元々の入力値
		* ************************************************************************/
		static gu::int16 And(volatile gu::int16* destination, gu::int16 value);
		
		/*!**********************************************************************
		*  @brief        int32型をアトミックにAnd演算します.
		*  @param[inout] volatile gu::int32* destination : 入力値とAnd演算の結果
		*  @param[in]    gu::int32 value : And演算したい値
		*  @return       gu::int32 : 元々の入力値
		* ************************************************************************/
		static gu::int32 And(volatile gu::int32* destination, gu::int32 value);
		
		/*!**********************************************************************
		*  @brief        int64型をアトミックにAnd演算します.
		*  @param[inout] volatile gu::int64* destination : 入力値とAnd演算の結果
		*  @param[in]    gu::int64 value : And演算したい値
		*  @return       gu::int64 : 元々の入力値
		* ************************************************************************/
		static gu::int64 And(volatile gu::int64* destination, gu::int64 value);

		/*!**********************************************************************
		*  @brief        int8型をアトミックにOr演算します.
		*  @param[inout] volatile gu::int8* destination : 入力値とOr演算の結果
		*  @param[in]    gu::int8 value : Or演算したい値
		*  @return       gu::int8 : 元々の入力値
		* ************************************************************************/
		static gu::int8 Or(volatile gu::int8* destination, gu::int8 value);

		/*!**********************************************************************
		*  @brief        int16型をアトミックにOr演算します.
		*  @param[inout] volatile gu::int16* destination : 入力値とOr演算の結果
		*  @param[in]    gu::int16 value : Or演算したい値
		*  @return       gu::int16 : 元々の入力値
		* ************************************************************************/
		static gu::int16 Or(volatile gu::int16* destination, gu::int16 value);

		/*!**********************************************************************
		*  @brief        int32型をアトミックにOr演算します.
		*  @param[inout] volatile gu::int32* destination : 入力値とOr演算の結果
		*  @param[in]    gu::int32 value : Or演算したい値
		*  @return       gu::int32 : 元々の入力値
		* ************************************************************************/
		static gu::int32 Or(volatile gu::int32* destination, gu::int32 value);

		/*!**********************************************************************
		*  @brief        int64型をアトミックにOr演算します.
		*  @param[inout] volatile gu::int64* destination : 入力値とOr演算の結果
		*  @param[in]    gu::int64 value : Or演算したい値
		*  @return       gu::int64 : 元々の入力値
		* ************************************************************************/
		static gu::int64 Or(volatile gu::int64* destination, gu::int64 value);

		/*!**********************************************************************
		*  @brief        int8型をアトミックにXor演算します.
		*  @param[inout] volatile gu::int8* destination : 入力値とXor演算の結果
		*  @param[in]    gu::int8 value : Xor演算したい値
		*  @return       gu::int8 : 元々の入力値
		* ************************************************************************/
		static gu::int8 Xor(volatile gu::int8* destination, gu::int8 value);

		/*!**********************************************************************
		*  @brief        int16型をアトミックにXor演算します.
		*  @param[inout] volatile gu::int16* destination : 入力値とXor演算の結果
		*  @param[in]    gu::int16 value : Xor演算したい値
		*  @return       gu::int16 : 元々の入力値
		* ************************************************************************/
		static gu::int16 Xor(volatile gu::int16* destination, gu::int16 value);

		/*!**********************************************************************
		*  @brief        int32型をアトミックにXor演算します.
		*  @param[inout] volatile gu::int32* destination : 入力値とXor演算の結果
		*  @param[in]    gu::int32 value : Xor演算したい値
		*  @return       gu::int32 : 元々の入力値
		* ************************************************************************/
		static gu::int32 Xor(volatile gu::int32* destination, gu::int32 value);

		/*!**********************************************************************
		*  @brief        int64型をアトミックにXor演算します.
		*  @param[inout] volatile gu::int64* destination : 入力値とXor演算の結果
		*  @param[in]    gu::int64 value : Xor演算したい値
		*  @return       gu::int64 : 元々の入力値
		* ************************************************************************/
		static gu::int64 Xor(volatile gu::int64* destination, gu::int64 value);

		/*!**********************************************************************
		*  @brief        int8型をアトミックに読み取ります
		* @param[in]     volatile const gu::int8* value : 読み取りたい値
		* @return        gu::int8 : 読み取った値
		* ************************************************************************/
		static gu::int8 Read(volatile const gu::int8* value)
		{
			return CompareExchange((gu::int8*)value, 0, 0);
		}

		/*!**********************************************************************
		*  @brief        int16型をアトミックに読み取ります
		*  @param[in]    volatile const gu::int16* value : 読み取りたい値
		*  @return       gu::int16 : 読み取った値
		* ************************************************************************/
		static gu::int16 Read(volatile const gu::int16* value)
		{
			return CompareExchange((gu::int16*)value, 0, 0);
		}

		/*!**********************************************************************
		*  @brief        int32型をアトミックに読み取ります
		*  @param[in]    volatile const gu::int32* value : 読み取りたい値
		*  @return       gu::int32 : 読み取った値
		* ************************************************************************/
		static gu::int32 Read(volatile const gu::int32* value)
		{
			return CompareExchange((gu::int32*)value, 0, 0);
		}

		/*!**********************************************************************
		*  @brief        int64型をアトミックに読み取ります
		*  @param[in]    volatile const gu::int64* value : 読み取りたい値
		*  @return       gu::int64 : 読み取った値
		* ************************************************************************/
		static gu::int64 Read(volatile const gu::int64* value)
		{
			return CompareExchange((gu::int64*)value, 0, 0);
		}

		/*!**********************************************************************
		*  @brief        アトミック操作を行わずにint8型を読み取ります
		*  @param[in]    volatile const gu::int8* value : 読み取りたい値
		*  @return       gu::int8 : 読み取った値
		* ************************************************************************/
		__forceinline static gu::int8 ReadRelaxed(volatile const gu::int8* value)
		{
			return *value;
		}

		/*!**********************************************************************
		*  @brief        アトミック操作を行わずにint16型を読み取ります
		*  @param[in]    volatile const gu::int16* value : 読み取りたい値
		*  @return       gu::int16 : 読み取った値
		* ************************************************************************/
		__forceinline static gu::int16 ReadRelaxed(volatile const gu::int16* value)
		{
			return *value;
		}

		/*!**********************************************************************
		*  @brief        アトミック操作を行わずにint32型を読み取ります
		*  @param[in]    volatile const gu::int32* value : 読み取りたい値
		*  @return       gu::int32 : 読み取った値
		* ************************************************************************/
		__forceinline static gu::int32 ReadRelaxed(volatile const gu::int32* value)
		{
			return *value;
		}

		/*!**********************************************************************
		*  @brief        アトミック操作を行わずにint64型を読み取ります
		*  @param[in]    volatile const gu::int64* value : 読み取りたい値
		*  @return       gu::int64 : 読み取った値
		* ************************************************************************/
		__forceinline static gu::int64 ReadRelaxed(volatile const gu::int64* value)
		{
			return *value;
		}

		/*!**********************************************************************
		*  @brief        int8型をアトミックに書き込みます
		*  @param[inout] volatile gu::int8* destination : 書き込み先
		*  @param[in]    gu::int8 value : 書き込みたい値
		* ************************************************************************/
		__forceinline static void Write(volatile gu::int8* destination, const gu::int8 value)
		{
			Exchange(destination, value);
		}

		/*!**********************************************************************
		*  @brief        int16型をアトミックに書き込みます
		*  @param[inout] volatile gu::int16* destination : 書き込み先
		*  @param[in]    gu::int16 value : 書き込みたい値
		* ************************************************************************/
		__forceinline static void Write(volatile gu::int16* destination, const gu::int16 value)
		{
			Exchange(destination, value);
		}

		/*!**********************************************************************
		*  @brief        int32型をアトミックに書き込みます
		*  @param[inout] volatile gu::int32* destination : 書き込み先
		*  @param[in]    gu::int32 value : 書き込みたい値
		* ************************************************************************/
		__forceinline static void Write(volatile gu::int32* destination, const gu::int32 value)
		{
			Exchange(destination, value);
		}

		/*!**********************************************************************
		*  @brief        int64型をアトミックに書き込みます
		*  @param[inout] volatile gu::int64* destination : 書き込み先
		*  @param[in]    gu::int64 value : 書き込みたい値
		* ************************************************************************/
		__forceinline static void Write(volatile gu::int64* destination, const gu::int64 value)
		{
			Exchange(destination, value);
		}

		/*!**********************************************************************
		*  @brief        アトミック操作を行わずにint8型を書き込みます
		*  @param[inout] volatile gu::int8* destination : 書き込み先
		*  @param[in]    gu::int8 value : 書き込みたい値
		* ************************************************************************/
		__forceinline static void WriteRelaxed(volatile gu::int8* destination, const gu::int8 value)
		{
			*destination = value;
		}

		/*!**********************************************************************
		*  @brief        アトミック操作を行わずにint16型を書き込みます
		*  @param[inout] volatile gu::int16* destination : 書き込み先
		*  @param[in]    gu::int16 value : 書き込みたい値
		* ************************************************************************/
		__forceinline static void WriteRelaxed(volatile gu::int16* destination, const gu::int16 value)
		{
			*destination = value;
		}

		/*!**********************************************************************
		*  @brief        アトミック操作を行わずにint32型を書き込みます
		*  @param[inout] volatile gu::int32* destination : 書き込み先
		*  @param[in]    gu::int32 value : 書き込みたい値
		* ************************************************************************/
		__forceinline static void WriteRelaxed(volatile gu::int32* destination, const gu::int32 value)
		{
			*destination = value;
		}

		/*!**********************************************************************
		*  @brief        アトミック操作を行わずにint64型を書き込みます
		*  @param[inout] volatile gu::int64* destination : 書き込み先
		*  @param[in]    gu::int64 value : 書き込みたい値
		* ************************************************************************/
		__forceinline static void WriteRelaxed(volatile gu::int64* destination, const gu::int64 value)
		{
			*destination = value;
		}


		#pragma endregion 

		#pragma region Public Property

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

		#pragma endregion 

	};
}

#endif