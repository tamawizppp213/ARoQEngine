//////////////////////////////////////////////////////////////////////////////////
///             @file   SimdVector128AVX.hpp
///             @brief  AVX�p��simd���Z
///             @author toide
///             @date   2024/03/11 2:28:12
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_VECTOR_128_AVX_HPP
#define GM_SIMD_VECTOR_128_AVX_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMSimdMacros.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_CPU_INSTRUCTION_AVX && !defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
#include "GMSimdVector128SSE4.hpp"

#include <immintrin.h>

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm::simd::avx
{
	/****************************************************************************
	*				  			   SimdVector128AVX
	*************************************************************************//**
	*  @class     SimdVector128AVX
	*  @brief     AVX�p��simd���Z
	*****************************************************************************/
	class Vector128Utility : public gm::simd::sse4::Vector128Utility
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : �x�N�g������ёւ��܂�.index���w�肷��Ƃ���index�ɂ�����Vector4�^���擾
		/*----------------------------------------------------------------------*/
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION Swizzle(ConstVector128 vector,
			const gu::uint32 xIndex, const gu::uint32 yIndex,
			const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g������v�f�𒊏o��, �V�����x�N�g�����쐬����
		*           0�`3 : a�����x, y, z, w
		*           4�`7 : b�����x, y, z, w
		/*----------------------------------------------------------------------*/
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION Permute(ConstVector128 a, ConstVector128,
			const gu::uint32 xIndex, const gu::uint32 yIndex,
			const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
	};

#pragma region Implement
	/****************************************************************************
	*                       Swizzle
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Swizzle(ConstVector128 vector,
				  const gu::uint32 xIndex, const gu::uint32 yIndex,
				  const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept
	*
	*  @brief     �x�N�g������ёւ��܂�.index���w�肷��Ƃ���index�ɂ�����Vector4�^���擾
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::Swizzle(ConstVector128 vector,
		const gu::uint32 xIndex, const gu::uint32 yIndex,
		const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept
	{
		unsigned int element[4] = { xIndex, yIndex, zIndex, wIndex };
		__m128i control = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&element[0]));

		return _mm_permutevar_ps(vector, control);
	}

	/****************************************************************************
	*                       Permute
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Permute(ConstVector128 a, ConstVector128 b,
				  const gu::uint32 xIndex, const gu::uint32 yIndex,
				  const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept
	*
	*  @brief     2�̃x�N�g������v�f�𒊏o��, �V�����x�N�g�����쐬����
	*             0�`3 : a�����x, y, z, w
	*             4�`7 : b�����x, y, z, w
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::Permute(ConstVector128 a, ConstVector128 b,
		const gu::uint32 xIndex, const gu::uint32 yIndex,
		const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept
	{
		static const sse::Vector128u three = { { { 3, 3, 3, 3 } } };

		ALIGNED_DATA(16) unsigned int element[4] = { xIndex, yIndex, zIndex, wIndex };
		__m128i vControl = _mm_load_si128(reinterpret_cast<const __m128i*>(&element[0]));

		__m128i vSelect = _mm_cmpgt_epi32(vControl, three);
		vControl = _mm_castps_si128(_mm_and_ps(_mm_castsi128_ps(vControl), three));

		__m128 shuffled1 = _mm_permutevar_ps(a, vControl);
		__m128 shuffled2 = _mm_permutevar_ps(b, vControl);

		__m128 masked1 = _mm_andnot_ps(_mm_castsi128_ps(vSelect), shuffled1);
		__m128 masked2 = _mm_and_ps(_mm_castsi128_ps(vSelect), shuffled2);

		return _mm_or_ps(masked1, masked2);
	}
#pragma endregion Implement
}
#endif
#endif