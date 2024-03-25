//////////////////////////////////////////////////////////////////////////////////
///             @file   GUTypeTraitsStruct.hpp
///             @brief  note : __で始まる関数はコンパイラ特有の関数であるため, 実装は分かりません
///                     https://github.com/hideakitai/ArxTypeTraits/blob/main/ArxTypeTraits/type_traits.h
///                     https://github.com/Quuxplusone/from-scratch/blob/master/include/scratch/bits/type-traits/compiler-magic.md
///             @author toide
///             @date   2024/02/05 1:02:01
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_TYPE_TRAITS_STRUCT_HPP
#define GU_TYPE_TRAITS_STRUCT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::details::type_traits
{
#pragma region TrueFalse
	template<class T, T value>
	struct IntegralConstant
	{
		// @brief : 整数定数値
		static constexpr T Value = value;

		// @brief : 型
		using ValueType = T;

		constexpr operator ValueType()   const noexcept { return Value; }
		constexpr ValueType operator()() const noexcept { return Value; }
	};

	template<bool value> 
	using BoolConstant = IntegralConstant<bool, value>;
	using TrueType     = BoolConstant<true>;
	using FalseType    = BoolConstant<false>;

	
#pragma endregion           TrueFalse
#pragma region Conditional
	template<bool Condition, typename TrueType, typename FalseType>
	struct Conditional {};

	template<class TrueType, class FalseType> struct Conditional<true, TrueType, FalseType>  { using ValueType = TrueType; };
	template<class TrueType, class FalseType> struct Conditional<false, TrueType, FalseType> { using ValueType = FalseType; };
#pragma endregion         Conditional
#pragma region IsSameType
	/*---------------------------------------------------------------
						型が同じかを判定します
	-----------------------------------------------------------------*/
	template<typename T, typename U>
	struct IsSameType : FalseType{};

	template<typename T>
	struct IsSameType<T, T> : TrueType{};
#pragma endregion          IsSameType
#pragma region IsIntegralType
	/*---------------------------------------------------------------
						整数型かを判定します
	-----------------------------------------------------------------*/
	template<typename T>
	struct IsIntegralType : FalseType {};

	template<>
	struct IsIntegralType<int8> : TrueType {};
	
	template<>
	struct IsIntegralType<int16> : TrueType {};
	
	template<>
	struct IsIntegralType<int32> : TrueType {};
	
	template<>
	struct IsIntegralType<int64> : TrueType {};

	template<>
	struct IsIntegralType<uint8> : TrueType {}; 

	template<>
	struct IsIntegralType<uint16> : TrueType {};
	
	template<>
	struct IsIntegralType<uint32> : TrueType {};

	template<>
	struct IsIntegralType<uint64> : TrueType {};

	template<>
	struct IsIntegralType<bool> : TrueType {};

	template<>
	struct IsIntegralType<char8> : TrueType {};

	template<>
	struct IsIntegralType<char16> : TrueType {};

	template<>
	struct IsIntegralType<char32> : TrueType {};

	template<>
	struct IsIntegralType<wchar> : TrueType {};

	/*---------------------------------------------------------------
					列挙型か
	-----------------------------------------------------------------*/
	template<typename T>
	struct IsEnumType : BoolConstant<__is_enum(T)>{};

#pragma endregion      IsIntegralType
#pragma region IsFloatingPointType
	/*---------------------------------------------------------------
						浮動小数点かを判定します
	-----------------------------------------------------------------*/
	template<typename T>
	struct IsFloatingPointType : FalseType {}; // デフォルトはfalse

	template<>
	struct IsFloatingPointType<float32> : TrueType {};

	template<>
	struct IsFloatingPointType<double64> : TrueType {};

	template<>
	struct IsFloatingPointType<long double> : TrueType {};
#pragma endregion IsFloatingPointType
#pragma region IsArithmetic
	/*---------------------------------------------------------------
						数値型かを判定します
	-----------------------------------------------------------------*/
	template<typename T>
	struct IsArithmeticType : Conditional<IsIntegralType<T>::Value || IsFloatingPointType<T>::Value, TrueType, FalseType>::ValueType{};
#pragma endregion IsArithmetic
#pragma region IsPointer
	template<typename T>
	struct IsPointerType : FalseType{};

	template<typename T>
	struct IsPointerType<T*> : TrueType{};

	template<typename T>
	struct IsPointerType<T* const> : TrueType{};

	template<typename T>
	struct IsPointerType<T* volatile> : TrueType{};

	template<typename T>
	struct IsPointerType<T* const volatile> : TrueType{};

#pragma endregion IsPointer
#pragma region IsArrayType
	/*---------------------------------------------------------------
						配列かを判定します
	-----------------------------------------------------------------*/
	template<typename T>
	struct IsArrayType : FalseType{};

	template<typename T>
	struct IsArrayType<T[]> : TrueType{};

	template<typename T, uint64 N>
	struct IsArrayType<T[N]> : TrueType{};

	/*---------------------------------------------------------------
						プラットフォーム配列が渡されたか
	-----------------------------------------------------------------*/
	template<class T>
	struct IsRefArrayType : BoolConstant<__is_ref_array(T)>{};

#pragma endregion         IsArrayType
#pragma region      IsUnion
	/*---------------------------------------------------------------
						Unionであるか
	-----------------------------------------------------------------*/
	template<typename T> struct IsUnion : BoolConstant<__is_union(T)> {};

#pragma endregion             IsUnion
#pragma region CPP GrammerType
	/*---------------------------------------------------------------
						定数型かを判定します
	-----------------------------------------------------------------*/
	template<typename T> struct IsConstType : FalseType{};

	template<typename T> struct IsConstType<const T> : TrueType{};

	/*---------------------------------------------------------------
						Volatileかを判定します
	-----------------------------------------------------------------*/
	template<typename T> struct IsVolatileType : FalseType{};

	template<typename T> struct IsVolatileType<volatile T> : TrueType{};

	/*---------------------------------------------------------------
						参照型かを判定します
	-----------------------------------------------------------------*/
	template<typename T> struct IsReferenceType : FalseType{};
	
	template<typename T> struct IsReferenceType<T&> : TrueType{};

	template<typename T> struct IsReferenceType<T&&> : TrueType{};

	/*---------------------------------------------------------------
						左辺値参照型かを判定します
	-----------------------------------------------------------------*/
	template<typename T> struct IsLValueReferenceType : FalseType{};

	template<typename T> struct IsLValueReferenceType<T&> : TrueType{};

	/*---------------------------------------------------------------
						右辺値参照型かを判定します
	-----------------------------------------------------------------*/
	template<typename T> struct IsRValueReferenceType : FalseType{};

	template<typename T> struct IsRValueReferenceType<T&&> : TrueType{};


#pragma endregion     CPP Grammer Type
#pragma region      CPP Class Type
	// 以下は特別な型特性のコンパイラサポート関数を使用しています
	//https://learn.microsoft.com/ja-jp/cpp/extensions/compiler-support-for-type-traits-cpp-component-extensions?view=msvc-170

	/*---------------------------------------------------------------
						抽象クラスか
	-----------------------------------------------------------------*/
	template<class T>
	struct IsAbstractClassType : BoolConstant<__is_abstract(T)>{};

	/*---------------------------------------------------------------
						仮想関数があるか
	-----------------------------------------------------------------*/
	template<class T>
	struct IsPolymorphicsClass : BoolConstant<__is_polymorphic(T)>{};

	/*---------------------------------------------------------------
		ユーザー定義のコンストラクタを持つことなく, 初期化子リストでの初期化が出来る集成体か
	-----------------------------------------------------------------*/
	template<class T>
	struct IsAggregateType : BoolConstant<__is_aggregate(T)>{};

	/*---------------------------------------------------------------
		コピー代入演算子を持っているかどうか
	-----------------------------------------------------------------*/
	template<class T>
	struct HasCopyAssignOperatorClass : BoolConstant<__has_assign(T)> {};

	/*---------------------------------------------------------------
		コピーコンストラクタを持っているかどうか
	-----------------------------------------------------------------*/
	template<class T>
	struct HasCopyConstructorClass : BoolConstant<__has_copy(T)>{};

	/*---------------------------------------------------------------
		コピー代入演算子に空の例外指定があるかどうか
	-----------------------------------------------------------------*/
	template<class T>
	struct HasCopyAssignOperatorWithNoExceptClass : BoolConstant<__has_nothrow_assign(T)>{};

	/*---------------------------------------------------------------
		コンストラクタに空の例外指定があるかどうか
	-----------------------------------------------------------------*/
	template<class T>
	struct HasConstructorWithNoExceptClass : BoolConstant<__has_nothrow_constructor(T)> {};

	/*---------------------------------------------------------------
		コピーコンストラクタに空の例外指定があるかどうか
	-----------------------------------------------------------------*/
	template<class T>
	struct HasCopyConstructorWithNoExceptClass : BoolConstant<__has_nothrow_copy(T)> {};

	/*---------------------------------------------------------------
		型に単純なコンパイラにより生成された代入演算子があるかどうか
	-----------------------------------------------------------------*/
	template<class T>
	struct HasTrivialAssignClass : BoolConstant<__has_trivial_assign(T)>{};

	/*---------------------------------------------------------------
		型に単純なコンパイラにより生成されたコンストラクターがあるかどうか
	-----------------------------------------------------------------*/
	template<class T>
	struct HasTrivialConstructorClass : BoolConstant<__has_trivial_constructor(T)>{};

	/*---------------------------------------------------------------
		型に単純なコンパイラにより生成されたコピー コンストラクターがあるかどうか
	-----------------------------------------------------------------*/
	template<class T>
	struct HasTrivialCopyConstructorClass : BoolConstant<__has_trivial_copy(T)>{};

	/*---------------------------------------------------------------
		型に単純なコンパイラにより生成されたデストラクターがあるかどうか
	-----------------------------------------------------------------*/
	template<class T>
	struct HasTrivialDestructorClass : BoolConstant<__has_trivial_destructor(T)>{};

	/*---------------------------------------------------------------
		プラットフォーム型またはネイティブ型にユーザーが宣言したデストラクターがあるかどうか
	-----------------------------------------------------------------*/
	template<class T>
	struct HasUserDestructorClass : BoolConstant<__has_user_destructor(T)>{};

	/*---------------------------------------------------------------
						仮想のデストラクタを持っているか
	-----------------------------------------------------------------*/
	template<class T>
	struct HasVirtualDestructorClass : BoolConstant<__has_virtual_destructor(T)> {};

	/*---------------------------------------------------------------
					最初の型が2番目の型の基底クラスであるか
	-----------------------------------------------------------------*/
	template<class Base, class Derived>
	struct IsBaseOfClass : BoolConstant<__is_base_of(Base, Derived)>{};

	/*---------------------------------------------------------------
					最初の型が2番目の型の派生クラスであるか
	-----------------------------------------------------------------*/
	template<class Derived, class Base>
	struct IsDerivedOfClass : BoolConstant<__is_base_of(Base, Derived)>{};

	/*---------------------------------------------------------------
					型がネイティブ クラスまたは構造体であるか
	-----------------------------------------------------------------*/
	template<class T>
	struct IsClassType : BoolConstant<__is_class(T)>{};

	/*---------------------------------------------------------------
					最初の型が 2 番目の型に変換できるか
	-----------------------------------------------------------------*/
	template<class From, class To>
	struct IsConvertibleClass : BoolConstant<__is_convertible_to(From, To)>{};

	/*---------------------------------------------------------------
					型にインスタンスデータメンバーがないか
	-----------------------------------------------------------------*/
	template<class T>
	struct IsEmptyClass : BoolConstant<__is_empty(T)>{};

	/*---------------------------------------------------------------
					プラットフォーム インターフェイスか
	-----------------------------------------------------------------*/
	template<class T>
	struct IsInterfaceClass : BoolConstant<__is_interface_class(T)>{};

	/*---------------------------------------------------------------
			型がコンストラクターを持たないクラスまたは共用体、
			プライベートまたは保護された非静的なメンバー、基底クラス、および仮想関数がないか
	-----------------------------------------------------------------*/
	template<class T>
	struct IsPODClass : BoolConstant<__is_pod(T)>{};

	/*---------------------------------------------------------------
					　参照型か
	-----------------------------------------------------------------*/
	template<class T>
	struct IsReferenceTypeClass : BoolConstant<__is_ref_class(T)> {};

	/*---------------------------------------------------------------
					　値型か
	-----------------------------------------------------------------*/
	template<class T>
	struct IsValueTypeClass : BoolConstant<__is_value_class(T)>{};

	/*---------------------------------------------------------------
				ガベージ コレクション ヒープへの参照を含まない値型か
	-----------------------------------------------------------------*/
	template<class T>
	struct IsSimpleValueTypeClass : BoolConstant<__is_simple_value_class(T)>{};

	/*---------------------------------------------------------------
				プラットフォーム型またはネイティブ型がシールドにマークされているか
	-----------------------------------------------------------------*/
	template<class T>
	struct IsSealedClass : BoolConstant<__is_sealed(T)>{};

#pragma endregion      CPP Class Type
#pragma region Delegate
	/*---------------------------------------------------------------
					デリゲートであるか
	-----------------------------------------------------------------*/
	template<typename T>
	struct IsDelegateType : BoolConstant<__is_delegate(T)>{};

#pragma endregion            Delegate
}
#endif