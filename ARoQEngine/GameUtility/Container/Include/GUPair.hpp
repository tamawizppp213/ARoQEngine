//////////////////////////////////////////////////////////////////////////////////
///  @file   GUPair.hpp
///  @brief  キーバリューを結びつける構造体. Templateのため, 最初にキーバリューそれぞれの型を定義してください.
///  @author toide
///  @date   2024/02/01 21:06:26
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_PAIR_HPP
#define GU_PAIR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			   Pair
	****************************************************************************/
	/* @brief  キーバリューを結びつける構造体. Templateのため, 最初にキーバリューそれぞれの型を定義してください
	*****************************************************************************/
	template<typename KeyType, typename ValueType>
	struct Pair
	{
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property
		
		/*! @brief キー*/
		KeyType Key;

		/*! @brief キーに照らし合わせて参照される値*/
		ValueType Value;

		#pragma endregion

		#pragma region Public Operator
		__forceinline bool operator == (const Pair& other) const { return Key == other.Key; }
		__forceinline bool operator != (const Pair& other) const { return Key != other.Key; }
		__forceinline bool operator <  (const Pair& other) const { return Key <  other.Key;}
		__forceinline bool operator >  (const Pair& other) const { return Key >  other.Key; }
		__forceinline bool operator() (const Pair& a, const Pair& b) const {return a.Key < B.Key; }
		#pragma endregion

		#pragma region Public Constructor and Destructor
		Pair(const KeyType& key, const ValueType& value) : Key(key), Value(value) {};

		Pair(const KeyType& key) : Key(key), Value(ValueType()) {};

		Pair() : Key(KeyType()), Value(ValueType()) {};

		Pair(const Pair& other) = default;

		Pair& operator=(const Pair& other) noexcept
		{
			Key = other.Key; Value = other.Value;
			return *this;
		}

		Pair(Pair&& other) = default;

		#pragma endregion
	protected:
		#pragma region Protected Function
		#pragma endregion 

		#pragma region Protected Property
		#pragma endregion
	};

}
#endif