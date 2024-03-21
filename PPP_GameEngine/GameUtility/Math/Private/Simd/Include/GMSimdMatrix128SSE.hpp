//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdMatrix128SSE.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/20 15:36:48
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_MATRIX_128_SSE_HPP
#define GM_SIMD_MATRIX_128_SSE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMSimdMacros.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_CPU_INSTRUCTION_SSE && !defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
#include "GMSimdVector128SSE.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gm::simd::sse
{
	struct Matrix128;

	

	/*----------------------------------------------------------------------
	*        float�z��ɂ��s�� : double�z���Matrix256�ƂȂ�܂�.
	*        �s�D��z�u�ɂȂ�܂��B
	*----------------------------------------------------------------------*/
	ALIGNED_STRUCT(16) Matrix128
	{
		// @brief : �s
		Vector128 Row[4];
		
		// @brief : Default constructor
		Matrix128() = default;

		// @brief : Copy constructor
		Matrix128(const Matrix128&)            = default;
		Matrix128& operator=(const Matrix128&) = default;

		// @brief : Move constructor
		Matrix128(Matrix128&&)            = default;
		Matrix128& operator=(Matrix128&&) = default;

		// @brief : float�^�Œ��ڎw��
		Matrix128
		(
			const float m00, const float m01, const float m02, const float m03,
			const float m10, const float m11, const float m12, const float m13,
			const float m20, const float m21, const float m22, const float m23,
			const float m30, const float m31, const float m32, const float m33
		) noexcept
		{
			Row[0] = Vector128Utility::Set(m00, m01, m02, m03);
			Row[1] = Vector128Utility::Set(m10, m11, m12, m13);
			Row[2] = Vector128Utility::Set(m20, m21, m22, m23);
			Row[3] = Vector128Utility::Set(m30, m31, m32, m33);
		};

		// @brief : float�z��
		Matrix128(const float* pArray) noexcept
		{
			Row[0] = Vector128Utility::LoadFloat4(&pArray[0]);
			Row[1] = Vector128Utility::LoadFloat4(&pArray[4]);
			Row[2] = Vector128Utility::LoadFloat4(&pArray[8]);
			Row[3] = Vector128Utility::LoadFloat4(&pArray[12]);
		}

		Matrix128(Vector128Utility::ConstVector128 r0, Vector128Utility::ConstVector128 r1, Vector128Utility::ConstVector128 r2, Vector128Utility::ConstVector128 r3) noexcept
		{
			Row[0] = r0;
			Row[1] = r1;
			Row[2] = r2;
			Row[3] = r3;
		}
	};

	/****************************************************************************
	*				  			   GMSimdMatrix128SSE
	*************************************************************************//**
	*  @class     GMSimdMatrix128SSE
	*  @brief     temp
	*****************************************************************************/
	class Matrix128Utility
	{
	private:
		using VectorFunction = Vector128Utility;

	public:
	#if ( defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC) || _GM_VECTORCALL_ || __aarch64__ )
		using ConstMatrix128 = const Matrix128;
	#else
		using ConstMatrix128f = const Matrix128&;
	#endif
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : �P�ʍs��
		/*----------------------------------------------------------------------*/
		__forceinline static Matrix128 SIMD_CALL_CONVENTION Identity() noexcept;

		#pragma region Setter
		/*----------------------------------------------------------------------
		*  @brief : float�l�𒼐ڎg���čs���ݒ肷��
		/*----------------------------------------------------------------------*/
		__forceinline static Matrix128 SIMD_CALL_CONVENTION Set
		(
			const float m00, const float m01, const float m02, const float m03,
			const float m10, const float m11, const float m12, const float m13,
			const float m20, const float m21, const float m22, const float m23,
			const float m30, const float m31, const float m32, const float m33
		) noexcept;
		#pragma endregion Setter

		#pragma region Operator
		/*----------------------------------------------------------------------
		*  @brief : �|���Z
		/*----------------------------------------------------------------------*/
		__forceinline static Matrix128 SIMD_CALL_CONVENTION Add(ConstMatrix128f left, ConstMatrix128f right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �|���Z
		/*----------------------------------------------------------------------*/
		__forceinline static Matrix128 SIMD_CALL_CONVENTION Subtract(ConstMatrix128f left, ConstMatrix128f right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �|���Z
		/*----------------------------------------------------------------------*/
		__forceinline static Matrix128 SIMD_CALL_CONVENTION Multiply(ConstMatrix128f left, ConstMatrix128f right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �]�u
		/*----------------------------------------------------------------------*/
		__forceinline static Matrix128 SIMD_CALL_CONVENTION Transpose(ConstMatrix128f matrix) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �t�s��
		*           ConstMatrix128f matrix
		*           float* �s��
		/*----------------------------------------------------------------------*/
		__forceinline static Matrix128 SIMD_CALL_CONVENTION Inverse(ConstMatrix128f matrix, float* determinant = nullptr) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �t�s��
		/*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION Determinant(ConstMatrix128f matrix) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �Ώۂ̍s���scale, rotation��quoternion, translation(���i)�ɕ������܂�. 
		/*----------------------------------------------------------------------*/
		__forceinline static bool SIMD_CALL_CONVENTION DecomposeSRT(ConstMatrix128f matrix, Vector128* scale, Vector128* quaternion, Vector128* translation) noexcept;


		#pragma endregion Operator

		#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : �P�ʍs��ł��邩�ǂ����𒲂ׂ܂�
		/*----------------------------------------------------------------------*/
		__forceinline static bool SIMD_CALL_CONVENTION IsIdentity(ConstMatrix128f matrix) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���i�ړ��p�̍s����쐬���܂�.
		/*----------------------------------------------------------------------*/
		__forceinline static Matrix128 SIMD_CALL_CONVENTION Translation(const float offsetX, const float offsetY, const float offsetZ) noexcept;
		__forceinline static Matrix128 SIMD_CALL_CONVENTION Translation(Vector128Utility::ConstVector128 offset) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �X�P�[�����O�p�̍s����쐬���܂�.
		/*----------------------------------------------------------------------*/
		__forceinline static Matrix128 SIMD_CALL_CONVENTION Scaling(const float scaleX, const float scaleY, const float scaleZ) noexcept;
		__forceinline static Matrix128 SIMD_CALL_CONVENTION Scaling(Vector128Utility::ConstVector128 scale) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : X, (Y, Z)�����ꂼ��ɑ΂���rad�P�ʂŊp�x����]���܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Matrix128 SIMD_CALL_CONVENTION RotationX(const float radian) noexcept;
		__forceinline static Matrix128 SIMD_CALL_CONVENTION RotationY(const float radian) noexcept;
		__forceinline static Matrix128 SIMD_CALL_CONVENTION RotationZ(const float radian) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���[(y axis)�A�s�b�`(x-axis)�A���[(z-axis)���g����rad�P�ʂŊp�x����]���܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Matrix128 SIMD_CALL_CONVENTION RotationRollPitchYaw(const float roll, const float pitch, const float yaw) noexcept;
		__forceinline static Matrix128 SIMD_CALL_CONVENTION RotationRollPitchYaw(Vector128Utility::ConstVector128 rollPitchYaw) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �@���x�N�g���𒆐S��rad�P�ʂŊp�x����]���܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Matrix128 SIMD_CALL_CONVENTION RotationNormal(Vector128Utility::ConstVector128 axis, const float radian) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���鎲�𒆐S��rad�P�ʂŊp�x����]���܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Matrix128 SIMD_CALL_CONVENTION RotationAxis(Vector128Utility::ConstVector128 axis, const float radian) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �N�H�[�^�j�I�����g����rad�P�ʂŊp�x����]���܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Matrix128 SIMD_CALL_CONVENTION RotationQuaternion(Vector128Utility::ConstVector128 quaternion) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �e�̕ϊ��s����쐬���܂�.
		/*----------------------------------------------------------------------*/
		__forceinline static Matrix128 SIMD_CALL_CONVENTION Shadow(Vector128Utility::ConstVector128 shadowPlane, Vector128Utility::ConstVector128 lightPosition) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : FocusPosition�Ɏ��_�������邽�߂̕ϊ��s����쐬���܂�(����n, �E��n)
		/*----------------------------------------------------------------------*/
		__forceinline static Matrix128 SIMD_CALL_CONVENTION LookAtLH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 focusPosition, Vector128Utility::ConstVector128 worldUp) noexcept;
		__forceinline static Matrix128 SIMD_CALL_CONVENTION LookAtRH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 focusPosition, Vector128Utility::ConstVector128 worldUp) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���_��Ώە����Ɍ����邽�߂̕ϊ��s����쐬���܂�(����n, �E��n)
		/*----------------------------------------------------------------------*/
		__forceinline static Matrix128 SIMD_CALL_CONVENTION LookToLH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 eyeDirection, Vector128Utility::ConstVector128 worldUp) noexcept;
		__forceinline static Matrix128 SIMD_CALL_CONVENTION LookToRH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 eyeDirection, Vector128Utility::ConstVector128 worldUp) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �������e���s�����߂̕ϊ��s����쐬���܂� (����n, �E��n)
		/*----------------------------------------------------------------------*/
		__forceinline static Matrix128 SIMD_CALL_CONVENTION PerspectiveLH(const float viewWidth, const float viewHeight, const float nearZ, const float farZ) noexcept;
		__forceinline static Matrix128 SIMD_CALL_CONVENTION PerspectiveRH(const float viewWidth, const float viewHeight, const float nearZ, const float farZ) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �������e������p���g���ĕϊ�����s����쐬���܂� (����n, �E��n)
		/*----------------------------------------------------------------------*/
		__forceinline static Matrix128 SIMD_CALL_CONVENTION PerspectiveFovLH(const float fovAngleY, const float aspectRatio, const float nearZ, const float farZ) noexcept;
		__forceinline static Matrix128 SIMD_CALL_CONVENTION PerspectiveFovRH(const float fovAngleY, const float aspectRatio, const float nearZ, const float farZ) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���s���e���s�����߂̕ϊ��s����쐬���܂� (����n, �E��n)
		/*----------------------------------------------------------------------*/
		__forceinline static Matrix128 SIMD_CALL_CONVENTION OrthographicLH(const float viewWidth, const float viewHeight, const float nearZ, const float farZ) noexcept;
		__forceinline static Matrix128 SIMD_CALL_CONVENTION OrthographicRH(const float viewWidth, const float viewHeight, const float nearZ, const float farZ) noexcept;


		#pragma endregion Math

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
	};

#pragma region Implement
	#pragma region Internal
	namespace internal
	{
		bool ScalarNearEqual(const float scalar1, const float scalar2, const float epsilon)
		{
			const float delta = scalar1 - scalar2;
			return fabsf(delta) <= epsilon;
		}

	#define RANK_DECOMPOSE(a, b, c, x, y, z)      \
		if((x) < (y))                   \
		{                               \
			if((y) < (z))               \
			{                           \
				(a) = 2;                \
				(b) = 1;                \
				(c) = 0;                \
			}                           \
			else                        \
			{                           \
				(a) = 1;                \
										\
				if((x) < (z))           \
				{                       \
					(b) = 2;            \
					(c) = 0;            \
				}                       \
				else                    \
				{                       \
					(b) = 0;            \
					(c) = 2;            \
				}                       \
			}                           \
		}                               \
		else                            \
		{                               \
			if((x) < (z))               \
			{                           \
				(a) = 2;                \
				(b) = 0;                \
				(c) = 1;                \
			}                           \
			else                        \
			{                           \
				(a) = 0;                \
										\
				if((y) < (z))           \
				{                       \
					(b) = 2;            \
					(c) = 1;            \
				}                       \
				else                    \
				{                       \
					(b) = 1;            \
					(c) = 2;            \
				}                       \
			}                           \
		}

		static constexpr float DECOMPOSE_EPSILON = 0.0001f;
	}
	#pragma endregion Internal
	#pragma region Setter
	/****************************************************************************
	*                       Identity
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Identity() noexcept
	*
	*  @brief     �P�ʍs��
	*
	*  @param[in] void
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Identity() noexcept
	{
		return Matrix128(VECTOR_128F_IDENTITY_R0.V, VECTOR_128F_IDENTITY_R1.V, VECTOR_128F_IDENTITY_R2.V, VECTOR_128F_IDENTITY_R3.V);
	}

	/****************************************************************************
	*                       Identity
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Set(
	    const float m00, const float m01, const float m02, const float m03,
		const float m10, const float m11, const float m12, const float m13,
		const float m20, const float m21, const float m22, const float m23,
		const float m30, const float m31, const float m32, const float m33
	) noexcept
	*
	*  @brief : float�l�𒼐ڎg���čs���ݒ肷��
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	/*----------------------------------------------------------------------
	*  @brief : float�l�𒼐ڎg���čs���ݒ肷��
	/*----------------------------------------------------------------------*/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Set
	(
		const float m00, const float m01, const float m02, const float m03,
		const float m10, const float m11, const float m12, const float m13,
		const float m20, const float m21, const float m22, const float m23,
		const float m30, const float m31, const float m32, const float m33
	) noexcept
	{
		return Matrix128
		(
			VectorFunction::Set(m00, m01, m02, m03),
			VectorFunction::Set(m10, m11, m12, m13),
			VectorFunction::Set(m20, m21, m22, m23),
			VectorFunction::Set(m30, m31, m32, m33)
		);
	}
	#pragma endregion Setter
	#pragma region Operator
	/****************************************************************************
	*                       Add
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Multiply(ConstMatrix128f left, ConstMatrix128f right) noexcept
	*
	*  @brief     ���Z
	*
	*  @param[in] ConstMatrix128f left
	*  @param[in] ConstMatrix128f right
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Add(ConstMatrix128f left, ConstMatrix128f right) noexcept
	{
		return Matrix128
		(
			_mm_add_ps(left.Row[0], right.Row[0]),
			_mm_add_ps(left.Row[1], right.Row[1]),
			_mm_add_ps(left.Row[2], right.Row[2]),
			_mm_add_ps(left.Row[3], right.Row[3])
		);
	}

	/****************************************************************************
	*                       Subtract
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Subtract(ConstMatrix128f left, ConstMatrix128f right) noexcept
	*
	*  @brief     ���Z
	*
	*  @param[in] ConstMatrix128f left
	*  @param[in] ConstMatrix128f right
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Subtract(ConstMatrix128f left, ConstMatrix128f right) noexcept
	{
		return Matrix128
		(
			_mm_sub_ps(left.Row[0], right.Row[0]),
			_mm_sub_ps(left.Row[1], right.Row[1]),
			_mm_sub_ps(left.Row[2], right.Row[2]),
			_mm_sub_ps(left.Row[3], right.Row[3])
		);
	}

	/****************************************************************************
	*                       Multiply
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Multiply(ConstMatrix128f left, ConstMatrix128f right) noexcept
	*
	*  @brief     �|���Z
	*
	*  @param[in] ConstMatrix128f left
	*  @param[in] ConstMatrix128f right
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Multiply(ConstMatrix128f left, ConstMatrix128f right) noexcept
	{
		Matrix128 result = {};
		// Splat the component X,Y,Z then W
	    // Use vW to hold the original row
		Vector128 vW = left.Row[0];
		Vector128 vX = PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
		Vector128 vY = PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
		Vector128 vZ = PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));

		// Perform the operation on the first row
		vX = _mm_mul_ps(vX, right.Row[0]);
		vY = _mm_mul_ps(vY, right.Row[1]);
		vZ = _mm_mul_ps(vZ, right.Row[2]);
		vW = _mm_mul_ps(vW, right.Row[3]);
		// Perform a binary add to reduce cumulative errors
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		result.Row[0] = vX;
		
		// Repeat for the other 3 rows
		vW = left.Row[1];
		vX = PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));

		vX = _mm_mul_ps(vX, right.Row[0]);
		vY = _mm_mul_ps(vY, right.Row[1]);
		vZ = _mm_mul_ps(vZ, right.Row[2]);
		vW = _mm_mul_ps(vW, right.Row[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		result.Row[1] = vX;

		vW = left.Row[2];
		vX = PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));

		vX = _mm_mul_ps(vX, right.Row[0]);
		vY = _mm_mul_ps(vY, right.Row[1]);
		vZ = _mm_mul_ps(vZ, right.Row[2]);
		vW = _mm_mul_ps(vW, right.Row[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		result.Row[2] = vX;

		vW = left.Row[3];
		vX = PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));

		vX = _mm_mul_ps(vX, right.Row[0]);
		vY = _mm_mul_ps(vY, right.Row[1]);
		vZ = _mm_mul_ps(vZ, right.Row[2]);
		vW = _mm_mul_ps(vW, right.Row[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		result.Row[3] = vX;
		return result;

	}

	/****************************************************************************
	*                       Transpose
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Transpose(ConstMatrix128f left, ConstMatrix128f right) noexcept
	*
	*  @brief     �]�u
	*
	*  @param[in] ConstMatrix128f matrix
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Transpose(ConstMatrix128f matrix) noexcept
	{
		// x.x,x.y,y.x,y.y
		Vector128 temp1 = _mm_shuffle_ps(matrix.Row[0], matrix.Row[1], _MM_SHUFFLE(1, 0, 1, 0));
		// x.z,x.w,y.z,y.w
		Vector128 temp3 = _mm_shuffle_ps(matrix.Row[0], matrix.Row[1], _MM_SHUFFLE(3, 2, 3, 2));
		// z.x,z.y,w.x,w.y
		Vector128 temp2 = _mm_shuffle_ps(matrix.Row[2], matrix.Row[3], _MM_SHUFFLE(1, 0, 1, 0));
		// z.z,z.w,w.z,w.w
		Vector128 temp4 = _mm_shuffle_ps(matrix.Row[2], matrix.Row[3], _MM_SHUFFLE(3, 2, 3, 2));

		Matrix128 result = {};
		// x.x,y.x,z.x,w.x
		result.Row[0] = _mm_shuffle_ps(temp1, temp2, _MM_SHUFFLE(2, 0, 2, 0));
		// x.y,y.y,z.y,w.y
		result.Row[1] = _mm_shuffle_ps(temp1, temp2, _MM_SHUFFLE(3, 1, 3, 1));
		// x.z,y.z,z.z,w.z
		result.Row[2] = _mm_shuffle_ps(temp3, temp4, _MM_SHUFFLE(2, 0, 2, 0));
		// x.w,y.w,z.w,w.w
		result.Row[3] = _mm_shuffle_ps(temp3, temp4, _MM_SHUFFLE(3, 1, 3, 1));
		return result;

	}

	/****************************************************************************
	*                       Inverse
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Inverse(ConstMatrix128f matrix, float* determinant) noexcept
	*
	*  @brief     �s��
	*
	*  @param[in] ConstMatrix128f matrix
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Inverse(ConstMatrix128f matrix, float* determinant) noexcept
	{
		// Transpose matrix
		Vector128 vTemp1 = _mm_shuffle_ps(matrix.Row[0], matrix.Row[1], _MM_SHUFFLE(1, 0, 1, 0));
		Vector128 vTemp3 = _mm_shuffle_ps(matrix.Row[0], matrix.Row[1], _MM_SHUFFLE(3, 2, 3, 2));
		Vector128 vTemp2 = _mm_shuffle_ps(matrix.Row[2], matrix.Row[3], _MM_SHUFFLE(1, 0, 1, 0));
		Vector128 vTemp4 = _mm_shuffle_ps(matrix.Row[2], matrix.Row[3], _MM_SHUFFLE(3, 2, 3, 2));

		Matrix128 MT = {};
		MT.Row[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
		MT.Row[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
		MT.Row[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
		MT.Row[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));

		Vector128 V00 = PERMUTE_PS(MT.Row[2], _MM_SHUFFLE(1, 1, 0, 0));
		Vector128 V10 = PERMUTE_PS(MT.Row[3], _MM_SHUFFLE(3, 2, 3, 2));
		Vector128 V01 = PERMUTE_PS(MT.Row[0], _MM_SHUFFLE(1, 1, 0, 0));
		Vector128 V11 = PERMUTE_PS(MT.Row[1], _MM_SHUFFLE(3, 2, 3, 2));
		Vector128 V02 = _mm_shuffle_ps(MT.Row[2], MT.Row[0], _MM_SHUFFLE(2, 0, 2, 0));
		Vector128 V12 = _mm_shuffle_ps(MT.Row[3], MT.Row[1], _MM_SHUFFLE(3, 1, 3, 1));

		Vector128 D0 = _mm_mul_ps(V00, V10);
		Vector128 D1 = _mm_mul_ps(V01, V11);
		Vector128 D2 = _mm_mul_ps(V02, V12);

		V00 = PERMUTE_PS(MT.Row[2], _MM_SHUFFLE(3, 2, 3, 2));
		V10 = PERMUTE_PS(MT.Row[3], _MM_SHUFFLE(1, 1, 0, 0));
		V01 = PERMUTE_PS(MT.Row[0], _MM_SHUFFLE(3, 2, 3, 2));
		V11 = PERMUTE_PS(MT.Row[1], _MM_SHUFFLE(1, 1, 0, 0));
		V02 = _mm_shuffle_ps(MT.Row[2], MT.Row[0], _MM_SHUFFLE(3, 1, 3, 1));
		V12 = _mm_shuffle_ps(MT.Row[3], MT.Row[1], _MM_SHUFFLE(2, 0, 2, 0));

		D0 = VectorFunction::NegativeMultiplySubtract(V00, V10, D0);
		D1 = VectorFunction::NegativeMultiplySubtract(V01, V11, D1);
		D2 = VectorFunction::NegativeMultiplySubtract(V02, V12, D2);
		// V11 = D0Y,D0W,D2Y,D2Y
		V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 1, 3, 1));
		V00 = PERMUTE_PS(MT.Row[1], _MM_SHUFFLE(1, 0, 2, 1));
		V10 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(0, 3, 0, 2));
		V01 = PERMUTE_PS(MT.Row[0], _MM_SHUFFLE(0, 1, 0, 2));
		V11 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(2, 1, 2, 1));
		// V13 = D1Y,D1W,D2W,D2W
		Vector128 V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 3, 3, 1));
		V02 = PERMUTE_PS(MT.Row[3], _MM_SHUFFLE(1, 0, 2, 1));
		V12 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(0, 3, 0, 2));
		Vector128 V03 = PERMUTE_PS(MT.Row[2], _MM_SHUFFLE(0, 1, 0, 2));
		V13 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(2, 1, 2, 1));

		Vector128 C0 = _mm_mul_ps(V00, V10);
		Vector128 C2 = _mm_mul_ps(V01, V11);
		Vector128 C4 = _mm_mul_ps(V02, V12);
		Vector128 C6 = _mm_mul_ps(V03, V13);

		// V11 = D0X,D0Y,D2X,D2X
		V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(0, 0, 1, 0));
		V00 = PERMUTE_PS(MT.Row[1], _MM_SHUFFLE(2, 1, 3, 2));
		V10 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(2, 1, 0, 3));
		V01 = PERMUTE_PS(MT.Row[0], _MM_SHUFFLE(1, 3, 2, 3));
		V11 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(0, 2, 1, 2));
		// V13 = D1X,D1Y,D2Z,D2Z
		V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(2, 2, 1, 0));
		V02 = PERMUTE_PS(MT.Row[3], _MM_SHUFFLE(2, 1, 3, 2));
		V12 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(2, 1, 0, 3));
		V03 = PERMUTE_PS(MT.Row[2], _MM_SHUFFLE(1, 3, 2, 3));
		V13 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(0, 2, 1, 2));

		C0 = VectorFunction::NegativeMultiplySubtract(V00, V10, C0);
		C2 = VectorFunction::NegativeMultiplySubtract(V01, V11, C2);
		C4 = VectorFunction::NegativeMultiplySubtract(V02, V12, C4);
		C6 = VectorFunction::NegativeMultiplySubtract(V03, V13, C6);

		V00 = PERMUTE_PS(MT.Row[1], _MM_SHUFFLE(0, 3, 0, 3));
		// V10 = D0Z,D0Z,D2X,D2Y
		V10 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 2, 2));
		V10 = PERMUTE_PS(V10, _MM_SHUFFLE(0, 2, 3, 0));
		V01 = PERMUTE_PS(MT.Row[0], _MM_SHUFFLE(2, 0, 3, 1));
		// V11 = D0X,D0W,D2X,D2Y
		V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 3, 0));
		V11 = PERMUTE_PS(V11, _MM_SHUFFLE(2, 1, 0, 3));
		V02 = PERMUTE_PS(MT.Row[3], _MM_SHUFFLE(0, 3, 0, 3));
		// V12 = D1Z,D1Z,D2Z,D2W
		V12 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 2, 2));
		V12 = PERMUTE_PS(V12, _MM_SHUFFLE(0, 2, 3, 0));
		V03 = PERMUTE_PS(MT.Row[2], _MM_SHUFFLE(2, 0, 3, 1));
		// V13 = D1X,D1W,D2Z,D2W
		V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 3, 0));
		V13 = PERMUTE_PS(V13, _MM_SHUFFLE(2, 1, 0, 3));

		V00 = _mm_mul_ps(V00, V10);
		V01 = _mm_mul_ps(V01, V11);
		V02 = _mm_mul_ps(V02, V12);
		V03 = _mm_mul_ps(V03, V13);
		Vector128 C1 = _mm_sub_ps(C0, V00);
		C0 = _mm_add_ps(C0, V00);
		Vector128 C3 = _mm_add_ps(C2, V01);
		C2 = _mm_sub_ps(C2, V01);
		Vector128 C5 = _mm_sub_ps(C4, V02);
		C4 = _mm_add_ps(C4, V02);
		Vector128 C7 = _mm_add_ps(C6, V03);
		C6 = _mm_sub_ps(C6, V03);

		C0 = _mm_shuffle_ps(C0, C1, _MM_SHUFFLE(3, 1, 2, 0));
		C2 = _mm_shuffle_ps(C2, C3, _MM_SHUFFLE(3, 1, 2, 0));
		C4 = _mm_shuffle_ps(C4, C5, _MM_SHUFFLE(3, 1, 2, 0));
		C6 = _mm_shuffle_ps(C6, C7, _MM_SHUFFLE(3, 1, 2, 0));
		C0 = PERMUTE_PS(C0, _MM_SHUFFLE(3, 1, 2, 0));
		C2 = PERMUTE_PS(C2, _MM_SHUFFLE(3, 1, 2, 0));
		C4 = PERMUTE_PS(C4, _MM_SHUFFLE(3, 1, 2, 0));
		C6 = PERMUTE_PS(C6, _MM_SHUFFLE(3, 1, 2, 0));
		// Get the determinant
		Vector128 vTemp = VectorFunction::Set(VectorFunction::DotVector4(C0, MT.Row[0]));
		if (determinant != nullptr)
		{
			*determinant = VectorFunction::GetX(vTemp);
		}

		vTemp = _mm_div_ps(VECTOR_128F_ONE, vTemp);
		Matrix128 mResult;
		mResult.Row[0] = _mm_mul_ps(C0, vTemp);
		mResult.Row[1] = _mm_mul_ps(C2, vTemp);
		mResult.Row[2] = _mm_mul_ps(C4, vTemp);
		mResult.Row[3] = _mm_mul_ps(C6, vTemp);
		return mResult;
	}

	/****************************************************************************
	*                       Determinant
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Determinant(ConstMatrix128f left, ConstMatrix128f right) noexcept
	*
	*  @brief     �s��
	*
	*  @param[in] ConstMatrix128f matrix
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Matrix128Utility::Determinant(ConstMatrix128f matrix) noexcept
	{
		static const Vector128f Sign = { { { 1.0f, -1.0f, 1.0f, -1.0f } } };

		Vector128 V0 = VectorSwizzle<GM_SWIZZLE_Y, GM_SWIZZLE_X, GM_SWIZZLE_X, GM_SWIZZLE_X>(matrix.Row[2]);
		Vector128 V1 = VectorSwizzle<GM_SWIZZLE_Z, GM_SWIZZLE_Z, GM_SWIZZLE_Y, GM_SWIZZLE_Y>(matrix.Row[3]);
		Vector128 V2 = VectorSwizzle<GM_SWIZZLE_Y, GM_SWIZZLE_X, GM_SWIZZLE_X, GM_SWIZZLE_X>(matrix.Row[2]);
		Vector128 V3 = VectorSwizzle<GM_SWIZZLE_W, GM_SWIZZLE_W, GM_SWIZZLE_W, GM_SWIZZLE_Z>(matrix.Row[3]);
		Vector128 V4 = VectorSwizzle<GM_SWIZZLE_Z, GM_SWIZZLE_Z, GM_SWIZZLE_Y, GM_SWIZZLE_Y>(matrix.Row[2]);
		Vector128 V5 = VectorSwizzle<GM_SWIZZLE_W, GM_SWIZZLE_W, GM_SWIZZLE_W, GM_SWIZZLE_Z>(matrix.Row[3]);

		Vector128 P0 = VectorFunction::Multiply(V0, V1);
		Vector128 P1 = VectorFunction::Multiply(V2, V3);
		Vector128 P2 = VectorFunction::Multiply(V4, V5);

		V0 = VectorSwizzle<GM_SWIZZLE_Z, GM_SWIZZLE_Z, GM_SWIZZLE_Y, GM_SWIZZLE_Y>(matrix.Row[2]);
		V1 = VectorSwizzle<GM_SWIZZLE_Y, GM_SWIZZLE_X, GM_SWIZZLE_X, GM_SWIZZLE_X>(matrix.Row[3]);
		V2 = VectorSwizzle<GM_SWIZZLE_W, GM_SWIZZLE_W, GM_SWIZZLE_W, GM_SWIZZLE_Z>(matrix.Row[2]);
		V3 = VectorSwizzle<GM_SWIZZLE_Y, GM_SWIZZLE_X, GM_SWIZZLE_X, GM_SWIZZLE_X>(matrix.Row[3]);
		V4 = VectorSwizzle<GM_SWIZZLE_W, GM_SWIZZLE_W, GM_SWIZZLE_W, GM_SWIZZLE_Z>(matrix.Row[2]);
		V5 = VectorSwizzle<GM_SWIZZLE_Z, GM_SWIZZLE_Z, GM_SWIZZLE_Y, GM_SWIZZLE_Y>(matrix.Row[3]);

		P0 = VectorFunction::NegativeMultiplySubtract(V0, V1, P0);
		P1 = VectorFunction::NegativeMultiplySubtract(V2, V3, P1);
		P2 = VectorFunction::NegativeMultiplySubtract(V4, V5, P2);

		V0 = VectorSwizzle<GM_SWIZZLE_W, GM_SWIZZLE_W, GM_SWIZZLE_W, GM_SWIZZLE_Z>(matrix.Row[1]);
		V1 = VectorSwizzle<GM_SWIZZLE_Z, GM_SWIZZLE_Z, GM_SWIZZLE_Y, GM_SWIZZLE_Y>(matrix.Row[1]);
		V2 = VectorSwizzle<GM_SWIZZLE_Y, GM_SWIZZLE_X, GM_SWIZZLE_X, GM_SWIZZLE_X>(matrix.Row[1]);

		Vector128 S = VectorFunction::Multiply(matrix.Row[0], Sign.V);
		Vector128 R = VectorFunction::Multiply(V0, P0);
		R = VectorFunction::NegativeMultiplySubtract(V1, P1, R);
		R = VectorFunction::MultiplyAdd(V2, P2, R);

		return VectorFunction::DotVector4(S, R);
	}

	/*----------------------------------------------------------------------
		*  @brief : �Ώۂ̍s���scale, rotation��quoternion, translation(���i)�ɕ������܂�.
		/*----------------------------------------------------------------------*/
	//inline bool SIMD_CALL_CONVENTION Matrix128Utility::DecomposeSRT(ConstMatrix128f matrix, Vector128* scale, Vector128* quaternion, Vector128* translation) noexcept
	//{
	//	static const Vector128* pvCanonicalBasis[3] = 
	//	{
	//     &VECTOR_128F_IDENTITY_R0.V,
	//     &VECTOR_128F_IDENTITY_R1.V,
	//     &VECTOR_128F_IDENTITY_R2.V
	//	};

	//	Check(scale       != nullptr);
	//	Check(quaternion  != nullptr);
	//	Check(translation != nullptr);

	//	// Get the translation
	//	translation[0] = matrix.Row[3];

	//	Vector128* ppvBasis[3];
	//	Matrix128 matTemp = {};
	//	ppvBasis[0] = &matTemp.Row[0];
	//	ppvBasis[1] = &matTemp.Row[1];
	//	ppvBasis[2] = &matTemp.Row[2];

	//	matTemp.Row[0] = matrix.Row[0];
	//	matTemp.Row[1] = matrix.Row[1];
	//	matTemp.Row[2] = matrix.Row[2];
	//	matTemp.Row[3] = VECTOR_128F_IDENTITY_R3.V;

	//	auto pfScales = reinterpret_cast<float*>(scale);

	//	size_t a, b, c;
	//	/*XMVectorGetXPtr(&pfScales[0], XMVector3Length(ppvBasis[0][0]));
	//	XMVectorGetXPtr(&pfScales[1], XMVector3Length(ppvBasis[1][0]));
	//	XMVectorGetXPtr(&pfScales[2], XMVector3Length(ppvBasis[2][0]));*/
	//	pfScales[3] = 0.f;

	//	RANK_DECOMPOSE(a, b, c, pfScales[0], pfScales[1], pfScales[2]);

	//	if (pfScales[a] < internal::DECOMPOSE_EPSILON)
	//	{
	//		ppvBasis[a][0] = pvCanonicalBasis[a][0];
	//	}
	//	ppvBasis[a][0] = VectorFunction::NormalizeVector3(ppvBasis[a][0]);

	//	if (pfScales[b] < internal::DECOMPOSE_EPSILON)
	//	{
	//		size_t aa, bb, cc;
	//		float fAbsX, fAbsY, fAbsZ;

	//		fAbsX = fabsf(VectorFunction::GetX(ppvBasis[a][0]));
	//		fAbsY = fabsf(VectorFunction::GetY(ppvBasis[a][0]));
	//		fAbsZ = fabsf(VectorFunction::GetZ(ppvBasis[a][0]));

	//		RANK_DECOMPOSE(aa, bb, cc, fAbsX, fAbsY, fAbsZ);

	//		ppvBasis[b][0] = VectorFunction::CrossVector3(ppvBasis[a][0], pvCanonicalBasis[cc][0]);
	//	}

	//	ppvBasis[b][0] = VectorFunction::NormalizeVector3(ppvBasis[b][0]);

	//	if (pfScales[c] < internal::DECOMPOSE_EPSILON)
	//	{
	//		ppvBasis[c][0] = VectorFunction::CrossVector3(ppvBasis[a][0], ppvBasis[b][0]);
	//	}

	//	ppvBasis[c][0] = VectorFunction::NormalizeVector3(ppvBasis[c][0]);

	//	float fDet = Determinant(matTemp);

	//	// use Kramer's rule to check for handedness of coordinate system
	//	if (fDet < 0.0f)
	//	{
	//		// switch coordinate system by negating the scale and inverting the basis vector on the x-axis
	//		pfScales[a] = -pfScales[a];
	//		ppvBasis[a][0] = VectorFunction::Negate(ppvBasis[a][0]);

	//		fDet = -fDet;
	//	}

	//	fDet -= 1.0f;
	//	fDet *= fDet;

	//	if (internal::DECOMPOSE_EPSILON < fDet)
	//	{
	//		// Non-SRT matrix encountered
	//		return false;
	//	}

	//	// generate the quaternion from the matrix
	//	//outRotQuat[0] = XMQuaternionRotationMatrix(matTemp);
	//	return true;
	//}
	#pragma endregion Operator
	#pragma region Math
	/****************************************************************************
	*                       IsIdentity
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Matrix128Utility::IsIdentity(ConstMatrix128f matrix) noexcept
	*
	*  @brief     �P�ʍs��ł��邩�ǂ����𒲂ׂ܂�
	*
	*  @param[in] ConstMatrix128f matrix
	*
	*  @return �@ bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Matrix128Utility::IsIdentity(ConstMatrix128f matrix) noexcept
	{
		Vector128 temp1 = _mm_cmpeq_ps(matrix.Row[0], VECTOR_128F_IDENTITY_R0);
		Vector128 temp2 = _mm_cmpeq_ps(matrix.Row[1], VECTOR_128F_IDENTITY_R1);
		Vector128 temp3 = _mm_cmpeq_ps(matrix.Row[2], VECTOR_128F_IDENTITY_R2);
		Vector128 temp4 = _mm_cmpeq_ps(matrix.Row[3], VECTOR_128F_IDENTITY_R3);
		temp1 = _mm_and_ps(temp1, temp2);
		temp3 = _mm_and_ps(temp3, temp4);
		temp1 = _mm_and_ps(temp1, temp3);
		return (_mm_movemask_ps(temp1) == 0b1111);
	}

	/****************************************************************************
	*                       Translation
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Translation(const float offsetX, const float offsetY, const float offsetZ) noexcept
	*
	*  @brief     ���i�ړ��p�̍s����쐬���܂�.
	*
	*  @param[in] const float offsetX
	*  @param[in] const float offsetY
	*  @param[in] const float offsetZ
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Translation(const float offsetX, const float offsetY, const float offsetZ) noexcept
	{
		return Matrix128
		(
			VECTOR_128F_IDENTITY_R0.V,
			VECTOR_128F_IDENTITY_R1.V,
			VECTOR_128F_IDENTITY_R2.V,
			VectorFunction::Set(offsetX, offsetY, offsetZ, 1.0f)
		);
	}

	/****************************************************************************
	*                       Translation
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Translation(const float offsetX, const float offsetY, const float offsetZ) noexcept
	*
	*  @brief     ���i�ړ��p�̍s����쐬���܂�.
	*
	*  @param[in] Vector128Utility::ConstVector128 offset
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Translation(Vector128Utility::ConstVector128 offset) noexcept
	{
		return Matrix128
		(
			VECTOR_128F_IDENTITY_R0.V,
			VECTOR_128F_IDENTITY_R1.V,
			VECTOR_128F_IDENTITY_R2.V,
			VectorFunction::Select(VECTOR_128F_IDENTITY_R3.V, offset, VECTOR_128F_SELECT_1110.V)
		);
	}

	/****************************************************************************
	*                       Scaling
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Scaling(const float scaleX, const float scaleY, const float scaleZ) noexcept
	*
	*  @brief     �X�P�[�����O�p�̍s����쐬���܂�.
	*
	*  @param[in] const float scaleX, 
	*  @param[in] const float scaleY,
	*  @param[in] const float scaleZ
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Scaling(const float scaleX, const float scaleY, const float scaleZ) noexcept
	{
		return Matrix128
		(
			_mm_set_ps(0, 0, 0, scaleX),
			_mm_set_ps(0, 0, scaleY, 0),
			_mm_set_ps(0, scaleZ, 0, 0),
			VECTOR_128F_IDENTITY_R3.V
		);
	}

	/****************************************************************************
	*                       Scaling
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Scaling(Vector128Utility::ConstVector128 scale) noexcept
	*
	*  @brief     �X�P�[�����O�p�̍s����쐬���܂�.
	*
	*  @param[in] Vector128Utility::ConstVector128 scale
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Scaling(Vector128Utility::ConstVector128 scale) noexcept
	{
		return Matrix128
		(
			_mm_and_ps(scale, VECTOR_128U_MASK_X),
			_mm_and_ps(scale, VECTOR_128U_MASK_Y),
			_mm_and_ps(scale, VECTOR_128U_MASK_Z),
			VECTOR_128F_IDENTITY_R3.V
		);
	}

	inline void ScalarSinCos( float* pSin, float* pCos, float Value ) noexcept
	{
		Check(pSin);
		Check(pCos);

		// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
		float quotient = GM_1_DIV_2PI_FLOAT * Value;
		if (Value >= 0.0f)
		{
			quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
		}
		else
		{
			quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
		}
		float y = Value - GM_2PI_FLOAT * quotient;

		// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
		float sign;
		if (y > GM_PI_DIV2_FLOAT)
		{
			y = GM_PI_FLOAT - y;
			sign = -1.0f;
		}
		else if (y < -GM_PI_DIV2_FLOAT)
		{
			y = -GM_PI_FLOAT - y;
			sign = -1.0f;
		}
		else
		{
			sign = +1.0f;
		}

		float y2 = y * y;

		// 11-degree minimax approximation
		*pSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

		// 10-degree minimax approximation
		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		*pCos = sign * p;
	}

	/****************************************************************************
	*                       RotationX
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::RotationX(const float radian) noexcept
	*
	*  @brief     X���ɑ΂���rad�P�ʂŊp�x����]���܂�
	*
	*  @param[in] const float radian
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::RotationX(const float radian) noexcept
	{
		float sinAngle = 0.0f, cosAngle = 0.0f;
		ScalarSinCos(&sinAngle, &cosAngle, radian);

		Vector128 sinVector = _mm_set_ss(sinAngle);
		Vector128 cosVector = _mm_set_ss(cosAngle);

		// x = 0, y = cos, z = sin, w = 0
		cosVector = _mm_shuffle_ps(cosVector, sinVector, _MM_SHUFFLE(3, 0, 0, 3));

		Matrix128 matrix = {};
		matrix.Row[0] = VECTOR_128F_IDENTITY_R0.V;
		matrix.Row[1] = cosVector;

		// x = 0, y = sin, z = cos, w = 0
		cosVector = PERMUTE_PS(cosVector, _MM_SHUFFLE(3, 1, 2, 0));

		// x = 0, y = -sin, z = cos, w = 0
		cosVector = _mm_mul_ps(cosVector, VECTOR_128F_NEGATE_Y);
		matrix.Row[2] = cosVector;
		matrix.Row[3] = VECTOR_128F_IDENTITY_R3.V;
		return matrix;
	}

	/****************************************************************************
	*                       RotationY
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::RotationX(const float radian) noexcept
	*
	*  @brief     X���ɑ΂���rad�P�ʂŊp�x����]���܂�
	*
	*  @param[in] const float radian
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::RotationY(const float radian) noexcept
	{
		float sinAngle = 0.0f, cosAngle = 0.0f;
		ScalarSinCos(&sinAngle, &cosAngle, radian);

		Vector128 sinVector = _mm_set_ss(sinAngle);
		Vector128 cosVector = _mm_set_ss(cosAngle);

		// x = sin, y = 0, z = cos, w = 0
		sinVector = _mm_shuffle_ps(sinVector, cosVector, _MM_SHUFFLE(3, 0, 3, 0));

		Matrix128 matrix = {};
		matrix.Row[0] = sinVector;
		matrix.Row[1] = VECTOR_128F_IDENTITY_R1.V;

		// x = cos, y = 0, z = sin, w = 0
		sinVector = PERMUTE_PS(sinVector, _MM_SHUFFLE(3, 0, 1, 2));

		// x = cos, y = 0, z = -sin, w = 0
		sinVector = _mm_mul_ps(sinVector, VECTOR_128F_NEGATE_Z);
		matrix.Row[2] = sinVector;
		matrix.Row[3] = VECTOR_128F_IDENTITY_R3.V;
		return matrix;
	}

	/****************************************************************************
	*                       RotationZ
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::RotationX(const float radian) noexcept
	*
	*  @brief     X���ɑ΂���rad�P�ʂŊp�x����]���܂�
	*
	*  @param[in] const float radian
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::RotationZ(const float radian) noexcept
	{
		float sinAngle = 0.0f, cosAngle = 0.0f;
		ScalarSinCos(&sinAngle, &cosAngle, radian);

		Vector128 sinVector = _mm_set_ss(sinAngle);
		Vector128 cosVector = _mm_set_ss(cosAngle);

		// x = cos, y = sin, z = 0, w = 0
		cosVector = _mm_unpacklo_ps(cosVector, sinVector);

		Matrix128 matrix = {};
		matrix.Row[0] = cosVector;

		// x = sin, y = cos, z = 0, w = 0
		cosVector = PERMUTE_PS(cosVector, _MM_SHUFFLE(3, 2, 0, 1));

		// x = cos, y = -sin, z = 0, w = 0
		cosVector = _mm_mul_ps(cosVector, VECTOR_128F_NEGATE_X);

		matrix.Row[1] = cosVector;
		matrix.Row[2] = VECTOR_128F_IDENTITY_R2.V;
		matrix.Row[3] = VECTOR_128F_IDENTITY_R3.V;
		return matrix;
	}

	/****************************************************************************
	*                       RotationRollPitchYaw
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::RotationX(const float radian) noexcept
	*
	*  @brief     ���[(y axis)�A�s�b�`(x-axis)�A���[(z-axis)���g����rad�P�ʂŊp�x����]���܂�
	*
	*  @param[in] const float roll
	*  @param[in] const float pitch
	*  @param[in] const float yaw
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::RotationRollPitchYaw(const float roll, const float pitch, const float yaw) noexcept
	{
		return RotationRollPitchYaw(VectorFunction::Set(roll, pitch, yaw, 0.0f));
	}

	/****************************************************************************
	*                       RotationRollPitchYaw
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::RotationRollPitchYaw(Vector128Utility::ConstVector128 radian) noexcept
	*
	*  @brief     ���[(y axis)�A�s�b�`(x-axis)�A���[(z-axis)���g����rad�P�ʂŊp�x����]���܂�
	*
	*  @param[in] Vector128Utility::ConstVector128 rollPitchYaw
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::RotationRollPitchYaw(Vector128Utility::ConstVector128 rollPitchYaw) noexcept
	{
		static const Vector128f  Sign = { { { 1.0f, -1.0f, -1.0f, 1.0f } } };

		Vector128 sinAngles = {};
		Vector128 cosAngles = {};
		VectorFunction::SinCos(rollPitchYaw, &sinAngles, &cosAngles);

		Vector128 P0 = VectorPermute<GM_PERMUTE_1X, GM_PERMUTE_0Z, GM_PERMUTE_1Z, GM_PERMUTE_1X>(sinAngles, cosAngles);
		Vector128 Y0 = VectorPermute<GM_PERMUTE_0Y, GM_PERMUTE_1X, GM_PERMUTE_1X, GM_PERMUTE_1Y>(sinAngles, cosAngles);
		Vector128 P1 = VectorPermute<GM_PERMUTE_1Z, GM_PERMUTE_0Z, GM_PERMUTE_1Z, GM_PERMUTE_0Z>(sinAngles, cosAngles);
		Vector128 Y1 = VectorPermute<GM_PERMUTE_1Y, GM_PERMUTE_1Y, GM_PERMUTE_0Y, GM_PERMUTE_0Y>(sinAngles, cosAngles);
		Vector128 P2 = VectorPermute<GM_PERMUTE_0Z, GM_PERMUTE_1Z, GM_PERMUTE_0Z, GM_PERMUTE_1Z>(sinAngles, cosAngles);
		Vector128 P3 = VectorPermute<GM_PERMUTE_0Y, GM_PERMUTE_0Y, GM_PERMUTE_1Y, GM_PERMUTE_1Y>(sinAngles, cosAngles);
		Vector128 Y2 = VectorFunction::SplatX(sinAngles);
		Vector128 NS = VectorFunction::Negate(sinAngles);

		Vector128 Q0 = VectorFunction::Multiply(P0, Y0);
		Vector128 Q1 = VectorFunction::Multiply(P1, Sign.V);
		Q1 = VectorFunction::Multiply(Q1, Y1);
		Vector128 Q2 = VectorFunction::Multiply(P2, Y2);
		Q2 = VectorFunction::MultiplyAdd(Q2, P3, Q1);

		Vector128 V0 = VectorPermute<GM_PERMUTE_1X, GM_PERMUTE_0Y, GM_PERMUTE_1Z, GM_PERMUTE_0W>(Q0, Q2);
		Vector128 V1 = VectorPermute<GM_PERMUTE_1Y, GM_PERMUTE_0Z, GM_PERMUTE_1W, GM_PERMUTE_0W>(Q0, Q2);
		Vector128 V2 = VectorPermute<GM_PERMUTE_0X, GM_PERMUTE_1X, GM_PERMUTE_0W, GM_PERMUTE_0W>(Q0, NS);

		return Matrix128
		(
			VectorFunction::Select(VECTOR_128F_ZERO, V0, VECTOR_128F_SELECT_1110.V),
			VectorFunction::Select(VECTOR_128F_ZERO, V1, VECTOR_128F_SELECT_1110.V),
			VectorFunction::Select(VECTOR_128F_ZERO, V2, VECTOR_128F_SELECT_1110.V),
			VECTOR_128F_IDENTITY_R3
		);
	}

	/****************************************************************************
	*                       RotationNormal
	*************************************************************************//**
	*  @fn       inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::RotationNormal(Vector128Utility::ConstVector128 normalAxis, const float radian) noexcept
	*
	*  @brief     �@�������𒆐S��rad�P�ʂŊp�x����]���܂�
	*
	*  @param[in] Vector128Utility::ConstVector128 normalAxis
	*  @param[in] const float radian
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::RotationNormal(Vector128Utility::ConstVector128 normalAxis, const float radian) noexcept
	{
		// rad�Œ�`���ꂽ�p�x���g����, sin��cos�̒l���擾����
		float sinAngle = 0.0f, cosAngle = 0.0f;
		ScalarSinCos(&sinAngle, &cosAngle, radian);

		// �s��v�Z�̂��߂̒萔�x�N�g�����擾
		Vector128 C2 = _mm_set_ps1(1.0f - cosAngle);
		Vector128 C1 = _mm_set_ps1(cosAngle);
		Vector128 C0 = _mm_set_ps1(sinAngle);

		Vector128 N0 = PERMUTE_PS(normalAxis, _MM_SHUFFLE(3, 0, 2, 1));
		Vector128 N1 = PERMUTE_PS(normalAxis, _MM_SHUFFLE(3, 1, 0, 2));

		Vector128 V0 = _mm_mul_ps(C2, N0);
		V0 = _mm_mul_ps(V0, N1);

		// ��]�s��̌v�Z
		Vector128 R0 = _mm_mul_ps(C2, normalAxis);
		R0 = _mm_mul_ps(R0, normalAxis);
		R0 = _mm_add_ps(R0, C1);

		Vector128 R1 = _mm_mul_ps(C0, normalAxis);
		R1 = _mm_add_ps(R1, V0);
		Vector128 R2 = _mm_mul_ps(C0, normalAxis);
		R2 = _mm_sub_ps(V0, R2);

		V0 = _mm_and_ps(R0, VECTOR_128F_MASK_XYZ);
		Vector128 V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2, 1, 2, 0));
		V1 = PERMUTE_PS(V1, _MM_SHUFFLE(0, 3, 2, 1));
		Vector128 V2 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(0, 0, 1, 1));
		V2 = PERMUTE_PS(V2, _MM_SHUFFLE(2, 0, 2, 0));

		R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(1, 0, 3, 0));
		R2 = PERMUTE_PS(R2, _MM_SHUFFLE(1, 3, 2, 0));

		Matrix128 matrix = {};
		matrix.Row[0] = R2;

		R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(3, 2, 3, 1));
		R2 = PERMUTE_PS(R2, _MM_SHUFFLE(1, 3, 0, 2));
		matrix.Row[1] = R2;

		V2 = _mm_shuffle_ps(V2, V0, _MM_SHUFFLE(3, 2, 1, 0));
		matrix.Row[2] = V2;
		matrix.Row[3] = VECTOR_128F_IDENTITY_R3.V;
		return matrix;
	}

	/****************************************************************************
	*                       RotationAxis
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::RotationRollPitchYaw(Vector128Utility::ConstVector128 radian) noexcept
	*
	*  @brief     ���鎲�𒆐S��rad�P�ʂŊp�x����]���܂�
	*
	*  @param[in] Vector128Utility::ConstVector128 axis
	*
	*  @return �@�@Matrix128
	*****************************************************************************/

	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::RotationAxis(Vector128Utility::ConstVector128 axis, const float radian) noexcept
	{
		Check(!VectorFunction::EqualVector3(axis, VECTOR_128F_ZERO));
		Check(!VectorFunction::IsInfiniteVector3(axis));

		return RotationNormal(VectorFunction::NormalizeVector3(axis), radian);
	}

	/****************************************************************************
	*                       RotationQuaternion
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::RotationRollPitchYaw(Vector128Utility::ConstVector128 radian) noexcept
	*
	*  @brief     �N�H�[�^�j�I�����g����rad�P�ʂŊp�x����]���܂�
	*
	*  @param[in] Vector128Utility::ConstVector128 wuaternion
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::RotationQuaternion(Vector128Utility::ConstVector128 quaternion) noexcept
	{
		static const Vector128f Constant1110 = { { { 1.0f, 1.0f, 1.0f, 0.0f } } };

		Vector128 Q0 = _mm_add_ps(quaternion, quaternion);
		Vector128 Q1 = _mm_mul_ps(quaternion, Q0);

		Vector128 V0 = PERMUTE_PS(Q1, _MM_SHUFFLE(3, 0, 0, 1));
		V0 = _mm_and_ps(V0, VECTOR_128F_MASK_XYZ);
		Vector128 V1 = PERMUTE_PS(Q1, _MM_SHUFFLE(3, 1, 2, 2));
		V1 = _mm_and_ps(V1, VECTOR_128F_MASK_XYZ);
		Vector128 R0 = _mm_sub_ps(Constant1110, V0);
		R0 = _mm_sub_ps(R0, V1);

		V0 = PERMUTE_PS(quaternion, _MM_SHUFFLE(3, 1, 0, 0));
		V1 = PERMUTE_PS(Q0, _MM_SHUFFLE(3, 2, 1, 2));
		V0 = _mm_mul_ps(V0, V1);

		V1 = PERMUTE_PS(quaternion, _MM_SHUFFLE(3, 3, 3, 3));
		Vector128 V2 = PERMUTE_PS(Q0, _MM_SHUFFLE(3, 0, 2, 1));
		V1 = _mm_mul_ps(V1, V2);

		Vector128 R1 = _mm_add_ps(V0, V1);
		Vector128 R2 = _mm_sub_ps(V0, V1);

		V0 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(1, 0, 2, 1));
		V0 = PERMUTE_PS(V0, _MM_SHUFFLE(1, 3, 2, 0));
		V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2, 2, 0, 0));
		V1 = PERMUTE_PS(V1, _MM_SHUFFLE(2, 0, 2, 0));

		Q1 = _mm_shuffle_ps(R0, V0, _MM_SHUFFLE(1, 0, 3, 0));
		Q1 = PERMUTE_PS(Q1, _MM_SHUFFLE(1, 3, 2, 0));

		Matrix128 M = {};
		M.Row[0] = Q1;

		Q1 = _mm_shuffle_ps(R0, V0, _MM_SHUFFLE(3, 2, 3, 1));
		Q1 = PERMUTE_PS(Q1, _MM_SHUFFLE(1, 3, 0, 2));
		M.Row[1] = Q1;

		Q1 = _mm_shuffle_ps(V1, R0, _MM_SHUFFLE(3, 2, 1, 0));
		M.Row[2] = Q1;
		M.Row[3] = VECTOR_128F_IDENTITY_R3;
		return M;
	}

	///*----------------------------------------------------------------------
	//*  @brief : �e�̕ϊ��s����쐬���܂�.
	///*----------------------------------------------------------------------*/
	//inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::Shadow(Vector128Utility::ConstVector128 shadowPlane, Vector128Utility::ConstVector128 lightPosition) noexcept
	//{
	//	static const Vector128u select0001 = { {{ GM_SELECT_0, GM_SELECT_0, GM_SELECT_0, GM_SELECT_1}} };

	//	// shadowPlane���[���łȂ��A�������ʂłȂ����Ƃ��m�F����
	//	Check(!VectorFunction::EqualVector3(shadowPlane, VECTOR_128F_ZERO));
	//	Check(!VectorFunction::IsInfiniteVector4(shadowPlane));

	//	// shadowPlane�𐳋K��
	//	//Vector128 normalizedPlane = VectorFunction::NormalizeVector3()

	//	return Matrix128();
	//}

	/****************************************************************************
	*                       LookAtLH
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::LookAtLH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 focusPosition, Vector128Utility::ConstVector128 worldUp) noexcept
	*
	*  @brief     FocusPosition�Ɏ��_�������邽�߂̕ϊ��s����쐬���܂�(����n)
	*
	*  @param[in] Vector128Utility::ConstVector128 eyePosition (�J�����̈ʒu)
	*  @param[in] Vector128Utility::ConstVector128 focusPosition (���߂����Ώە��̂̈ʒu) 
	*  @param[in] Vector128Utility::ConstVector128 worldUp (���[���h�̏�����F�O�ς̂��߂Ɏg�p)
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::LookAtLH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 focusPosition, Vector128Utility::ConstVector128 worldUp) noexcept
	{
		return LookToLH(eyePosition, VectorFunction::Subtract(focusPosition, eyePosition), worldUp);
	}

	/****************************************************************************
	*                       LookAtRH
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::LookAtRH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 focusPosition, Vector128Utility::ConstVector128 worldUp) noexcept
	*
	*  @brief     FocusPosition�Ɏ��_�������邽�߂̕ϊ��s����쐬���܂�(�E��n)
	*
	*  @param[in] Vector128Utility::ConstVector128 eyePosition (�J�����̈ʒu)
	*  @param[in] Vector128Utility::ConstVector128 focusPosition (���߂����Ώە��̂̈ʒu)
	*  @param[in] Vector128Utility::ConstVector128 worldUp (���[���h�̏�����F�O�ς̂��߂Ɏg�p)
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::LookAtRH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 focusPosition, Vector128Utility::ConstVector128 worldUp) noexcept
	{
		return LookToRH(eyePosition, VectorFunction::Subtract(eyePosition, focusPosition), worldUp);
	}

	/****************************************************************************
	*                       LookToLH
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::LookToLH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 eyeDirection, Vector128Utility::ConstVector128 worldUp) noexcept
	*
	*  @brief     ���_��Ώە����Ɍ����邽�߂̕ϊ��s����쐬���܂�(����n)
	*
	*  @param[in] Vector128Utility::ConstVector128 eyePosition (�J�����̈ʒu)
	*  @param[in] Vector128Utility::ConstVector128 eyeDirection (�J���������������)
	*  @param[in] Vector128Utility::ConstVector128 worldUp (���[���h�̏�����F�O�ς̂��߂Ɏg�p)
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::LookToLH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 eyeDirection, Vector128Utility::ConstVector128 worldUp) noexcept
	{
		Check(!VectorFunction::EqualVector3(eyeDirection, VECTOR_128F_ZERO));
		Check(!VectorFunction::IsInfiniteVector3(eyeDirection));
		Check(!VectorFunction::EqualVector3(worldUp, VECTOR_128F_ZERO));
		Check(!VectorFunction::IsInfiniteVector3(worldUp));

		Vector128 normalizedForwardDirection = VectorFunction::NormalizeVector3(eyeDirection);
		Vector128 normalizedRightDirection   = VectorFunction::NormalizeVector3(VectorFunction::CrossVector3(worldUp, normalizedForwardDirection));
		Vector128 normalizedUpDirection      = VectorFunction::CrossVector3(normalizedForwardDirection, normalizedRightDirection);

		Vector128 negativeEyePosition = VectorFunction::Negate(eyePosition);

		// ���_�̈ʒu�ɑ΂���e�����̃I�t�Z�b�g���v�Z����
		Vector128 dotRight   = VectorFunction::Set(VectorFunction::DotVector3(normalizedRightDirection, negativeEyePosition));
		Vector128 dotUp      = VectorFunction::Set(VectorFunction::DotVector3(normalizedUpDirection, negativeEyePosition));
		Vector128 dotForward = VectorFunction::Set(VectorFunction::DotVector3(normalizedForwardDirection, negativeEyePosition));

		Matrix128 matrix
		(
			VectorFunction::Select(dotRight, normalizedRightDirection, VECTOR_128F_SELECT_1110.V),
			VectorFunction::Select(dotRight, normalizedRightDirection, VECTOR_128F_SELECT_1110.V),
			VectorFunction::Select(dotRight, normalizedRightDirection, VECTOR_128F_SELECT_1110.V),
			VectorFunction::Select(dotRight, normalizedRightDirection, VECTOR_128F_SELECT_1110.V)
		);

		return Transpose(matrix);
	}

	/****************************************************************************
	*                       LookToRH
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::LookToRH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 eyeDirection, Vector128Utility::ConstVector128 worldUp) noexcept
	*
	*  @brief     ���_��Ώە����Ɍ����邽�߂̕ϊ��s����쐬���܂�(�E��n)
	*
	*  @param[in] Vector128Utility::ConstVector128 eyePosition (�J�����̈ʒu)
	*  @param[in] Vector128Utility::ConstVector128 eyeDirection (�J���������������)
	*  @param[in] Vector128Utility::ConstVector128 worldUp (���[���h�̏�����F�O�ς̂��߂Ɏg�p)
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::LookToRH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 eyeDirection, Vector128Utility::ConstVector128 worldUp) noexcept
	{
		return LookToLH(eyePosition, Vector128Utility::Negate(eyeDirection), worldUp);
	}

	/****************************************************************************
	*                       PerspectiveLH
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::PerspectiveLH(const float viewWidth, const float viewHeight, const float nearZ, const float farZ) noexcept
	*
	*  @brief     �������e���s�����߂̕ϊ��s����쐬���܂� (����n)
	*
	*  @param[in] const float viewWidth
	*  @param[in] const float viewHeight
	*  @param[in] const float nearZ (��{0�`1)
	*  @param[in] const float farZ (��{0�`1)
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::PerspectiveLH(const float viewWidth, const float viewHeight, const float nearZ, const float farZ) noexcept
	{
		Check(nearZ > 0.0f && farZ > 0.0f);
		Check(!internal::ScalarNearEqual(viewWidth, 0.0f, 0.00001f));
		Check(!internal::ScalarNearEqual(viewHeight, 0.0f, 0.00001f));
		Check(!internal::ScalarNearEqual(farZ, nearZ, 0.00001f));

		// ������(�J����������͈�)�̃p�����[�^�v�Z
		const float twoNearZ = nearZ + nearZ;
		const float range    = farZ / (farZ - nearZ);

		// �ˉe�s��̊e�v�f���v�Z
		// stack�ɋL�^
		Vector128 memory = { twoNearZ / viewWidth, twoNearZ / viewHeight, range, -range * nearZ };

		// sse register���烁�������R�s�[ (��Œl���ς�邽��)
		Vector128 values = memory;
		Vector128 temp   = _mm_setzero_ps();

		// x�����̂݃R�s�[
		temp = _mm_move_ss(temp, values);

		// twoNearZ / viewWidth, 0, 0, 0
		Matrix128 result = {};
		result.Row[0] = temp;

		// 0, twoNearZ / viewHeight, 0, 0
		temp = values;
		temp = _mm_and_ps(temp, VECTOR_128U_MASK_Y);
		result.Row[1] = temp;

		// x = range, y = -range * nearZ, 0, 1.0f
		values = _mm_shuffle_ps(values, VECTOR_128F_IDENTITY_R3, _MM_SHUFFLE(3, 2, 3, 2));

		// 0, 0, range, 1.0f
		temp = _mm_setzero_ps();
		temp = _mm_shuffle_ps(temp, values, _MM_SHUFFLE(3, 0, 0, 0));
		result.Row[2] = temp;

		// 0, 0, -range * nearZ, 0
		temp = _mm_shuffle_ps(temp, values, _MM_SHUFFLE(2, 1, 0, 0));
		result.Row[3] = temp;
		return result;
	}

	/****************************************************************************
	*                       PerspectiveRH
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::PerspectiveRH(const float viewWidth, const float viewHeight, const float nearZ, const float farZ) noexcept
	*
	*  @brief     �������e���s�����߂̕ϊ��s����쐬���܂� (����n)
	*
	*  @param[in] const float viewWidth
	*  @param[in] const float viewHeight
	*  @param[in] const float nearZ (��{0�`1)
	*  @param[in] const float farZ (��{0�`1)
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::PerspectiveRH(const float viewWidth, const float viewHeight, const float nearZ, const float farZ) noexcept
	{
		Check(nearZ > 0.0f && farZ > 0.0f);
		Check(!internal::ScalarNearEqual(viewWidth, 0.0f, 0.00001f));
		Check(!internal::ScalarNearEqual(viewHeight, 0.0f, 0.00001f));
		Check(!internal::ScalarNearEqual(farZ, nearZ, 0.00001f));

		// ������(�J����������͈�)�̃p�����[�^�v�Z
		const float twoNearZ = nearZ + nearZ;
		const float range    = farZ / (nearZ - farZ);

		// �ˉe�s��̊e�v�f���v�Z
		// stack�ɋL�^
		Vector128 memory = { twoNearZ / viewWidth, twoNearZ / viewHeight, range, range * nearZ };

		// sse register���烁�������R�s�[ (��Œl���ς�邽��)
		Vector128 values = memory;
		Vector128 temp   = _mm_setzero_ps();

		// x�����̂݃R�s�[
		temp = _mm_move_ss(temp, values);

		// twoNearZ / viewWidth, 0, 0, 0
		Matrix128 result = {};
		result.Row[0] = temp;

		// 0, twoNearZ / viewHeight, 0, 0
		temp = values;
		temp = _mm_and_ps(temp, VECTOR_128U_MASK_Y);
		result.Row[1] = temp;

		// x = range, y = -range * nearZ, 0, -1.0f
		values = _mm_shuffle_ps(values, VECTOR_128F_NEGATIVE_IDENTITY_R3, _MM_SHUFFLE(3, 2, 3, 2));

		// 0, 0, range, -1.0f
		temp = _mm_setzero_ps();
		temp = _mm_shuffle_ps(temp, values, _MM_SHUFFLE(3, 0, 0, 0));
		result.Row[2] = temp;

		// 0, 0, -range * nearZ, 0
		temp = _mm_shuffle_ps(temp, values, _MM_SHUFFLE(2, 1, 0, 0));
		result.Row[3] = temp;
		return result;
	}

	/****************************************************************************
	*                       PerspectiveFovLH
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::PerspectiveFovLH(const float fovAngleY, const float aspectRatio, const float nearZ, const float farZ) noexcept
	*
	*  @brief     �������e������p���g���ĕϊ�����s����쐬���܂�  (����n)
	*
	*  @param[in] const float fovAngleY   (���������̎���p : rad)
	*  @param[in] const float aspectRatio (�A�X�y�N�g��)
	*  @param[in] const float nearZ (��{0�`1)
	*  @param[in] const float farZ (��{0�`1)
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::PerspectiveFovLH(const float fovAngleY, const float aspectRatio, const float nearZ, const float farZ) noexcept
	{
		Check(nearZ > 0.0f && farZ > 0.0f);
		Check(!internal::ScalarNearEqual(fovAngleY, 0.0f, 0.00001f * 2.0f));
		Check(!internal::ScalarNearEqual(aspectRatio, 0.0f, 0.00001f));
		Check(!internal::ScalarNearEqual(farZ, nearZ, 0.00001f));

		// fov�̊p�x�����Ƃ�sin, cos���擾
		float sinFOV = 0.0f;
		float cosFOV = 0.0f;
		ScalarSinCos(&sinFOV, &cosFOV, 0.5f * fovAngleY);

		// ������(�J����������͈�)�̃p�����[�^�v�Z
		const float range  = farZ / (farZ - nearZ);
		const float height = cosFOV / sinFOV;

		// �ˉe�s��̊e�v�f���v�Z
		// stack�ɋL�^
		Vector128 memory = { height / aspectRatio, height, range, -range * nearZ };

		// sse register���烁�������R�s�[ (��Œl���ς�邽��)
		Vector128 values = memory;
		Vector128 temp = _mm_setzero_ps();

		// x�����̂݃R�s�[
		temp = _mm_move_ss(temp, values);

		// cosFOV / sinFOV, 0, 0, 0
		Matrix128 result = {};
		result.Row[0] = temp;

		// 0, height/ aspectRatio, 0, 0
		temp = values;
		temp = _mm_and_ps(temp, VECTOR_128U_MASK_Y);
		result.Row[1] = temp;

		// x = range, y = -range * nearZ, 0, 1.0f
		temp = _mm_setzero_ps();
		values = _mm_shuffle_ps(values, VECTOR_128F_IDENTITY_R3, _MM_SHUFFLE(3, 2, 3, 2));

		// 0, 0, range, 1.0f
		temp = _mm_shuffle_ps(temp, values, _MM_SHUFFLE(3, 0, 0, 0));
		result.Row[2] = temp;

		// 0, 0, -range * nearZ, 0
		temp = _mm_shuffle_ps(temp, values, _MM_SHUFFLE(2, 1, 0, 0));
		result.Row[3] = temp;
		return result;
	}

	/****************************************************************************
	*                       PerspectiveFovRH
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::PerspectiveFovRH(const float fovAngleY, const float aspectRatio, const float nearZ, const float farZ) noexcept
	*
	*  @brief     �������e������p���g���ĕϊ�����s����쐬���܂�  (�E��n)
	*
	*  @param[in] const float fovAngleY   (���������̎���p : rad)
	*  @param[in] const float aspectRatio (�A�X�y�N�g��)
	*  @param[in] const float nearZ (��{0�`1)
	*  @param[in] const float farZ (��{0�`1)
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::PerspectiveFovRH(const float fovAngleY, const float aspectRatio, const float nearZ, const float farZ) noexcept
	{
		Check(nearZ > 0.0f && farZ > 0.0f);
		Check(!internal::ScalarNearEqual(fovAngleY, 0.0f, 0.00001f * 2.0f));
		Check(!internal::ScalarNearEqual(aspectRatio, 0.0f, 0.00001f));
		Check(!internal::ScalarNearEqual(farZ, nearZ, 0.00001f));

		// fov�̊p�x�����Ƃ�sin, cos���擾
		float sinFOV = 0.0f;
		float cosFOV = 0.0f;
		ScalarSinCos(&sinFOV, &cosFOV, 0.5f * fovAngleY);

		// ������(�J����������͈�)�̃p�����[�^�v�Z
		const float range  = farZ / (nearZ - farZ);
		const float height = cosFOV / sinFOV;

		// �ˉe�s��̊e�v�f���v�Z
		// stack�ɋL�^
		Vector128 memory = { height / aspectRatio, height, range, range * nearZ };

		// sse register���烁�������R�s�[ (��Œl���ς�邽��)
		Vector128 values = memory;
		Vector128 temp = _mm_setzero_ps();

		// x�����̂݃R�s�[
		temp = _mm_move_ss(temp, values);

		// cosFOV / sinFOV, 0, 0, 0
		Matrix128 result = {};
		result.Row[0] = temp;

		// 0, height/ aspectRatio, 0, 0
		temp = values;
		temp = _mm_and_ps(temp, VECTOR_128U_MASK_Y);
		result.Row[1] = temp;

		// x = range, y = -range * nearZ, 0, 1.0f
		temp = _mm_setzero_ps();
		values = _mm_shuffle_ps(values, VECTOR_128F_NEGATIVE_IDENTITY_R3, _MM_SHUFFLE(3, 2, 3, 2));

		// 0, 0, range, -1.0f
		temp = _mm_shuffle_ps(temp, values, _MM_SHUFFLE(3, 0, 0, 0));
		result.Row[2] = temp;

		// 0, 0, range * nearZ, 0
		temp = _mm_shuffle_ps(temp, values, _MM_SHUFFLE(2, 1, 0, 0));
		result.Row[3] = temp;
		return result;
	}

	/****************************************************************************
	*                      OrthographicLH
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::OrthographicLH(const float viewWidth, const float viewHeight, const float nearZ, const float farZ) noexcept
	*
	*  @brief     ���s���e������p���g���ĕϊ�����s����쐬���܂�  (����n)
	*
	*  @param[in] const float viewWidth
	*  @param[in] const float viewHeight
	*  @param[in] const float nearZ (��{0�`1)
	*  @param[in] const float farZ (��{0�`1)
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::OrthographicLH(const float viewWidth, const float viewHeight, const float nearZ, const float farZ) noexcept
	{
		Check(!internal::ScalarNearEqual(viewWidth, 0.0f, 0.00001f));
		Check(!internal::ScalarNearEqual(viewHeight, 0.0f, 0.00001f));
		Check(!internal::ScalarNearEqual(farZ, nearZ, 0.00001f));

		// ������(�J����������͈�)�̃p�����[�^�v�Z
		const float range    = 1.0f / (farZ - nearZ);

		// �ˉe�s��̊e�v�f���v�Z
		// stack�ɋL�^
		Vector128 memory = { 2.0f / viewWidth, 2.0f / viewHeight, range, -range * nearZ };

		// sse register���烁�������R�s�[ (��Œl���ς�邽��)
		Vector128 values = memory;
		Vector128 temp   = _mm_setzero_ps();

		// x�����̂݃R�s�[
		temp = _mm_move_ss(temp, values);

		// 2.0f / viewWidth, 0, 0, 0
		Matrix128 result = {};
		result.Row[0] = temp;

		// 0, 2.0f / viewHeight, 0, 0
		temp = values;
		temp = _mm_and_ps(temp, VECTOR_128U_MASK_Y);
		result.Row[1] = temp;

		// x = range, y = range * nearZ, 0, 1.0f
		temp = _mm_setzero_ps();
		values = _mm_shuffle_ps(values, VECTOR_128F_IDENTITY_R3, _MM_SHUFFLE(3, 2, 3, 2));

		// 0, 0, range, 1.0f
		temp = _mm_shuffle_ps(temp, values, _MM_SHUFFLE(2, 0, 0, 0));
		result.Row[2] = temp;

		// 0, 0, -range * nearZ, 0
		temp = _mm_shuffle_ps(temp, values, _MM_SHUFFLE(3, 1, 0, 0));
		result.Row[3] = temp;
		return result;
	}

	/****************************************************************************
	*                      OrthographicRH
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::OrthographicRH(const float viewWidth, const float viewHeight, const float nearZ, const float farZ) noexcept
	*
	*  @brief     ���s���e������p���g���ĕϊ�����s����쐬���܂�  (�E��n)
	*
	*  @param[in] const float viewWidth
	*  @param[in] const float viewHeight
	*  @param[in] const float nearZ (��{0�`1)
	*  @param[in] const float farZ (��{0�`1)
	*
	*  @return �@�@Matrix128
	*****************************************************************************/
	inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::OrthographicRH(const float viewWidth, const float viewHeight, const float nearZ, const float farZ) noexcept
	{
		Check(!internal::ScalarNearEqual(viewWidth, 0.0f, 0.00001f));
		Check(!internal::ScalarNearEqual(viewHeight, 0.0f, 0.00001f));
		Check(!internal::ScalarNearEqual(farZ, nearZ, 0.00001f));

		// ������(�J����������͈�)�̃p�����[�^�v�Z
		const float range = 1.0f / (nearZ - farZ);

		// �ˉe�s��̊e�v�f���v�Z
		// stack�ɋL�^
		Vector128 memory = { 2.0f / viewWidth, 2.0f / viewHeight, range, range * nearZ };

		// sse register���烁�������R�s�[ (��Œl���ς�邽��)
		Vector128 values = memory;
		Vector128 temp = _mm_setzero_ps();

		// x�����̂݃R�s�[
		temp = _mm_move_ss(temp, values);

		// 2.0f / viewWidth, 0, 0, 0
		Matrix128 result = {};
		result.Row[0] = temp;

		// 0, 2.0f / viewHeight, 0, 0
		temp = values;
		temp = _mm_and_ps(temp, VECTOR_128U_MASK_Y);
		result.Row[1] = temp;

		// x = range, y = range * nearZ, 0, 1.0f
		temp = _mm_setzero_ps();
		values = _mm_shuffle_ps(values, VECTOR_128F_IDENTITY_R3, _MM_SHUFFLE(3, 2, 3, 2));

		// 0, 0, range, 1.0f
		temp = _mm_shuffle_ps(temp, values, _MM_SHUFFLE(2, 0, 0, 0));
		result.Row[2] = temp;

		// 0, 0, -range * nearZ, 0
		temp = _mm_shuffle_ps(temp, values, _MM_SHUFFLE(3, 1, 0, 0));
		result.Row[3] = temp;
		return result;
	}
	#pragma endregion Math
#pragma endregion Implement
}
#endif
#endif