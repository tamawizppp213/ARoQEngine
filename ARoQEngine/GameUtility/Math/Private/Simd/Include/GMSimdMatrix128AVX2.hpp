//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdMatrix128SSE.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/20 15:36:48
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_MATRIX_128_AVX2_HPP
#define GM_SIMD_MATRIX_128_AVX2_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMSimdMacros.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_CPU_INSTRUCTION_AVX2 && !defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
#include "GMSimdVector128AVX2.hpp"
#include "GMSimdMatrix128AVX.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gm::simd::avx2
{
	/****************************************************************************
	*				  			   GMSimdMatrix128SSE
	*************************************************************************//**
	*  @class     GMSimdMatrix128SSE
	*  @brief     temp
	*****************************************************************************/
	class Matrix128Utility : public gm::simd::avx::Matrix128Utility
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
		**                Protected Member Variables
		*****************************************************************************/
	};

#pragma region Implement
	#pragma region Math
	/****************************************************************************
	*                       Multiply
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Multiply(ConstMatrix128f left, ConstMatrix128f right) noexcept
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
		__m256 t0 = _mm256_castps128_ps256(left.Row[0]);
		t0 = _mm256_insertf128_ps(t0, left.Row[1], 1);
		__m256 t1 = _mm256_castps128_ps256(left.Row[2]);
		t1 = _mm256_insertf128_ps(t1, left.Row[3], 1);

		__m256 u0 = _mm256_castps128_ps256(right.Row[0]);
		u0 = _mm256_insertf128_ps(u0, right.Row[1], 1);
		__m256 u1 = _mm256_castps128_ps256(right.Row[2]);
		u1 = _mm256_insertf128_ps(u1, right.Row[3], 1);

		__m256 a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(0, 0, 0, 0));
		__m256 a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(0, 0, 0, 0));
		__m256 b0 = _mm256_permute2f128_ps(u0, u0, 0x00);
		__m256 c0 = _mm256_mul_ps(a0, b0);
		__m256 c1 = _mm256_mul_ps(a1, b0);

		a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(1, 1, 1, 1));
		a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(1, 1, 1, 1));
		b0 = _mm256_permute2f128_ps(u0, u0, 0x11);
		__m256 c2 = _mm256_fmadd_ps(a0, b0, c0);
		__m256 c3 = _mm256_fmadd_ps(a1, b0, c1);

		a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(2, 2, 2, 2));
		a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(2, 2, 2, 2));
		__m256 b1 = _mm256_permute2f128_ps(u1, u1, 0x00);
		__m256 c4 = _mm256_mul_ps(a0, b1);
		__m256 c5 = _mm256_mul_ps(a1, b1);

		a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(3, 3, 3, 3));
		a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(3, 3, 3, 3));
		b1 = _mm256_permute2f128_ps(u1, u1, 0x11);
		__m256 c6 = _mm256_fmadd_ps(a0, b1, c4);
		__m256 c7 = _mm256_fmadd_ps(a1, b1, c5);

		t0 = _mm256_add_ps(c2, c6);
		t1 = _mm256_add_ps(c3, c7);

		sse::Matrix128 mResult = {};
		mResult.Row[0] = _mm256_castps256_ps128(t0);
		mResult.Row[1] = _mm256_extractf128_ps(t0, 1);
		mResult.Row[2] = _mm256_castps256_ps128(t1);
		mResult.Row[3] = _mm256_extractf128_ps(t1, 1);
		return mResult;
	}
	#pragma endregion Math
#pragma endregion Implement
}
#endif
#endif