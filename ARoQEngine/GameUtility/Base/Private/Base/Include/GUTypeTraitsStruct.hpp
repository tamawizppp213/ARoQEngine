//////////////////////////////////////////////////////////////////////////////////
///             @file   GUTypeTraitsStruct.hpp
///             @brief  note : __�Ŏn�܂�֐��̓R���p�C�����L�̊֐��ł��邽��, �����͕�����܂���
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
		// @brief : �����萔�l
		static constexpr T Value = value;

		// @brief : �^
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
						�^���������𔻒肵�܂�
	-----------------------------------------------------------------*/
	template<typename T, typename U>
	struct IsSameType : FalseType{};

	template<typename T>
	struct IsSameType<T, T> : TrueType{};
#pragma endregion          IsSameType
#pragma region IsIntegralType
	/*---------------------------------------------------------------
						�����^���𔻒肵�܂�
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
					�񋓌^��
	-----------------------------------------------------------------*/
	template<typename T>
	struct IsEnumType : BoolConstant<__is_enum(T)>{};

#pragma endregion      IsIntegralType
#pragma region IsFloatingPointType
	/*---------------------------------------------------------------
						���������_���𔻒肵�܂�
	-----------------------------------------------------------------*/
	template<typename T>
	struct IsFloatingPointType : FalseType {}; // �f�t�H���g��false

	template<>
	struct IsFloatingPointType<float32> : TrueType {};

	template<>
	struct IsFloatingPointType<double64> : TrueType {};

	template<>
	struct IsFloatingPointType<long double> : TrueType {};
#pragma endregion IsFloatingPointType
#pragma region IsArithmetic
	/*---------------------------------------------------------------
						���l�^���𔻒肵�܂�
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
						�z�񂩂𔻒肵�܂�
	-----------------------------------------------------------------*/
	template<typename T>
	struct IsArrayType : FalseType{};

	template<typename T>
	struct IsArrayType<T[]> : TrueType{};

	template<typename T, uint64 N>
	struct IsArrayType<T[N]> : TrueType{};

	/*---------------------------------------------------------------
						�v���b�g�t�H�[���z�񂪓n���ꂽ��
	-----------------------------------------------------------------*/
	template<class T>
	struct IsRefArrayType : BoolConstant<__is_ref_array(T)>{};

#pragma endregion         IsArrayType
#pragma region      IsUnion
	/*---------------------------------------------------------------
						Union�ł��邩
	-----------------------------------------------------------------*/
	template<typename T> struct IsUnion : BoolConstant<__is_union(T)> {};

#pragma endregion             IsUnion
#pragma region CPP GrammerType
	/*---------------------------------------------------------------
						�萔�^���𔻒肵�܂�
	-----------------------------------------------------------------*/
	template<typename T> struct IsConstType : FalseType{};

	template<typename T> struct IsConstType<const T> : TrueType{};

	/*---------------------------------------------------------------
						Volatile���𔻒肵�܂�
	-----------------------------------------------------------------*/
	template<typename T> struct IsVolatileType : FalseType{};

	template<typename T> struct IsVolatileType<volatile T> : TrueType{};

	/*---------------------------------------------------------------
						�Q�ƌ^���𔻒肵�܂�
	-----------------------------------------------------------------*/
	template<typename T> struct IsReferenceType : FalseType{};
	
	template<typename T> struct IsReferenceType<T&> : TrueType{};

	template<typename T> struct IsReferenceType<T&&> : TrueType{};

	/*---------------------------------------------------------------
						���Ӓl�Q�ƌ^���𔻒肵�܂�
	-----------------------------------------------------------------*/
	template<typename T> struct IsLValueReferenceType : FalseType{};

	template<typename T> struct IsLValueReferenceType<T&> : TrueType{};

	/*---------------------------------------------------------------
						�E�Ӓl�Q�ƌ^���𔻒肵�܂�
	-----------------------------------------------------------------*/
	template<typename T> struct IsRValueReferenceType : FalseType{};

	template<typename T> struct IsRValueReferenceType<T&&> : TrueType{};


#pragma endregion     CPP Grammer Type
#pragma region      CPP Class Type
	// �ȉ��͓��ʂȌ^�����̃R���p�C���T�|�[�g�֐����g�p���Ă��܂�
	//https://learn.microsoft.com/ja-jp/cpp/extensions/compiler-support-for-type-traits-cpp-component-extensions?view=msvc-170

	/*---------------------------------------------------------------
						���ۃN���X��
	-----------------------------------------------------------------*/
	template<class T>
	struct IsAbstractClassType : BoolConstant<__is_abstract(T)>{};

	/*---------------------------------------------------------------
						���z�֐������邩
	-----------------------------------------------------------------*/
	template<class T>
	struct IsPolymorphicsClass : BoolConstant<__is_polymorphic(T)>{};

	/*---------------------------------------------------------------
		���[�U�[��`�̃R���X�g���N�^�������ƂȂ�, �������q���X�g�ł̏��������o����W���̂�
	-----------------------------------------------------------------*/
	template<class T>
	struct IsAggregateType : BoolConstant<__is_aggregate(T)>{};

	/*---------------------------------------------------------------
		�R�s�[������Z�q�������Ă��邩�ǂ���
	-----------------------------------------------------------------*/
	template<class T>
	struct HasCopyAssignOperatorClass : BoolConstant<__has_assign(T)> {};

	/*---------------------------------------------------------------
		�R�s�[�R���X�g���N�^�������Ă��邩�ǂ���
	-----------------------------------------------------------------*/
	template<class T>
	struct HasCopyConstructorClass : BoolConstant<__has_copy(T)>{};

	/*---------------------------------------------------------------
		�R�s�[������Z�q�ɋ�̗�O�w�肪���邩�ǂ���
	-----------------------------------------------------------------*/
	template<class T>
	struct HasCopyAssignOperatorWithNoExceptClass : BoolConstant<__has_nothrow_assign(T)>{};

	/*---------------------------------------------------------------
		�R���X�g���N�^�ɋ�̗�O�w�肪���邩�ǂ���
	-----------------------------------------------------------------*/
	template<class T>
	struct HasConstructorWithNoExceptClass : BoolConstant<__has_nothrow_constructor(T)> {};

	/*---------------------------------------------------------------
		�R�s�[�R���X�g���N�^�ɋ�̗�O�w�肪���邩�ǂ���
	-----------------------------------------------------------------*/
	template<class T>
	struct HasCopyConstructorWithNoExceptClass : BoolConstant<__has_nothrow_copy(T)> {};

	/*---------------------------------------------------------------
		�^�ɒP���ȃR���p�C���ɂ�萶�����ꂽ������Z�q�����邩�ǂ���
	-----------------------------------------------------------------*/
	template<class T>
	struct HasTrivialAssignClass : BoolConstant<__has_trivial_assign(T)>{};

	/*---------------------------------------------------------------
		�^�ɒP���ȃR���p�C���ɂ�萶�����ꂽ�R���X�g���N�^�[�����邩�ǂ���
	-----------------------------------------------------------------*/
	template<class T>
	struct HasTrivialConstructorClass : BoolConstant<__has_trivial_constructor(T)>{};

	/*---------------------------------------------------------------
		�^�ɒP���ȃR���p�C���ɂ�萶�����ꂽ�R�s�[ �R���X�g���N�^�[�����邩�ǂ���
	-----------------------------------------------------------------*/
	template<class T>
	struct HasTrivialCopyConstructorClass : BoolConstant<__has_trivial_copy(T)>{};

	/*---------------------------------------------------------------
		�^�ɒP���ȃR���p�C���ɂ�萶�����ꂽ�f�X�g���N�^�[�����邩�ǂ���
	-----------------------------------------------------------------*/
	template<class T>
	struct HasTrivialDestructorClass : BoolConstant<__has_trivial_destructor(T)>{};

	/*---------------------------------------------------------------
		�v���b�g�t�H�[���^�܂��̓l�C�e�B�u�^�Ƀ��[�U�[���錾�����f�X�g���N�^�[�����邩�ǂ���
	-----------------------------------------------------------------*/
	template<class T>
	struct HasUserDestructorClass : BoolConstant<__has_user_destructor(T)>{};

	/*---------------------------------------------------------------
						���z�̃f�X�g���N�^�������Ă��邩
	-----------------------------------------------------------------*/
	template<class T>
	struct HasVirtualDestructorClass : BoolConstant<__has_virtual_destructor(T)> {};

	/*---------------------------------------------------------------
					�ŏ��̌^��2�Ԗڂ̌^�̊��N���X�ł��邩
	-----------------------------------------------------------------*/
	template<class Base, class Derived>
	struct IsBaseOfClass : BoolConstant<__is_base_of(Base, Derived)>{};

	/*---------------------------------------------------------------
					�ŏ��̌^��2�Ԗڂ̌^�̔h���N���X�ł��邩
	-----------------------------------------------------------------*/
	template<class Derived, class Base>
	struct IsDerivedOfClass : BoolConstant<__is_base_of(Base, Derived)>{};

	/*---------------------------------------------------------------
					�^���l�C�e�B�u �N���X�܂��͍\���̂ł��邩
	-----------------------------------------------------------------*/
	template<class T>
	struct IsClassType : BoolConstant<__is_class(T)>{};

	/*---------------------------------------------------------------
					�ŏ��̌^�� 2 �Ԗڂ̌^�ɕϊ��ł��邩
	-----------------------------------------------------------------*/
	template<class From, class To>
	struct IsConvertibleClass : BoolConstant<__is_convertible_to(From, To)>{};

	/*---------------------------------------------------------------
					�^�ɃC���X�^���X�f�[�^�����o�[���Ȃ���
	-----------------------------------------------------------------*/
	template<class T>
	struct IsEmptyClass : BoolConstant<__is_empty(T)>{};

	/*---------------------------------------------------------------
					�v���b�g�t�H�[�� �C���^�[�t�F�C�X��
	-----------------------------------------------------------------*/
	template<class T>
	struct IsInterfaceClass : BoolConstant<__is_interface_class(T)>{};

	/*---------------------------------------------------------------
			�^���R���X�g���N�^�[�������Ȃ��N���X�܂��͋��p�́A
			�v���C�x�[�g�܂��͕ی삳�ꂽ��ÓI�ȃ����o�[�A���N���X�A����щ��z�֐����Ȃ���
	-----------------------------------------------------------------*/
	template<class T>
	struct IsPODClass : BoolConstant<__is_pod(T)>{};

	/*---------------------------------------------------------------
					�@�Q�ƌ^��
	-----------------------------------------------------------------*/
	template<class T>
	struct IsReferenceTypeClass : BoolConstant<__is_ref_class(T)> {};

	/*---------------------------------------------------------------
					�@�l�^��
	-----------------------------------------------------------------*/
	template<class T>
	struct IsValueTypeClass : BoolConstant<__is_value_class(T)>{};

	/*---------------------------------------------------------------
				�K�x�[�W �R���N�V���� �q�[�v�ւ̎Q�Ƃ��܂܂Ȃ��l�^��
	-----------------------------------------------------------------*/
	template<class T>
	struct IsSimpleValueTypeClass : BoolConstant<__is_simple_value_class(T)>{};

	/*---------------------------------------------------------------
				�v���b�g�t�H�[���^�܂��̓l�C�e�B�u�^���V�[���h�Ƀ}�[�N����Ă��邩
	-----------------------------------------------------------------*/
	template<class T>
	struct IsSealedClass : BoolConstant<__is_sealed(T)>{};

#pragma endregion      CPP Class Type
#pragma region Delegate
	/*---------------------------------------------------------------
					�f���Q�[�g�ł��邩
	-----------------------------------------------------------------*/
	template<typename T>
	struct IsDelegateType : BoolConstant<__is_delegate(T)>{};

#pragma endregion            Delegate
}
#endif