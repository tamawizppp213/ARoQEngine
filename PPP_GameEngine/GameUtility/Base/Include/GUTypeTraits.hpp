//////////////////////////////////////////////////////////////////////////////////
///             @file   GUTypeTraits.hpp
///             @brief  型を判定する処理を入れてます
///             @author toide
///             @date   2024/02/05 0:55:12
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
	/*---------------------------------------------------------------
						型が同じかを判定します
	-----------------------------------------------------------------*/
	template<typename T, typename U>
	constexpr bool IS_SAME = details::type_traits::IsSameType<T, U>::Value;

	/*---------------------------------------------------------------
						型が違うかを判定します
	-----------------------------------------------------------------*/
	template<typename T, typename U>
	constexpr bool IS_DIFFERENT = !details::type_traits::IsSameType<T, U>::Value;

	/*---------------------------------------------------------------
						整数型かを判定します
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_INTEGRAL = details::type_traits::IsIntegralType<T>::Value;
	
	/*---------------------------------------------------------------
						浮動小数点かを判定します
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_FLOATING_POINT = details::type_traits::IsIntegralType<T>::Value;

	/*---------------------------------------------------------------
						数値型かを判定します
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_ARITHMETIC = details::type_traits::IsArithmeticType<T>::Value;

	/*---------------------------------------------------------------
					 列挙型か
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_ENUM = details::type_traits::IsEnumType<T>::Value;

	/*---------------------------------------------------------------
					 ポインタか
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_POINTER = details::type_traits::IsPointerType<T>::Value;

	/*---------------------------------------------------------------
						Unionであるか
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_UNION = details::type_traits::IsUnion<T>::Value;

	/*---------------------------------------------------------------
						定数型かを判定します
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_CONST = details::type_traits::IsConstType<T>::Value;

	/*---------------------------------------------------------------
						Volatileかを判定します
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_VOLATILE = details::type_traits::IsVolatileType<T>::Value;

	/*---------------------------------------------------------------
						参照型かを判定します
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_REFERENCE = details::type_traits::IsReferenceType<T>::Value;

	/*---------------------------------------------------------------
						左辺値参照型かを判定します
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_L_VALUE_REFERENCE = details::type_traits::IsLValueReferenceType<T>::Value;

	/*---------------------------------------------------------------
						右辺値参照型かを判定します
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_R_VALUE_REFERENCE = details::type_traits::IsRValueReferenceType<T>::Value;

	/*---------------------------------------------------------------
						配列かを判定します
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_ARRAY = details::type_traits::IsArrayType<T>::Value;

	/*---------------------------------------------------------------
						プラットフォーム配列が渡されたか
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_REF_ARRAY = details::type_traits::IsRefArrayType<T>::Value;

	/*---------------------------------------------------------------
						抽象クラスか
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool IS_ABSTRACT_CLASS = details::type_traits::IsAbstractClassType<T>::Value;

	/*---------------------------------------------------------------
						仮想関数があるか
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool IS_POLYMORPHIC = details::type_traits::IsPolymorphicsClass<T>::Value;

	/*---------------------------------------------------------------
		ユーザー定義のコンストラクタを持つことなく, 初期化子リストでの初期化が出来る集成体か
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool IS_AGGREGATE = details::type_traits::IsAggregateType<T>::Value;

	/*---------------------------------------------------------------
		コピー代入演算子を持っているかどうか
	-----------------------------------------------------------------*/
	template<class T >
	constexpr bool HAS_COPY_ASSIGN_OPERATOR = details::type_traits::HasCopyAssignOperatorClass<T>::Value;

	/*---------------------------------------------------------------
		コピーコンストラクタを持っているかどうか
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool HAS_COPY_CONSTRUCTOR = details::type_traits::HasCopyConstructorClass<T>::Value;

	/*---------------------------------------------------------------
		コピー代入演算子に空の例外指定があるかどうか
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool HAS_COPY_ASSIGN_OPERATOR_WITH_NO_EXCEPT = details::type_traits::HasCopyAssignOperatorWithNoThrowClass<T>::Value;

	/*---------------------------------------------------------------
		コンストラクタに空の例外指定があるかどうか
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool HAS_CONSTRUCTOR_WITH_NO_EXCEPT = details::type_traits::HasConstructorWithNoExceptClass<T>::Value;

	/*---------------------------------------------------------------
		コピーコンストラクタに空の例外指定があるかどうか
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool HAS_COPY_CONSTRUCTOR_WITH_NO_EXCEPT = details::type_traits::HasCopyConstructorWithNoExceptClass<T>::Value;

	/*---------------------------------------------------------------
		型に単純なコンパイラにより生成された代入演算子があるかどうか
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool HAS_TRIVIAL_ASSIGN_OPERATOR = details::type_traits::HasTrivialAssignClass<T>::Value;

	/*---------------------------------------------------------------
		型に単純なコンパイラにより生成されたコンストラクターがあるかどうか
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool HAS_TRIVIAL_CONSTRUCTOR = details::type_traits::HasTrivialConstructorClass<T>::Value;

	/*---------------------------------------------------------------
		型に単純なコンパイラにより生成されたコピー コンストラクターがあるかどうか
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool HAS_TRIVIAL_COPY_CONSTRUCTOR = details::type_traits::HasTrivialCopyConstructorClass<T>::Value;

	/*---------------------------------------------------------------
		型に単純なコンパイラにより生成されたデストラクターがあるかどうか
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool HAS_TRIVIAL_DESTRUCTOR = details::type_traits::HasTrivialDestructorClass<T>::Value;

	/*---------------------------------------------------------------
						仮想のデストラクタを持っているか
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool HAS_VIRTUAL_DESTRUCTOR = details::type_traits::HasVirtualDestructorClass<T>::Value;

	/*---------------------------------------------------------------
					最初の型が2番目の型の基底クラスであるか (同じである場合もtrue)
	-----------------------------------------------------------------*/
	template<class Base, class Derived>
	constexpr bool IS_BASE_OF = details::type_traits::IsBaseOfClass<Base, Derived>::Value;

	/*---------------------------------------------------------------
					最初の型が2番目の型の派生クラスであるか
	-----------------------------------------------------------------*/
	template<class Derived, class Base>
	constexpr bool IS_DERIVED_OF = details::type_traits::IsDerivedOfClass<Derived, Base>::Value;

	/*---------------------------------------------------------------
					型がネイティブ クラスまたは構造体であるか
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool IS_CLASS = details::type_traits::IsClassType<T>::Value;

	/*---------------------------------------------------------------
					最初の型が 2 番目の型に変換できるか
	-----------------------------------------------------------------*/
	template<class From, class To>
	constexpr bool IS_CONVERTIBLE_TO = details::type_traits::IsConvertibleClass<From, To>::Value;

	/*---------------------------------------------------------------
					型にインスタンスデータメンバーがないか
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool IS_EMPTY_CLASS = details::type_traits::IsEmptyClass<T>::Value;

	/*---------------------------------------------------------------
					プラットフォーム インターフェイスか
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool IS_INTERFACE_CLASS = details::type_traits::IsInterfaceClass<T>::Value;

	/*---------------------------------------------------------------
			型がコンストラクターを持たないクラスまたは共用体、
			プライベートまたは保護された非静的なメンバー、基底クラス、および仮想関数がないか
			C言語との互換性を持つかどうかであり, POD型はmemcpyでデータをコピーでき, レイアウトが明確に定義されている(Plain Old Data)
			https://learn.microsoft.com/ja-jp/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-170
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool IS_POD = details::type_traits::IsPODClass<T>::Value;

	/*---------------------------------------------------------------
					　参照型か
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool IS_REFERENCE_TYPE = details::type_traits::IsReferenceTypeClass<T>::Value;

	/*---------------------------------------------------------------
					　値型か
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool IS_VALUE_TYPE = details::type_traits::IsValueTypeClass<T>::Value;

	/*---------------------------------------------------------------
					ガベージ コレクション ヒープへの参照を含まない値型か
	-----------------------------------------------------------------*/
	template<class T>

	constexpr bool IS_SIMPLE_VALUE_TYPE = details::type_traits::IsSimpleValueTypeClass<T>::Value;
	/*---------------------------------------------------------------
				プラットフォーム型またはネイティブ型がシールドにマークされているか
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool IS_SEALED = details::type_traits::IsSealedClass<T>::Value;

	/*---------------------------------------------------------------
					デリゲートであるか
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_DELEGATE = details::type_traits::IsDelegateType<T>::Value;
}
#endif