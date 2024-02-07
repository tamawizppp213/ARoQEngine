//////////////////////////////////////////////////////////////////////////////////
///             @file   GUTypeTraits.hpp
///             @brief  �^�𔻒肷�鏈�������Ă܂�
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

namespace gu
{
	/*---------------------------------------------------------------
						�^���������𔻒肵�܂�
	-----------------------------------------------------------------*/
	template<typename T, typename U>
	constexpr bool IS_SAME = details::IsSameType<T, U>::Value;

	/*---------------------------------------------------------------
						�^���Ⴄ���𔻒肵�܂�
	-----------------------------------------------------------------*/
	template<typename T, typename U>
	constexpr bool IS_DIFFERENT = !details::IsSameType<T, U>::Value;

	/*---------------------------------------------------------------
						�����^���𔻒肵�܂�
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_INTEGRAL = details::IsIntegralType<T>::Value;
	
	/*---------------------------------------------------------------
						���������_���𔻒肵�܂�
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_FLOATING_POINT = details::IsIntegralType<T>::Value;

	/*---------------------------------------------------------------
						���l�^���𔻒肵�܂�
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_ARITHMETIC = details::IsArithmeticType<T>::Value;

	/*---------------------------------------------------------------
					 �񋓌^��
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_ENUM = details::IsEnumType<T>::Value;

	/*---------------------------------------------------------------
						Union�ł��邩
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_UNION = details::IsUnion<T>::Value;

	/*---------------------------------------------------------------
						�萔�^���𔻒肵�܂�
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_CONST = details::IsConstType<T>::Value;

	/*---------------------------------------------------------------
						Volatile���𔻒肵�܂�
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_VOLATILE = details::IsVolatileType<T>::Value;

	/*---------------------------------------------------------------
						�Q�ƌ^���𔻒肵�܂�
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_REFERENCE = details::IsReferenceType<T>::Value;

	/*---------------------------------------------------------------
						���Ӓl�Q�ƌ^���𔻒肵�܂�
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_L_VALUE_REFERENCE = details::IsLValueReferenceType<T>::Value;

	/*---------------------------------------------------------------
						�E�Ӓl�Q�ƌ^���𔻒肵�܂�
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_R_VALUE_REFERENCE = details::IsRValueReferenceType<T>::Value;

	/*---------------------------------------------------------------
						�z�񂩂𔻒肵�܂�
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_ARRAY = details::IsArrayType<T>::Value;

	/*---------------------------------------------------------------
						�v���b�g�t�H�[���z�񂪓n���ꂽ��
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_REF_ARRAY = details::IsRefArrayType<T>::Value;

	/*---------------------------------------------------------------
						���ۃN���X��
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool IS_ABSTRACT_CLASS = details::IsAbstractClassType<T>::Value;

	/*---------------------------------------------------------------
						���z�֐������邩
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool IS_POLYMORPHIC = details::IsPolymorphicsClass<T>::Value;

	/*---------------------------------------------------------------
		���[�U�[��`�̃R���X�g���N�^�������ƂȂ�, �������q���X�g�ł̏��������o����W���̂�
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool IS_AGGREGATE = details::IsAggregateType<T>::Value;

	/*---------------------------------------------------------------
		�R�s�[������Z�q�������Ă��邩�ǂ���
	-----------------------------------------------------------------*/
	template<class T >
	constexpr bool HAS_COPY_ASSIGN_OPERATOR = details::HasCopyAssignOperatorClass<T>::Value;

	/*---------------------------------------------------------------
		�R�s�[�R���X�g���N�^�������Ă��邩�ǂ���
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool HAS_COPY_CONSTRUCTOR = details::HasCopyConstructorClass<T>::Value;

	/*---------------------------------------------------------------
		�R�s�[������Z�q�ɋ�̗�O�w�肪���邩�ǂ���
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool HAS_COPY_ASSIGN_OPERATOR_WITH_NO_EXCEPT = details::HasCopyAssignOperatorWithNoThrowClass<T>::Value;

	/*---------------------------------------------------------------
		�R���X�g���N�^�ɋ�̗�O�w�肪���邩�ǂ���
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool HAS_CONSTRUCTOR_WITH_NO_EXCEPT = details::HasConstructorWithNoExceptClass<T>::Value;

	/*---------------------------------------------------------------
		�R�s�[�R���X�g���N�^�ɋ�̗�O�w�肪���邩�ǂ���
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool HAS_COPY_CONSTRUCTOR_WITH_NO_EXCEPT = details::HasCopyConstructorWithNoExceptClass<T>::Value;

	/*---------------------------------------------------------------
		�^�ɒP���ȃR���p�C���ɂ�萶�����ꂽ������Z�q�����邩�ǂ���
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool HAS_TRIVIAL_ASSIGN_OPERATOR = details::HasTrivialAssignClass<T>::Value;

	/*---------------------------------------------------------------
		�^�ɒP���ȃR���p�C���ɂ�萶�����ꂽ�R���X�g���N�^�[�����邩�ǂ���
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool HAS_TRIVIAL_CONSTRUCTOR = details::HasTrivialConstructorClass<T>::Value;

	/*---------------------------------------------------------------
		�^�ɒP���ȃR���p�C���ɂ�萶�����ꂽ�R�s�[ �R���X�g���N�^�[�����邩�ǂ���
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool HAS_TRIVIAL_COPY_CONSTRUCTOR = details::HasTrivialCopyConstructorClass<T>::Value;

	/*---------------------------------------------------------------
		�^�ɒP���ȃR���p�C���ɂ�萶�����ꂽ�f�X�g���N�^�[�����邩�ǂ���
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool HAS_TRIVIAL_DESTRUCTOR = details::HasTrivialDestructorClass<T>::Value;

	/*---------------------------------------------------------------
						���z�̃f�X�g���N�^�������Ă��邩
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool HAS_VIRTUAL_DESTRUCTOR = details::HasVirtualDestructorClass<T>::Value;

	/*---------------------------------------------------------------
					�ŏ��̌^��2�Ԗڂ̌^�̊��N���X�ł��邩 (�����ł���ꍇ��true)
	-----------------------------------------------------------------*/
	template<class Base, class Derived>
	constexpr bool IS_BASE_OF = details::IsBaseOfClass<Base, Derived>::Value;

	/*---------------------------------------------------------------
					�ŏ��̌^��2�Ԗڂ̌^�̔h���N���X�ł��邩
	-----------------------------------------------------------------*/
	template<class Derived, class Base>
	constexpr bool IS_DERIVED_OF = details::IsDerivedOfClass<Derived, Base>::Value;

	/*---------------------------------------------------------------
					�^���l�C�e�B�u �N���X�܂��͍\���̂ł��邩
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool IS_CLASS = details::IsClassType<T>::Value;

	/*---------------------------------------------------------------
					�ŏ��̌^�� 2 �Ԗڂ̌^�ɕϊ��ł��邩
	-----------------------------------------------------------------*/
	template<class From, class To>
	constexpr bool IS_CONVERTIBLE_TO = details::IsConvertibleClass<From, To>::Value;

	/*---------------------------------------------------------------
					�^�ɃC���X�^���X�f�[�^�����o�[���Ȃ���
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool IS_EMPTY_CLASS = details::IsEmptyClass<T>::Value;

	/*---------------------------------------------------------------
					�v���b�g�t�H�[�� �C���^�[�t�F�C�X��
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool IS_INTERFACE_CLASS = details::IsInterfaceClass<T>::Value;

	/*---------------------------------------------------------------
			�^���R���X�g���N�^�[�������Ȃ��N���X�܂��͋��p�́A
			�v���C�x�[�g�܂��͕ی삳�ꂽ��ÓI�ȃ����o�[�A���N���X�A����щ��z�֐����Ȃ���
			C����Ƃ̌݊����������ǂ����ł���, POD�^��memcpy�Ńf�[�^���R�s�[�ł�, ���C�A�E�g�����m�ɒ�`����Ă���(Plain Old Data)
			https://learn.microsoft.com/ja-jp/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-170
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool IS_POD = details::IsPODClass<T>::Value;

	/*---------------------------------------------------------------
					�@�l�^��
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool IS_REFERENCE_TYPE = details::IsReferenceTypeClass<T>::Value;

	/*---------------------------------------------------------------
					�@�l�^��
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool IS_VALUE_TYPE = details::IsValueTypeClass<T>::Value;

	/*---------------------------------------------------------------
					�K�x�[�W �R���N�V���� �q�[�v�ւ̎Q�Ƃ��܂܂Ȃ��l�^��
	-----------------------------------------------------------------*/
	template<class T>

	constexpr bool IS_SIMPLE_VALUE_TYPE = details::IsSimpleValueTypeClass<T>::Value;
	/*---------------------------------------------------------------
				�v���b�g�t�H�[���^�܂��̓l�C�e�B�u�^���V�[���h�Ƀ}�[�N����Ă��邩
	-----------------------------------------------------------------*/
	template<class T>
	constexpr bool IS_SEALED = details::IsSealedClass<T>::Value;

	/*---------------------------------------------------------------
					�f���Q�[�g�ł��邩
	-----------------------------------------------------------------*/
	template<typename T>
	constexpr bool IS_DELEGATE = details::IsDelegateType<T>::Value;
}
#endif