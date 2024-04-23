//////////////////////////////////////////////////////////////////////////////////
///  @file   GUAlignment.hpp
///  @brief  ��1�����œ��͂��ꂽ�l���2�����̔{���ɋ߂��l�ɑ�����A���C�������g���������s���邽�߂̃N���X�ł�. @n
///          AlignUp       : �ł���2�����̔{���ɋ߂��l�̂���, �傫�����̒l�ɑ����܂�. bit���Z�̂���, �����ł͂��邪Alignment��2�ׂ̂���ł���K�v������.@n 
///          AlignDown     : �ł���2�����̔{���ɋ߂��l�̂���, ���������̒l�ɑ����܂�. 2�ׂ̂���ɂ��܂� @n
///          AlignArbitary : �ł���2�����̔{���ɋ߂��l�̂���, �傫�����̒l�ɑ����܂�.�@bit���Z���g�p���Ȃ�����, 2�ׂ̂���ł���K�v�͂���܂���@n 
///  @author toide
///  @date   2024/04/22 1:41:58
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_ALIGNMENT_HPP
#define GU_ALIGNMENT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Base/Include/GUTypeTraits.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{ 
	/****************************************************************************
	*				  			   Alignment
	*************************************************************************//**
	/* @brief      ��1�����œ��͂��ꂽ�l���2�����̔{���ɋ߂��l�ɑ�����A���C�������g���������s���邽�߂̃N���X�ł�. @n
    *              AlignUp       : �ł���2�����̔{���ɋ߂��l�̂���, �傫�����̒l�ɑ����܂�. bit���Z�̂���, �����ł͂��邪Alignment��2�ׂ̂���ł���K�v������.@n 
    *              AlignDown     : �ł���2�����̔{���ɋ߂��l�̂���, ���������̒l�ɑ����܂�. 2�ׂ̂���ɂ��܂� @n
    *              AlignArbitary : �ł���2�����̔{���ɋ߂��l�̂���, �傫�����̒l�ɑ����܂�.�@bit���Z���g�p���Ȃ�����, 2�ׂ̂���ł���K�v�͂���܂���@n 
	*****************************************************************************/
	class Alignment
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     Alignment�̍ł��߂��{���i2�ׂ̂���łȂ���΂Ȃ�Ȃ��j�ɒl�𑵂���B
		*  @param[in] �A���C�������g���s���l
		*  @param[in] 2�ׂ̂���Ŏw�肷��, �{���ɂ��낦�����l
		*  @return    �A���C�������g���ꂽ��̒l
		*************************************************************************/
		template<typename T>
		__forceinline static constexpr T AlignUp(const T value, const gu::uint64 alignment)
		{
			static_assert(gu::type::IS_INTEGRAL<T> || gu::type::IS_POINTER<T>, "Align expects an integer or pointer type");

			return (T)(((uint64)value + alignment - 1) & ~(alignment - 1));
		}

		/*!**********************************************************************
		*  @brief     Alignment�ɍł��߂��Ⴂ���̒l�i2�ׂ̂���łȂ���΂Ȃ�Ȃ��j�ɒl�𑵂���B
		*  @param[in] �A���C�������g���s���l
		*  @param[in] 2�ׂ̂���̒l
		*  @return    �A���C�������g���ꂽ��̒l
		*************************************************************************/
		template<typename T>
		__forceinline static constexpr T AlignDown(const T value, const gu::uint64 alignment)
		{
			static_assert(gu::type::IS_INTEGRAL<T> || gu::type::IS_POINTER<T>, "Align down expects an integer or pointer type");

			return (T)(((uint64)value) & ~(alignment - 1));
		}

		/*!**********************************************************************
		*  @brief     �A���C�������g�ς݂��ǂ���
		*  @param[in] �A���C�������g���s���l
		*  @param[in] 2�ׂ̂���̒l
		*  @return    �A���C�������g���ꂽ��̒l
		*************************************************************************/
		template<typename T>
		__forceinline static constexpr bool IsAligned(const T value, const gu::uint64 alignment)
		{
			static_assert(gu::type::IS_INTEGRAL<T> || gu::type::IS_POINTER<T>, "IsAligned expects an integer or pointer type");

			return !((uint64)value & (alignment - 1));
		}

		/*!**********************************************************************
		*  @brief     Alignment�̍ł��߂��{���ɑ�����. ���̎�, 2�ׂ̂���ȊO�ł��Ή��\�ł�
		*  @param[in] �A���C�������g���s���l
		*  @param[in] 2�ׂ̂���̒l
		*  @return    �A���C�������g���ꂽ��̒l
		*************************************************************************/
		template<typename T>
		__forceinline static constexpr T AlignUpArbitary(const T value, const gu::uint64 alignment)
		{
			static_assert(gu::type::IS_INTEGRAL<T> || gu::type::IS_POINTER<T>, "AlignArbitrary expects an integer or pointer type");

			return (T)((((uint64)value + alignment - 1) / alignment) * alignment);
		}

		#pragma endregion 

		#pragma region Public Member Variables

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

		#pragma region Protected Member Variables

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Member Variables

		#pragma endregion 

	};

}
#endif