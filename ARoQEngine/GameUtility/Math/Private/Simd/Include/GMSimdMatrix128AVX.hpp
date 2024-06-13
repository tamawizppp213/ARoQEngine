//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdMatrix128SSE.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/20 15:36:48
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_MATRIX_128_AVX_HPP
#define GM_SIMD_MATRIX_128_AVX_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMSimdMacros.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_CPU_INSTRUCTION_AVX && !defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
#include "GMSimdVector128AVX.hpp"
#include "GMSimdMatrix128SSE4.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gm::simd::avx
{
	/****************************************************************************
	*				  			   GMSimdMatrix128SSE
	****************************************************************************/
	/* @class     GMSimdMatrix128SSE
	*  @brief     temp
	*****************************************************************************/
	class Matrix128Utility : public gm::simd::sse4::Matrix128Utility
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		#pragma region Operator
		/*----------------------------------------------------------------------
		*  @brief : ä|ÇØéZ
		/*----------------------------------------------------------------------*/
		__forceinline static sse::Matrix128 SIMD_CALL_CONVENTION Multiply(ConstMatrix128 left, ConstMatrix128 right) noexcept;
		#pragma endregion Operator

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
	};

#pragma region Implement
	#pragma region Math
	/****************************************************************************
	*                       Multiply
	****************************************************************************/
	/* @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Multiply(ConstMatrix128f left, ConstMatrix128f right) noexcept
	*
	*  @brief     ä|ÇØéZ
	*
	*  @param[in] ConstMatrix128f left
	*  @param[in] ConstMatrix128f right
	*
	*  @return Å@Å@Matrix128
	*****************************************************************************/
	inline sse::Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Multiply(ConstMatrix128 left, ConstMatrix128 right) noexcept
	{
		sse::Vector128 vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&left.Row[0]) + 0);
		sse::Vector128 vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&left.Row[0]) + 1);
		sse::Vector128 vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&left.Row[0]) + 2);
		sse::Vector128 vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&left.Row[0]) + 3);

		sse::Matrix128 mResult = {};

		// Perform the operation on the first row
		vX = _mm_mul_ps(vX, right.Row[0]);
		vY = _mm_mul_ps(vY, right.Row[1]);
		vZ = _mm_mul_ps(vZ, right.Row[2]);
		vW = _mm_mul_ps(vW, right.Row[3]);
		// Perform a binary add to reduce cumulative errors
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		mResult.Row[0] = vX;

		// Repeat for the other 3 rows
		vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&left.Row[1]) + 0);
		vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&left.Row[1]) + 1);
		vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&left.Row[1]) + 2);
		vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&left.Row[1]) + 3);

		vX = _mm_mul_ps(vX, right.Row[0]);
		vY = _mm_mul_ps(vY, right.Row[1]);
		vZ = _mm_mul_ps(vZ, right.Row[2]);
		vW = _mm_mul_ps(vW, right.Row[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		mResult.Row[1] = vX;

		vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&left.Row[2]) + 0);
		vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&left.Row[2]) + 1);
		vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&left.Row[2]) + 2);
		vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&left.Row[2]) + 3);

		vX = _mm_mul_ps(vX, right.Row[0]);
		vY = _mm_mul_ps(vY, right.Row[1]);
		vZ = _mm_mul_ps(vZ, right.Row[2]);
		vW = _mm_mul_ps(vW, right.Row[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		mResult.Row[2] = vX;

		vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&left.Row[3]) + 0);
		vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&left.Row[3]) + 1);
		vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&left.Row[3]) + 2);
		vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&left.Row[3]) + 3);

		vX = _mm_mul_ps(vX, right.Row[0]);
		vY = _mm_mul_ps(vY, right.Row[1]);
		vZ = _mm_mul_ps(vZ, right.Row[2]);
		vW = _mm_mul_ps(vW, right.Row[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		mResult.Row[3] = vX;

		return mResult;
	}
	#pragma endregion Math
#pragma endregion Implement
}
#endif
#endif