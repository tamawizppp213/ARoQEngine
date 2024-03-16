//////////////////////////////////////////////////////////////////////////////////
///             @file   GUEnumFlags.hpp
///             @brief  Enum class���g����Flag��ݒ肷��ꍇ�ɒʏ��int�^�Ȃǂ̃t���O�������s����悤�ɂ���}�N��
/// �@�@�@�@�@�@�@�@�@�@�@�@�@Enum class�ݒ���EnumClassFlags���Ăяo���Ċ�{�t���O���Z��L��������,
///                     �t���O�m�F��
/// �@�@�@�@�@�@�@�@�@�@�@�@  HasAllFlags : �S�Ẵt���O���ݒ肳��Ă��邩, 
/// �@�@�@�@�@�@�@�@�@�@�@�@�@HasAnyFlags  : �Ώۂ̃t���O����ł��܂܂�Ă��邩�Ŋm�F
///                     �t���O�ǉ�/������AddFlags, RemoveFlags��ǉ����Ă������� �@�@�@�@�@�@             �@�@�@�@�@�@�@�@�@�@�@�@
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
	*  @brief : enum class�̂��ׂẴr�b�g���Z�q���`���A�ʏ�̃t���Oenum�Ƃ��āi�قƂ�ǁj�g�p�ł���悤�ɂ���B
	*           underlyingtype��enum class�̎��ۂ̌^ (int, char...)���擾����
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
	*  @brief : �S�Ẵt���O���܂܂�Ă��邩
	* �@�@�@�@�@�@ flags    : ���ׂ����Ώ� (�F��ȃt���O���܂܂�Ă���\���̂�����)
	*           contains : �t���O���܂܂�Ă��邩�����o���������e 
	/*----------------------------------------------------------------------*/
	template<typename EnumClass>
	constexpr bool HasAllFlags(const EnumClass flags, const EnumClass contains)
	{
		using UnderlyingType = __underlying_type(EnumClass);
		return ((UnderlyingType)flags & (UnderlyingType)contains) == (UnderlyingType)contains;
	}

	/*----------------------------------------------------------------------
	*  @brief :  �ΏۂƂȂ�t���O����ł��܂܂�Ă��邩
	* �@�@�@�@�@�@ flags    : ���ׂ����Ώ� (�F��ȃt���O���܂܂�Ă���\���̂�����)
	*           contains : �t���O���܂܂�Ă��邩�����o���������e
	/*----------------------------------------------------------------------*/
	template<typename EnumClass>
	constexpr bool HasAnyFlags(const EnumClass flags, const EnumClass contains)
	{
		using UnderlyingType = __underlying_type(EnumClass);
		return ((UnderlyingType)flags & (UnderlyingType)contains) != 0;
	}

	/*----------------------------------------------------------------------
	*  @brief :  �t���O��ǉ����� (�l�͂����ƃr�b�g�t���O�ɂȂ��Ă���悤�ɂ��Ă�������)
	* �@�@�@�@�@�@ flags    : ���ׂ����Ώ� (�F��ȃt���O���܂܂�Ă���\���̂�����)
	*           contains : �t���O��ǉ����������
	/*----------------------------------------------------------------------*/
	template<typename EnumClass>
	constexpr void AddFlags(const EnumClass& flags, const EnumClass addFlag)
	{
		using UnderlyingType = __underlying_type(EnumClass);
		flags = (EnumClass)((UnderlyingType)flags | (UnderlyingType)addFlag);
	}

	/*----------------------------------------------------------------------
	*  @brief :  �t���O���폜���� (�l�͂����ƃr�b�g�t���O�ɂȂ��Ă���悤�ɂ��Ă�������)
	* �@�@�@�@�@�@ flags    : ���ׂ����Ώ� (�F��ȃt���O���܂܂�Ă���\���̂�����)
	*           contains : �t���O����菜���������
	/*----------------------------------------------------------------------*/
	template<typename EnumClass>
	constexpr void RemoveFlags(const EnumClass& flags, const EnumClass removeFlag)
	{
		using UnderlyingType = __underlying_type(EnumClass);
		flags = (EnumClass)((UnderlyingType)flags & ~(UnderlyingType)removeFlag);
	}
}

#endif