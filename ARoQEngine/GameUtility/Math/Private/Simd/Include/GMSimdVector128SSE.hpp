//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdVector128SSE.hpp
///             @brief  SSE�ɂ��Simd���Z�ł�.
///                     Vector128�^�̎��� 
///             @author toide
///             @date   2024/03/11 2:25:36
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_VECTOR_128_SSE_HPP
#define GM_SIMD_VECTOR_128_SSE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMSimdMacros.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_CPU_INSTRUCTION_SSE && !defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)

#include "GameUtility/Base/Include/GUAssert.hpp"
// SSE�̃C���N���[�h
#include <intrin.h>
#include <xmmintrin.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gm::simd::sse
{
	/*----------------------------------------------------------------------
	*         128 bit�̐����^��float�^���i�[�\�ȃx�N�g�����쐬����
	*----------------------------------------------------------------------*/
	using Vector128 = __m128;

	/*----------------------------------------------------------------------
	*        �萔�Ƃ��Ă̕ϊ��p, float�|�C���^�ɕϊ�������, m128�̊e�^�ɕϊ�������
	*----------------------------------------------------------------------*/
	ALIGNED_STRUCT(16) Vector128u
	{
		union
		{
			gu::uint32 U[4];
			Vector128  V;
		};

		inline operator Vector128() const noexcept { return V; }
		inline operator __m128i() const noexcept { return _mm_castps_si128(V); }
		inline operator __m128d() const noexcept { return _mm_castps_pd(V); }
	};

	/*----------------------------------------------------------------------
	*        �萔�Ƃ��Ă̕ϊ��p, float�|�C���^�ɕϊ�������, m128�̊e�^�ɕϊ�������
	*----------------------------------------------------------------------*/
	ALIGNED_STRUCT(16) Vector128i
	{
		union
		{
			gu::int32 U[4];
			Vector128  V;
		};

		inline operator Vector128() const noexcept { return V; }
		inline operator __m128i() const noexcept { return _mm_castps_si128(V); }
		inline operator __m128d() const noexcept { return _mm_castps_pd(V); }
	};

	/*----------------------------------------------------------------------
	*        �萔�Ƃ��Ă̕ϊ��p, float�|�C���^�ɕϊ�������, m128�̊e�^�ɕϊ�������
	*----------------------------------------------------------------------*/
	ALIGNED_STRUCT(16) Vector128f
	{
		union
		{
			float     F[4];
			Vector128 V;
		};

		inline operator Vector128()    const noexcept { return V; }
		inline operator const float* () const noexcept { return F; }
		inline operator __m128i() const noexcept { return _mm_castps_si128(V); }
		inline operator __m128d() const noexcept { return _mm_castps_pd(V); }
	};

	constexpr float GM_PI_FLOAT        = 3.141592654f;
	constexpr float GM_2PI_FLOAT       = 6.283185307f;
	constexpr float GM_1_DIV_PI_FLOAT  = 0.318309886f;
	constexpr float GM_1_DIV_2PI_FLOAT = 0.159154943f;
	constexpr float GM_PI_DIV2_FLOAT   = 1.570796327f;
	constexpr float GM_PI_DIV4_FLOAT   = 0.785398163f;
	constexpr gu::uint32 GM_SELECT_0 = 0x00000000;
	constexpr gu::uint32 GM_SELECT_1 = 0xFFFFFFFF;
	constexpr gu::uint32 GM_SWIZZLE_X = 0;
	constexpr gu::uint32 GM_SWIZZLE_Y = 1;
	constexpr gu::uint32 GM_SWIZZLE_Z = 2;
	constexpr gu::uint32 GM_SWIZZLE_W = 3;
	constexpr gu::uint32 GM_PERMUTE_0X = 0;
	constexpr gu::uint32 GM_PERMUTE_0Y = 1;
	constexpr gu::uint32 GM_PERMUTE_0Z = 2;
	constexpr gu::uint32 GM_PERMUTE_0W = 3;
	constexpr gu::uint32 GM_PERMUTE_1X = 4;
	constexpr gu::uint32 GM_PERMUTE_1Y = 5;
	constexpr gu::uint32 GM_PERMUTE_1Z = 6;
	constexpr gu::uint32 GM_PERMUTE_1W = 7;

	GLOBAL_CONST Vector128f VECTOR_128F_ONE            = { { {  1.0f, 1.0f, 1.0f, 1.0f  } } };
	GLOBAL_CONST Vector128f VECTOR_128F_ZERO           = { { {  0.0f, 0.0f, 0.0f, 0.0f  } } };
	GLOBAL_CONST Vector128f VECTOR_128F_NEGATIVE_ONE   = { { {  -1.0f, -1.0f, -1.0f, -1.0f  } } };
	GLOBAL_CONST Vector128f VECTOR_128F_NEGATE_X       = { { { -1.0f, 1.0f, 1.0f, 1.0f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_NEGATE_Y       = { { { 1.0f, -1.0f, 1.0f, 1.0f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_NEGATE_Z       = { { { 1.0f, 1.0f, -1.0f, 1.0f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_NEGATE_W       = { { { 1.0f, 1.0f, 1.0f, -1.0f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_INFINITY       = { { {  0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000  } }};
	GLOBAL_CONST Vector128f VECTOR_128F_EPSILON        = { { {  1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f } } };
	GLOBAL_CONST Vector128i VECTOR_128F_QNAN           = { { {  0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000  } } };
	GLOBAL_CONST Vector128i VECTOR_128I_QNAN_TEST      = { { {  0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 } } };
	GLOBAL_CONST Vector128f VECTOR_128F_IDENTITY_R0    = { { { 1.0f, 0.0f, 0.0f, 0.0f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_IDENTITY_R1    = { { { 0.0f, 1.0f, 0.0f, 0.0f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_IDENTITY_R2    = { { { 0.0f, 0.0f, 1.0f, 0.0f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_IDENTITY_R3    = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_NEGATIVE_IDENTITY_R0 = { { { -1.0f, 0.0f, 0.0f, 0.0f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_NEGATIVE_IDENTITY_R1 = { { { 0.0f, -1.0f, 0.0f, 0.0f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_NEGATIVE_IDENTITY_R2 = { { { 0.0f, 0.0f, -1.0f, 0.0f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_NEGATIVE_IDENTITY_R3 = { { { 0.0f, 0.0f, 0.0f, -1.0f } } };
	GLOBAL_CONST Vector128u VECTOR_128F_SELECT_1000    = { { { GM_SELECT_1, GM_SELECT_0, GM_SELECT_0, GM_SELECT_0 } } };
	GLOBAL_CONST Vector128u VECTOR_128F_SELECT_1100    = { { { GM_SELECT_1, GM_SELECT_1, GM_SELECT_0, GM_SELECT_0 } } };
	GLOBAL_CONST Vector128u VECTOR_128F_SELECT_1110    = { { { GM_SELECT_1, GM_SELECT_1, GM_SELECT_1, GM_SELECT_0 } } };
	GLOBAL_CONST Vector128u VECTOR_128F_SELECT_1011    = { { { GM_SELECT_1, GM_SELECT_0, GM_SELECT_1, GM_SELECT_1 } } };
	GLOBAL_CONST Vector128u VECTOR_128U_MASK_X         = { { {  0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000   } } };
	GLOBAL_CONST Vector128u VECTOR_128U_MASK_Y         = { { {  0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000   } } };
	GLOBAL_CONST Vector128u VECTOR_128U_MASK_Z         = { { {  0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000   } } };
	GLOBAL_CONST Vector128u VECTOR_128U_MASK_W         = { { {  0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF   } } };
	GLOBAL_CONST Vector128u VECTOR_128U_MASK_XY        = { { {  0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000 } } };
	GLOBAL_CONST Vector128u VECTOR_128F_MASK_XYZ       = { { {  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000   } } };
	GLOBAL_CONST Vector128u VECTOR_128U_MASK_ABS       = { { {  0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF   } } };
	GLOBAL_CONST Vector128u VECTOR_128U_NEGATIVE_ZERO  = { { {  0x80000000, 0x80000000, 0x80000000, 0x80000000  } } };
	GLOBAL_CONST Vector128u VECTOR_128U_NEGATIVE_ONE_MASK = { { {  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF  } } };
	GLOBAL_CONST Vector128u VECTOR_128U_BIN_NEGATIVE_150 = { { { 0xC3160000, 0xC3160000, 0xC3160000, 0xC3160000 } } };
	GLOBAL_CONST Vector128i VECTOR_128I_BIN128           = { { { 0x43000000, 0x43000000, 0x43000000, 0x43000000 } } };
	GLOBAL_CONST Vector128i VECTOR_128I_253              = { { { 253, 253, 253, 253 } } };
	GLOBAL_CONST Vector128i VECTOR_128I_EXPONENTIAL_BIAS = { { { 127, 127, 127, 127 } } };
	GLOBAL_CONST Vector128i VECTOR_128I_EXPONENTIAL_SUBNORMAL = { { { -126, -126, -126, -126 } } };
	GLOBAL_CONST Vector128i VECTOR_128I_NUM_TRAILING      = { { { 23, 23, 23, 23 } } };
	GLOBAL_CONST Vector128i VECTOR_128I_MIN_NORMAL        = { { { 0x00800000, 0x00800000, 0x00800000, 0x00800000 } } };
	GLOBAL_CONST Vector128f VECTOR_128F_ONE_HALF          = { { { 0.5f, 0.5f, 0.5f, 0.5f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_NEGATIVE_ONE_HALF = { { { -0.5f, -0.5f, -0.5f, -0.5f } } };
	GLOBAL_CONST Vector128u VECTOR_128F_NEGATIVE_INFINITY = { { { 0xFF800000, 0xFF800000, 0xFF800000, 0xFF800000 } } };
	GLOBAL_CONST Vector128u VECTOR_128F_NEGATIVE_QNAN     = { { { 0xFFC00000, 0xFFC00000, 0xFFC00000, 0xFFC00000 } } };
	GLOBAL_CONST Vector128f VECTOR_128F_HALF_PI           = { { {  GM_PI_DIV2_FLOAT  , GM_PI_DIV2_FLOAT  , GM_PI_DIV2_FLOAT , GM_PI_DIV2_FLOAT   } }};
	GLOBAL_CONST Vector128f VECTOR_128F_PI                = { { {  GM_PI_FLOAT       , GM_PI_FLOAT       , GM_PI_FLOAT , GM_PI_FLOAT  } } };
	GLOBAL_CONST Vector128f VECTOR_128F_NEGATIVE_PI       = { { {  -GM_PI_FLOAT       , -GM_PI_FLOAT       , -GM_PI_FLOAT , -GM_PI_FLOAT  } } };
	GLOBAL_CONST Vector128f VECTOR_128F_RECIPROCAL_PI     = { { {  GM_1_DIV_PI_FLOAT , GM_1_DIV_PI_FLOAT , GM_1_DIV_PI_FLOAT , GM_1_DIV_PI_FLOAT   } } };
	GLOBAL_CONST Vector128f VECTOR_128F_RECIPROCAL_2PI    = { { {  GM_1_DIV_2PI_FLOAT, GM_1_DIV_2PI_FLOAT, GM_1_DIV_2PI_FLOAT, GM_1_DIV_2PI_FLOAT   } } };
	GLOBAL_CONST Vector128f VECTOR_128F_2PI               = { { { GM_2PI_FLOAT      , GM_2PI_FLOAT      , GM_2PI_FLOAT, GM_2PI_FLOAT} } };
	GLOBAL_CONST Vector128f VECTOR_128F_SIN_COEFFICIENTS0 = { { {  -0.16666667f, +0.0083333310f, -0.00019840874f, +2.7525562e-06f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_SIN_COEFFICIENTS1 = { { {  -2.3889859e-08f, -0.16665852f /*Est1*/, +0.0083139502f /*Est2*/, -0.00018524670f /*Est3*/ } } };
	GLOBAL_CONST Vector128f VECTOR_128F_COS_COEFFICIENTS0 = { { {  -0.5f, +0.041666638f, -0.0013888378f, +2.4760495e-05f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_COS_COEFFICIENTS1 = { { {  -2.6051615e-07f, -0.49992746f /*Est1*/, +0.041493919f /*Est2*/, -0.0012712436f /*Est3*/ } } } ;
	GLOBAL_CONST Vector128f VECTOR_128F_TAN_COEFFICIENTS0 = { { {  1.0f, 0.333333333f, 0.133333333f, 5.396825397e-2f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_TAN_COEFFICIENTS1 = { { {  2.186948854e-2f, 8.863235530e-3f, 3.592128167e-3f, 1.455834485e-3f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_TAN_COEFFICIENTS2 = { { {  5.900274264e-4f, 2.391290764e-4f, 9.691537707e-5f, 3.927832950e-5f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_ARC_COEFFICIENTS0 = { { {  +1.5707963050f, -0.2145988016f, +0.0889789874f, -0.0501743046f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_ARC_COEFFICIENTS1 = { { {  +0.0308918810f, -0.0170881256f, +0.0066700901f, -0.0012624911f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_ATAN_COEFFICIENTS0 = { { {  -0.3333314528f, +0.1999355085f, -0.1420889944f, +0.1065626393f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_ATAN_COEFFICIENTS1 = { { {  -0.0752896400f, +0.0429096138f, -0.0161657367f, +0.0028662257f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_NO_FRACTION        = { { { 8388608.0f, 8388608.0f, 8388608.0f, 8388608.0f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_EXPONENTIAL_ESTIMATE1 = { { { -6.93147182e-1f, -6.93147182e-1f, -6.93147182e-1f, -6.93147182e-1f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_EXPONENTIAL_ESTIMATE2 = { { { +2.40226462e-1f, +2.40226462e-1f, +2.40226462e-1f, +2.40226462e-1f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_EXPONENTIAL_ESTIMATE3 = { { { -5.55036440e-2f, -5.55036440e-2f, -5.55036440e-2f, -5.55036440e-2f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_EXPONENTIAL_ESTIMATE4 = { { { +9.61597636e-3f, +9.61597636e-3f, +9.61597636e-3f, +9.61597636e-3f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_EXPONENTIAL_ESTIMATE5 = { { { -1.32823968e-3f, -1.32823968e-3f, -1.32823968e-3f, -1.32823968e-3f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_EXPONENTIAL_ESTIMATE6 = { { { +1.47491097e-4f, +1.47491097e-4f, +1.47491097e-4f, +1.47491097e-4f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_EXPONENTIAL_ESTIMATE7 = { { { -1.08635004e-5f, -1.08635004e-5f, -1.08635004e-5f, -1.08635004e-5f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_LOG_ESTIMATE0 = { { { +1.442693f, +1.442693f, +1.442693f, +1.442693f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_LOG_ESTIMATE1 = { { { -0.721242f, -0.721242f, -0.721242f, -0.721242f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_LOG_ESTIMATE2 = { { { +0.479384f, +0.479384f, +0.479384f, +0.479384f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_LOG_ESTIMATE3 = { { { -0.350295f, -0.350295f, -0.350295f, -0.350295f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_LOG_ESTIMATE4 = { { { +0.248590f, +0.248590f, +0.248590f, +0.248590f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_LOG_ESTIMATE5 = { { { -0.145700f, -0.145700f, -0.145700f, -0.145700f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_LOG_ESTIMATE6 = { { { +0.057148f, +0.057148f, +0.057148f, +0.057148f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_LOG_ESTIMATE7 = { { { -0.010578f, -0.010578f, -0.010578f, -0.010578f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_LOG_E         = { { { +1.442695f, +1.442695f, +1.442695f, +1.442695f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_LOG_10        = { { { +3.321928f, +3.321928f, +3.321928f, +3.321928f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_LOG_E_INVERSE = { { { +6.93147182e-1f, +6.93147182e-1f, +6.93147182e-1f, +6.93147182e-1f } } };
	GLOBAL_CONST Vector128f VECTOR_128F_LOG_10_INVERSE = { { { +3.010299956e-1f, +3.010299956e-1f, +3.010299956e-1f, +3.010299956e-1f } } };

	// �萔���łȂ��ƃG���[�ƂȂ邽��
	#if PLATFORM_CPU_INSTRUCTION_AVX
		#define PERMUTE_PS(v, c) _mm_permute_ps((v), c)
	#else
		#define PERMUTE_PS(v, c) _mm_shuffle_ps((v), (v), c)
	#endif
	/****************************************************************************
	*				  			  Vector128Utility
	****************************************************************************/
	/* @class     Vector128Utility
	*  @brief     SSE�̃o�[�W�����ɂ��Simd���Z
	*****************************************************************************/
	class Vector128Utility
	{
	public:
		#if ( defined(_M_IX86) || defined(_M_ARM) || defined(_M_ARM64) || USE_VECTOR_CALL || __i386__ || __arm__ || __aarch64__ )
			using ConstVector128 = const Vector128;
		#else
			using ConstVector128 = const Vector128&;
		#endif
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		#pragma region Setter
		/*----------------------------------------------------------------------
		*  @brief : �[������������Vector128��Ԃ��֐�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Zero() noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ẵr�b�g��true (1)�ɐݒ肳��Ă���Vector128��Ԃ�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION TrueIntMask() noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ẵr�b�g��false (0)�ɐݒ肳��Ă���Vector128��Ԃ� Zero()�Ɠ���
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION FalseIntMask() noexcept;

		/*----------------------------------------------------------------------
		*  @brief : float4����Vector128�N���X�𐶐�����
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Set(const float x, const float y, const float z, const float w) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : float1����Vector128�N���X�𐶐�����
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Set(const float value) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : Int4����Vector128�N���X�𐶐�����
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION SetInt(const gu::uint32 x, const gu::uint32 y, const gu::uint32 z, const gu::uint32 w) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : Float���g����Vector��X(, Y, Z, W)�v�f�ɒl��������
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION SetX(ConstVector128 vector, const float x) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION SetY(ConstVector128 vector, const float y) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION SetZ(ConstVector128 vector, const float z) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION SetW(ConstVector128 vector, const float w) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �x�N�g�����g����float�z��ɑ������
		/*----------------------------------------------------------------------*/
		__forceinline static void SIMD_CALL_CONVENTION StoreFloat (float* destination, ConstVector128 source) noexcept;
		__forceinline static void SIMD_CALL_CONVENTION StoreFloat2(float* destination, ConstVector128 source) noexcept;
		__forceinline static void SIMD_CALL_CONVENTION StoreFloat3(float* destination, ConstVector128 source) noexcept;
		__forceinline static void SIMD_CALL_CONVENTION StoreFloat4(float* destination, ConstVector128 source) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : X(, Y, Z, W)���̗v�f�Ɋ�Â���, �S�Ă̗v�f�ɓ����l��������Vector128�N���X���쐬����
		*           X,Y,Z,W  : �e�v�f
		*           One      : 1, 1, 1, 1
		*           Infinity : INF, INF, INF, INF
		*           Epsilon  : floatEpsilon
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION SplatX(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION SplatY(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION SplatZ(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION SplatW(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION SplatOne() noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION SplatInfinity() noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION SplatEpsilon() noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION SplatSignMask() noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �x�N�g������ёւ��܂�.index���w�肷��Ƃ���index�ɂ�����Vector4�^���擾
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Swizzle(ConstVector128 vector,
			const gu::uint32 xIndex, const gu::uint32 yIndex,
			const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g������v�f�𒊏o��, �V�����x�N�g�����쐬����
		*           0�`3 : a�����x, y, z, w
		*           4�`7 : b�����x, y, z, w
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Permute(ConstVector128 a, ConstVector128,
			const gu::uint32 xIndex, const gu::uint32 yIndex,
			const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g������v�f�𒊏o��, �V�����x�N�g�����쐬����
		*           control vector��0�̏ꍇ��left����, ��0�̏ꍇ��right���炻�ꂼ�ꒊ�o���s���܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Select(ConstVector128 left, ConstVector128 right, ConstVector128 control) noexcept;
		#pragma endregion Setter

		#pragma region Getter
		/*----------------------------------------------------------------------
		*  @brief : FPU���W�X�^�Ɋi�[���ꂽX(, Y, Z, W)���̗v�f�����o��
		/*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION GetX(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION GetY(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION GetZ(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION GetW(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : float�̔z����g���ăx�N�g���Ɋi�[����
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION LoadFloat (const float* source) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION LoadFloat2(const float* source) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION LoadFloat3(const float* source) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION LoadFloat4(const float* source) noexcept;

		#pragma endregion Getter

		#pragma region Operator
		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g���̊e�v�f���m�̘a���Z�o���܂�. (left + right)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Add(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g���̊e�v�f���m�̍����Z�o���܂�. (left - right)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Subtract(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g���̊e�v�f���m�̐ς��Z�o���܂�. (left * right)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Multiply(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g���̊e�v�f���m�̏��Z���Z�o���܂�. (left / right)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Divide(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g���̊e�v�f���m�̂��܂���Z�o���܂�. (left % right)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Mod(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief :  subtractLeft - (multiplyLeft * multiplyRight)���Z�o���܂�.
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION NegativeMultiplySubtract(ConstVector128 multiplyLeft, ConstVector128 multiplyRight, ConstVector128 subtractLeft) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : multiplyLeft * multiplyRight + add���Z�o���܂�.
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION MultiplyAdd(ConstVector128 multiplyLeft, ConstVector128 multiplyRight, ConstVector128 add) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �x�N�g���̊e�v�f���m�̋t�����Z�o���܂�. (1.0f / vector)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Reciprocal(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : float�l���g���Ċe�v�f�Ɋ|���܂� scale * vector
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Scale(ConstVector128 vector, const float scale) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ��̃x�N�g�������������ǂ����𒲂ׂ܂�
		*           (Each�͊e�v�f���ɓ��������ǂ����𒲂ׂ܂�.)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION EqualVector2(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION EqualVector3(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION EqualVector4(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION EqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ��̃x�N�g�����������Ȃ����𒲂ׂ܂�
		*           (Each�͊e�v�f���ɓ��������ǂ����𒲂ׂ܂�.)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualVector2(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualVector3(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualVector4(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION NotEqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ��̃x�N�g�������ׂē��������ǂ����𒲂ׂ܂�.
		*           ��r���ɂ͕����Ȃ�����(U32)�Ƃ��Ĕ�r���܂�.
		*           (Each�͊e�v�f���Ƃɓ��������ǂ����𒲂ׂ܂�)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION EqualAsIntVector2(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION EqualAsIntVector3(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION EqualAsIntVector4(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION EqualAsIntVectorEach(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ��̃x�N�g������ł��������Ȃ����ǂ����𒲂ׂ܂�.
		*           ��r���ɂ͕����Ȃ�����(U32)�Ƃ��Ĕ�r���܂�.
		*           (Each�͊e�v�f���Ƃɓ��������ǂ����𒲂ׂ܂�)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualAsIntVector2(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualAsIntVector3(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualAsIntVector4(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION NotEqualAsIntVectorEach(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���͈͓��Œl�����������𒲂ׂ� (left - epsilon <= right <= left + epsilon)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector2   (ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector3   (ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector4   (ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION NearEqualVectorEach(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̕����S�Ă̗v�f�ɂ����đ傫�����𒲂ׂ܂� (left > right)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterVector2   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterVector3   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterVector4   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION GreaterVectorEach(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̕����S�Ă̗v�f�ɂ����đ傫��or���������𒲂ׂ܂� (left >= right)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterOrEqualVector2   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterOrEqualVector3   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterOrEqualVector4   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION GreaterOrEqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̕����S�Ă̗v�f�ɂ����ď��������𒲂ׂ܂� (left < right)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION LessVector2   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessVector3   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessVector4   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION LessVectorEach(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̕����S�Ă̗v�f�ɂ����ď�����or���������𒲂ׂ܂� (left <= right)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION LessOrEqualVector2   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessOrEqualVector3   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessOrEqualVector4   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION LessOrEqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �w��͈͓��ɂ��邩�ǂ��� -bounds <= vector <= +bounds
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION InBoundsVector2(ConstVector128 vector, ConstVector128 bounds) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION InBoundsVector3(ConstVector128 vector, ConstVector128 bounds) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION InBoundsVector4(ConstVector128 vector, ConstVector128 bounds) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION InBoundsVectorEach(ConstVector128 vector, ConstVector128 bounds) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ��������shiftCount�� (3�܂�)�v�f����]����
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION RotateLeft(ConstVector128 vector, const gu::uint32 shiftCount) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �E������shiftCount�� (3�܂�)�v�f����]����
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION RotateRight(ConstVector128 vector, const gu::uint32 shiftCount) noexcept;

		#pragma endregion Operator

		#pragma region Bit
		/*----------------------------------------------------------------------
		*  @brief : �r�b�g�P�ʂ̘_���ς̉��Z���s���܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION AndInt(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �r�b�g�P�ʂ̘_���a�̉��Z���s���܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION OrInt(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �r�b�g�P�ʂ̘_���ϔے�̉��Z���s���܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION NorInt(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �r�b�g�P�ʂ̔r���I�_���a�̉��Z���s���܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION XorInt(ConstVector128 left, ConstVector128 right) noexcept;

		#pragma endregion Bit

		#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : �e�v�f�Ƀ}�C�i�X����������̂�Ԃ�.
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Negate(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2���m����(�e�v�f��2��a�����[�g��������Ƃ��̃x�N�g���̑傫��)���Z�o
		/*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION LengthVector2(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION LengthVector3(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION LengthVector4(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2���m������2����Z�o
		/*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION LengthSquaredVector2(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION LengthSquaredVector3(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION LengthSquaredVector4(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���ς��Z�o (������, �Ԃ�l��float�ł͂Ȃ�Vector128�^�ŕԂ���܂��̂�, ��XGetX�ȂǂőΉ����Ă�������.)
		/*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION DotVector2(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION DotVector3(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION DotVector4(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �O�ς��Z�o left, right�ɐ����ȃx�N�g����Ԃ�
		*           2�����̏ꍇ�͕��s�l�ӌ`�̖ʐς�Ԃ��Ƃ����Ӗ�����
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION CrossVector2(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION CrossVector3(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION CrossVector4(ConstVector128 first, ConstVector128 second, ConstVector128 third) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION NormalizeVector2(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION NormalizeVector3(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION NormalizeVector4(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief     ���˃x�N�g�����Z�o
		*  @param[in] ConstVector128 : input  (���˃x�N�g��)
		*  @param[in] ConstVector128 : normal (���˂��s���@���x�N�g��)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION ReflectVector2(ConstVector128 input, ConstVector128 normal) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION ReflectVector3(ConstVector128 input, ConstVector128 normal) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION ReflectVector4(ConstVector128 input, ConstVector128 normal) noexcept;

		/*----------------------------------------------------------------------
		*  @brief     ���܃x�N�g�����Z�o
		*  @param[in] ConstVector128 : input  (���˃x�N�g��)
		*  @param[in] ConstVector128 : normal (���˂��s���@���x�N�g��)
		*  @param[in] const float    : refractionIndex (���ܗ�)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION RefractVector2(ConstVector128 input, ConstVector128 normal, const float refractionIndex) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION RefractVector3(ConstVector128 input, ConstVector128 normal, const float refractionIndex) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION RefractVector4(ConstVector128 input, ConstVector128 normal, const float refractionIndex) noexcept;

		__forceinline static Vector128 SIMD_CALL_CONVENTION RefractVector2V(ConstVector128 input, ConstVector128 normal, ConstVector128 refractionIndex) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION RefractVector3V(ConstVector128 input, ConstVector128 normal, ConstVector128 refractionIndex) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION RefractVector4V(ConstVector128 input, ConstVector128 normal, ConstVector128 refractionIndex) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���ɑ傫���l��
		/*----------------------------------------------------------------------*/
		__forceinline static bool IsInfiniteVector2(ConstVector128 vector) noexcept;
		__forceinline static bool IsInfiniteVector3(ConstVector128 vector) noexcept;
		__forceinline static bool IsInfiniteVector4(ConstVector128 vector) noexcept;
		__forceinline static Vector128 IsInfiniteVectorEach(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�ɍ�������������̂�Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Sqrt(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�̋t�������, ����ɍ�������������̂�Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION ReciprocalSqrt(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��sin, cos, tan��Ԃ��܂� [rad]
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Sin(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION Cos(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION Tan(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��arcsin, arccos, arctan��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION ArcSin(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION ArcCos(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION ArcTan(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION ArcTan2(ConstVector128 y, ConstVector128 x) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��sinH, cosH, tanH��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION SinH(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION CosH(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION TanH(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�ɑ΂���sin��cos�����o���܂�
		*           input      : radian��P�ʂɎ��p�x
		*           Vector128* : sin(radian)�Ǝ������4�����x�N�g��
		*           Vector128* : cos(radian)�Ǝ������4�����x�N�g��
		/*----------------------------------------------------------------------*/
		__forceinline static void SIMD_CALL_CONVENTION SinCos(ConstVector128 input, Vector128* sin, Vector128* cos) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��2^{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Exp2(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��10^{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Exp10(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��e^{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION ExpE(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��log2{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Log2(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��log10{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Log10(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��loge{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION LogE(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��Pow��Ԃ��܂�
		*           base^{power}
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Pow(ConstVector128 base, ConstVector128 power) noexcept;

		/*----------------------------------------------------------------------
		*  @brief      ���`��Ԃ�Ԃ��܂�
		*  @param[in]  t = 0�̎��̒l
		   @param[in]  t = 1�̎��̒l
		   @param[in]  t : ���`��Ԃ̊���
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Lerp(ConstVector128 start, ConstVector128 end, const float t) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION LerpV(ConstVector128 start, ConstVector128 end, ConstVector128 t) noexcept;


		/*----------------------------------------------------------------------
		*  @brief      �G���~�[�g��Ԃ�Ԃ��܂�
		*  @param[in]  �n�_�̈ʒu
		   @param[in]  �n�_�ɂ�����X�v���C���̐ڐ���\���x�N�g��
		   @param[in]  �I�_�̈ʒu
		   @param[in]  �I�_�ɂ�����X�v���C���̐ڐ���\���x�N�g��
		   @param[in]  t : �G���~�[�g��Ԃ̊���
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Hermite(ConstVector128 startPosition, ConstVector128 startTangent, ConstVector128 endPosition, ConstVector128 endTangent, const float t) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION HermiteV(ConstVector128 startPosition, ConstVector128 startTangent, ConstVector128 endPosition, ConstVector128 endTangent, ConstVector128 t) noexcept;

		/*----------------------------------------------------------------------
		*  @brief      CatMull-Rom�X�v���C����Ԃ̌��ʂ�Ԃ��܂��B4�̐���_��S�Ēʂ�X�v���C�����
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION CatMullRom(ConstVector128 position0, ConstVector128 position1, ConstVector128 position2, ConstVector128 position3, const float t   ) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION CatMullRomV(ConstVector128 position0, ConstVector128 position1, ConstVector128 position2, ConstVector128 position3, ConstVector128 t) noexcept;

		/*----------------------------------------------------------------------
		*  @brief      �O�p�`�̏d�S�d�݈ʒu���Z�o
		*              (f>=0 && g>=0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�͎O�p�`�� Position0>Position1>Position2 �̓����ɂ���܂�>�B
                       (f==0 && g>=0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�� Position0>Position2 �s�ڂɂ���܂�>�B
                       (f>=0 && g==0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�� Position0>Position1> �s�ڂɂ���܂��B
                       (f>=0 && g>=0 && 1-f-g==0) �̏ꍇ�A�|�C���g�� Position1>Position2 �s�ڂɂ���܂�>�B
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION BaryCentric (ConstVector128 position0, ConstVector128 position1, ConstVector128 position2, const float f, const float g) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION BaryCentricV(ConstVector128 position0, ConstVector128 position1, ConstVector128 position2, ConstVector128 f, ConstVector128 g) noexcept;
		
		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�ɂ��čŏ��l�ƂȂ����I�����܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Min(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�ɂ��čő�l�ƂȂ����I�����܂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Max(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �ł��߂������Ɋۂ߂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Round(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �؂�̂� (�w�肵�����l�̒[����؂�̂Ă������Ɋۂ߂�B���������0�̕����ɂ��鎟�̐���
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Truncate(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ���̕����Ɋۂ߂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Floor(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �؂�グ ��������Ő��̕����ɂ��鎟�̐����Ɋۂ߂�
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Ceiling(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : �w��͈͓��Ő��l��ݒ肷��
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Clamp(ConstVector128 vector, ConstVector128 min, ConstVector128 max) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 1�ȏ�ɂ͍s���Ȃ��悤�ɂ���
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Saturate(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ��Βl
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Abs(ConstVector128 vector) noexcept;

		#pragma endregion Math

		#pragma region Angle
		/*----------------------------------------------------------------------
		*  @brief : - pi <= theta <= pi�͈̔͂Ɏ��߂�悤�ɂ��܂�.
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION ModAngles(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : - pi <= theta <= pi�͈̔͂Ɏ��߂�悤�ɂ�����ő����Z���s���܂�.
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION AddAngles(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : - pi <= theta <= pi�͈̔͂Ɏ��߂�悤�ɂ�����ň����Z���s���܂�.
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION SubtractAngles(ConstVector128 left, ConstVector128 right) noexcept;
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
	#pragma region Swizzle
	template<gu::uint32 swizzleX, gu::uint32 swizzleY, gu::uint32 swizzleZ, gu::uint32 swizzleW>
	inline Vector128 SIMD_CALL_CONVENTION VectorSwizzle(Vector128Utility::ConstVector128 vector) noexcept
	{
		static_assert(swizzleX <= 3, "SwizzleX template parameter out of range");
		static_assert(swizzleY <= 3, "SwizzleY template parameter out of range");
		static_assert(swizzleZ <= 3, "SwizzleZ template parameter out of range");
		static_assert(swizzleW <= 3, "SwizzleW template parameter out of range");

		#if PLATFORM_CPU_INSTRUCTION_SSE && !defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
			return PERMUTE_PS(vector, _MM_SHUFFLE(swizzleW, swizzleZ, swizzleY, swizzleX));
		#else
			return Vector128Utility::Swizzle(vector, swizzleX, swizzleY, swizzleZ, sw)
		#endif
	}


	template<> constexpr Vector128 SIMD_CALL_CONVENTION VectorSwizzle<0, 1, 2, 3>(Vector128Utility::ConstVector128 vector) noexcept { return vector; }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorSwizzle<0, 1, 0, 1>(Vector128Utility::ConstVector128 vector) noexcept { return _mm_movelh_ps(vector, vector); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorSwizzle<2, 3, 2, 3>(Vector128Utility::ConstVector128 vector) noexcept { return _mm_movehl_ps(vector, vector); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorSwizzle<0, 0, 1, 1>(Vector128Utility::ConstVector128 vector) noexcept { return _mm_unpacklo_ps(vector, vector); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorSwizzle<2, 2, 3, 3>(Vector128Utility::ConstVector128 vector) noexcept { return _mm_unpackhi_ps(vector, vector); }
	#pragma endregion Swizzle
	#pragma region Permute
	// SIMD���Z�̃o�[�W�������Ƃɓ��ʂȑΉ����K�v�ƂȂ�Permute���Z
	namespace internal
	{
		// Slow path fallback for permutes that do not map to a single SSE shuffle opcode.
		template<gu::uint32 Shuffle, bool WhichX, bool WhichY, bool WhichZ, bool WhichW> struct PermuteHelper
		{
			static Vector128 SIMD_CALL_CONVENTION Permute(Vector128Utility::ConstVector128 v1, Vector128Utility::ConstVector128 v2) noexcept
			{
				static const Vector128u selectMask =
				{ { {
						WhichX ? 0xFFFFFFFF : 0,
						WhichY ? 0xFFFFFFFF : 0,
						WhichZ ? 0xFFFFFFFF : 0,
						WhichW ? 0xFFFFFFFF : 0,
				} } };

				Vector128 shuffled1 = PERMUTE_PS(v1, Shuffle);
				Vector128 shuffled2 = PERMUTE_PS(v2, Shuffle);

				Vector128 masked1 = _mm_andnot_ps(selectMask, shuffled1);
				Vector128 masked2 = _mm_and_ps(selectMask, shuffled2);

				return _mm_or_ps(masked1, masked2);
			}
		};

		// Fast path for permutes that only read from the first vector.
		template<gu::uint32 Shuffle> struct PermuteHelper<Shuffle, false, false, false, false>
		{
			static Vector128 SIMD_CALL_CONVENTION Permute(Vector128Utility::ConstVector128 v1, Vector128Utility::ConstVector128) noexcept { return PERMUTE_PS(v1, Shuffle); }
		};

		// Fast path for permutes that only read from the second vector.
		template<gu::uint32 Shuffle> struct PermuteHelper<Shuffle, true, true, true, true>
		{
			static Vector128 SIMD_CALL_CONVENTION Permute(Vector128Utility::ConstVector128, Vector128Utility::ConstVector128 v2) noexcept { return PERMUTE_PS(v2, Shuffle); }
		};

		// Fast path for permutes that read XY from the first vector, ZW from the second.
		template<gu::uint32 Shuffle> struct PermuteHelper<Shuffle, false, false, true, true>
		{
			static Vector128 SIMD_CALL_CONVENTION Permute(Vector128Utility::ConstVector128 v1, Vector128Utility::ConstVector128 v2) noexcept { return _mm_shuffle_ps(v1, v2, Shuffle); }
		};

		// Fast path for permutes that read XY from the second vector, ZW from the first.
		template<gu::uint32 Shuffle> struct PermuteHelper<Shuffle, true, true, false, false>
		{
			static Vector128 SIMD_CALL_CONVENTION Permute(Vector128Utility::ConstVector128 v1, Vector128Utility::ConstVector128 v2) noexcept { return _mm_shuffle_ps(v2, v1, Shuffle); }
		};
	}


	template<gu::uint32 permuteX, gu::uint32 permuteY, gu::uint32 permuteZ, gu::uint32 permuteW>
	inline Vector128 SIMD_CALL_CONVENTION VectorPermute(Vector128Utility::ConstVector128 v1, Vector128Utility::ConstVector128 v2) noexcept
	{
		static_assert(permuteX <= 7, "PermuteX template parameter out of range");
		static_assert(permuteY <= 7, "PermuteY template parameter out of range");
		static_assert(permuteZ <= 7, "PermuteZ template parameter out of range");
		static_assert(permuteW <= 7, "PermuteW template parameter out of range");

	#if PLATFORM_CPU_INSTRUCTION_SSE && !defined(_XM_NO_INTRINSICS_)
		constexpr gu::uint32 shuffle = _MM_SHUFFLE(permuteW & 3, permuteZ & 3, permuteY & 3, permuteX & 3);

		constexpr bool WhichX = permuteX > 3;
		constexpr bool WhichY = permuteY > 3;
		constexpr bool WhichZ = permuteZ > 3;
		constexpr bool WhichW = permuteW > 3;

		return internal::PermuteHelper<shuffle, WhichX, WhichY, WhichZ, WhichW>::Permute(v1, v2);
	#else
		return Vector128Utility::Permute(v1, v2, permuteX, permuteY, permuteZ, permuteW);
	#endif
	}

	// Special-case permute templates
	template<> constexpr Vector128 SIMD_CALL_CONVENTION VectorPermute<0, 1, 2, 3>(Vector128Utility::ConstVector128 V1, Vector128Utility::ConstVector128) noexcept { return V1; }
	template<> constexpr Vector128 SIMD_CALL_CONVENTION VectorPermute<4, 5, 6, 7>(Vector128Utility::ConstVector128, Vector128Utility::ConstVector128 V2) noexcept { return V2; }

	template<> inline Vector128 SIMD_CALL_CONVENTION VectorPermute<0, 1, 4, 5>(Vector128Utility::ConstVector128 V1, Vector128Utility::ConstVector128 V2) noexcept { return _mm_movelh_ps(V1, V2); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorPermute<6, 7, 2, 3>(Vector128Utility::ConstVector128 V1, Vector128Utility::ConstVector128 V2) noexcept { return _mm_movehl_ps(V1, V2); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorPermute<0, 4, 1, 5>(Vector128Utility::ConstVector128 V1, Vector128Utility::ConstVector128 V2) noexcept { return _mm_unpacklo_ps(V1, V2); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorPermute<2, 6, 3, 7>(Vector128Utility::ConstVector128 V1, Vector128Utility::ConstVector128 V2) noexcept { return _mm_unpackhi_ps(V1, V2); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorPermute<2, 3, 6, 7>(Vector128Utility::ConstVector128 V1, Vector128Utility::ConstVector128 V2) noexcept { return _mm_castpd_ps(_mm_unpackhi_pd(_mm_castps_pd(V1), _mm_castps_pd(V2))); }
	#pragma endregion Permute
	#pragma region Setter
	/****************************************************************************
	*                       Zero
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Zero()noexcept
	*
	*  @brief     �[������������Vector128��Ԃ��֐�
	*
	*  @param[in] void
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Zero() noexcept
	{
		return _mm_setzero_ps(); // 4�̒P���x���������_�l���N���A����
	}

	/****************************************************************************
	*                      TrueIntMask
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::TrueIntMask() noexcept
	*
	*  @brief     �S�Ẵr�b�g��true (1)�ɐݒ肳��Ă���Vector128��Ԃ�
	*
	*  @param[in] void
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::TrueIntMask() noexcept
	{
		return _mm_castsi128_ps(_mm_set1_epi32(-1));
	}

	/****************************************************************************
	*                      FalseIntMask
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::FalseIntMask() noexcept
	*
	*  @brief     �S�Ẵr�b�g��false (0)�ɐݒ肳��Ă���Vector128��Ԃ�
	*
	*  @param[in] void
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::FalseIntMask() noexcept
	{
		return _mm_setzero_ps();
	}

	/****************************************************************************
	*                       Set
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Set(const float x, const float y, const float z, const float w) noexcept
	*
	*  @brief     float4����Vector128�N���X�𐶐�����
	*
	*  @param[in] const float x
	*  @param[in] const float y
	*  @param[in] const float z
	*  @param[in] const float w
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Set(const float x, const float y, const float z, const float w) noexcept
	{
		// 4�̒P���x���������_�l��ݒ肷��
		// ���ӓ_�Ƃ���, set_ps�̈����͌�납�珇��vector128�̐擪�Ɉڂ��Ă���.(���g���G���f�B�A��)
		return _mm_set_ps(w, z, y, x);
	}

	/****************************************************************************
	*                       Set
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Set(const float value) noexcept
	*
	*  @brief     float1����Vector128�N���X�𐶐�����
	*
	*  @param[in] const float value
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Set(const float value) noexcept
	{
		return _mm_set_ps1(value); // �����l�ŏ�����
	}

	/****************************************************************************
	*                       SetInt
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Set(const float x, const float y, const float z, const float w) noexcept
	*
	*  @brief     int4����Vector128�N���X�𐶐�����
	*
	*  @param[in] const gu::uint32 x
	*  @param[in] const gu::uint32 y
	*  @param[in] const gu::uint32 z
	*  @param[in] const gu::uint32 w
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SetInt(const gu::uint32 x, const gu::uint32 y, const gu::uint32 z, const gu::uint32 w) noexcept
	{
		__m128i intVector = _mm_set_epi32(static_cast<gu::int32>(w), static_cast<gu::int32>(z), static_cast<gu::int32>(y), static_cast<gu::int32>(x));
		return _mm_castsi128_ps(intVector);
	}

	/****************************************************************************
	*                       SetX
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION SetX(ConstVector128 vector, const float x) noexcept
	*
	*  @brief     Float���g����Vector��X�ɒl��������
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] const float x
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SetX(ConstVector128 vector, const float x) noexcept
	{
		Vector128 result = _mm_set_ss(x);     // (x, 0, 0, 0)
		result = _mm_move_ss(vector, result); // (x, v.y, v.z, v.w)
		return result;
	}

	/****************************************************************************
	*                       SetY
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION SetY(ConstVector128 vector, const float y) noexcept
	*
	*  @brief     Float���g����Vector��Y�ɒl��������
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] const float y
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SetY(ConstVector128 vector, const float y) noexcept
	{
		// y��x����������
		Vector128 result = _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(3, 2, 0, 1));
		// ���͂��x�N�g���ɕϊ�
		Vector128 temp = _mm_set_ss(y);
		// x�̗v�f��u��������
		result = _mm_move_ss(result, temp);
		// y��x����������
		result = _mm_shuffle_ps(result, result, _MM_SHUFFLE(3, 2, 0, 1));
		return result;
	}

	/****************************************************************************
	*                       SetZ
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION SetZ(ConstVector128 vector, const float z) noexcept
	*
	*  @brief     Float���g����Vector��Z�ɒl��������
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] const float z
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SetZ(ConstVector128 vector, const float z) noexcept
	{
		// z��x����������
		Vector128 result = _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(3, 0, 1, 2));
		// ���͂��x�N�g���ɕϊ�
		Vector128 temp = _mm_set_ss(z);
		// x�̗v�f��u��������
		result = _mm_move_ss(result, temp);
		// z��x����������
		result = _mm_shuffle_ps(result, result, _MM_SHUFFLE(3, 0, 1, 2));
		return result;
	}

	/****************************************************************************
	*                       SetW
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION SetW(ConstVector128 vector, const float w) noexcept
	*
	*  @brief     Float���g����Vector��W�ɒl��������
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] const float w
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SetW(ConstVector128 vector, const float w) noexcept
	{
		// w��x����������
		Vector128 result = _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(0, 2, 1, 3));
		// ���͂��x�N�g���ɕϊ�
		Vector128 temp = _mm_set_ss(w);
		// x�̗v�f��u��������
		result = _mm_move_ss(result, temp);
		// w��x����������
		result = _mm_shuffle_ps(result, result, _MM_SHUFFLE(0, 2, 1, 3));
		return result;
	}

	/****************************************************************************
	*                       StoreFloat
	****************************************************************************/
	/* @fn        inline void SIMD_CALL_CONVENTION Vector128Utility::StoreFloat(float* destination, ConstVector128 source) noexcept
	*
	*  @brief     Vector128�����Ƃ�Float�z��ɑ�����܂�
	*
	*  @param[in] float* destination
	*  @param[in] ConstVector128 source
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline void SIMD_CALL_CONVENTION Vector128Utility::StoreFloat(float* destination, ConstVector128 source) noexcept
	{
		Check(destination);
		_mm_store_ss(destination, source);
	}

	/****************************************************************************
	*                       StoreFloat2
	****************************************************************************/
	/* @fn        inline void SIMD_CALL_CONVENTION Vector128Utility::StoreFloat2(float* destination, ConstVector128 source) noexcept
	*
	*  @brief     Vector128�����Ƃ�Float�z��ɑ�����܂�
	*
	*  @param[in] float* destination
	*  @param[in] ConstVector128 source
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline void SIMD_CALL_CONVENTION Vector128Utility::StoreFloat2(float* destination, ConstVector128 source) noexcept
	{
		Check(destination);
		_mm_store_sd(reinterpret_cast<double*>(destination), _mm_castps_pd(source));
	}

	/****************************************************************************
	*                       StoreFloat3
	****************************************************************************/
	/* @fn        inline void SIMD_CALL_CONVENTION Vector128Utility::StoreFloat3(float* destination, ConstVector128 source) noexcept
	*
	*  @brief     Vector128�����Ƃ�Float�z��ɑ�����܂�
	*
	*  @param[in] float* destination
	*  @param[in] ConstVector128 source
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline void SIMD_CALL_CONVENTION Vector128Utility::StoreFloat3(float* destination, ConstVector128 source) noexcept
	{
		Check(destination);
		_mm_store_sd(reinterpret_cast<double*>(destination), _mm_castps_pd(source));
		__m128 z = PERMUTE_PS(source, _MM_SHUFFLE(2, 2, 2, 2));
		_mm_store_ss(&destination[2], z);
	}

	/****************************************************************************
	*                       StoreFloat4
	****************************************************************************/
	/* @fn        inline void SIMD_CALL_CONVENTION Vector128Utility::StoreFloat4(float* destination, ConstVector128 source) noexcept
	*
	*  @brief     Vector128�����Ƃ�Float�z��ɑ�����܂�
	*
	*  @param[in] float* destination
	*  @param[in] ConstVector128 source
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline void SIMD_CALL_CONVENTION Vector128Utility::StoreFloat4(float* destination, ConstVector128 source) noexcept
	{
		Check(destination);
		return _mm_storeu_ps(destination, source);
	}

	/****************************************************************************
	*                       SplatX
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatX(ConstVector128 vector) noexcept
	*
	*  @brief     X�̗v�f�Ɋ�Â���, �S�Ă̗v�f�ɓ����l��������Vector128�N���X���쐬����
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatX(ConstVector128 vector) noexcept
	{
		return _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(0, 0, 0, 0));
	}

	/****************************************************************************
	*                       SplatY
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatY(ConstVector128 vector) noexcept
	*
	*  @brief     Y�̗v�f�Ɋ�Â���, �S�Ă̗v�f�ɓ����l��������Vector128�N���X���쐬����
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatY(ConstVector128 vector) noexcept
	{
		return _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(1, 1, 1, 1));
	}

	/****************************************************************************
	*                       SplatZ
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatZ(ConstVector128 vector) noexcept
	*
	*  @brief     Z�̗v�f�Ɋ�Â���, �S�Ă̗v�f�ɓ����l��������Vector128�N���X���쐬����
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatZ(ConstVector128 vector) noexcept
	{
		return _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(2, 2, 2, 2));
	}

	/****************************************************************************
	*                       SplatW
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatZ(ConstVector128 vector) noexcept
	*
	*  @brief     W�̗v�f�Ɋ�Â���, �S�Ă̗v�f�ɓ����l��������Vector128�N���X���쐬����
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatW(ConstVector128 vector) noexcept
	{
		return _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(3, 3, 3, 3));
	}

	/****************************************************************************
	*                       SplatOne
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatOne(ConstVector128 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��1��������Vector128�N���X���쐬����
	*
	*  @param[in] void
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatOne() noexcept
	{
		return VECTOR_128F_ONE;
	}

	/****************************************************************************
	*                       SplatInfinity
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatInfinity() noexcept
	*
	*  @brief     W�̗v�f�Ɋ�Â���, �S�Ă̗v�f��INF��������Vector128�N���X���쐬����
	*
	*  @param[in] void
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatInfinity() noexcept
	{
		return VECTOR_128F_INFINITY;
	}

	/****************************************************************************
	*                       SplatEpsilon
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatEpsilon() noexcept
	*
	*  @brief     W�̗v�f�Ɋ�Â���, �S�Ă̗v�f��1.192092896e-7f��������Vector128�N���X���쐬����
	*
	*  @param[in] void
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatEpsilon() noexcept
	{
		return VECTOR_128F_EPSILON;
	}

	/****************************************************************************
	*                       SplatSignMask
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatSignMask() noexcept
	*
	*  @brief     W�̗v�f�Ɋ�Â���, �S�Ă̗v�f��-0.0f (0x80000000)��������Vector128�N���X���쐬����
	*
	*  @param[in] void
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatSignMask() noexcept
	{
		__m128i vector = _mm_set1_epi32(static_cast<int>(0x80000000U));
		return _mm_castsi128_ps(vector);
	}

	/****************************************************************************
	*                       Swizzle
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Swizzle(ConstVector128 vector,
				  const gu::uint32 xIndex, const gu::uint32 yIndex,
				  const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept
	*
	*  @brief     �x�N�g������ёւ��܂�.index���w�肷��Ƃ���index�ɂ�����Vector4�^���擾
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Swizzle(ConstVector128 vector,
		const gu::uint32 xIndex, const gu::uint32 yIndex,
		const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept
	{
		const auto pointer = reinterpret_cast<const gu::uint32*>(&vector);

		Vector128 result = {};
		const auto work = reinterpret_cast<gu::uint32*>(&result);

		work[0] = pointer[xIndex];
		work[1] = pointer[yIndex];
		work[2] = pointer[zIndex];
		work[3] = pointer[wIndex];

		return result;
	}

	/****************************************************************************
	*                       Permute
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Permute(ConstVector128 a, ConstVector128 b,
				  const gu::uint32 xIndex, const gu::uint32 yIndex,
				  const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept
	*
	*  @brief     2�̃x�N�g������v�f�𒊏o��, �V�����x�N�g�����쐬����
	*             0�`3 : a�����x, y, z, w
	*             4�`7 : b�����x, y, z, w
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Permute(ConstVector128 a, ConstVector128 b,
		const gu::uint32 xIndex, const gu::uint32 yIndex,
		const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept
	{
		const gu::uint32* pointer[2] = {};
		pointer[0] = reinterpret_cast<const gu::uint32*>(&a);
		pointer[1] = reinterpret_cast<const gu::uint32*>(&b);

		Vector128 result = {};
		auto pWork = reinterpret_cast<gu::uint32*>(&result);

		const gu::uint32 i0 = xIndex & 3;
		const gu::uint32 vi0 = xIndex >> 2;
		pWork[0] = pointer[vi0][i0];

		const gu::uint32 i1 = yIndex & 3;
		const gu::uint32 vi1 = yIndex >> 2;
		pWork[1] = pointer[vi1][i1];

		const gu::uint32 i2 = zIndex & 3;
		const gu::uint32 vi2 = zIndex >> 2;
		pWork[2] = pointer[vi2][i2];

		const gu::uint32 i3 = wIndex & 3;
		const gu::uint32 vi3 = wIndex >> 2;
		pWork[3] = pointer[vi3][i3];

		return result;
	}

	/****************************************************************************
	*                       Select
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Select(ConstVector128 left, ConstVector128 right, ConstVector128 control) noexcept
	*
	*  @brief     2�̃x�N�g������v�f�𒊏o��, �V�����x�N�g�����쐬����
	*             control vector��0�̏ꍇ��left����, ��0�̏ꍇ��right���炻�ꂼ�ꒊ�o���s���܂�
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Select(ConstVector128 left, ConstVector128 right, ConstVector128 control) noexcept
	{
		Vector128 temp1 = _mm_andnot_ps(control, left);
		Vector128 temp2 = _mm_and_ps(right, control);
		return _mm_or_ps(temp1, temp2);
	}
	#pragma endregion Setter

	#pragma region Getter
	/****************************************************************************
	*                       GetX
	****************************************************************************/
	/* @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::GetX(ConstVector128 vector) noexcept
	*
	*  @brief     FPU���W�X�^�Ɋi�[���ꂽX�̗v�f�����o��
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::GetX(ConstVector128 vector) noexcept
	{
		return _mm_cvtss_f32(vector);
	}

	/****************************************************************************
	*                       GetY
	****************************************************************************/
	/* @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::GetY(ConstVector128 vector) noexcept
	*
	*  @brief     FPU���W�X�^�Ɋi�[���ꂽY�̗v�f�����o��
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::GetY(ConstVector128 vector) noexcept
	{
		return _mm_cvtss_f32(_mm_shuffle_ps(vector, vector, _MM_SHUFFLE(1, 1, 1, 1)));
	}

	/****************************************************************************
	*                       GetZ
	****************************************************************************/
	/* @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::GetZ(ConstVector128 vector) noexcept
	*
	*  @brief     FPU���W�X�^�Ɋi�[���ꂽZ�̗v�f�����o��
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::GetZ(ConstVector128 vector) noexcept
	{
		return _mm_cvtss_f32(_mm_shuffle_ps(vector, vector, _MM_SHUFFLE(2, 2, 2, 2)));
	}

	/****************************************************************************
	*                       GetW
	****************************************************************************/
	/* @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::GetZ(ConstVector128 vector) noexcept
	*
	*  @brief     FPU���W�X�^�Ɋi�[���ꂽW�̗v�f�����o��
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::GetW(ConstVector128 vector) noexcept
	{
		return _mm_cvtss_f32(_mm_shuffle_ps(vector, vector, _MM_SHUFFLE(3, 3, 3, 3)));
	}

	/****************************************************************************
	*                       LoadFloat
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LoadFloat(const float* source) noexcept
	*
	*  @brief     Float�̔z����g���Ċi�[���� (*source, 0.0, 0.0, 0.0f)
	*
	*  @param[in] const float* source
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LoadFloat(const float* source) noexcept
	{
		Check(source);
		return _mm_load_ss(source);
	}

	/****************************************************************************
	*                       LoadFloat2
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LoadFloat2(const float* source) noexcept
	*
	*  @brief     Float�̔z����g���Ċi�[���� (source->x, source->y, 0.0, 0.0f)
	*
	*  @param[in] const float* source
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LoadFloat2(const float* source) noexcept
	{
		Check(source);
		return _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(source)));
	}

	/****************************************************************************
	*                       LoadFloat3
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LoadFloat3(const float* source) noexcept
	*
	*  @brief     Float�̔z����g���Ċi�[���� (source->x, source->y, source->z, 0.0f)
	*
	*  @param[in] const float* source
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LoadFloat3(const float* source) noexcept
	{
		Check(source);
		__m128 xy = _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(source)));
		__m128 z  = _mm_load_ss(&source[2]);
		return _mm_movelh_ps(xy, z);
	}

	/****************************************************************************
	*                       LoadFloat3
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LoadFloat3(const float* source) noexcept
	*
	*  @brief     Float�̔z����g���Ċi�[���� (source->x, source->y, source->z, source->w)
	*
	*  @param[in] const float* source
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LoadFloat4(const float* source) noexcept
	{
		Check(source);
		return _mm_loadu_ps(source);
	}

	#pragma endregion Getter

	#pragma region Operator
	/****************************************************************************
	*                       Add
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Add(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     2�̃x�N�g���̊e�v�f���m�̘a���Z�o���܂�. (left + right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Add(ConstVector128 left, ConstVector128 right) noexcept
	{
		return _mm_add_ps(left, right);
	}

	/****************************************************************************
	*                       Subtract
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Subtract(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     2�̃x�N�g���̊e�v�f���m�̍����Z�o���܂�. (left - right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Subtract(ConstVector128 left, ConstVector128 right) noexcept
	{
		return _mm_sub_ps(left, right);
	}

	/****************************************************************************
	*                       Multiply
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Multiply(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     2�̃x�N�g���̊e�v�f���m�̍����Z�o���܂�. (left * right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Multiply(ConstVector128 left, ConstVector128 right) noexcept
	{
		return _mm_mul_ps(left, right);
	}

	/****************************************************************************
	*                       Divide
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Divide(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     2�̃x�N�g���̊e�v�f���m�̍����Z�o���܂�. (left / right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Divide(ConstVector128 left, ConstVector128 right) noexcept
	{
		return _mm_div_ps(left, right);
	}

	/****************************************************************************
	*                       Mod
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Mod(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     2�̃x�N�g���̊e�v�f���m�̂��܂���Z�o���܂�. (left % right)
	*             left % right = left - right * truncate(left / right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Mod(ConstVector128 left, ConstVector128 right) noexcept
	{
		Vector128 result = _mm_div_ps(left, right);
		result = Truncate(result);
		return NegativeMultiplySubtract(result, right, left); // left - result * right
	}

	/****************************************************************************
	*                      NegativeMultiplySubstract
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NegativeMultiplySubtract(ConstVector128 first, ConstVector128 second, ConstVector128 third)
	*
	*  @brief     subtractLeft - (multiplyLeft * multiplyRight)���Z�o���܂�.
	*
	*  @param[in] ConstVector128 multiplyLeft
	*  @param[in] ConstVector128 multiplyRight
	*  @param[in] ConstVector128 subtractLeft
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NegativeMultiplySubtract(ConstVector128 multiplyLeft, ConstVector128 multiplyRight, ConstVector128 subtractLeft) noexcept
	{
		return _mm_sub_ps(subtractLeft, _mm_mul_ps(multiplyLeft, multiplyRight));
	}

	/****************************************************************************
	*                      MultiplyAdd
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::MultiplyAdd(ConstVector128 first, ConstVector128 second, ConstVector128 third)
	*
	*  @brief     multiplyLeft * multiplyRight + add���Z�o���܂�.
	*
	*  @param[in] ConstVector128 multiplyLeft
	*  @param[in] ConstVector128 multiplyRight
	*  @param[in] ConstVector128 add
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::MultiplyAdd(ConstVector128 multiplyLeft, ConstVector128 multiplyRight, ConstVector128 add) noexcept
	{
		return _mm_add_ps(_mm_mul_ps(multiplyLeft, multiplyRight), add);
	}

	/****************************************************************************
	*                       Reciprocal
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Reciprocal(ConstVector128 vector) noexcept
	*
	*  @brief     2�̃x�N�g���̊e�v�f���m�̋t�����Z�o���܂�. (1.0f / vector)
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Reciprocal(ConstVector128 vector) noexcept
	{
		return _mm_div_ps(VECTOR_128F_ONE, vector);
	}

	/****************************************************************************
	*                       Scale
	****************************************************************************/
	/* @fn       inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Scale(ConstVector128 vector, const float scale) noexcept
	*
	*  @brief     float�l���g���Ċe�v�f�Ɋ|���܂� scale * vector
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] const float scale
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Scale(ConstVector128 vector, const float scale) noexcept
	{
		Vector128 result = _mm_set_ps1(scale); // (scale, scale, scale, scale)
		return _mm_mul_ps(result, vector);
	}

	/****************************************************************************
	*                       EqualVector2
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualVector2(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     ���2�����x�N�g�������������ǂ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualVector2(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂��ē������ꍇ��1��Ԃ��x�N�g��
		Vector128 equalMaskVector = _mm_cmpeq_ps(left, right); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		// w, z�͖�������悤�ɂ���
		return (_mm_movemask_ps(equalMaskVector) & 0b0011) == 0b0011; // �S������0b1111�Ƃ����r�b�g�t���O������
	}

	/****************************************************************************
	*                       EqualVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualVector3(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     ���3�����x�N�g�������������ǂ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualVector3(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂��ē������ꍇ��1��Ԃ��x�N�g��
		Vector128 equalMaskVector = _mm_cmpeq_ps(left, right); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		// w�͖�������悤�ɂ���
		return (_mm_movemask_ps(equalMaskVector) & 0b0111) == 0b0111; // �S������0b1111�Ƃ����r�b�g�t���O������
	}

	/****************************************************************************
	*                       EqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualVector4(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     ���4�����x�N�g�������������ǂ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualVector4(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂��ē������ꍇ��1��Ԃ��x�N�g��
		Vector128 notEqualMaskVector = _mm_cmpeq_ps(left, right); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		return (_mm_movemask_ps(notEqualMaskVector) & 0b1111) == 0b1111; // �S������0b1111�Ƃ����r�b�g�t���O������
	}

	/****************************************************************************
	*                       EqualVectorEach
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     ���4�����x�N�g�����e�v�f���Ƃɓ��������ǂ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::EqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	{
		return _mm_cmpeq_ps(left, right);
	}

	/****************************************************************************
	*                       NotEqualVector2
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector2(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     ���2�����x�N�g�����������Ȃ����ǂ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector2(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂��ē������ꍇ��1��Ԃ��x�N�g��
		Vector128 equalMaskVector = _mm_cmpeq_ps(left, right); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		// ��ł��t���O�������Ă��Ȃ������瓙�����Ȃ�����
		// ������w, z�͖���
		return (_mm_movemask_ps(equalMaskVector) & 0b0011) != 0b0011;
	}

	/****************************************************************************
	*                       NotEqualVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector4(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     ���3�����x�N�g�����������Ȃ����ǂ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector3(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂��ē������ꍇ��1��Ԃ��x�N�g��
		Vector128 equalMaskVector = _mm_cmpeq_ps(left, right); // �S�����Ȃ�{1, 1, 1, 1}, w�����قȂ�Ȃ�{0, 1, 1, 1}

		// ��ł��t���O�������Ă��Ȃ������瓙�����Ȃ�����
		// ������w�͖���
		return (_mm_movemask_ps(equalMaskVector) & 0b0111) != 0b0111;
	}

	/****************************************************************************
	*                       NotEqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector4(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     ���4�����x�N�g�����������Ȃ����ǂ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector4(ConstVector128 left, ConstVector128 right) noexcept
	{
		Vector128 equalMaskVector = _mm_cmpneq_ps(left, right); // �S�s�����Ȃ�{1, 1, 1, 1}, w�����������Ȃ�{0, 1, 1, 1}

		return (_mm_movemask_ps(equalMaskVector)) != 0;
	}

	/****************************************************************************
	*                       NotEqualVectorEach
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     ���4�����x�N�g�����e�v�f���Ƃɓ������Ȃ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NotEqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	{
		return _mm_cmpneq_ps(left, right);
	}

	/****************************************************************************
	*                       EqualAsIntVector2
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualAsIntVector2(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     ��̃x�N�g�������������ǂ����𒲂ׂ܂�.
	*             ��r���ɂ͕����Ȃ�����(U32)�Ƃ��Ĉ����܂�.
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualAsIntVector2(ConstVector128 left, ConstVector128 right) noexcept
	{
		__m128i temp = _mm_cmpeq_epi32(_mm_castps_si128(left), _mm_castps_si128(right));
		return (_mm_movemask_ps(_mm_castsi128_ps(temp)) & 0b0011) == 0b0011;
	}

	/****************************************************************************
	*                       EqualAsIntVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualAsIntVector3(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     ��̃x�N�g�������������ǂ����𒲂ׂ܂�.
	*             ��r���ɂ͕����Ȃ������Ƃ��Ĉ����܂�.
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualAsIntVector3(ConstVector128 left, ConstVector128 right) noexcept
	{
		__m128i temp = _mm_cmpeq_epi32(_mm_castps_si128(left), _mm_castps_si128(right));
		return (_mm_movemask_ps(_mm_castsi128_ps(temp)) & 0b0111) == 0b0111;
	}

	/****************************************************************************
	*                       EqualAsIntVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualAsIntVector4(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     ��̃x�N�g�������������ǂ����𒲂ׂ܂�.
	*             ��r���ɂ͕����Ȃ������Ƃ��Ĉ����܂�.
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualAsIntVector4(ConstVector128 left, ConstVector128 right) noexcept
	{
		__m128i temp = _mm_cmpeq_epi32(_mm_castps_si128(left), _mm_castps_si128(right));
		return _mm_movemask_ps(_mm_castsi128_ps(temp)) == 0b1111;
	}

	/****************************************************************************
	*                       EqualAsIntVectorEach
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualAsIntVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     ��̃x�N�g�������������ǂ��������ꂼ��̗v�f�Œ��ׂ܂�.
	*             ��r���ɂ͕����Ȃ������Ƃ��Ĉ����܂�.
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::EqualAsIntVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	{
		__m128i result = _mm_cmpeq_epi32(_mm_castps_si128(left), _mm_castps_si128(right));
		return _mm_castsi128_ps(result);
	}

	/****************************************************************************
	*                       NotEqualVector2
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualAsIntVector2(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     ��̃x�N�g������ł��������Ȃ����ǂ����𒲂ׂ܂�.
	*            ��r���ɂ͕����Ȃ�����(U32)�Ƃ��Ĕ�r���܂�.
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualAsIntVector2(ConstVector128 left, ConstVector128 right) noexcept
	{
		__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(left), _mm_castps_si128(right));
		return ((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 0b0011) != 0b0011);
	}

	/****************************************************************************
	*                       NotEqualVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualAsIntVector3(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     ���3�����x�N�g�����������Ȃ����ǂ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualAsIntVector3(ConstVector128 left, ConstVector128 right) noexcept
	{
		__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(left), _mm_castps_si128(right));
		return ((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 0b0111) != 0b0111);
	}

	/****************************************************************************
	*                       NotEqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector4(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     ���4�����x�N�g�����������Ȃ����ǂ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualAsIntVector4(ConstVector128 left, ConstVector128 right) noexcept
	{
		__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(left), _mm_castps_si128(right));
		return (_mm_movemask_ps(_mm_castsi128_ps(vTemp)) != 0b1111);
	}

	/****************************************************************************
	*                       NotEqualVector
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     ���4�����x�N�g�����e�v�f���Ƃɓ������Ȃ����𒲂ׂ܂�
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NotEqualAsIntVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	{
		__m128i v = _mm_cmpeq_epi32(_mm_castps_si128(left), _mm_castps_si128(right));
		return _mm_xor_ps(_mm_castsi128_ps(v), VECTOR_128U_NEGATIVE_ONE_MASK);
	}

	/****************************************************************************
	*                       NearEqualVector2
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector4(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept
	*
	*  @brief     ���͈͓��Œl�����������𒲂ׂ� (left - epsilon <= right <= left + epsilon)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*  @param[in] ConstVector128 epsilon
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::NearEqualVector2(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept
	{
		// �����̓��o
		Vector128 delta = _mm_sub_ps(left, right);

		// ��Βl�̍����̎擾
		Vector128 result = _mm_setzero_ps();
		result = _mm_sub_ps(result, delta);
		result = _mm_max_ps(result, delta);
		result = _mm_cmple_ps(result, epsilon);

		// z��w�͖�������, x��y�ɒ��ڂ���
		return ((_mm_movemask_ps(result) & 0b0011) == 0b0011);
	}

	/****************************************************************************
	*                       NearEqualVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector3(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept
	*
	*  @brief     ���͈͓��Œl�����������𒲂ׂ� (left - epsilon <= right <= left + epsilon)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*  @param[in] ConstVector128 epsilon
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::NearEqualVector3(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept
	{
		// �����̓��o
		Vector128 delta = _mm_sub_ps(left, right);

		// ��Βl�̍����̎擾
		Vector128 result = _mm_setzero_ps();
		result = _mm_sub_ps(result, delta);
		result = _mm_max_ps(result, delta);
		result = _mm_cmple_ps(result, epsilon);

		// z��w�͖�������, x��y�ɒ��ڂ���
		return ((_mm_movemask_ps(result) & 0b0111) == 0b0111);
	}

	/****************************************************************************
	*                       NearEqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector4(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept
	*
	*  @brief     ���͈͓��Œl�����������𒲂ׂ� (left - epsilon <= right <= left + epsilon)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*  @param[in] ConstVector128 epsilon
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::NearEqualVector4(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept
	{
		// �����̓��o
		Vector128 delta = _mm_sub_ps(left, right);

		// ��Βl�̍����̎擾
		Vector128 result = _mm_setzero_ps();
		result = _mm_sub_ps(result, delta);
		result = _mm_max_ps(result, delta);
		result = _mm_cmple_ps(result, epsilon);

		// z��w�͖�������, x��y�ɒ��ڂ���
		return ((_mm_movemask_ps(result) & 0b1111) == 0b1111);
	}

	/****************************************************************************
	*                       NearEqualVectorEach
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVectorEach(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept
	*
	*  @brief     ���͈͓��Œl�����������𒲂ׂ� (left - epsilon <= right <= left + epsilon)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*  @param[in] ConstVector128 epsilon
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NearEqualVectorEach(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept
	{
		// ����
		Vector128 delta = _mm_sub_ps(left, right);

		// ��Βl�̎擾
		Vector128 temp = _mm_setzero_ps();
		temp = _mm_sub_ps(temp, delta);
		temp = _mm_max_ps(temp, delta);
		temp = _mm_cmple_ps(temp, epsilon);
		return temp;
	}

	/****************************************************************************
	*                       GreaterVector2
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterVector2(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector2�^�ɂ����č��̕����S�Ă̗v�f�ɂ����đ傫�����𒲂ׂ܂� (left > right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterVector2(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left > right�̏ꍇ��1��Ԃ��x�N�g��
		Vector128 equalMaskVector = _mm_cmpgt_ps(left, right); // �S�đ傫���Ȃ�{1, 1, 1, 1}, w�����������Ȃ�{0, 1, 1, 1}

		// w, z�͖�������
		return (_mm_movemask_ps(equalMaskVector) & 0b0011) == 0b0011;
	}

	/****************************************************************************
	*                       GreaterVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterVector3(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector3�^�ɂ����č��̕����S�Ă̗v�f�ɂ����đ傫�����𒲂ׂ܂� (left > right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterVector3(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left > right�̏ꍇ��1��Ԃ��x�N�g��
		Vector128 equalMaskVector = _mm_cmpgt_ps(left, right); // �S�đ傫���Ȃ�{1, 1, 1, 1}, w�����������Ȃ�{0, 1, 1, 1}

		// w�͖�������
		return (_mm_movemask_ps(equalMaskVector) & 0b0111) == 0b0111;
	}

	/****************************************************************************
	*                       GreaterVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterVector4(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector4�^�ɂ����č��̕����S�Ă̗v�f�ɂ����đ傫�����𒲂ׂ܂� (left > right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterVector4(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left > right�̏ꍇ��1��Ԃ��x�N�g��
		Vector128 equalMaskVector = _mm_cmpgt_ps(left, right); // �S�đ傫���Ȃ�{1, 1, 1, 1}, w�����������Ȃ�{0, 1, 1, 1}

		return _mm_movemask_ps(equalMaskVector) == 0b1111;
	}

	/****************************************************************************
	*                       GreaterVectorEach
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterVector4(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector4�^�ɂ����č��̕����S�Ă̗v�f�ɂ����đ傫�����𒲂ׂ܂� (left > right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::GreaterVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	{
		return _mm_cmpgt_ps(left, right);
	}


	/****************************************************************************
	*                       GreaterOrEqualVector2
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterOrEqualVector2(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector2�^�ɂ����č��̕����S�Ă̗v�f�ɂ����đ傫��or���������𒲂ׂ܂� (left >= right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterOrEqualVector2(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left > right�̏ꍇ��1��Ԃ��x�N�g��
		Vector128 equalMaskVector = _mm_cmpge_ps(left, right); // �S�đ傫���Ȃ�{1, 1, 1, 1}, w�����������Ȃ�{0, 1, 1, 1}

		return (_mm_movemask_ps(equalMaskVector) & 0b0011) == 0b0011;
	}

	/****************************************************************************
	*                       GreaterVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterOrEqualVector3(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector3�^�ɂ����č��̕����S�Ă̗v�f�ɂ����đ傫��or���������𒲂ׂ܂� (left >= right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterOrEqualVector3(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left > right�̏ꍇ��1��Ԃ��x�N�g��
		Vector128 equalMaskVector = _mm_cmpge_ps(left, right); // �S�đ傫���Ȃ�{1, 1, 1, 1}, w�����������Ȃ�{0, 1, 1, 1}

		return (_mm_movemask_ps(equalMaskVector) & 0b0111) == 0b0111;
	}

	/****************************************************************************
	*                       GreaterOrEqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterOrEqualVector4(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector4�^�ɂ����č��̕����S�Ă̗v�f�ɂ����đ傫��or���������𒲂ׂ܂� (left >= right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterOrEqualVector4(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left >= right�̏ꍇ��1��Ԃ��x�N�g��
		Vector128 equalMaskVector = _mm_cmpge_ps(left, right); // �S�đ傫��or�������Ȃ�{1, 1, 1, 1}, w�����������Ȃ�{0, 1, 1, 1}

		return _mm_movemask_ps(equalMaskVector) == 0b1111;
	}

	/****************************************************************************
	*                       GreaterOrEqualVectorEach
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::GreaterOrEqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector4�^�ɂ����č��̕����S�Ă̗v�f�ɂ����đ傫��or���������𒲂ׂ܂� (left >= right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::GreaterOrEqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	{
		return _mm_cmpge_ps(left, right);
	}

	/****************************************************************************
	*                       LessVector2
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::LessVector2(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector2�^�ɂ����č��̕����S�Ă̗v�f�ɂ����ď��������𒲂ׂ܂� (left < right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::LessVector2(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left < right�̏ꍇ��1��Ԃ��x�N�g��
		Vector128 equalMaskVector = _mm_cmplt_ps(left, right); // �S�ď������Ȃ�{1, 1, 1, 1}, w�����傫���Ȃ�{0, 1, 1, 1}

		return (_mm_movemask_ps(equalMaskVector) & 0b0011) == 0b0011;
	}

	/****************************************************************************
	*                       LessVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::LessVector3(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector3�^�ɂ����č��̕����S�Ă̗v�f�ɂ����ď��������𒲂ׂ܂� (left < right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::LessVector3(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left > right�̏ꍇ��1��Ԃ��x�N�g��
		Vector128 equalMaskVector = _mm_cmplt_ps(left, right); // �S�đ傫���Ȃ�{1, 1, 1, 1}, w�����������Ȃ�{0, 1, 1, 1}

		// w�͖�������
		return (_mm_movemask_ps(equalMaskVector) & 0b0111) == 0b0111;
	}

	/****************************************************************************
	*                       LessVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::LessVector4(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector4�^�ɂ����č��̕����S�Ă̗v�f�ɂ����ď��������𒲂ׂ܂� (left < right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::LessVector4(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left > right�̏ꍇ��1��Ԃ��x�N�g��
		Vector128 equalMaskVector = _mm_cmplt_ps(left, right); // �S�đ傫���Ȃ�{1, 1, 1, 1}, w�����������Ȃ�{0, 1, 1, 1}

		return _mm_movemask_ps(equalMaskVector) == 0b1111;
	}

	/****************************************************************************
	*                       LessVectorEach
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LessVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector4�^�ɂ����č��̕����S�Ă̗v�f�ɂ����ď��������𒲂ׂ܂� (left < right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LessVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	{
		return _mm_cmpge_ps(left, right);
	}

	/****************************************************************************
	*                       LessOrEqualVector2
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::LessOrEqualVector2(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector2�^�ɂ����č��̕����S�Ă̗v�f�ɂ����ď�����or���������𒲂ׂ܂� (left <= right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::LessOrEqualVector2(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left > right�̏ꍇ��1��Ԃ��x�N�g��
		Vector128 equalMaskVector = _mm_cmple_ps(left, right); // �S�đ傫���Ȃ�{1, 1, 1, 1}, w�����������Ȃ�{0, 1, 1, 1}

		return (_mm_movemask_ps(equalMaskVector) & 0b0011) == 0b0011;
	}

	/****************************************************************************
	*                       LessOrEqualVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::LessOrEqualVector3(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector3�^�ɂ����č��̕����S�Ă̗v�f�ɂ����ď�����or���������𒲂ׂ܂� (left <= right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::LessOrEqualVector3(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left > right�̏ꍇ��1��Ԃ��x�N�g��
		Vector128 equalMaskVector = _mm_cmple_ps(left, right); // �S�đ傫���Ȃ�{1, 1, 1, 1}, w�����������Ȃ�{0, 1, 1, 1}

		return (_mm_movemask_ps(equalMaskVector) & 0b0111) == 0b0111;
	}

	/****************************************************************************
	*                       LessOrEqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::LessOrEqualVector4(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector4�^�ɂ����č��̕����S�Ă̗v�f�ɂ����ď�����or���������𒲂ׂ܂� (left <= right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::LessOrEqualVector4(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, x���ꂼ��ɑ΂���left >= right�̏ꍇ��1��Ԃ��x�N�g��
		Vector128 equalMaskVector = _mm_cmple_ps(left, right); // �S�đ傫��or�������Ȃ�{1, 1, 1, 1}, w�����������Ȃ�{0, 1, 1, 1}

		return _mm_movemask_ps(equalMaskVector) == 0b1111;
	}

	/****************************************************************************
	*                       LessVectorEach
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LessOrEqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector4�^�ɂ����č��̕����S�Ă̗v�f�ɂ����ď�����or���������𒲂ׂ܂� (left <= right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LessOrEqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	{
		return _mm_cmple_ps(left, right);
	}

	/****************************************************************************
	*                       InBoundsVector2
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::InBoundsVector2(ConstVector128 vector, ConstVector128 bounds) noexcept
	*
	*  @brief     �S�Ă̗v�f�𒲂ׂĎw��͈͓��ɂ��邩�ǂ��� -bounds <= vector <= +bounds
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] ConstVector128 bounds 
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::InBoundsVector2(ConstVector128 vector, ConstVector128 bounds) noexcept
	{
		Vector128 lessOrEqual = _mm_cmple_ps(vector, bounds);
		Vector128 negateBound = _mm_mul_ps(bounds, VECTOR_128F_NEGATIVE_ONE);
		negateBound = _mm_cmple_ps(negateBound, vector); // greater or equal
		lessOrEqual = _mm_and_ps(lessOrEqual, negateBound);
		return (_mm_movemask_ps(lessOrEqual) & 0b0011) == 0b0011;
	}

	/****************************************************************************
	*                       InBoundsVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::InBoundsVector3(ConstVector128 vector, ConstVector128 bounds) noexcept
	*
	*  @brief     �S�Ă̗v�f�𒲂ׂĎw��͈͓��ɂ��邩�ǂ��� -bounds <= vector <= +bounds
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] ConstVector128 bounds
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::InBoundsVector3(ConstVector128 vector, ConstVector128 bounds) noexcept
	{
		Vector128 lessOrEqual = _mm_cmple_ps(vector, bounds);
		Vector128 negateBound = _mm_mul_ps(bounds, VECTOR_128F_NEGATIVE_ONE);
		negateBound = _mm_cmple_ps(negateBound, vector); // greater or equal
		lessOrEqual = _mm_and_ps(lessOrEqual, negateBound);
		return (_mm_movemask_ps(lessOrEqual) & 0b0111) == 0b0111;

	}

	/****************************************************************************
	*                       InBoundsVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::InBoundsVector4(ConstVector128 vector, ConstVector128 bounds) noexcept
	*
	*  @brief     �S�Ă̗v�f�𒲂ׂĎw��͈͓��ɂ��邩�ǂ��� -bounds <= vector <= +bounds
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] ConstVector128 bounds
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::InBoundsVector4(ConstVector128 vector, ConstVector128 bounds) noexcept
	{
		Vector128 lessOrEqual = _mm_cmple_ps(vector, bounds);
		Vector128 negateBound = _mm_mul_ps(bounds, VECTOR_128F_NEGATIVE_ONE);
		negateBound = _mm_cmple_ps(negateBound, vector); // greater or equal
		lessOrEqual = _mm_and_ps(lessOrEqual, negateBound);
		return _mm_movemask_ps(lessOrEqual) == 0b1111;
	}

	/****************************************************************************
	*                       InBoundsVectorEach
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::InBoundsVectorEach(ConstVector128 vector, ConstVector128 bounds) noexcept
	*
	*  @brief     �w��͈͓��ɂ��邩�ǂ������e�v�f�Ŕ�r���܂� -bounds <= vector <= +bounds
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] ConstVector128 bounds
	*
	*  @return �@�@bool
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::InBoundsVectorEach(ConstVector128 vector, ConstVector128 bounds) noexcept
	{
		Vector128 lessOrEqual = _mm_cmple_ps(vector, bounds);
		Vector128 negateBound = _mm_mul_ps(bounds, VECTOR_128F_NEGATIVE_ONE);
		negateBound = _mm_cmple_ps(negateBound, vector); // greater or equal
		lessOrEqual = _mm_and_ps(lessOrEqual, negateBound);
		return lessOrEqual;
	}

	/****************************************************************************
	*                       RotateLeft
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RotateLeft(ConstVector128 vector, const gu::uint32 shiftCount) noexcept
	*
	*  @brief     ��������shiftCount�� (3�܂�)�v�f����]����
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] const gu::uint32 shiftCount
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RotateLeft(ConstVector128 vector, const gu::uint32 shiftCount) noexcept
	{
		Check(shiftCount < 4);
		return Swizzle(vector, shiftCount & 3, (shiftCount + 1) & 3, (shiftCount + 2) & 3, (shiftCount + 3) & 3);
	}

	/****************************************************************************
	*                       RotateRight
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RotateRight(ConstVector128 vector, const gu::uint32 shiftCount) noexcept
	*
	*  @brief     �E������shiftCount�� (3�܂�)�v�f����]����
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] const gu::uint32 shiftCount
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RotateRight(ConstVector128 vector, const gu::uint32 shiftCount) noexcept
	{
		Check(shiftCount < 4);
		return Swizzle(vector, (4 - shiftCount) & 3, (5 - shiftCount) & 3, (6 - shiftCount) & 3, (7 - shiftCount) & 3);
	}

	#pragma endregion Operator

	#pragma region Bit
	/****************************************************************************
	*                       AndInt
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::AndInt(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     �r�b�g�P�ʂ̘_���ς̉��Z���s���܂�
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::AndInt(ConstVector128 left, ConstVector128 right) noexcept
	{
		return _mm_and_ps(left, right);
	}

	/****************************************************************************
	*                        OrInt
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::AndInt(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     �r�b�g�P�ʂ̘_���ς̉��Z���s���܂�
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::OrInt(ConstVector128 left, ConstVector128 right) noexcept
	{
		__m128i result = _mm_or_si128(_mm_castps_si128(left), _mm_castps_si128(right));
		return _mm_castsi128_ps(result);
	}

	/****************************************************************************
	*                        NorInt
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NorInt(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     �r�b�g�P�ʂ̘_���ϔے�̉��Z���s���܂�
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NorInt(ConstVector128 left, ConstVector128 right) noexcept
	{
		__m128i result;
		result = _mm_or_si128(_mm_castps_si128(left), _mm_castps_si128(right));
		result = _mm_andnot_si128(result, VECTOR_128U_NEGATIVE_ONE_MASK);
		return _mm_castsi128_ps(result);
	}

	/****************************************************************************
	*                        XorInt
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::AndInt(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     �r�b�g�P�ʂ̔r���I�_���a�̉��Z���s���܂�
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::XorInt(ConstVector128 left, ConstVector128 right) noexcept
	{
		__m128i result = _mm_xor_si128(_mm_castps_si128(left), _mm_castps_si128(right));
		return _mm_castsi128_ps(result);
	}

	#pragma endregion Bit

	#pragma region Math
	/****************************************************************************
	*                       Negate
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Negate(ConstVector128 vector) noexcept
	*
	*  @brief     Vector�̑S�Ă̗v�f�̕����𔻒肵�܂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Negate(ConstVector128 vector) noexcept
	{
		Vector128 result = _mm_setzero_ps();
		return _mm_sub_ps(result, vector);
	}

	/****************************************************************************
	*                      LengthVector2
	****************************************************************************/
	/* @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::LengthVector2(ConstVector128 vector) noexcept
	*
	*  @brief     2���m����(�e�v�f��2��a�����[�g��������Ƃ��̃x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::LengthVector2(ConstVector128 vector) noexcept
	{
		// Dot�ς̌v�Z
		Vector128 squareLength = _mm_mul_ps(vector, vector);

		// y�݂̂̃x�N�g�����擾
		Vector128 temp = PERMUTE_PS(squareLength, _MM_SHUFFLE(1, 1, 1, 1));

		// x + y
		squareLength = _mm_add_ss(squareLength, temp);
		squareLength = PERMUTE_PS(squareLength, _MM_SHUFFLE(0, 0, 0, 0));
		squareLength = _mm_sqrt_ps(squareLength);
		return _mm_cvtss_f32(squareLength);
	}

	/****************************************************************************
	*                     LengthVector3
	****************************************************************************/
	/* @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::NormVector3(ConstVector128 vector) noexcept
	*
	*  @brief     2���m����(�e�v�f��2��a�����[�g��������Ƃ��̃x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::LengthVector3(ConstVector128 vector) noexcept
	{
		// Dot�ς̌v�Z
		Vector128 squareLength = _mm_mul_ps(vector, vector);

		// z��y��v�f�ɂ��x�N�g���쐬
		Vector128 temp = PERMUTE_PS(squareLength, _MM_SHUFFLE(1, 2, 1, 2));

		// x + z, y
		squareLength = _mm_add_ss(squareLength, temp);

		// y, y, y, y
		temp = PERMUTE_PS(temp, _MM_SHUFFLE(1, 1, 1, 1));

		// x + z + y, ?? , ??, ??
		squareLength = _mm_add_ss(squareLength, temp);

		// splat the length squared
		squareLength = PERMUTE_PS(squareLength, _MM_SHUFFLE(0, 0, 0, 0));

		// �m�����̎擾
		squareLength = _mm_sqrt_ps(squareLength);

		return _mm_cvtss_f32(squareLength);
	}

	/****************************************************************************
	*                      NLengthVector4
	****************************************************************************/
	/* @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::LengthVector4(ConstVector128 vector) noexcept
	*
	*  @brief     2���m����(�e�v�f��2��a�����[�g��������Ƃ��̃x�N�g���̑傫��)���Z�o
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::LengthVector4(ConstVector128 vector) noexcept
	{
		// Dot�ς̌v�Z
		Vector128 squareLength = _mm_mul_ps(vector, vector);

		// z, w�̃x�N�g�����擾
		Vector128 temp = PERMUTE_PS(squareLength, _MM_SHUFFLE(3, 2, 3, 2));

		// x + z, y + w
		squareLength = _mm_add_ps(squareLength, temp);

		// x + z, x + z, x + w, y + w
		squareLength = PERMUTE_PS(squareLength, _MM_SHUFFLE(1, 0, 0, 0));

		// ??, ??, y + w, y + w
		temp = _mm_shuffle_ps(temp, squareLength, _MM_SHUFFLE(3, 3, 0, 0));

		// ??, ??, x + z + y + w, ??
		squareLength = _mm_add_ps(squareLength, temp);
		squareLength = PERMUTE_PS(squareLength, _MM_SHUFFLE(2, 2, 2, 2));
		squareLength = _mm_sqrt_ps(squareLength);
		return _mm_cvtss_f32(squareLength);
	}

	/****************************************************************************
	*                      LengthSquaredVector2
	****************************************************************************/
	/* @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::LengthSquaredVector2(ConstVector128 vector) noexcept
	*
	*  @brief     2���m������2����Z�o
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::LengthSquaredVector2(ConstVector128 vector) noexcept
	{
		return DotVector2(vector, vector);
	}

	/****************************************************************************
	*                      LengthSquaredVector3
	****************************************************************************/
	/* @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::LengthSquaredVector3(ConstVector128 vector) noexcept
	*
	*  @brief     2���m������2����Z�o
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::LengthSquaredVector3(ConstVector128 vector) noexcept
	{
		return DotVector3(vector, vector);
	}

	/****************************************************************************
	*                      LengthSquaredVector4
	****************************************************************************/
	/* @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::LengthSquaredVector4(ConstVector128 vector) noexcept
	*
	*  @brief     2���m������2����Z�o
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::LengthSquaredVector4(ConstVector128 vector) noexcept
	{
		return DotVector4(vector, vector);
	}

	/****************************************************************************
	*                      DotVector2
	****************************************************************************/
	/* @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector2(ConstVector128 left, ConstVector128 right)
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
		// �v�f���Ƃ̐ς��v�Z 
		Vector128 multiply = _mm_mul_ps(left, right);

		// z�ȍ~�̌v�Z�͍l�����Ȃ�����, y�̌��ʂ��i�[
		Vector128 y = PERMUTE_PS(multiply, _MM_SHUFFLE(1, 1, 1, 1));

		// x�����݂̂̌��ʂɒ��ڂ���.  
		return _mm_cvtss_f32(_mm_add_ss(multiply, y));
	}

	/****************************************************************************
	*                      DotVector3
	****************************************************************************/
	/* @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector3(ConstVector128 left, ConstVector128 right)
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
		// �v�f���Ƃ̐ς��v�Z 
		Vector128 multiply = _mm_mul_ps(left, right);

		// x=Dot.F32[1], y=Dot.F32[2]
		Vector128 temp = PERMUTE_PS(multiply, _MM_SHUFFLE(2, 1, 2, 1));

		// result.F32[0] = x + y
		multiply = _mm_add_ss(multiply, temp);

		// x=multiply.F32[2]
		temp = PERMUTE_PS(temp, _MM_SHUFFLE(1, 1, 1, 1));

		// Result.F32[0] = (x+y)+z
		temp = _mm_add_ss(multiply, temp);

		// x�����݂̂̌��ʂɒ��ڂ���.  
		return _mm_cvtss_f32(PERMUTE_PS(multiply, _MM_SHUFFLE(0, 0, 0, 0)));
	}

	/****************************************************************************
	*                      DotVector4
	****************************************************************************/
	/* @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector4(ConstVector128 left, ConstVector128 right)
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
		// �v�f���Ƃ̐ς��v�Z 
		Vector128 temp2 = right;
		Vector128 temp = _mm_mul_ps(left, temp2);

		// �e�v�f�̉��Z�ɂ����, X��Z�̈ʒu�ɁAY��W�̈ʒu�ɃR�s�[����
		temp2 = _mm_shuffle_ps(temp2, temp, _MM_SHUFFLE(1, 0, 0, 0));

		// Z = X+Z; W = Y+W;
		temp2 = _mm_add_ps(temp2, temp);

		// W ��Z�̈ʒu�ɃR�s�[
		temp = _mm_shuffle_ps(temp, temp2, _MM_SHUFFLE(0, 3, 0, 0));

		// Z��W�𗼕����Z
		temp = _mm_add_ps(temp, temp2);

		// x�����݂̂̌��ʂɒ��ڂ���.  
		return _mm_cvtss_f32(PERMUTE_PS(temp, _MM_SHUFFLE(2, 2, 2, 2)));
	}

	/****************************************************************************
	*                      CrossVector2
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CrossVector2(ConstVector128 left, ConstVector128 right)
	*
	*  @brief     ���s�l�ӌ`�̖ʐς��Z�o����
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@Vector128�ł͂��邪, ����float
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CrossVector2(ConstVector128 left, ConstVector128 right) noexcept
	{
		// [ left.x*right.y - left.y*right.x, left.x*right.y - left.y*right.x ]
		
		// right��x, y�����ւ��� (w,z�͌v�Z��g�p���Ȃ�)
		Vector128 result = PERMUTE_PS(right, _MM_SHUFFLE(0, 1, 0, 1));

		// left�Ɠ���ւ����right�̊e�v�f����Z����
		result = _mm_mul_ps(result, left);

		// y�𒊏o����
		Vector128 y = PERMUTE_PS(result, _MM_SHUFFLE(1, 1, 1, 1));

		// �l�����Z����
		result = _mm_sub_ss(result, y);

		// x�����݂̂����o���Č��ʂƂ���
		return PERMUTE_PS(result, _MM_SHUFFLE(0, 0, 0, 0));
	}

	/****************************************************************************
	*                      CrossVector3
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CrossVector3(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     �O�ς��Z�o left, right�ɐ����ȃx�N�g����Ԃ�
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CrossVector3(ConstVector128 left, ConstVector128 right) noexcept
	{
		// [ V1.y*V2.z - V1.z*V2.y, V1.z*V2.x - V1.x*V2.z, V1.x*V2.y - V1.y*V2.x ]
		
		// left�̗v�f��(y1, z1, x1, w1)�ɏ��ɕ��בւ���
		Vector128 temp1 = PERMUTE_PS(left, _MM_SHUFFLE(3, 0, 2, 1)); 

		// right�̗v�f��(z2, x2. y2, w2)�̏��ɕ��בւ���
		Vector128 temp2 = PERMUTE_PS(right, _MM_SHUFFLE(3, 1, 0, 2));

		// �ꎞ�x�N�g���̗v�f���Ƃ̏�Z
		Vector128 result = _mm_mul_ps(temp1, temp2);

		// z1, x1, y1, w1�̏���temp1����בւ���
		temp1 = PERMUTE_PS(temp1, _MM_SHUFFLE(3, 0, 2, 1));
		
		// y2, z2, x2, w2�̏���temp2����בւ���
		temp2 = PERMUTE_PS(temp2, _MM_SHUFFLE(3, 1, 0, 2));

		result = NegativeMultiplySubtract(temp1, temp2, result);

		// w�̗v�f���[���ɐݒ�
		return _mm_and_ps(result, VECTOR_128F_MASK_XYZ);
	}

	/****************************************************************************
	*                      CrossVector4
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CrossVector4(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     �O�ς��Z�o left, right�ɐ����ȃx�N�g����Ԃ�
	*
	*  @param[in] ConstVector128 first
	*  @param[in] ConstVector128 second
	*  @param[in] ConstVector128 third
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CrossVector4(ConstVector128 first, ConstVector128 second, ConstVector128 third) noexcept
	{
		// [ ((v2.z*v3.w-v2.w*v3.z)*v1.y)-((v2.y*v3.w-v2.w*v3.y)*v1.z)+((v2.y*v3.z-v2.z*v3.y)*v1.w),
		//   ((v2.w*v3.z-v2.z*v3.w)*v1.x)-((v2.w*v3.x-v2.x*v3.w)*v1.z)+((v2.z*v3.x-v2.x*v3.z)*v1.w),
		//   ((v2.y*v3.w-v2.w*v3.y)*v1.x)-((v2.x*v3.w-v2.w*v3.x)*v1.y)+((v2.x*v3.y-v2.y*v3.x)*v1.w),
		//   ((v2.z*v3.y-v2.y*v3.z)*v1.x)-((v2.z*v3.x-v2.x*v3.z)*v1.y)+((v2.y*v3.x-v2.x*v3.y)*v1.z) ]

		// second.zwyz * third.wzxy
		Vector128 result = PERMUTE_PS(second, _MM_SHUFFLE(2, 1, 3, 2));
		Vector128 temp3  = PERMUTE_PS(third, _MM_SHUFFLE(1, 3, 2, 3));
		result = _mm_mul_ps(result, temp3);

		// - second.wzwy * third.zwyz
		Vector128 temp2 = PERMUTE_PS(second, _MM_SHUFFLE(1, 3, 2, 3));
		temp3  = PERMUTE_PS(temp3, _MM_SHUFFLE(1, 3, 0, 1));
		result = NegativeMultiplySubtract(temp2, temp3, result);

		// term1 * first.yxxx
		Vector128 temp1 = PERMUTE_PS(first, _MM_SHUFFLE(0, 0, 0, 1));
		result = _mm_mul_ps(result, temp1);

		// second.ywxz * third.wxwx
		temp2 = PERMUTE_PS(second, _MM_SHUFFLE(2, 0, 3, 1));
		temp3 = PERMUTE_PS(third, _MM_SHUFFLE(0, 3, 0, 3));
		temp3 = _mm_mul_ps(temp3, temp2);

		// - second.wxwx * third.ywxz
		temp2 = PERMUTE_PS(temp2, _MM_SHUFFLE(2, 1, 2, 1));
		temp1 = PERMUTE_PS(third, _MM_SHUFFLE(2, 0, 3, 1));
		temp3 = NegativeMultiplySubtract(temp2, temp1, temp3);

		// result - temp * first.zzyy
		temp1 = PERMUTE_PS(first, _MM_SHUFFLE(1, 1, 2, 2));
		result = NegativeMultiplySubtract(temp1, temp3, result);

		// second.yzxy * third.zxyx
		temp2 = PERMUTE_PS(second, _MM_SHUFFLE(1, 0, 2, 1));
		temp3 = PERMUTE_PS(third, _MM_SHUFFLE(0, 1, 0, 2));
		temp3 = _mm_mul_ps(temp3, temp2);

		// -second.zxyx * third.yzxy
		temp2 = PERMUTE_PS(temp2, _MM_SHUFFLE(2, 0, 2, 1));
		temp1 = PERMUTE_PS(third, _MM_SHUFFLE(1, 0, 2, 1));
		temp3 = NegativeMultiplySubtract(temp1, temp2, temp3);

		// result + term + first.wwwz
		temp1 = PERMUTE_PS(first, _MM_SHUFFLE(2, 3, 3, 3));
		result = MultiplyAdd(temp3, temp1, result);
	}

	/****************************************************************************
	*                      NormalizeVector2
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector2(ConstVector128 vector) noexcept
	*
	*  @brief     ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector2(ConstVector128 vector) noexcept
	{
		// 1��norm�v�Z
		Vector128 squareLength = _mm_mul_ps(vector, vector);
		Vector128 temp = PERMUTE_PS(squareLength, _MM_SHUFFLE(1, 1, 1, 1));
		squareLength   = _mm_add_ps(squareLength, temp);
		squareLength   = PERMUTE_PS(squareLength, _MM_SHUFFLE(0, 0, 0, 0));
		Vector128 norm = _mm_sqrt_ps(squareLength);

		// 0�Ŋ�����\�������邩�ǂ����𒲂ׂ�. �l�������Ă�����1
		Vector128 zeroMask = _mm_setzero_ps();
		zeroMask = _mm_cmpneq_ps(zeroMask, norm);

		// ������łȂ��ꍇ�͑Ή�����v�f��1�ɂȂ�}�X�N�̍쐬
		Vector128 infinityMask = _mm_cmpneq_ps(squareLength, VECTOR_128F_INFINITY);

		// ���K���̎��s
		Vector128 result = _mm_div_ps(vector, norm);

		// 0�t�߂Ŋ����Ė�����ɂȂ������̂ɑ΂��Ă�0�ɐݒ�
		result = _mm_and_ps(result, zeroMask);

		// �����傩�Ɋ�Â���qnan�܂���result��I������
		Vector128 temp1 = _mm_andnot_ps(infinityMask, VECTOR_128F_QNAN);
		Vector128 temp2 = _mm_and_ps(result, infinityMask);
		result = _mm_or_ps(temp1, temp2);
		return result;
	}

	/****************************************************************************
	*                      NormalizeVector3
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector3(ConstVector128 vector) noexcept
	*
	*  @brief     ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector3(ConstVector128 vector) noexcept
	{
		// x, y, z�ɑ΂���h�b�g�ς̌v�Z
		// x*x, y*y, z*z, w*w
		Vector128 squareLength = _mm_mul_ps(vector, vector);

		// z*z, y*y, z*z, y*y
		Vector128 temp         = PERMUTE_PS(squareLength, _MM_SHUFFLE(2, 1, 2, 1)); 

		// x*x + z*z, y*y, z*z, w*w
		squareLength           = _mm_add_ss(squareLength, temp);

		// �S��y*y
		temp = PERMUTE_PS(temp, _MM_SHUFFLE(1, 1, 1, 1));

		// 1�v�f�ڂ�x*x + y*y + z*z
		squareLength = _mm_add_ss(squareLength, temp);
		// �S�Ă̗v�f���h�b�g��
		squareLength = PERMUTE_PS(squareLength, _MM_SHUFFLE(0, 0, 0, 0));

		// 1���m�����̌v�Z
		Vector128 norm = _mm_sqrt_ps(squareLength);

		// 0�Ŋ�����\�������邩�ǂ����𒲂ׂ�. �l�������Ă�����1
		Vector128 zeroMask = _mm_setzero_ps();
		zeroMask = _mm_cmpneq_ps(zeroMask, norm);

		// ������łȂ��ꍇ�͑Ή�����v�f��1�ɂȂ�}�X�N�̍쐬
		Vector128 infinityMask = _mm_cmpneq_ps(squareLength, VECTOR_128F_INFINITY);

		// ���K���̎��s
		Vector128 result = _mm_div_ps(vector, norm);

		// 0�t�߂Ŋ����Ė�����ɂȂ������̂ɑ΂��Ă�0�ɐݒ�
		result = _mm_and_ps(result, zeroMask);

		// �����傩�Ɋ�Â���qnan�܂���result��I������
		Vector128 temp1 = _mm_andnot_ps(infinityMask, VECTOR_128F_QNAN);
		Vector128 temp2 = _mm_and_ps(result, infinityMask);
		result = _mm_or_ps(temp1, temp2);
		return result;
	}

	/****************************************************************************
	*                      NormalizeVector4
	****************************************************************************/ 
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector4(ConstVector128 vector) noexcept
	*
	*  @brief     ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector4(ConstVector128 vector) noexcept
	{
		// x, y, z, w�ɑ΂���h�b�g�ς̌v�Z
		// x*x, y*y, z*z, w*w
		Vector128 squareLength = _mm_mul_ps(vector, vector);

		// w*w, z*z, w*w, z*z
		Vector128 temp = PERMUTE_PS(squareLength, _MM_SHUFFLE(3, 2, 3, 2));

		// squareLength(x*x + w*w, y*y + z*z, z*z + w*w, w*w +z*z)
		squareLength = _mm_add_ps(squareLength, temp);

		// squareLength(y*y + z*z. x*x+w*w, x*x+w*w, x*x+w*w)
		squareLength = PERMUTE_PS(squareLength, _MM_SHUFFLE(1, 0, 0, 0));

		// temp(z*z, z*z, y*y + z*z, y*y + z*z 
		temp = _mm_shuffle_ps(temp, squareLength, _MM_SHUFFLE(3, 3, 0, 0));

		// ??, ??, x*x+w*w + y*y + z*z, ?? 
		squareLength = _mm_add_ss(squareLength, temp);

		// �S�Ă̗v�f��4�v�f�̓��a
		squareLength = PERMUTE_PS(squareLength, _MM_SHUFFLE(2, 2, 2, 2));

		// 1���m�����̌v�Z
		Vector128 norm = _mm_sqrt_ps(squareLength);

		// 0�Ŋ�����\�������邩�ǂ����𒲂ׂ�. �l�������Ă�����1
		Vector128 zeroMask = _mm_setzero_ps();
		zeroMask = _mm_cmpneq_ps(zeroMask, norm);

		// ������łȂ��ꍇ�͑Ή�����v�f��1�ɂȂ�}�X�N�̍쐬
		Vector128 infinityMask = _mm_cmpneq_ps(squareLength, VECTOR_128F_INFINITY);

		// ���K���̎��s
		Vector128 result = _mm_div_ps(vector, norm);

		// 0�t�߂Ŋ����Ė�����ɂȂ������̂ɑ΂��Ă�0�ɐݒ�
		result = _mm_and_ps(result, zeroMask);

		// �����傩�Ɋ�Â���qnan�܂���result��I������
		Vector128 temp1 = _mm_andnot_ps(infinityMask, VECTOR_128F_QNAN);
		Vector128 temp2 = _mm_and_ps(result, infinityMask);
		result = _mm_or_ps(temp1, temp2);
		return result;
	}

	/****************************************************************************
	*                      ReflectVector2
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ReflectVector2(ConstVector128 input, ConstVector128 normal) noexcept
	*
	*  @brief     ���˃x�N�g�����Z�o (input - (2 * dot(input, normal)) * normal)
	*
	*  @param[in] ConstVector128 : input  (���˃x�N�g��)
	*  @param[in] ConstVector128 : normal (���˂��s���@���x�N�g��)
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ReflectVector2(ConstVector128 input, ConstVector128 normal) noexcept
	{
		Vector128 result = Set(DotVector2(input, normal) * 2);
		return NegativeMultiplySubtract(result, normal, input);
	}

	/****************************************************************************
	*                      ReflectVector4
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ReflectVector4(ConstVector128 input, ConstVector128 normal) noexcept
	*
	*  @brief     ���˃x�N�g�����Z�o (input - (2 * dot(input, normal)) * normal)
	*
	*  @param[in] ConstVector128 : input  (���˃x�N�g��)
	*  @param[in] ConstVector128 : normal (���˂��s���@���x�N�g��)
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ReflectVector3(ConstVector128 input, ConstVector128 normal) noexcept
	{
		Vector128 result = Set(DotVector3(input, normal) * 2);
		return NegativeMultiplySubtract(result, normal, input);
	}

	/****************************************************************************
	*                      ReflectVector4
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ReflectVector4(ConstVector128 input, ConstVector128 normal)
	*
	*  @brief     ���˃x�N�g�����Z�o (input - (2 * dot(input, normal)) * normal)
	*
	*  @param[in] ConstVector128 : input  (���˃x�N�g��)
	*  @param[in] ConstVector128 : normal (���˂��s���@���x�N�g��)
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ReflectVector4(ConstVector128 input, ConstVector128 normal) noexcept
	{
		Vector128 result = Set(DotVector4(input, normal) * 2);
		return NegativeMultiplySubtract(result, normal, input);
	}

	/****************************************************************************
	*                      RefractVector2
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RefractVector2(ConstVector128 input, ConstVector128 normal, const float refractionIndex)
	*
	*  @brief     ���܃x�N�g�����Z�o
	*
	*  @param[in] ConstVector128 : input  (���˃x�N�g��)
	*  @param[in] ConstVector128 : normal (���܂��s���@���x�N�g��)
	*  @param[in]  const float   : refractionIndex (���ܗ�)
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RefractVector2(ConstVector128 input, ConstVector128 normal, const float refractionIndex) noexcept
	{
		return RefractVector2V(input, normal, Set(refractionIndex));
	}

	/****************************************************************************
	*                      RefractVector3
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RefractVector3(ConstVector128 input, ConstVector128 normal, const float refractionIndex)
	*
	*  @brief     ���܃x�N�g�����Z�o
	*
	*  @param[in] ConstVector128 : input  (���˃x�N�g��)
	*  @param[in] ConstVector128 : normal (���܂��s���@���x�N�g��)
	*  @param[in]  const float   : refractionIndex (���ܗ�)
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RefractVector3(ConstVector128 input, ConstVector128 normal, const float refractionIndex) noexcept
	{
		return RefractVector3V(input, normal, Set(refractionIndex));
	}

	/****************************************************************************
	*                      RefractVector4
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RefractVector4(ConstVector128 input, ConstVector128 normal, const float refractionIndex)
	*
	*  @brief     ���܃x�N�g�����Z�o
	*
	*  @param[in] ConstVector128 : input  (���˃x�N�g��)
	*  @param[in] ConstVector128 : normal (���܂��s���@���x�N�g��)
	*  @param[in]  const float   : refractionIndex (���ܗ�)
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RefractVector4(ConstVector128 input, ConstVector128 normal, const float refractionIndex) noexcept
	{
		return RefractVector4V(input, normal, Set(refractionIndex));
	}

	/****************************************************************************
	*                      RefractVector2
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RefractVector2(ConstVector128 input, ConstVector128 normal, const float refractionIndex)
	*
	*  @brief     ���܃x�N�g�����Z�o (�ٕ���������ꍇ)
	*
	*  @param[in] ConstVector128 : input  (���˃x�N�g��)
	*  @param[in] ConstVector128 : normal (���܂��s���@���x�N�g��)
	*  @param[in]  const float   : refractionIndex (���ܗ�)
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RefractVector2V(ConstVector128 input, ConstVector128 normal, ConstVector128 refractionIndex) noexcept
	{
		// Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) +
		// sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))

		Vector128 inputDotNormal = Set(DotVector2(input, normal));

		// vTemp = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
		Vector128 temp = NegativeMultiplySubtract(inputDotNormal, inputDotNormal, VECTOR_128F_ONE);
		temp = _mm_mul_ps(temp, refractionIndex);
		temp = NegativeMultiplySubtract(temp, refractionIndex, VECTOR_128F_ONE);

		// ���ʂ�0�ȉ��ł����, sqrt�͎��s�Ƃ������Ƃ�zero�ɒu��������
		Vector128 zeroMask = _mm_cmpgt_ps(temp, VECTOR_128F_ZERO);

		// r = refractionIndex * inputDotNormal + sqrt(r)
		temp = _mm_sqrt_ps(temp);
		temp = MultiplyAdd(refractionIndex, inputDotNormal, temp);

		// result = refractionIndex * input - normal * r
		Vector128 result = _mm_mul_ps(refractionIndex, input);
		result = NegativeMultiplySubtract(temp, normal, result);
		result = _mm_and_ps(result, zeroMask);

		return result;
	}

	/****************************************************************************
	*                      RefractVector3
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RefractVector3(ConstVector128 input, ConstVector128 normal, const float refractionIndex)
	*
	*  @brief     ���܃x�N�g�����Z�o (�ٕ���������ꍇ)
	*
	*  @param[in] ConstVector128 : input  (���˃x�N�g��)
	*  @param[in] ConstVector128 : normal (���܂��s���@���x�N�g��)
	*  @param[in]  const float   : refractionIndex (���ܗ�)
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RefractVector3V(ConstVector128 input, ConstVector128 normal, ConstVector128 refractionIndex) noexcept
	{
		// Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) +
		// sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))
		Vector128 inputDotNormal = Set(DotVector3(input, normal));

		// vTemp = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
		Vector128 r  = NegativeMultiplySubtract(inputDotNormal, inputDotNormal, VECTOR_128F_ONE);
		Vector128 r2 = _mm_mul_ps(refractionIndex, refractionIndex);
		r = NegativeMultiplySubtract(r, r2, VECTOR_128F_ONE);

		Vector128 result = _mm_cmple_ps(r, VECTOR_128F_ZERO);

		if (_mm_movemask_ps(result) == 0x0f)
		{
			// Total internal reflection
			result = VECTOR_128F_ZERO;
		}
		else
		{
			// r = refractionIndex * inputDotNormal + sqrt(r)
			r = _mm_sqrt_ps(r);
			r = MultiplyAdd(refractionIndex, inputDotNormal, r);

			// result = refractionIndex * input - normal * r
			result = _mm_mul_ps(refractionIndex, input);
			result = NegativeMultiplySubtract(r, normal, result);
		}

		return result;
	}

	/****************************************************************************
	*                      RefractVector4
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RefractVector4(ConstVector128 input, ConstVector128 normal, const float refractionIndex)
	*
	*  @brief     ���܃x�N�g�����Z�o (�ٕ���������ꍇ)
	*
	*  @param[in] ConstVector128 : input  (���˃x�N�g��)
	*  @param[in] ConstVector128 : normal (���܂��s���@���x�N�g��)
	*  @param[in]  const float   : refractionIndex (���ܗ�)
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RefractVector4V(ConstVector128 input, ConstVector128 normal, ConstVector128 refractionIndex) noexcept
	{
		// Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) +
		// sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))
		Vector128 inputDotNormal = Set(DotVector4(input, normal));

		// vTemp = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
		Vector128 r  = NegativeMultiplySubtract(inputDotNormal, inputDotNormal, VECTOR_128F_ONE);
		Vector128 r2 = _mm_mul_ps(refractionIndex, refractionIndex);
		r = NegativeMultiplySubtract(r, r2, VECTOR_128F_ONE);

		Vector128 result = _mm_cmple_ps(r, VECTOR_128F_ZERO);

		if (_mm_movemask_ps(result) == 0x0f)
		{
			// Total internal reflection
			result = VECTOR_128F_ZERO;
		}
		else
		{
			// r = refractionIndex * inputDotNormal + sqrt(r)
			r = _mm_sqrt_ps(r);
			r = MultiplyAdd(refractionIndex, inputDotNormal, r);

			// result = refractionIndex * input - normal * r
			result = _mm_mul_ps(refractionIndex, input);
			result = NegativeMultiplySubtract(r, normal, result);
		}

		return result;
	}

	/****************************************************************************
	*                      IsInfiniteVector2
	****************************************************************************/
	/* @fn        inline bool Vector128Utility::IsInfiniteVector2(ConstVector128 vector) noexcept
	*
	*  @brief     ���ɑ傫���l��
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline bool Vector128Utility::IsInfiniteVector2(ConstVector128 vector) noexcept
	{
		// �����r�b�g���}�X�N�I�t����
		Vector128 temp = _mm_and_ps(vector, VECTOR_128U_MASK_ABS);
		// ������Ɣ�r
		temp = _mm_cmpeq_ps(temp, VECTOR_128F_INFINITY);
		// �ǂꂩ��������ł���΁A�����͐^�ł���B
		return (_mm_movemask_ps(temp) & 0b0011) != 0;
	}

	/****************************************************************************
	*                      IsInfiniteVector3
	****************************************************************************/
	/* @fn        inline bool Vector128Utility::IsInfiniteVector3(ConstVector128 vector) noexcept
	*
	*  @brief     ���ɑ傫���l��
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline bool Vector128Utility::IsInfiniteVector3(ConstVector128 vector) noexcept
	{
		// �����r�b�g���}�X�N�I�t����
		Vector128 temp = _mm_and_ps(vector, VECTOR_128U_MASK_ABS);
		// ������Ɣ�r
		temp = _mm_cmpeq_ps(temp, VECTOR_128F_INFINITY);
		// �ǂꂩ��������ł���΁A�����͐^�ł���B
		return (_mm_movemask_ps(temp) & 0b0111) != 0;
	}

	/****************************************************************************
	*                      IsInfiniteVector4
	****************************************************************************/
	/* @fn        inline bool Vector128Utility::IsInfiniteVector4(ConstVector128 vector) noexcept
	*
	*  @brief     ���ɑ傫���l��
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline bool Vector128Utility::IsInfiniteVector4(ConstVector128 vector) noexcept
	{
		// �����r�b�g���}�X�N�I�t����
		Vector128 temp = _mm_and_ps(vector, VECTOR_128U_MASK_ABS);
		// ������Ɣ�r
		temp = _mm_cmpeq_ps(temp, VECTOR_128F_INFINITY);
		// �ǂꂩ��������ł���΁A�����͐^�ł���B
		return (_mm_movemask_ps(temp) != 0);
	}

	/****************************************************************************
	*                      IsInfiniteVectorEach
	****************************************************************************/
	/* @fn        inline Vector128 Vector128Utility::IsInfiniteVectorEach(ConstVector128 vector) noexcept
	*
	*  @brief     ���ɑ傫���l��
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 Vector128Utility::IsInfiniteVectorEach(ConstVector128 vector) noexcept
	{
		// �����r�b�g���}�X�N�I�t����
		Vector128 temp = _mm_and_ps(vector, VECTOR_128U_MASK_ABS);
		// ������Ɣ�r
		temp = _mm_cmpeq_ps(temp, VECTOR_128F_INFINITY);
		// �ǂꂩ��������ł���΁A�����͐^�ł���B
		return temp;
	}


	/****************************************************************************
	*                      Sqrt
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Sqrt(ConstVector128 vector) noexcept
	*
	*  @brief      �S�Ă̗v�f�ɍ�������������̂�Ԃ��܂�
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Sqrt(ConstVector128 vector) noexcept
	{
		return _mm_sqrt_ps(vector);
	}

	/****************************************************************************
	*                      ReciprocalSqrt
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ReciprocalSqrt(ConstVector128 vector) noexcept
	*
	*  @brief       �S�Ă̗v�f�̋t�������, ����ɍ�������������̂�Ԃ��܂�
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ReciprocalSqrt(ConstVector128 vector) noexcept
	{
		Vector128 result = _mm_sqrt_ps(vector);
		result = _mm_div_ps(VECTOR_128F_ONE, result);
		return result;
	}

	/****************************************************************************
	*                      Exp10
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Exp10(ConstVector128 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��2^{vector}��Ԃ��܂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Exp2(ConstVector128 vector) noexcept
	{
		__m128i itrunc = _mm_cvttps_epi32(vector);
		__m128  ftrunc = _mm_cvtepi32_ps(itrunc);
		__m128  y      = _mm_sub_ps(vector, ftrunc);

		__m128 poly = MultiplyAdd(VECTOR_128F_EXPONENTIAL_ESTIMATE7, y, VECTOR_128F_EXPONENTIAL_ESTIMATE6);
		poly = MultiplyAdd(poly, y, VECTOR_128F_EXPONENTIAL_ESTIMATE5);
		poly = MultiplyAdd(poly, y, VECTOR_128F_EXPONENTIAL_ESTIMATE4);
		poly = MultiplyAdd(poly, y, VECTOR_128F_EXPONENTIAL_ESTIMATE3);
		poly = MultiplyAdd(poly, y, VECTOR_128F_EXPONENTIAL_ESTIMATE2);
		poly = MultiplyAdd(poly, y, VECTOR_128F_EXPONENTIAL_ESTIMATE1);
		poly = MultiplyAdd(poly, y, VECTOR_128F_ONE);

		__m128i biased = _mm_add_epi32(itrunc, VECTOR_128I_EXPONENTIAL_BIAS);
		biased = _mm_slli_epi32(biased, 23);
		__m128 result0 = _mm_div_ps(_mm_castsi128_ps(biased), poly);

		biased = _mm_add_epi32(itrunc, VECTOR_128I_253);
		biased = _mm_slli_epi32(biased, 23);
		__m128 result1 = _mm_div_ps(_mm_castsi128_ps(biased), poly);
		result1 = _mm_mul_ps(_mm_castsi128_ps(VECTOR_128I_MIN_NORMAL), result1);

		// Use selection to handle the cases
		//  if (V is NaN) -> QNaN;
		//  else if (V sign bit set)
		//      if (V > -150)
		//         if (V.exponent < -126) -> result1
		//         else -> result0
		//      else -> +0
		//  else
		//      if (V < 128) -> result0
		//      else -> +inf

		__m128i comp = _mm_cmplt_epi32(_mm_castps_si128(vector), VECTOR_128I_BIN128);
		__m128i select0 = _mm_and_si128(comp, _mm_castps_si128(result0));
		__m128i select1 = _mm_andnot_si128(comp, VECTOR_128F_INFINITY);
		__m128i result2 = _mm_or_si128(select0, select1);

		comp = _mm_cmplt_epi32(itrunc, _mm_set1_epi32(-126));
		select1 = _mm_and_si128(comp, _mm_castps_si128(result1));
		select0 = _mm_andnot_si128(comp, _mm_castps_si128(result0));
		__m128i result3 = _mm_or_si128(select0, select1);

		comp = _mm_cmplt_epi32(_mm_castps_si128(vector), VECTOR_128U_BIN_NEGATIVE_150);
		select0 = _mm_and_si128(comp, result3);
		select1 = _mm_andnot_si128(comp, VECTOR_128F_ZERO);
		__m128i result4 = _mm_or_si128(select0, select1);

		__m128i sign = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128U_NEGATIVE_ZERO);
		comp = _mm_cmpeq_epi32(sign, VECTOR_128U_NEGATIVE_ZERO);
		select0 = _mm_and_si128(comp, result4);
		select1 = _mm_andnot_si128(comp, result2);
		__m128i result5 = _mm_or_si128(select0, select1);

		__m128i t0 = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128I_QNAN_TEST);
		__m128i t1 = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128F_INFINITY);
		t0 = _mm_cmpeq_epi32(t0, VECTOR_128F_ZERO);
		t1 = _mm_cmpeq_epi32(t1, VECTOR_128F_INFINITY);
		__m128i isNaN = _mm_andnot_si128(t0, t1);

		select0 = _mm_and_si128(isNaN, VECTOR_128F_QNAN);
		select1 = _mm_andnot_si128(isNaN, result5);
		__m128i vResult = _mm_or_si128(select0, select1);

		return _mm_castsi128_ps(vResult);
	}

	/****************************************************************************
	*                      Exp10
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Exp10(ConstVector128 vector) noexcept
	*
	*  @brief      �S�Ă̗v�f��10^{vector}��Ԃ��܂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Exp10(ConstVector128 vector) noexcept
	{
		// Exp10(vector) = Exp2(vector * log2(10))
		Vector128 result = Multiply(VECTOR_128F_LOG_10, vector);
		return Exp2(vector);
	}

	/****************************************************************************
	*                      ExpE
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ExpE(ConstVector128 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��e^{vector}��Ԃ��܂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ExpE(ConstVector128 vector) noexcept
	{
		Vector128 result = Multiply(VECTOR_128F_LOG_E, vector);
		return Exp2(vector);
	}

	namespace internal
	{
		inline __m128i multi_sll_epi32(__m128i value, __m128i count) noexcept
		{
			__m128i v = _mm_shuffle_epi32(value, _MM_SHUFFLE(0, 0, 0, 0));
			__m128i c = _mm_shuffle_epi32(count, _MM_SHUFFLE(0, 0, 0, 0));
			c = _mm_and_si128(c, VECTOR_128U_MASK_X);
			__m128i r0 = _mm_sll_epi32(v, c);

			v = _mm_shuffle_epi32(value, _MM_SHUFFLE(1, 1, 1, 1));
			c = _mm_shuffle_epi32(count, _MM_SHUFFLE(1, 1, 1, 1));
			c = _mm_and_si128(c, VECTOR_128U_MASK_X);
			__m128i r1 = _mm_sll_epi32(v, c);

			v = _mm_shuffle_epi32(value, _MM_SHUFFLE(2, 2, 2, 2));
			c = _mm_shuffle_epi32(count, _MM_SHUFFLE(2, 2, 2, 2));
			c = _mm_and_si128(c, VECTOR_128U_MASK_X);
			__m128i r2 = _mm_sll_epi32(v, c);

			v = _mm_shuffle_epi32(value, _MM_SHUFFLE(3, 3, 3, 3));
			c = _mm_shuffle_epi32(count, _MM_SHUFFLE(3, 3, 3, 3));
			c = _mm_and_si128(c, VECTOR_128U_MASK_X);
			__m128i r3 = _mm_sll_epi32(v, c);

			// (r0,r0,r1,r1)
			__m128 r01 = _mm_shuffle_ps(_mm_castsi128_ps(r0), _mm_castsi128_ps(r1), _MM_SHUFFLE(0, 0, 0, 0));
			// (r2,r2,r3,r3)
			__m128 r23 = _mm_shuffle_ps(_mm_castsi128_ps(r2), _mm_castsi128_ps(r3), _MM_SHUFFLE(0, 0, 0, 0));
			// (r0,r1,r2,r3)
			__m128 result = _mm_shuffle_ps(r01, r23, _MM_SHUFFLE(2, 0, 2, 0));
			return _mm_castps_si128(result);
		}

		inline __m128i multi_srl_epi32(__m128i value, __m128i count) noexcept
		{
			__m128i v = _mm_shuffle_epi32(value, _MM_SHUFFLE(0, 0, 0, 0));
			__m128i c = _mm_shuffle_epi32(count, _MM_SHUFFLE(0, 0, 0, 0));
			c = _mm_and_si128(c, VECTOR_128U_MASK_X);
			__m128i r0 = _mm_srl_epi32(v, c);

			v = _mm_shuffle_epi32(value, _MM_SHUFFLE(1, 1, 1, 1));
			c = _mm_shuffle_epi32(count, _MM_SHUFFLE(1, 1, 1, 1));
			c = _mm_and_si128(c, VECTOR_128U_MASK_X);
			__m128i r1 = _mm_srl_epi32(v, c);

			v = _mm_shuffle_epi32(value, _MM_SHUFFLE(2, 2, 2, 2));
			c = _mm_shuffle_epi32(count, _MM_SHUFFLE(2, 2, 2, 2));
			c = _mm_and_si128(c, VECTOR_128U_MASK_X);
			__m128i r2 = _mm_srl_epi32(v, c);

			v = _mm_shuffle_epi32(value, _MM_SHUFFLE(3, 3, 3, 3));
			c = _mm_shuffle_epi32(count, _MM_SHUFFLE(3, 3, 3, 3));
			c = _mm_and_si128(c, VECTOR_128U_MASK_X);
			__m128i r3 = _mm_srl_epi32(v, c);

			// (r0,r0,r1,r1)
			__m128 r01 = _mm_shuffle_ps(_mm_castsi128_ps(r0), _mm_castsi128_ps(r1), _MM_SHUFFLE(0, 0, 0, 0));
			// (r2,r2,r3,r3)
			__m128 r23 = _mm_shuffle_ps(_mm_castsi128_ps(r2), _mm_castsi128_ps(r3), _MM_SHUFFLE(0, 0, 0, 0));
			// (r0,r1,r2,r3)
			__m128 result = _mm_shuffle_ps(r01, r23, _MM_SHUFFLE(2, 0, 2, 0));
			return _mm_castps_si128(result);
		}

		inline __m128i GetLeadingBit(const __m128i value) noexcept
		{
			static const Vector128i g_XM0000FFFF = { { { 0x0000FFFF, 0x0000FFFF, 0x0000FFFF, 0x0000FFFF } } };
			static const Vector128i g_XM000000FF = { { { 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF } } };
			static const Vector128i g_XM0000000F = { { { 0x0000000F, 0x0000000F, 0x0000000F, 0x0000000F } } };
			static const Vector128i g_XM00000003 = { { { 0x00000003, 0x00000003, 0x00000003, 0x00000003 } } };

			__m128i v = value, r, c, b, s;

			c = _mm_cmpgt_epi32(v, g_XM0000FFFF);   // c = (v > 0xFFFF)
			b = _mm_srli_epi32(c, 31);              // b = (c ? 1 : 0)
			r = _mm_slli_epi32(b, 4);               // r = (b << 4)
			v = multi_srl_epi32(v, r);              // v = (v >> r)

			c = _mm_cmpgt_epi32(v, g_XM000000FF);   // c = (v > 0xFF)
			b = _mm_srli_epi32(c, 31);              // b = (c ? 1 : 0)
			s = _mm_slli_epi32(b, 3);               // s = (b << 3)
			v = multi_srl_epi32(v, s);              // v = (v >> s)
			r = _mm_or_si128(r, s);                 // r = (r | s)

			c = _mm_cmpgt_epi32(v, g_XM0000000F);   // c = (v > 0xF)
			b = _mm_srli_epi32(c, 31);              // b = (c ? 1 : 0)
			s = _mm_slli_epi32(b, 2);               // s = (b << 2)
			v = multi_srl_epi32(v, s);              // v = (v >> s)
			r = _mm_or_si128(r, s);                 // r = (r | s)

			c = _mm_cmpgt_epi32(v, g_XM00000003);   // c = (v > 0x3)
			b = _mm_srli_epi32(c, 31);              // b = (c ? 1 : 0)
			s = _mm_slli_epi32(b, 1);               // s = (b << 1)
			v = multi_srl_epi32(v, s);              // v = (v >> s)
			r = _mm_or_si128(r, s);                 // r = (r | s)

			s = _mm_srli_epi32(v, 1);
			r = _mm_or_si128(r, s);
			return r;
		}
	} // n

	/****************************************************************************
	*                      Log2
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Log2(ConstVector128 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��log2{vector}��Ԃ��܂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Log2(ConstVector128 vector) noexcept
	{
		/*----------------------------------------------------------------------
		*   �w�����Ɖ����������o��.
		/*----------------------------------------------------------------------*/
		// vector�̎w���������o��  
		__m128i rawBiased      = _mm_and_si128(_mm_castps_si128(vector),VECTOR_128F_INFINITY);
		// vector�̉����������o��
		__m128i trailing       = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128I_QNAN_TEST);
		// vector�̎w������0�ł��邩���m�F����
		__m128i isExponentZero = _mm_cmpeq_epi32(VECTOR_128F_ZERO, rawBiased);

		// Compute exponent and significand for normals.
		__m128i biased      = _mm_srli_epi32(rawBiased, 23); // �V�t�g���邱�Ǝw�����̐����݂̂����o��. 
		__m128i exponentNor = _mm_sub_epi32(biased, VECTOR_128I_EXPONENTIAL_BIAS);
		__m128i trailingNor = trailing;

		// Compute exponent and significand for subnormals.
		__m128i leading     = internal::GetLeadingBit(trailing); // �������̍ŏ���1�̈ʒu���擾
		__m128i shift       = _mm_sub_epi32(VECTOR_128I_NUM_TRAILING, leading); // �������𐳋K�����邽�߂ɃV�t�g�ʂ��v�Z
		__m128i exponentSub = _mm_sub_epi32(VECTOR_128I_EXPONENTIAL_SUBNORMAL, shift); // �񐳋K�����̎w�������v�Z
		__m128i trailingSub = internal::multi_sll_epi32(trailing, shift);
		trailingSub = _mm_and_si128(trailingSub, VECTOR_128I_QNAN_TEST);

		__m128i select0 = _mm_and_si128(isExponentZero, exponentSub);
		__m128i select1 = _mm_andnot_si128(isExponentZero, exponentNor);
		__m128i e = _mm_or_si128(select0, select1);

		select0 = _mm_and_si128(isExponentZero, trailingSub);
		select1 = _mm_andnot_si128(isExponentZero, trailingNor);
		__m128i t = _mm_or_si128(select0, select1); // ������

		/*----------------------------------------------------------------------
		*   �ȉ��ł̓e�C���[�W�J��p����Log2�̋ߎ��l���v�Z����
		*   log2(y) = a7 * mantissa^7 + a6 * mantissa^6 + .. + a0
		/*----------------------------------------------------------------------*/
		// Compute the approximation.
		// temp��1.f (f�͉�����)��\��
		__m128i temp = _mm_or_si128(VECTOR_128F_ONE, t); 

		// mantissa�͉�������\��. mantissa = 1.f(������) - 1
		__m128 mantissa = _mm_sub_ps(_mm_castsi128_ps(temp), VECTOR_128F_ONE);

		// �ȉ��̌v�Z�ł�, �J��Ԃ�mantissa�������邱�Ƃ�, �搔�Ƃ��ĕ\���\
		__m128 log2 = MultiplyAdd(VECTOR_128F_LOG_ESTIMATE7, mantissa, VECTOR_128F_LOG_ESTIMATE6); // a7*mantissa + a6
		log2 = MultiplyAdd(log2, mantissa, VECTOR_128F_LOG_ESTIMATE5); // (a7 * mantissa + a6) * manttisa + a5 
		log2 = MultiplyAdd(log2, mantissa, VECTOR_128F_LOG_ESTIMATE4);
		log2 = MultiplyAdd(log2, mantissa, VECTOR_128F_LOG_ESTIMATE3);
		log2 = MultiplyAdd(log2, mantissa, VECTOR_128F_LOG_ESTIMATE2);
		log2 = MultiplyAdd(log2, mantissa, VECTOR_128F_LOG_ESTIMATE1);
		log2 = MultiplyAdd(log2, mantissa, VECTOR_128F_LOG_ESTIMATE0);
		log2 = MultiplyAdd(log2, mantissa, _mm_cvtepi32_ps(e)); // �Ō�Ɏw�����������邱�Ƃ�, ���������_��Log�𓾂���.

		//  if (x is NaN) -> QNaN
		//  else if (V is positive)
		//      if (V is infinite) -> +inf
		//      else -> log2(V)
		//  else
		//      if (V is zero) -> -inf
		//      else -> -QNaN

		__m128i isInfinite = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128U_MASK_ABS);
		        isInfinite = _mm_cmpeq_epi32(isInfinite, VECTOR_128F_INFINITY);

		__m128i isGreaterZero = _mm_cmpgt_epi32(_mm_castps_si128(vector), VECTOR_128F_ZERO);
		__m128i isNotFinite   = _mm_cmpgt_epi32(_mm_castps_si128(vector), VECTOR_128F_INFINITY);
		__m128i isPositive    = _mm_andnot_si128(isNotFinite, isGreaterZero);

		__m128i isZero = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128U_MASK_ABS);
		        isZero = _mm_cmpeq_epi32(isZero, VECTOR_128F_ZERO);

		__m128i t0 = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128I_QNAN_TEST);
		__m128i t1 = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128F_INFINITY);
		t0 = _mm_cmpeq_epi32(t0, VECTOR_128F_ZERO);
		t1 = _mm_cmpeq_epi32(t1, VECTOR_128F_INFINITY);
		__m128i isNaN = _mm_andnot_si128(t0, t1);

		select0 = _mm_and_si128   (isInfinite, VECTOR_128F_INFINITY);
		select1 = _mm_andnot_si128(isInfinite, _mm_castps_si128(log2));
		__m128i result = _mm_or_si128(select0, select1);

		select0 = _mm_and_si128   (isZero, VECTOR_128F_NEGATIVE_INFINITY);
		select1 = _mm_andnot_si128(isZero, VECTOR_128F_NEGATIVE_QNAN);
		temp = _mm_or_si128(select0, select1);

		select0 = _mm_and_si128   (isPositive, result);
		select1 = _mm_andnot_si128(isPositive, temp);
		result = _mm_or_si128(select0, select1);

		select0 = _mm_and_si128   (isNaN, VECTOR_128F_QNAN);
		select1 = _mm_andnot_si128(isNaN, result);
		result = _mm_or_si128(select0, select1);

		return _mm_castsi128_ps(result);
	}

	/****************************************************************************
	*                      Log10
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Log10(ConstVector128 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��log10{vector}��Ԃ��܂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Log10(ConstVector128 vector) noexcept
	{
		/*----------------------------------------------------------------------
		*   �w�����Ɖ����������o��.
		/*----------------------------------------------------------------------*/
		__m128i rawBiased      = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128F_INFINITY);
		__m128i trailing       = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128I_QNAN_TEST);
		__m128i isExponentZero = _mm_cmpeq_epi32(VECTOR_128F_ZERO, rawBiased);

		// Compute exponent and significand for normals.
		__m128i biased      = _mm_srli_epi32(rawBiased, 23);
		__m128i exponentNor = _mm_sub_epi32(biased, VECTOR_128I_EXPONENTIAL_BIAS);
		__m128i trailingNor = trailing;

		// Compute exponent and significand for subnormals.
		__m128i leading     = internal::GetLeadingBit(trailing);
		__m128i shift       = _mm_sub_epi32(VECTOR_128I_NUM_TRAILING, leading);
		__m128i exponentSub = _mm_sub_epi32(VECTOR_128I_EXPONENTIAL_SUBNORMAL, shift);
		__m128i trailingSub = internal::multi_sll_epi32(trailing, shift);
		trailingSub         = _mm_and_si128(trailingSub, VECTOR_128I_QNAN_TEST);

		__m128i select0 = _mm_and_si128(isExponentZero, exponentSub);
		__m128i select1 = _mm_andnot_si128(isExponentZero, exponentNor);
		__m128i e = _mm_or_si128(select0, select1);

		select0   = _mm_and_si128(isExponentZero, trailingSub);
		select1   = _mm_andnot_si128(isExponentZero, trailingNor);
		__m128i t = _mm_or_si128(select0, select1);

		/*----------------------------------------------------------------------
		*   �ȉ��ł̓e�C���[�W�J��p����Log2�̋ߎ��l���v�Z����
		*   log2(y) = a7 * mantissa^7 + a6 * mantissa^6 + .. + a0
		/*----------------------------------------------------------------------*/
		// Compute the approximation.
		// temp��1.f (f�͉�����)��\��
		__m128i tmp = _mm_or_si128(VECTOR_128F_ONE, t);
		__m128 y = _mm_sub_ps(_mm_castsi128_ps(tmp), VECTOR_128F_ONE);

		__m128 log2 = MultiplyAdd(VECTOR_128F_LOG_ESTIMATE7, y, VECTOR_128F_LOG_ESTIMATE6);
		log2 = MultiplyAdd(log2, y, VECTOR_128F_LOG_ESTIMATE5);
		log2 = MultiplyAdd(log2, y, VECTOR_128F_LOG_ESTIMATE4);
		log2 = MultiplyAdd(log2, y, VECTOR_128F_LOG_ESTIMATE3);
		log2 = MultiplyAdd(log2, y, VECTOR_128F_LOG_ESTIMATE2);
		log2 = MultiplyAdd(log2, y, VECTOR_128F_LOG_ESTIMATE1);
		log2 = MultiplyAdd(log2, y, VECTOR_128F_LOG_ESTIMATE0);
		log2 = MultiplyAdd(log2, y, _mm_cvtepi32_ps(e));

		// ����ω�������
		log2 = _mm_mul_ps(VECTOR_128F_LOG_10_INVERSE, log2);

		//  if (x is NaN) -> QNaN
		//  else if (V is positive)
		//      if (V is infinite) -> +inf
		//      else -> log2(V)
		//  else
		//      if (V is zero) -> -inf
		//      else -> -QNaN

		__m128i isInfinite = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128U_MASK_ABS);
		isInfinite = _mm_cmpeq_epi32(isInfinite, VECTOR_128F_INFINITY);

		__m128i isGreaterZero = _mm_cmpgt_epi32(_mm_castps_si128(vector), VECTOR_128F_ZERO);
		__m128i isNotFinite = _mm_cmpgt_epi32(_mm_castps_si128(vector), VECTOR_128F_INFINITY);
		__m128i isPositive = _mm_andnot_si128(isNotFinite, isGreaterZero);

		__m128i isZero = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128U_MASK_ABS);
		isZero = _mm_cmpeq_epi32(isZero, VECTOR_128F_ZERO);

		__m128i t0 = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128I_QNAN_TEST);
		__m128i t1 = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128F_INFINITY);
		t0 = _mm_cmpeq_epi32(t0, VECTOR_128F_ZERO);
		t1 = _mm_cmpeq_epi32(t1, VECTOR_128F_INFINITY);
		__m128i isNaN = _mm_andnot_si128(t0, t1);

		select0 = _mm_and_si128(isInfinite, VECTOR_128F_INFINITY);
		select1 = _mm_andnot_si128(isInfinite, _mm_castps_si128(log2));
		__m128i result = _mm_or_si128(select0, select1);

		select0 = _mm_and_si128(isZero, VECTOR_128F_NEGATIVE_INFINITY);
		select1 = _mm_andnot_si128(isZero, VECTOR_128F_NEGATIVE_QNAN);
		tmp     = _mm_or_si128(select0, select1);

		select0 = _mm_and_si128(isPositive, result);
		select1 = _mm_andnot_si128(isPositive, tmp);
		result  = _mm_or_si128(select0, select1);

		select0 = _mm_and_si128(isNaN, VECTOR_128F_QNAN);
		select1 = _mm_andnot_si128(isNaN, result);
		result  = _mm_or_si128(select0, select1);

		return _mm_castsi128_ps(result);
	}

	/****************************************************************************
	*                      LogE
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LogE(ConstVector128 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��loge{vector}��Ԃ��܂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LogE(ConstVector128 vector) noexcept
	{
		/*----------------------------------------------------------------------
		*   �w�����Ɖ����������o��.
		/*----------------------------------------------------------------------*/
		__m128i rawBiased      = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128F_INFINITY);
		__m128i trailing       = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128I_QNAN_TEST);
		__m128i isExponentZero = _mm_cmpeq_epi32(VECTOR_128F_ZERO, rawBiased);

		// Compute exponent and significand for normals.
		__m128i biased      = _mm_srli_epi32(rawBiased, 23);
		__m128i exponentNor = _mm_sub_epi32(biased, VECTOR_128I_EXPONENTIAL_BIAS);
		__m128i trailingNor = trailing;

		// Compute exponent and significand for subnormals.
		__m128i leading     = internal::GetLeadingBit(trailing);
		__m128i shift       = _mm_sub_epi32(VECTOR_128I_NUM_TRAILING, leading);
		__m128i exponentSub = _mm_sub_epi32(VECTOR_128I_EXPONENTIAL_SUBNORMAL, shift);
		__m128i trailingSub = internal::multi_sll_epi32(trailing, shift);
		trailingSub = _mm_and_si128(trailingSub, VECTOR_128I_QNAN_TEST);

		__m128i select0 = _mm_and_si128(isExponentZero, exponentSub);
		__m128i select1 = _mm_andnot_si128(isExponentZero, exponentNor);
		__m128i e       = _mm_or_si128(select0, select1);

		select0 = _mm_and_si128(isExponentZero, trailingSub);
		select1 = _mm_andnot_si128(isExponentZero, trailingNor);
		__m128i t = _mm_or_si128(select0, select1);

		/*----------------------------------------------------------------------
		*   �ȉ��ł̓e�C���[�W�J��p����Log2�̋ߎ��l���v�Z����
		*   log2(y) = a7 * mantissa^7 + a6 * mantissa^6 + .. + a0
		/*----------------------------------------------------------------------*/
		// Compute the approximation.
		__m128i tmp = _mm_or_si128(VECTOR_128F_ONE, t);
		__m128 y = _mm_sub_ps(_mm_castsi128_ps(tmp), VECTOR_128F_ONE);

		__m128 log2 = MultiplyAdd(VECTOR_128F_LOG_ESTIMATE7, y, VECTOR_128F_LOG_ESTIMATE6);
		log2 = MultiplyAdd(log2, y, VECTOR_128F_LOG_ESTIMATE5);
		log2 = MultiplyAdd(log2, y, VECTOR_128F_LOG_ESTIMATE4);
		log2 = MultiplyAdd(log2, y, VECTOR_128F_LOG_ESTIMATE3);
		log2 = MultiplyAdd(log2, y, VECTOR_128F_LOG_ESTIMATE2);
		log2 = MultiplyAdd(log2, y, VECTOR_128F_LOG_ESTIMATE1);
		log2 = MultiplyAdd(log2, y, VECTOR_128F_LOG_ESTIMATE0);
		log2 = MultiplyAdd(log2, y, _mm_cvtepi32_ps(e));

		// ����ω�������
		log2 = _mm_mul_ps(VECTOR_128F_LOG_E_INVERSE, log2);

		//  if (x is NaN) -> QNaN
		//  else if (V is positive)
		//      if (V is infinite) -> +inf
		//      else -> log2(V)
		//  else
		//      if (V is zero) -> -inf
		//      else -> -QNaN

		__m128i isInfinite = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128U_MASK_ABS);
		isInfinite = _mm_cmpeq_epi32(isInfinite, VECTOR_128F_INFINITY);

		__m128i isGreaterZero = _mm_cmpgt_epi32(_mm_castps_si128(vector), VECTOR_128F_ZERO);
		__m128i isNotFinite = _mm_cmpgt_epi32  (_mm_castps_si128(vector), VECTOR_128F_INFINITY);
		__m128i isPositive = _mm_andnot_si128  (isNotFinite, isGreaterZero);

		__m128i isZero = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128U_MASK_ABS);
		isZero = _mm_cmpeq_epi32(isZero, VECTOR_128F_ZERO);

		__m128i t0 = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128I_QNAN_TEST);
		__m128i t1 = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128F_INFINITY);
		t0 = _mm_cmpeq_epi32(t0, VECTOR_128F_ZERO);
		t1 = _mm_cmpeq_epi32(t1, VECTOR_128F_INFINITY);
		__m128i isNaN = _mm_andnot_si128(t0, t1);

		select0 = _mm_and_si128(isInfinite, VECTOR_128F_INFINITY);
		select1 = _mm_andnot_si128(isInfinite, _mm_castps_si128(log2));
		__m128i result = _mm_or_si128(select0, select1);

		select0 = _mm_and_si128   (isZero, VECTOR_128F_NEGATIVE_INFINITY);
		select1 = _mm_andnot_si128(isZero, VECTOR_128F_NEGATIVE_QNAN);
		tmp = _mm_or_si128(select0, select1);

		select0 = _mm_and_si128(isPositive, result);
		select1 = _mm_andnot_si128(isPositive, tmp);
		result = _mm_or_si128(select0, select1);

		select0 = _mm_and_si128(isNaN, VECTOR_128F_QNAN);
		select1 = _mm_andnot_si128(isNaN, result);
		result = _mm_or_si128(select0, select1);

		return _mm_castsi128_ps(result);
	}

	/****************************************************************************
	*                      Pow
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Pow(ConstVector128 base, ConstVector128 power)
	*
	*  @brief     �S�Ă̗v�f��Pow��Ԃ��܂�  base^{power}
	*
	*  @param[in] ConstVector128 : base
	*  @param[in] ConstVector128 : power
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Pow(ConstVector128 base, ConstVector128 power) noexcept
	{
		ALIGNED_DATA(16) float a[4];
		ALIGNED_DATA(16) float b[4];
		_mm_store_ps(a, base);
		_mm_store_ps(b, power);
		Vector128 vResult = _mm_setr_ps(
			powf(a[0], b[0]),
			powf(a[1], b[1]),
			powf(a[2], b[2]),
			powf(a[3], b[3]));
		return vResult;

	}

	/****************************************************************************
	*                      Sin
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Sin(ConstVector128 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��Sin����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Sin(ConstVector128 vector) noexcept
	{
		// -PI <= theta <= PI�͈̔͂Ɏ��߂�
		Vector128 normalizedAngle = ModAngles(vector);

		// -PI/2 <= theta < PI/2�̎���sin�̑Ώ̐����g�p����
		// �p�x�̕����r�b�g���擾����
		__m128 sign = _mm_and_ps(normalizedAngle, VECTOR_128U_NEGATIVE_ZERO);

		// angle�����̏ꍇ��PI, ���̏ꍇ��-PI�𐶐�����
		__m128 piOrMinusPi = _mm_or_ps(VECTOR_128F_PI, sign);

		// |normalizedAngle|���擾����
		__m128 absoluteAngle = _mm_andnot_ps(sign, normalizedAngle);

		// ���ˊp�̌v�Z
		__m128 reflectedAngle = _mm_sub_ps(piOrMinusPi, normalizedAngle);

		// ���ی��ɂ��邩�𔻒肷��
		__m128 isFirstQuadrant = _mm_cmple_ps(absoluteAngle, VECTOR_128F_HALF_PI);

		// ���ی��ɂ���ꍇ�͂��̒l���g�p��, �����łȂ��ꍇ��0���擾����
		__m128 angleInFirstQuadrant = _mm_and_ps(isFirstQuadrant, normalizedAngle);

		// ���ی��ɂ���ꍇ�͔��ˊp���g�p��, �����łȂ��ꍇ��0���擾����
		__m128 angleInSecondQuadrant = _mm_andnot_ps(isFirstQuadrant, reflectedAngle);

		// �r�b�g�P�ʂ�OR����邱�Ƃ�, angle�����ی��ɂ���ꍇ�͂��̒l��, ���ی��ɂ���ꍇ�͔��ˊp���擾
		normalizedAngle = _mm_or_ps(angleInFirstQuadrant, angleInSecondQuadrant);

		__m128 angleSquared = _mm_mul_ps(normalizedAngle, normalizedAngle);

		// �������ߎ��̌v�Z
		// �e�C���[�W�J�̌W��
		const Vector128 sinCoefficient0 = VECTOR_128F_SIN_COEFFICIENTS0;
		const Vector128 sinCoefficient1 = VECTOR_128F_SIN_COEFFICIENTS1;
		__m128 constantsA = PERMUTE_PS(sinCoefficient0, _MM_SHUFFLE(3, 3, 3, 3));
		__m128 constantsB = PERMUTE_PS(sinCoefficient1, _MM_SHUFFLE(0, 0, 0, 0));

		// �e�C���[�W�J�̌��ʂ��擾����
		__m128 result = MultiplyAdd(constantsB, angleSquared, constantsA);

		constantsA = PERMUTE_PS(sinCoefficient0, _MM_SHUFFLE(2, 2, 2, 2));
		result = MultiplyAdd(result, angleSquared, constantsA);

		constantsA = PERMUTE_PS(sinCoefficient0, _MM_SHUFFLE(1, 1, 1, 1));
		result = MultiplyAdd(result, angleSquared, constantsA);

		constantsA = PERMUTE_PS(sinCoefficient0, _MM_SHUFFLE(0, 0, 0, 0));
		result = MultiplyAdd(result, angleSquared, constantsA);

		result = MultiplyAdd(result, angleSquared, VECTOR_128F_ONE);
		result = _mm_mul_ps(result, normalizedAngle);
		return result;
	}

	/****************************************************************************
	*                      Cos
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Cos(ConstVector128 vector) noexcepts
	*
	*  @brief     �S�Ă̗v�f��Cos����������̂�Ԃ��܂�
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Cos(ConstVector128 vector) noexcept
	{
		// -PI <= theta <= PI�͈̔͂Ɏ��߂�
		Vector128 normalizedAngle = ModAngles(vector);

		// -PI/2 <= theta < PI/2�̎���cos�̑Ώ̐����g�p����cos(y) = sign*cos(x).
		// �p�x�̕����r�b�g���擾����
		__m128 sign = _mm_and_ps(normalizedAngle, VECTOR_128U_NEGATIVE_ZERO);

		// angle�����̏ꍇ��PI, ���̏ꍇ��-PI�𐶐�����
		__m128 piOrMinusPi = _mm_or_ps(VECTOR_128F_PI, sign);

		// |normalizedAngle|���擾����
		__m128 absoluteAngle = _mm_andnot_ps(sign, normalizedAngle);

		// ���ˊp�̌v�Z
		__m128 reflectedAngle = _mm_sub_ps(piOrMinusPi, normalizedAngle);

		// ���ی��ɂ��邩�𔻒肷��
		__m128 isFirstQuadrant = _mm_cmple_ps(absoluteAngle, VECTOR_128F_HALF_PI);

		// ���ی��ɂ���ꍇ�͂��̒l���g�p��, �����łȂ��ꍇ��0���擾����
		__m128 angleInFirstQuadrant = _mm_and_ps(isFirstQuadrant, normalizedAngle);

		// ���ی��ɂ���ꍇ�͔��ˊp���g�p��, �����łȂ��ꍇ��0���擾����
		__m128 angleInSecondQuadrant = _mm_andnot_ps(isFirstQuadrant, reflectedAngle);

		// �r�b�g�P�ʂ�OR����邱�Ƃ�, angle�����ی��ɂ���ꍇ�͂��̒l��, ���ی��ɂ���ꍇ�͔��ˊp���擾
		normalizedAngle       = _mm_or_ps(angleInFirstQuadrant, angleInSecondQuadrant);
		angleInFirstQuadrant  = _mm_and_ps(isFirstQuadrant   , VECTOR_128F_ONE);
		angleInSecondQuadrant = _mm_andnot_ps(isFirstQuadrant, VECTOR_128F_NEGATIVE_ONE);
		sign = _mm_or_ps(angleInFirstQuadrant, angleInSecondQuadrant);

		__m128 angleSquared = _mm_mul_ps(normalizedAngle, normalizedAngle);

		// �������ߎ��̌v�Z
		// �e�C���[�W�J�̌W��
		const Vector128 cosCoefficient0 = VECTOR_128F_COS_COEFFICIENTS0;
		const Vector128 cosCoefficient1 = VECTOR_128F_COS_COEFFICIENTS1;
		__m128 constantsA = PERMUTE_PS(cosCoefficient0, _MM_SHUFFLE(3, 3, 3, 3));
		__m128 constantsB = PERMUTE_PS(cosCoefficient1, _MM_SHUFFLE(0, 0, 0, 0));

		// �e�C���[�W�J�̌��ʂ��擾����
		__m128 result = MultiplyAdd(constantsB, angleSquared, constantsA);

		constantsA = PERMUTE_PS(cosCoefficient0, _MM_SHUFFLE(2, 2, 2, 2));
		result = MultiplyAdd(result, angleSquared, constantsA);

		constantsA = PERMUTE_PS(cosCoefficient0, _MM_SHUFFLE(1, 1, 1, 1));
		result = MultiplyAdd(result, angleSquared, constantsA);

		constantsA = PERMUTE_PS(cosCoefficient0, _MM_SHUFFLE(0, 0, 0, 0));
		result = MultiplyAdd(result, angleSquared, constantsA);

		result = MultiplyAdd(result, angleSquared, VECTOR_128F_ONE);
		result = _mm_mul_ps(result, normalizedAngle);
		return result;
	}

	/****************************************************************************
	*                      Tan
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Tan(ConstVector128 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��Tan����������̂�Ԃ��܂�
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Tan(ConstVector128 vector) noexcept
	{
		static const Vector128f tanCoefficients0 = { { { 1.0f, -4.667168334e-1f, 2.566383229e-2f, -3.118153191e-4f } } };
		static const Vector128f tanCoefficients1 = { { { 4.981943399e-7f, -1.333835001e-1f, 3.424887824e-3f, -1.786170734e-5f } } };
		static const Vector128f tanConstants = { { { 1.570796371f, 6.077100628e-11f, 0.000244140625f, 0.63661977228f /*2 / Pi*/ } } };
		static const Vector128u mask         = { { { 0x1, 0x1, 0x1, 0x1 } } };

		// �萔�l�̎��O����
		Vector128 twoDivPi = SplatW(tanConstants.V);
		Vector128 zero     = Zero();
		Vector128 c0       = SplatX(tanConstants.V);
		Vector128 c1       = SplatY(tanConstants.V);
		Vector128 epsilon  = SplatZ(tanConstants.V);

		// 2 / PI�ŏ�Z��, ���̌��ʂ��l�̌ܓ�����. 
		Vector128 va = Multiply(vector, twoDivPi);
		va = Round(va);

		// va�̊e�v�f��c0, c1���g���ĕ␳���邱�Ƃ�, V�̊e�v�f���ł��߂�PI / 2����ǂꂾ������Ă��邩�����߂�
		Vector128 vc = NegativeMultiplySubtract(va, c0, vector);
		Vector128 vb = Abs(va);
		vc = NegativeMultiplySubtract(va, c1, vc);

		// vb��va�����������𔻒肷�邽�߂Ɏg�p����
		reinterpret_cast<__m128i*>(&vb)[0] = _mm_cvttps_epi32(vb);

		// �e�C���[�W�J�Ɏg�p����萔�̍쐬
		Vector128 vc2 = Multiply(vc, vc);
		Vector128 t7 = SplatW(tanCoefficients1.V);
		Vector128 t6 = SplatZ(tanCoefficients1.V);
		Vector128 t4 = SplatX(tanCoefficients1.V);
		Vector128 t3 = SplatW(tanCoefficients0.V);
		Vector128 t5 = SplatY(tanCoefficients1.V);
		Vector128 t2 = SplatZ(tanCoefficients0.V);
		Vector128 t1 = SplatY(tanCoefficients0.V);
		Vector128 t0 = SplatX(tanCoefficients0.V);

		Vector128 isEvenVB = AndInt(vb, mask.V);
		isEvenVB = EqualAsIntVectorEach(isEvenVB, zero);

		Vector128 n = MultiplyAdd(vc2, t7, t6);
		Vector128 d = MultiplyAdd(vc2, t4, t3);
		n = MultiplyAdd(vc2, n, t5);
		d = MultiplyAdd(vc2, d, t2);
		n = Multiply(vc2, n);
		d = MultiplyAdd(vc2, d, t1);
		n = MultiplyAdd(vc, n, vc);
		Vector128 nearZeroVC = InBoundsVectorEach(vc, epsilon);
		d = MultiplyAdd(vc2, d, t0);

		n = Select(n, vc, nearZeroVC);
		d = Select(d, VECTOR_128F_ONE.V, nearZeroVC);

		Vector128 r0 = Negate(n);
		Vector128 r1 = Divide(n, d);
		r0 = Divide(d, r0);

		Vector128 isZero = EqualVectorEach(vector, zero);

		Vector128 result = Select(r0, r1, isEvenVB);

		result = Select(result, zero, isZero);

		return result;

	}

	/****************************************************************************
	*                      SinCos
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SinCos(ConstVector128 input, Vector128* sin, Vector128* cos) noexcept
	*
	*  @brief     �S�Ă̗v�f�ɑ΂���sin������cos���������o���܂� 
	*
	*  @param[in] ConstVector128 : input
	*  @param[out] Vector128* sin
	*  @param[out] Vector128* cos
	*
	*  @return �@�@void
	*****************************************************************************/
	inline void SIMD_CALL_CONVENTION Vector128Utility::SinCos(ConstVector128 input, Vector128* sin, Vector128* cos) noexcept
	{
		// -PI <= theta <= PI�͈̔͂Ɏ��߂�
		Vector128 normalizedAngle = ModAngles(input);

		// -PI/2 <= theta < PI/2�̎���sin, cos�̑Ώ̐����g�p����sin(y) = sin(x), cos(y) = sign*cos(x).
		Vector128 sign = _mm_and_ps(normalizedAngle, VECTOR_128U_NEGATIVE_ZERO);

		// angle�����̏ꍇ��PI, ���̏ꍇ��-PI�𐶐�����
		__m128 piOrMinusPi = _mm_or_ps(VECTOR_128F_PI, sign);  // pi when x >= 0, -pi when x < 0

		// |absoluteAngle|���擾����
		__m128 absoluteAngle = _mm_andnot_ps(sign, normalizedAngle);  // |x|

		// ���ˊp�̌v�Z
		__m128 reflectedAngle = _mm_sub_ps(piOrMinusPi, normalizedAngle);

		// ���ی��ɂ��邩�𔻒肷��
		__m128 isFirstQuadrant = _mm_cmple_ps(absoluteAngle, VECTOR_128F_HALF_PI);

		// ���ی��ɂ���ꍇ�͂��̒l���g�p��, �����łȂ��ꍇ��0���擾����
		__m128 angleInFirstQuadrant  = _mm_and_ps(isFirstQuadrant, normalizedAngle);

		// ���ی��ɂ���ꍇ�͔��ˊp���g�p��, �����łȂ��ꍇ��0���擾����
		__m128 angleInSecondQuadrant = _mm_andnot_ps(isFirstQuadrant, reflectedAngle);

		// �r�b�g�P�ʂ�OR����邱�Ƃ�, angle�����ی��ɂ���ꍇ�͂��̒l��, ���ی��ɂ���ꍇ�͔��ˊp���擾
		normalizedAngle       = _mm_or_ps(angleInFirstQuadrant, angleInSecondQuadrant);
		angleInFirstQuadrant  = _mm_and_ps(isFirstQuadrant, VECTOR_128F_ONE);
		angleInSecondQuadrant = _mm_andnot_ps(isFirstQuadrant, VECTOR_128F_NEGATIVE_ONE);
		sign    = _mm_or_ps(angleInFirstQuadrant, angleInSecondQuadrant);

		__m128 angle2 = _mm_mul_ps(normalizedAngle, normalizedAngle);

		// Compute polynomial approximation of sine
		const Vector128 sinCoefficients1 = VECTOR_128F_SIN_COEFFICIENTS1;
		__m128 vConstantsB = PERMUTE_PS(sinCoefficients1, _MM_SHUFFLE(0, 0, 0, 0));
		const Vector128 sinCoefficients0 = VECTOR_128F_SIN_COEFFICIENTS0;
		__m128 vConstantsA = PERMUTE_PS(sinCoefficients0, _MM_SHUFFLE(3, 3, 3, 3));
		__m128 result = MultiplyAdd(vConstantsB, angle2, vConstantsA);

		vConstantsA = PERMUTE_PS(sinCoefficients0, _MM_SHUFFLE(2, 2, 2, 2));
		result     = MultiplyAdd(result, angle2, vConstantsA);

		vConstantsA = PERMUTE_PS(sinCoefficients0, _MM_SHUFFLE(1, 1, 1, 1));
		result = MultiplyAdd(result, angle2, vConstantsA);

		vConstantsA = PERMUTE_PS(sinCoefficients0, _MM_SHUFFLE(0, 0, 0, 0));
		result = MultiplyAdd(result, angle2, vConstantsA);

		result = MultiplyAdd(result, angle2, VECTOR_128F_ONE);
		result = _mm_mul_ps(result, normalizedAngle);
		*sin = result;

		// Compute polynomial approximation of cosine
		const Vector128 cosCoefficients1 = VECTOR_128F_COS_COEFFICIENTS1;
		vConstantsB = PERMUTE_PS(cosCoefficients1, _MM_SHUFFLE(0, 0, 0, 0));
		const Vector128 cosCoefficients0 = VECTOR_128F_COS_COEFFICIENTS0;
		vConstantsA = PERMUTE_PS(cosCoefficients0, _MM_SHUFFLE(3, 3, 3, 3));
		result = MultiplyAdd(vConstantsB, angle2, vConstantsA);

		vConstantsA = PERMUTE_PS(cosCoefficients0, _MM_SHUFFLE(2, 2, 2, 2));
		result      = MultiplyAdd(result, angle2, vConstantsA);

		vConstantsA = PERMUTE_PS(cosCoefficients0, _MM_SHUFFLE(1, 1, 1, 1));
		result      = MultiplyAdd(result, angle2, vConstantsA);

		vConstantsA = PERMUTE_PS(cosCoefficients0, _MM_SHUFFLE(0, 0, 0, 0));
		result     = MultiplyAdd(result, angle2, vConstantsA);

		result = MultiplyAdd(result, angle2, VECTOR_128F_ONE);
		result = _mm_mul_ps(result, sign);
		*cos = result;
	}

	/****************************************************************************
	*                      SinH
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SinH(ConstVector128 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��SinH����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SinH(ConstVector128 vector) noexcept
	{
		// ���R�ΐ��̒��2����e�ɕϊ�
		static const Vector128f scale = { { { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f } } }; // 1.0f / ln(2.0f)

		// sinh(x) = (e^x + e^-x) / 2 - 1�Ƃ��Čv�Z
		Vector128 v1 = MultiplyAdd(vector, scale.V, VECTOR_128F_NEGATIVE_ONE);
		Vector128 v2 = NegativeMultiplySubtract(vector, scale, VECTOR_128F_NEGATIVE_ONE);
		Vector128 e1 = Exp2(vector);
		Vector128 e2 = Exp2(vector);

		return _mm_sub_ps(e1, e2);
	}

	/****************************************************************************
	*                      CosH
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CosH(ConstVector128 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��CosH����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CosH(ConstVector128 vector) noexcept
	{
		// ���R�ΐ��̒��2����e�ɕϊ�
		static const Vector128f scale = { { { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f } } }; // 1.0f / ln(2.0f)

		Vector128 v1 = MultiplyAdd(vector, scale.V, VECTOR_128F_NEGATIVE_ONE);
		Vector128 v2 = NegativeMultiplySubtract(vector, scale.V, VECTOR_128F_NEGATIVE_ONE);
		Vector128 e1 = Exp2(vector);
		Vector128 e2 = Exp2(vector);

		return _mm_add_ps(e1, e2);
	}

	/****************************************************************************
	*                      TanH
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::TanH(ConstVector128 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��TanH����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::TanH(ConstVector128 vector) noexcept
	{
		static const Vector128f scale = { { { 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f } } }; // 2.0f / ln(2.0f)

		Vector128 exponential = _mm_mul_ps(vector, scale.V);
		exponential = Exp2(exponential);
		exponential = MultiplyAdd(exponential, VECTOR_128F_ONE_HALF.V, VECTOR_128F_ONE_HALF.V);
		exponential = _mm_div_ps(VECTOR_128F_ONE.V, exponential);
		return _mm_sub_ps(VECTOR_128F_ONE.V, exponential);
	}

	/****************************************************************************
	*                      ArcSin
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ArcSin(ConstVector128 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��ArcSin����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ArcSin(ConstVector128 vector) noexcept
	{
		__m128 isNonNegative  = _mm_cmpge_ps(vector, VECTOR_128F_ZERO); // 0�ȏ�ł��邩
		__m128 mValue         = _mm_sub_ps(VECTOR_128F_ZERO, vector);   // -vector
		__m128 absoluteVector = _mm_max_ps(vector, mValue);             // |vector|

		// (1-|V|)���v�Z���A���̐���sqrt������邽�߂Ƀ[���ɃN�����v����
		__m128 oneMinusValue      = _mm_sub_ps(VECTOR_128F_ONE, absoluteVector);
		__m128 clampOneMinusValue = _mm_max_ps(VECTOR_128F_ZERO, oneMinusValue);
		__m128 rootOneMinusValue  = _mm_sqrt_ps(clampOneMinusValue);

		// �e�C���[�W�J��p����arcsin�̋ߎ��l���v�Z����
		const Vector128 arcCoefficients1 = VECTOR_128F_ARC_COEFFICIENTS1;
		__m128 constantsB = PERMUTE_PS(arcCoefficients1, _MM_SHUFFLE(3, 3, 3, 3));
		__m128 constantsA = PERMUTE_PS(arcCoefficients1, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 t0 = MultiplyAdd(constantsB, absoluteVector, constantsA);

		constantsA = PERMUTE_PS(arcCoefficients1, _MM_SHUFFLE(1, 1, 1, 1));
		t0 = MultiplyAdd(t0, absoluteVector, constantsA);

		constantsA = PERMUTE_PS(arcCoefficients1, _MM_SHUFFLE(0, 0, 0, 0));
		t0 = MultiplyAdd(t0, absoluteVector, constantsA);

		const Vector128 arcCoefficients0 = VECTOR_128F_ARC_COEFFICIENTS0;
		constantsA = PERMUTE_PS(arcCoefficients0, _MM_SHUFFLE(3, 3, 3, 3));
		t0 = MultiplyAdd(t0, absoluteVector, constantsA);

		constantsA = PERMUTE_PS(arcCoefficients0, _MM_SHUFFLE(2, 2, 2, 2));
		t0 = MultiplyAdd(t0, absoluteVector, constantsA);

		constantsA = PERMUTE_PS(arcCoefficients0, _MM_SHUFFLE(1, 1, 1, 1));
		t0 = MultiplyAdd(t0, absoluteVector, constantsA);

		constantsA = PERMUTE_PS(arcCoefficients0, _MM_SHUFFLE(0, 0, 0, 0));
		t0 = MultiplyAdd(t0, absoluteVector, constantsA);
		t0 = _mm_mul_ps(t0, rootOneMinusValue);

		__m128 t1 = _mm_sub_ps(VECTOR_128F_PI, t0);
		t0 = _mm_and_ps(isNonNegative, t0);
		t1 = _mm_andnot_ps(isNonNegative, t1);
		t0 = _mm_or_ps(t0, t1);
		t0 = _mm_sub_ps(VECTOR_128F_HALF_PI, t0);
		return t0;
	}

	/****************************************************************************
	*                      ArcCos
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ArcCos(ConstVector128 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��ArcSin����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ArcCos(ConstVector128 vector) noexcept
	{
		__m128 isNonNegative  = _mm_cmpge_ps(vector, VECTOR_128F_ZERO); // 0�ȏ�ł��邩
		__m128 mValue         = _mm_sub_ps(VECTOR_128F_ZERO, vector);   // -vector
		__m128 absoluteVector = _mm_max_ps(vector, mValue);             // |vector|

		// (1-|V|)���v�Z���A���̐���sqrt������邽�߂Ƀ[���ɃN�����v����
		__m128 oneMinusValue      = _mm_sub_ps(VECTOR_128F_ONE, absoluteVector);
		__m128 clampOneMinusValue = _mm_max_ps(VECTOR_128F_ZERO, oneMinusValue);
		__m128 rootOneMinusValue  = _mm_sqrt_ps(clampOneMinusValue);

		// �e�C���[�W�J��p����arcsin�̋ߎ��l���v�Z����
		const Vector128 arcCoefficients1 = VECTOR_128F_ARC_COEFFICIENTS1;
		__m128 constantsB = PERMUTE_PS(arcCoefficients1, _MM_SHUFFLE(3, 3, 3, 3));
		__m128 constantsA = PERMUTE_PS(arcCoefficients1, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 t0 = MultiplyAdd(constantsB, absoluteVector, constantsA);

		constantsA = PERMUTE_PS(arcCoefficients1, _MM_SHUFFLE(1, 1, 1, 1));
		t0 = MultiplyAdd(t0, absoluteVector, constantsA);

		constantsA = PERMUTE_PS(arcCoefficients1, _MM_SHUFFLE(0, 0, 0, 0));
		t0 = MultiplyAdd(t0, absoluteVector, constantsA);

		const Vector128 arcCoefficients0 = VECTOR_128F_ARC_COEFFICIENTS0;
		constantsA = PERMUTE_PS(arcCoefficients0, _MM_SHUFFLE(3, 3, 3, 3));
		t0 = MultiplyAdd(t0, absoluteVector, constantsA);

		constantsA = PERMUTE_PS(arcCoefficients0, _MM_SHUFFLE(2, 2, 2, 2));
		t0 = MultiplyAdd(t0, absoluteVector, constantsA);

		constantsA = PERMUTE_PS(arcCoefficients0, _MM_SHUFFLE(1, 1, 1, 1));
		t0 = MultiplyAdd(t0, absoluteVector, constantsA);

		constantsA = PERMUTE_PS(arcCoefficients0, _MM_SHUFFLE(0, 0, 0, 0));
		t0 = MultiplyAdd(t0, absoluteVector, constantsA);
		t0 = _mm_mul_ps(t0, rootOneMinusValue);

		__m128 t1 = _mm_sub_ps(VECTOR_128F_PI, t0);
		t0 = _mm_and_ps(isNonNegative, t0);
		t1 = _mm_andnot_ps(isNonNegative, t1);
		t0 = _mm_or_ps(t0, t1);
		return t0;
	}

	/****************************************************************************
	*                      ArcTan
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ArcTan(ConstVector128 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��ArcTan����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ArcTan(ConstVector128 vector) noexcept
	{
		__m128 absoluteVector = Abs(vector);                           // |vector|
		__m128 inverseVector  = _mm_div_ps(VECTOR_128F_ONE, vector);   // 1 / vector
		__m128 comparison     = _mm_cmpgt_ps(vector, VECTOR_128F_ONE); // vector > 1
		__m128 select0        = _mm_and_ps(comparison, VECTOR_128F_ONE);
		__m128 select1        = _mm_andnot_ps(comparison, VECTOR_128F_NEGATIVE_ONE);
		__m128 sign           = _mm_or_ps(select0, select1);           // vector�̕���
		comparison = _mm_cmple_ps(absoluteVector, VECTOR_128F_ONE);    // |vector| < 1
		select0    = _mm_and_ps(comparison, VECTOR_128F_ZERO);
		select1    = _mm_andnot_ps(comparison, sign);
		__m128 x   = _mm_or_ps(select0, select1);
		__m128 x2  = _mm_mul_ps(x, x);

		// �e�C���[�W�J��p����arcsin�̋ߎ��l���v�Z����
		const Vector128 arcTanCoefficients1 = VECTOR_128F_ATAN_COEFFICIENTS1;
		__m128 constantsB = PERMUTE_PS(arcTanCoefficients1, _MM_SHUFFLE(3, 3, 3, 3));
		__m128 constantsA = PERMUTE_PS(arcTanCoefficients1, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 result = MultiplyAdd(constantsB, x2, constantsA);

		constantsA = PERMUTE_PS(arcTanCoefficients1, _MM_SHUFFLE(1, 1, 1, 1));
		result = MultiplyAdd(result, x2, constantsA);

		constantsA = PERMUTE_PS(arcTanCoefficients1, _MM_SHUFFLE(0, 0, 0, 0));
		result = MultiplyAdd(result, x2, constantsA);

		const Vector128 arcTanCoefficients0 = VECTOR_128F_ATAN_COEFFICIENTS0;
		constantsA = PERMUTE_PS(arcTanCoefficients0, _MM_SHUFFLE(3, 3, 3, 3));
		result = MultiplyAdd(result, x2, constantsA);

		constantsA = PERMUTE_PS(arcTanCoefficients0, _MM_SHUFFLE(2, 2, 2, 2));
		result = MultiplyAdd(result, x2, constantsA);

		constantsA = PERMUTE_PS(arcTanCoefficients0, _MM_SHUFFLE(1, 1, 1, 1));
		result = MultiplyAdd(result, x2, constantsA);

		constantsA = PERMUTE_PS(arcTanCoefficients0, _MM_SHUFFLE(0, 0, 0, 0));
		result = MultiplyAdd(result, x2, constantsA);

		result = MultiplyAdd(result, x2, VECTOR_128F_ONE);

		result = _mm_mul_ps(result, x);
		__m128 result1 = _mm_mul_ps(sign, VECTOR_128F_HALF_PI);
		result1 = _mm_sub_ps(result1, result);

		comparison = _mm_cmpeq_ps(sign, VECTOR_128F_ZERO);
		select0 = _mm_and_ps(comparison, result);
		select1 = _mm_andnot_ps(comparison, result1);
		result = _mm_or_ps(select0, select1);
		return result;
	}

	/****************************************************************************
	*                      ArcTan2
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ArcTan2(ConstVector128 vector) noexcept
	*
	*  @brief     �S�Ă̗v�f��ArcTan2����������̂�Ԃ��܂�[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ArcTan2(ConstVector128 y, ConstVector128 x) noexcept
	{
		static const Vector128f ATan2Constants = { { { GM_PI_FLOAT, GM_PI_DIV2_FLOAT, GM_PI_DIV4_FLOAT, GM_PI_FLOAT * 3.0f / 4.0f } } };

		Vector128 Zero = Vector128Utility::Zero();
		Vector128 ATanResultValid = Vector128Utility::TrueIntMask();

		Vector128 Pi = Vector128Utility::SplatX(ATan2Constants);
		Vector128 PiOverTwo = Vector128Utility::SplatY(ATan2Constants);
		Vector128 PiOverFour = Vector128Utility::SplatZ(ATan2Constants);
		Vector128 ThreePiOverFour = Vector128Utility::SplatW(ATan2Constants);

		Vector128 YEqualsZero = Vector128Utility::EqualVectorEach(y, Zero);
		Vector128 XEqualsZero = Vector128Utility::EqualVectorEach(x, Zero);
		Vector128 XIsPositive = Vector128Utility::AndInt(x, VECTOR_128U_NEGATIVE_ZERO.V);
		XIsPositive = Vector128Utility::EqualAsIntVectorEach(XIsPositive, Zero);
		Vector128 YEqualsInfinity = Vector128Utility::IsInfiniteVectorEach(y);
		Vector128 XEqualsInfinity = Vector128Utility::IsInfiniteVectorEach(x);

		Vector128 YSign = Vector128Utility::AndInt(y, VECTOR_128U_NEGATIVE_ZERO.V);
		Pi = Vector128Utility::OrInt(Pi, YSign);
		PiOverTwo = Vector128Utility::OrInt(PiOverTwo, YSign);
		PiOverFour = Vector128Utility::OrInt(PiOverFour, YSign);
		ThreePiOverFour = Vector128Utility::OrInt(ThreePiOverFour, YSign);

		Vector128 R1 = Vector128Utility::Select(Pi, YSign, XIsPositive);
		Vector128 R2 = Vector128Utility::Select(ATanResultValid, PiOverTwo, XEqualsZero);
		Vector128 R3 = Vector128Utility::Select(R2, R1, YEqualsZero);
		Vector128 R4 = Vector128Utility::Select(ThreePiOverFour, PiOverFour, XIsPositive);
		Vector128 R5 = Vector128Utility::Select(PiOverTwo, R4, XEqualsInfinity);
		Vector128 Result = Vector128Utility::Select(R3, R5, YEqualsInfinity);
		ATanResultValid = Vector128Utility::EqualAsIntVectorEach(Result, ATanResultValid);

		Vector128 V = Vector128Utility::Divide(y, x);

		Vector128 R0 = Vector128Utility::ArcTan(V);

		R1 = Vector128Utility::Select(Pi, VECTOR_128U_NEGATIVE_ZERO.V, XIsPositive);
		R2 = Vector128Utility::Add(R0, R1);

		return Vector128Utility::Select(Result, R2, ATanResultValid);

	}

	/****************************************************************************
	*                       Lerp
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Lerp(ConstVector128 start, ConstVector128 end, const float t) noexcept
	*
	*  @brief     ���`��Ԃ�Ԃ��܂�
	*
	*  @param[in] ConstVector128  t = 0�̎��̒l
	*  @param[in] ConstVector128  t = 1�̎��̒l
	*  @param[in] float  t : ���`��Ԃ̊���
	*
	*  @return �@�@Vector
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Lerp(ConstVector128 start, ConstVector128 end, const float t) noexcept
	{
		Vector128 diff = _mm_sub_ps(end, start);
		Vector128 scale = _mm_set_ps1(t);
		return MultiplyAdd(diff, scale, start);
	}

	/****************************************************************************
	*                       LerpV
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LerpV(ConstVector128 start, ConstVector128 end, const float t) noexcept
	*
	*  @brief     ���`��Ԃ�Ԃ��܂�
	*
	*  @param[in] ConstVector128  t = 0�̎��̒l
	*  @param[in] ConstVector128  t = 1�̎��̒l
	*  @param[in] ConstVector128  t : ���`��Ԃ̊���
	*
	*  @return �@�@Vector
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LerpV(ConstVector128 start, ConstVector128 end, ConstVector128 t) noexcept
	{
		Vector128 diff = _mm_sub_ps(end, start);
		return MultiplyAdd(diff, t, start);
	}

	/****************************************************************************
	*                       Hermite
	****************************************************************************/
	/* @fn        __forceinline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Hermite(ConstVector128 startPosition, ConstVector128 startTangent, ConstVector128 endPosition, ConstVector128 endTangent, const float t) noexcept
	*
	*  @brief     �G���~�[�g��Ԃ�Ԃ��܂�
	*
	*  @param[in]  �n�_�̈ʒu
	   @param[in]  �n�_�ɂ�����X�v���C���̐ڐ���\���x�N�g��
	   @param[in]  �I�_�̈ʒu
	   @param[in]  �I�_�ɂ�����X�v���C���̐ڐ���\���x�N�g��
	   @param[in]  t : �G���~�[�g��Ԃ̊���
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Hermite(ConstVector128 startPosition, ConstVector128 startTangent, ConstVector128 endPosition, ConstVector128 endTangent, const float t) noexcept
	{
		// Result = (2 * t^3 - 3 * t^2 + 1) * Position0 +
        //          (t^3 - 2 * t^2 + t) * Tangent0 +
        //          (-2 * t^3 + 3 * t^2) * Position1 +
        //          (t^3 - t^2) * Tangent1
		const float t2 = t * t;
		const float t3 = t * t2;

		Vector128 p0 = _mm_set_ps1(2.0f * t3 - 3.0f * t2 + 1.0f);
		Vector128 t0 = _mm_set_ps1(t3 - 2.0f * t2 + t);
		Vector128 p1 = _mm_set_ps1(-2.0f * t3 + 3.0f * t2);
		Vector128 t1 = _mm_set_ps1(t3 - t2);
		
		Vector128 result = _mm_mul_ps(p0, startPosition);
		result = MultiplyAdd(startTangent, t0, result);
		result = MultiplyAdd(endPosition, p1, result);
		result = MultiplyAdd(endTangent , t1, result);
		return result;
	}

	/****************************************************************************
	*                       HermiteV
	****************************************************************************/
	/* @fn        __forceinline Vector128 SIMD_CALL_CONVENTION Vector128Utility::HermiteV(ConstVector128 startPosition, ConstVector128 startTangent, ConstVector128 endPosition, ConstVector128 endTangent, const float t) noexcept
	*
	*  @brief     �G���~�[�g��Ԃ�Ԃ��܂�
	*
	*  @param[in]  �n�_�̈ʒu
	   @param[in]  �n�_�ɂ�����X�v���C���̐ڐ���\���x�N�g��
	   @param[in]  �I�_�̈ʒu
	   @param[in]  �I�_�ɂ�����X�v���C���̐ڐ���\���x�N�g��
	   @param[in]  t : �G���~�[�g��Ԃ̊���
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::HermiteV(ConstVector128 startPosition, ConstVector128 startTangent, ConstVector128 endPosition, ConstVector128 endTangent, ConstVector128 t) noexcept
	{
		static const Vector128f CatMulT2 = { { { -3.0f, -2.0f, 3.0f, -1.0f } } };
		static const Vector128f CatMulT3 = { { { 2.0f, 1.0f, -2.0f, 1.0f } } };

		Vector128 t2 = _mm_mul_ps(t, t);
		Vector128 t3 = _mm_mul_ps(t, t2);
		// Mul by the constants against t^2
		t2 = _mm_mul_ps(t2, CatMulT2);
		// Mul by the constants against t^3
		t3 = MultiplyAdd(t3, CatMulT3, t2);
		// T3 now has the pre-result.
		// I need to add t.y only
		t2 = _mm_and_ps(t, VECTOR_128U_MASK_Y);
		t3 = _mm_add_ps(t3, t2);
		// Add 1.0f to x
		t3 = _mm_add_ps(t3, VECTOR_128F_IDENTITY_R0);
		// Now, I have the constants created
		// Mul the x constant to Position0
		Vector128 result = PERMUTE_PS(t3, _MM_SHUFFLE(0, 0, 0, 0));
		result = _mm_mul_ps(result, startPosition);
		// Mul the y constant to Tangent0
		t2 = PERMUTE_PS(t3, _MM_SHUFFLE(1, 1, 1, 1));
		result = MultiplyAdd(t2, startTangent, result);
		// Mul the z constant to Position1
		t2 = PERMUTE_PS(t3, _MM_SHUFFLE(2, 2, 2, 2));
		result = MultiplyAdd(t2, endPosition, result);
		// Mul the w constant to Tangent1
		t3 = PERMUTE_PS(t3, _MM_SHUFFLE(3, 3, 3, 3));
		result = MultiplyAdd(t3, endTangent, result);
		return result;
	}

	/****************************************************************************
	*                       CatMullRom
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CatMullRom(ConstVector128 position0, ConstVector128 position1, ConstVector128 position2, ConstVector128 position3, const float t) noexcept
	*
	*  @brief     CatMull-Rom�X�v���C����Ԃ̌��ʂ�Ԃ��܂��B4�̐���_��S�Ēʂ�X�v���C�����
	*
	*  @param[in]  ����_1
	   @param[in]  ����_2
	   @param[in]  ����_3
	   @param[in]  ����_4
	   @param[in]  t : ��Ԃ̊���
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CatMullRom(ConstVector128 position0, ConstVector128 position1, ConstVector128 position2, ConstVector128 position3, const float t) noexcept
	{
		const float t2 = t * t;
		const float t3 = t * t2;

		Vector128 p0 = _mm_set_ps1((-t3 + 2.0f * t2 - t) * 0.5f);
		Vector128 p1 = _mm_set_ps1((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
		Vector128 p2 = _mm_set_ps1((-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
		Vector128 p3 = _mm_set_ps1((t3 - t2) * 0.5f);

		p1 = _mm_mul_ps(position1, p1);
		p0 = MultiplyAdd(position0, p0, p1);
		p3 = _mm_mul_ps(position3, p3);
		p2 = MultiplyAdd(position2, p2, p3);
		p0 = _mm_add_ps(p0, p2);
		return p0;
	}

	/****************************************************************************
	*                       CatMullRom
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CatMullRom(ConstVector128 position0, ConstVector128 position1, ConstVector128 position2, ConstVector128 position3, const float t) noexcept
	*
	*  @brief     CatMull-Rom�X�v���C����Ԃ̌��ʂ�Ԃ��܂��B4�̐���_��S�Ēʂ�X�v���C�����
	*
	*  @param[in]  ����_1
	   @param[in]  ����_2
	   @param[in]  ����_3
	   @param[in]  ����_4
	   @param[in]  t : ��Ԃ̊���
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CatMullRomV(ConstVector128 position0, ConstVector128 position1, ConstVector128 position2, ConstVector128 position3, ConstVector128 t) noexcept
	{
		static const Vector128f Catmul2 = { { { 2.0f, 2.0f, 2.0f, 2.0f } } };
		static const Vector128f Catmul3 = { { { 3.0f, 3.0f, 3.0f, 3.0f } } };
		static const Vector128f Catmul4 = { { { 4.0f, 4.0f, 4.0f, 4.0f } } };
		static const Vector128f Catmul5 = { { { 5.0f, 5.0f, 5.0f, 5.0f } } };
		// Cache T^2 and T^3
		Vector128 t2 = _mm_mul_ps(t, t);
		Vector128 t3 = _mm_mul_ps(t, t2);
		// Perform the Position0 term
		Vector128 vResult = _mm_add_ps(t2, t2);
		vResult = _mm_sub_ps(vResult, t);
		vResult = _mm_sub_ps(vResult, t3);
		vResult = _mm_mul_ps(vResult, position0);
		// Perform the Position1 term and add
		Vector128 vTemp = _mm_mul_ps(t3, Catmul3);
		vTemp = NegativeMultiplySubtract(t2, Catmul5, vTemp);
		vTemp = _mm_add_ps(vTemp, Catmul2);
		vResult = MultiplyAdd(vTemp, position1, vResult);
		// Perform the Position2 term and add
		vTemp = _mm_mul_ps(t2, Catmul4);
		vTemp = NegativeMultiplySubtract(t3, Catmul3, vTemp);
		vTemp = _mm_add_ps(vTemp, t);
		vResult = MultiplyAdd(vTemp, position2, vResult);
		// Position3 is the last term
		t3 = _mm_sub_ps(t3, t2);
		vResult = MultiplyAdd(t3, position3, vResult);
		// Multiply by 0.5f and exit
		vResult = _mm_mul_ps(vResult, VECTOR_128F_ONE_HALF);
		return vResult;
	}

	/****************************************************************************
	*                      BaryCentric
	****************************************************************************/
	/* @fn        __forceinline Vector128 SIMD_CALL_CONVENTION Vector128Utility::BaryCentric(ConstVector128 position0, ConstVector128 position1, ConstVector128 position2, const float f, const float g) noexcept
	*
	*  @brief     BaryCentric �O�p�`�̏d�S�d�݈ʒu���Z�o
	*              (f>=0 && g>=0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�͎O�p�`�� Position0>Position1>Position2 �̓����ɂ���܂�>�B
				   (f==0 && g>=0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�� Position0>Position2 �s�ڂɂ���܂�>�B
				   (f>=0 && g==0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�� Position0>Position1> �s�ڂɂ���܂��B
				   (f>=0 && g>=0 && 1-f-g==0) �̏ꍇ�A�|�C���g�� Position1>Position2 �s�ڂɂ���܂�>�B
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::BaryCentric(ConstVector128 position0, ConstVector128 position1, ConstVector128 position2, const float f, const float g) noexcept
	{
		Vector128 r1 = _mm_sub_ps(position1, position0);
		Vector128 r2 = _mm_sub_ps(position2, position0);
		Vector128 sf = _mm_set_ps1(f);
		r1 = MultiplyAdd(r1, sf, position0);
		Vector128 sg = _mm_set_ps1(g);
		return MultiplyAdd(r2, sg, r1);
	}

	/****************************************************************************
	*                      BaryCentricV
	****************************************************************************/
	/* @fn        __forceinline Vector128 SIMD_CALL_CONVENTION Vector128Utility::BaryCentricV(ConstVector128 position0, ConstVector128 position1, ConstVector128 position2, const float f, const float g) noexcept
	*
	*  @brief     BaryCentric �O�p�`�̏d�S�d�݈ʒu���Z�o
	*              (f>=0 && g>=0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�͎O�p�`�� Position0>Position1>Position2 �̓����ɂ���܂�>�B
				   (f==0 && g>=0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�� Position0>Position2 �s�ڂɂ���܂�>�B
				   (f>=0 && g==0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�� Position0>Position1> �s�ڂɂ���܂��B
				   (f>=0 && g>=0 && 1-f-g==0) �̏ꍇ�A�|�C���g�� Position1>Position2 �s�ڂɂ���܂�>�B
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::BaryCentricV(ConstVector128 position0, ConstVector128 position1, ConstVector128 position2, ConstVector128 f, ConstVector128 g) noexcept
	{
		Vector128 r1 = _mm_sub_ps(position1, position0);
		Vector128 r2 = _mm_sub_ps(position2, position0);
		r1 = MultiplyAdd(r1, f, position0);
		return MultiplyAdd(r2, g, r1);
	}

	/****************************************************************************
	*                       Min
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Min(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     �S�Ă̗v�f�ɂ��čŏ��l�ƂȂ����I�����܂�
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@Vector
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Min(ConstVector128 left, ConstVector128 right) noexcept
	{
		return _mm_min_ps(left, right);
	}

	/****************************************************************************
	*                       Max
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Max(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     �S�Ă̗v�f�ɂ��čő�l�ƂȂ����I�����܂�
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Max(ConstVector128 left, ConstVector128 right) noexcept
	{
		return _mm_max_ps(left, right);
	}

	/****************************************************************************
	*                       Round
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Round(ConstVector128 vector) noexcept
	*
	*  @brief     �ł��߂������Ɋۂ߂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Round(ConstVector128 vector) noexcept
	{
		__m128 sign = _mm_and_ps(vector, VECTOR_128U_NEGATIVE_ZERO);

		// magic��vector�̕������ێ�����, ����������0�ɂ���
		__m128 magic = _mm_or_ps(VECTOR_128F_NO_FRACTION, sign);

		// �����������ۂ߂�
		__m128 r1 = _mm_add_ps(vector, magic);
		r1 = _mm_sub_ps(r1, magic);

		// vector�̐�Βl���擾
		__m128 r2 = _mm_and_ps(vector, VECTOR_128U_MASK_ABS);

		// mask��, r2��1.0�����̏ꍇ��true��Ԃ�
		__m128 mask = _mm_cmple_ps(r2, VECTOR_128F_NO_FRACTION);

		// mask��false�̏ꍇ��vector��Ԃ�
		r2 = _mm_andnot_ps(mask, vector);

		// r1��mask��true�̏ꍇ��R1��Ԃ��܂�
		r1 = _mm_and_ps(r1, mask);

		// �r���I�_���a�ɂ��, V��1.0�����̏ꍇ��R1���A�����łȂ��ꍇ��R2��Ԃ��܂��B
		return _mm_xor_ps(r1, r2);
	}

	/****************************************************************************
	*                       Truncate
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Truncate(ConstVector128 vector) noexcept
	*
	*  @brief     �؂�̂� (�w�肵�����l�̒[����؂�̂Ă������Ɋۂ߂�B���������0�̕����ɂ��鎟�̐���
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Truncate(ConstVector128 vector) noexcept
	{
		// NAN, INF, 8388608���傫�����l�������ɂ�, �}�X�L���O���g�p����
		// ��Βl���擾
		__m128i test = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128U_MASK_ABS);

		// 8388608���傫�����ǂ����̃e�X�g (���ׂĂ̕��������_���ANAN�AINF�Ȃ�)
		test = _mm_cmplt_epi32(test, VECTOR_128F_NO_FRACTION);

		// �؂�̂ĂŊۂ߂邽�߂ɁAint�ɕϊ�����float�ɖ߂��B
		__m128i intVector = _mm_cvttps_epi32(vector);

		Vector128 result = _mm_cvtepi32_ps(intVector);

		// 8388608�����̐��l�͂��ׂ�int�^�Ɋۂ߂�
		result = _mm_and_ps(result, _mm_castsi128_ps(test));

		// ����ȊO�̏ꍇ�́A���̒l���g�p����
		test = _mm_andnot_si128(test, _mm_castps_si128(vector));
		result = _mm_or_ps(result, _mm_castsi128_ps(test));
		return result;
	}

	/****************************************************************************
	*                       Floor
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Floor(ConstVector128 vector) noexcept
	*
	*  @brief     ���̕����Ɋۂ߂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Floor(ConstVector128 vector) noexcept
	{
		// NAN, INF, 8388608���傫�����l�������ɂ�, �}�X�L���O���g�p����
		// ��Βl���擾
		__m128i test = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128U_MASK_ABS);

		// 8388608���傫�����ǂ����̃e�X�g (���ׂĂ̕��������_���ANAN�AINF�Ȃ�)
		test = _mm_cmplt_epi32(test, VECTOR_128F_NO_FRACTION);

		// �؂�̂ĂŊۂ߂邽�߂ɁAint�ɕϊ�����float�ɖ߂��B
		__m128i intVector = _mm_cvttps_epi32(vector);

		Vector128 result = _mm_cvtepi32_ps(intVector);

		__m128 larger = _mm_cmpgt_ps(result, vector);

		// 0 -> 0, 0xffffffff -> -1.0f
		larger = _mm_cvtepi32_ps(_mm_castps_si128(larger));
		result = _mm_add_ps(result, larger);

		// 8388608�����̐��l�͂��ׂ�int�^�Ɋۂ߂�
		result = _mm_and_ps(result, _mm_castsi128_ps(test));

		// ����ȊO�̏ꍇ�́A���̒l���g�p����
		test = _mm_andnot_si128(test, _mm_castps_si128(vector));
		result = _mm_or_ps(result, _mm_castsi128_ps(test));
		return result;
	}

	/****************************************************************************
	*                      Ceiling
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Ceiling(ConstVector128 vector) noexcept
	*
	*  @brief     �؂�グ ��������Ő��̕����ɂ��鎟�̐����Ɋۂ߂�
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Ceiling(ConstVector128 vector) noexcept
	{
		// NAN, INF, 8388608���傫�����l�������ɂ�, �}�X�L���O���g�p����
		// ��Βl���擾
		__m128i test = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128U_MASK_ABS);

		// 8388608���傫�����ǂ����̃e�X�g (���ׂĂ̕��������_���ANAN�AINF�Ȃ�)
		test = _mm_cmplt_epi32(test, VECTOR_128F_NO_FRACTION);

		// �؂�̂ĂŊۂ߂邽�߂ɁAint�ɕϊ�����float�ɖ߂��B
		__m128i intVector = _mm_cvttps_epi32(vector);

		Vector128 result = _mm_cvtepi32_ps(intVector);

		__m128 smaller = _mm_cmplt_ps(result, vector);

		// 0 -> 0, 0xffffffff -> -1.0f
		smaller = _mm_cvtepi32_ps(_mm_castps_si128(smaller));
		result = _mm_add_ps(result, smaller);

		// 8388608�����̐��l�͂��ׂ�int�^�Ɋۂ߂�
		result = _mm_and_ps(result, _mm_castsi128_ps(test));

		// ����ȊO�̏ꍇ�́A���̒l���g�p����
		test = _mm_andnot_si128(test, _mm_castps_si128(vector));
		result = _mm_or_ps(result, _mm_castsi128_ps(test));
		return result;
	}

	/****************************************************************************
	*                       Clamp
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Clamp(ConstVector128 vector, ConstVector128 min, ConstVector128 max) noexcept
	*
	*  @brief     �w��͈͓��Ő��l��ݒ肷��
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] ConstVector128 min
	*  @param[in] ConstVector128 max
	*
	*  @return �@�@Vector
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Clamp(ConstVector128 vector, ConstVector128 min, ConstVector128 max) noexcept
	{
		Check(LessOrEqualVector4(min, max));
		Vector128 result = _mm_max_ps(min, vector);
		return _mm_min_ps(max, result);
	}

	/****************************************************************************
	*                       Saturate
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Saturate(ConstVector128 vector)
	*
	*  @brief     1�ȏ�ɂ͍s���Ȃ��悤�ɂ���
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Saturate(ConstVector128 vector) noexcept
	{
		Vector128 result = _mm_max_ps(vector, VECTOR_128F_ZERO);
		return _mm_min_ps(result, VECTOR_128F_ONE);
	}

	/****************************************************************************
	*                       Abs
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Saturate(ConstVector128 vector)
	*
	*  @brief     ��Βl
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return �@�@Vector
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Abs(ConstVector128 vector) noexcept
	{
		Vector128 result = _mm_setzero_ps();
		result = _mm_sub_ps(result, vector);
		result = _mm_max_ps(result, vector);
		return result;
	}

	#pragma endregion Math

	#pragma region Angle
	/****************************************************************************
	*                       ModAngles
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ModAngles(ConstVector128 vector)
	*
	*  @brief      - pi <= theta <= pi�͈̔͂Ɏ��߂�悤�ɂ��܂�.
	*
	*  @param[in] ConstVector128 angles : �p�xrad
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ModAngles(ConstVector128 angles) noexcept
	{
		// PI <= angle < PI �ƂȂ�悤�ɁA�^����ꂽ�p�x�͈̔͂̃��W�������Z���s��
		Vector128 result = _mm_mul_ps(angles, VECTOR_128F_RECIPROCAL_2PI.V);

		// �ۂߏ��������G�Ȃ��߁A�C�����C���֐����g�p����B
		result = Round(result);
		return NegativeMultiplySubtract(result, VECTOR_128F_2PI, angles);
	}

	/****************************************************************************
	*                       AddAngles
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::AddAngles(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     - pi <= theta <= pi�͈̔͂Ɏ��߂�悤�ɂ�����ő����Z���s���܂�.
	*
	*  @param[in] ConstVector128 left angles : �p�xrad
	*  @param[in] ConstVector128 right angles : �p�xrad
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::AddAngles(ConstVector128 left, ConstVector128 right) noexcept
	{
		// �p�x�̏����ȍ���
		Vector128 result = _mm_add_ps(left, right);

		// -Pi�������������𔻒肷��
		Vector128 offset = _mm_cmplt_ps(result, VECTOR_128F_NEGATIVE_PI);
		offset = _mm_and_ps(offset, VECTOR_128F_2PI);

		// -Pi�����S�Ă̗v�f�ŏ������ꍇ, 2Pi��������
		result = _mm_add_ps(result, offset);

		// Pi�����傫���Ȃ��Ă��܂��Ă��邩
		offset = _mm_cmpge_ps(result, VECTOR_128F_PI);
		offset = _mm_and_ps(offset, VECTOR_128F_2PI);

		// pi�����傫���ꍇ��2pi��S�Ă̗v�f�ň���
		result = _mm_sub_ps(result, offset);
		return result;
	}

	/****************************************************************************
	*                       SubtractAngles
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ModAngles(ConstVector128 vector)
	*
	*  @brief     - pi <= theta <= pi�͈̔͂Ɏ��߂�悤�ɂ�����ň����Z���s���܂�.
	*
	*  @param[in] ConstVector128 left angles : �p�xrad
	*  @param[in] ConstVector128 right angles : �p�xrad
	*
	*  @return �@�@Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SubtractAngles(ConstVector128 left, ConstVector128 right) noexcept
	{
		// �p�x�̏����ȍ���
		Vector128 result = _mm_sub_ps(left, right);

		// -Pi�������������𔻒肷��
		Vector128 offset = _mm_cmplt_ps(result, VECTOR_128F_NEGATIVE_PI);
		offset           = _mm_and_ps(offset, VECTOR_128F_2PI);

		// -Pi�����S�Ă̗v�f�ŏ������ꍇ, 2Pi��������
		result = _mm_add_ps(result, offset);

		// Pi�����傫���Ȃ��Ă��܂��Ă��邩
		offset = _mm_cmpge_ps(result, VECTOR_128F_PI);
		offset = _mm_and_ps(offset, VECTOR_128F_2PI);

		// pi�����傫���ꍇ��2pi��S�Ă̗v�f�ň���
		result = _mm_sub_ps(result, offset);
		return result;
	}
	#pragma endregion Angle
#pragma endregion Implement
}



#endif PLATFORM_CPU_INSTRUCTION_SSE

#endif GM_SIMD_VECTOR_128_SSE_HPP