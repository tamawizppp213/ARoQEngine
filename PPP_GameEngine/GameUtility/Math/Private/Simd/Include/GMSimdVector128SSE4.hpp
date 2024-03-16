//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdVector128SSE4.hpp
///             @brief   SSE4���g����Simd���Z
///             @author toide
///             @date   2024/03/11 2:27:05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_VECTOR_128_SSE4_HPP
#define GM_SIMD_VECTOR_128_SSE4_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMSimdMacros.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_CPU_INSTRUCTION_SSE4_1 && !defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
#include "GMSimdVector128SSE3.hpp"

#include <smmintrin.h>
#if PLATFORM_CPU_INSTRUCTION_SSE4_2
#include <nmmintrin.h>
#endif

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gm::simd::sse4
{
	/****************************************************************************
	*				  			   GVector128Utility
	*************************************************************************//**
	*  @class     Vector128Utility
	*  @brief     SSE4���g����Simd���Z
	*****************************************************************************/
	class Vector128Utility : public gm::simd::sse3::Vector128Utility
	{ 
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
	#pragma region Setter
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION SetY(ConstVector128 vector, const float y) noexcept;
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION SetZ(ConstVector128 vector, const float z) noexcept;
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION SetW(ConstVector128 vector, const float w) noexcept;
	#pragma endregion Setter

	#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : 1���m����(�e�v�f�̐�Βl�̘a�ɂ�����x�N�g���̑傫��)���Z�o
		/*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION NormVector2(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION NormVector3(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION NormVector4(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���ς��Z�o (������, �Ԃ�l��float�ł͂Ȃ�Vector128�^�ŕԂ���܂��̂�, ��XGetX�ȂǂőΉ����Ă�������.)
		/*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION DotVector2(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION DotVector3(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION DotVector4(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION NormalizeVector2(ConstVector128 vector) noexcept;
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION NormalizeVector3(ConstVector128 vector) noexcept;
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION NormalizeVector4(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �ł��߂������Ɋۂ߂�
		/*----------------------------------------------------------------------*/
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION Round(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �؂�̂� (�w�肵�����l�̒[����؂�̂Ă������Ɋۂ߂�B���������0�̕����ɂ��鎟�̐���
		/*----------------------------------------------------------------------*/
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION Truncate(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̕����Ɋۂ߂�
		/*----------------------------------------------------------------------*/
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION Floor(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �؂�グ ��������Ő��̕����ɂ��鎟�̐����Ɋۂ߂�
		/*----------------------------------------------------------------------*/
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION Ceiling(ConstVector128 vector) noexcept;

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
	#pragma region Setter
	/****************************************************************************
	*                       SetY
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION SetY(ConstVector128 vector, const float y) noexcept
	*
	*  @brief     Float���g����Vector��Y�ɒl��������
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] const float y
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::SetY(ConstVector128 vector, const float y) noexcept
	{
		return _mm_insert_ps(vector, _mm_set_ss(y), 0x10);
	}

	/****************************************************************************
	*                       SetZ
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION SetZ(ConstVector128 vector, const float z) noexcept
	*
	*  @brief     Float���g����Vector��Z�ɒl��������
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] const float z
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::SetZ(ConstVector128 vector, const float z) noexcept
	{
		return _mm_insert_ps(vector, _mm_set_ss(z), 0x20);
	}

	/****************************************************************************
	*                       SetW
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION SetW(ConstVector128 vector, const float w) noexcept
	*
	*  @brief     Float���g����Vector��W�ɒl��������
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] const float w
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::SetW(ConstVector128 vector, const float w) noexcept
	{
		return _mm_insert_ps(vector, _mm_set_ss(w), 0x30);
	}

	#pragma endregion Setter

	#pragma region Math
	/****************************************************************************
	*                      NormVector2
	*************************************************************************//**
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::NormVector2(ConstVector128 vector) noexcept
	*
	*  @brief     1���m����(�e�v�f�̐�Βl�̘a�ɂ�����x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::NormVector2(ConstVector128 vector) noexcept
	{
		return _mm_cvtss_f32(_mm_sqrt_ps(_mm_dp_ps(vector, vector, 0x3f)));
	}

	/****************************************************************************
	*                      NormVector3
	*************************************************************************//**
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::NormVector3(ConstVector128 vector) noexcept
	*
	*  @brief     1���m����(�e�v�f�̐�Βl�̘a�ɂ�����x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::NormVector3(ConstVector128 vector) noexcept
	{
		return _mm_cvtss_f32(_mm_sqrt_ps(_mm_dp_ps(vector, vector, 0x7f)));
	}

	/****************************************************************************
	*                      NormVector4
	*************************************************************************//**
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::NormVector4(ConstVector128 vector) noexcept
	*
	*  @brief     1���m����(�e�v�f�̐�Βl�̘a�ɂ�����x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::NormVector4(ConstVector128 vector) noexcept
	{
		return _mm_cvtss_f32(_mm_sqrt_ps(_mm_dp_ps(vector, vector, 0xff)));
	}

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
	inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector2(ConstVector128 left, ConstVector128 right) noexcept
	{
		return _mm_cvtss_f32(_mm_dp_ps(left, right, 0x3f));
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
	inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector3(ConstVector128 left, ConstVector128 right) noexcept
	{
		return _mm_cvtss_f32(_mm_dp_ps(left, right, 0x7f));
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
	inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector4(ConstVector128 left, ConstVector128 right) noexcept
	{
		return _mm_cvtss_f32(_mm_dp_ps(left, right, 0xff));
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
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector2(ConstVector128 vector) noexcept
	{
		sse::Vector128 squareLength = _mm_dp_ps(vector, vector, 0x3f);
		sse::Vector128 norm         = _mm_sqrt_ps(squareLength);

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

	/****************************************************************************
	*                      NormalizeVector3
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector3(ConstVector128 vector) noexcept
	*
	*  @brief     ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector3(ConstVector128 vector) noexcept
	{
		sse::Vector128 squareLength = _mm_dp_ps(vector, vector, 0x7f);
		sse::Vector128 norm         = _mm_sqrt_ps(squareLength);

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

	/****************************************************************************
	*                      NormalizeVector4
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector4(ConstVector128 vector) noexcept
	*
	*  @brief     ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector4(ConstVector128 vector) noexcept
	{
		sse::Vector128 squareLength = _mm_dp_ps(vector, vector, 0xff);
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
	*                       Round
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Round(ConstVector128 vector) noexcept
	*
	*  @brief     �ł��߂������Ɋۂ߂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::Round(ConstVector128 vector) noexcept
	{
		return _mm_round_ps(vector, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
	}

	/****************************************************************************
	*                       Truncate
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Truncate(ConstVector128 vector) noexcept
	*
	*  @brief     �؂�̂� (�w�肵�����l�̒[����؂�̂Ă������Ɋۂ߂�B���������0�̕����ɂ��鎟�̐���
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::Truncate(ConstVector128 vector) noexcept
	{
		return _mm_round_ps(vector, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
	}

	/****************************************************************************
	*                       Floor
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Floor(ConstVector128 vector) noexcept
	*
	*  @brief     ���̕����Ɋۂ߂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::Floor(ConstVector128 vector) noexcept
	{
		return _mm_floor_ps(vector);
	}

	/****************************************************************************
	*                      Ceiling
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Ceiling(ConstVector128 vector) noexcept
	*
	*  @brief     �؂�グ ��������Ő��̕����ɂ��鎟�̐����Ɋۂ߂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::Ceiling(ConstVector128 vector) noexcept
	{
		return _mm_ceil_ps(vector);
	}
	#pragma endregion Math
}


#pragma endregion Implement
#endif
#endif