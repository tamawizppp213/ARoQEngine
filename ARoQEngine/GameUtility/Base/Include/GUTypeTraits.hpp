//////////////////////////////////////////////////////////////////////////////////
///  @file   GUTypeTraits.hpp
///  @brief  型を判定する処理を入れてます @n
/// 　　　　　　constexprを使っているため, コンパイル時と実行時両方で呼び出すことが出来る定数値となります.
///  @author toide
///  @date   2024/02/05 0:55:12
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_TYPE_TRAITS_HPP
#define GU_TYPE_TRAITS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Private/Base/Include/GUTypeTraitsStruct.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu::type
{
	/*!**********************************************************************
	*  @brief  型が同じかを判定します
	*************************************************************************/
	template<typename T, typename U>
	constexpr bool IS_SAME = details::type_traits::IsSameType<T, U>::Value;

	/*!**********************************************************************
	*  @brief  型が違うかを判定します
	*************************************************************************/

	template<typename T, typename U>
	constexpr bool IS_DIFFERENT = !details::type_traits::IsSameType<T, U>::Value;

	/*!**********************************************************************
	*  @brief  整数型かを判定します
	*************************************************************************/
	template<typename T>
	constexpr bool IS_INTEGRAL = details::type_traits::IsIntegralType<T>::Value;
	
	/*!**********************************************************************
	*  @brief  浮動小数点かを判定します
	*************************************************************************/
	template<typename T>
	constexpr bool IS_FLOATING_POINT = details::type_traits::IsIntegralType<T>::Value;

	/*!**********************************************************************
	*  @brief  数値型かを判定します
	*************************************************************************/
	template<typename T>
	constexpr bool IS_ARITHMETIC = details::type_traits::IsArithmeticType<T>::Value;

	/*!**********************************************************************
	*  @brief  列挙型かを判定します
	*************************************************************************/
	template<typename T>
	constexpr bool IS_ENUM = details::type_traits::IsEnumType<T>::Value;

	/*!**********************************************************************
	*  @brief  ポインタかを判定します
	*************************************************************************/
	template<typename T>
	constexpr bool IS_POINTER = details::type_traits::IsPointerType<T>::Value;

	/*!**********************************************************************
	*  @brief  Unionであるかを判定します
	*************************************************************************/
	template<typename T>
	constexpr bool IS_UNION = details::type_traits::IsUnion<T>::Value;

	/*!**********************************************************************
	*  @brief  定数型かを判定します
	*************************************************************************/
	template<typename T>
	constexpr bool IS_CONST = details::type_traits::IsConstType<T>::Value;

	/*!**********************************************************************
	*  @brief  Volatileかを判定します
	*************************************************************************/
	template<typename T>
	constexpr bool IS_VOLATILE = details::type_traits::IsVolatileType<T>::Value;

	/*!**********************************************************************
	*  @brief  参照型かを判定します
	*************************************************************************/
	template<typename T>
	constexpr bool IS_REFERENCE = details::type_traits::IsReferenceType<T>::Value;

	/*!**********************************************************************
	*  @brief  左辺値参照型かを判定します
	*************************************************************************/
	template<typename T>
	constexpr bool IS_L_VALUE_REFERENCE = details::type_traits::IsLValueReferenceType<T>::Value;

	/*!**********************************************************************
	*  @brief  右辺値参照型かを判定します
	*************************************************************************/
	template<typename T>
	constexpr bool IS_R_VALUE_REFERENCE = details::type_traits::IsRValueReferenceType<T>::Value;

	/*!**********************************************************************
	*  @brief  配列かを判定します
	*************************************************************************/
	template<typename T>
	constexpr bool IS_ARRAY = details::type_traits::IsArrayType<T>::Value;

	/*!**********************************************************************
	*  @brief  プラットフォーム配列が渡されたか
	*************************************************************************/
	template<typename T>
	constexpr bool IS_REF_ARRAY = details::type_traits::IsRefArrayType<T>::Value;

	/*!**********************************************************************
	*  @brief  抽象クラスか
	*************************************************************************/
	template<class T>
	constexpr bool IS_ABSTRACT_CLASS = details::type_traits::IsAbstractClassType<T>::Value;

	/*!**********************************************************************
	*  @brief  仮想関数があるか
	*************************************************************************/
	template<class T>
	constexpr bool IS_POLYMORPHIC = details::type_traits::IsPolymorphicsClass<T>::Value;

	/*!**********************************************************************
	*  @brief  ユーザー定義のコンストラクタを持つことなく, 初期化子リストでの初期化が出来る集成体か
	*************************************************************************/
	template<class T>
	constexpr bool IS_AGGREGATE = details::type_traits::IsAggregateType<T>::Value;

	/*!**********************************************************************
	*  @brief  コピー代入演算子を持っているかどうか
	*************************************************************************/
	template<class T >
	constexpr bool HAS_COPY_ASSIGN_OPERATOR = details::type_traits::HasCopyAssignOperatorClass<T>::Value;

	/*!**********************************************************************
	*  @brief  コピーコンストラクタを持っているかどうか
	*************************************************************************/
	template<class T>
	constexpr bool HAS_COPY_CONSTRUCTOR = details::type_traits::HasCopyConstructorClass<T>::Value;

	/*!**********************************************************************
	*  @brief  コピー代入演算子に空の例外指定があるかどうか
	*************************************************************************/
	template<class T>
	constexpr bool HAS_COPY_ASSIGN_OPERATOR_WITH_NO_EXCEPT = details::type_traits::HasCopyAssignOperatorWithNoThrowClass<T>::Value;

	/*!**********************************************************************
	*  @brief  コンストラクタに空の例外指定があるかどうか
	*************************************************************************/
	template<class T>
	constexpr bool HAS_CONSTRUCTOR_WITH_NO_EXCEPT = details::type_traits::HasConstructorWithNoExceptClass<T>::Value;

	/*!**********************************************************************
	*  @brief  コピーコンストラクタに空の例外指定があるかどうか
	*************************************************************************/
	template<class T>
	constexpr bool HAS_COPY_CONSTRUCTOR_WITH_NO_EXCEPT = details::type_traits::HasCopyConstructorWithNoExceptClass<T>::Value;

	/*!**********************************************************************
	*  @brief  型に単純なコンパイラにより生成された代入演算子があるかどうか
	*************************************************************************/
	template<class T>
	constexpr bool HAS_TRIVIAL_ASSIGN_OPERATOR = details::type_traits::HasTrivialAssignClass<T>::Value;

	/*!**********************************************************************
	*  @brief  型に単純なコンパイラにより生成されたコンストラクターがあるかどうか
	*************************************************************************/
	template<class T>
	constexpr bool HAS_TRIVIAL_CONSTRUCTOR = details::type_traits::HasTrivialConstructorClass<T>::Value;

	/*!**********************************************************************
	*  @brief  型に単純なコンパイラにより生成されたコピーコンストラクターがあるかどうか
	*************************************************************************/
	template<class T>
	constexpr bool HAS_TRIVIAL_COPY_CONSTRUCTOR = details::type_traits::HasTrivialCopyConstructorClass<T>::Value;

	/*!**********************************************************************
	*  @brief  型に単純なコンパイラにより生成されたデストラクターがあるかどうか
	*************************************************************************/
	template<class T>
	constexpr bool HAS_TRIVIAL_DESTRUCTOR = details::type_traits::HasTrivialDestructorClass<T>::Value;

	/*!**********************************************************************
	*  @brief  仮想のデストラクタを持っているか
	*************************************************************************/
	template<class T>
	constexpr bool HAS_VIRTUAL_DESTRUCTOR = details::type_traits::HasVirtualDestructorClass<T>::Value;

	/*!**********************************************************************
	*  @brief  最初の型が2番目の型の基底クラスであるか (同じである場合もtrue)
	*************************************************************************/
	template<class Base, class Derived>
	constexpr bool IS_BASE_OF = details::type_traits::IsBaseOfClass<Base, Derived>::Value;

	/*!**********************************************************************
	*  @brief  最初の型が2番目の型の派生クラスであるか
	*************************************************************************/
	template<class Derived, class Base>
	constexpr bool IS_DERIVED_OF = details::type_traits::IsDerivedOfClass<Derived, Base>::Value;

	/*!**********************************************************************
	*  @brief  型がネイティブ クラスまたは構造体であるか
	*************************************************************************/
	template<class T>
	constexpr bool IS_CLASS = details::type_traits::IsClassType<T>::Value;

	/*!**********************************************************************
	*  @brief  最初の型が2番目の型に変換できるか
	*************************************************************************/
	template<class From, class To>
	constexpr bool IS_CONVERTIBLE_TO = details::type_traits::IsConvertibleClass<From, To>::Value;

	/*!**********************************************************************
	*  @brief  型にインスタンスデータメンバーがないか
	*************************************************************************/
	template<class T>
	constexpr bool IS_EMPTY_CLASS = details::type_traits::IsEmptyClass<T>::Value;

	/*!**********************************************************************
	*  @brief  プラットフォーム インターフェイスか
	*************************************************************************/
	template<class T>
	constexpr bool IS_INTERFACE_CLASS = details::type_traits::IsInterfaceClass<T>::Value;

	/*!**********************************************************************
	*  @brief  型がコンストラクターを持たないクラスまたは共用体、@n
			プライベートまたは保護された非静的なメンバー、基底クラス、および仮想関数がないか @n
			C言語との互換性を持つかどうかであり, POD型はmemcpyでデータをコピーでき, レイアウトが明確に定義されている(Plain Old Data) @n
			https://learn.microsoft.com/ja-jp/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-170
	*************************************************************************/
	template<class T>
	constexpr bool IS_POD = details::type_traits::IsPODClass<T>::Value;

	/*!**********************************************************************
	*  @brief  参照型か
	*************************************************************************/
	template<class T>
	constexpr bool IS_REFERENCE_TYPE = details::type_traits::IsReferenceTypeClass<T>::Value;

	/*!**********************************************************************
	*  @brief  値型か
	*************************************************************************/
	template<class T>
	constexpr bool IS_VALUE_TYPE = details::type_traits::IsValueTypeClass<T>::Value;

	/*!**********************************************************************
	*  @brief  ガベージ コレクション ヒープへの参照を含まない値型か
	*************************************************************************/
	template<class T>
	constexpr bool IS_SIMPLE_VALUE_TYPE = details::type_traits::IsSimpleValueTypeClass<T>::Value;

	/*!**********************************************************************
	*  @brief  プラットフォーム型またはネイティブ型がシールドにマークされているか
	*************************************************************************/
	template<class T>
	constexpr bool IS_SEALED = details::type_traits::IsSealedClass<T>::Value;

	/*!**********************************************************************
	*  @brief  デリゲートであるか
	*************************************************************************/
	template<typename T>
	constexpr bool IS_DELEGATE = details::type_traits::IsDelegateType<T>::Value;
}
#endif