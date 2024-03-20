//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdVector256Non.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/11 2:29:40
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_VECTOR_256_NON_HPP
#define GM_SIMD_VECTOR_256_NON_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMSimdMacros.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if !defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
#include "GameUtility/Base/Include/GUAssert.hpp"
#include <math.h>

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm::simd::non
{
	/*----------------------------------------------------------------------
	*           256 bit�̐����^��double�^���i�[�\�ȃx�N�g�����쐬����
	*----------------------------------------------------------------------*/
	struct __vector256
	{
		union
		{
			gu::double64 D64[4];
			gu::uint64   U64[4];
		};
	};
	using Vector256 = __vector256;

	/*----------------------------------------------------------------------
	*        �萔�Ƃ��Ă̕ϊ��p, double�|�C���^�ɕϊ�������, m128�̊e�^�ɕϊ�������
	*----------------------------------------------------------------------*/
	ALIGNED_STRUCT(16) Vector256d
	{
		union
		{
			double     D[4];
			Vector256 V;
		};

		inline operator Vector256()    const noexcept { return V; }
		inline operator const double* () const noexcept { return D; }
	};

	/*----------------------------------------------------------------------
	*        �萔�Ƃ��Ă̕ϊ��p, double�|�C���^�ɕϊ�������, m128�̊e�^�ɕϊ�������
	*----------------------------------------------------------------------*/
	ALIGNED_STRUCT(16) Vector256u
	{
		union
		{
			gu::uint64 U[4];
			Vector256  V;
		};

		inline operator Vector256()    const noexcept { return V; }
	};

	constexpr double GM_PI_DOUBLE        = 3.141592653589793238463;
	constexpr double GM_2PI_DOUBLE       = 6.283185307179586476925;
	constexpr double GM_1_DIV_PI_DOUBLE  = 0.3183098861837906715378;
	constexpr double GM_1_DIV_2PI_DOUBLE = 0.1591549430918953357689;
	constexpr double GM_PI_DIV2_DOUBLE   = 1.570796326794896619231;
	constexpr double GM_PI_DIV4_DOUBLE   = 0.7853981633974483096157;
	GLOBAL_CONST Vector256d VECTOR_256D_ONE               = { { {  1.0, 1.0, 1.0, 1.0  } } };
	GLOBAL_CONST Vector256d VECTOR_256D_ZERO              = { { {  0.0, 0.0, 0.0, 0.0  } } };
	GLOBAL_CONST Vector256d VECTOR_256D_NEGATIVE_ONE      = { { {  -1.0, -1.0, -1.0, -1.0 } } };
	GLOBAL_CONST Vector256u VECTOR_256U_MASK_SIGN         = { { { ~(gu::uint64(1) << 63), ~(gu::uint64(1) << 63), ~(gu::uint64(1) << 63), ~(gu::uint64(1) << 63) } } };
	GLOBAL_CONST Vector256u VECTOR_256U_MASK_ABS          = { { {  ~gu::uint64(0), ~gu::uint64(0), ~gu::uint64(0), gu::uint64(0)  } } };
	GLOBAL_CONST Vector256u VECTOR_256U_NEGATIVE_ONE_MASK = { { { 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF } } };
	GLOBAL_CONST Vector256d VECTOR_256D_ONE_HALF          = { { { 0.5, 0.5, 0.5, 0.5 } } };
	GLOBAL_CONST Vector256d VECTOR_256D_HALF_PI           = { { {  GM_PI_DIV2_DOUBLE  , GM_PI_DIV2_DOUBLE  , GM_PI_DIV2_DOUBLE , GM_PI_DIV2_DOUBLE } } };
	GLOBAL_CONST Vector256d VECTOR_256D_PI                = { { {  GM_PI_DOUBLE       , GM_PI_DOUBLE       , GM_PI_DOUBLE , GM_PI_DOUBLE  } } };
	GLOBAL_CONST Vector256d VECTOR_256D_NEGATIVE_PI       = { { {  -GM_PI_DOUBLE      , -GM_PI_DOUBLE      , -GM_PI_DOUBLE , -GM_PI_DOUBLE  } } };
	GLOBAL_CONST Vector256d VECTOR_256D_NEGATIVE_2PI      = { { {  -GM_2PI_DOUBLE      , -GM_2PI_DOUBLE      , -GM_2PI_DOUBLE , -GM_2PI_DOUBLE  } } };
	GLOBAL_CONST Vector256d VECTOR_256D_RECIPROCAL_PI     = { { {  GM_1_DIV_PI_DOUBLE , GM_1_DIV_PI_DOUBLE , GM_1_DIV_PI_DOUBLE , GM_1_DIV_PI_DOUBLE  } } };
	GLOBAL_CONST Vector256d VECTOR_256D_RECIPROCAL_2PI    = { { {  GM_1_DIV_2PI_DOUBLE, GM_1_DIV_2PI_DOUBLE, GM_1_DIV_2PI_DOUBLE, GM_1_DIV_2PI_DOUBLE } } };
	GLOBAL_CONST Vector256d VECTOR_256D_2PI               = { { {  GM_2PI_DOUBLE      , GM_2PI_DOUBLE      , GM_2PI_DOUBLE, GM_2PI_DOUBLE} } };
	GLOBAL_CONST Vector256u VECTOR_256U_MASK_X            = { { { 0xFFFFFFFFFFFFFFFF, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000  } } };
	GLOBAL_CONST Vector256u VECTOR_256U_MASK_Y            = { { { 0x0000000000000000, 0xFFFFFFFFFFFFFFFF, 0x0000000000000000, 0x0000000000000000  } } };
	GLOBAL_CONST Vector256u VECTOR_256U_MASK_Z            = { { { 0x0000000000000000, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF, 0x0000000000000000  } } };
	GLOBAL_CONST Vector256u VECTOR_256U_MASK_W            = { { { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  } } };
	GLOBAL_CONST Vector256u VECTOR_256U_MASK_XYZ          = { { { 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0x0000000000000000  } } };
	GLOBAL_CONST Vector256d VECTOR_256D_IDENTITY_R0       = { { { 1.0, 0.0, 0.0, 0.0 } } };
	GLOBAL_CONST Vector256d VECTOR_256D_IDENTITY_R1       = { { { 0.0, 1.0, 0.0, 0.0 } } };
	GLOBAL_CONST Vector256d VECTOR_256D_IDENTITY_R2       = { { { 0.0, 0.0, 1.0, 0.0 } } };
	GLOBAL_CONST Vector256d VECTOR_256D_IDENTITY_R3       = { { { 0.0, 0.0, 0.0, 1.0 } } };
	GLOBAL_CONST Vector256d VECTOR_256D_INFINITY          = { { { 0x7FF0000000000000, 0x7FF0000000000000, 0x7FF0000000000000, 0x7FF0000000000000 } } };
	GLOBAL_CONST Vector256d VECTOR_256D_QNAN              = { { { 0x7FF8000000000000, 0x7FF8000000000000, 0x7FF8000000000000, 0x7FF8000000000000 } } };

	/****************************************************************************
	*				  			  Vector256Utility
	*************************************************************************//**
	*  @class     Vector256Utility
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
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Zero() noexcept;

		/*----------------------------------------------------------------------
		*  @brief : double4����Vector256�N���X�𐶐�����
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Set(const double x, const double y, const double z, const double w) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : double1����Vector256�N���X�𐶐�����
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Set(const double value) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : Int4����Vector256�N���X�𐶐�����
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION SetInt(const gu::uint32 x, const gu::uint32 y, const gu::uint32 z, const gu::uint32 w) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : Double���g����Vector��X(, Y, Z, W)�v�f�ɒl��������
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION SetX(ConstVector256 vector, const double x) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION SetY(ConstVector256 vector, const double y) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION SetZ(ConstVector256 vector, const double z) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION SetW(ConstVector256 vector, const double w) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �x�N�g�����g����double�z��ɑ������
		/*----------------------------------------------------------------------*/
		__forceinline static void SIMD_CALL_CONVENTION StoreDouble(double* destination, ConstVector256 source) noexcept;
		__forceinline static void SIMD_CALL_CONVENTION StoreDouble2(double* destination, ConstVector256 source) noexcept;
		__forceinline static void SIMD_CALL_CONVENTION StoreDouble3(double* destination, ConstVector256 source) noexcept;
		__forceinline static void SIMD_CALL_CONVENTION StoreDouble4(double* destination, ConstVector256 source) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : X(, Y, Z, W)���̗v�f�Ɋ�Â���, �S�Ă̗v�f�ɓ����l��������Vector256�N���X���쐬����
		*           X,Y,Z,W  : �e�v�f
		*           One      : 1, 1, 1, 1
		*           Infinity : INF, INF, INF, INF
		*           Epsilon  : doubleEpsilon
		/*----------------------------------------------------------------------*/
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
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Swizzle(ConstVector256 vector,
			const gu::uint32 xIndex, const gu::uint32 yIndex,
			const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g������v�f�𒊏o��, �V�����x�N�g�����쐬����
		*           0�`3 : a�����x, y, z, w
		*           4�`7 : b�����x, y, z, w
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Permute(ConstVector256 a, ConstVector256,
			const gu::uint32 xIndex, const gu::uint32 yIndex,
			const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g������v�f�𒊏o��, �V�����x�N�g�����쐬����
		*           control vector��0�̏ꍇ��left����, ��0�̏ꍇ��right���炻�ꂼ�ꒊ�o���s���܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Select(ConstVector256 left, ConstVector256 right, ConstVector256 control) noexcept;
		#pragma endregion Setter

		#pragma region Getter
		/*----------------------------------------------------------------------
		*  @brief : FPU���W�X�^�Ɋi�[���ꂽX(, Y, Z, W)���̗v�f�����o��
		/*----------------------------------------------------------------------*/
		__forceinline static double SIMD_CALL_CONVENTION GetX(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION GetY(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION GetZ(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION GetW(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : double�̔z����g���ăx�N�g���Ɋi�[����
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION LoadDouble(const double* source) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION LoadDouble2(const double* source) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION LoadDouble3(const double* source) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION LoadDouble4(const double* source) noexcept;

		#pragma endregion Getter

		#pragma region Operator
		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g���̊e�v�f���m�̘a���Z�o���܂�. (left + right)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Add(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g���̊e�v�f���m�̍����Z�o���܂�. (left - right)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Subtract(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g���̊e�v�f���m�̐ς��Z�o���܂�. (left * right)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Multiply(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g���̊e�v�f���m�̏��Z���Z�o���܂�. (left / right)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Divide(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g���̊e�v�f���m�̂��܂���Z�o���܂�. (left % right)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Mod(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief :  subtractLeft - (multiplyLeft * multiplyRight)���Z�o���܂�.
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION NegativeMultiplySubtract(ConstVector256 multiplyLeft, ConstVector256 multiplyRight, ConstVector256 subtractLeft) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : multiplyLeft * multiplyRight + add���Z�o���܂�.
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION MultiplyAdd(ConstVector256 multiplyLeft, ConstVector256 multiplyRight, ConstVector256 add) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �x�N�g���̊e�v�f���m�̋t�����Z�o���܂�. (1.0f / vector)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Reciprocal(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : double�l���g���Ċe�v�f�Ɋ|���܂� scale * vector
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Scale(ConstVector256 vector, const double scale) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ��̃x�N�g�������������ǂ����𒲂ׂ܂�
		*           (Each�͊e�v�f���ɓ��������ǂ����𒲂ׂ܂�.)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION EqualVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION EqualVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION EqualVector4(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION EqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ��̃x�N�g�����������Ȃ����𒲂ׂ܂�
		*           (Each�͊e�v�f���ɓ��������ǂ����𒲂ׂ܂�.)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualVector4(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION NotEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ��̃x�N�g�������ׂē��������ǂ����𒲂ׂ܂�.
		*           ��r���ɂ͕����Ȃ�����(U64)�Ƃ��Ĕ�r���܂�.
		*           (Each�͊e�v�f���Ƃɓ��������ǂ����𒲂ׂ܂�)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION EqualAsIntVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION EqualAsIntVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION EqualAsIntVector4(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION EqualAsIntVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ��̃x�N�g������ł��������Ȃ����ǂ����𒲂ׂ܂�.
		*           ��r���ɂ͕����Ȃ�����(U64)�Ƃ��Ĕ�r���܂�.
		*           (Each�͊e�v�f���Ƃɓ��������ǂ����𒲂ׂ܂�)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualAsIntVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualAsIntVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualAsIntVector4(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION NotEqualAsIntVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���͈͓��Œl�����������𒲂ׂ� (left - epsilon <= right <= left + epsilon)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector2(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector3(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector4(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION NearEqualVectorEach(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̕����S�Ă̗v�f�ɂ����đ傫�����𒲂ׂ܂� (left > right)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterVector4(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION GreaterVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̕����S�Ă̗v�f�ɂ����đ傫��or���������𒲂ׂ܂� (left >= right)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterOrEqualVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterOrEqualVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterOrEqualVector4(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION GreaterOrEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̕����S�Ă̗v�f�ɂ����ď��������𒲂ׂ܂� (left < right)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION LessVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessVector4(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION LessVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̕����S�Ă̗v�f�ɂ����ď�����or���������𒲂ׂ܂� (left <= right)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION LessOrEqualVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessOrEqualVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessOrEqualVector4(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION LessOrEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �w��͈͓��ɂ��邩�ǂ��� -bounds <= vector <= +bounds
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION InBoundsVector2(ConstVector256 vector, ConstVector256 bounds) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION InBoundsVector3(ConstVector256 vector, ConstVector256 bounds) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION InBoundsVector4(ConstVector256 vector, ConstVector256 bounds) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION InBoundsVectorEach(ConstVector256 vector, ConstVector256 bounds) noexcept;

		#pragma endregion Operator

		#pragma region Bit
		/*----------------------------------------------------------------------
		*  @brief : �r�b�g�P�ʂ̘_���ς̉��Z���s���܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION AndInt(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �r�b�g�P�ʂ̘_���a�̉��Z���s���܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION OrInt(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �r�b�g�P�ʂ̘_���ϔے�̉��Z���s���܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION NorInt(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �r�b�g�P�ʂ̔r���I�_���a�̉��Z���s���܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION XorInt(ConstVector256 left, ConstVector256 right) noexcept;

		#pragma endregion Bit

		#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : �e�v�f�Ƀ}�C�i�X����������̂�Ԃ�.
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Negate(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 1���m����(�e�v�f�̐�Βl�̘a�ɂ�����x�N�g���̑傫��)���Z�o
		/*----------------------------------------------------------------------*/
		__forceinline static double SIMD_CALL_CONVENTION NormVector2(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION NormVector3(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION NormVector4(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2���m����(�e�v�f��2��a�����[�g��������Ƃ��̃x�N�g���̑傫��)���Z�o
		/*----------------------------------------------------------------------*/
		__forceinline static double SIMD_CALL_CONVENTION NormSquaredVector2(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION NormSquaredVector3(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION NormSquaredVector4(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���ς��Z�o (������, �Ԃ�l��double�ł͂Ȃ�Vector256�^�ŕԂ���܂��̂�, ��XGetX�ȂǂőΉ����Ă�������.)
		/*----------------------------------------------------------------------*/
		__forceinline static double SIMD_CALL_CONVENTION DotVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION DotVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION DotVector4(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �O�ς��Z�o left, right�ɐ����ȃx�N�g����Ԃ�
		*           2�����̏ꍇ�͕��s�l�ӌ`�̖ʐς�Ԃ��Ƃ����Ӗ�����
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION CrossVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION CrossVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION CrossVector4(ConstVector256 first, ConstVector256 second, ConstVector256 third) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION NormalizeVector2(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION NormalizeVector3(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION NormalizeVector4(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief     ���˃x�N�g�����Z�o
		*  @param[in] ConstVector256 : input  (���˃x�N�g��)
		*  @param[in] ConstVector256 : normal (���˂��s���@���x�N�g��)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION ReflectVector2(ConstVector256 input, ConstVector256 normal) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION ReflectVector3(ConstVector256 input, ConstVector256 normal) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION ReflectVector4(ConstVector256 input, ConstVector256 normal) noexcept;

		/*----------------------------------------------------------------------
		*  @brief     ���܃x�N�g�����Z�o
		*  @param[in] ConstVector256 : input  (���˃x�N�g��)
		*  @param[in] ConstVector256 : normal (���˂��s���@���x�N�g��)
		*  @param[in] const double    : refractionIndex (���ܗ�)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION RefractVector2(ConstVector256 input, ConstVector256 normal, const double refractionIndex) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION RefractVector3(ConstVector256 input, ConstVector256 normal, const double refractionIndex) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION RefractVector4(ConstVector256 input, ConstVector256 normal, const double refractionIndex) noexcept;

		__forceinline static Vector256 SIMD_CALL_CONVENTION RefractVector2V(ConstVector256 input, ConstVector256 normal, ConstVector256 refractionIndex) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION RefractVector3V(ConstVector256 input, ConstVector256 normal, ConstVector256 refractionIndex) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION RefractVector4V(ConstVector256 input, ConstVector256 normal, ConstVector256 refractionIndex) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���ɑ傫���l��
		/*----------------------------------------------------------------------*/
		__forceinline static bool IsInfiniteVector2(ConstVector256 vector) noexcept;
		__forceinline static bool IsInfiniteVector3(ConstVector256 vector) noexcept;
		__forceinline static bool IsInfiniteVector4(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�ɍ�������������̂�Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Sqrt(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�̋t�������, ����ɍ�������������̂�Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION ReciprocalSqrt(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��sin, cos, tan��Ԃ��܂� [rad]
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Sin(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION Cos(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION Tan(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��arcsin, arccos, arctan��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION ArcSin(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION ArcCos(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION ArcTan(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��sinH, cosH, tanH��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION SinH(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION CosH(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION TanH(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�ɑ΂���sin������cos���������o���܂�
		/*----------------------------------------------------------------------*/
		__forceinline static void SIMD_CALL_CONVENTION SinCos(ConstVector256 input, Vector256* sin, Vector256* cos) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��2^{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Exp2(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��10^{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Exp10(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��e^{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION ExpE(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��log2{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Log2(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��log10{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Log10(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��loge{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION LogE(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��Pow��Ԃ��܂�
		*           base^{power}
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Pow(ConstVector256 base, ConstVector256 power) noexcept;

		/*----------------------------------------------------------------------
		*  @brief      ���`��Ԃ�Ԃ��܂�
		*  @param[in]  t = 0�̎��̒l
		   @param[in]  t = 1�̎��̒l
		   @param[in]  t : ���`��Ԃ̊���
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Lerp(ConstVector256 start, ConstVector256 end, const double t) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION LerpV(ConstVector256 start, ConstVector256 end, ConstVector256 t) noexcept;


		/*----------------------------------------------------------------------
		*  @brief      �G���~�[�g��Ԃ�Ԃ��܂�
		*  @param[in]  �n�_�̈ʒu
		   @param[in]  �n�_�ɂ�����X�v���C���̐ڐ���\���x�N�g��
		   @param[in]  �I�_�̈ʒu
		   @param[in]  �I�_�ɂ�����X�v���C���̐ڐ���\���x�N�g��
		   @param[in]  t : �G���~�[�g��Ԃ̊���
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Hermite(ConstVector256 startPosition, ConstVector256 startTangent, ConstVector256 endPosition, ConstVector256 endTangent, const double t) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION HermiteV(ConstVector256 startPosition, ConstVector256 startTangent, ConstVector256 endPosition, ConstVector256 endTangent, ConstVector256 t) noexcept;

		/*----------------------------------------------------------------------
		*  @brief      CatMull-Rom�X�v���C����Ԃ̌��ʂ�Ԃ��܂��B4�̐���_��S�Ēʂ�X�v���C�����
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION CatMullRom(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, ConstVector256 position3, const double t) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION CatMullRomV(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, ConstVector256 position3, ConstVector256 t) noexcept;

		/*----------------------------------------------------------------------
		*  @brief      �O�p�`�̏d�S�d�݈ʒu���Z�o
		*              (f>=0 && g>=0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�͎O�p�`�� Position0>Position1>Position2 �̓����ɂ���܂�>�B
					   (f==0 && g>=0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�� Position0>Position2 �s�ڂɂ���܂�>�B
					   (f>=0 && g==0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�� Position0>Position1> �s�ڂɂ���܂��B
					   (f>=0 && g>=0 && 1-f-g==0) �̏ꍇ�A�|�C���g�� Position1>Position2 �s�ڂɂ���܂�>�B
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION BaryCentric(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, const double f, const double g) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION BaryCentricV(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, ConstVector256 f, ConstVector256 g) noexcept;
		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�ɂ��čŏ��l�ƂȂ����I�����܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Min(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�ɂ��čő�l�ƂȂ����I�����܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Max(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �ł��߂������Ɋۂ߂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Round(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �؂�̂� (�w�肵�����l�̒[����؂�̂Ă������Ɋۂ߂�B���������0�̕����ɂ��鎟�̐���
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Truncate(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̕����Ɋۂ߂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Floor(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �؂�グ ��������Ő��̕����ɂ��鎟�̐����Ɋۂ߂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Ceiling(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �w��͈͓��Ő��l��ݒ肷��
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Clamp(ConstVector256 vector, ConstVector256 min, ConstVector256 max) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 1�ȏ�ɂ͍s���Ȃ��悤�ɂ���
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Saturate(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ��Βl
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Abs(ConstVector256 vector) noexcept;

		#pragma endregion Math

		#pragma region Angle
		/*----------------------------------------------------------------------
		*  @brief : - pi <= theta <= pi�͈̔͂Ɏ��߂�悤�ɂ��܂�.
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION ModAngles(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : - pi <= theta <= pi�͈̔͂Ɏ��߂�悤�ɂ�����ő����Z���s���܂�.
		/*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION AddAngles(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : - pi <= theta <= pi�͈̔͂Ɏ��߂�悤�ɂ�����ň����Z���s���܂�.
		/*----------------------------------------------------------------------*/
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
		**                Protected Member Variables
		*****************************************************************************/

	};

#pragma region Setter
	/****************************************************************************
	*                       Zero
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Zero()noexcept
	*
	*  @brief     �[������������Vector256��Ԃ��֐�
	*
	*  @param[in] void
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Zero() noexcept
	{
		Vector256d result = { { { 0.0f, 0.0f, 0.0f, 0.0f } } };
		return result.V;
	}

	/****************************************************************************
	*                       Set
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Set(const double x, const double y, const double z, const double w) noexcept
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
		Vector256d result = { { { x, y, z, w } } };
		return result.V;
	}

	/****************************************************************************
	*                       Set
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Set(const double value) noexcept
	*
	*  @brief     double1����Vector256�N���X�𐶐�����
	*
	*  @param[in] const double value
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Set(const double value) noexcept
	{
		Vector256d result = { { { value, value, value, value } } };
		return result.V;
	}

	/****************************************************************************
	*                       SetInt
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Set(const double x, const double y, const double z, const double w) noexcept
	*
	*  @brief     int4����Vector256�N���X�𐶐�����
	*
	*  @param[in] const gu::uint32 x
	*  @param[in] const gu::uint32 y
	*  @param[in] const gu::uint32 z
	*  @param[in] const gu::uint32 w
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SetInt(const gu::uint32 x, const gu::uint32 y, const gu::uint32 z, const gu::uint32 w) noexcept
	{
		Vector256d result = { { { x, y, z, w } } };
		return result.V;
	}

	/****************************************************************************
	*                       SetX
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION SetX(ConstVector256 vector, const double x) noexcept
	*
	*  @brief     Double���g����Vector��X�ɒl��������
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] const double x
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SetX(ConstVector256 vector, const double x) noexcept
	{
		Vector256d result = { { { x, vector.D64[1], vector.D64[2], vector.D64[3] } } };
		return result.V;
	}

	/****************************************************************************
	*                       SetY
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION SetY(ConstVector256 vector, const double y) noexcept
	*
	*  @brief     Double���g����Vector��Y�ɒl��������
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] const double y
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SetY(ConstVector256 vector, const double y) noexcept
	{
		Vector256d result = { { { vector.D64[0], y, vector.D64[2], vector.D64[3]}} };
		return result.V;
	}

	/****************************************************************************
	*                       SetZ
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION SetZ(ConstVector256 vector, const double z) noexcept
	*
	*  @brief     Double���g����Vector��Z�ɒl��������
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] const double z
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SetZ(ConstVector256 vector, const double z) noexcept
	{
		Vector256d result = { { { vector.D64[0], vector.D64[1], z, vector.D64[3]}} };
		return result.V;
	}

	/****************************************************************************
	*                       SetW
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION SetW(ConstVector256 vector, const double w) noexcept
	*
	*  @brief     Double���g����Vector��W�ɒl��������
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] const double w
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SetW(ConstVector256 vector, const double w) noexcept
	{
		Vector256d result = { { { vector.D64[0], vector.D64[1], vector.D64[2], w}} };
		return result.V;
	}

	/****************************************************************************
	*                       StoreDouble
	*************************************************************************//**
	*  @fn        inline void SIMD_CALL_CONVENTION Vector256Utility::StoreDouble(double* destination, ConstVector256 source) noexcept
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
		*destination = GetX(source);
	}

	/****************************************************************************
	*                       StoreDouble2
	*************************************************************************//**
	*  @fn        inline void SIMD_CALL_CONVENTION Vector256Utility::StoreDouble2(double* destination, ConstVector256 source) noexcept
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
		destination[0] = source.D64[0];
		destination[1] = source.D64[1];
	}

	/****************************************************************************
	*                       StoreDouble3
	*************************************************************************//**
	*  @fn        inline void SIMD_CALL_CONVENTION Vector256Utility::StoreDouble3(double* destination, ConstVector256 source) noexcept
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
		destination[0] = source.D64[0];
		destination[1] = source.D64[1];
		destination[2] = source.D64[2];
	}

	/****************************************************************************
	*                       StoreDouble4
	*************************************************************************//**
	*  @fn        inline void SIMD_CALL_CONVENTION Vector256Utility::StoreDouble4(double* destination, ConstVector256 source) noexcept
	*
	*  @brief     Vector256�����Ƃ�Double�z��ɑ�����܂�
	*
	*  @param[in] double* destination
	*  @param[in] ConstVector256 source
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline void SIMD_CALL_CONVENTION Vector256Utility::StoreDouble4(double* destination, ConstVector256 source) noexcept
	{
		Check(destination);
		destination[0] = source.D64[0];
		destination[1] = source.D64[1];
		destination[2] = source.D64[2];
		destination[3] = source.D64[3];
	}

	/****************************************************************************
	*                       SplatX
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatX(ConstVector256 vector) noexcept
	*
	*  @brief     X�̗v�f�Ɋ�Â���, �S�Ă̗v�f�ɓ����l��������Vector256�N���X���쐬����
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatX(ConstVector256 vector) noexcept
	{
		Vector256d result = { { { vector.D64[0], vector.D64[0], vector.D64[0],vector.D64[0]}} };
		return result.V;
	}

	/****************************************************************************
	*                       SplatY
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatY(ConstVector256 vector) noexcept
	*
	*  @brief     Y�̗v�f�Ɋ�Â���, �S�Ă̗v�f�ɓ����l��������Vector256�N���X���쐬����
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatY(ConstVector256 vector) noexcept
	{
		Vector256d result = { { { vector.D64[1], vector.D64[1], vector.D64[1],vector.D64[1]} } };
		return result.V;
	}

	/****************************************************************************
	*                       SplatZ
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatZ(ConstVector256 vector) noexcept
	*
	*  @brief     Z�̗v�f�Ɋ�Â���, �S�Ă̗v�f�ɓ����l��������Vector256�N���X���쐬����
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatZ(ConstVector256 vector) noexcept
	{
		Vector256d result = { { { vector.D64[2], vector.D64[2], vector.D64[2],vector.D64[2]} } };
		return result.V;
	}

	/****************************************************************************
	*                       SplatW
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatZ(ConstVector256 vector) noexcept
	*
	*  @brief     W�̗v�f�Ɋ�Â���, �S�Ă̗v�f�ɓ����l��������Vector256�N���X���쐬����
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatW(ConstVector256 vector) noexcept
	{
		Vector256d result = { { { vector.D64[3], vector.D64[3], vector.D64[3],vector.D64[3]} } };
		return result.V;
	}

	/****************************************************************************
	*                       SplatOne
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatOne(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��1��������Vector256�N���X���쐬����
	*
	*  @param[in] void
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatOne() noexcept
	{
		Vector256d result = { { { 1.0f, 1.0f, 1.0f, 1.0f } } };
		return result.V;
	}

	/****************************************************************************
	*                       SplatInfinity
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatInfinity() noexcept
	*
	*  @brief     W�̗v�f�Ɋ�Â���, �S�Ă̗v�f��INF��������Vector256�N���X���쐬����
	*
	*  @param[in] void
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatInfinity() noexcept
	{
		Vector256d result = { { { 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 } } };
		return result.V;
	}

	/****************************************************************************
	*                       SplatEpsilon
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatEpsilon() noexcept
	*
	*  @brief     W�̗v�f�Ɋ�Â���, �S�Ă̗v�f��1.192092896e-7f��������Vector256�N���X���쐬����
	*
	*  @param[in] void
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatEpsilon() noexcept
	{
		Vector256d result = { { { 0x34000000, 0x34000000, 0x34000000, 0x34000000 } } };
		return result.V;
	}

	/****************************************************************************
	*                       SplatSignMask
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatSignMask() noexcept
	*
	*  @brief     W�̗v�f�Ɋ�Â���, �S�Ă̗v�f��-0.0f (0x80000000)��������Vector256�N���X���쐬����
	*
	*  @param[in] void
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SplatSignMask() noexcept
	{
		Vector256d result = { { { 0x80000000U, 0x80000000U, 0x80000000U, 0x80000000U } } };
		return result.V;
	}

	/****************************************************************************
	*                       Swizzle
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Swizzle(ConstVector256 vector,
				  const gu::uint32 xIndex, const gu::uint32 yIndex,
				  const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept
	*
	*  @brief     �x�N�g������ёւ��܂�.index���w�肷��Ƃ���index�ɂ�����Vector4�^���擾
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Swizzle(ConstVector256 vector,
		const gu::uint32 xIndex, const gu::uint32 yIndex,
		const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept
	{
		Check((xIndex < 4) && (yIndex < 4) && (zIndex < 4) && (wIndex < 4));

		Vector256d result = { { { vector.D64[xIndex], vector.D64[yIndex], vector.D64[zIndex], vector.D64[wIndex]}}};
		return result.V;
	}

	/****************************************************************************
	*                       Permute
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Permute(ConstVector256 a, ConstVector256 b,
				  const gu::uint32 xIndex, const gu::uint32 yIndex,
				  const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept
	*
	*  @brief     2�̃x�N�g������v�f�𒊏o��, �V�����x�N�g�����쐬����
	*             0�`3 : a�����x, y, z, w
	*             4�`7 : b�����x, y, z, w
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Permute(ConstVector256 a, ConstVector256 b,
		const gu::uint32 xIndex, const gu::uint32 yIndex,
		const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept
	{
		Check((xIndex < 7) && (yIndex < 7) && (zIndex < 7) && (wIndex < 7));

		const gu::uint32* pointer[2] = {};
		pointer[0] = reinterpret_cast<const gu::uint32*>(&a);
		pointer[1] = reinterpret_cast<const gu::uint32* > (&b);

		Vector256 result = {};
		auto pWork = reinterpret_cast<gu::uint32*>(&result);

		const gu::uint32 i0 = xIndex & 3;
		const gu::uint32 vi0 = xIndex >> 2;
		pWork[0] = pointer[vi0][i0];

		const gu::uint32 i1  = yIndex & 3;
		const gu::uint32 vi1 = yIndex >> 2;
		pWork[1] = pointer[vi1][i1];

		const gu::uint32 i2  = zIndex & 3;
		const gu::uint32 vi2 = zIndex >> 2;
		pWork[2] = pointer[vi2][i2];

		const gu::uint32 i3  = wIndex & 3;
		const gu::uint32 vi3 = wIndex >> 2;
		pWork[3] = pointer[vi3][i3];

		return result;
	}

	/****************************************************************************
	*                       Select
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Select(ConstVector256 left, ConstVector256 right, ConstVector256 control) noexcept
	*
	*  @brief     2�̃x�N�g������v�f�𒊏o��, �V�����x�N�g�����쐬����
	*             control vector��0�̏ꍇ��left����, ��0�̏ꍇ��right���炻�ꂼ�ꒊ�o���s���܂�
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Select(ConstVector256 left, ConstVector256 right, ConstVector256 control) noexcept
	{
		Vector256d result = { { { 
				(left.U64[0] & ~control.U64[0]) | (right.U64[0] & ~control.U64[0]),
				(left.U64[1] & ~control.U64[1]) | (right.U64[1] & ~control.U64[1]),
				(left.U64[2] & ~control.U64[2]) | (right.U64[2] & ~control.U64[2]),
				(left.U64[3] & ~control.U64[3]) | (right.U64[3] & ~control.U64[3]),
			} } };
		return result.V;
	}
	#pragma endregion Setter

	#pragma region Getter
	/****************************************************************************
	*                       GetX
	*************************************************************************//**
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::GetX(ConstVector256 vector) noexcept
	*
	*  @brief     FPU���W�X�^�Ɋi�[���ꂽX�̗v�f�����o��
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::GetX(ConstVector256 vector) noexcept
	{
		return vector.D64[0];
	}

	/****************************************************************************
	*                       GetY
	*************************************************************************//**
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::GetY(ConstVector256 vector) noexcept
	*
	*  @brief     FPU���W�X�^�Ɋi�[���ꂽY�̗v�f�����o��
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::GetY(ConstVector256 vector) noexcept
	{
		return vector.D64[1];
	}

	/****************************************************************************
	*                       GetZ
	*************************************************************************//**
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::GetZ(ConstVector256 vector) noexcept
	*
	*  @brief     FPU���W�X�^�Ɋi�[���ꂽZ�̗v�f�����o��
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::GetZ(ConstVector256 vector) noexcept
	{
		return vector.D64[2];
	}

	/****************************************************************************
	*                       GetW
	*************************************************************************//**
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::GetZ(ConstVector256 vector) noexcept
	*
	*  @brief     FPU���W�X�^�Ɋi�[���ꂽW�̗v�f�����o��
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::GetW(ConstVector256 vector) noexcept
	{
		return vector.D64[3];
	}

	/****************************************************************************
	*                       LoadDouble
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble(const double* source) noexcept
	*
	*  @brief     Double�̔z����g���Ċi�[���� (*source, 0.0, 0.0, 0.0f)
	*
	*  @param[in] const double* source
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble(const double* source) noexcept
	{
		Check(source);
		Vector256d result = { { { source[0], 0.0f, 0.0f, 0.0f } } };
		return result.V;
	}

	/****************************************************************************
	*                       LoadDouble2
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble2(const double* source) noexcept
	*
	*  @brief     Double�̔z����g���Ċi�[���� (source->x, source->y, 0.0, 0.0f)
	*
	*  @param[in] const double* source
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble2(const double* source) noexcept
	{
		Check(source);
		Vector256d result = { { { source[0], source[1], 0.0f, 0.0f}}};
		return result.V;
	}

	/****************************************************************************
	*                       LoadDouble3
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble3(const double* source) noexcept
	*
	*  @brief     Double�̔z����g���Ċi�[���� (source->x, source->y, source->z, 0.0f)
	*
	*  @param[in] const double* source
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble3(const double* source) noexcept
	{
		Check(source);
		Vector256d result = { { { source[0], source[1], source[2], 0.0f}}};
		return result.V;
	}

	/****************************************************************************
	*                       LoadDouble3
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble3(const double* source) noexcept
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
		Vector256d result = { { { source[0], source[1], source[2], source[3]}}};
		return result.V;
	}

	#pragma endregion Getter

	#pragma region Operator
	/****************************************************************************
	*                       Add
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Add(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256d result =
		{ { {
			left.D64[0] + right.D64[0],
			left.D64[1] + right.D64[1],
			left.D64[2] + right.D64[2],
			left.D64[3] + right.D64[3]
		} } };
		return result.V;
	}

	/****************************************************************************
	*                       Subtract
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Subtract(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256d result =
		{ { {
			left.D64[0] - right.D64[0],
			left.D64[1] - right.D64[1],
			left.D64[2] - right.D64[2],
			left.D64[3] - right.D64[3]
		} } };
		return result.V;
	}

	/****************************************************************************
	*                       Multiply
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Multiply(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256d result =
		{ { {
			left.D64[0] * right.D64[0],
			left.D64[1] * right.D64[1],
			left.D64[2] * right.D64[2],
			left.D64[3] * right.D64[3]
		} } };
		return result.V;
	}

	/****************************************************************************
	*                       Divide
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Divide(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256d result =
		{ { {
			left.D64[0] / right.D64[0],
			left.D64[1] / right.D64[1],
			left.D64[2] / right.D64[2],
			left.D64[3] / right.D64[3]
		} } };
		return result.V;
	}

	/****************************************************************************
	*                       Mod
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Mod(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 quotient = Divide(left, right);
		quotient = Truncate(quotient);

		return NegativeMultiplySubtract(right, quotient, left); // left - result * right
	}

	/****************************************************************************
	*                      NegativeMultiplySubstract
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NegativeMultiplySubtract(ConstVector256 first, ConstVector256 second, ConstVector256 third)
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
		Vector256d result =
		{ { {
			subtractLeft.D64[0] - (multiplyLeft.D64[0] * multiplyRight.D64[0]),
			subtractLeft.D64[1] - (multiplyLeft.D64[1] * multiplyRight.D64[1]),
			subtractLeft.D64[2] - (multiplyLeft.D64[2] * multiplyRight.D64[2]),
			subtractLeft.D64[3] - (multiplyLeft.D64[3] * multiplyRight.D64[3])
		} } };
		return result.V;
	}

	/****************************************************************************
	*                      MultiplyAdd
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::MultiplyAdd(ConstVector256 first, ConstVector256 second, ConstVector256 third)
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
		Vector256d result =
		{ { {
			add.D64[0] + (multiplyLeft.D64[0] * multiplyRight.D64[0]),
			add.D64[1] + (multiplyLeft.D64[1] * multiplyRight.D64[1]),
			add.D64[2] + (multiplyLeft.D64[2] * multiplyRight.D64[2]),
			add.D64[3] + (multiplyLeft.D64[3] * multiplyRight.D64[3])
		} } };
		return result.V;
	}

	/****************************************************************************
	*                       Reciprocal
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Reciprocal(ConstVector256 vector) noexcept
	*
	*  @brief     2�̃x�N�g���̊e�v�f���m�̋t�����Z�o���܂�. (1.0f / vector)
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Reciprocal(ConstVector256 vector) noexcept
	{
		Vector256d result =
		{ { {
			1.0f / vector.D64[0],
			1.0f / vector.D64[1],
			1.0f / vector.D64[2],
			1.0f / vector.D64[3],
		} } };
		return result.V;
	}

	/****************************************************************************
	*                       Scale
	*************************************************************************//**
	*  @fn       inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Scale(ConstVector256 vector, const double scale) noexcept
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
		Vector256d result =
		{ { {
			vector.D64[0] * scale,
			vector.D64[1] * scale,
			vector.D64[2] * scale,
			vector.D64[3] * scale
		} } };
		return result.V;
	}

	/****************************************************************************
	*                       EqualVector2
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVector2(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.D64[0] == right.D64[0]) && (left.D64[1] == right.D64[1]);
	}

	/****************************************************************************
	*                       EqualVector3
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVector3(ConstVector256 left, ConstVector256 right) noexcept
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
		return(left.D64[0] == right.D64[0]) && (left.D64[1] == right.D64[1]) && (left.D64[2] == right.D64[2]);
	}

	/****************************************************************************
	*                       EqualVector4
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVector4(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.D64[0] == right.D64[0]) && (left.D64[1] == right.D64[1]) && (left.D64[2] == right.D64[2]) && (left.D64[3] == right.D64[3]);
	}

	/****************************************************************************
	*                       EqualVectorEach
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256u result = { { {
				(left.D64[0] == right.D64[0]) ? 0xFFFFFFFF : 0,
				(left.D64[1] == right.D64[1]) ? 0xFFFFFFFF : 0,
				(left.D64[2] == right.D64[2]) ? 0xFFFFFFFF : 0,
				(left.D64[3] == right.D64[3]) ? 0xFFFFFFFF : 0
			} } };
		return result.V;
	}

	/****************************************************************************
	*                       NotEqualVector2
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector2(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.D64[0] != right.D64[0]) || (left.D64[1] != right.D64[1]);
	}

	/****************************************************************************
	*                       NotEqualVector3
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector3(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.D64[0] != right.D64[0]) || (left.D64[1] != right.D64[1]) || (left.D64[2] != right.D64[2]);
	}

	/****************************************************************************
	*                       NotEqualVector4
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.D64[0] != right.D64[0]) || (left.D64[1] != right.D64[1]) || (left.D64[2] != right.D64[2]) || (left.D64[3] != right.D64[3]);
	}

	/****************************************************************************
	*                       NotEqualVectorEach
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256u result = { { {
				(left.D64[0] != right.D64[0]) ? 0xFFFFFFFF : 0,
				(left.D64[1] != right.D64[1]) ? 0xFFFFFFFF : 0,
				(left.D64[2] != right.D64[2]) ? 0xFFFFFFFF : 0,
				(left.D64[3] != right.D64[3]) ? 0xFFFFFFFF : 0
			} } };
		return result.V;
	}

	/****************************************************************************
	*                       EqualAsIntVector2
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualAsIntVector2(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     ��̃x�N�g�������������ǂ����𒲂ׂ܂�.
	*             ��r���ɂ͕����Ȃ�����(U64)�Ƃ��Ĉ����܂�.
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualAsIntVector2(ConstVector256 left, ConstVector256 right) noexcept
	{
		return (left.U64[0] == right.U64[0]) && (left.U64[1] == right.U64[1]);
	}

	/****************************************************************************
	*                       EqualAsIntVector3
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualAsIntVector3(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.U64[0] == right.U64[0]) && (left.U64[1] == right.U64[1]) && (left.U64[2] == right.U64[2]);
	}

	/****************************************************************************
	*                       EqualAsIntVector4
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualAsIntVector4(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.U64[0] == right.U64[0]) && (left.U64[1] == right.U64[1]) && (left.U64[2] == right.U64[2]) && (left.U64[3] == right.U64[3]);
	}

	/****************************************************************************
	*                       EqualAsIntVectorEach
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualAsIntVectorEach(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256u result = { { {
				(left.U64[0] == right.U64[0]) ? 0xFFFFFFFF : 0,
				(left.U64[1] == right.U64[1]) ? 0xFFFFFFFF : 0,
				(left.U64[2] == right.U64[2]) ? 0xFFFFFFFF : 0,
				(left.U64[3] == right.U64[3]) ? 0xFFFFFFFF : 0
			} } };
		return result.V;
	}

	/****************************************************************************
	*                       NotEqualVector2
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualAsIntVector2(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     ��̃x�N�g������ł��������Ȃ����ǂ����𒲂ׂ܂�.
	*            ��r���ɂ͕����Ȃ�����(U64)�Ƃ��Ĕ�r���܂�.
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualAsIntVector2(ConstVector256 left, ConstVector256 right) noexcept
	{
		return (left.U64[0] != right.U64[0]) || (left.U64[1] != right.U64[1]);
	}

	/****************************************************************************
	*                       NotEqualVector3
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualAsIntVector3(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.U64[0] != right.U64[0]) || (left.U64[1] != right.U64[1]) || (left.U64[2] != right.U64[2]);
	}

	/****************************************************************************
	*                       NotEqualVector4
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.U64[0] != right.U64[0]) || (left.U64[1] != right.U64[1]) || (left.U64[2] != right.U64[2]) || (left.U64[3] != right.U64[3]);
	}

	/****************************************************************************
	*                       NotEqualVector
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256u result = { { {
				(left.U64[0] != right.U64[0]) ? 0xFFFFFFFF : 0,
				(left.U64[1] != right.U64[1]) ? 0xFFFFFFFF : 0,
				(left.U64[2] != right.U64[2]) ? 0xFFFFFFFF : 0,
				(left.U64[3] != right.U64[3]) ? 0xFFFFFFFF : 0
			} } };
		return result.V;
	}

	/****************************************************************************
	*                       NearEqualVector2
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector4(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
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
		const double deltaX = fabsf(left.D64[0] - right.D64[0]);
		const double deltaY = fabsf(left.D64[1] - right.D64[1]);
		return ((deltaX <= epsilon.D64[0]) && (deltaY <= epsilon.D64[1]));
	}

	/****************************************************************************
	*                       NearEqualVector3
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector3(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
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
		const double deltaX = fabsf(left.D64[0] - right.D64[0]);
		const double deltaY = fabsf(left.D64[1] - right.D64[1]);
		const double deltaZ = fabsf(left.D64[2] - right.D64[2]);
		return ((deltaX <= epsilon.D64[0]) && (deltaY <= epsilon.D64[1]) && (deltaZ <= epsilon.D64[2]));
	}

	/****************************************************************************
	*                       NearEqualVector4
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector4(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
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
		const double deltaX = fabsf(left.D64[0] - right.D64[0]);
		const double deltaY = fabsf(left.D64[1] - right.D64[1]);
		const double deltaZ = fabsf(left.D64[2] - right.D64[2]);
		const double deltaW = fabsf(left.D64[3] - right.D64[3]);
		return ((deltaX <= epsilon.D64[0]) && (deltaY <= epsilon.D64[1]) && (deltaZ <= epsilon.D64[2]) && (deltaW <= epsilon.D64[3]));
	}

	/****************************************************************************
	*                       NearEqualVectorEach
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVectorEach(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
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
		const double deltaX = fabsf(left.D64[0] - right.D64[0]);
		const double deltaY = fabsf(left.D64[1] - right.D64[1]);
		const double deltaZ = fabsf(left.D64[2] - right.D64[2]);
		const double deltaW = fabsf(left.D64[3] - right.D64[3]);

		Vector256u control = { { {
				deltaX <= epsilon.D64[0] ? 0xFFFFFFFFU : 0,
				deltaY <= epsilon.D64[1] ? 0xFFFFFFFFU : 0,
				deltaZ <= epsilon.D64[2] ? 0xFFFFFFFFU : 0,
				deltaW <= epsilon.D64[3] ? 0xFFFFFFFFU : 0
			} } };

		return control.V;
	}

	/****************************************************************************
	*                       GreaterVector2
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterVector2(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.D64[0] > right.D64[0]) && (left.D64[1] > right.D64[1]);
	}

	/****************************************************************************
	*                       GreaterVector3
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterVector3(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.D64[0] > right.D64[0]) && (left.D64[1] > right.D64[1]) && (left.D64[2] > right.D64[2]);
	}

	/****************************************************************************
	*                       GreaterVector4
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterVector4(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.D64[0] > right.D64[0]) && (left.D64[1] > right.D64[1]) && (left.D64[2] > right.D64[2]) && (left.D64[3] > right.D64[3]);
	}

	/****************************************************************************
	*                       GreaterVectorEach
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::GreaterVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector4�^�ɂ����č��̕����S�Ă̗v�f�ɂ����đ傫�����𒲂ׂ܂� (left > right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::GreaterVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	{
		Vector256u control = { { {
				left.D64[0] > right.D64[0] ? 0xFFFFFFFFU : 0,
				left.D64[1] > right.D64[1] ? 0xFFFFFFFFU : 0,
				left.D64[2] > right.D64[2] ? 0xFFFFFFFFU : 0,
				left.D64[3] > right.D64[3] ? 0xFFFFFFFFU : 0
			} } };

		return control.V;
	}


	/****************************************************************************
	*                       GreaterOrEqualVector2
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVector2(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.D64[0] >= right.D64[0]) && (left.D64[1] >= right.D64[1]);
	}

	/****************************************************************************
	*                       GreaterVector3
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVector3(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.D64[0] >= right.D64[0]) && (left.D64[1] >= right.D64[1]) && (left.D64[2] >= right.D64[2]);
	}

	/****************************************************************************
	*                       GreaterOrEqualVector4
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.D64[0] >= right.D64[0]) && (left.D64[1] >= right.D64[1]) && (left.D64[2] >= right.D64[2]) && (left.D64[3] >= right.D64[3]);
	}

	/****************************************************************************
	*                       GreaterOrEqualVectorEach
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector4�^�ɂ����č��̕����S�Ă̗v�f�ɂ����đ傫��or���������𒲂ׂ܂� (left >= right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	{
		Vector256u control = { { {
				left.D64[0] >= right.D64[0] ? 0xFFFFFFFFU : 0,
				left.D64[1] >= right.D64[1] ? 0xFFFFFFFFU : 0,
				left.D64[2] >= right.D64[2] ? 0xFFFFFFFFU : 0,
				left.D64[3] >= right.D64[3] ? 0xFFFFFFFFU : 0
			} } };

		return control.V;
	}


	/****************************************************************************
	*                       LessVector2
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessVector2(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.D64[0] < right.D64[0]) && (left.D64[1] < right.D64[1]);
	}

	/****************************************************************************
	*                       LessVector3
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessVector3(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.D64[0] < right.D64[0]) && (left.D64[1] < right.D64[1]) && (left.D64[2] < right.D64[2]);
	}

	/****************************************************************************
	*                       LessVector4
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessVector4(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.D64[0] < right.D64[0]) && (left.D64[1] < right.D64[1]) && (left.D64[2] < right.D64[2]) && (left.D64[3] < right.D64[3]);
	}

	/****************************************************************************
	*                       LessVectorEach
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector4�^�ɂ����č��̕����S�Ă̗v�f�ɂ����ď�����or���������𒲂ׂ܂� (left <= right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LessVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	{
		Vector256u control = { { {
				left.D64[0] < right.D64[0] ? 0xFFFFFFFFU : 0,
				left.D64[1] < right.D64[1] ? 0xFFFFFFFFU : 0,
				left.D64[2] < right.D64[2] ? 0xFFFFFFFFU : 0,
				left.D64[3] < right.D64[3] ? 0xFFFFFFFFU : 0
			} } };

		return control.V;
	}


	/****************************************************************************
	*                       LessOrEqualVector2
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVector2(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.D64[0] <= right.D64[0]) && (left.D64[1] <= right.D64[1]);
	}

	/****************************************************************************
	*                       LessOrEqualVector3
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVector3(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.D64[0] <= right.D64[0]) && (left.D64[1] <= right.D64[1]) && (left.D64[2] <= right.D64[2]);
	}

	/****************************************************************************
	*                       LessOrEqualVector4
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
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
		return (left.D64[0] <= right.D64[0]) && (left.D64[1] <= right.D64[1]) && (left.D64[2] <= right.D64[2]) && (left.D64[3] <= right.D64[3]);
	}

	/****************************************************************************
	*                       LessVectorEach
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256u control = { { {
				left.D64[0] <= right.D64[0] ? 0xFFFFFFFFU : 0,
				left.D64[1] <= right.D64[1] ? 0xFFFFFFFFU : 0,
				left.D64[2] <= right.D64[2] ? 0xFFFFFFFFU : 0,
				left.D64[3] <= right.D64[3] ? 0xFFFFFFFFU : 0
			} } };

		return control.V;
	}

	/****************************************************************************
	*                       InBoundsVector2
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::InBoundsVector2(ConstVector256 vector, ConstVector256 bounds) noexcept
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
		return (
			(vector.D64[0] <= bounds.D64[0] && vector.D64[0] >= -bounds.D64[0]) &&
			(vector.D64[1] <= bounds.D64[1] && vector.D64[1] >= -bounds.D64[1]));
	}

	/****************************************************************************
	*                       InBoundsVector3
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::InBoundsVector3(ConstVector256 vector, ConstVector256 bounds) noexcept
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
		return (
			(vector.D64[0] <= bounds.D64[0] && vector.D64[0] >= -bounds.D64[0]) &&
			(vector.D64[1] <= bounds.D64[1] && vector.D64[1] >= -bounds.D64[1]) &&
			(vector.D64[2] <= bounds.D64[2] && vector.D64[2] >= -bounds.D64[2]));

	}

	/****************************************************************************
	*                       InBoundsVector4
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::InBoundsVector4(ConstVector256 vector, ConstVector256 bounds) noexcept
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
		return (
			(vector.D64[0] <= bounds.D64[0] && vector.D64[0] >= -bounds.D64[0]) &&
			(vector.D64[1] <= bounds.D64[1] && vector.D64[1] >= -bounds.D64[1]) &&
			(vector.D64[2] <= bounds.D64[2] && vector.D64[2] >= -bounds.D64[2]) &&
			(vector.D64[3] <= bounds.D64[3] && vector.D64[3] >= -bounds.D64[3]));
	}

	/****************************************************************************
	*                       InBoundsVectorEach
	*************************************************************************//**
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::InBoundsVectorEach(ConstVector256 vector, ConstVector256 bounds) noexcept
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
		Vector256u control = { { {
			(vector.D64[0] <= bounds.D64[0] && vector.D64[0] >= -bounds.D64[0]) ? 0xFFFFFFFF : 0,
			(vector.D64[1] <= bounds.D64[1] && vector.D64[1] >= -bounds.D64[1]) ? 0xFFFFFFFF : 0,
			(vector.D64[2] <= bounds.D64[2] && vector.D64[2] >= -bounds.D64[2]) ? 0xFFFFFFFF : 0,
			(vector.D64[3] <= bounds.D64[3] && vector.D64[3] >= -bounds.D64[3]) ? 0xFFFFFFFF : 0,
		} } };
		return control.V;
	}

	#pragma endregion Operator

	#pragma region Bit
	/****************************************************************************
	*                       AndInt
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::AndInt(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     �r�b�g�P�ʂ̘_���ς̉��Z���s���܂�
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::AndInt(ConstVector256 left, ConstVector256 right) noexcept
	{
		Vector256u result = { { {
		   left.U64[0] & right.U64[0],
		   left.U64[1] & right.U64[1],
		   left.U64[2] & right.U64[2],
		   left.U64[3] & right.U64[3]
	   } } };
		return result.V;
	}

	/****************************************************************************
	*                        OrInt
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::AndInt(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     �r�b�g�P�ʂ̘_���ς̉��Z���s���܂�
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::OrInt(ConstVector256 left, ConstVector256 right) noexcept
	{
		Vector256u result = { { {
		   left.U64[0] | right.U64[0],
		   left.U64[1] | right.U64[1],
		   left.U64[2] | right.U64[2],
		   left.U64[3] | right.U64[3]
	   } } };
		return result.V;
	}

	/****************************************************************************
	*                        NorInt
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NorInt(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     �r�b�g�P�ʂ̘_���ϔے�̉��Z���s���܂�
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NorInt(ConstVector256 left, ConstVector256 right) noexcept
	{
		Vector256u result = { { {
		   ~(left.U64[0] | right.U64[0]),
		   ~(left.U64[1] & right.U64[1]),
		   ~(left.U64[2] & right.U64[2]),
		   ~(left.U64[3] & right.U64[3])
	   } } };
		return result.V;
	}

	/****************************************************************************
	*                        XorInt
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::AndInt(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     �r�b�g�P�ʂ̔r���I�_���a�̉��Z���s���܂�
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::XorInt(ConstVector256 left, ConstVector256 right) noexcept
	{
		Vector256u result = { { {
		   left.U64[0] ^ right.U64[0],
		   left.U64[1] ^ right.U64[1],
		   left.U64[2] ^ right.U64[2],
		   left.U64[3] ^ right.U64[3]
	   } } };
		return result.V;
	}

	#pragma endregion Bit

	#pragma region Math
	/****************************************************************************
	*                       Negate
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Negate(ConstVector256 vector) noexcept
	*
	*  @brief     Vector�̑S�Ă̗v�f�̕����𔻒肵�܂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Negate(ConstVector256 vector) noexcept
	{
		Vector256d Result = { { {
			-vector.D64[0],
			-vector.D64[1],
			-vector.D64[2],
			-vector.D64[3],
		} } };
		return Result.V;
	}

	/****************************************************************************
	*                      NormVector2
	*************************************************************************//**
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::NormVector2(ConstVector256 vector) noexcept
	*
	*  @brief     1���m����(�e�v�f�̐�Βl�̘a�ɂ�����x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::NormVector2(ConstVector256 vector) noexcept
	{
		return sqrtf(NormSquaredVector2(vector));
	}

	/****************************************************************************
	*                      NormVector3
	*************************************************************************//**
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::NormVector3(ConstVector256 vector) noexcept
	*
	*  @brief     1���m����(�e�v�f�̐�Βl�̘a�ɂ�����x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::NormVector3(ConstVector256 vector) noexcept
	{
		return sqrtf(NormSquaredVector3(vector));
	}

	/****************************************************************************
	*                      NormVector4
	*************************************************************************//**
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::NormVector4(ConstVector256 vector) noexcept
	*
	*  @brief     1���m����(�e�v�f�̐�Βl�̘a�ɂ�����x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::NormVector4(ConstVector256 vector) noexcept
	{
		return sqrtf(NormSquaredVector4(vector));
	}

	/****************************************************************************
	*                      NormSquaredVector2
	*************************************************************************//**
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::NormSquaredVector2(ConstVector256 vector) noexcept
	*
	*  @brief     2���m����(�e�v�f��2��a�����[�g��������Ƃ��̃x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::NormSquaredVector2(ConstVector256 vector) noexcept
	{
		return DotVector2(vector, vector);
	}

	/****************************************************************************
	*                      NormSquaredVector3
	*************************************************************************//**
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::NormSquaredVector3(ConstVector256 vector) noexcept
	*
	*  @brief     2���m����(�e�v�f��2��a�����[�g��������Ƃ��̃x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::NormSquaredVector3(ConstVector256 vector) noexcept
	{
		return DotVector3(vector, vector);
	}

	/****************************************************************************
	*                      NormSquaredVector4
	*************************************************************************//**
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::NormSquaredVector4(ConstVector256 vector) noexcept
	*
	*  @brief     2���m����(�e�v�f��2��a�����[�g��������Ƃ��̃x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::NormSquaredVector4(ConstVector256 vector) noexcept
	{
		return DotVector4(vector, vector);
	}

	/****************************************************************************
	*                      DotVector2
	*************************************************************************//**
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::DotVector2(ConstVector256 left, ConstVector256 right)
	*
	*  @brief     ���ς��Z�o
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::DotVector2(ConstVector256 left, ConstVector256 right)
	{
		return left.D64[0] * right.D64[0] + left.D64[1] * right.D64[1];
	}

	/****************************************************************************
	*                      DotVector3
	*************************************************************************//**
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::DotVector3(ConstVector256 left, ConstVector256 right)
	*
	*  @brief     ���ς��Z�o
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::DotVector3(ConstVector256 left, ConstVector256 right)
	{
		return left.D64[0] * right.D64[0] + left.D64[1] * right.D64[1] + left.D64[2] * right.D64[2];
	}

	/****************************************************************************
	*                      DotVector4
	*************************************************************************//**
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::DotVector4(ConstVector256 left, ConstVector256 right)
	*
	*  @brief     ���ς��Z�o
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::DotVector4(ConstVector256 left, ConstVector256 right)
	{
		return left.D64[0] * right.D64[0] + left.D64[1] * right.D64[1] + left.D64[2] * right.D64[2] + left.D64[3] * right.D64[3];
	}

	/****************************************************************************
	*                      CrossVector2
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CrossVector2(ConstVector256 left, ConstVector256 right)
	*
	*  @brief     ���s�l�ӌ`�̖ʐς��Z�o����
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return �@�@Vector256�ł͂��邪, ����double
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CrossVector2(ConstVector256 left, ConstVector256 right)
	{
		// [ left.x*right.y - left.y*right.x, left.x*right.y - left.y*right.x ]
		return Set((left.D64[0] * right.D64[1]) - (left.D64[1] * right.D64[0]));
	}

	/****************************************************************************
	*                      CrossVector3
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CrossVector3(ConstVector256 left, ConstVector256 right) noexcept
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
		
		Vector256d result = { { {
			(left.D64[1] * right.D64[2]) - (left.D64[2] * right.D64[1]),
			(left.D64[2] * right.D64[0]) - (left.D64[0] * right.D64[2]),
			(left.D64[0] * right.D64[1]) - (left.D64[1] * right.D64[0]),
			0.0f
		} } };
		return result.V;
	}

	/****************************************************************************
	*                      CrossVector4
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CrossVector4(ConstVector256 left, ConstVector256 right) noexcept
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

		Vector256d result = { { {
			(((second.D64[2] * third.D64[3]) - (second.D64[3] * third.D64[2])) * first.D64[1]) - (((second.D64[1] * third.D64[3]) - (second.D64[3] * third.D64[1])) * first.D64[2]) + (((second.D64[1] * third.D64[2]) - (second.D64[2] * third.D64[1])) * first.D64[3]),
			(((second.D64[3] * third.D64[2]) - (second.D64[2] * third.D64[3])) * first.D64[0]) - (((second.D64[3] * third.D64[0]) - (second.D64[0] * third.D64[3])) * first.D64[2]) + (((second.D64[2] * third.D64[0]) - (second.D64[0] * third.D64[2])) * first.D64[3]),
			(((second.D64[1] * third.D64[3]) - (second.D64[3] * third.D64[1])) * first.D64[0]) - (((second.D64[0] * third.D64[3]) - (second.D64[3] * third.D64[0])) * first.D64[1]) + (((second.D64[0] * third.D64[1]) - (second.D64[1] * third.D64[0])) * first.D64[3]),
			(((second.D64[2] * third.D64[1]) - (second.D64[1] * third.D64[2])) * first.D64[0]) - (((second.D64[2] * third.D64[0]) - (second.D64[0] * third.D64[2])) * first.D64[1]) + (((second.D64[1] * third.D64[0]) - (second.D64[0] * third.D64[1])) * first.D64[2]),
		} } };
		return result.V;
	}

	/****************************************************************************
	*                      NormalizeVector2
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NormalizeVector2(ConstVector256 vector) noexcept
	*
	*  @brief     ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NormalizeVector2(ConstVector256 vector) noexcept
	{
		const double norm = NormVector2(vector);
		const double reciprocalNorm = norm > 0 ? 1.0f / norm : 0.0f;

		Vector256d result = { { {
				vector.D64[0] * reciprocalNorm,
				vector.D64[1] * reciprocalNorm,
				vector.D64[2] * reciprocalNorm,
				vector.D64[3] * reciprocalNorm
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      NormalizeVector3
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NormalizeVector3(ConstVector256 vector) noexcept
	*
	*  @brief     ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NormalizeVector3(ConstVector256 vector) noexcept
	{
		const double norm = NormVector3(vector);
		const double reciprocalNorm = norm > 0 ? 1.0f / norm : 0.0f;

		Vector256d result = { { {
				vector.D64[0] * reciprocalNorm,
				vector.D64[1] * reciprocalNorm,
				vector.D64[2] * reciprocalNorm,
				vector.D64[3] * reciprocalNorm
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      NormalizeVector4
	*************************************************************************//** 
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NormalizeVector4(ConstVector256 vector) noexcept
	*
	*  @brief     ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NormalizeVector4(ConstVector256 vector) noexcept
	{
		const double norm = NormVector4(vector);
		const double reciprocalNorm = norm > 0 ? 1.0f / norm : 0.0f;

		Vector256d result = { { {
				vector.D64[0] * reciprocalNorm,
				vector.D64[1] * reciprocalNorm,
				vector.D64[2] * reciprocalNorm,
				vector.D64[3] * reciprocalNorm
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      ReflectVector2
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ReflectVector2(ConstVector256 input, ConstVector256 normal) noexcept
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
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ReflectVector4(ConstVector256 input, ConstVector256 normal) noexcept
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
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ReflectVector4(ConstVector256 input, ConstVector256 normal)
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
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector2(ConstVector256 input, ConstVector256 normal, const double refractionIndex)
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
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector3(ConstVector256 input, ConstVector256 normal, const double refractionIndex)
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
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector4(ConstVector256 input, ConstVector256 normal, const double refractionIndex)
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
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector2(ConstVector256 input, ConstVector256 normal, const double refractionIndex)
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

		const double inputDotNormal = (input.D64[0] * normal.D64[0]) + (input.D64[1] * normal.D64[1]);
		// R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
		double refractionY = 1.0f - (inputDotNormal * inputDotNormal);
		double refractionX = 1.0f - (refractionY * refractionIndex.D64[0] * refractionIndex.D64[0]);
		refractionY = 1.0f - (refractionY * refractionIndex.D64[1] * refractionIndex.D64[1]);
		if (refractionX >= 0.0f)
		{
			refractionX = (refractionIndex.D64[0] * input.D64[0]) - (normal.D64[0] * ((refractionIndex.D64[0] * inputDotNormal) + sqrtf(refractionX)));
		}
		else
		{
			refractionX = 0.0f;
		}
		if (refractionY >= 0.0f)
		{
			refractionY = (refractionIndex.D64[1] * input.D64[1]) - (normal.D64[1] * ((refractionIndex.D64[1] * inputDotNormal) + sqrtf(refractionY)));
		}
		else
		{
			refractionY = 0.0f;
		}

		Vector256 result = {};
		result.D64[0] = refractionX;
		result.D64[1] = refractionY;
		result.D64[2] = 0.0f;
		result.D64[3] = 0.0f;
		return result;
	}

	/****************************************************************************
	*                      RefractVector3
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector3(ConstVector256 input, ConstVector256 normal, const double refractionIndex)
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
		Vector256 r  = NegativeMultiplySubtract(inputDotNormal, inputDotNormal, VECTOR_256D_ONE.V);
		r = Multiply(r, refractionIndex);
		r = NegativeMultiplySubtract(r, refractionIndex, VECTOR_256D_ONE.V);

		Vector256 result = {};
		if (LessOrEqualVector3(r, VECTOR_256D_ZERO.V))
		{
			// Total internal reflection
			result = VECTOR_256D_ZERO;
		}
		else
		{
			// r = refractionIndex * inputDotNormal + sqrt(r)
			r = Sqrt(r);
			r = MultiplyAdd(refractionIndex, inputDotNormal, r);

			// result = refractionIndex * input - normal * r
			result = Multiply(refractionIndex, input);
			result = NegativeMultiplySubtract(normal, r, result);
		}

		return result;
	}

	/****************************************************************************
	*                      RefractVector4
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector4(ConstVector256 input, ConstVector256 normal, const double refractionIndex)
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
		Vector256 r  = NegativeMultiplySubtract(inputDotNormal, inputDotNormal, VECTOR_256D_ONE.V);
		r = Multiply(r, refractionIndex);
		r = NegativeMultiplySubtract(r, refractionIndex, VECTOR_256D_ONE.V);

		Vector256 result = {};

		if (LessOrEqualVector4(r, VECTOR_256D_ZERO))
		{
			// Total internal reflection
			result = VECTOR_256D_ZERO;
		}
		else
		{
			// r = refractionIndex * inputDotNormal + sqrt(r)
			r = Sqrt(r);
			r = MultiplyAdd(refractionIndex, inputDotNormal, r);

			// result = refractionIndex * input - normal * r
			result = Multiply(refractionIndex, input);
			result = NegativeMultiplySubtract(normal, r, result);
		}

		return result;
	}

	///****************************************************************************
	//*                      IsInfiniteVector2
	//*************************************************************************//**
	//*  @fn        inline bool Vector256Utility::IsInfiniteVector2(ConstVector256 vector) noexcept
	//*
	//*  @brief     ���ɑ傫���l��
	//*
	//*  @param[in] ConstVector256 : vector
	//*
	//*  @return �@�@Vector256
	//*****************************************************************************/
	//inline bool Vector256Utility::IsInfiniteVector2(ConstVector256 vector) noexcept
	//{
	//	return IS_INF(vector.D64[0]) || IS_INF(vector.D64[1]);
	//}

	///****************************************************************************
	//*                      IsInfiniteVector3
	//*************************************************************************//**
	//*  @fn        inline bool Vector256Utility::IsInfiniteVector3(ConstVector256 vector) noexcept
	//*
	//*  @brief     ���ɑ傫���l��
	//*
	//*  @param[in] ConstVector256 : vector
	//*
	//*  @return �@�@Vector256
	//*****************************************************************************/
	//inline bool Vector256Utility::IsInfiniteVector3(ConstVector256 vector) noexcept
	//{
	//	return IS_INF(vector.D64[0]) || IS_INF(vector.D64[1]) || IS_INF(vector.D64[2]);
	//}

	///****************************************************************************
	//*                      IsInfiniteVector4
	//*************************************************************************//**
	//*  @fn        inline bool Vector256Utility::IsInfiniteVector4(ConstVector256 vector) noexcept
	//*
	//*  @brief     ���ɑ傫���l��
	//*
	//*  @param[in] ConstVector256 : vector
	//*
	//*  @return �@�@Vector256
	//*****************************************************************************/
	//inline bool Vector256Utility::IsInfiniteVector4(ConstVector256 vector) noexcept
	//{
	//	return IS_INF(vector.D64[0]) || IS_INF(vector.D64[1]) || IS_INF(vector.D64[2]) || IS_INF(vector.D64[3]);
	//}

	/****************************************************************************
	*                      Sqrt
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Sqrt(ConstVector256 vector) noexcept
	*
	*  @brief      �S�Ă̗v�f�ɍ�������������̂�Ԃ��܂�
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Sqrt(ConstVector256 vector) noexcept
	{
		Vector256d result = { { {
				sqrtf(vector.D64[0]),
				sqrtf(vector.D64[1]),
				sqrtf(vector.D64[2]),
				sqrtf(vector.D64[3]),
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      ReciprocalSqrt
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ReciprocalSqrt(ConstVector256 vector) noexcept
	*
	*  @brief       �S�Ă̗v�f�̋t�������, ����ɍ�������������̂�Ԃ��܂�
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ReciprocalSqrt(ConstVector256 vector) noexcept
	{
		Vector256d result = { { {
				1.0f / sqrtf(vector.D64[0]),
				1.0f / sqrtf(vector.D64[1]),
				1.0f / sqrtf(vector.D64[2]),
				1.0f / sqrtf(vector.D64[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      Exp10
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Exp10(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��2^{vector}��Ԃ��܂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Exp2(ConstVector256 vector) noexcept
	{
		Vector256d result = { { {
				powf(2.0f, vector.D64[0]),
				powf(2.0f, vector.D64[1]),
				powf(2.0f, vector.D64[2]),
				powf(2.0f, vector.D64[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      Exp10
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Exp10(ConstVector256 vector) noexcept
	*
	*  @brief      �S�Ă̗v�f��10^{vector}��Ԃ��܂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Exp10(ConstVector256 vector) noexcept
	{
		Vector256d result = { { {
				powf(10.0f, vector.D64[0]),
				powf(10.0f, vector.D64[1]),
				powf(10.0f, vector.D64[2]),
				powf(10.0f, vector.D64[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      ExpE
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ExpE(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��e^{vector}��Ԃ��܂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ExpE(ConstVector256 vector) noexcept
	{
		Vector256d result = { { {
				expf(vector.D64[0]),
				expf(vector.D64[1]),
				expf(vector.D64[2]),
				expf(vector.D64[3])
			} } };

		return result.V;
	}
	/****************************************************************************
	*                      Log2
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Log2(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��log2{vector}��Ԃ��܂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Log2(ConstVector256 vector) noexcept
	{
		const double scale = 1.4426950f; // (1.0f / logf(2.0f));

		Vector256d result = { { {
				logf(vector.D64[0]),
				logf(vector.D64[1]),
				logf(vector.D64[2]),
				logf(vector.D64[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      Log10
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Log10(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��log10{vector}��Ԃ��܂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Log10(ConstVector256 vector) noexcept
	{
		Vector256d result = { { {
				log10f(vector.D64[0]),
				log10f(vector.D64[1]),
				log10f(vector.D64[2]),
				log10f(vector.D64[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      LogE
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LogE(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��loge{vector}��Ԃ��܂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LogE(ConstVector256 vector) noexcept
	{
		Vector256d result = { { {
				logf(vector.D64[0]),
				logf(vector.D64[1]),
				logf(vector.D64[2]),
				logf(vector.D64[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      Pow
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Pow(ConstVector256 base, ConstVector256 power)
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
		Vector256d result = { { {
				powf(base.D64[0], power.D64[0]),
				powf(base.D64[1], power.D64[1]),
				powf(base.D64[2], power.D64[2]),
				powf(base.D64[3], power.D64[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      Sin
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Sin(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��Sin����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Sin(ConstVector256 vector) noexcept
	{
		Vector256d result = { { {
				sinf(vector.D64[0]),
				sinf(vector.D64[1]),
				sinf(vector.D64[2]),
				sinf(vector.D64[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      Cos
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Cos(ConstVector256 vector) noexcepts
	*
	*  @brief     �S�Ă̗v�f��Cos����������̂�Ԃ��܂�
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Cos(ConstVector256 vector) noexcept
	{
		Vector256d result = { { {
				cosf(vector.D64[0]),
				cosf(vector.D64[1]),
				cosf(vector.D64[2]),
				cosf(vector.D64[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      Tan
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Tan(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��Tan����������̂�Ԃ��܂�
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Tan(ConstVector256 vector) noexcept
	{
		Vector256d result = { { {
				tanf(vector.D64[0]),
				tanf(vector.D64[1]),
				tanf(vector.D64[2]),
				tanf(vector.D64[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      SinCos
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SinCos(ConstVector256 input, Vector256* sin, Vector256* cos) noexcept
	*
	*  @brief     �S�Ă̗v�f�ɑ΂���sin������cos���������o���܂� 
	*
	*  @param[in] ConstVector256 : input
	*  @param[out] Vector256* sin
	*  @param[out] Vector256* cos
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline void SIMD_CALL_CONVENTION Vector256Utility::SinCos(ConstVector256 input, Vector256* sin, Vector256* cos) noexcept
	{
		Check(sin != nullptr);
		Check(cos != nullptr);

		Vector256d sinResult = { { {
				sinf(input.D64[0]),
				sinf(input.D64[1]),
				sinf(input.D64[2]),
				sinf(input.D64[3])
			} } };

		Vector256d cosResult = { { {
				cosf(input.D64[0]),
				cosf(input.D64[1]),
				cosf(input.D64[2]),
				cosf(input.D64[3])
			} } };

		*sin = sinResult.V;
		*cos = cosResult.V;
	}

	/****************************************************************************
	*                      SinH
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SinH(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��SinH����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SinH(ConstVector256 vector) noexcept
	{
		Vector256d result = { { {
				sinhf(vector.D64[0]),
				sinhf(vector.D64[1]),
				sinhf(vector.D64[2]),
				sinhf(vector.D64[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      CosH
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CosH(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��CosH����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CosH(ConstVector256 vector) noexcept
	{
		Vector256d result = { { {
				coshf(vector.D64[0]),
				coshf(vector.D64[1]),
				coshf(vector.D64[2]),
				coshf(vector.D64[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      TanH
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::TanH(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��TanH����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::TanH(ConstVector256 vector) noexcept
	{
		Vector256d result = { { {
				tanhf(vector.D64[0]),
				tanhf(vector.D64[1]),
				tanhf(vector.D64[2]),
				tanhf(vector.D64[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      ArcSin
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ArcSin(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��ArcSin����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ArcSin(ConstVector256 vector) noexcept
	{
		Vector256d result = { { {
				asinf(vector.D64[0]),
				asinf(vector.D64[1]),
				asinf(vector.D64[2]),
				asinf(vector.D64[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      ArcCos
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ArcCos(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��ArcSin����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ArcCos(ConstVector256 vector) noexcept
	{
		Vector256d result = { { {
				acosf(vector.D64[0]),
				acosf(vector.D64[1]),
				acosf(vector.D64[2]),
				acosf(vector.D64[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      ArcTan
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ArcCos(ConstVector256 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��ArcSin����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ArcTan(ConstVector256 vector) noexcept
	{
		Vector256d result = { { {
				atanf(vector.D64[0]),
				atanf(vector.D64[1]),
				atanf(vector.D64[2]),
				atanf(vector.D64[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                       Lerp
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Lerp(ConstVector256 start, ConstVector256 end, const double t) noexcept
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
		Vector256 scale  = Set(t);
		Vector256 length = Subtract(end, start);
		return MultiplyAdd(length, scale, start);
	}

	/****************************************************************************
	*                       LerpV
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LerpV(ConstVector256 start, ConstVector256 end, const double t) noexcept
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
		Vector256 length = Subtract(end, start);
		return MultiplyAdd(length, t, start);
	}

	/****************************************************************************
	*                       Hermite
	*************************************************************************//**
	*  @fn        __forceinline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Hermite(ConstVector256 startPosition, ConstVector256 startTangent, ConstVector256 endPosition, ConstVector256 endTangent, const double t) noexcept
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

		Vector256 p0 = Set(2.0f * t3 - 3.0f * t2 + 1.0f);
		Vector256 t0 = Set(t3 - 2.0f * t2 + t);
		Vector256 p1 = Set(-2.0f * t3 + 3.0f * t2);
		Vector256 t1 = Set(t3 - t2);
		
		Vector256 result = Multiply(p0, startPosition);
		result = MultiplyAdd(startTangent, t0, result);
		result = MultiplyAdd(endPosition, p1, result);
		result = MultiplyAdd(endTangent , t1, result);
		return result;
	}

	/****************************************************************************
	*                       HermiteV
	*************************************************************************//**
	*  @fn        __forceinline Vector256 SIMD_CALL_CONVENTION Vector256Utility::HermiteV(ConstVector256 startPosition, ConstVector256 startTangent, ConstVector256 endPosition, ConstVector256 endTangent, const double t) noexcept
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
		Vector256 t2 = Multiply(t, t);
		Vector256 t3 = Multiply(t, t2);

		Vector256 p0 = Set(2.0f * t3.D64[0] - 3.0f * t2.D64[0] + 1.0f);
		Vector256 t0 = Set(t3.D64[1] - 2.0f * t2.D64[1] + t.D64[1]);
		Vector256 p1 = Set(-2.0f * t3.D64[2] + 3.0f * t2.D64[2]);
		Vector256 t1 = Set(t3.D64[3] - t2.D64[3]);

		Vector256 result = Multiply(p0, startPosition);
		result = MultiplyAdd(t0, startTangent, result);
		result = MultiplyAdd(p1, endPosition, result);
		result = MultiplyAdd(t1, endTangent, result);

		return result;
	}

	/****************************************************************************
	*                       CatMullRom
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CatMullRom(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, ConstVector256 position3, const double t) noexcept
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

		Vector256 p0 = Set((-t3 + 2.0f * t2 - t) * 0.5f);
		Vector256 p1 = Set((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
		Vector256 p2 = Set((-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
		Vector256 p3 = Set((t3 - t2) * 0.5f);

		Vector256 result = Multiply(p0, position0);
		result = MultiplyAdd(p1, position1, result);
		result = MultiplyAdd(p2, position2, result);
		result = MultiplyAdd(p3, position3, result);

		return result;
	}

	/****************************************************************************
	*                       CatMullRom
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CatMullRom(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, ConstVector256 position3, const double t) noexcept
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
		const double fx = t.D64[0];
		const double fy = t.D64[1];
		const double fz = t.D64[2];
		const double fw = t.D64[3];
		Vector256d result = { { {
				0.5f * ((-fx * fx * fx + 2 * fx * fx - fx) * position0.D64[0]
				+ (3 * fx * fx * fx - 5 * fx * fx + 2) * position1.D64[0]
				+ (-3 * fx * fx * fx + 4 * fx * fx + fx) * position2.D64[0]
				+ (fx * fx * fx - fx * fx) * position3.D64[0]),

				0.5f * ((-fy * fy * fy + 2 * fy * fy - fy) * position0.D64[1]
				+ (3 * fy * fy * fy - 5 * fy * fy + 2) * position1.D64[1]
				+ (-3 * fy * fy * fy + 4 * fy * fy + fy) * position2.D64[1]
				+ (fy * fy * fy - fy * fy) * position3.D64[1]),

				0.5f * ((-fz * fz * fz + 2 * fz * fz - fz) * position0.D64[2]
				+ (3 * fz * fz * fz - 5 * fz * fz + 2) * position1.D64[2]
				+ (-3 * fz * fz * fz + 4 * fz * fz + fz) * position2.D64[2]
				+ (fz * fz * fz - fz * fz) * position3.D64[2]),

				0.5f * ((-fw * fw * fw + 2 * fw * fw - fw) * position0.D64[3]
				+ (3 * fw * fw * fw - 5 * fw * fw + 2) * position1.D64[3]
				+ (-3 * fw * fw * fw + 4 * fw * fw + fw) * position2.D64[3]
				+ (fw * fw * fw - fw * fw) * position3.D64[3])
			} } };
		return result.V;
	}

	/****************************************************************************
	*                      BaryCentric
	*************************************************************************//**
	*  @fn        __forceinline Vector256 SIMD_CALL_CONVENTION Vector256Utility::BaryCentric(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, const double f, const double g) noexcept
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
		Vector256 p10    = Subtract(position1, position0);
		Vector256 scaleF = Set(f);

		Vector256 p20    = Subtract(position2, position0);
		Vector256 scaleG = Set(g);

		Vector256 result = MultiplyAdd(p10, scaleF, position0);
		result = MultiplyAdd(p20, scaleG, result);

		return result;
	}

	/****************************************************************************
	*                      BaryCentricV
	*************************************************************************//**
	*  @fn        __forceinline Vector256 SIMD_CALL_CONVENTION Vector256Utility::BaryCentricV(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, const double f, const double g) noexcept
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
		Vector256 p10 = Subtract(position1, position0);
		Vector256 p20 = Subtract(position2, position0);

		Vector256 result = MultiplyAdd(p10, f, position0);
		result = MultiplyAdd(p20, g, result);

		return result;
	}

	/****************************************************************************
	*                       Min
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Min(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256d result = { { {
				left.D64[0] < right.D64[0] ? left.D64[0] : right.D64[0],
				left.D64[1] < right.D64[1] ? left.D64[1] : right.D64[1],
				left.D64[2] < right.D64[2] ? left.D64[2] : right.D64[2],
				left.D64[3] < right.D64[3] ? left.D64[3] : right.D64[3],
			} } };

		return result.V;
	}

	/****************************************************************************
	*                       Max
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Max(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256d result = { { {
				left.D64[0] > right.D64[0] ? left.D64[0] : right.D64[0],
				left.D64[1] > right.D64[1] ? left.D64[1] : right.D64[1],
				left.D64[2] > right.D64[2] ? left.D64[2] : right.D64[2],
				left.D64[3] > right.D64[3] ? left.D64[3] : right.D64[3],
			} } };

		return result.V;
	}

	namespace internal
	{
		inline double round_to_nearest(double x) noexcept
		{
			double i = floor(x);
			x -= i;
			if (x < 0.5f)
				return i;
			if (x > 0.5f)
				return i + 1.0;

			double int_part;
			(void)modf(i / 2.0, &int_part);
			if ((2.0 * int_part) == i)
			{
				return i;
			}

			return i + 1.0;
		}
	}

	/****************************************************************************
	*                       Round
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Round(ConstVector256 vector) noexcept
	*
	*  @brief     �ł��߂������Ɋۂ߂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Round(ConstVector256 vector) noexcept
	{
		Vector256d result = { { {
				internal::round_to_nearest(vector.D64[0]),
				internal::round_to_nearest(vector.D64[1]),
				internal::round_to_nearest(vector.D64[2]),
				internal::round_to_nearest(vector.D64[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                       Truncate
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Truncate(ConstVector256 vector) noexcept
	*
	*  @brief     �؂�̂� (�w�肵�����l�̒[����؂�̂Ă������Ɋۂ߂�B���������0�̕����ɂ��鎟�̐���
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Truncate(ConstVector256 vector) noexcept
	{
		Vector256 result = {};

		/*for (gu::uint32 i = 0; i < 4; i++)
		{
			if (IS_NAN(vector.D64[i]))
			{
				result.U64[i] = 0x7FC00000;
			}
			else if (fabs(vector.D64[i]) < 8388608.0f)
			{
				result.D64[i] = static_cast<double>(static_cast<gu::int32>(vector.D64[i]));
			}
			else
			{
				result.D64[i] = vector.D64[i];
			}
		}*/
		return result;
	}

	/****************************************************************************
	*                       Floor
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Floor(ConstVector256 vector) noexcept
	*
	*  @brief     ���̕����Ɋۂ߂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Floor(ConstVector256 vector) noexcept
	{
		Vector256d result = { { {
				floor(vector.D64[0]),
				floor(vector.D64[1]),
				floor(vector.D64[2]),
				floor(vector.D64[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      Ceiling
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Ceiling(ConstVector256 vector) noexcept
	*
	*  @brief     �؂�グ ��������Ő��̕����ɂ��鎟�̐����Ɋۂ߂�
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Ceiling(ConstVector256 vector) noexcept
	{
		Vector256d result = { { {
				ceil(vector.D64[0]),
				ceil(vector.D64[1]),
				ceil(vector.D64[2]),
				ceil(vector.D64[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                       Clamp
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Clamp(ConstVector256 vector, ConstVector256 min, ConstVector256 max) noexcept
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
		Vector256 result = Max(min, vector);
		return Min(max, result);
	}

	/****************************************************************************
	*                       Saturate
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Saturate(ConstVector256 vector)
	*
	*  @brief     1�ȏ�ɂ͍s���Ȃ��悤�ɂ���
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Saturate(ConstVector256 vector) noexcept
	{
		return Clamp(vector, VECTOR_256D_ZERO, VECTOR_256D_ONE);
	}

	/****************************************************************************
	*                       Abs
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Saturate(ConstVector256 vector)
	*
	*  @brief     ��Βl
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return �@�@Vector
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Abs(ConstVector256 vector) noexcept
	{
		Vector256d result = { { {
				fabs(vector.D64[0]),
				fabs(vector.D64[1]),
				fabs(vector.D64[2]),
				fabs(vector.D64[3])
			} } };

		return result.V;
	}

	#pragma endregion Math

	#pragma region Angle
	/****************************************************************************
	*                       ModAngles
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ModAngles(ConstVector256 vector)
	*
	*  @brief      - pi <= theta <= pi�͈̔͂Ɏ��߂�悤�ɂ��܂�.
	*
	*  @param[in] ConstVector256 angles : �p�xrad
	*
	*  @return �@�@Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ModAngles(ConstVector256 angles) noexcept
	{
		Vector256 vector = Multiply(angles, VECTOR_256D_RECIPROCAL_2PI.V);
		vector = Round(vector);
		return NegativeMultiplySubtract(VECTOR_256D_2PI.V, vector, angles);
	}

	/****************************************************************************
	*                       AddAngles
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::AddAngles(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 result = Add(left, right);
		Vector256 mask   = LessVectorEach(result, VECTOR_256D_NEGATIVE_PI.V);
		Vector256 offset = Select(VECTOR_256D_ZERO, VECTOR_256D_2PI.V, mask);

		mask = GreaterOrEqualVectorEach(result, VECTOR_256D_PI.V);
		offset = Select(offset, VECTOR_256D_NEGATIVE_2PI.V, mask);

		return Add(result, offset);
	}

	/****************************************************************************
	*                       SubtractAngles
	*************************************************************************//**
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ModAngles(ConstVector256 vector)
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
		Vector256 result = Subtract(left, right);
		Vector256 mask   = LessVectorEach(result, VECTOR_256D_NEGATIVE_PI.V);
		Vector256 offset = Select(VECTOR_256D_ZERO, VECTOR_256D_2PI.V, mask);

		mask = GreaterOrEqualVectorEach(result, VECTOR_256D_PI.V);
		offset = Select(offset, VECTOR_256D_NEGATIVE_2PI.V, mask);

		return Add(result, offset);
	}
	#pragma endregion Angle
#pragma endregion Implement
}
#endif
#endif