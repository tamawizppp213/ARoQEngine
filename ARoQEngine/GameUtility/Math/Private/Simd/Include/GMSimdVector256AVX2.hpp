//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdVector256AVX2.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/18 9:24:51
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_VECTOR_256_AVX2_HPP
#define GM_SIMD_VECTOR_256_AVX2_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMSimdMacros.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_CPU_INSTRUCTION_AVX2 && !defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
#include "GMSimdVector256AVX.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm::simd::avx2
{
	/****************************************************************************
	*				  			   GMSimdVector256AVX2
	****************************************************************************/
	/* @class     GMSimdVector256AVX2
	*  @brief     temp
	*****************************************************************************/
	class Vector256Utility : public gm::simd::avx::Vector256Utility
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		#pragma region Getter
		/*----------------------------------------------------------------------
		*  @brief : doubleの配列を使ってベクトルに格納する
		*----------------------------------------------------------------------*/
		__forceinline static avx::Vector256 SIMD_CALL_CONVENTION LoadDouble3(const double* source) noexcept;
		#pragma endregion Getter
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
	};

#pragma region Implement
	#pragma region Setter
	#pragma endregion Setter
	#pragma region Getter
	/****************************************************************************
	*                       LoadDouble3
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble3(const double* source) noexcept
	*
	*  @brief     Doubleの配列を使って格納する (source->x, source->y, source->z, source->w)
	*
	*  @param[in] const float* source
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline avx::Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble3(const double* source) noexcept
	{
		Check(source);

		avx::Vector256d result = {};
		result = _mm256_maskload_pd(source, _mm256_castpd_si256(avx::VECTOR_256U_MASK_ABS));
		return result.u.V;
	}

	#pragma endregion Getter
#pragma endregion Implement
}
#endif
#endif