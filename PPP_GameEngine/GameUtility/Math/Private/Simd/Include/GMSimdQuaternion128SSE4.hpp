//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdMatrix128SSE.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/20 15:36:48
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_QUATERNION_128_SSE4_HPP
#define GM_SIMD_QUATERNION_128_SSE4_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMSimdMacros.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_CPU_INSTRUCTION_SSE4_1 && !defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
#include "GMSimdVector128SSE4.hpp"
#include "GMSimdQuaternion128SSE3.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gm::simd::sse4
{
	/****************************************************************************
	*				  			   GMSimdMatrix128SSE
	*************************************************************************//**
	*  @class     GMSimdMatrix128SSE
	*  @brief     temp
	*****************************************************************************/
	class Quaternion128Utility : public gm::simd::sse3::Quaternion128Utility
	{
	public:

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : ���ς��Z�o���܂�. �ʏ�̃x�N�g�����l, �݂��̃x�N�g���̂Ȃ��p�x�𓱏o�������Ƃ��Ɏg�p���܂�
		/*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION Dot(ConstQuaternion128 left, ConstQuaternion128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2���m����(�e�v�f��2��a�����[�g��������Ƃ��̃x�N�g���̑傫��)���Z�o
		/*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION Length(ConstQuaternion128 quaternion) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �N�H�[�^�j�I���̐��K�����s���܂�
		/*----------------------------------------------------------------------*/
		__forceinline static sse::Quaternion128 SIMD_CALL_CONVENTION Normalize(ConstQuaternion128 quaternion) noexcept;
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
	};

#pragma region Implement
#pragma region Math
	/****************************************************************************
	*                       Dot
	*************************************************************************//**
	*  @fn        float SIMD_CALL_CONVENTION Quaternion128Utility::Dot(ConstQuaternion128 left, ConstQuaternion128 right) noexcept
	*
	*  @brief      ���ς��Z�o���܂�. �ʏ�̃x�N�g�����l, �݂��̃x�N�g���̂Ȃ��p�x�𓱏o�������Ƃ��Ɏg�p���܂�
	*
	*  @param[in] ConstQuaternion128 quaternion
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Quaternion128Utility::Dot(ConstQuaternion128 left, ConstQuaternion128 right) noexcept
	{
		return Vector128Utility::DotVector4(left, right);
	}

	/****************************************************************************
	*                       Length
	*************************************************************************//**
	*  @fn        float SIMD_CALL_CONVENTION Quaternion128Utility::Length(ConstQuaternion128 quaternion) noexcept
	*
	*  @brief     1���m���� (��Βl�̑��a���������ɍ��������)���擾���܂�
	*
	*  @param[in] ConstQuaternion128 quaternion
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Quaternion128Utility::Length(ConstQuaternion128 quaternion) noexcept
	{
		return Vector128Utility::LengthVector4(quaternion);
	}

	/****************************************************************************
	*                       Normalize
	*************************************************************************//**
	*  @fn        Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::Normalize(ConstQuaternion128 quaternion) noexcept
	*
	*  @brief      �N�H�[�^�j�I���̐��K�����s���܂�
	*
	*  @param[in] ConstQuaternion128 quaternion
	*
	*  @return �@�@Quaternion128
	*****************************************************************************/
	inline sse::Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::Normalize(ConstQuaternion128 quaternion) noexcept
	{
		return Vector128Utility::NormalizeVector4(quaternion);
	}
#pragma endregion Math
#pragma endregion Implement
}
#endif
#endif