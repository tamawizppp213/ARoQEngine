//////////////////////////////////////////////////////////////////////////////////
///             @file   GUTypeCast.hpp
///             @brief  forwardやmoveなどを定義しています. (ファイル名が変更される場合があります)
///             @author toide
///             @date   2024/02/17 12:15:42
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_TYPE_CAST_HPP
#define GU_TYPE_CAST_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Private/Base/Include/GUTypeReferenceStruct.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::type
{
	/*---------------------------------------------------------------
			 参照型を右辺値としてキャストを行います. (Moveなどに使用)
	-----------------------------------------------------------------*/
	template<typename T>
	__forceinline T&& Forward(typename details::type_traits::RemoveReferenceType<T>::Type& object) { return (T&&)object; }

	template<typename T>
	__forceinline T&& Forward(typename details::type_traits::RemoveReferenceType<T>::Type&& object) { return (T&&)object; }

	/*---------------------------------------------------------------
			 互いの要素を交換します.
	-----------------------------------------------------------------*/
	template<typename T>
	__forceinline void Swap(T& left, T& right) noexcept
	{
		T temp = Forward<T>(left);
		left   = Forward<T>(right);
		right  = Forward<T>(temp);
	}
}
#endif