/////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdVector256SSE.hpp
///             @brief  SSE�ɂ��Simd���Z�ł�.
///                     Vector256�^�̎��� 
///             @author toide
///             @date   2024/03/11 2:25:36
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_VECTOR_256_AVX_HPP
#define GM_SIMD_VECTOR_256_AVX_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMSimdMacros.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_CPU_INSTRUCTION_AVX && !defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
#include <intrin.h>
#include <immintrin.h>
#include <math.h>
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm::simd::avx
{
	/*----------------------------------------------------------------------
	*         256 bit�̐����^��double�^���i�[�\�ȃx�N�g�����쐬����
	*----------------------------------------------------------------------*/
	using Vector256 = __m256d;

	/*----------------------------------------------------------------------
	*        �萔�Ƃ��Ă̕ϊ��p, double�|�C���^�ɕϊ�������, m256�̊e�^�ɕϊ�������
	*----------------------------------------------------------------------*/
	ALIGNED_STRUCT(32) Vector256d
	{
		union Union256
		{
			double D[4];
			struct Two128
			{
				__m128d XY;
				__m128d ZW;
			} t;
			__m256d V;
		} u;

		inline operator const double* () const noexcept { return u.D; }
		inline operator __m256d() const noexcept { return u.V; }
		inline Vector256d& operator=(const __m256d& v) { u.V = v;}
	};

	constexpr double GM_PI_DOUBLE        = 3.141592653589793238463;
	constexpr double GM_2PI_DOUBLE       = 6.283185307179586476925;
	constexpr double GM_1_DIV_PI_DOUBLE  = 0.3183098861837906715378;
	constexpr double GM_1_DIV_2PI_DOUBLE = 0.1591549430918953357689;
	constexpr double GM_PI_DIV2_DOUBLE   = 1.570796326794896619231;
	constexpr double GM_PI_DIV4_DOUBLE   = 0.7853981633974483096157;
	GLOBAL_CONST Vector256 VECTOR_256D_ONE               = { {  1.0, 1.0, 1.0, 1.0  } };
	GLOBAL_CONST Vector256 VECTOR_256D_ZERO              = { {  0.0, 0.0, 0.0, 0.0  } };
	GLOBAL_CONST Vector256 VECTOR_256D_NEGATIVE_ONE      = { {  -1.0, -1.0, -1.0, -1.0 } };
	GLOBAL_CONST Vector256 VECTOR_256D_MASK_SIGN         = { { (double)~(gu::uint64(1) << 63), (double)~(gu::uint64(1) << 63), (double)~(gu::uint64(1) << 63), (double)~(gu::uint64(1) << 63) } };
	GLOBAL_CONST Vector256 VECTOR_256U_MASK_ABS          = { { (double)~gu::uint64(0), (double)~gu::uint64(0), (double)~gu::uint64(0), (double)gu::uint64(0)}};
	GLOBAL_CONST Vector256 VECTOR_256U_NEGATIVE_ONE_MASK = { { (double)0xFFFFFFFFFFFFFFFF, (double)0xFFFFFFFFFFFFFFFF, (double)0xFFFFFFFFFFFFFFFF, (double)0xFFFFFFFFFFFFFFFF } };
	GLOBAL_CONST Vector256 VECTOR_256D_ONE_HALF          = { { 0.5, 0.5, 0.5, 0.5 } };
	GLOBAL_CONST Vector256 VECTOR_256D_HALF_PI           = { {  GM_PI_DIV2_DOUBLE  , GM_PI_DIV2_DOUBLE  , GM_PI_DIV2_DOUBLE , GM_PI_DIV2_DOUBLE } };
	GLOBAL_CONST Vector256 VECTOR_256D_PI                = { {  GM_PI_DOUBLE       , GM_PI_DOUBLE       , GM_PI_DOUBLE , GM_PI_DOUBLE  } };
	GLOBAL_CONST Vector256 VECTOR_256D_NEGATIVE_PI       = { {  -GM_PI_DOUBLE      , -GM_PI_DOUBLE      , -GM_PI_DOUBLE , -GM_PI_DOUBLE  } };
	GLOBAL_CONST Vector256 VECTOR_256D_RECIPROCAL_PI     = { {  GM_1_DIV_PI_DOUBLE , GM_1_DIV_PI_DOUBLE , GM_1_DIV_PI_DOUBLE , GM_1_DIV_PI_DOUBLE  } };
	GLOBAL_CONST Vector256 VECTOR_256D_RECIPROCAL_2PI    = { {  GM_1_DIV_2PI_DOUBLE, GM_1_DIV_2PI_DOUBLE, GM_1_DIV_2PI_DOUBLE, GM_1_DIV_2PI_DOUBLE } };
	GLOBAL_CONST Vector256 VECTOR_256D_2PI               = { {  GM_2PI_DOUBLE      , GM_2PI_DOUBLE      , GM_2PI_DOUBLE, GM_2PI_DOUBLE} };
	GLOBAL_CONST Vector256 VECTOR_256U_MASK_X            = { { (double)0xFFFFFFFFFFFFFFFF, (double)0x0000000000000000, (double)0x0000000000000000, (double)0x0000000000000000  } };
	GLOBAL_CONST Vector256 VECTOR_256U_MASK_Y            = { { (double)0x0000000000000000, (double)0xFFFFFFFFFFFFFFFF, (double)0x0000000000000000, (double)0x0000000000000000  } };
	GLOBAL_CONST Vector256 VECTOR_256U_MASK_Z            = { {(double)0x0000000000000000, (double)0x0000000000000000, (double)0xFFFFFFFFFFFFFFFF, (double)0x0000000000000000  } };
	GLOBAL_CONST Vector256 VECTOR_256U_MASK_W            = { { (double)0x0000000000000000, (double)0x0000000000000000, (double)0x0000000000000000, (double)0xFFFFFFFFFFFFFFFF  } };
	GLOBAL_CONST Vector256 VECTOR_256U_MASK_XYZ          = { { (double)0xFFFFFFFFFFFFFFFF, (double)0xFFFFFFFFFFFFFFFF, (double)0xFFFFFFFFFFFFFFFF, (double)0x0000000000000000  } };
	GLOBAL_CONST Vector256 VECTOR_256D_IDENTITY_R0       = { { 1.0, 0.0, 0.0, 0.0 } };
	GLOBAL_CONST Vector256 VECTOR_256D_IDENTITY_R1       = { { 0.0, 1.0, 0.0, 0.0 } };
	GLOBAL_CONST Vector256 VECTOR_256D_IDENTITY_R2       = { { 0.0, 0.0, 1.0, 0.0 } };
	GLOBAL_CONST Vector256 VECTOR_256D_IDENTITY_R3       = { { 0.0, 0.0, 0.0, 1.0 } };
	GLOBAL_CONST Vector256 VECTOR_256D_INFINITY          = { { 0x7FF0000000000000, 0x7FF0000000000000, 0x7FF0000000000000, 0x7FF0000000000000 } };
	GLOBAL_CONST Vector256 VECTOR_256D_QNAN              = { { 0x7FF8000000000000, 0x7FF8000000000000, 0x7FF8000000000000, 0x7FF8000000000000 } };


	/****************************************************************************
	*				  			  Vector256Utility
	****************************************************************************/
	/* @class     Vector256Utility
	*  @brief     SSE�̃o�[�W�����ɂ��Simd���Z
	*****************************************************************************/
	class Vector256Utility
	{
	public:
		#if ( defined(_M_IX86) || defined(_M_ARM) || defined(_M_ARM64) || USE_VECTOR_CALL || __i386__ || __arm__ || __aarch64__ )
			using ConstVector256 = const Vector256;
		#else
			using ConstVector256 = const Vector256&;
		#endif
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		#pragma region Setter
		/*----------------------------------------------------------------------
		*  @brief : �[������������Vector256��Ԃ��֐�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Zero() noexcept;

		/*----------------------------------------------------------------------
		*  @brief : double4����Vector256�N���X�𐶐�����
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Set(const double x, const double y, const double z, const double w) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : double1����Vector256�N���X�𐶐�����
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Set(const double value) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : Int4����Vector256�N���X�𐶐�����
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION SetInt(const gu::uint64 x, const gu::uint64 y, const gu::uint64 z, const gu::uint64 w) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : Float���g����Vector��X(, Y, Z, W)�v�f�ɒl��������
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION SetX(ConstVector256 vector, const double x) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION SetY(ConstVector256 vector, const double y) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION SetZ(ConstVector256 vector, const double z) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION SetW(ConstVector256 vector, const double w) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �x�N�g�����g����double�z��ɑ������
		*----------------------------------------------------------------------*/
		__forceinline static void SIMD_CALL_CONVENTION StoreDouble (double* destination, ConstVector256 source) noexcept;
		__forceinline static void SIMD_CALL_CONVENTION StoreDouble2(double* destination, ConstVector256 source) noexcept;
		__forceinline static void SIMD_CALL_CONVENTION StoreDouble3(double* destination, ConstVector256 source) noexcept;
		__forceinline static void SIMD_CALL_CONVENTION StoreDouble4(double* destination, ConstVector256 source) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : X(, Y, Z, W)���̗v�f�Ɋ�Â���, �S�Ă̗v�f�ɓ����l��������Vector256�N���X���쐬����
		*           X,Y,Z,W  : �e�v�f
		*           One      : 1, 1, 1, 1
		*           Infinity : INF, INF, INF, INF
		*           Epsilon  : doubleEpsilon
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION SplatX(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION SplatY(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION SplatZ(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION SplatW(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION SplatOne() noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION SplatInfinity() noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION SplatEpsilon() noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION SplatSignMask() noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �x�N�g������ёւ��܂�.index���w�肷��Ƃ���index�ɂ�����Vector4�^���擾
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Swizzle(ConstVector256 vector,
			const gu::uint64 xIndex, const gu::uint64 yIndex,
			const gu::uint64 zIndex, const gu::uint64 wIndex) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g������v�f�𒊏o��, �V�����x�N�g�����쐬����
		*           0�`3 : a�����x, y, z, w
		*           4�`7 : b�����x, y, z, w
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Permute(ConstVector256 a, ConstVector256,
			const gu::uint64 xIndex, const gu::uint64 yIndex,
			const gu::uint64 zIndex, const gu::uint64 wIndex) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g������v�f�𒊏o��, �V�����x�N�g�����쐬����
		*           control vector��0�̏ꍇ��left����, ��0�̏ꍇ��right���炻�ꂼ�ꒊ�o���s���܂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Select(ConstVector256 left, ConstVector256 right, ConstVector256 control) noexcept;
		#pragma endregion Setter

		#pragma region Getter
		/*----------------------------------------------------------------------
		*  @brief : FPU���W�X�^�Ɋi�[���ꂽX(, Y, Z, W)���̗v�f�����o��
		*----------------------------------------------------------------------*/
		__forceinline static double SIMD_CALL_CONVENTION GetX(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION GetY(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION GetZ(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION GetW(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : double�̔z����g���ăx�N�g���Ɋi�[����
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION LoadDouble (const double* source) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION LoadDouble2(const double* source) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION LoadDouble3(const double* source) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION LoadDouble4(const double* source) noexcept;

		#pragma endregion Getter

		#pragma region Operator
		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g���̊e�v�f���m�̘a���Z�o���܂�. (left + right)
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Add(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g���̊e�v�f���m�̍����Z�o���܂�. (left - right)
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Subtract(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g���̊e�v�f���m�̐ς��Z�o���܂�. (left * right)
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Multiply(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g���̊e�v�f���m�̏��Z���Z�o���܂�. (left / right)
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Divide(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g���̊e�v�f���m�̂��܂���Z�o���܂�. (left % right)
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Mod(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief :  subtractLeft - (multiplyLeft * multiplyRight)���Z�o���܂�.
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION NegativeMultiplySubtract(ConstVector256 multiplyLeft, ConstVector256 multiplyRight, ConstVector256 subtractLeft) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : multiplyLeft * multiplyRight + add���Z�o���܂�.
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION MultiplyAdd(ConstVector256 multiplyLeft, ConstVector256 multiplyRight, ConstVector256 add) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �x�N�g���̊e�v�f���m�̋t�����Z�o���܂�. (1.0f / vector)
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Reciprocal(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : double�l���g���Ċe�v�f�Ɋ|���܂� scale * vector
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Scale(ConstVector256 vector, const double scale) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ��̃x�N�g�������������ǂ����𒲂ׂ܂�
		*           (Each�͊e�v�f���ɓ��������ǂ����𒲂ׂ܂�.)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION EqualVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION EqualVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION EqualVector4(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION EqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ��̃x�N�g�����������Ȃ����𒲂ׂ܂�
		*           (Each�͊e�v�f���ɓ��������ǂ����𒲂ׂ܂�.)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualVector4(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION NotEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ��̃x�N�g�������ׂē��������ǂ����𒲂ׂ܂�.
		*           ��r���ɂ͕����Ȃ�����(U32)�Ƃ��Ĕ�r���܂�.
		*           (Each�͊e�v�f���Ƃɓ��������ǂ����𒲂ׂ܂�)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION EqualAsIntVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION EqualAsIntVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION EqualAsIntVector4(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION EqualAsIntVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ��̃x�N�g������ł��������Ȃ����ǂ����𒲂ׂ܂�.
		*           ��r���ɂ͕����Ȃ�����(U32)�Ƃ��Ĕ�r���܂�.
		*           (Each�͊e�v�f���Ƃɓ��������ǂ����𒲂ׂ܂�)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualAsIntVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualAsIntVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualAsIntVector4(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION NotEqualAsIntVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���͈͓��Œl�����������𒲂ׂ� (left - epsilon <= right <= left + epsilon)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector2   (ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector3   (ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector4   (ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION NearEqualVectorEach(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̕����S�Ă̗v�f�ɂ����đ傫�����𒲂ׂ܂� (left > right)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterVector2   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterVector3   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterVector4   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION GreaterVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̕����S�Ă̗v�f�ɂ����đ傫��or���������𒲂ׂ܂� (left >= right)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterOrEqualVector2   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterOrEqualVector3   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterOrEqualVector4   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION GreaterOrEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̕����S�Ă̗v�f�ɂ����ď��������𒲂ׂ܂� (left < right)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION LessVector2   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessVector3   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessVector4   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION LessVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̕����S�Ă̗v�f�ɂ����ď�����or���������𒲂ׂ܂� (left <= right)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION LessOrEqualVector2   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessOrEqualVector3   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessOrEqualVector4   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION LessOrEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �w��͈͓��ɂ��邩�ǂ��� -bounds <= vector <= +bounds
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION InBoundsVector2(ConstVector256 vector, ConstVector256 bounds) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION InBoundsVector3(ConstVector256 vector, ConstVector256 bounds) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION InBoundsVector4(ConstVector256 vector, ConstVector256 bounds) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION InBoundsVectorEach(ConstVector256 vector, ConstVector256 bounds) noexcept;

		#pragma endregion Operator

		#pragma region Bit
		/*----------------------------------------------------------------------
		*  @brief : �r�b�g�P�ʂ̘_���ς̉��Z���s���܂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION AndInt(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �r�b�g�P�ʂ̘_���a�̉��Z���s���܂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION OrInt(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �r�b�g�P�ʂ̘_���ϔے�̉��Z���s���܂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION NorInt(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �r�b�g�P�ʂ̔r���I�_���a�̉��Z���s���܂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION XorInt(ConstVector256 left, ConstVector256 right) noexcept;

		#pragma endregion Bit

		#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : �e�v�f�Ƀ}�C�i�X����������̂�Ԃ�.
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Negate(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2���m����(�e�v�f��2��a�����[�g��������Ƃ��̃x�N�g���̑傫��)���Z�o
		*----------------------------------------------------------------------*/
		__forceinline static double SIMD_CALL_CONVENTION LengthVector2(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION LengthVector3(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION LengthVector4(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2���m�����̓����Z�o
		*----------------------------------------------------------------------*/
		__forceinline static double SIMD_CALL_CONVENTION LengthSquaredVector2(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION LengthSquaredVector3(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION LengthSquaredVector4(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���ς��Z�o (������, �Ԃ�l��double�ł͂Ȃ�Vector256�^�ŕԂ���܂��̂�, ��XGetX�ȂǂőΉ����Ă�������.)
		*----------------------------------------------------------------------*/
		__forceinline static double SIMD_CALL_CONVENTION DotVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION DotVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION DotVector4(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �O�ς��Z�o left, right�ɐ����ȃx�N�g����Ԃ�
		*           2�����̏ꍇ�͕��s�l�ӌ`�̖ʐς�Ԃ��Ƃ����Ӗ�����
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION CrossVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION CrossVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION CrossVector4(ConstVector256 first, ConstVector256 second, ConstVector256 third) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION NormalizeVector2(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION NormalizeVector3(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION NormalizeVector4(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief     ���˃x�N�g�����Z�o
		*  @param[in] ConstVector256 : input  (���˃x�N�g��)
		*  @param[in] ConstVector256 : normal (���˂��s���@���x�N�g��)
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION ReflectVector2(ConstVector256 input, ConstVector256 normal) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION ReflectVector3(ConstVector256 input, ConstVector256 normal) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION ReflectVector4(ConstVector256 input, ConstVector256 normal) noexcept;

		/*----------------------------------------------------------------------
		*  @brief     ���܃x�N�g�����Z�o
		*  @param[in] ConstVector256 : input  (���˃x�N�g��)
		*  @param[in] ConstVector256 : normal (���˂��s���@���x�N�g��)
		*  @param[in] const double    : refractionIndex (���ܗ�)
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION RefractVector2(ConstVector256 input, ConstVector256 normal, const double refractionIndex) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION RefractVector3(ConstVector256 input, ConstVector256 normal, const double refractionIndex) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION RefractVector4(ConstVector256 input, ConstVector256 normal, const double refractionIndex) noexcept;

		__forceinline static Vector256 SIMD_CALL_CONVENTION RefractVector2V(ConstVector256 input, ConstVector256 normal, ConstVector256 refractionIndex) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION RefractVector3V(ConstVector256 input, ConstVector256 normal, ConstVector256 refractionIndex) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION RefractVector4V(ConstVector256 input, ConstVector256 normal, ConstVector256 refractionIndex) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���ɑ傫���l��
		*----------------------------------------------------------------------*/
		__forceinline static bool IsInfiniteVector2(ConstVector256 vector) noexcept;
		__forceinline static bool IsInfiniteVector3(ConstVector256 vector) noexcept;
		__forceinline static bool IsInfiniteVector4(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�ɍ�������������̂�Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Sqrt(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�̋t�������, ����ɍ�������������̂�Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION ReciprocalSqrt(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��sin, cos, tan��Ԃ��܂� [rad]
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Sin(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION Cos(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION Tan(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��arcsin, arccos, arctan��Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION ArcSin(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION ArcCos(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION ArcTan(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��sinH, cosH, tanH��Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION SinH(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION CosH(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION TanH(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�ɑ΂���sin������cos���������o���܂�
		*----------------------------------------------------------------------*/
		__forceinline static void SIMD_CALL_CONVENTION SinCos(ConstVector256 input, Vector256* sin, Vector256* cos) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��2^{vector}��Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Exp2(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��10^{vector}��Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Exp10(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��e^{vector}��Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION ExpE(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��log2{vector}��Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Log2(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��log10{vector}��Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Log10(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��loge{vector}��Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION LogE(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��Pow��Ԃ��܂�
		*           base^{power}
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Pow(ConstVector256 base, ConstVector256 power) noexcept;

		/*----------------------------------------------------------------------
		*  @brief      ���`��Ԃ�Ԃ��܂�
		*  @param[in]  t = 0�̎��̒l
		   @param[in]  t = 1�̎��̒l
		   @param[in]  t : ���`��Ԃ̊���
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Lerp(ConstVector256 start, ConstVector256 end, const double t) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION LerpV(ConstVector256 start, ConstVector256 end, ConstVector256 t) noexcept;


		/*----------------------------------------------------------------------
		*  @brief      �G���~�[�g��Ԃ�Ԃ��܂�
		*  @param[in]  �n�_�̈ʒu
		   @param[in]  �n�_�ɂ�����X�v���C���̐ڐ���\���x�N�g��
		   @param[in]  �I�_�̈ʒu
		   @param[in]  �I�_�ɂ�����X�v���C���̐ڐ���\���x�N�g��
		   @param[in]  t : �G���~�[�g��Ԃ̊���
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Hermite(ConstVector256 startPosition, ConstVector256 startTangent, ConstVector256 endPosition, ConstVector256 endTangent, const double t) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION HermiteV(ConstVector256 startPosition, ConstVector256 startTangent, ConstVector256 endPosition, ConstVector256 endTangent, ConstVector256 t) noexcept;

		/*----------------------------------------------------------------------
		*  @brief      CatMull-Rom�X�v���C����Ԃ̌��ʂ�Ԃ��܂��B4�̐���_��S�Ēʂ�X�v���C�����
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION CatMullRom(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, ConstVector256 position3, const double t   ) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION CatMullRomV(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, ConstVector256 position3, ConstVector256 t) noexcept;

		/*----------------------------------------------------------------------
		*  @brief      �O�p�`�̏d�S�d�݈ʒu���Z�o
		*              (f>=0 && g>=0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�͎O�p�`�� Position0>Position1>Position2 �̓����ɂ���܂�>�B
                       (f==0 && g>=0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�� Position0>Position2 �s�ڂɂ���܂�>�B
                       (f>=0 && g==0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�� Position0>Position1> �s�ڂɂ���܂��B
                       (f>=0 && g>=0 && 1-f-g==0) �̏ꍇ�A�|�C���g�� Position1>Position2 �s�ڂɂ���܂�>�B
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION BaryCentric (ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, const double f, const double g) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION BaryCentricV(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, ConstVector256 f, ConstVector256 g) noexcept;
		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�ɂ��čŏ��l�ƂȂ����I�����܂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Min(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�ɂ��čő�l�ƂȂ����I�����܂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Max(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �ł��߂������Ɋۂ߂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Round(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �؂�̂� (�w�肵�����l�̒[����؂�̂Ă������Ɋۂ߂�B���������0�̕����ɂ��鎟�̐���
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Truncate(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̕����Ɋۂ߂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Floor(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �؂�グ ��������Ő��̕����ɂ��鎟�̐����Ɋۂ߂�
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Ceiling(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �w��͈͓��Ő��l��ݒ肷��
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Clamp(ConstVector256 vector, ConstVector256 min, ConstVector256 max) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 1�ȏ�ɂ͍s���Ȃ��悤�ɂ���
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Saturate(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ��Βl
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Abs(ConstVector256 vector) noexcept;

		#pragma endregion Math

		#pragma region Angle
		/*----------------------------------------------------------------------
		*  @brief : - pi <= theta <= pi�͈̔͂Ɏ��߂�悤�ɂ��܂�.
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION ModAngles(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : - pi <= theta <= pi�͈̔͂Ɏ��߂�悤�ɂ�����ő����Z���s���܂�.
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION AddAngles(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : - pi <= theta <= pi�͈̔͂Ɏ��߂�悤�ɂ�����ň����Z���s���܂�.
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION SubtractAngles(ConstVector256 left, ConstVector256 right) noexcept;
		#pragma endregion Angle
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
	/****************************************************************************
	*                       Zero
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Zero()noexcept
	*
	*  @brief     �[������������Vector256��Ԃ��֐�
	*
	*  @param[in] void
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Zero() noexcept
	{
		return _mm256_setzero_pd(); // 4�̒P���x���������_�l���N���A����
	}

	/****************************************************************************
	*                       Set
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Set(const double x, const double y, const double z, const double w) noexcept
	*
	*  @brief     double4����Vector256�N���X�𐶐�����
	*
	*  @param[in] const double x
	*  @param[in] const double y
	*  @param[in] const double z
	*  @param[in] const double w
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Set(const double x, const double y, const double z, const double w) noexcept
	{
		// 4�̒P���x���������_�l��ݒ肷��
		// ���g���G���f�B�A���ɂ�����ł�
		return _mm256_set_pd(w, z, y, x);
	}

	/****************************************************************************
	*                       Set
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Set(const double value) noexcept
	*
	*  @brief     double1����Vector256�N���X�𐶐�����
	*
	*  @param[in] const double value
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Set(const double value) noexcept
	{
		return _mm256_set1_pd(value);
	}

	/****************************************************************************
	*                       SetX
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION SetX(ConstVector256 vector, const double x) noexcept
	*
	*  @brief     Float���g����Vector��X�ɒl��������
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] const double x
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SetX(ConstVector256 vector, const double x) noexcept
	{
		return _mm256_set_pd(vector.m256d_f64[3], vector.m256d_f64[2], vector.m256d_f64[1], x);
	}

	/****************************************************************************
	*                       SetY
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION SetY(ConstVector256 vector, const double y) noexcept
	*
	*  @brief     Float���g����Vector��Y�ɒl��������
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] const double y
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SetY(ConstVector256 vector, const double y) noexcept
	{
		return _mm256_set_pd(vector.m256d_f64[3], vector.m256d_f64[2], y, vector.m256d_f64[0]);
	}

	/****************************************************************************
	*                       SetZ
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION SetZ(ConstVector256 vector, const double z) noexcept
	*
	*  @brief     Float���g����Vector��Z�ɒl��������
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] const double z
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SetZ(ConstVector256 vector, const double z) noexcept
	{
		return _mm256_set_pd(vector.m256d_f64[3], z, vector.m256d_f64[1], vector.m256d_f64[0]);
	}

	/****************************************************************************
	*                       SetW
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION SetW(ConstVector256 vector, const double w) noexcept
	*
	*  @brief     Float���g����Vector��W�ɒl��������
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] const double w
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SetW(ConstVector256 vector, const double w) noexcept
	{
		return _mm256_set_pd(w, vector.m256d_f64[2], vector.m256d_f64[1], vector.m256d_f64[0]);
	}

	/****************************************************************************
	*                       SplatX
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatX(ConstVector256 vector) noexcept
	*
	*  @brief     X�̗v�f�Ɋ�Â���, �S�Ă̗v�f�ɓ����l��������Vector256�N���X���쐬����
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatX(ConstVector256 vector) noexcept
	{
		return _mm256_permute4x64_pd(vector, _MM_SHUFFLE(0, 0, 0, 0));
	}

	/****************************************************************************
	*                       SplatY
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatY(ConstVector256 vector) noexcept
	*
	*  @brief     Y�̗v�f�Ɋ�Â���, �S�Ă̗v�f�ɓ����l��������Vector256�N���X���쐬����
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatY(ConstVector256 vector) noexcept
	{
		return _mm256_permute4x64_pd(vector, _MM_SHUFFLE(1, 1, 1, 1));
	}

	/****************************************************************************
	*                       SplatZ
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatZ(ConstVector256 vector) noexcept
	*
	*  @brief     Z�̗v�f�Ɋ�Â���, �S�Ă̗v�f�ɓ����l��������Vector256�N���X���쐬����
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatZ(ConstVector256 vector) noexcept
	{
		return _mm256_permute4x64_pd(vector, _MM_SHUFFLE(2, 2, 2, 2));
	}

	/****************************************************************************
	*                       SplatW
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatZ(ConstVector256 vector) noexcept
	*
	*  @brief     W�̗v�f�Ɋ�Â���, �S�Ă̗v�f�ɓ����l��������Vector256�N���X���쐬����
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatW(ConstVector256 vector) noexcept
	{
		return _mm256_permute4x64_pd(vector,_MM_SHUFFLE(3, 3, 3, 3));
	}

	/****************************************************************************
	*                       SplatOne
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatOne(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��1��������Vector256�N���X���쐬����
	*
	*  @param[in] void
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatOne() noexcept
	{
		return VECTOR_256D_ONE;
	}

	/****************************************************************************
	*                       SplatInfinity
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatInfinity() noexcept
	*
	*  @brief     W�̗v�f�Ɋ�Â���, �S�Ă̗v�f��INF��������Vector256�N���X���쐬����
	*
	*  @param[in] void
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatInfinity() noexcept
	{
		return VECTOR_256D_INFINITY;
	}

	/****************************************************************************
	*                       StoreDouble
	****************************************************************************/
	/* @fn        inline void SIMD_CALL_CONVENTION Vector256Utility::StoreDouble2(double* destination, ConstVector256 source) noexcept
	*
	*  @brief     Vector256�����Ƃ�Double�z��ɑ�����܂�
	*
	*  @param[in] double* destination
	*  @param[in] ConstVector256 source
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline void SIMD_CALL_CONVENTION Vector256Utility::StoreDouble(double* destination, ConstVector256 source) noexcept
	{
		Check(destination);
		_mm_storeu_pd(destination, _mm256_castpd256_pd128(source));
	}

	/****************************************************************************
	*                       StoreDouble2
	****************************************************************************/
	/* @fn        inline void SIMD_CALL_CONVENTION Vector256Utility::StoreDouble2(double* destination, ConstVector256 source) noexcept
	*
	*  @brief     Vector256�����Ƃ�Double�z��ɑ�����܂�
	*
	*  @param[in] double* destination
	*  @param[in] ConstVector256 source
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline void SIMD_CALL_CONVENTION Vector256Utility::StoreDouble2(double* destination, ConstVector256 source) noexcept
	{
		Check(destination);
		_mm_storeu_pd(destination, _mm256_extractf128_pd(source, 0));
	}

	/****************************************************************************
	*                       StoreDouble3
	****************************************************************************/
	/* @fn        inline void SIMD_CALL_CONVENTION Vector256Utility::StoreDouble3(double* destination, ConstVector256 source) noexcept
	*
	*  @brief     Vector256�����Ƃ�Double�z��ɑ�����܂�
	*
	*  @param[in] double* destination
	*  @param[in] ConstVector256 source
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline void SIMD_CALL_CONVENTION Vector256Utility::StoreDouble3(double* destination, ConstVector256 source) noexcept
	{
		Check(destination);
		_mm_storeu_pd(destination   , _mm256_extractf128_pd(source, 0));
		_mm_store_pd(destination + 2, _mm256_extractf128_pd(source, 1));
	}

	/****************************************************************************
	*                       StoreDouble4
	****************************************************************************/
	/* @fn        inline void SIMD_CALL_CONVENTION Vector256Utility::StoreDouble4(double* destination, ConstVector256 source) noexcept
	*
	*  @brief     Vector256�����Ƃ�Float�z��ɑ�����܂�
	*
	*  @param[in] double* destination
	*  @param[in] ConstVector256 source
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline void SIMD_CALL_CONVENTION Vector256Utility::StoreDouble4(double* destination, ConstVector256 source) noexcept
	{
		Check(destination);
		_mm256_storeu_pd(destination, source);
	}

	#pragma endregion Setter
	#pragma region Getter
	/****************************************************************************
	*                       GetX
	****************************************************************************/
	/* @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::GetX(ConstVector256 vector) noexcept
	*
	*  @brief     FPU���W�X�^�Ɋi�[���ꂽX�̗v�f�����o��
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::GetX(ConstVector256 vector) noexcept
	{
		return _mm256_cvtsd_f64(vector);
	}

	/****************************************************************************
	*                       GetY
	****************************************************************************/
	/* @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::GetY(ConstVector256 vector) noexcept
	*
	*  @brief     FPU���W�X�^�Ɋi�[���ꂽY�̗v�f�����o��
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::GetY(ConstVector256 vector) noexcept
	{
		return _mm256_cvtsd_f64(_mm256_permute4x64_pd(vector,_MM_SHUFFLE(1, 1, 1, 1)));
	}

	/****************************************************************************
	*                       GetZ
	****************************************************************************/
	/* @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::GetZ(ConstVector256 vector) noexcept
	*
	*  @brief     FPU���W�X�^�Ɋi�[���ꂽZ�̗v�f�����o��
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::GetZ(ConstVector256 vector) noexcept
	{
		return _mm256_cvtsd_f64(_mm256_permute4x64_pd(vector,_MM_SHUFFLE(2, 2, 2, 2)));
	}

	/****************************************************************************
	*                       GetW
	****************************************************************************/
	/* @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::GetZ(ConstVector256 vector) noexcept
	*
	*  @brief     FPU���W�X�^�Ɋi�[���ꂽW�̗v�f�����o��
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::GetW(ConstVector256 vector) noexcept
	{
		return _mm256_cvtsd_f64(_mm256_permute4x64_pd(vector, _MM_SHUFFLE(3, 3, 3, 3)));
	}

	/****************************************************************************
	*                       LoadDouble
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble2(const double* source) noexcept
	*
	*  @brief     Double�̔z����g���Ċi�[���� (source->x, source->y, source->z, source->w)
	*
	*  @param[in] const double* source
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble(const double* source) noexcept
	{
		Check(source);

		return _mm256_broadcast_sd(source);
	}

	/****************************************************************************
	*                       LoadDouble2
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble2(const double* source) noexcept
	*
	*  @brief     Double�̔z����g���Ċi�[���� (source->x, source->y, source->z, source->w)
	*
	*  @param[in] const double* source
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble2(const double* source) noexcept
	{
		Check(source);

		Vector256d result = {};
		const __m128d temp = _mm_loadu_pd(source);
		result = _mm256_set_m128d(temp, temp);
		return result.u.V;
	}

	/****************************************************************************
	*                       LoadDouble3
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble3(const double* source) noexcept
	*
	*  @brief     Double�̔z����g���Ċi�[���� (source->x, source->y, source->z, source->w)
	*
	*  @param[in] const double* source
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble3(const double* source) noexcept
	{
		Check(source);

		Vector256d result = {};
		result.u.t.XY = _mm_loadu_pd((double*)(source));
		result.u.t.ZW = _mm_loadu_pd((double*)(source + 2));
		return result.u.V;
	}

	/****************************************************************************
	*                       LoadDouble4
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble4(const double* source) noexcept
	*
	*  @brief     Double�̔z����g���Ċi�[���� (source->x, source->y, source->z, source->w)
	*
	*  @param[in] const double* source
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble4(const double* source) noexcept
	{
		Check(source);
		return _mm256_loadu_pd(source);
	}

	#pragma endregion Getter

	#pragma region Operator
	/****************************************************************************
	*                       Add
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Add(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     2�̃x�N�g���̊e�v�f���m�̘a���Z�o���܂�. (left + right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Add(ConstVector256 left, ConstVector256 right) noexcept
	{
		return _mm256_add_pd(left, right);
	}

	/****************************************************************************
	*                       Subtract
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Subtract(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     2�̃x�N�g���̊e�v�f���m�̍����Z�o���܂�. (left - right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Subtract(ConstVector256 left, ConstVector256 right) noexcept
	{
		return _mm256_sub_pd(left, right);
	}

	/****************************************************************************
	*                       Multiply
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Multiply(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     2�̃x�N�g���̊e�v�f���m�̍����Z�o���܂�. (left * right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Multiply(ConstVector256 left, ConstVector256 right) noexcept
	{
		return _mm256_mul_pd(left, right);
	}

	/****************************************************************************
	*                       Divide
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Divide(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     2�̃x�N�g���̊e�v�f���m�̍����Z�o���܂�. (left / right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Divide(ConstVector256 left, ConstVector256 right) noexcept
	{
		return _mm256_div_pd(left, right);
	}

	/****************************************************************************
	*                       Mod
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Mod(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     2�̃x�N�g���̊e�v�f���m�̂��܂���Z�o���܂�. (left % right)
	*             left % right = left - right * truncate(left / right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Mod(ConstVector256 left, ConstVector256 right) noexcept
	{
		Vector256 result = _mm256_div_pd(left, right);
		result = Truncate(result);
		return NegativeMultiplySubtract(result, right, left); // left - result * right
	}

	/****************************************************************************
	*                      NegativeMultiplySubstract
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NegativeMultiplySubtract(ConstVector256 first, ConstVector256 second, ConstVector256 third)
	*
	*  @brief     subtractLeft - (multiplyLeft * multiplyRight)���Z�o���܂�.
	*
	*  @param[in] ConstVector256 multiplyLeft
	*  @param[in] ConstVector256 multiplyRight
	*  @param[in] ConstVector256 subtractLeft
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NegativeMultiplySubtract(ConstVector256 multiplyLeft, ConstVector256 multiplyRight, ConstVector256 subtractLeft) noexcept
	{
		return Subtract(subtractLeft, Multiply(multiplyLeft, multiplyRight));
	}

	/****************************************************************************
	*                      MultiplyAdd
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::MultiplyAdd(ConstVector256 first, ConstVector256 second, ConstVector256 third)
	*
	*  @brief     multiplyLeft * multiplyRight + add���Z�o���܂�.
	*
	*  @param[in] ConstVector256 multiplyLeft
	*  @param[in] ConstVector256 multiplyRight
	*  @param[in] ConstVector256 add
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::MultiplyAdd(ConstVector256 multiplyLeft, ConstVector256 multiplyRight, ConstVector256 add) noexcept
	{
		return Add(Multiply(multiplyLeft, multiplyRight), add);
	}

	/****************************************************************************
	*                       Reciprocal
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Reciprocal(ConstVector256 vector) noexcept
	*
	*  @brief     2�̃x�N�g���̊e�v�f���m�̋t�����Z�o���܂�. (1.0f / vector)
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Reciprocal(ConstVector256 vector) noexcept
	{
		return _mm256_div_pd(VECTOR_256D_ONE, vector);
	}

	/****************************************************************************
	*                       Scale
	****************************************************************************/
	/* @fn       inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Scale(ConstVector256 vector, const double scale) noexcept
	*
	*  @brief     double�l���g���Ċe�v�f�Ɋ|���܂� scale * vector
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] const double scale
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Scale(ConstVector256 vector, const double scale) noexcept
	{
		Vector256 result = _mm256_set1_pd(scale); // (scale, scale, scale, scale)
		return _mm256_mul_pd(result, vector);
	}

	/****************************************************************************
	*                       EqualVector2
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVector2(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     ���2�����x�N�g�������������ǂ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVector2(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂��ē������ꍇ��1��Ԃ��x�N�g��
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_EQ_OQ); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		// w, z�͖�������悤�ɂ���
		return (_mm256_movemask_pd(equalMaskVector) & 0b0011) == 0b0011; // �S������0b1111�Ƃ����r�b�g�t���O������
	}

	/****************************************************************************
	*                       EqualVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVector3(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     ���3�����x�N�g�������������ǂ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVector3(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂��ē������ꍇ��1��Ԃ��x�N�g��
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_EQ_OQ); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		// w�͖�������悤�ɂ���
		return (_mm256_movemask_pd(equalMaskVector) & 0b0111) == 0b0111; // �S������0b1111�Ƃ����r�b�g�t���O������
	}

	/****************************************************************************
	*                       EqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVector4(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     ���4�����x�N�g�������������ǂ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVector4(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂��ē������ꍇ��1��Ԃ��x�N�g��
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_EQ_OQ); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		return _mm256_movemask_pd(equalMaskVector) == 0b1111; // �S������0b1111�Ƃ����r�b�g�t���O������
	}

	/****************************************************************************
	*                       EqualVectorEach
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     ���4�����x�N�g�����e�v�f���Ƃɓ��������ǂ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::EqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	{
		return _mm256_cmp_pd(left, right, _CMP_EQ_OQ);
	}

	/****************************************************************************
	*                       NotEqualVector2
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector2(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     ���2�����x�N�g�����������Ȃ����ǂ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector2(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂��ē������Ȃ��ꍇ��1��Ԃ��x�N�g��
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_EQ_OQ); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		// w, z�͖�������悤�ɂ���
		return (_mm256_movemask_pd(equalMaskVector) & 0b0011) != 0b0011; // �S������0b1111�Ƃ����r�b�g�t���O������
	}

	/****************************************************************************
	*                       NotEqualVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     ���3�����x�N�g�����������Ȃ����ǂ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector3(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂��ē������Ȃ��ꍇ��1��Ԃ��x�N�g��
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_EQ_OQ); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		// w�͖�������悤�ɂ���
		return (_mm256_movemask_pd(equalMaskVector) & 0b0111) != 0b0111; // �S������0b1111�Ƃ����r�b�g�t���O������
	}

	/****************************************************************************
	*                       NotEqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     ���4�����x�N�g�����������Ȃ����ǂ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂��ē������Ȃ��ꍇ��1��Ԃ��x�N�g��
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_NEQ_OQ); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		return _mm256_movemask_pd(equalMaskVector) != 0; // �S������0b1111�Ƃ����r�b�g�t���O������
	}

	/****************************************************************************
	*                       NotEqualVectorEach
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     ���4�����x�N�g�����e�v�f���Ƃɓ������Ȃ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NotEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	{
		return _mm256_cmp_pd(left, right, _CMP_NEQ_OQ);
	}

	/****************************************************************************
	*                       EqualAsIntVector2
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualAsIntVector2(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     ��̃x�N�g�������������ǂ����𒲂ׂ܂�.
	*             ��r���ɂ͕����Ȃ�����(U32)�Ƃ��Ĉ����܂�.
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualAsIntVector2(ConstVector256 left, ConstVector256 right) noexcept
	{
		__m256i temp = _mm256_cmpeq_epi64(_mm256_castpd_si256(left), _mm256_castpd_si256(right));
		return (_mm256_movemask_pd(_mm256_castsi256_pd(temp)) & 0b0011) == 0b0011;
	}

	/****************************************************************************
	*                       EqualAsIntVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualAsIntVector3(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     ��̃x�N�g�������������ǂ����𒲂ׂ܂�.
	*             ��r���ɂ͕����Ȃ������Ƃ��Ĉ����܂�.
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualAsIntVector3(ConstVector256 left, ConstVector256 right) noexcept
	{
		__m256i temp = _mm256_cmpeq_epi64(_mm256_castpd_si256(left), _mm256_castpd_si256(right));
		return (_mm256_movemask_pd(_mm256_castsi256_pd(temp)) & 0b0111) == 0b0111;
	}

	/****************************************************************************
	*                       EqualAsIntVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualAsIntVector4(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     ��̃x�N�g�������������ǂ����𒲂ׂ܂�.
	*             ��r���ɂ͕����Ȃ������Ƃ��Ĉ����܂�.
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualAsIntVector4(ConstVector256 left, ConstVector256 right) noexcept
	{
		__m256i temp = _mm256_cmpeq_epi64(_mm256_castpd_si256(left), _mm256_castpd_si256(right));
		return _mm256_movemask_pd(_mm256_castsi256_pd(temp)) == 0b1111;
	}

	/****************************************************************************
	*                       EqualAsIntVectorEach
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualAsIntVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     ��̃x�N�g�������������ǂ��������ꂼ��̗v�f�Œ��ׂ܂�.
	*             ��r���ɂ͕����Ȃ������Ƃ��Ĉ����܂�.
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::EqualAsIntVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	{
		__m256i result = _mm256_cmpeq_epi64(_mm256_castpd_si256(left), _mm256_castpd_si256(right));
		return _mm256_castsi256_pd(result);
	}

	/****************************************************************************
	*                       NotEqualVector2
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualAsIntVector2(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     ��̃x�N�g������ł��������Ȃ����ǂ����𒲂ׂ܂�.
	*            ��r���ɂ͕����Ȃ�����(U32)�Ƃ��Ĕ�r���܂�.
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualAsIntVector2(ConstVector256 left, ConstVector256 right) noexcept
	{
		__m256i vTemp = _mm256_cmpeq_epi64(_mm256_castpd_si256(left), _mm256_castpd_si256(right));
		return ((_mm256_movemask_pd(_mm256_castsi256_pd(vTemp)) & 0b0011) != 0b0011);
	}

	/****************************************************************************
	*                       NotEqualVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualAsIntVector3(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     ���3�����x�N�g�����������Ȃ����ǂ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualAsIntVector3(ConstVector256 left, ConstVector256 right) noexcept
	{
		__m256i vTemp = _mm256_cmpeq_epi64(_mm256_castpd_si256(left), _mm256_castpd_si256(right));
		return ((_mm256_movemask_pd(_mm256_castsi256_pd(vTemp)) & 0b0111) != 0b0111);
	}

	/****************************************************************************
	*                       NotEqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     ���4�����x�N�g�����������Ȃ����ǂ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualAsIntVector4(ConstVector256 left, ConstVector256 right) noexcept
	{
		__m256i vTemp = _mm256_cmpeq_epi64(_mm256_castpd_si256(left), _mm256_castpd_si256(right));
		return (_mm256_movemask_pd(_mm256_castsi256_pd(vTemp)) != 0b1111);
	}

	/****************************************************************************
	*                       NotEqualVector
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     ���4�����x�N�g�����e�v�f���Ƃɓ������Ȃ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NotEqualAsIntVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	{
		__m256i v = _mm256_cmpeq_epi64(_mm256_castpd_si256(left), _mm256_castpd_si256(right));
		return _mm256_xor_pd(_mm256_castsi256_pd(v), VECTOR_256U_NEGATIVE_ONE_MASK);
	}

	/****************************************************************************
	*                       NearEqualVector2
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector4(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
	*
	*  @brief     ���͈͓��Œl�����������𒲂ׂ� (left - epsilon <= right <= left + epsilon)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*  @param[in] ConstVector256 epsilon
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::NearEqualVector2(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
	{
		// �����̓��o
		Vector256 delta = _mm256_sub_pd(left, right);

		// ��Βl�̍����̎擾
		Vector256 result = _mm256_setzero_pd();
		result = _mm256_sub_pd(result, delta);
		result = _mm256_max_pd(result, delta);
		result = _mm256_cmp_pd(result, epsilon, _CMP_LE_OQ);

		// z��w�͖�������, x��y�ɒ��ڂ���
		return ((_mm256_movemask_pd(result) & 0b0011) == 0b0011);
	}

	/****************************************************************************
	*                       NearEqualVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector3(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
	*
	*  @brief     ���͈͓��Œl�����������𒲂ׂ� (left - epsilon <= right <= left + epsilon)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*  @param[in] ConstVector256 epsilon
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::NearEqualVector3(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
	{
		// �����̓��o
		Vector256 delta = _mm256_sub_pd(left, right);

		// ��Βl�̍����̎擾
		Vector256 result = _mm256_setzero_pd();
		result = _mm256_sub_pd(result, delta);
		result = _mm256_max_pd(result, delta);
		result = _mm256_cmp_pd(result, epsilon, _CMP_LE_OQ);

		// z��w�͖�������, x��y�ɒ��ڂ���
		return ((_mm256_movemask_pd(result) & 0b0111) == 0b0111);
	}

	/****************************************************************************
	*                       NearEqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector4(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
	*
	*  @brief     ���͈͓��Œl�����������𒲂ׂ� (left - epsilon <= right <= left + epsilon)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*  @param[in] ConstVector256 epsilon
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::NearEqualVector4(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
	{
		// �����̓��o
		Vector256 delta = _mm256_sub_pd(left, right);

		// ��Βl�̍����̎擾
		Vector256 result = _mm256_setzero_pd();
		result = _mm256_sub_pd(result, delta);
		result = _mm256_max_pd(result, delta);
		result = _mm256_cmp_pd(result, epsilon, _CMP_LE_OQ);

		// z��w�͖�������, x��y�ɒ��ڂ���
		return ((_mm256_movemask_pd(result) & 0b1111) == 0b1111);
	}

	/****************************************************************************
	*                       NearEqualVectorEach
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVectorEach(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
	*
	*  @brief     ���͈͓��Œl�����������𒲂ׂ� (left - epsilon <= right <= left + epsilon)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*  @param[in] ConstVector256 epsilon
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NearEqualVectorEach(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
	{
		// ����
		Vector256 delta = _mm256_sub_pd(left, right);

		// ��Βl�̎擾
		Vector256 temp = _mm256_setzero_pd();
		temp = _mm256_sub_pd(temp, delta);
		temp = _mm256_max_pd(temp, delta);
		temp = _mm256_cmp_pd(temp, epsilon, _CMP_LE_OQ);
		return temp;
	}

	/****************************************************************************
	*                       GreaterVector2
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterVector2(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector2�^�ɂ����č��̕����S�Ă̗v�f�ɂ����đ傫�����𒲂ׂ܂� (left > right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterVector2(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left > right�̏ꍇ��1��Ԃ��x�N�g��
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_GT_OQ); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		// w, z�͖�������悤�ɂ���
		return (_mm256_movemask_pd(equalMaskVector) & 0b0011) == 0b0011; // �S������0b1111�Ƃ����r�b�g�t���O������
	}

	/****************************************************************************
	*                       GreaterVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterVector3(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector3�^�ɂ����č��̕����S�Ă̗v�f�ɂ����đ傫�����𒲂ׂ܂� (left > right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterVector3(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left > right�̏ꍇ��1��Ԃ��x�N�g��
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_GT_OQ); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		// w�͖�������悤�ɂ���
		return (_mm256_movemask_pd(equalMaskVector) & 0b0111) == 0b0111; // �S������0b1111�Ƃ����r�b�g�t���O������
	}

	/****************************************************************************
	*                       GreaterVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterVector4(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector4�^�ɂ����č��̕����S�Ă̗v�f�ɂ����đ傫�����𒲂ׂ܂� (left > right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterVector4(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left > right�ꍇ��1��Ԃ��x�N�g��
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_GT_OQ); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		return _mm256_movemask_pd(equalMaskVector) == 0b1111; // �S������0b1111�Ƃ����r�b�g�t���O������
	}

	/****************************************************************************
	*                       GreaterVectorEach
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterVector4(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector4�^�ɂ����č��̕����S�Ă̗v�f�ɂ����đ傫�����𒲂ׂ܂� (left > right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::GreaterVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	{
		return _mm256_cmp_pd(left, right, _CMP_GT_OQ);
	}


	/****************************************************************************
	*                       GreaterOrEqualVector2
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVector2(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector2�^�ɂ����č��̕����S�Ă̗v�f�ɂ����đ傫��or���������𒲂ׂ܂� (left >= right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVector2(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left >= right�ꍇ��1��Ԃ��x�N�g��
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_GE_OQ); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		// w, z�͖�������悤�ɂ���
		return (_mm256_movemask_pd(equalMaskVector) & 0b0011) == 0b0011; // �S������0b1111�Ƃ����r�b�g�t���O������
	}

	/****************************************************************************
	*                       GreaterVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVector3(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector3�^�ɂ����č��̕����S�Ă̗v�f�ɂ����đ傫��or���������𒲂ׂ܂� (left >= right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVector3(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left >= right�̏ꍇ��1��Ԃ��x�N�g��
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_GE_OQ); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		// w�͖�������悤�ɂ���
		return (_mm256_movemask_pd(equalMaskVector) & 0b0111) == 0b0111; // �S������0b1111�Ƃ����r�b�g�t���O������
	}

	/****************************************************************************
	*                       GreaterOrEqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector4�^�ɂ����č��̕����S�Ă̗v�f�ɂ����đ傫��or���������𒲂ׂ܂� (left >= right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left >= right�̏ꍇ��1��Ԃ��x�N�g��
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_GE_OQ); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		return _mm256_movemask_pd(equalMaskVector) == 0b1111; // �S������0b1111�Ƃ����r�b�g�t���O������
	}

	/****************************************************************************
	*                       GreaterOrEqualVectorEach
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector4�^�ɂ����č��̕����S�Ă̗v�f�ɂ����đ傫��or���������𒲂ׂ܂� (left >= right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	{
		return _mm256_cmp_pd(left, right, _CMP_GE_OQ);
	}

	/****************************************************************************
	*                       LessVector2
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessVector2(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector2�^�ɂ����č��̕����S�Ă̗v�f�ɂ����ď��������𒲂ׂ܂� (left < right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::LessVector2(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left < right�̏ꍇ��1��Ԃ��x�N�g��
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_LT_OQ); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		// w, z�͖�������悤�ɂ���
		return (_mm256_movemask_pd(equalMaskVector) & 0b0011) == 0b0011; // �S������0b1111�Ƃ����r�b�g�t���O������

	}

	/****************************************************************************
	*                       LessVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessVector3(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector3�^�ɂ����č��̕����S�Ă̗v�f�ɂ����ď��������𒲂ׂ܂� (left < right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::LessVector3(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left < right�̏ꍇ��1��Ԃ��x�N�g��
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_LT_OQ); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		// w�͖�������悤�ɂ���
		return (_mm256_movemask_pd(equalMaskVector) & 0b0111) == 0b0111; // �S������0b1111�Ƃ����r�b�g�t���O������
	}

	/****************************************************************************
	*                       LessVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessVector4(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector4�^�ɂ����č��̕����S�Ă̗v�f�ɂ����ď��������𒲂ׂ܂� (left < right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::LessVector4(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left < right�̏ꍇ��1��Ԃ��x�N�g��
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_LT_OQ); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		return _mm256_movemask_pd(equalMaskVector) == 0b1111; // �S������0b1111�Ƃ����r�b�g�t���O������
	}

	/****************************************************************************
	*                       LessVectorEach
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LessVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector4�^�ɂ����č��̕����S�Ă̗v�f�ɂ����ď��������𒲂ׂ܂� (left < right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LessVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	{
		return _mm256_cmp_pd(left, right, _CMP_LT_OQ);
	}

	/****************************************************************************
	*                       LessOrEqualVector2
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVector2(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector2�^�ɂ����č��̕����S�Ă̗v�f�ɂ����ď�����or���������𒲂ׂ܂� (left <= right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVector2(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left <= right�̏ꍇ��1��Ԃ��x�N�g��
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_LE_OQ); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		// w, z�͖�������悤�ɂ���
		return (_mm256_movemask_pd(equalMaskVector) & 0b0011) == 0b0011; // �S������0b1111�Ƃ����r�b�g�t���O������
	}

	/****************************************************************************
	*                       LessOrEqualVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVector3(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector3�^�ɂ����č��̕����S�Ă̗v�f�ɂ����ď�����or���������𒲂ׂ܂� (left <= right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVector3(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left <= right�̏ꍇ��1��Ԃ��x�N�g��
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_LE_OQ); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		// w�͖�������悤�ɂ���
		return (_mm256_movemask_pd(equalMaskVector) & 0b0111) == 0b0111; // �S������0b1111�Ƃ����r�b�g�t���O������
	}

	/****************************************************************************
	*                       LessOrEqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector4�^�ɂ����č��̕����S�Ă̗v�f�ɂ����ď�����or���������𒲂ׂ܂� (left <= right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left <= right�̏ꍇ��1��Ԃ��x�N�g��
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_LE_OQ); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		return _mm256_movemask_pd(equalMaskVector) == 0b0011; // �S������0b1111�Ƃ����r�b�g�t���O������
	}

	/****************************************************************************
	*                       LessVectorEach
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector4�^�ɂ����č��̕����S�Ă̗v�f�ɂ����ď�����or���������𒲂ׂ܂� (left <= right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	{
		return _mm256_cmp_pd(left, right, _CMP_LE_OQ);
	}

	/****************************************************************************
	*                       InBoundsVector2
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::InBoundsVector2(ConstVector256 vector, ConstVector256 bounds) noexcept
	*
	*  @brief     �S�Ă̗v�f�𒲂ׂĎw��͈͓��ɂ��邩�ǂ��� -bounds <= vector <= +bounds
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] ConstVector256 bounds 
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::InBoundsVector2(ConstVector256 vector, ConstVector256 bounds) noexcept
	{
		Vector256 lessOrEqual = _mm256_cmp_pd(vector, bounds, _CMP_LE_OQ);
		Vector256 negateBound = _mm256_mul_pd(bounds, VECTOR_256D_NEGATIVE_ONE);
		negateBound = _mm256_cmp_pd(negateBound, vector, _CMP_LE_OQ); // greater or equal
		lessOrEqual = _mm256_and_pd(lessOrEqual, negateBound);
		return (_mm256_movemask_pd(lessOrEqual) & 0b0011) == 0b0011;
	}

	/****************************************************************************
	*                       InBoundsVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::InBoundsVector3(ConstVector256 vector, ConstVector256 bounds) noexcept
	*
	*  @brief     �S�Ă̗v�f�𒲂ׂĎw��͈͓��ɂ��邩�ǂ��� -bounds <= vector <= +bounds
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] ConstVector256 bounds
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::InBoundsVector3(ConstVector256 vector, ConstVector256 bounds) noexcept
	{
		Vector256 lessOrEqual = _mm256_cmp_pd(vector, bounds, _CMP_LE_OQ);
		Vector256 negateBound = _mm256_mul_pd(bounds, VECTOR_256D_NEGATIVE_ONE);
		negateBound = _mm256_cmp_pd(negateBound, vector, _CMP_LE_OQ); // greater or equal
		lessOrEqual = _mm256_and_pd(lessOrEqual, negateBound);
		return (_mm256_movemask_pd(lessOrEqual) & 0b0111) == 0b0111;

	}

	/****************************************************************************
	*                       InBoundsVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::InBoundsVector4(ConstVector256 vector, ConstVector256 bounds) noexcept
	*
	*  @brief     �S�Ă̗v�f�𒲂ׂĎw��͈͓��ɂ��邩�ǂ��� -bounds <= vector <= +bounds
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] ConstVector256 bounds
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::InBoundsVector4(ConstVector256 vector, ConstVector256 bounds) noexcept
	{
		Vector256 lessOrEqual = _mm256_cmp_pd(vector, bounds, _CMP_LE_OQ);
		Vector256 negateBound = _mm256_mul_pd(bounds, VECTOR_256D_NEGATIVE_ONE);
		negateBound = _mm256_cmp_pd(negateBound, vector, _CMP_LE_OQ); // greater or equal
		lessOrEqual = _mm256_and_pd(lessOrEqual, negateBound);
		return _mm256_movemask_pd(lessOrEqual) == 0b1111;
	}

	/****************************************************************************
	*                       InBoundsVectorEach
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::InBoundsVectorEach(ConstVector256 vector, ConstVector256 bounds) noexcept
	*
	*  @brief     �w��͈͓��ɂ��邩�ǂ������e�v�f�Ŕ�r���܂� -bounds <= vector <= +bounds
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] ConstVector256 bounds
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::InBoundsVectorEach(ConstVector256 vector, ConstVector256 bounds) noexcept
	{
		Vector256 lessOrEqual = _mm256_cmp_pd(vector, bounds, _CMP_LE_OQ);
		Vector256 negateBound = _mm256_mul_pd(bounds, VECTOR_256D_NEGATIVE_ONE);
		negateBound = _mm256_cmp_pd(negateBound, vector, _CMP_LE_OQ); // greater or equal
		lessOrEqual = _mm256_and_pd(lessOrEqual, negateBound);
		return lessOrEqual;
	}

	#pragma endregion Operator

	#pragma region Math
	/****************************************************************************
	*                       Negate
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Negate(ConstVector256 vector) noexcept
	*
	*  @brief      �e�v�f�Ƀ}�C�i�X����������̂�Ԃ�.
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Negate(ConstVector256 vector) noexcept
	{
		return _mm256_sub_pd(_mm256_setzero_pd(), vector);
	}

	/****************************************************************************
	*                      LengthVector2
	****************************************************************************/
	/* @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::LengthVector2(ConstVector256 vector) noexcept
	*
	*  @brief     2���m����(�e�v�f��2��a�����[�g��������Ƃ��̃x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::LengthVector2(ConstVector256 vector) noexcept
	{
		return sqrt(DotVector2(vector, vector));
	}

	/****************************************************************************
	*                      LengthVector3
	****************************************************************************/
	/* @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::LengthVector3(ConstVector256 vector) noexcept
	*
	*  @brief     2���m����(�e�v�f��2��a�����[�g��������Ƃ��̃x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::LengthVector3(ConstVector256 vector) noexcept
	{
		return sqrt(DotVector3(vector, vector));
	}

	/****************************************************************************
	*                      LengthVector4
	****************************************************************************/
	/* @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::LengthVector4(ConstVector256 vector) noexcept
	*
	*  @brief     2���m����(�e�v�f��2��a�����[�g��������Ƃ��̃x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::LengthVector4(ConstVector256 vector) noexcept
	{
		return sqrt(DotVector4(vector, vector));
	}

	/****************************************************************************
	*                      LengthSquaredVector2
	****************************************************************************/
	/* @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::LengthSquaredVector2(ConstVector256 vector) noexcept
	*
	*  @brief     2���m����(�e�v�f��2��a�����[�g��������Ƃ��̃x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::LengthSquaredVector2(ConstVector256 vector) noexcept
	{
		return DotVector2(vector, vector);
	}

	/****************************************************************************
	*                      LengthSquaredVector3
	****************************************************************************/
	/* @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::LengthSquaredVector3(ConstVector256 vector) noexcept
	*
	*  @brief     2���m����(�e�v�f��2��a�����[�g��������Ƃ��̃x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::LengthSquaredVector3(ConstVector256 vector) noexcept
	{
		return DotVector3(vector, vector);
	}

	/****************************************************************************
	*                      LengthSquaredVector4
	****************************************************************************/
	/* @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::LengthSquaredVector4(ConstVector256 vector) noexcept
	*
	*  @brief     2���m����(�e�v�f��2��a�����[�g��������Ƃ��̃x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::LengthSquaredVector4(ConstVector256 vector) noexcept
	{
		return DotVector4(vector, vector);
	}

	/****************************************************************************
	*                      DotVector2
	****************************************************************************/
	/* @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::DotVector2(ConstVector256 left, ConstVector256 right)
	*
	*  @brief     ���ς��Z�o
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::DotVector2(ConstVector256 left, ConstVector256 right) noexcept
	{
		// �v�f���Ƃ̐ς��v�Z 
		Vector256 multiply = _mm256_mul_pd(left, right);

		// z�ȍ~�̌v�Z�͍l�����Ȃ�����, y�̌��ʂ��i�[
		Vector256 y = _mm256_permute4x64_pd(multiply, _MM_SHUFFLE(1, 1, 1, 1));

		// x�����݂̂̌��ʂɒ��ڂ���.  
		y.m256d_f64[0] = multiply.m256d_f64[0] + y.m256d_f64[0];
		return _mm256_cvtsd_f64(y);
	}

	/****************************************************************************
	*                      DotVector3
	****************************************************************************/
	/* @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::DotVector3(ConstVector256 left, ConstVector256 right)
	*
	*  @brief     ���ς��Z�o
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::DotVector3(ConstVector256 left, ConstVector256 right) noexcept
	{
		// �v�f���Ƃ̐ς��v�Z 
		Vector256 multiply = _mm256_mul_pd(left, right);

		// x=Dot.D64[1], y=Dot.D64[2]
		Vector256 temp = _mm256_permute4x64_pd(multiply, _MM_SHUFFLE(2, 1, 2, 1));

		// result.D64[0] = x + y 256 bit�ɑΉ�����_mm_add_ss���Ȃ�����
		multiply.m256d_f64[0] = multiply.m256d_f64[0] + temp.m256d_f64[0];

		// x=multiply.D64[2]
		temp = _mm256_permute4x64_pd(temp, _MM_SHUFFLE(1, 1, 1, 1));

		// Result.F32[0] = (x+y)+z
		temp.m256d_f64[0] = multiply.m256d_f64[0] + temp.m256d_f64[0];

		// x�����݂̂̌��ʂɒ��ڂ���.  
		return _mm256_cvtsd_f64(_mm256_permute4x64_pd(temp, _MM_SHUFFLE(0, 0, 0, 0)));
	}

	/****************************************************************************
	*                      DotVector4
	****************************************************************************/
	/* @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::DotVector4(ConstVector256 left, ConstVector256 right)
	*
	*  @brief     ���ς��Z�o
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::DotVector4(ConstVector256 left, ConstVector256 right) noexcept
	{
		// �v�f���Ƃ̐ς��v�Z 
		Vector256 temp2 = right;
		Vector256 temp = _mm256_mul_pd(left, temp2);

		temp2 = _mm256_permute4x64_pd(temp, _MM_SHUFFLE(3, 2, 3, 2));

		temp2 = _mm256_add_pd(temp2, temp);

		temp = _mm256_permute4x64_pd(temp2, _MM_SHUFFLE(1, 1, 1, 1));

		temp = _mm256_add_pd(temp, temp2);

		// x�����݂̂̌��ʂɒ��ڂ���.  
		return _mm256_cvtsd_f64(_mm256_permute4x64_pd(temp, _MM_SHUFFLE(0, 0, 0, 0)));
	}

	/****************************************************************************
	*                      CrossVector2
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CrossVector2(ConstVector256 left, ConstVector256 right)
	*
	*  @brief     ���s�l�ӌ`�̖ʐς��Z�o����
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@Vector256�ł͂��邪, ����double
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CrossVector2(ConstVector256 left, ConstVector256 right) noexcept
	{
		// [ left.x*right.y - left.y*right.x, left.x*right.y - left.y*right.x ]
		// right��x, y�����ւ��� (w,z�͌v�Z��g�p���Ȃ�)
		Vector256 result = _mm256_permute4x64_pd(right, _MM_SHUFFLE(0, 1, 0, 1));

		// left�Ɠ���ւ����right�̊e�v�f����Z����
		result = _mm256_mul_pd(result, left);

		// y�𒊏o����
		Vector256 y = _mm256_permute4x64_pd(result, _MM_SHUFFLE(1, 1, 1, 1));

		// �l�����Z���� (ss�ɑΉ�����֐����Ȃ�)
		result.m256d_f64[0] = result.m256d_f64[0] - y.m256d_f64[0];

		// x�����݂̂����o���Č��ʂƂ���
		return _mm256_permute4x64_pd(result, _MM_SHUFFLE(0, 0, 0, 0));
	}

	/****************************************************************************
	*                      CrossVector3
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CrossVector3(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     �O�ς��Z�o left, right�ɐ����ȃx�N�g����Ԃ�
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CrossVector3(ConstVector256 left, ConstVector256 right) noexcept
	{
		// [ V1.y*V2.z - V1.z*V2.y, V1.z*V2.x - V1.x*V2.z, V1.x*V2.y - V1.y*V2.x ]
		
		// left�̗v�f��(y1, z1, x1, w1)�ɏ��ɕ��בւ���
		Vector256 temp1 = _mm256_permute4x64_pd(left, _MM_SHUFFLE(3, 0, 2, 1)); 

		// right�̗v�f��(z2, x2. y2, w2)�̏��ɕ��בւ���
		Vector256 temp2 = _mm256_permute4x64_pd(right, _MM_SHUFFLE(3, 1, 0, 2));

		// �ꎞ�x�N�g���̗v�f���Ƃ̏�Z
		Vector256 result = _mm256_mul_pd(temp1, temp2);

		// z1, x1, y1, w1�̏���temp1����בւ���
		temp1 = _mm256_permute4x64_pd(temp1, _MM_SHUFFLE(3, 0, 2, 1));
		
		// y2, z2, x2, w2�̏���temp2����בւ���
		temp2 = _mm256_permute4x64_pd(temp2, _MM_SHUFFLE(3, 1, 0, 2));

		result = NegativeMultiplySubtract(temp1, temp2, result);

		// w�̗v�f���[���ɐݒ�
		return _mm256_and_pd(result, VECTOR_256U_MASK_XYZ);
	}

	/****************************************************************************
	*                      CrossVector4
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CrossVector4(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     �O�ς��Z�o left, right�ɐ����ȃx�N�g����Ԃ�
	*
	*  @param[in] ConstVector256 first
	*  @param[in] ConstVector256 second
	*  @param[in] ConstVector256 third
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CrossVector4(ConstVector256 first, ConstVector256 second, ConstVector256 third) noexcept
	{
		// [ ((v2.z*v3.w-v2.w*v3.z)*v1.y)-((v2.y*v3.w-v2.w*v3.y)*v1.z)+((v2.y*v3.z-v2.z*v3.y)*v1.w),
		//   ((v2.w*v3.z-v2.z*v3.w)*v1.x)-((v2.w*v3.x-v2.x*v3.w)*v1.z)+((v2.z*v3.x-v2.x*v3.z)*v1.w),
		//   ((v2.y*v3.w-v2.w*v3.y)*v1.x)-((v2.x*v3.w-v2.w*v3.x)*v1.y)+((v2.x*v3.y-v2.y*v3.x)*v1.w),
		//   ((v2.z*v3.y-v2.y*v3.z)*v1.x)-((v2.z*v3.x-v2.x*v3.z)*v1.y)+((v2.y*v3.x-v2.x*v3.y)*v1.z) ]

		// second.zwyz * third.wzxy
		Vector256 result = _mm256_permute4x64_pd(second, _MM_SHUFFLE(2, 1, 3, 2));
		Vector256 temp3  = _mm256_permute4x64_pd(third, _MM_SHUFFLE(1, 3, 2, 3));
		result = _mm256_mul_pd(result, temp3);

		// - second.wzwy * third.zwyz
		Vector256 temp2 = _mm256_permute4x64_pd(second, _MM_SHUFFLE(1, 3, 2, 3));
		temp3  = _mm256_permute4x64_pd(temp3, _MM_SHUFFLE(1, 3, 0, 1));
		result = NegativeMultiplySubtract(temp2, temp3, result);

		// term1 * first.yxxx
		Vector256 temp1 = _mm256_permute4x64_pd(first, _MM_SHUFFLE(0, 0, 0, 1));
		result = _mm256_mul_pd(result, temp1);

		// second.ywxz * third.wxwx
		temp2 = _mm256_permute4x64_pd(second, _MM_SHUFFLE(2, 0, 3, 1));
		temp3 = _mm256_permute4x64_pd(third, _MM_SHUFFLE(0, 3, 0, 3));
		temp3 = _mm256_mul_pd(temp3, temp2);

		// - second.wxwx * third.ywxz
		temp2 = _mm256_permute4x64_pd(temp2, _MM_SHUFFLE(2, 1, 2, 1));
		temp1 = _mm256_permute4x64_pd(third, _MM_SHUFFLE(2, 0, 3, 1));
		temp3 = NegativeMultiplySubtract(temp2, temp1, temp3);

		// result - temp * first.zzyy
		temp1 = _mm256_permute4x64_pd(first, _MM_SHUFFLE(1, 1, 2, 2));
		result = NegativeMultiplySubtract(temp1, temp3, result);

		// second.yzxy * third.zxyx
		temp2 = _mm256_permute4x64_pd(second, _MM_SHUFFLE(1, 0, 2, 1));
		temp3 = _mm256_permute4x64_pd(third, _MM_SHUFFLE(0, 1, 0, 2));
		temp3 = _mm256_mul_pd(temp3, temp2);

		// -second.zxyx * third.yzxy
		temp2 = _mm256_permute4x64_pd(temp2, _MM_SHUFFLE(2, 0, 2, 1));
		temp1 = _mm256_permute4x64_pd(third, _MM_SHUFFLE(1, 0, 2, 1));
		temp3 = NegativeMultiplySubtract(temp1, temp2, temp3);

		// result + term + first.wwwz
		temp1 = _mm256_permute4x64_pd(first, _MM_SHUFFLE(2, 3, 3, 3));
		result = MultiplyAdd(temp3, temp1, result);
	}

	/****************************************************************************
	*                      NormalizeVector2
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NormalizeVector2(ConstVector256 vector) noexcept
	*
	*  @brief     ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NormalizeVector2(ConstVector256 vector) noexcept
	{
		// 1��norm�v�Z
		Vector256 squareLength = _mm256_mul_pd(vector, vector);
		Vector256 temp = _mm256_permute4x64_pd(squareLength, _MM_SHUFFLE(1, 1, 1, 1));
		squareLength   = _mm256_add_pd(squareLength, temp);
		squareLength   = _mm256_permute4x64_pd(squareLength, _MM_SHUFFLE(0, 0, 0, 0));
		Vector256 norm = _mm256_sqrt_pd(squareLength);

		// 0�Ŋ�����\�������邩�ǂ����𒲂ׂ�. �l�������Ă�����1
		Vector256 zeroMask = _mm256_setzero_pd();
		zeroMask = _mm256_cmp_pd(zeroMask, norm, _CMP_NEQ_OQ);

		// ������łȂ��ꍇ�͑Ή�����v�f��1�ɂȂ�}�X�N�̍쐬
		Vector256 infinityMask = _mm256_cmp_pd(squareLength, VECTOR_256D_INFINITY, _CMP_NEQ_OQ);

		// ���K���̎��s
		Vector256 result = _mm256_div_pd(vector, norm);

		// 0�t�߂Ŋ����Ė�����ɂȂ������̂ɑ΂��Ă�0�ɐݒ�
		result = _mm256_and_pd(result, zeroMask);

		// �����傩�Ɋ�Â���qnan�܂���result��I������
		Vector256 temp1 = _mm256_andnot_pd(infinityMask, VECTOR_256D_QNAN);
		Vector256 temp2 = _mm256_and_pd(result, infinityMask);
		result = _mm256_or_pd(temp1, temp2);
		return result;
	}

	/****************************************************************************
	*                      NormalizeVector3
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NormalizeVector3(ConstVector256 vector) noexcept
	*
	*  @brief     ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NormalizeVector3(ConstVector256 vector) noexcept
	{
		// x, y, z�ɑ΂���h�b�g�ς̌v�Z
		// x*x, y*y, z*z, w*w
		Vector256 squareLength = _mm256_mul_pd(vector, vector);

		// z*z, y*y, z*z, y*y
		Vector256 temp         = _mm256_permute4x64_pd(squareLength, _MM_SHUFFLE(2, 1, 2, 1)); 

		// x*x + z*z, y*y, z*z, w*w
		squareLength .m256d_f64[0] = squareLength.m256d_f64[0] + temp.m256d_f64[0];

		// �S��y*y
		temp = _mm256_permute4x64_pd(temp, _MM_SHUFFLE(1, 1, 1, 1));

		// 1�v�f�ڂ�x*x + y*y + z*z
		squareLength.m256d_f64[0] = squareLength.m256d_f64[0] +  temp.m256d_f64[0];
		// �S�Ă̗v�f���h�b�g��
		squareLength = _mm256_permute4x64_pd(squareLength, _MM_SHUFFLE(0, 0, 0, 0));

		// 1���m�����̌v�Z
		Vector256 norm = _mm256_sqrt_pd(squareLength);

		// 0�Ŋ�����\�������邩�ǂ����𒲂ׂ�. �l�������Ă�����1
		Vector256 zeroMask = _mm256_setzero_pd();
		zeroMask = _mm256_cmp_pd(zeroMask, norm, _CMP_NEQ_OQ);

		// ������łȂ��ꍇ�͑Ή�����v�f��1�ɂȂ�}�X�N�̍쐬
		Vector256 infinityMask = _mm256_cmp_pd(squareLength, VECTOR_256D_INFINITY, _CMP_NEQ_OQ);

		// ���K���̎��s
		Vector256 result = _mm256_div_pd(vector, norm);

		// 0�t�߂Ŋ����Ė�����ɂȂ������̂ɑ΂��Ă�0�ɐݒ�
		result = _mm256_and_pd(result, zeroMask);

		// �����傩�Ɋ�Â���qnan�܂���result��I������
		Vector256 temp1 = _mm256_andnot_pd(infinityMask, VECTOR_256D_QNAN);
		Vector256 temp2 = _mm256_and_pd(result, infinityMask);
		result = _mm256_or_pd(temp1, temp2);
		return result;
	}

	/****************************************************************************
	*                      NormalizeVector4
	****************************************************************************/ 
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NormalizeVector4(ConstVector256 vector) noexcept
	*
	*  @brief     ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NormalizeVector4(ConstVector256 vector) noexcept
	{
		// �S�Ă̗v�f��4�v�f�̓��a
		Vector256 squareLength = _mm256_set1_pd(LengthSquaredVector4(vector));

		// 2���m�����̌v�Z
		Vector256 norm = _mm256_sqrt_pd(squareLength);

		// 0�Ŋ�����\�������邩�ǂ����𒲂ׂ�. �l�������Ă�����1
		Vector256 zeroMask = _mm256_setzero_pd();
		zeroMask = _mm256_cmp_pd(zeroMask, norm, _CMP_NEQ_OQ);

		// ������łȂ��ꍇ�͑Ή�����v�f��1�ɂȂ�}�X�N�̍쐬
		Vector256 infinityMask = _mm256_cmp_pd(squareLength, VECTOR_256D_INFINITY, _CMP_NEQ_OQ);

		// ���K���̎��s
		Vector256 result = _mm256_div_pd(vector, norm);

		// 0�t�߂Ŋ����Ė�����ɂȂ������̂ɑ΂��Ă�0�ɐݒ�
		result = _mm256_and_pd(result, zeroMask);

		// �����傩�Ɋ�Â���qnan�܂���result��I������
		Vector256 temp1 = _mm256_andnot_pd(infinityMask, VECTOR_256D_QNAN);
		Vector256 temp2 = _mm256_and_pd(result, infinityMask);
		result = _mm256_or_pd(temp1, temp2);
		return result;
	}

	/****************************************************************************
	*                      ReflectVector2
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ReflectVector2(ConstVector256 input, ConstVector256 normal) noexcept
	*
	*  @brief     ���˃x�N�g�����Z�o (input - (2 * dot(input, normal)) * normal)
	*
	*  @param[in] ConstVector256 : input  (���˃x�N�g��)
	*  @param[in] ConstVector256 : normal (���˂��s���@���x�N�g��)
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ReflectVector2(ConstVector256 input, ConstVector256 normal) noexcept
	{
		Vector256 result = Set(DotVector2(input, normal) * 2);
		return NegativeMultiplySubtract(result, normal, input);
	}

	/****************************************************************************
	*                      ReflectVector4
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ReflectVector4(ConstVector256 input, ConstVector256 normal) noexcept
	*
	*  @brief     ���˃x�N�g�����Z�o (input - (2 * dot(input, normal)) * normal)
	*
	*  @param[in] ConstVector256 : input  (���˃x�N�g��)
	*  @param[in] ConstVector256 : normal (���˂��s���@���x�N�g��)
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ReflectVector3(ConstVector256 input, ConstVector256 normal) noexcept
	{
		Vector256 result = Set(DotVector3(input, normal) * 2);
		return NegativeMultiplySubtract(result, normal, input);
	}

	/****************************************************************************
	*                      ReflectVector4
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ReflectVector4(ConstVector256 input, ConstVector256 normal)
	*
	*  @brief     ���˃x�N�g�����Z�o (input - (2 * dot(input, normal)) * normal)
	*
	*  @param[in] ConstVector256 : input  (���˃x�N�g��)
	*  @param[in] ConstVector256 : normal (���˂��s���@���x�N�g��)
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ReflectVector4(ConstVector256 input, ConstVector256 normal) noexcept
	{
		Vector256 result = Set(DotVector4(input, normal) * 2);
		return NegativeMultiplySubtract(result, normal, input);
	}

	/****************************************************************************
	*                      RefractVector2
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector2(ConstVector256 input, ConstVector256 normal, const double refractionIndex)
	*
	*  @brief     ���܃x�N�g�����Z�o
	*
	*  @param[in] ConstVector256 : input  (���˃x�N�g��)
	*  @param[in] ConstVector256 : normal (���܂��s���@���x�N�g��)
	*  @param[in]  const double   : refractionIndex (���ܗ�)
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector2(ConstVector256 input, ConstVector256 normal, const double refractionIndex) noexcept
	{
		return RefractVector2V(input, normal, Set(refractionIndex));
	}

	/****************************************************************************
	*                      RefractVector3
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector3(ConstVector256 input, ConstVector256 normal, const double refractionIndex)
	*
	*  @brief     ���܃x�N�g�����Z�o
	*
	*  @param[in] ConstVector256 : input  (���˃x�N�g��)
	*  @param[in] ConstVector256 : normal (���܂��s���@���x�N�g��)
	*  @param[in]  const double   : refractionIndex (���ܗ�)
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector3(ConstVector256 input, ConstVector256 normal, const double refractionIndex) noexcept
	{
		return RefractVector3V(input, normal, Set(refractionIndex));
	}

	/****************************************************************************
	*                      RefractVector4
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector4(ConstVector256 input, ConstVector256 normal, const double refractionIndex)
	*
	*  @brief     ���܃x�N�g�����Z�o
	*
	*  @param[in] ConstVector256 : input  (���˃x�N�g��)
	*  @param[in] ConstVector256 : normal (���܂��s���@���x�N�g��)
	*  @param[in]  const double   : refractionIndex (���ܗ�)
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector4(ConstVector256 input, ConstVector256 normal, const double refractionIndex) noexcept
	{
		return RefractVector4V(input, normal, Set(refractionIndex));
	}

	/****************************************************************************
	*                      RefractVector2
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector2(ConstVector256 input, ConstVector256 normal, const double refractionIndex)
	*
	*  @brief     ���܃x�N�g�����Z�o (�ٕ���������ꍇ)
	*
	*  @param[in] ConstVector256 : input  (���˃x�N�g��)
	*  @param[in] ConstVector256 : normal (���܂��s���@���x�N�g��)
	*  @param[in]  const double   : refractionIndex (���ܗ�)
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector2V(ConstVector256 input, ConstVector256 normal, ConstVector256 refractionIndex) noexcept
	{
		// Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) +
		// sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))

		Vector256 inputDotNormal = Set(DotVector2(input, normal));

		// vTemp = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
		Vector256 temp = NegativeMultiplySubtract(inputDotNormal, inputDotNormal, VECTOR_256D_ONE);
		temp = _mm256_mul_pd(temp, refractionIndex);
		temp = NegativeMultiplySubtract(temp, refractionIndex, VECTOR_256D_ONE);

		// ���ʂ�0�ȉ��ł����, sqrt�͎��s�Ƃ������Ƃ�zero�ɒu��������
		Vector256 zeroMask = _mm256_cmp_pd(temp, VECTOR_256D_ZERO, _CMP_GT_OQ);

		// r = refractionIndex * inputDotNormal + sqrt(r)
		temp = _mm256_sqrt_pd(temp);
		temp = MultiplyAdd(refractionIndex, inputDotNormal, temp);

		// result = refractionIndex * input - normal * r
		Vector256 result = _mm256_mul_pd(refractionIndex, input);
		result = NegativeMultiplySubtract(temp, normal, result);
		result = _mm256_and_pd(result, zeroMask);

		return result;
	}

	/****************************************************************************
	*                      RefractVector3
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector3(ConstVector256 input, ConstVector256 normal, const double refractionIndex)
	*
	*  @brief     ���܃x�N�g�����Z�o (�ٕ���������ꍇ)
	*
	*  @param[in] ConstVector256 : input  (���˃x�N�g��)
	*  @param[in] ConstVector256 : normal (���܂��s���@���x�N�g��)
	*  @param[in]  const double   : refractionIndex (���ܗ�)
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector3V(ConstVector256 input, ConstVector256 normal, ConstVector256 refractionIndex) noexcept
	{
		// Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) +
		// sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))
		Vector256 inputDotNormal = Set(DotVector3(input, normal));

		// vTemp = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
		Vector256 r  = NegativeMultiplySubtract(inputDotNormal, inputDotNormal, VECTOR_256D_ONE);
		Vector256 r2 = _mm256_mul_pd(refractionIndex, refractionIndex);
		r = NegativeMultiplySubtract(r, r2, VECTOR_256D_ONE);

		Vector256 result = _mm256_cmp_pd(r, VECTOR_256D_ZERO, _CMP_LE_OQ);

		if (_mm256_movemask_pd(result) == 0x0f)
		{
			// Total internal reflection
			result = VECTOR_256D_ZERO;
		}
		else
		{
			// r = refractionIndex * inputDotNormal + sqrt(r)
			r = _mm256_sqrt_pd(r);
			r = MultiplyAdd(refractionIndex, inputDotNormal, r);

			// result = refractionIndex * input - normal * r
			result = _mm256_mul_pd(refractionIndex, input);
			result = NegativeMultiplySubtract(r, normal, result);
		}

		return result;
	}

	/****************************************************************************
	*                      RefractVector4
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector4(ConstVector256 input, ConstVector256 normal, const double refractionIndex)
	*
	*  @brief     ���܃x�N�g�����Z�o (�ٕ���������ꍇ)
	*
	*  @param[in] ConstVector256 : input  (���˃x�N�g��)
	*  @param[in] ConstVector256 : normal (���܂��s���@���x�N�g��)
	*  @param[in]  const double   : refractionIndex (���ܗ�)
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector4V(ConstVector256 input, ConstVector256 normal, ConstVector256 refractionIndex) noexcept
	{
		// Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) +
		// sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))
		Vector256 inputDotNormal = Set(DotVector4(input, normal));

		// vTemp = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
		Vector256 r  = NegativeMultiplySubtract(inputDotNormal, inputDotNormal, VECTOR_256D_ONE);
		Vector256 r2 = _mm256_mul_pd(refractionIndex, refractionIndex);
		r = NegativeMultiplySubtract(r, r2, VECTOR_256D_ONE);

		Vector256 result = _mm256_cmp_pd(r, VECTOR_256D_ZERO, _CMP_LE_OQ);

		if (_mm256_movemask_pd(result) == 0x0f)
		{
			// Total internal reflection
			result = VECTOR_256D_ZERO;
		}
		else
		{
			// r = refractionIndex * inputDotNormal + sqrt(r)
			r = _mm256_sqrt_pd(r);
			r = MultiplyAdd(refractionIndex, inputDotNormal, r);

			// result = refractionIndex * input - normal * r
			result = _mm256_mul_pd(refractionIndex, input);
			result = NegativeMultiplySubtract(r, normal, result);
		}

		return result;
	}

	/****************************************************************************
	*                      IsInfiniteVector2
	****************************************************************************/
	/* @fn        inline bool Vector256Utility::IsInfiniteVector2(ConstVector256 vector) noexcept
	*
	*  @brief     ���ɑ傫���l��
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline bool Vector256Utility::IsInfiniteVector2(ConstVector256 vector) noexcept
	{
		// �����r�b�g���}�X�N�I�t����
		Vector256 temp = _mm256_and_pd(vector, VECTOR_256U_MASK_ABS);
		// ������Ɣ�r
		temp = _mm256_cmp_pd(temp, VECTOR_256D_INFINITY, _CMP_EQ_OQ);
		// �ǂꂩ��������ł���΁A�����͐^�ł���B
		return (_mm256_movemask_pd(temp) & 0b0011) != 0;
	}

	/****************************************************************************
	*                      IsInfiniteVector3
	****************************************************************************/
	/* @fn        inline bool Vector256Utility::IsInfiniteVector3(ConstVector256 vector) noexcept
	*
	*  @brief     ���ɑ傫���l��
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline bool Vector256Utility::IsInfiniteVector3(ConstVector256 vector) noexcept
	{
		// �����r�b�g���}�X�N�I�t����
		Vector256 temp = _mm256_and_pd(vector, VECTOR_256U_MASK_ABS);
		// ������Ɣ�r
		temp = _mm256_cmp_pd(temp, VECTOR_256D_INFINITY, _CMP_EQ_OQ);
		// �ǂꂩ��������ł���΁A�����͐^�ł���B
		return (_mm256_movemask_pd(temp) & 0b0111) != 0;
	}

	/****************************************************************************
	*                      IsInfiniteVector4
	****************************************************************************/
	/* @fn        inline bool Vector256Utility::IsInfiniteVector4(ConstVector256 vector) noexcept
	*
	*  @brief     ���ɑ傫���l��
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline bool Vector256Utility::IsInfiniteVector4(ConstVector256 vector) noexcept
	{
		// �����r�b�g���}�X�N�I�t����
		Vector256 temp = _mm256_and_pd(vector, VECTOR_256U_MASK_ABS);
		// ������Ɣ�r
		temp = _mm256_cmp_pd(temp, VECTOR_256D_INFINITY, _CMP_EQ_OQ);
		// �ǂꂩ��������ł���΁A�����͐^�ł���B
		return (_mm256_movemask_pd(temp) != 0);
	}

	/****************************************************************************
	*                      Sqrt
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Sqrt(ConstVector256 vector) noexcept
	*
	*  @brief      �S�Ă̗v�f�ɍ�������������̂�Ԃ��܂�
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Sqrt(ConstVector256 vector) noexcept
	{
		return _mm256_sqrt_pd(vector);
	}

	/****************************************************************************
	*                      ReciprocalSqrt
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ReciprocalSqrt(ConstVector256 vector) noexcept
	*
	*  @brief       �S�Ă̗v�f�̋t�������, ����ɍ�������������̂�Ԃ��܂�
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ReciprocalSqrt(ConstVector256 vector) noexcept
	{
		Vector256 result = _mm256_sqrt_pd(vector);
		result = _mm256_div_pd(VECTOR_256D_ONE, result);
		return result;
	}

	/****************************************************************************
	*                      Exp10
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Exp10(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��2^{vector}��Ԃ��܂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Exp2(ConstVector256 vector) noexcept
	{
		#if PLATFORM_CPU_INSTRUCTION_SVML
			return _mm256_exp2_pd(vector);
		#endif
	}

	/****************************************************************************
	*                      Exp10
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Exp10(ConstVector256 vector) noexcept
	*
	*  @brief      �S�Ă̗v�f��10^{vector}��Ԃ��܂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Exp10(ConstVector256 vector) noexcept
	{
		#if PLATFORM_CPU_INSTRUCTION_SVML
			return _mm256_exp10_pd(vector);
		#endif
	}

	/****************************************************************************
	*                      ExpE
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ExpE(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��e^{vector}��Ԃ��܂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ExpE(ConstVector256 vector) noexcept
	{
		#if PLATFORM_CPU_INSTRUCTION_SVML
			return _mm256_exp_pd(vector);
		#endif;
	}

	/****************************************************************************
	*                      Log2
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Log2(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��log2{vector}��Ԃ��܂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Log2(ConstVector256 vector) noexcept
	{
		#if PLATFORM_CPU_INSTRUCTION_SVML
			return _mm256_log2_pd(vector);
		#endif;
	}

	/****************************************************************************
	*                      Log10
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Log10(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��log10{vector}��Ԃ��܂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Log10(ConstVector256 vector) noexcept
	{
		#if PLATFORM_CPU_INSTRUCTION_SVML
			return _mm256_log10_pd(vector);
		#endif;
	}

	/****************************************************************************
	*                      LogE
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LogE(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��loge{vector}��Ԃ��܂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LogE(ConstVector256 vector) noexcept
	{
		#if PLATFORM_CPU_INSTRUCTION_SVML
			return _mm256_log_pd(vector);
		#endif;
	}

	/****************************************************************************
	*                      Pow
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Pow(ConstVector256 base, ConstVector256 power)
	*
	*  @brief     �S�Ă̗v�f��Pow��Ԃ��܂�  base^{power}
	*
	*  @param[in] ConstVector256 : base
	*  @param[in] ConstVector256 : power
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Pow(ConstVector256 base, ConstVector256 power) noexcept
	{
		return _mm256_pow_pd(base, power);
		ALIGNED_DATA(16) double a[4];
		ALIGNED_DATA(16) double b[4];
		_mm256_store_pd(a, base);
		_mm256_store_pd(b, power);
		Vector256 vResult = _mm256_setr_pd(
			pow(a[0], b[0]),
			pow(a[1], b[1]),
			pow(a[2], b[2]),
			pow(a[3], b[3]));
		return vResult;

	}

	/****************************************************************************
	*                      Sin
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Sin(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��Sin����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Sin(ConstVector256 vector) noexcept
	{
		#if PLATFORM_CPU_INSTRUCTION_SVML
			return _mm256_sin_pd(vector);
		#endif;
	}

	/****************************************************************************
	*                      Cos
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Cos(ConstVector256 vector) noexcepts
	*
	*  @brief     �S�Ă̗v�f��Cos����������̂�Ԃ��܂�
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Cos(ConstVector256 vector) noexcept
	{
		#if PLATFORM_CPU_INSTRUCTION_SVML
			return _mm256_cos_pd(vector);
		#endif;
	}

	/****************************************************************************
	*                      Tan
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Tan(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��Tan����������̂�Ԃ��܂�
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Tan(ConstVector256 vector) noexcept
	{
		#if PLATFORM_CPU_INSTRUCTION_SVML
			return _mm256_tan_pd(vector);
		#endif;
	}

	/****************************************************************************
	*                      SinCos
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SinCos(ConstVector256 input, Vector256* sin, Vector256* cos) noexcept
	*
	*  @brief     �S�Ă̗v�f�ɑ΂���sin������cos���������o���܂� 
	*
	*  @param[in] ConstVector256 : input
	*  @param[out] Vector256* sin
	*  @param[out] Vector256* cos
	*
	*  @return �@�@void
	*****************************************************************************/
	inline void SIMD_CALL_CONVENTION Vector256Utility::SinCos(ConstVector256 input, Vector256* sin, Vector256* cos) noexcept
	{
		#if PLATFORM_CPU_INSTRUCTION_SVML
			*sin = _mm256_sincos_pd(cos, input);
		#endif;
	}

	/****************************************************************************
	*                      SinH
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SinH(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��SinH����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SinH(ConstVector256 vector) noexcept
	{
		#if PLATFORM_CPU_INSTRUCTION_SVML
			return _mm256_sinh_pd(vector);
		#else
			// ���R�ΐ��̒��2����e�ɕϊ�
			static const Vector256 scale = { {  1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f } } ; // 1.0f / ln(2.0f)

			// sinh(x) = (e^x + e^-x) / 2 - 1�Ƃ��Čv�Z
			Vector256 v1 = MultiplyAdd(vector, scale, VECTOR_256D_NEGATIVE_ONE.V);
			Vector256 v2 = NegativeMultiplySubtract(vector, scale, VECTOR_256D_NEGATIVE_ONE.V);
			Vector256 e1 = Exp2(vector);
			Vector256 e2 = Exp2(vector);

			return _mm256_sub_pd(e1, e2);
		#endif;
	}

	/****************************************************************************
	*                      CosH
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CosH(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��CosH����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CosH(ConstVector256 vector) noexcept
	{
		#if PLATFORM_CPU_INSTRUCTION_SVML
			return _mm256_cosh_pd(vector);
		#else
		#endif
	}

	/****************************************************************************
	*                      TanH
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::TanH(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��TanH����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::TanH(ConstVector256 vector) noexcept
	{
		#if PLATFORM_CPU_INSTRUCTION_SVML
			return _mm256_tanh_pd(vector);
		#else
		#endif
	}

	/****************************************************************************
	*                      ArcSin
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ArcSin(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��ArcSin����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ArcSin(ConstVector256 vector) noexcept
	{
		#if PLATFORM_CPU_INSTRUCTION_SVML
			return _mm256_asin_pd(vector);
		#else
		#endif
	}

	/****************************************************************************
	*                      ArcCos
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ArcCos(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��ArcSin����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ArcCos(ConstVector256 vector) noexcept
	{
		#if PLATFORM_CPU_INSTRUCTION_SVML
			return _mm256_acos_pd(vector);
		#else
		#endif
	}

	/****************************************************************************
	*                      ArcTan
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ArcCos(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��ArcSin����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ArcTan(ConstVector256 vector) noexcept
	{
		#if PLATFORM_CPU_INSTRUCTION_SVML
			return _mm256_atan_pd(vector);
		#else
		#endif
	}

	/****************************************************************************
	*                       Lerp
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Lerp(ConstVector256 start, ConstVector256 end, const double t) noexcept
	*
	*  @brief     ���`��Ԃ�Ԃ��܂�
	*
	*  @param[in] ConstVector256  t = 0�̎��̒l
	*  @param[in] ConstVector256  t = 1�̎��̒l
	*  @param[in] double  t : ���`��Ԃ̊���
	*
	*  @return �@�@Vector
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Lerp(ConstVector256 start, ConstVector256 end, const double t) noexcept
	{
		Vector256 diff = _mm256_sub_pd(end, start);
		Vector256 scale = _mm256_set1_pd(t);
		return MultiplyAdd(diff, scale, start);
	}

	/****************************************************************************
	*                       LerpV
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LerpV(ConstVector256 start, ConstVector256 end, const double t) noexcept
	*
	*  @brief     ���`��Ԃ�Ԃ��܂�
	*
	*  @param[in] ConstVector256  t = 0�̎��̒l
	*  @param[in] ConstVector256  t = 1�̎��̒l
	*  @param[in] ConstVector256  t : ���`��Ԃ̊���
	*
	*  @return �@�@Vector
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LerpV(ConstVector256 start, ConstVector256 end, ConstVector256 t) noexcept
	{
		Vector256 diff = _mm256_sub_pd(end, start);
		return MultiplyAdd(diff, t, start);
	}

	/****************************************************************************
	*                       Hermite
	****************************************************************************/
	/* @fn        __forceinline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Hermite(ConstVector256 startPosition, ConstVector256 startTangent, ConstVector256 endPosition, ConstVector256 endTangent, const double t) noexcept
	*
	*  @brief     �G���~�[�g��Ԃ�Ԃ��܂�
	*
	*  @param[in]  �n�_�̈ʒu
	   @param[in]  �n�_�ɂ�����X�v���C���̐ڐ���\���x�N�g��
	   @param[in]  �I�_�̈ʒu
	   @param[in]  �I�_�ɂ�����X�v���C���̐ڐ���\���x�N�g��
	   @param[in]  t : �G���~�[�g��Ԃ̊���
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Hermite(ConstVector256 startPosition, ConstVector256 startTangent, ConstVector256 endPosition, ConstVector256 endTangent, const double t) noexcept
	{
		// Result = (2 * t^3 - 3 * t^2 + 1) * Position0 +
        //          (t^3 - 2 * t^2 + t) * Tangent0 +
        //          (-2 * t^3 + 3 * t^2) * Position1 +
        //          (t^3 - t^2) * Tangent1
		const double t2 = t * t;
		const double t3 = t * t2;

		Vector256 p0 = _mm256_set1_pd(2.0f * t3 - 3.0f * t2 + 1.0f);
		Vector256 t0 = _mm256_set1_pd(t3 - 2.0f * t2 + t);
		Vector256 p1 = _mm256_set1_pd(-2.0f * t3 + 3.0f * t2);
		Vector256 t1 = _mm256_set1_pd(t3 - t2);
		
		Vector256 result = _mm256_mul_pd(p0, startPosition);
		result = MultiplyAdd(startTangent, t0, result);
		result = MultiplyAdd(endPosition, p1, result);
		result = MultiplyAdd(endTangent , t1, result);
		return result;
	}

	/****************************************************************************
	*                       HermiteV
	****************************************************************************/
	/* @fn        __forceinline Vector256 SIMD_CALL_CONVENTION Vector256Utility::HermiteV(ConstVector256 startPosition, ConstVector256 startTangent, ConstVector256 endPosition, ConstVector256 endTangent, const double t) noexcept
	*
	*  @brief     �G���~�[�g��Ԃ�Ԃ��܂�
	*
	*  @param[in]  �n�_�̈ʒu
	   @param[in]  �n�_�ɂ�����X�v���C���̐ڐ���\���x�N�g��
	   @param[in]  �I�_�̈ʒu
	   @param[in]  �I�_�ɂ�����X�v���C���̐ڐ���\���x�N�g��
	   @param[in]  t : �G���~�[�g��Ԃ̊���
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::HermiteV(ConstVector256 startPosition, ConstVector256 startTangent, ConstVector256 endPosition, ConstVector256 endTangent, ConstVector256 t) noexcept
	{
		static const Vector256d CatMulT2 = { { { -3.0, -2.0, 3.0, -1.0 } } };
		static const Vector256d CatMulT3 = { { { 2.0, 1.0, -2.0, 1.0 } } };

		Vector256 t2 = _mm256_mul_pd(t, t);
		Vector256 t3 = _mm256_mul_pd(t, t2);
		// Mul by the constants against t^2
		t2 = _mm256_mul_pd(t2, CatMulT2.u.V);
		// Mul by the constants against t^3
		t3 = MultiplyAdd(t3, CatMulT3.u.V, t2);
		// T3 now has the pre-result.
		// I need to add t.y only
		t2 = _mm256_and_pd(t, VECTOR_256U_MASK_Y);
		t3 = _mm256_add_pd(t3, t2);
		// Add 1.0f to x
		t3 = _mm256_add_pd(t3, VECTOR_256D_IDENTITY_R0);
		// Now, I have the constants created
		// Mul the x constant to Position0
		Vector256 result = _mm256_permute4x64_pd(t3, _MM_SHUFFLE(0, 0, 0, 0));
		result = _mm256_mul_pd(result, startPosition);
		// Mul the y constant to Tangent0
		t2 = _mm256_permute4x64_pd(t3, _MM_SHUFFLE(1, 1, 1, 1));
		result = MultiplyAdd(t2, startTangent, result);
		// Mul the z constant to Position1
		t2 = _mm256_permute4x64_pd(t3, _MM_SHUFFLE(2, 2, 2, 2));
		result = MultiplyAdd(t2, endPosition, result);
		// Mul the w constant to Tangent1
		t3 = _mm256_permute4x64_pd(t3, _MM_SHUFFLE(3, 3, 3, 3));
		result = MultiplyAdd(t3, endTangent, result);
		return result;
	}

	/****************************************************************************
	*                       CatMullRom
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CatMullRom(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, ConstVector256 position3, const double t) noexcept
	*
	*  @brief     CatMull-Rom�X�v���C����Ԃ̌��ʂ�Ԃ��܂��B4�̐���_��S�Ēʂ�X�v���C�����
	*
	*  @param[in]  ����_1
	   @param[in]  ����_2
	   @param[in]  ����_3
	   @param[in]  ����_4
	   @param[in]  t : ��Ԃ̊���
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CatMullRom(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, ConstVector256 position3, const double t) noexcept
	{
		const double t2 = t * t;
		const double t3 = t * t2;

		Vector256 p0 = _mm256_set1_pd((-t3 + 2.0f * t2 - t) * 0.5f);
		Vector256 p1 = _mm256_set1_pd((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
		Vector256 p2 = _mm256_set1_pd((-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
		Vector256 p3 = _mm256_set1_pd((t3 - t2) * 0.5f);

		p1 = _mm256_mul_pd(position1, p1);
		p0 = MultiplyAdd  (position0, p0, p1);
		p3 = _mm256_mul_pd(position3, p3);
		p2 = MultiplyAdd  (position2, p2, p3);
		p0 = _mm256_add_pd(p0, p2);
		return p0;
	}

	/****************************************************************************
	*                       CatMullRom
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CatMullRom(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, ConstVector256 position3, const double t) noexcept
	*
	*  @brief     CatMull-Rom�X�v���C����Ԃ̌��ʂ�Ԃ��܂��B4�̐���_��S�Ēʂ�X�v���C�����
	*
	*  @param[in]  ����_1
	   @param[in]  ����_2
	   @param[in]  ����_3
	   @param[in]  ����_4
	   @param[in]  t : ��Ԃ̊���
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CatMullRomV(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, ConstVector256 position3, ConstVector256 t) noexcept
	{
		static const Vector256d Catmul2 = { { { 2.0, 2.0, 2.0, 2.0 } } };
		static const Vector256d Catmul3 = { { { 3.0, 3.0, 3.0, 3.0 } } };
		static const Vector256d Catmul4 = { { { 4.0, 4.0, 4.0, 4.0 } } };
		static const Vector256d Catmul5 = { { { 5.0, 5.0, 5.0, 5.0 } } };
		// Cache T^2 and T^3
		Vector256 t2 = _mm256_mul_pd(t, t);
		Vector256 t3 = _mm256_mul_pd(t, t2);
		// Perform the Position0 term
		Vector256 vResult = _mm256_add_pd(t2, t2);
		vResult = _mm256_sub_pd(vResult, t);
		vResult = _mm256_sub_pd(vResult, t3);
		vResult = _mm256_mul_pd(vResult, position0);
		// Perform the Position1 term and add
		Vector256 vTemp = _mm256_mul_pd(t3, Catmul3.u.V);
		vTemp = NegativeMultiplySubtract(t2, Catmul5.u.V, vTemp);
		vTemp = _mm256_add_pd(vTemp, Catmul2.u.V);
		vResult = MultiplyAdd(vTemp, position1, vResult);
		// Perform the Position2 term and add
		vTemp = _mm256_mul_pd(t2, Catmul4.u.V);
		vTemp = NegativeMultiplySubtract(t3, Catmul3.u.V, vTemp);
		vTemp = _mm256_add_pd(vTemp, t);
		vResult = MultiplyAdd(vTemp, position2, vResult);
		// Position3 is the last term
		t3 = _mm256_sub_pd(t3, t2);
		vResult = MultiplyAdd(t3, position3, vResult);
		// Multiply by 0.5f and exit
		vResult = _mm256_mul_pd(vResult, VECTOR_256D_ONE_HALF);
		return vResult;
	}

	/****************************************************************************
	*                      BaryCentric
	****************************************************************************/
	/* @fn        __forceinline Vector256 SIMD_CALL_CONVENTION Vector256Utility::BaryCentric(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, const double f, const double g) noexcept
	*
	*  @brief     BaryCentric �O�p�`�̏d�S�d�݈ʒu���Z�o
	*              (f>=0 && g>=0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�͎O�p�`�� Position0>Position1>Position2 �̓����ɂ���܂�>�B
				   (f==0 && g>=0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�� Position0>Position2 �s�ڂɂ���܂�>�B
				   (f>=0 && g==0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�� Position0>Position1> �s�ڂɂ���܂��B
				   (f>=0 && g>=0 && 1-f-g==0) �̏ꍇ�A�|�C���g�� Position1>Position2 �s�ڂɂ���܂�>�B
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::BaryCentric(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, const double f, const double g) noexcept
	{
		Vector256 r1 = _mm256_sub_pd(position1, position0);
		Vector256 r2 = _mm256_sub_pd(position2, position0);
		Vector256 sf = _mm256_set1_pd(f);
		r1 = MultiplyAdd(r1, sf, position0);
		Vector256 sg = _mm256_set1_pd(g);
		return MultiplyAdd(r2, sg, r1);
	}

	/****************************************************************************
	*                      BaryCentricV
	****************************************************************************/
	/* @fn        __forceinline Vector256 SIMD_CALL_CONVENTION Vector256Utility::BaryCentricV(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, const double f, const double g) noexcept
	*
	*  @brief     BaryCentric �O�p�`�̏d�S�d�݈ʒu���Z�o
	*              (f>=0 && g>=0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�͎O�p�`�� Position0>Position1>Position2 �̓����ɂ���܂�>�B
				   (f==0 && g>=0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�� Position0>Position2 �s�ڂɂ���܂�>�B
				   (f>=0 && g==0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�� Position0>Position1> �s�ڂɂ���܂��B
				   (f>=0 && g>=0 && 1-f-g==0) �̏ꍇ�A�|�C���g�� Position1>Position2 �s�ڂɂ���܂�>�B
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::BaryCentricV(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, ConstVector256 f, ConstVector256 g) noexcept
	{
		Vector256 r1 = _mm256_sub_pd(position1, position0);
		Vector256 r2 = _mm256_sub_pd(position2, position0);
		r1 = MultiplyAdd(r1, f, position0);
		return MultiplyAdd(r2, g, r1);
	}

	/****************************************************************************
	*                       Min
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Min(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     �S�Ă̗v�f�ɂ��čŏ��l�ƂȂ����I�����܂�
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@Vector
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Min(ConstVector256 left, ConstVector256 right) noexcept
	{
		return _mm256_min_pd(left, right);
	}

	/****************************************************************************
	*                       Max
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Max(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     �S�Ă̗v�f�ɂ��čő�l�ƂȂ����I�����܂�
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Max(ConstVector256 left, ConstVector256 right) noexcept
	{
		return _mm256_max_pd(left, right);
	}

	/****************************************************************************
	*                       Round
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Round(ConstVector256 vector) noexcept
	*
	*  @brief     �ł��߂������Ɋۂ߂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Round(ConstVector256 vector) noexcept
	{
		return _mm256_round_pd(vector, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
	}

	/****************************************************************************
	*                       Truncate
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Truncate(ConstVector256 vector) noexcept
	*
	*  @brief     �؂�̂� (�w�肵�����l�̒[����؂�̂Ă������Ɋۂ߂�B���������0�̕����ɂ��鎟�̐���
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Truncate(ConstVector256 vector) noexcept
	{
		return _mm256_round_pd(vector, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
	}

	/****************************************************************************
	*                       Floor
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Floor(ConstVector256 vector) noexcept
	*
	*  @brief     ���̕����Ɋۂ߂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Floor(ConstVector256 vector) noexcept
	{
		return _mm256_round_pd(vector, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC);
	}

	/****************************************************************************
	*                      Ceiling
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Ceiling(ConstVector256 vector) noexcept
	*
	*  @brief     �؂�グ ��������Ő��̕����ɂ��鎟�̐����Ɋۂ߂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Ceiling(ConstVector256 vector) noexcept
	{
		return _mm256_round_pd(vector, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC);
	}

	/****************************************************************************
	*                       Clamp
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Clamp(ConstVector256 vector, ConstVector256 min, ConstVector256 max) noexcept
	*
	*  @brief     �w��͈͓��Ő��l��ݒ肷��
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] ConstVector256 min
	*  @param[in] ConstVector256 max
	*
	*  @return �@�@Vector
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Clamp(ConstVector256 vector, ConstVector256 min, ConstVector256 max) noexcept
	{
		Check(LessOrEqualVector4(min, max));
		Vector256 result = _mm256_max_pd(min, vector);
		return _mm256_min_pd(max, result);
	}

	/****************************************************************************
	*                       Saturate
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Saturate(ConstVector256 vector)
	*
	*  @brief     1�ȏ�ɂ͍s���Ȃ��悤�ɂ���
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Saturate(ConstVector256 vector) noexcept
	{
		Vector256 result = _mm256_max_pd(vector, VECTOR_256D_ZERO);
		return _mm256_min_pd(result, VECTOR_256D_ONE);
	}

	/****************************************************************************
	*                       Abs
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Saturate(ConstVector256 vector)
	*
	*  @brief     ��Βl
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Abs(ConstVector256 vector) noexcept
	{
		return _mm256_and_pd(vector, VECTOR_256D_MASK_SIGN);
	}

	#pragma endregion Math

	#pragma region Angle
	/****************************************************************************
	*                       ModAngles
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ModAngles(ConstVector256 vector)
	*
	*  @brief      - pi <= theta <= pi�͈̔͂Ɏ��߂�悤�ɂ��܂�.
	*
	*  @param[in] ConstVector256 angles : �p�xrad
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ModAngles(ConstVector256 angles) noexcept
	{
		// PI <= angle < PI �ƂȂ�悤�ɁA�^����ꂽ�p�x�͈̔͂̃��W�������Z���s��
		Vector256 result = _mm256_mul_pd(angles, VECTOR_256D_RECIPROCAL_2PI);

		// �ۂߏ��������G�Ȃ��߁A�C�����C���֐����g�p����B
		result = Round(result);
		return NegativeMultiplySubtract(result, VECTOR_256D_2PI, angles);
	}

	/****************************************************************************
	*                       AddAngles
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::AddAngles(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     - pi <= theta <= pi�͈̔͂Ɏ��߂�悤�ɂ�����ő����Z���s���܂�.
	*
	*  @param[in] ConstVector256 left angles : �p�xrad
	*  @param[in] ConstVector256 right angles : �p�xrad
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::AddAngles(ConstVector256 left, ConstVector256 right) noexcept
	{
		// �p�x�̏����ȍ���
		Vector256 result = _mm256_add_pd(left, right);

		// -Pi�������������𔻒肷��
		Vector256 offset = _mm256_cmp_pd(result, VECTOR_256D_NEGATIVE_PI, _CMP_LT_OQ);
		offset = _mm256_and_pd(offset, VECTOR_256D_2PI);

		// -Pi�����S�Ă̗v�f�ŏ������ꍇ, 2Pi��������
		result = _mm256_add_pd(result, offset);

		// Pi�����傫���Ȃ��Ă��܂��Ă��邩
		offset = _mm256_cmp_pd(result, VECTOR_256D_PI, _CMP_GE_OQ);
		offset = _mm256_and_pd(offset, VECTOR_256D_2PI);

		// pi�����傫���ꍇ��2pi��S�Ă̗v�f�ň���
		result = _mm256_sub_pd(result, offset);
		return result;
	}

	/****************************************************************************
	*                       SubtractAngles
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ModAngles(ConstVector256 vector)
	*
	*  @brief     - pi <= theta <= pi�͈̔͂Ɏ��߂�悤�ɂ�����ň����Z���s���܂�.
	*
	*  @param[in] ConstVector256 left angles : �p�xrad
	*  @param[in] ConstVector256 right angles : �p�xrad
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SubtractAngles(ConstVector256 left, ConstVector256 right) noexcept
	{
		// �p�x�̏����ȍ���
		Vector256 result = _mm256_sub_pd(left, right);

		// -Pi�������������𔻒肷��
		Vector256 offset = _mm256_cmp_pd(result, VECTOR_256D_NEGATIVE_PI, _CMP_LT_OQ);
		offset           = _mm256_and_pd(offset, VECTOR_256D_2PI);

		// -Pi�����S�Ă̗v�f�ŏ������ꍇ, 2Pi��������
		result = _mm256_add_pd(result, offset);

		// Pi�����傫���Ȃ��Ă��܂��Ă��邩
		offset = _mm256_cmp_pd(result, VECTOR_256D_PI, _CMP_GE_OQ);
		offset = _mm256_and_pd(offset, VECTOR_256D_2PI);

		// pi�����傫���ꍇ��2pi��S�Ă̗v�f�ň���
		result = _mm256_sub_pd(result, offset);
		return result;
	}
	#pragma endregion Angle
#pragma endregion Implement
}


#endif
#endif