//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdVector128SSE.hpp
///             @brief  SSEによるSimd演算です.
///                     Vector128型の実装 
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
// SSEのインクルード
#include <intrin.h>
#include <xmmintrin.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gm::simd::sse
{
	/*----------------------------------------------------------------------
	*         128 bitの整数型もfloat型も格納可能なベクトルを作成する
	*----------------------------------------------------------------------*/
	using Vector128 = __m128;

	/*----------------------------------------------------------------------
	*        定数としての変換用, floatポインタに変換したり, m128の各型に変換したり
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
	*        定数としての変換用, floatポインタに変換したり, m128の各型に変換したり
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
	*        定数としての変換用, floatポインタに変換したり, m128の各型に変換したり
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

	// 定数式でないとエラーとなるため
	#if PLATFORM_CPU_INSTRUCTION_AVX
		#define PERMUTE_PS(v, c) _mm_permute_ps((v), c)
	#else
		#define PERMUTE_PS(v, c) _mm_shuffle_ps((v), (v), c)
	#endif
	/****************************************************************************
	*				  			  Vector128Utility
	****************************************************************************/
	/* @class     Vector128Utility
	*  @brief     SSEのバージョンによるSimd演算
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
		*  @brief : ゼロ初期化したVector128を返す関数
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Zero() noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全てのビットがtrue (1)に設定されているVector128を返す
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION TrueIntMask() noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全てのビットがfalse (0)に設定されているVector128を返す Zero()と同じ
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION FalseIntMask() noexcept;

		/*----------------------------------------------------------------------
		*  @brief : float4つからVector128クラスを生成する
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Set(const float x, const float y, const float z, const float w) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : float1つからVector128クラスを生成する
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Set(const float value) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : Int4つからVector128クラスを生成する
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION SetInt(const gu::uint32 x, const gu::uint32 y, const gu::uint32 z, const gu::uint32 w) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : Floatを使ってVectorのX(, Y, Z, W)要素に値を代入する
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION SetX(ConstVector128 vector, const float x) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION SetY(ConstVector128 vector, const float y) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION SetZ(ConstVector128 vector, const float z) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION SetW(ConstVector128 vector, const float w) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ベクトルを使ってfloat配列に代入する
		/*----------------------------------------------------------------------*/
		__forceinline static void SIMD_CALL_CONVENTION StoreFloat (float* destination, ConstVector128 source) noexcept;
		__forceinline static void SIMD_CALL_CONVENTION StoreFloat2(float* destination, ConstVector128 source) noexcept;
		__forceinline static void SIMD_CALL_CONVENTION StoreFloat3(float* destination, ConstVector128 source) noexcept;
		__forceinline static void SIMD_CALL_CONVENTION StoreFloat4(float* destination, ConstVector128 source) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : X(, Y, Z, W)等の要素に基づいて, 全ての要素に同じ値を持ったVector128クラスを作成する
		*           X,Y,Z,W  : 各要素
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
		*  @brief : ベクトルを並び替えます.indexを指定するとそのindexにあったVector4型を取得
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Swizzle(ConstVector128 vector,
			const gu::uint32 xIndex, const gu::uint32 yIndex,
			const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルから要素を抽出し, 新しいベクトルを作成する
		*           0～3 : aからのx, y, z, w
		*           4～7 : bからのx, y, z, w
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Permute(ConstVector128 a, ConstVector128,
			const gu::uint32 xIndex, const gu::uint32 yIndex,
			const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルから要素を抽出し, 新しいベクトルを作成する
		*           control vectorが0の場合はleftから, 非0の場合はrightからそれぞれ抽出が行われます
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Select(ConstVector128 left, ConstVector128 right, ConstVector128 control) noexcept;
		#pragma endregion Setter

		#pragma region Getter
		/*----------------------------------------------------------------------
		*  @brief : FPUレジスタに格納されたX(, Y, Z, W)等の要素を取り出す
		/*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION GetX(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION GetY(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION GetZ(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION GetW(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : floatの配列を使ってベクトルに格納する
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION LoadFloat (const float* source) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION LoadFloat2(const float* source) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION LoadFloat3(const float* source) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION LoadFloat4(const float* source) noexcept;

		#pragma endregion Getter

		#pragma region Operator
		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルの各要素同士の和を算出します. (left + right)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Add(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルの各要素同士の差を算出します. (left - right)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Subtract(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルの各要素同士の積を算出します. (left * right)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Multiply(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルの各要素同士の除算を算出します. (left / right)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Divide(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルの各要素同士のあまりを算出します. (left % right)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Mod(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief :  subtractLeft - (multiplyLeft * multiplyRight)を算出します.
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION NegativeMultiplySubtract(ConstVector128 multiplyLeft, ConstVector128 multiplyRight, ConstVector128 subtractLeft) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : multiplyLeft * multiplyRight + addを算出します.
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION MultiplyAdd(ConstVector128 multiplyLeft, ConstVector128 multiplyRight, ConstVector128 add) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ベクトルの各要素同士の逆数を算出します. (1.0f / vector)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Reciprocal(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : float値を使って各要素に掛けます scale * vector
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Scale(ConstVector128 vector, const float scale) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 二つのベクトルが等しいかどうかを調べます
		*           (Eachは各要素毎に等しいかどうかを調べます.)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION EqualVector2(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION EqualVector3(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION EqualVector4(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION EqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 二つのベクトルが等しくないかを調べます
		*           (Eachは各要素毎に等しいかどうかを調べます.)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualVector2(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualVector3(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualVector4(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION NotEqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 二つのベクトルがすべて等しいかどうかを調べます.
		*           比較時には符号なし整数(U32)として比較します.
		*           (Eachは各要素ごとに等しいかどうかを調べます)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION EqualAsIntVector2(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION EqualAsIntVector3(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION EqualAsIntVector4(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION EqualAsIntVectorEach(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 二つのベクトルが一つでも等しくないかどうかを調べます.
		*           比較時には符号なし整数(U32)として比較します.
		*           (Eachは各要素ごとに等しいかどうかを調べます)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualAsIntVector2(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualAsIntVector3(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualAsIntVector4(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION NotEqualAsIntVectorEach(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 一定範囲内で値が等しいかを調べる (left - epsilon <= right <= left + epsilon)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector2   (ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector3   (ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector4   (ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION NearEqualVectorEach(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 左の方が全ての要素において大きいかを調べます (left > right)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterVector2   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterVector3   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterVector4   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION GreaterVectorEach(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 左の方が全ての要素において大きいor等しいかを調べます (left >= right)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterOrEqualVector2   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterOrEqualVector3   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterOrEqualVector4   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION GreaterOrEqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 左の方が全ての要素において小さいかを調べます (left < right)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION LessVector2   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessVector3   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessVector4   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION LessVectorEach(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 左の方が全ての要素において小さいor等しいかを調べます (left <= right)
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION LessOrEqualVector2   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessOrEqualVector3   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessOrEqualVector4   (ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION LessOrEqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 指定範囲内にあるかどうか -bounds <= vector <= +bounds
		/*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION InBoundsVector2(ConstVector128 vector, ConstVector128 bounds) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION InBoundsVector3(ConstVector128 vector, ConstVector128 bounds) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION InBoundsVector4(ConstVector128 vector, ConstVector128 bounds) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION InBoundsVectorEach(ConstVector128 vector, ConstVector128 bounds) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 左方向にshiftCount分 (3まで)要素を回転する
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION RotateLeft(ConstVector128 vector, const gu::uint32 shiftCount) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 右方向にshiftCount分 (3まで)要素を回転する
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION RotateRight(ConstVector128 vector, const gu::uint32 shiftCount) noexcept;

		#pragma endregion Operator

		#pragma region Bit
		/*----------------------------------------------------------------------
		*  @brief : ビット単位の論理積の演算を行います
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION AndInt(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ビット単位の論理和の演算を行います
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION OrInt(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ビット単位の論理積否定の演算を行います
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION NorInt(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ビット単位の排他的論理和の演算を行います
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION XorInt(ConstVector128 left, ConstVector128 right) noexcept;

		#pragma endregion Bit

		#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : 各要素にマイナスを取ったものを返す.
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Negate(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
		/*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION LengthVector2(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION LengthVector3(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION LengthVector4(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2次ノルムの2乗を算出
		/*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION LengthSquaredVector2(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION LengthSquaredVector3(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION LengthSquaredVector4(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 内積を算出 (ただし, 返り値はfloatではなくVector128型で返されますので, 後々GetXなどで対応してください.)
		/*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION DotVector2(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION DotVector3(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION DotVector4(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 外積を算出 left, rightに垂直なベクトルを返す
		*           2次元の場合は平行四辺形の面積を返すという意味合い
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION CrossVector2(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION CrossVector3(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION CrossVector4(ConstVector128 first, ConstVector128 second, ConstVector128 third) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 入力ベクトルの単位ベクトルを返します
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION NormalizeVector2(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION NormalizeVector3(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION NormalizeVector4(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief     反射ベクトルを算出
		*  @param[in] ConstVector128 : input  (入射ベクトル)
		*  @param[in] ConstVector128 : normal (反射を行う法線ベクトル)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION ReflectVector2(ConstVector128 input, ConstVector128 normal) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION ReflectVector3(ConstVector128 input, ConstVector128 normal) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION ReflectVector4(ConstVector128 input, ConstVector128 normal) noexcept;

		/*----------------------------------------------------------------------
		*  @brief     屈折ベクトルを算出
		*  @param[in] ConstVector128 : input  (入射ベクトル)
		*  @param[in] ConstVector128 : normal (反射を行う法線ベクトル)
		*  @param[in] const float    : refractionIndex (屈折率)
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION RefractVector2(ConstVector128 input, ConstVector128 normal, const float refractionIndex) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION RefractVector3(ConstVector128 input, ConstVector128 normal, const float refractionIndex) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION RefractVector4(ConstVector128 input, ConstVector128 normal, const float refractionIndex) noexcept;

		__forceinline static Vector128 SIMD_CALL_CONVENTION RefractVector2V(ConstVector128 input, ConstVector128 normal, ConstVector128 refractionIndex) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION RefractVector3V(ConstVector128 input, ConstVector128 normal, ConstVector128 refractionIndex) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION RefractVector4V(ConstVector128 input, ConstVector128 normal, ConstVector128 refractionIndex) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 非常に大きい値か
		/*----------------------------------------------------------------------*/
		__forceinline static bool IsInfiniteVector2(ConstVector128 vector) noexcept;
		__forceinline static bool IsInfiniteVector3(ConstVector128 vector) noexcept;
		__forceinline static bool IsInfiniteVector4(ConstVector128 vector) noexcept;
		__forceinline static Vector128 IsInfiniteVectorEach(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素に根号を取ったものを返します
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Sqrt(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の逆数を取り, それに根号を取ったものを返します
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION ReciprocalSqrt(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のsin, cos, tanを返します [rad]
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Sin(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION Cos(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION Tan(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のarcsin, arccos, arctanを返します
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION ArcSin(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION ArcCos(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION ArcTan(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION ArcTan2(ConstVector128 y, ConstVector128 x) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のsinH, cosH, tanHを返します
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION SinH(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION CosH(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION TanH(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素に対するsinとcosを取り出します
		*           input      : radianを単位に持つ角度
		*           Vector128* : sin(radian)と示される4次元ベクトル
		*           Vector128* : cos(radian)と示される4次元ベクトル
		/*----------------------------------------------------------------------*/
		__forceinline static void SIMD_CALL_CONVENTION SinCos(ConstVector128 input, Vector128* sin, Vector128* cos) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の2^{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Exp2(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の10^{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Exp10(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のe^{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION ExpE(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のlog2{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Log2(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のlog10{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Log10(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のloge{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION LogE(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のPowを返します
		*           base^{power}
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Pow(ConstVector128 base, ConstVector128 power) noexcept;

		/*----------------------------------------------------------------------
		*  @brief      線形補間を返します
		*  @param[in]  t = 0の時の値
		   @param[in]  t = 1の時の値
		   @param[in]  t : 線形補間の割合
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Lerp(ConstVector128 start, ConstVector128 end, const float t) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION LerpV(ConstVector128 start, ConstVector128 end, ConstVector128 t) noexcept;


		/*----------------------------------------------------------------------
		*  @brief      エルミート補間を返します
		*  @param[in]  始点の位置
		   @param[in]  始点におけるスプラインの接線を表すベクトル
		   @param[in]  終点の位置
		   @param[in]  終点におけるスプラインの接線を表すベクトル
		   @param[in]  t : エルミート補間の割合
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Hermite(ConstVector128 startPosition, ConstVector128 startTangent, ConstVector128 endPosition, ConstVector128 endTangent, const float t) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION HermiteV(ConstVector128 startPosition, ConstVector128 startTangent, ConstVector128 endPosition, ConstVector128 endTangent, ConstVector128 t) noexcept;

		/*----------------------------------------------------------------------
		*  @brief      CatMull-Romスプライン補間の結果を返します。4つの制御点を全て通るスプライン補間
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION CatMullRom(ConstVector128 position0, ConstVector128 position1, ConstVector128 position2, ConstVector128 position3, const float t   ) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION CatMullRomV(ConstVector128 position0, ConstVector128 position1, ConstVector128 position2, ConstVector128 position3, ConstVector128 t) noexcept;

		/*----------------------------------------------------------------------
		*  @brief      三角形の重心重み位置を算出
		*              (f>=0 && g>=0 && 1-f-g>=0) の場合、ポイントは三角形の Position0>Position1>Position2 の内側にあります>。
                       (f==0 && g>=0 && 1-f-g>=0) の場合、ポイントは Position0>Position2 行目にあります>。
                       (f>=0 && g==0 && 1-f-g>=0) の場合、ポイントは Position0>Position1> 行目にあります。
                       (f>=0 && g>=0 && 1-f-g==0) の場合、ポイントは Position1>Position2 行目にあります>。
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION BaryCentric (ConstVector128 position0, ConstVector128 position1, ConstVector128 position2, const float f, const float g) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION BaryCentricV(ConstVector128 position0, ConstVector128 position1, ConstVector128 position2, ConstVector128 f, ConstVector128 g) noexcept;
		
		/*----------------------------------------------------------------------
		*  @brief : 全ての要素について最小値となる方を選択します
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Min(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素について最大値となる方を選択します
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Max(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 最も近い整数に丸める
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Round(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 切り捨て (指定した数値の端数を切り捨てた整数に丸める。数直線上で0の方向にある次の整数
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Truncate(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 負の方向に丸める
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Floor(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 切り上げ 数直線上で正の方向にある次の整数に丸める
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Ceiling(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 指定範囲内で数値を設定する
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Clamp(ConstVector128 vector, ConstVector128 min, ConstVector128 max) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 1以上には行かないようにする
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Saturate(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 絶対値
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Abs(ConstVector128 vector) noexcept;

		#pragma endregion Math

		#pragma region Angle
		/*----------------------------------------------------------------------
		*  @brief : - pi <= theta <= piの範囲に収めるようにします.
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION ModAngles(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : - pi <= theta <= piの範囲に収めるようにした上で足し算を行います.
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION AddAngles(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : - pi <= theta <= piの範囲に収めるようにした上で引き算を行います.
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
	// SIMD演算のバージョンごとに特別な対応が必要となるPermute演算
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
	*  @brief     ゼロ初期化したVector128を返す関数
	*
	*  @param[in] void
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Zero() noexcept
	{
		return _mm_setzero_ps(); // 4つの単精度浮動小数点値をクリアする
	}

	/****************************************************************************
	*                      TrueIntMask
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::TrueIntMask() noexcept
	*
	*  @brief     全てのビットがtrue (1)に設定されているVector128を返す
	*
	*  @param[in] void
	*
	*  @return 　　Vector128
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
	*  @brief     全てのビットがfalse (0)に設定されているVector128を返す
	*
	*  @param[in] void
	*
	*  @return 　　Vector128
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
	*  @brief     float4つからVector128クラスを生成する
	*
	*  @param[in] const float x
	*  @param[in] const float y
	*  @param[in] const float z
	*  @param[in] const float w
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Set(const float x, const float y, const float z, const float w) noexcept
	{
		// 4つの単精度浮動小数点値を設定する
		// 注意点として, set_psの引数は後ろから順にvector128の先頭に移っていく.(リトルエンディアン)
		return _mm_set_ps(w, z, y, x);
	}

	/****************************************************************************
	*                       Set
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Set(const float value) noexcept
	*
	*  @brief     float1つからVector128クラスを生成する
	*
	*  @param[in] const float value
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Set(const float value) noexcept
	{
		return _mm_set_ps1(value); // 同じ値で初期化
	}

	/****************************************************************************
	*                       SetInt
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Set(const float x, const float y, const float z, const float w) noexcept
	*
	*  @brief     int4つからVector128クラスを生成する
	*
	*  @param[in] const gu::uint32 x
	*  @param[in] const gu::uint32 y
	*  @param[in] const gu::uint32 z
	*  @param[in] const gu::uint32 w
	*
	*  @return 　　Vector128
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
	*  @brief     Floatを使ってVectorのXに値を代入する
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] const float x
	*
	*  @return 　　Vector128
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
	*  @brief     Floatを使ってVectorのYに値を代入する
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] const float y
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SetY(ConstVector128 vector, const float y) noexcept
	{
		// yとxを交換する
		Vector128 result = _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(3, 2, 0, 1));
		// 入力をベクトルに変換
		Vector128 temp = _mm_set_ss(y);
		// xの要素を置き換える
		result = _mm_move_ss(result, temp);
		// yとxを交換する
		result = _mm_shuffle_ps(result, result, _MM_SHUFFLE(3, 2, 0, 1));
		return result;
	}

	/****************************************************************************
	*                       SetZ
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION SetZ(ConstVector128 vector, const float z) noexcept
	*
	*  @brief     Floatを使ってVectorのZに値を代入する
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] const float z
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SetZ(ConstVector128 vector, const float z) noexcept
	{
		// zとxを交換する
		Vector128 result = _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(3, 0, 1, 2));
		// 入力をベクトルに変換
		Vector128 temp = _mm_set_ss(z);
		// xの要素を置き換える
		result = _mm_move_ss(result, temp);
		// zとxを交換する
		result = _mm_shuffle_ps(result, result, _MM_SHUFFLE(3, 0, 1, 2));
		return result;
	}

	/****************************************************************************
	*                       SetW
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION SetW(ConstVector128 vector, const float w) noexcept
	*
	*  @brief     Floatを使ってVectorのWに値を代入する
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] const float w
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SetW(ConstVector128 vector, const float w) noexcept
	{
		// wとxを交換する
		Vector128 result = _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(0, 2, 1, 3));
		// 入力をベクトルに変換
		Vector128 temp = _mm_set_ss(w);
		// xの要素を置き換える
		result = _mm_move_ss(result, temp);
		// wとxを交換する
		result = _mm_shuffle_ps(result, result, _MM_SHUFFLE(0, 2, 1, 3));
		return result;
	}

	/****************************************************************************
	*                       StoreFloat
	****************************************************************************/
	/* @fn        inline void SIMD_CALL_CONVENTION Vector128Utility::StoreFloat(float* destination, ConstVector128 source) noexcept
	*
	*  @brief     Vector128をもとにFloat配列に代入します
	*
	*  @param[in] float* destination
	*  @param[in] ConstVector128 source
	*
	*  @return 　　Vector128
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
	*  @brief     Vector128をもとにFloat配列に代入します
	*
	*  @param[in] float* destination
	*  @param[in] ConstVector128 source
	*
	*  @return 　　Vector128
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
	*  @brief     Vector128をもとにFloat配列に代入します
	*
	*  @param[in] float* destination
	*  @param[in] ConstVector128 source
	*
	*  @return 　　Vector128
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
	*  @brief     Vector128をもとにFloat配列に代入します
	*
	*  @param[in] float* destination
	*  @param[in] ConstVector128 source
	*
	*  @return 　　Vector128
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
	*  @brief     Xの要素に基づいて, 全ての要素に同じ値を持ったVector128クラスを作成する
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
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
	*  @brief     Yの要素に基づいて, 全ての要素に同じ値を持ったVector128クラスを作成する
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
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
	*  @brief     Zの要素に基づいて, 全ての要素に同じ値を持ったVector128クラスを作成する
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
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
	*  @brief     Wの要素に基づいて, 全ての要素に同じ値を持ったVector128クラスを作成する
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
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
	*  @brief     全ての要素に1を代入したVector128クラスを作成する
	*
	*  @param[in] void
	*
	*  @return 　　Vector128
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
	*  @brief     Wの要素に基づいて, 全ての要素にINFを持ったVector128クラスを作成する
	*
	*  @param[in] void
	*
	*  @return 　　Vector128
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
	*  @brief     Wの要素に基づいて, 全ての要素に1.192092896e-7fを持ったVector128クラスを作成する
	*
	*  @param[in] void
	*
	*  @return 　　Vector128
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
	*  @brief     Wの要素に基づいて, 全ての要素に-0.0f (0x80000000)を持ったVector128クラスを作成する
	*
	*  @param[in] void
	*
	*  @return 　　Vector128
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
	*  @brief     ベクトルを並び替えます.indexを指定するとそのindexにあったVector4型を取得
	*
	*  @return 　　Vector128
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
	*  @brief     2つのベクトルから要素を抽出し, 新しいベクトルを作成する
	*             0～3 : aからのx, y, z, w
	*             4～7 : bからのx, y, z, w
	*
	*  @return 　　Vector128
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
	*  @brief     2つのベクトルから要素を抽出し, 新しいベクトルを作成する
	*             control vectorが0の場合はleftから, 非0の場合はrightからそれぞれ抽出が行われます
	*
	*  @return 　　Vector128
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
	*  @brief     FPUレジスタに格納されたXの要素を取り出す
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　float
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
	*  @brief     FPUレジスタに格納されたYの要素を取り出す
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　float
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
	*  @brief     FPUレジスタに格納されたZの要素を取り出す
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　float
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
	*  @brief     FPUレジスタに格納されたWの要素を取り出す
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　float
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
	*  @brief     Floatの配列を使って格納する (*source, 0.0, 0.0, 0.0f)
	*
	*  @param[in] const float* source
	*
	*  @return 　　Vector128
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
	*  @brief     Floatの配列を使って格納する (source->x, source->y, 0.0, 0.0f)
	*
	*  @param[in] const float* source
	*
	*  @return 　　Vector128
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
	*  @brief     Floatの配列を使って格納する (source->x, source->y, source->z, 0.0f)
	*
	*  @param[in] const float* source
	*
	*  @return 　　Vector128
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
	*  @brief     Floatの配列を使って格納する (source->x, source->y, source->z, source->w)
	*
	*  @param[in] const float* source
	*
	*  @return 　　Vector128
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
	*  @brief     2つのベクトルの各要素同士の和を算出します. (left + right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　Vector128
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
	*  @brief     2つのベクトルの各要素同士の差を算出します. (left - right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　Vector128
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
	*  @brief     2つのベクトルの各要素同士の差を算出します. (left * right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　Vector128
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
	*  @brief     2つのベクトルの各要素同士の差を算出します. (left / right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　Vector128
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
	*  @brief     2つのベクトルの各要素同士のあまりを算出します. (left % right)
	*             left % right = left - right * truncate(left / right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　Vector128
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
	*  @brief     subtractLeft - (multiplyLeft * multiplyRight)を算出します.
	*
	*  @param[in] ConstVector128 multiplyLeft
	*  @param[in] ConstVector128 multiplyRight
	*  @param[in] ConstVector128 subtractLeft
	*
	*  @return 　　Vector128
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
	*  @brief     multiplyLeft * multiplyRight + addを算出します.
	*
	*  @param[in] ConstVector128 multiplyLeft
	*  @param[in] ConstVector128 multiplyRight
	*  @param[in] ConstVector128 add
	*
	*  @return 　　Vector128
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
	*  @brief     2つのベクトルの各要素同士の逆数を算出します. (1.0f / vector)
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
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
	*  @brief     float値を使って各要素に掛けます scale * vector
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] const float scale
	*
	*  @return 　　Vector128
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
	*  @brief     二つの2次元ベクトルが等しいかどうかを調べます
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualVector2(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, xそれぞれに対して等しい場合は1を返すベクトル
		Vector128 equalMaskVector = _mm_cmpeq_ps(left, right); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		// w, zは無視するようにする
		return (_mm_movemask_ps(equalMaskVector) & 0b0011) == 0b0011; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       EqualVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualVector3(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     二つの3次元ベクトルが等しいかどうかを調べます
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualVector3(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, xそれぞれに対して等しい場合は1を返すベクトル
		Vector128 equalMaskVector = _mm_cmpeq_ps(left, right); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		// wは無視するようにする
		return (_mm_movemask_ps(equalMaskVector) & 0b0111) == 0b0111; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       EqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualVector4(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     二つの4次元ベクトルが等しいかどうかを調べます
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualVector4(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, xそれぞれに対して等しい場合は1を返すベクトル
		Vector128 notEqualMaskVector = _mm_cmpeq_ps(left, right); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		return (_mm_movemask_ps(notEqualMaskVector) & 0b1111) == 0b1111; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       EqualVectorEach
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     二つの4次元ベクトルが各要素ごとに等しいかどうかを調べます
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
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
	*  @brief     二つの2次元ベクトルが等しくないかどうかを調べます
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector2(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, xそれぞれに対して等しい場合は1を返すベクトル
		Vector128 equalMaskVector = _mm_cmpeq_ps(left, right); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		// 一個でもフラグが立っていなかったら等しくない判定
		// ただしw, zは無視
		return (_mm_movemask_ps(equalMaskVector) & 0b0011) != 0b0011;
	}

	/****************************************************************************
	*                       NotEqualVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector4(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     二つの3次元ベクトルが等しくないかどうかを調べます
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector3(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, xそれぞれに対して等しい場合は1を返すベクトル
		Vector128 equalMaskVector = _mm_cmpeq_ps(left, right); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		// 一個でもフラグが立っていなかったら等しくない判定
		// ただしwは無視
		return (_mm_movemask_ps(equalMaskVector) & 0b0111) != 0b0111;
	}

	/****************************************************************************
	*                       NotEqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector4(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     二つの4次元ベクトルが等しくないかどうかを調べます
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector4(ConstVector128 left, ConstVector128 right) noexcept
	{
		Vector128 equalMaskVector = _mm_cmpneq_ps(left, right); // 全不正解なら{1, 1, 1, 1}, wだけ正しいなら{0, 1, 1, 1}

		return (_mm_movemask_ps(equalMaskVector)) != 0;
	}

	/****************************************************************************
	*                       NotEqualVectorEach
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     二つの4次元ベクトルが各要素ごとに等しくないかを調べます
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
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
	*  @brief     二つのベクトルが等しいかどうかを調べます.
	*             比較時には符号なし整数(U32)として扱います.
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
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
	*  @brief     二つのベクトルが等しいかどうかを調べます.
	*             比較時には符号なし整数として扱います.
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
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
	*  @brief     二つのベクトルが等しいかどうかを調べます.
	*             比較時には符号なし整数として扱います.
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
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
	*  @brief     二つのベクトルが等しいかどうかをそれぞれの要素で調べます.
	*             比較時には符号なし整数として扱います.
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
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
	*  @brief     二つのベクトルが一つでも等しくないかどうかを調べます.
	*            比較時には符号なし整数(U32)として比較します.
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
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
	*  @brief     二つの3次元ベクトルが等しくないかどうかを調べます
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
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
	*  @brief     二つの4次元ベクトルが等しくないかどうかを調べます
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
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
	*  @brief     二つの4次元ベクトルが各要素ごとに等しくないかを調べます
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
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
	*  @brief     一定範囲内で値が等しいかを調べる (left - epsilon <= right <= left + epsilon)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*  @param[in] ConstVector128 epsilon
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::NearEqualVector2(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept
	{
		// 差分の導出
		Vector128 delta = _mm_sub_ps(left, right);

		// 絶対値の差分の取得
		Vector128 result = _mm_setzero_ps();
		result = _mm_sub_ps(result, delta);
		result = _mm_max_ps(result, delta);
		result = _mm_cmple_ps(result, epsilon);

		// zとwは無視して, xとyに着目する
		return ((_mm_movemask_ps(result) & 0b0011) == 0b0011);
	}

	/****************************************************************************
	*                       NearEqualVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector3(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept
	*
	*  @brief     一定範囲内で値が等しいかを調べる (left - epsilon <= right <= left + epsilon)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*  @param[in] ConstVector128 epsilon
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::NearEqualVector3(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept
	{
		// 差分の導出
		Vector128 delta = _mm_sub_ps(left, right);

		// 絶対値の差分の取得
		Vector128 result = _mm_setzero_ps();
		result = _mm_sub_ps(result, delta);
		result = _mm_max_ps(result, delta);
		result = _mm_cmple_ps(result, epsilon);

		// zとwは無視して, xとyに着目する
		return ((_mm_movemask_ps(result) & 0b0111) == 0b0111);
	}

	/****************************************************************************
	*                       NearEqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector4(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept
	*
	*  @brief     一定範囲内で値が等しいかを調べる (left - epsilon <= right <= left + epsilon)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*  @param[in] ConstVector128 epsilon
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::NearEqualVector4(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept
	{
		// 差分の導出
		Vector128 delta = _mm_sub_ps(left, right);

		// 絶対値の差分の取得
		Vector128 result = _mm_setzero_ps();
		result = _mm_sub_ps(result, delta);
		result = _mm_max_ps(result, delta);
		result = _mm_cmple_ps(result, epsilon);

		// zとwは無視して, xとyに着目する
		return ((_mm_movemask_ps(result) & 0b1111) == 0b1111);
	}

	/****************************************************************************
	*                       NearEqualVectorEach
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVectorEach(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept
	*
	*  @brief     一定範囲内で値が等しいかを調べる (left - epsilon <= right <= left + epsilon)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*  @param[in] ConstVector128 epsilon
	*
	*  @return 　　bool
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NearEqualVectorEach(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept
	{
		// 差分
		Vector128 delta = _mm_sub_ps(left, right);

		// 絶対値の取得
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
	*  @brief     Vector2型において左の方が全ての要素において大きいかを調べます (left > right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterVector2(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft > rightの場合は1を返すベクトル
		Vector128 equalMaskVector = _mm_cmpgt_ps(left, right); // 全て大きいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		// w, zは無視する
		return (_mm_movemask_ps(equalMaskVector) & 0b0011) == 0b0011;
	}

	/****************************************************************************
	*                       GreaterVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterVector3(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector3型において左の方が全ての要素において大きいかを調べます (left > right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterVector3(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft > rightの場合は1を返すベクトル
		Vector128 equalMaskVector = _mm_cmpgt_ps(left, right); // 全て大きいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		// wは無視する
		return (_mm_movemask_ps(equalMaskVector) & 0b0111) == 0b0111;
	}

	/****************************************************************************
	*                       GreaterVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterVector4(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector4型において左の方が全ての要素において大きいかを調べます (left > right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterVector4(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft > rightの場合は1を返すベクトル
		Vector128 equalMaskVector = _mm_cmpgt_ps(left, right); // 全て大きいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		return _mm_movemask_ps(equalMaskVector) == 0b1111;
	}

	/****************************************************************************
	*                       GreaterVectorEach
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterVector4(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector4型において左の方が全ての要素において大きいかを調べます (left > right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　Vector128
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
	*  @brief     Vector2型において左の方が全ての要素において大きいor等しいかを調べます (left >= right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterOrEqualVector2(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft > rightの場合は1を返すベクトル
		Vector128 equalMaskVector = _mm_cmpge_ps(left, right); // 全て大きいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		return (_mm_movemask_ps(equalMaskVector) & 0b0011) == 0b0011;
	}

	/****************************************************************************
	*                       GreaterVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterOrEqualVector3(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector3型において左の方が全ての要素において大きいor等しいかを調べます (left >= right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterOrEqualVector3(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft > rightの場合は1を返すベクトル
		Vector128 equalMaskVector = _mm_cmpge_ps(left, right); // 全て大きいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		return (_mm_movemask_ps(equalMaskVector) & 0b0111) == 0b0111;
	}

	/****************************************************************************
	*                       GreaterOrEqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterOrEqualVector4(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector4型において左の方が全ての要素において大きいor等しいかを調べます (left >= right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterOrEqualVector4(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft >= rightの場合は1を返すベクトル
		Vector128 equalMaskVector = _mm_cmpge_ps(left, right); // 全て大きいor等しいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		return _mm_movemask_ps(equalMaskVector) == 0b1111;
	}

	/****************************************************************************
	*                       GreaterOrEqualVectorEach
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::GreaterOrEqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector4型において左の方が全ての要素において大きいor等しいかを調べます (left >= right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　Vector128
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
	*  @brief     Vector2型において左の方が全ての要素において小さいかを調べます (left < right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::LessVector2(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft < rightの場合は1を返すベクトル
		Vector128 equalMaskVector = _mm_cmplt_ps(left, right); // 全て小さいなら{1, 1, 1, 1}, wだけ大きいなら{0, 1, 1, 1}

		return (_mm_movemask_ps(equalMaskVector) & 0b0011) == 0b0011;
	}

	/****************************************************************************
	*                       LessVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::LessVector3(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector3型において左の方が全ての要素において小さいかを調べます (left < right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::LessVector3(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft > rightの場合は1を返すベクトル
		Vector128 equalMaskVector = _mm_cmplt_ps(left, right); // 全て大きいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		// wは無視する
		return (_mm_movemask_ps(equalMaskVector) & 0b0111) == 0b0111;
	}

	/****************************************************************************
	*                       LessVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::LessVector4(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector4型において左の方が全ての要素において小さいかを調べます (left < right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::LessVector4(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft > rightの場合は1を返すベクトル
		Vector128 equalMaskVector = _mm_cmplt_ps(left, right); // 全て大きいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		return _mm_movemask_ps(equalMaskVector) == 0b1111;
	}

	/****************************************************************************
	*                       LessVectorEach
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LessVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector4型において左の方が全ての要素において小さいかを調べます (left < right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
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
	*  @brief     Vector2型において左の方が全ての要素において小さいor等しいかを調べます (left <= right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::LessOrEqualVector2(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft > rightの場合は1を返すベクトル
		Vector128 equalMaskVector = _mm_cmple_ps(left, right); // 全て大きいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		return (_mm_movemask_ps(equalMaskVector) & 0b0011) == 0b0011;
	}

	/****************************************************************************
	*                       LessOrEqualVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::LessOrEqualVector3(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector3型において左の方が全ての要素において小さいor等しいかを調べます (left <= right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::LessOrEqualVector3(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft > rightの場合は1を返すベクトル
		Vector128 equalMaskVector = _mm_cmple_ps(left, right); // 全て大きいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		return (_mm_movemask_ps(equalMaskVector) & 0b0111) == 0b0111;
	}

	/****************************************************************************
	*                       LessOrEqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::LessOrEqualVector4(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector4型において左の方が全ての要素において小さいor等しいかを調べます (left <= right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector128Utility::LessOrEqualVector4(ConstVector128 left, ConstVector128 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft >= rightの場合は1を返すベクトル
		Vector128 equalMaskVector = _mm_cmple_ps(left, right); // 全て大きいor等しいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		return _mm_movemask_ps(equalMaskVector) == 0b1111;
	}

	/****************************************************************************
	*                       LessVectorEach
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LessOrEqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector4型において左の方が全ての要素において小さいor等しいかを調べます (left <= right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
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
	*  @brief     全ての要素を調べて指定範囲内にあるかどうか -bounds <= vector <= +bounds
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] ConstVector128 bounds 
	*
	*  @return 　　bool
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
	*  @brief     全ての要素を調べて指定範囲内にあるかどうか -bounds <= vector <= +bounds
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] ConstVector128 bounds
	*
	*  @return 　　bool
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
	*  @brief     全ての要素を調べて指定範囲内にあるかどうか -bounds <= vector <= +bounds
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] ConstVector128 bounds
	*
	*  @return 　　bool
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
	*  @brief     指定範囲内にあるかどうかを各要素で比較します -bounds <= vector <= +bounds
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] ConstVector128 bounds
	*
	*  @return 　　bool
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
	*  @brief     左方向にshiftCount分 (3まで)要素を回転する
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] const gu::uint32 shiftCount
	*
	*  @return 　　Vector128
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
	*  @brief     右方向にshiftCount分 (3まで)要素を回転する
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] const gu::uint32 shiftCount
	*
	*  @return 　　Vector128
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
	*  @brief     ビット単位の論理積の演算を行います
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　Vector128
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
	*  @brief     ビット単位の論理積の演算を行います
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　Vector128
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
	*  @brief     ビット単位の論理積否定の演算を行います
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　Vector128
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
	*  @brief     ビット単位の排他的論理和の演算を行います
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　Vector128
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
	*  @brief     Vectorの全ての要素の符号を判定します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
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
	*  @brief     2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::LengthVector2(ConstVector128 vector) noexcept
	{
		// Dot積の計算
		Vector128 squareLength = _mm_mul_ps(vector, vector);

		// yのみのベクトルを取得
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
	*  @brief     2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::LengthVector3(ConstVector128 vector) noexcept
	{
		// Dot積の計算
		Vector128 squareLength = _mm_mul_ps(vector, vector);

		// zとyを要素にもつベクトル作成
		Vector128 temp = PERMUTE_PS(squareLength, _MM_SHUFFLE(1, 2, 1, 2));

		// x + z, y
		squareLength = _mm_add_ss(squareLength, temp);

		// y, y, y, y
		temp = PERMUTE_PS(temp, _MM_SHUFFLE(1, 1, 1, 1));

		// x + z + y, ?? , ??, ??
		squareLength = _mm_add_ss(squareLength, temp);

		// splat the length squared
		squareLength = PERMUTE_PS(squareLength, _MM_SHUFFLE(0, 0, 0, 0));

		// ノルムの取得
		squareLength = _mm_sqrt_ps(squareLength);

		return _mm_cvtss_f32(squareLength);
	}

	/****************************************************************************
	*                      NLengthVector4
	****************************************************************************/
	/* @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::LengthVector4(ConstVector128 vector) noexcept
	*
	*  @brief     2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::LengthVector4(ConstVector128 vector) noexcept
	{
		// Dot積の計算
		Vector128 squareLength = _mm_mul_ps(vector, vector);

		// z, wのベクトルを取得
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
	*  @brief     2次ノルムの2乗を算出
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　float
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
	*  @brief     2次ノルムの2乗を算出
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　float
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
	*  @brief     2次ノルムの2乗を算出
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　float
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
	*  @brief     内積を算出
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector2(ConstVector128 left, ConstVector128 right) noexcept
	{
		// 要素ごとの積を計算 
		Vector128 multiply = _mm_mul_ps(left, right);

		// z以降の計算は考慮しないため, yの結果を格納
		Vector128 y = PERMUTE_PS(multiply, _MM_SHUFFLE(1, 1, 1, 1));

		// x成分のみの結果に着目する.  
		return _mm_cvtss_f32(_mm_add_ss(multiply, y));
	}

	/****************************************************************************
	*                      DotVector3
	****************************************************************************/
	/* @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector3(ConstVector128 left, ConstVector128 right)
	*
	*  @brief     内積を算出
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector3(ConstVector128 left, ConstVector128 right) noexcept
	{
		// 要素ごとの積を計算 
		Vector128 multiply = _mm_mul_ps(left, right);

		// x=Dot.F32[1], y=Dot.F32[2]
		Vector128 temp = PERMUTE_PS(multiply, _MM_SHUFFLE(2, 1, 2, 1));

		// result.F32[0] = x + y
		multiply = _mm_add_ss(multiply, temp);

		// x=multiply.F32[2]
		temp = PERMUTE_PS(temp, _MM_SHUFFLE(1, 1, 1, 1));

		// Result.F32[0] = (x+y)+z
		temp = _mm_add_ss(multiply, temp);

		// x成分のみの結果に着目する.  
		return _mm_cvtss_f32(PERMUTE_PS(multiply, _MM_SHUFFLE(0, 0, 0, 0)));
	}

	/****************************************************************************
	*                      DotVector4
	****************************************************************************/
	/* @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector4(ConstVector128 left, ConstVector128 right)
	*
	*  @brief     内積を算出
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector4(ConstVector128 left, ConstVector128 right) noexcept
	{
		// 要素ごとの積を計算 
		Vector128 temp2 = right;
		Vector128 temp = _mm_mul_ps(left, temp2);

		// 各要素の加算によって, XをZの位置に、YをWの位置にコピーする
		temp2 = _mm_shuffle_ps(temp2, temp, _MM_SHUFFLE(1, 0, 0, 0));

		// Z = X+Z; W = Y+W;
		temp2 = _mm_add_ps(temp2, temp);

		// W をZの位置にコピー
		temp = _mm_shuffle_ps(temp, temp2, _MM_SHUFFLE(0, 3, 0, 0));

		// ZとWを両方加算
		temp = _mm_add_ps(temp, temp2);

		// x成分のみの結果に着目する.  
		return _mm_cvtss_f32(PERMUTE_PS(temp, _MM_SHUFFLE(2, 2, 2, 2)));
	}

	/****************************************************************************
	*                      CrossVector2
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CrossVector2(ConstVector128 left, ConstVector128 right)
	*
	*  @brief     平行四辺形の面積を算出する
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　Vector128ではあるが, 実質float
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CrossVector2(ConstVector128 left, ConstVector128 right) noexcept
	{
		// [ left.x*right.y - left.y*right.x, left.x*right.y - left.y*right.x ]
		
		// rightのx, yを入れ替える (w,zは計算上使用しない)
		Vector128 result = PERMUTE_PS(right, _MM_SHUFFLE(0, 1, 0, 1));

		// leftと入れ替え後のrightの各要素を乗算する
		result = _mm_mul_ps(result, left);

		// yを抽出する
		Vector128 y = PERMUTE_PS(result, _MM_SHUFFLE(1, 1, 1, 1));

		// 値を減算する
		result = _mm_sub_ss(result, y);

		// x成分のみを取り出して結果とする
		return PERMUTE_PS(result, _MM_SHUFFLE(0, 0, 0, 0));
	}

	/****************************************************************************
	*                      CrossVector3
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CrossVector3(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     外積を算出 left, rightに垂直なベクトルを返す
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CrossVector3(ConstVector128 left, ConstVector128 right) noexcept
	{
		// [ V1.y*V2.z - V1.z*V2.y, V1.z*V2.x - V1.x*V2.z, V1.x*V2.y - V1.y*V2.x ]
		
		// leftの要素を(y1, z1, x1, w1)に順に並べ替える
		Vector128 temp1 = PERMUTE_PS(left, _MM_SHUFFLE(3, 0, 2, 1)); 

		// rightの要素を(z2, x2. y2, w2)の順に並べ替える
		Vector128 temp2 = PERMUTE_PS(right, _MM_SHUFFLE(3, 1, 0, 2));

		// 一時ベクトルの要素ごとの乗算
		Vector128 result = _mm_mul_ps(temp1, temp2);

		// z1, x1, y1, w1の順にtemp1を並べ替える
		temp1 = PERMUTE_PS(temp1, _MM_SHUFFLE(3, 0, 2, 1));
		
		// y2, z2, x2, w2の順にtemp2を並べ替える
		temp2 = PERMUTE_PS(temp2, _MM_SHUFFLE(3, 1, 0, 2));

		result = NegativeMultiplySubtract(temp1, temp2, result);

		// wの要素をゼロに設定
		return _mm_and_ps(result, VECTOR_128F_MASK_XYZ);
	}

	/****************************************************************************
	*                      CrossVector4
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CrossVector4(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     外積を算出 left, rightに垂直なベクトルを返す
	*
	*  @param[in] ConstVector128 first
	*  @param[in] ConstVector128 second
	*  @param[in] ConstVector128 third
	*
	*  @return 　　Vector128
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
	*  @brief     入力ベクトルの単位ベクトルを返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector2(ConstVector128 vector) noexcept
	{
		// 1次norm計算
		Vector128 squareLength = _mm_mul_ps(vector, vector);
		Vector128 temp = PERMUTE_PS(squareLength, _MM_SHUFFLE(1, 1, 1, 1));
		squareLength   = _mm_add_ps(squareLength, temp);
		squareLength   = PERMUTE_PS(squareLength, _MM_SHUFFLE(0, 0, 0, 0));
		Vector128 norm = _mm_sqrt_ps(squareLength);

		// 0で割られる可能性があるかどうかを調べる. 値を持っていたら1
		Vector128 zeroMask = _mm_setzero_ps();
		zeroMask = _mm_cmpneq_ps(zeroMask, norm);

		// 無限大でない場合は対応する要素が1になるマスクの作成
		Vector128 infinityMask = _mm_cmpneq_ps(squareLength, VECTOR_128F_INFINITY);

		// 正規化の実行
		Vector128 result = _mm_div_ps(vector, norm);

		// 0付近で割られて無限大になったものに対しては0に設定
		result = _mm_and_ps(result, zeroMask);

		// 無限大かに基づいてqnanまたはresultを選択する
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
	*  @brief     入力ベクトルの単位ベクトルを返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector3(ConstVector128 vector) noexcept
	{
		// x, y, zに対するドット積の計算
		// x*x, y*y, z*z, w*w
		Vector128 squareLength = _mm_mul_ps(vector, vector);

		// z*z, y*y, z*z, y*y
		Vector128 temp         = PERMUTE_PS(squareLength, _MM_SHUFFLE(2, 1, 2, 1)); 

		// x*x + z*z, y*y, z*z, w*w
		squareLength           = _mm_add_ss(squareLength, temp);

		// 全てy*y
		temp = PERMUTE_PS(temp, _MM_SHUFFLE(1, 1, 1, 1));

		// 1要素目がx*x + y*y + z*z
		squareLength = _mm_add_ss(squareLength, temp);
		// 全ての要素がドット積
		squareLength = PERMUTE_PS(squareLength, _MM_SHUFFLE(0, 0, 0, 0));

		// 1次ノルムの計算
		Vector128 norm = _mm_sqrt_ps(squareLength);

		// 0で割られる可能性があるかどうかを調べる. 値を持っていたら1
		Vector128 zeroMask = _mm_setzero_ps();
		zeroMask = _mm_cmpneq_ps(zeroMask, norm);

		// 無限大でない場合は対応する要素が1になるマスクの作成
		Vector128 infinityMask = _mm_cmpneq_ps(squareLength, VECTOR_128F_INFINITY);

		// 正規化の実行
		Vector128 result = _mm_div_ps(vector, norm);

		// 0付近で割られて無限大になったものに対しては0に設定
		result = _mm_and_ps(result, zeroMask);

		// 無限大かに基づいてqnanまたはresultを選択する
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
	*  @brief     入力ベクトルの単位ベクトルを返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector4(ConstVector128 vector) noexcept
	{
		// x, y, z, wに対するドット積の計算
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

		// 全ての要素が4要素の二乗和
		squareLength = PERMUTE_PS(squareLength, _MM_SHUFFLE(2, 2, 2, 2));

		// 1次ノルムの計算
		Vector128 norm = _mm_sqrt_ps(squareLength);

		// 0で割られる可能性があるかどうかを調べる. 値を持っていたら1
		Vector128 zeroMask = _mm_setzero_ps();
		zeroMask = _mm_cmpneq_ps(zeroMask, norm);

		// 無限大でない場合は対応する要素が1になるマスクの作成
		Vector128 infinityMask = _mm_cmpneq_ps(squareLength, VECTOR_128F_INFINITY);

		// 正規化の実行
		Vector128 result = _mm_div_ps(vector, norm);

		// 0付近で割られて無限大になったものに対しては0に設定
		result = _mm_and_ps(result, zeroMask);

		// 無限大かに基づいてqnanまたはresultを選択する
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
	*  @brief     反射ベクトルを算出 (input - (2 * dot(input, normal)) * normal)
	*
	*  @param[in] ConstVector128 : input  (入射ベクトル)
	*  @param[in] ConstVector128 : normal (反射を行う法線ベクトル)
	*
	*  @return 　　Vector128
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
	*  @brief     反射ベクトルを算出 (input - (2 * dot(input, normal)) * normal)
	*
	*  @param[in] ConstVector128 : input  (入射ベクトル)
	*  @param[in] ConstVector128 : normal (反射を行う法線ベクトル)
	*
	*  @return 　　Vector128
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
	*  @brief     反射ベクトルを算出 (input - (2 * dot(input, normal)) * normal)
	*
	*  @param[in] ConstVector128 : input  (入射ベクトル)
	*  @param[in] ConstVector128 : normal (反射を行う法線ベクトル)
	*
	*  @return 　　Vector128
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
	*  @brief     屈折ベクトルを算出
	*
	*  @param[in] ConstVector128 : input  (入射ベクトル)
	*  @param[in] ConstVector128 : normal (屈折を行う法線ベクトル)
	*  @param[in]  const float   : refractionIndex (屈折率)
	*
	*  @return 　　Vector128
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
	*  @brief     屈折ベクトルを算出
	*
	*  @param[in] ConstVector128 : input  (入射ベクトル)
	*  @param[in] ConstVector128 : normal (屈折を行う法線ベクトル)
	*  @param[in]  const float   : refractionIndex (屈折率)
	*
	*  @return 　　Vector128
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
	*  @brief     屈折ベクトルを算出
	*
	*  @param[in] ConstVector128 : input  (入射ベクトル)
	*  @param[in] ConstVector128 : normal (屈折を行う法線ベクトル)
	*  @param[in]  const float   : refractionIndex (屈折率)
	*
	*  @return 　　Vector128
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
	*  @brief     屈折ベクトルを算出 (異方性がある場合)
	*
	*  @param[in] ConstVector128 : input  (入射ベクトル)
	*  @param[in] ConstVector128 : normal (屈折を行う法線ベクトル)
	*  @param[in]  const float   : refractionIndex (屈折率)
	*
	*  @return 　　Vector128
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

		// 結果が0以下であれば, sqrtは失敗ということでzeroに置き換える
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
	*  @brief     屈折ベクトルを算出 (異方性がある場合)
	*
	*  @param[in] ConstVector128 : input  (入射ベクトル)
	*  @param[in] ConstVector128 : normal (屈折を行う法線ベクトル)
	*  @param[in]  const float   : refractionIndex (屈折率)
	*
	*  @return 　　Vector128
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
	*  @brief     屈折ベクトルを算出 (異方性がある場合)
	*
	*  @param[in] ConstVector128 : input  (入射ベクトル)
	*  @param[in] ConstVector128 : normal (屈折を行う法線ベクトル)
	*  @param[in]  const float   : refractionIndex (屈折率)
	*
	*  @return 　　Vector128
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
	*  @brief     非常に大きい値か
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline bool Vector128Utility::IsInfiniteVector2(ConstVector128 vector) noexcept
	{
		// 符号ビットをマスクオフする
		Vector128 temp = _mm_and_ps(vector, VECTOR_128U_MASK_ABS);
		// 無限大と比較
		temp = _mm_cmpeq_ps(temp, VECTOR_128F_INFINITY);
		// どれかが無限大であれば、符号は真である。
		return (_mm_movemask_ps(temp) & 0b0011) != 0;
	}

	/****************************************************************************
	*                      IsInfiniteVector3
	****************************************************************************/
	/* @fn        inline bool Vector128Utility::IsInfiniteVector3(ConstVector128 vector) noexcept
	*
	*  @brief     非常に大きい値か
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline bool Vector128Utility::IsInfiniteVector3(ConstVector128 vector) noexcept
	{
		// 符号ビットをマスクオフする
		Vector128 temp = _mm_and_ps(vector, VECTOR_128U_MASK_ABS);
		// 無限大と比較
		temp = _mm_cmpeq_ps(temp, VECTOR_128F_INFINITY);
		// どれかが無限大であれば、符号は真である。
		return (_mm_movemask_ps(temp) & 0b0111) != 0;
	}

	/****************************************************************************
	*                      IsInfiniteVector4
	****************************************************************************/
	/* @fn        inline bool Vector128Utility::IsInfiniteVector4(ConstVector128 vector) noexcept
	*
	*  @brief     非常に大きい値か
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline bool Vector128Utility::IsInfiniteVector4(ConstVector128 vector) noexcept
	{
		// 符号ビットをマスクオフする
		Vector128 temp = _mm_and_ps(vector, VECTOR_128U_MASK_ABS);
		// 無限大と比較
		temp = _mm_cmpeq_ps(temp, VECTOR_128F_INFINITY);
		// どれかが無限大であれば、符号は真である。
		return (_mm_movemask_ps(temp) != 0);
	}

	/****************************************************************************
	*                      IsInfiniteVectorEach
	****************************************************************************/
	/* @fn        inline Vector128 Vector128Utility::IsInfiniteVectorEach(ConstVector128 vector) noexcept
	*
	*  @brief     非常に大きい値か
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 Vector128Utility::IsInfiniteVectorEach(ConstVector128 vector) noexcept
	{
		// 符号ビットをマスクオフする
		Vector128 temp = _mm_and_ps(vector, VECTOR_128U_MASK_ABS);
		// 無限大と比較
		temp = _mm_cmpeq_ps(temp, VECTOR_128F_INFINITY);
		// どれかが無限大であれば、符号は真である。
		return temp;
	}


	/****************************************************************************
	*                      Sqrt
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Sqrt(ConstVector128 vector) noexcept
	*
	*  @brief      全ての要素に根号を取ったものを返します
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
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
	*  @brief       全ての要素の逆数を取り, それに根号を取ったものを返します
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
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
	*  @brief     全ての要素の2^{vector}を返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
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
	*  @brief      全ての要素の10^{vector}を返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
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
	*  @brief     全ての要素のe^{vector}を返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
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
	*  @brief     全ての要素のlog2{vector}を返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Log2(ConstVector128 vector) noexcept
	{
		/*----------------------------------------------------------------------
		*   指数部と仮数部を取り出す.
		/*----------------------------------------------------------------------*/
		// vectorの指数部を取り出す  
		__m128i rawBiased      = _mm_and_si128(_mm_castps_si128(vector),VECTOR_128F_INFINITY);
		// vectorの仮数部を取り出す
		__m128i trailing       = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128I_QNAN_TEST);
		// vectorの指数部が0であるかを確認する
		__m128i isExponentZero = _mm_cmpeq_epi32(VECTOR_128F_ZERO, rawBiased);

		// Compute exponent and significand for normals.
		__m128i biased      = _mm_srli_epi32(rawBiased, 23); // シフトすること指数部の数字のみを取り出す. 
		__m128i exponentNor = _mm_sub_epi32(biased, VECTOR_128I_EXPONENTIAL_BIAS);
		__m128i trailingNor = trailing;

		// Compute exponent and significand for subnormals.
		__m128i leading     = internal::GetLeadingBit(trailing); // 仮数部の最初の1の位置を取得
		__m128i shift       = _mm_sub_epi32(VECTOR_128I_NUM_TRAILING, leading); // 仮数部を正規化するためにシフト量を計算
		__m128i exponentSub = _mm_sub_epi32(VECTOR_128I_EXPONENTIAL_SUBNORMAL, shift); // 非正規化数の指数部を計算
		__m128i trailingSub = internal::multi_sll_epi32(trailing, shift);
		trailingSub = _mm_and_si128(trailingSub, VECTOR_128I_QNAN_TEST);

		__m128i select0 = _mm_and_si128(isExponentZero, exponentSub);
		__m128i select1 = _mm_andnot_si128(isExponentZero, exponentNor);
		__m128i e = _mm_or_si128(select0, select1);

		select0 = _mm_and_si128(isExponentZero, trailingSub);
		select1 = _mm_andnot_si128(isExponentZero, trailingNor);
		__m128i t = _mm_or_si128(select0, select1); // 仮数部

		/*----------------------------------------------------------------------
		*   以下ではテイラー展開を用いてLog2の近似値を計算する
		*   log2(y) = a7 * mantissa^7 + a6 * mantissa^6 + .. + a0
		/*----------------------------------------------------------------------*/
		// Compute the approximation.
		// tempは1.f (fは仮数部)を表す
		__m128i temp = _mm_or_si128(VECTOR_128F_ONE, t); 

		// mantissaは仮数部を表現. mantissa = 1.f(仮数部) - 1
		__m128 mantissa = _mm_sub_ps(_mm_castsi128_ps(temp), VECTOR_128F_ONE);

		// 以下の計算では, 繰り返しmantissaをかけることで, 乗数として表現可能
		__m128 log2 = MultiplyAdd(VECTOR_128F_LOG_ESTIMATE7, mantissa, VECTOR_128F_LOG_ESTIMATE6); // a7*mantissa + a6
		log2 = MultiplyAdd(log2, mantissa, VECTOR_128F_LOG_ESTIMATE5); // (a7 * mantissa + a6) * manttisa + a5 
		log2 = MultiplyAdd(log2, mantissa, VECTOR_128F_LOG_ESTIMATE4);
		log2 = MultiplyAdd(log2, mantissa, VECTOR_128F_LOG_ESTIMATE3);
		log2 = MultiplyAdd(log2, mantissa, VECTOR_128F_LOG_ESTIMATE2);
		log2 = MultiplyAdd(log2, mantissa, VECTOR_128F_LOG_ESTIMATE1);
		log2 = MultiplyAdd(log2, mantissa, VECTOR_128F_LOG_ESTIMATE0);
		log2 = MultiplyAdd(log2, mantissa, _mm_cvtepi32_ps(e)); // 最後に指数部を加えることで, 浮動小数点のLogを得られる.

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
	*  @brief     全ての要素のlog10{vector}を返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Log10(ConstVector128 vector) noexcept
	{
		/*----------------------------------------------------------------------
		*   指数部と仮数部を取り出す.
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
		*   以下ではテイラー展開を用いてLog2の近似値を計算する
		*   log2(y) = a7 * mantissa^7 + a6 * mantissa^6 + .. + a0
		/*----------------------------------------------------------------------*/
		// Compute the approximation.
		// tempは1.f (fは仮数部)を表す
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

		// 基底を変化させる
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
	*  @brief     全ての要素のloge{vector}を返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LogE(ConstVector128 vector) noexcept
	{
		/*----------------------------------------------------------------------
		*   指数部と仮数部を取り出す.
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
		*   以下ではテイラー展開を用いてLog2の近似値を計算する
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

		// 基底を変化させる
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
	*  @brief     全ての要素のPowを返します  base^{power}
	*
	*  @param[in] ConstVector128 : base
	*  @param[in] ConstVector128 : power
	*
	*  @return 　　Vector128
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
	*  @brief     全ての要素にSinを取ったものを返します[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Sin(ConstVector128 vector) noexcept
	{
		// -PI <= theta <= PIの範囲に収める
		Vector128 normalizedAngle = ModAngles(vector);

		// -PI/2 <= theta < PI/2の時はsinの対称性を使用する
		// 角度の符号ビットを取得する
		__m128 sign = _mm_and_ps(normalizedAngle, VECTOR_128U_NEGATIVE_ZERO);

		// angleが正の場合はPI, 負の場合は-PIを生成する
		__m128 piOrMinusPi = _mm_or_ps(VECTOR_128F_PI, sign);

		// |normalizedAngle|を取得する
		__m128 absoluteAngle = _mm_andnot_ps(sign, normalizedAngle);

		// 反射角の計算
		__m128 reflectedAngle = _mm_sub_ps(piOrMinusPi, normalizedAngle);

		// 第一象限にあるかを判定する
		__m128 isFirstQuadrant = _mm_cmple_ps(absoluteAngle, VECTOR_128F_HALF_PI);

		// 第一象限にある場合はその値を使用し, そうでない場合は0を取得する
		__m128 angleInFirstQuadrant = _mm_and_ps(isFirstQuadrant, normalizedAngle);

		// 第二象限にある場合は反射角を使用し, そうでない場合は0を取得する
		__m128 angleInSecondQuadrant = _mm_andnot_ps(isFirstQuadrant, reflectedAngle);

		// ビット単位のORを取ることで, angleが第一象限にある場合はその値を, 第二象限にある場合は反射角を取得
		normalizedAngle = _mm_or_ps(angleInFirstQuadrant, angleInSecondQuadrant);

		__m128 angleSquared = _mm_mul_ps(normalizedAngle, normalizedAngle);

		// 多項式近似の計算
		// テイラー展開の係数
		const Vector128 sinCoefficient0 = VECTOR_128F_SIN_COEFFICIENTS0;
		const Vector128 sinCoefficient1 = VECTOR_128F_SIN_COEFFICIENTS1;
		__m128 constantsA = PERMUTE_PS(sinCoefficient0, _MM_SHUFFLE(3, 3, 3, 3));
		__m128 constantsB = PERMUTE_PS(sinCoefficient1, _MM_SHUFFLE(0, 0, 0, 0));

		// テイラー展開の結果を取得する
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
	*  @brief     全ての要素にCosを取ったものを返します
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Cos(ConstVector128 vector) noexcept
	{
		// -PI <= theta <= PIの範囲に収める
		Vector128 normalizedAngle = ModAngles(vector);

		// -PI/2 <= theta < PI/2の時はcosの対称性を使用するcos(y) = sign*cos(x).
		// 角度の符号ビットを取得する
		__m128 sign = _mm_and_ps(normalizedAngle, VECTOR_128U_NEGATIVE_ZERO);

		// angleが正の場合はPI, 負の場合は-PIを生成する
		__m128 piOrMinusPi = _mm_or_ps(VECTOR_128F_PI, sign);

		// |normalizedAngle|を取得する
		__m128 absoluteAngle = _mm_andnot_ps(sign, normalizedAngle);

		// 反射角の計算
		__m128 reflectedAngle = _mm_sub_ps(piOrMinusPi, normalizedAngle);

		// 第一象限にあるかを判定する
		__m128 isFirstQuadrant = _mm_cmple_ps(absoluteAngle, VECTOR_128F_HALF_PI);

		// 第一象限にある場合はその値を使用し, そうでない場合は0を取得する
		__m128 angleInFirstQuadrant = _mm_and_ps(isFirstQuadrant, normalizedAngle);

		// 第二象限にある場合は反射角を使用し, そうでない場合は0を取得する
		__m128 angleInSecondQuadrant = _mm_andnot_ps(isFirstQuadrant, reflectedAngle);

		// ビット単位のORを取ることで, angleが第一象限にある場合はその値を, 第二象限にある場合は反射角を取得
		normalizedAngle       = _mm_or_ps(angleInFirstQuadrant, angleInSecondQuadrant);
		angleInFirstQuadrant  = _mm_and_ps(isFirstQuadrant   , VECTOR_128F_ONE);
		angleInSecondQuadrant = _mm_andnot_ps(isFirstQuadrant, VECTOR_128F_NEGATIVE_ONE);
		sign = _mm_or_ps(angleInFirstQuadrant, angleInSecondQuadrant);

		__m128 angleSquared = _mm_mul_ps(normalizedAngle, normalizedAngle);

		// 多項式近似の計算
		// テイラー展開の係数
		const Vector128 cosCoefficient0 = VECTOR_128F_COS_COEFFICIENTS0;
		const Vector128 cosCoefficient1 = VECTOR_128F_COS_COEFFICIENTS1;
		__m128 constantsA = PERMUTE_PS(cosCoefficient0, _MM_SHUFFLE(3, 3, 3, 3));
		__m128 constantsB = PERMUTE_PS(cosCoefficient1, _MM_SHUFFLE(0, 0, 0, 0));

		// テイラー展開の結果を取得する
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
	*  @brief     全ての要素にTanを取ったものを返します
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Tan(ConstVector128 vector) noexcept
	{
		static const Vector128f tanCoefficients0 = { { { 1.0f, -4.667168334e-1f, 2.566383229e-2f, -3.118153191e-4f } } };
		static const Vector128f tanCoefficients1 = { { { 4.981943399e-7f, -1.333835001e-1f, 3.424887824e-3f, -1.786170734e-5f } } };
		static const Vector128f tanConstants = { { { 1.570796371f, 6.077100628e-11f, 0.000244140625f, 0.63661977228f /*2 / Pi*/ } } };
		static const Vector128u mask         = { { { 0x1, 0x1, 0x1, 0x1 } } };

		// 定数値の事前準備
		Vector128 twoDivPi = SplatW(tanConstants.V);
		Vector128 zero     = Zero();
		Vector128 c0       = SplatX(tanConstants.V);
		Vector128 c1       = SplatY(tanConstants.V);
		Vector128 epsilon  = SplatZ(tanConstants.V);

		// 2 / PIで乗算し, その結果を四捨五入する. 
		Vector128 va = Multiply(vector, twoDivPi);
		va = Round(va);

		// vaの各要素をc0, c1を使って補正することで, Vの各要素が最も近いPI / 2からどれだけ離れているかを求める
		Vector128 vc = NegativeMultiplySubtract(va, c0, vector);
		Vector128 vb = Abs(va);
		vc = NegativeMultiplySubtract(va, c1, vc);

		// vbはvaが正か負かを判定するために使用する
		reinterpret_cast<__m128i*>(&vb)[0] = _mm_cvttps_epi32(vb);

		// テイラー展開に使用する定数の作成
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
	*  @brief     全ての要素に対するsin成分とcos成分を取り出します 
	*
	*  @param[in] ConstVector128 : input
	*  @param[out] Vector128* sin
	*  @param[out] Vector128* cos
	*
	*  @return 　　void
	*****************************************************************************/
	inline void SIMD_CALL_CONVENTION Vector128Utility::SinCos(ConstVector128 input, Vector128* sin, Vector128* cos) noexcept
	{
		// -PI <= theta <= PIの範囲に収める
		Vector128 normalizedAngle = ModAngles(input);

		// -PI/2 <= theta < PI/2の時はsin, cosの対称性を使用するsin(y) = sin(x), cos(y) = sign*cos(x).
		Vector128 sign = _mm_and_ps(normalizedAngle, VECTOR_128U_NEGATIVE_ZERO);

		// angleが正の場合はPI, 負の場合は-PIを生成する
		__m128 piOrMinusPi = _mm_or_ps(VECTOR_128F_PI, sign);  // pi when x >= 0, -pi when x < 0

		// |absoluteAngle|を取得する
		__m128 absoluteAngle = _mm_andnot_ps(sign, normalizedAngle);  // |x|

		// 反射角の計算
		__m128 reflectedAngle = _mm_sub_ps(piOrMinusPi, normalizedAngle);

		// 第一象限にあるかを判定する
		__m128 isFirstQuadrant = _mm_cmple_ps(absoluteAngle, VECTOR_128F_HALF_PI);

		// 第一象限にある場合はその値を使用し, そうでない場合は0を取得する
		__m128 angleInFirstQuadrant  = _mm_and_ps(isFirstQuadrant, normalizedAngle);

		// 第二象限にある場合は反射角を使用し, そうでない場合は0を取得する
		__m128 angleInSecondQuadrant = _mm_andnot_ps(isFirstQuadrant, reflectedAngle);

		// ビット単位のORを取ることで, angleが第一象限にある場合はその値を, 第二象限にある場合は反射角を取得
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
	*  @brief     全ての要素にSinHを取ったものを返します[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SinH(ConstVector128 vector) noexcept
	{
		// 自然対数の底を2からeに変換
		static const Vector128f scale = { { { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f } } }; // 1.0f / ln(2.0f)

		// sinh(x) = (e^x + e^-x) / 2 - 1として計算
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
	*  @brief     全ての要素にCosHを取ったものを返します[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CosH(ConstVector128 vector) noexcept
	{
		// 自然対数の底を2からeに変換
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
	*  @brief     全ての要素にTanHを取ったものを返します[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
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
	*  @brief     全ての要素にArcSinを取ったものを返します[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ArcSin(ConstVector128 vector) noexcept
	{
		__m128 isNonNegative  = _mm_cmpge_ps(vector, VECTOR_128F_ZERO); // 0以上であるか
		__m128 mValue         = _mm_sub_ps(VECTOR_128F_ZERO, vector);   // -vector
		__m128 absoluteVector = _mm_max_ps(vector, mValue);             // |vector|

		// (1-|V|)を計算し、負の数のsqrtを避けるためにゼロにクランプする
		__m128 oneMinusValue      = _mm_sub_ps(VECTOR_128F_ONE, absoluteVector);
		__m128 clampOneMinusValue = _mm_max_ps(VECTOR_128F_ZERO, oneMinusValue);
		__m128 rootOneMinusValue  = _mm_sqrt_ps(clampOneMinusValue);

		// テイラー展開を用いてarcsinの近似値を計算する
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
	*  @brief     全ての要素にArcSinを取ったものを返します[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ArcCos(ConstVector128 vector) noexcept
	{
		__m128 isNonNegative  = _mm_cmpge_ps(vector, VECTOR_128F_ZERO); // 0以上であるか
		__m128 mValue         = _mm_sub_ps(VECTOR_128F_ZERO, vector);   // -vector
		__m128 absoluteVector = _mm_max_ps(vector, mValue);             // |vector|

		// (1-|V|)を計算し、負の数のsqrtを避けるためにゼロにクランプする
		__m128 oneMinusValue      = _mm_sub_ps(VECTOR_128F_ONE, absoluteVector);
		__m128 clampOneMinusValue = _mm_max_ps(VECTOR_128F_ZERO, oneMinusValue);
		__m128 rootOneMinusValue  = _mm_sqrt_ps(clampOneMinusValue);

		// テイラー展開を用いてarcsinの近似値を計算する
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
	*  @brief     全ての要素にArcTanを取ったものを返します[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ArcTan(ConstVector128 vector) noexcept
	{
		__m128 absoluteVector = Abs(vector);                           // |vector|
		__m128 inverseVector  = _mm_div_ps(VECTOR_128F_ONE, vector);   // 1 / vector
		__m128 comparison     = _mm_cmpgt_ps(vector, VECTOR_128F_ONE); // vector > 1
		__m128 select0        = _mm_and_ps(comparison, VECTOR_128F_ONE);
		__m128 select1        = _mm_andnot_ps(comparison, VECTOR_128F_NEGATIVE_ONE);
		__m128 sign           = _mm_or_ps(select0, select1);           // vectorの符号
		comparison = _mm_cmple_ps(absoluteVector, VECTOR_128F_ONE);    // |vector| < 1
		select0    = _mm_and_ps(comparison, VECTOR_128F_ZERO);
		select1    = _mm_andnot_ps(comparison, sign);
		__m128 x   = _mm_or_ps(select0, select1);
		__m128 x2  = _mm_mul_ps(x, x);

		// テイラー展開を用いてarcsinの近似値を計算する
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
	*  @brief     全ての要素にArcTan2を取ったものを返します[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
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
	*  @brief     線形補間を返します
	*
	*  @param[in] ConstVector128  t = 0の時の値
	*  @param[in] ConstVector128  t = 1の時の値
	*  @param[in] float  t : 線形補間の割合
	*
	*  @return 　　Vector
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
	*  @brief     線形補間を返します
	*
	*  @param[in] ConstVector128  t = 0の時の値
	*  @param[in] ConstVector128  t = 1の時の値
	*  @param[in] ConstVector128  t : 線形補間の割合
	*
	*  @return 　　Vector
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
	*  @brief     エルミート補間を返します
	*
	*  @param[in]  始点の位置
	   @param[in]  始点におけるスプラインの接線を表すベクトル
	   @param[in]  終点の位置
	   @param[in]  終点におけるスプラインの接線を表すベクトル
	   @param[in]  t : エルミート補間の割合
	*
	*  @return 　　Vector128
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
	*  @brief     エルミート補間を返します
	*
	*  @param[in]  始点の位置
	   @param[in]  始点におけるスプラインの接線を表すベクトル
	   @param[in]  終点の位置
	   @param[in]  終点におけるスプラインの接線を表すベクトル
	   @param[in]  t : エルミート補間の割合
	*
	*  @return 　　Vector128
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
	*  @brief     CatMull-Romスプライン補間の結果を返します。4つの制御点を全て通るスプライン補間
	*
	*  @param[in]  制御点1
	   @param[in]  制御点2
	   @param[in]  制御点3
	   @param[in]  制御点4
	   @param[in]  t : 補間の割合
	*
	*  @return 　　Vector128
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
	*  @brief     CatMull-Romスプライン補間の結果を返します。4つの制御点を全て通るスプライン補間
	*
	*  @param[in]  制御点1
	   @param[in]  制御点2
	   @param[in]  制御点3
	   @param[in]  制御点4
	   @param[in]  t : 補間の割合
	*
	*  @return 　　Vector128
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
	*  @brief     BaryCentric 三角形の重心重み位置を算出
	*              (f>=0 && g>=0 && 1-f-g>=0) の場合、ポイントは三角形の Position0>Position1>Position2 の内側にあります>。
				   (f==0 && g>=0 && 1-f-g>=0) の場合、ポイントは Position0>Position2 行目にあります>。
				   (f>=0 && g==0 && 1-f-g>=0) の場合、ポイントは Position0>Position1> 行目にあります。
				   (f>=0 && g>=0 && 1-f-g==0) の場合、ポイントは Position1>Position2 行目にあります>。
	*
	*  @return 　　Vector128
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
	*  @brief     BaryCentric 三角形の重心重み位置を算出
	*              (f>=0 && g>=0 && 1-f-g>=0) の場合、ポイントは三角形の Position0>Position1>Position2 の内側にあります>。
				   (f==0 && g>=0 && 1-f-g>=0) の場合、ポイントは Position0>Position2 行目にあります>。
				   (f>=0 && g==0 && 1-f-g>=0) の場合、ポイントは Position0>Position1> 行目にあります。
				   (f>=0 && g>=0 && 1-f-g==0) の場合、ポイントは Position1>Position2 行目にあります>。
	*
	*  @return 　　Vector128
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
	*  @brief     全ての要素について最小値となる方を選択します
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　Vector
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
	*  @brief     全ての要素について最大値となる方を選択します
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　Vector128
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
	*  @brief     最も近い整数に丸める
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Round(ConstVector128 vector) noexcept
	{
		__m128 sign = _mm_and_ps(vector, VECTOR_128U_NEGATIVE_ZERO);

		// magicはvectorの符号を維持しつつ, 小数部分を0にする
		__m128 magic = _mm_or_ps(VECTOR_128F_NO_FRACTION, sign);

		// 小数部分を丸める
		__m128 r1 = _mm_add_ps(vector, magic);
		r1 = _mm_sub_ps(r1, magic);

		// vectorの絶対値を取得
		__m128 r2 = _mm_and_ps(vector, VECTOR_128U_MASK_ABS);

		// maskは, r2が1.0未満の場合にtrueを返す
		__m128 mask = _mm_cmple_ps(r2, VECTOR_128F_NO_FRACTION);

		// maskがfalseの場合にvectorを返す
		r2 = _mm_andnot_ps(mask, vector);

		// r1はmaskがtrueの場合にR1を返します
		r1 = _mm_and_ps(r1, mask);

		// 排他的論理和により, Vが1.0未満の場合はR1を、そうでない場合はR2を返します。
		return _mm_xor_ps(r1, r2);
	}

	/****************************************************************************
	*                       Truncate
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Truncate(ConstVector128 vector) noexcept
	*
	*  @brief     切り捨て (指定した数値の端数を切り捨てた整数に丸める。数直線上で0の方向にある次の整数
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Truncate(ConstVector128 vector) noexcept
	{
		// NAN, INF, 8388608より大きい数値を扱うには, マスキングを使用する
		// 絶対値を取得
		__m128i test = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128U_MASK_ABS);

		// 8388608より大きいかどうかのテスト (すべての浮動小数点数、NAN、INFなし)
		test = _mm_cmplt_epi32(test, VECTOR_128F_NO_FRACTION);

		// 切り捨てで丸めるために、intに変換してfloatに戻す。
		__m128i intVector = _mm_cvttps_epi32(vector);

		Vector128 result = _mm_cvtepi32_ps(intVector);

		// 8388608未満の数値はすべてint型に丸める
		result = _mm_and_ps(result, _mm_castsi128_ps(test));

		// それ以外の場合は、元の値を使用する
		test = _mm_andnot_si128(test, _mm_castps_si128(vector));
		result = _mm_or_ps(result, _mm_castsi128_ps(test));
		return result;
	}

	/****************************************************************************
	*                       Floor
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Floor(ConstVector128 vector) noexcept
	*
	*  @brief     負の方向に丸める
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Floor(ConstVector128 vector) noexcept
	{
		// NAN, INF, 8388608より大きい数値を扱うには, マスキングを使用する
		// 絶対値を取得
		__m128i test = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128U_MASK_ABS);

		// 8388608より大きいかどうかのテスト (すべての浮動小数点数、NAN、INFなし)
		test = _mm_cmplt_epi32(test, VECTOR_128F_NO_FRACTION);

		// 切り捨てで丸めるために、intに変換してfloatに戻す。
		__m128i intVector = _mm_cvttps_epi32(vector);

		Vector128 result = _mm_cvtepi32_ps(intVector);

		__m128 larger = _mm_cmpgt_ps(result, vector);

		// 0 -> 0, 0xffffffff -> -1.0f
		larger = _mm_cvtepi32_ps(_mm_castps_si128(larger));
		result = _mm_add_ps(result, larger);

		// 8388608未満の数値はすべてint型に丸める
		result = _mm_and_ps(result, _mm_castsi128_ps(test));

		// それ以外の場合は、元の値を使用する
		test = _mm_andnot_si128(test, _mm_castps_si128(vector));
		result = _mm_or_ps(result, _mm_castsi128_ps(test));
		return result;
	}

	/****************************************************************************
	*                      Ceiling
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Ceiling(ConstVector128 vector) noexcept
	*
	*  @brief     切り上げ 数直線上で正の方向にある次の整数に丸める
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Ceiling(ConstVector128 vector) noexcept
	{
		// NAN, INF, 8388608より大きい数値を扱うには, マスキングを使用する
		// 絶対値を取得
		__m128i test = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128U_MASK_ABS);

		// 8388608より大きいかどうかのテスト (すべての浮動小数点数、NAN、INFなし)
		test = _mm_cmplt_epi32(test, VECTOR_128F_NO_FRACTION);

		// 切り捨てで丸めるために、intに変換してfloatに戻す。
		__m128i intVector = _mm_cvttps_epi32(vector);

		Vector128 result = _mm_cvtepi32_ps(intVector);

		__m128 smaller = _mm_cmplt_ps(result, vector);

		// 0 -> 0, 0xffffffff -> -1.0f
		smaller = _mm_cvtepi32_ps(_mm_castps_si128(smaller));
		result = _mm_add_ps(result, smaller);

		// 8388608未満の数値はすべてint型に丸める
		result = _mm_and_ps(result, _mm_castsi128_ps(test));

		// それ以外の場合は、元の値を使用する
		test = _mm_andnot_si128(test, _mm_castps_si128(vector));
		result = _mm_or_ps(result, _mm_castsi128_ps(test));
		return result;
	}

	/****************************************************************************
	*                       Clamp
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Clamp(ConstVector128 vector, ConstVector128 min, ConstVector128 max) noexcept
	*
	*  @brief     指定範囲内で数値を設定する
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] ConstVector128 min
	*  @param[in] ConstVector128 max
	*
	*  @return 　　Vector
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
	*  @brief     1以上には行かないようにする
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector
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
	*  @brief     絶対値
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector
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
	*  @brief      - pi <= theta <= piの範囲に収めるようにします.
	*
	*  @param[in] ConstVector128 angles : 角度rad
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ModAngles(ConstVector128 angles) noexcept
	{
		// PI <= angle < PI となるように、与えられた角度の範囲のモジュロ演算を行う
		Vector128 result = _mm_mul_ps(angles, VECTOR_128F_RECIPROCAL_2PI.V);

		// 丸め処理が複雑なため、インライン関数を使用する。
		result = Round(result);
		return NegativeMultiplySubtract(result, VECTOR_128F_2PI, angles);
	}

	/****************************************************************************
	*                       AddAngles
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::AddAngles(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     - pi <= theta <= piの範囲に収めるようにした上で足し算を行います.
	*
	*  @param[in] ConstVector128 left angles : 角度rad
	*  @param[in] ConstVector128 right angles : 角度rad
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::AddAngles(ConstVector128 left, ConstVector128 right) noexcept
	{
		// 角度の純粋な差分
		Vector128 result = _mm_add_ps(left, right);

		// -Piよりも小さいかを判定する
		Vector128 offset = _mm_cmplt_ps(result, VECTOR_128F_NEGATIVE_PI);
		offset = _mm_and_ps(offset, VECTOR_128F_2PI);

		// -Piよりも全ての要素で小さい場合, 2Piを加える
		result = _mm_add_ps(result, offset);

		// Piよりも大きくなってしまっているか
		offset = _mm_cmpge_ps(result, VECTOR_128F_PI);
		offset = _mm_and_ps(offset, VECTOR_128F_2PI);

		// piよりも大きい場合は2piを全ての要素で引く
		result = _mm_sub_ps(result, offset);
		return result;
	}

	/****************************************************************************
	*                       SubtractAngles
	****************************************************************************/
	/* @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ModAngles(ConstVector128 vector)
	*
	*  @brief     - pi <= theta <= piの範囲に収めるようにした上で引き算を行います.
	*
	*  @param[in] ConstVector128 left angles : 角度rad
	*  @param[in] ConstVector128 right angles : 角度rad
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SubtractAngles(ConstVector128 left, ConstVector128 right) noexcept
	{
		// 角度の純粋な差分
		Vector128 result = _mm_sub_ps(left, right);

		// -Piよりも小さいかを判定する
		Vector128 offset = _mm_cmplt_ps(result, VECTOR_128F_NEGATIVE_PI);
		offset           = _mm_and_ps(offset, VECTOR_128F_2PI);

		// -Piよりも全ての要素で小さい場合, 2Piを加える
		result = _mm_add_ps(result, offset);

		// Piよりも大きくなってしまっているか
		offset = _mm_cmpge_ps(result, VECTOR_128F_PI);
		offset = _mm_and_ps(offset, VECTOR_128F_2PI);

		// piよりも大きい場合は2piを全ての要素で引く
		result = _mm_sub_ps(result, offset);
		return result;
	}
	#pragma endregion Angle
#pragma endregion Implement
}



#endif PLATFORM_CPU_INSTRUCTION_SSE

#endif GM_SIMD_VECTOR_128_SSE_HPP