//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdVector128SSE3.hpp
///             @brief  SSE�ɂ��Simd���Z�ł�.
///                     Vector128�^�̎��� 
///             @author toide
///             @date   2024/03/11 2:26:47
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_VECTOR_128_SSE3_HPP
#define GM_SIMD_VECTOR_128_SSE3_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMSimdMacros.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_CPU_INSTRUCTION_SSE3 && !defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
#include "GMSimdVector128SSE2.hpp"

#include <pmmintrin.h> // SSE3
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm::simd::sse
{
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorSwizzle<0, 0, 2, 2>(Vector128Utility::ConstVector128 V) noexcept { return _mm_moveldup_ps(V); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorSwizzle<1, 1, 3, 3>(Vector128Utility::ConstVector128 V) noexcept { return _mm_movehdup_ps(V); }
}
namespace gm::simd::sse3
{
	/****************************************************************************
	*				  			   GMSimdVector128SSE3
	*************************************************************************//**
	*  @class     GMSimdVector128SSE3
	*  @brief     SSE3�̃o�[�W�����ɂ��Simd���Z
	*****************************************************************************/
	class Vector128Utility : public gm::simd::sse2::Vector128Utility
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : ���ς��Z�o (������, �Ԃ�l��float�ł͂Ȃ�Vector128�^�ŕԂ���܂��̂�, ��XGetX�ȂǂőΉ����Ă�������.)
		/*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION DotVector2(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION DotVector3(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION DotVector4(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 1���m����(�e�v�f�̐�Βl�̘a�ɂ�����x�N�g���̑傫��)���Z�o
		/*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION NormVector2(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION NormVector3(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION NormVector4(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION NormalizeVector2(ConstVector128 vector) noexcept;
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION NormalizeVector3(ConstVector128 vector) noexcept;
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION NormalizeVector4(ConstVector128 vector) noexcept;
		#pragma endregion Math
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
	*                      DotVector2
	*************************************************************************//**
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector2(ConstVector128 left, ConstVector128 right)
	*
	*  @brief     ���ς��Z�o
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION sse3::Vector128Utility::DotVector2(ConstVector128 left, ConstVector128 right) noexcept
	{
		sse::Vector128 dot = _mm_mul_ps(left, right);
		dot = _mm_hadd_ps(dot, dot);
		dot = _mm_moveldup_ps(dot);
		return _mm_cvtss_f32(dot);
	}

	/****************************************************************************
	*                      DotVector3
	*************************************************************************//**
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector3(ConstVector128 left, ConstVector128 right)
	*
	*  @brief     ���ς��Z�o
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION sse3::Vector128Utility::DotVector3(ConstVector128 left, ConstVector128 right) noexcept
	{
		// �v�f���Ƃ̐ς��v�Z
		sse::Vector128 dot = _mm_mul_ps(left, right);

		// (a0+a1, a2+a3, b0+b1, b2+b3)
		dot = _mm_hadd_ps(dot, sse::VECTOR_128F_MASK_XYZ);
		dot = _mm_hadd_ps(dot, dot);
		return _mm_cvtss_f32(_mm_hadd_ps(dot, dot));
	}

	/****************************************************************************
	*                      DotVector4
	*************************************************************************//**
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector4(ConstVector128 left, ConstVector128 right)
	*
	*  @brief     ���ς��Z�o
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION sse3::Vector128Utility::DotVector4(ConstVector128 left, ConstVector128 right) noexcept
	{
		sse::Vector128 temp = _mm_mul_ps(left, right);
		temp = _mm_hadd_ps(temp, temp);
		return _mm_cvtss_f32(_mm_hadd_ps(temp, temp));
	}

	/****************************************************************************
	*                      NormVector2
	*************************************************************************//**
	*  @fn        inline float SIMD_CALL_CONVENTION sse3::Vector128Utility::NormVector2(ConstVector128 vector) noexcept
	*
	*  @brief     1���m����(�e�v�f�̐�Βl�̘a�ɂ�����x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION sse3::Vector128Utility::NormVector2(ConstVector128 vector) noexcept
	{
		sse::Vector128 squareLength = _mm_mul_ps(vector, vector);
		sse::Vector128 temp = _mm_hadd_ps(squareLength, squareLength);
		squareLength = _mm_sqrt_ps(temp);
		squareLength = PERMUTE_PS(squareLength, _MM_SHUFFLE(0, 0, 0, 0));
		return _mm_cvtss_f32(squareLength);
	}
	
	/****************************************************************************
	*                      NormVector3
	*************************************************************************//**
	*  @fn        inline float SIMD_CALL_CONVENTION sse3::Vector128Utility::NormVector3(ConstVector128 vector) noexcept
	*
	*  @brief     1���m����(�e�v�f�̐�Βl�̘a�ɂ�����x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION sse3::Vector128Utility::NormVector3(ConstVector128 vector) noexcept
	{
		sse::Vector128 squareLength = _mm_mul_ps(vector, vector);
		squareLength = _mm_and_ps(squareLength, sse::VECTOR_128F_MASK_XYZ);
		squareLength = _mm_hadd_ps(squareLength, squareLength);
		squareLength = _mm_hadd_ps(squareLength, squareLength);
		squareLength = _mm_sqrt_ps(squareLength);
		return _mm_cvtss_f32(squareLength);
	}

	/****************************************************************************
	*                      NormVector4
	*************************************************************************//**
	*  @fn        inline float SIMD_CALL_CONVENTION sse3::Vector128Utility::NormVector4(ConstVector128 vector) noexcept
	*
	*  @brief     1���m����(�e�v�f�̐�Βl�̘a�ɂ�����x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION sse3::Vector128Utility::NormVector4(ConstVector128 vector) noexcept
	{
		sse::Vector128 squareLength = _mm_mul_ps(vector, vector);
		squareLength = _mm_hadd_ps(squareLength, squareLength);
		squareLength = _mm_hadd_ps(squareLength, squareLength);
		squareLength = _mm_sqrt_ps(squareLength);
		return _mm_cvtss_f32(squareLength);
	}

	/****************************************************************************
	*                      NormalizeVector2
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector2(ConstVector128 vector) noexcept
	*
	*  @brief     ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION sse3::Vector128Utility::NormalizeVector2(ConstVector128 vector) noexcept
	{
		// norm�v�Z
		sse::Vector128 squareLength = _mm_mul_ps(vector, vector);
		squareLength = _mm_hadd_ps(squareLength, squareLength);
		squareLength = _mm_moveldup_ps(squareLength);
		sse::Vector128 norm = _mm_sqrt_ps(squareLength);

		// 0�Ŋ�����\�������邩�ǂ����𒲂ׂ�. �l�������Ă�����1
		sse::Vector128 zeroMask = _mm_setzero_ps();
		zeroMask = _mm_cmpneq_ps(zeroMask, norm);

		// ������łȂ��ꍇ�͑Ή�����v�f��1�ɂȂ�}�X�N�̍쐬
		sse::Vector128 infinityMask = _mm_cmpneq_ps(squareLength, sse::VECTOR_128F_INFINITY);

		// ���K���̎��s
		sse::Vector128 result = _mm_div_ps(vector, norm);

		// 0�t�߂Ŋ����Ė�����ɂȂ������̂ɑ΂��Ă�0�ɐݒ�
		result = _mm_and_ps(result, zeroMask);

		// �����傩�Ɋ�Â���qnan�܂���result��I������
		sse::Vector128 temp = _mm_andnot_ps(infinityMask, sse::VECTOR_128F_QNAN);
		sse::Vector128 temp2 = _mm_and_ps(result, infinityMask);
		result = _mm_or_ps(temp, temp2);
		return result;
	}

	/****************************************************************************
	*                      NormalizeVector3
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector2(ConstVector128 vector) noexcept
	*
	*  @brief     ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION sse3::Vector128Utility::NormalizeVector3(ConstVector128 vector) noexcept
	{
		// norm�v�Z
		sse::Vector128 squareLength = _mm_mul_ps(vector, vector);
		squareLength = _mm_and_ps(squareLength, sse::VECTOR_128F_MASK_XYZ);
		squareLength = _mm_hadd_ps(squareLength, squareLength);
		squareLength = _mm_hadd_ps(squareLength, squareLength);
		sse::Vector128 norm = _mm_sqrt_ps(squareLength);

		// 0�Ŋ�����\�������邩�ǂ����𒲂ׂ�. �l�������Ă�����1
		sse::Vector128 zeroMask = _mm_setzero_ps();
		zeroMask = _mm_cmpneq_ps(zeroMask, norm);

		// ������łȂ��ꍇ�͑Ή�����v�f��1�ɂȂ�}�X�N�̍쐬
		sse::Vector128 infinityMask = _mm_cmpneq_ps(squareLength, sse::VECTOR_128F_INFINITY);

		// ���K���̎��s
		sse::Vector128 result = _mm_div_ps(vector, norm);

		// 0�t�߂Ŋ����Ė�����ɂȂ������̂ɑ΂��Ă�0�ɐݒ�
		result = _mm_and_ps(result, zeroMask);

		// �����傩�Ɋ�Â���qnan�܂���result��I������
		sse::Vector128 temp = _mm_andnot_ps(infinityMask, sse::VECTOR_128F_QNAN);
		sse::Vector128 temp2 = _mm_and_ps(result, infinityMask);
		result = _mm_or_ps(temp, temp2);
		return result;
	}

	/****************************************************************************
	*                      NormalizeVector4
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector2(ConstVector128 vector) noexcept
	*
	*  @brief     ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION sse3::Vector128Utility::NormalizeVector4(ConstVector128 vector) noexcept
	{
		// norm�v�Z
		sse::Vector128 squareLength = _mm_mul_ps(vector, vector);
		squareLength                = _mm_hadd_ps(squareLength, squareLength);
		squareLength                = _mm_hadd_ps(squareLength, squareLength); // �S�Ă̗v�f�̓��a�v�Z
		sse::Vector128 norm         = _mm_sqrt_ps(squareLength);               // ����

		// 0�Ŋ�����\�������邩�ǂ����𒲂ׂ�. �l�������Ă�����1
		sse::Vector128 zeroMask = _mm_setzero_ps();
		zeroMask           = _mm_cmpneq_ps(zeroMask, norm);

		// ������łȂ��ꍇ�͑Ή�����v�f��1�ɂȂ�}�X�N�̍쐬
		sse::Vector128 infinityMask = _mm_cmpneq_ps(squareLength, sse::VECTOR_128F_INFINITY);

		// ���K���̎��s
		sse::Vector128 result = _mm_div_ps(vector, norm);

		// 0�t�߂Ŋ����Ė�����ɂȂ������̂ɑ΂��Ă�0�ɐݒ�
		result = _mm_and_ps(result, zeroMask);

		// �����傩�Ɋ�Â���qnan�܂���result��I������
		sse::Vector128 temp  = _mm_andnot_ps(infinityMask, sse::VECTOR_128F_QNAN);
		sse::Vector128 temp2 = _mm_and_ps(result, infinityMask);
		result = _mm_or_ps(temp, temp2);
		return result;
	}
#pragma endregion Implement
}

#endif
#endif