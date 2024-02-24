//////////////////////////////////////////////////////////////////////////////////
///             @file   GUEnumFlags.hpp
///             @brief  Enum classのFlag処理を通常のenumのように扱えるようにするマクロ
///             @author toide
///             @date   2024/02/18 17:15:36
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_ENUM_CLASS_FLAGS_HPP
#define GU_ENUM_CLASS_FLAGS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{

	/*----------------------------------------------------------------------
	*  @brief : enum classのすべてのビット演算子を定義し、通常のフラグenumとして（ほとんど）使用できるようにする。
	*           underlyingtypeはenum classの実際の型 (int, char...)を取得する
	/*----------------------------------------------------------------------*/
#define ENUM_CLASS_FLAGS(EnumClass) \
	inline           EnumClass& operator|=(EnumClass& left, EnumClass right) { return left = (EnumClass)((__underlying_type(EnumClass))left | (__underlying_type(EnumClass))right); } \
	inline           EnumClass& operator&=(EnumClass& left, EnumClass right) { return left = (EnumClass)((__underlying_type(EnumClass))left & (__underlying_type(EnumClass))right); } \
	inline           EnumClass& operator^=(EnumClass& left, EnumClass right) { return left = (EnumClass)((__underlying_type(EnumClass))left ^ (__underlying_type(EnumClass))right); } \
	inline constexpr EnumClass  operator| (EnumClass  left, EnumClass right) { return (EnumClass)((__underlying_type(EnumClass))left | (__underlying_type(EnumClass))right); } \
	inline constexpr EnumClass  operator& (EnumClass  left, EnumClass right) { return (EnumClass)((__underlying_type(EnumClass))left & (__underlying_type(EnumClass))right); } \
	inline constexpr EnumClass  operator^ (EnumClass  left, EnumClass right) { return (EnumClass)((__underlying_type(EnumClass))left ^ (__underlying_type(EnumClass))right); } \
	inline constexpr bool       operator! (EnumClass  enumClass)             { return !(__underlying_type(EnumClass))enumClass; } \
	inline constexpr EnumClass  operator~ (EnumClass  enumClass)             { return (EnumClass)~(__underlying_type(EnumClass))enumClass; }

	/*----------------------------------------------------------------------
	*  @brief : 全てのフラグが含まれているか
	* 　　　　　　 flags    : 調べたい対象 (色んなフラグが含まれている可能性のあるやつ)
	*           contains : フラグが含まれているかを検出したい内容 
	/*----------------------------------------------------------------------*/
	template<typename EnumClass>
	constexpr bool HasAllFlags(const EnumClass flags, const EnumClass contains)
	{
		using UnderlyingType = __underlying_type(EnumClass);
		return ((UnderlyingType)flags & (UnderlyingType)contains) == (UnderlyingType)contains;
	}

	/*----------------------------------------------------------------------
	*  @brief :  対象となるフラグが一個でも含まれているか
	* 　　　　　　 flags    : 調べたい対象 (色んなフラグが含まれている可能性のあるやつ)
	*           contains : フラグが含まれているかを検出したい内容
	/*----------------------------------------------------------------------*/
	template<typename EnumClass>
	constexpr bool HasAnyFlags(const EnumClass flags, const EnumClass contains)
	{
		using UnderlyingType = __underlying_type(EnumClass);
		return ((UnderlyingType)flags & (UnderlyingType)contains) != 0;
	}

	/*----------------------------------------------------------------------
	*  @brief :  フラグを追加する (値はちゃんとビットフラグになっているようにしてください)
	* 　　　　　　 flags    : 調べたい対象 (色んなフラグが含まれている可能性のあるやつ)
	*           contains : フラグを追加したいやつ
	/*----------------------------------------------------------------------*/
	template<typename EnumClass>
	constexpr void AddFlags(const EnumClass& flags, const EnumClass addFlag)
	{
		using UnderlyingType = __underlying_type(EnumClass);
		flags = (EnumClass)((UnderlyingType)flags | (UnderlyingType)addFlag);
	}

	/*----------------------------------------------------------------------
	*  @brief :  フラグを削除する (値はちゃんとビットフラグになっているようにしてください)
	* 　　　　　　 flags    : 調べたい対象 (色んなフラグが含まれている可能性のあるやつ)
	*           contains : フラグを取り除きたいやつ
	/*----------------------------------------------------------------------*/
	template<typename EnumClass>
	constexpr void RemoveFlags(const EnumClass& flags, const EnumClass removeFlag)
	{
		using UnderlyingType = __underlying_type(EnumClass);
		flags = (EnumClass)((UnderlyingType)flags & ~(UnderlyingType)removeFlag);
	}
}

#endif