/////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdVector256SSE.hpp
///             @brief  SSEによるSimd演算です.
///                     Vector256型の実装 
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
	*         256 bitの整数型もdouble型も格納可能なベクトルを作成する
	*----------------------------------------------------------------------*/
	using Vector256 = __m256d;

	/*----------------------------------------------------------------------
	*        定数としての変換用, doubleポインタに変換したり, m256の各型に変換したり
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
	*  @brief     SSEのバージョンによるSimd演算
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
		*  @brief : ゼロ初期化したVector256を返す関数
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Zero() noexcept;

		/*----------------------------------------------------------------------
		*  @brief : double4つからVector256クラスを生成する
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Set(const double x, const double y, const double z, const double w) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : double1つからVector256クラスを生成する
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Set(const double value) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : Int4つからVector256クラスを生成する
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION SetInt(const gu::uint64 x, const gu::uint64 y, const gu::uint64 z, const gu::uint64 w) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : Floatを使ってVectorのX(, Y, Z, W)要素に値を代入する
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION SetX(ConstVector256 vector, const double x) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION SetY(ConstVector256 vector, const double y) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION SetZ(ConstVector256 vector, const double z) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION SetW(ConstVector256 vector, const double w) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ベクトルを使ってdouble配列に代入する
		*----------------------------------------------------------------------*/
		__forceinline static void SIMD_CALL_CONVENTION StoreDouble (double* destination, ConstVector256 source) noexcept;
		__forceinline static void SIMD_CALL_CONVENTION StoreDouble2(double* destination, ConstVector256 source) noexcept;
		__forceinline static void SIMD_CALL_CONVENTION StoreDouble3(double* destination, ConstVector256 source) noexcept;
		__forceinline static void SIMD_CALL_CONVENTION StoreDouble4(double* destination, ConstVector256 source) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : X(, Y, Z, W)等の要素に基づいて, 全ての要素に同じ値を持ったVector256クラスを作成する
		*           X,Y,Z,W  : 各要素
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
		*  @brief : ベクトルを並び替えます.indexを指定するとそのindexにあったVector4型を取得
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Swizzle(ConstVector256 vector,
			const gu::uint64 xIndex, const gu::uint64 yIndex,
			const gu::uint64 zIndex, const gu::uint64 wIndex) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルから要素を抽出し, 新しいベクトルを作成する
		*           0～3 : aからのx, y, z, w
		*           4～7 : bからのx, y, z, w
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Permute(ConstVector256 a, ConstVector256,
			const gu::uint64 xIndex, const gu::uint64 yIndex,
			const gu::uint64 zIndex, const gu::uint64 wIndex) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルから要素を抽出し, 新しいベクトルを作成する
		*           control vectorが0の場合はleftから, 非0の場合はrightからそれぞれ抽出が行われます
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Select(ConstVector256 left, ConstVector256 right, ConstVector256 control) noexcept;
		#pragma endregion Setter

		#pragma region Getter
		/*----------------------------------------------------------------------
		*  @brief : FPUレジスタに格納されたX(, Y, Z, W)等の要素を取り出す
		*----------------------------------------------------------------------*/
		__forceinline static double SIMD_CALL_CONVENTION GetX(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION GetY(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION GetZ(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION GetW(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : doubleの配列を使ってベクトルに格納する
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION LoadDouble (const double* source) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION LoadDouble2(const double* source) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION LoadDouble3(const double* source) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION LoadDouble4(const double* source) noexcept;

		#pragma endregion Getter

		#pragma region Operator
		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルの各要素同士の和を算出します. (left + right)
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Add(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルの各要素同士の差を算出します. (left - right)
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Subtract(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルの各要素同士の積を算出します. (left * right)
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Multiply(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルの各要素同士の除算を算出します. (left / right)
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Divide(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルの各要素同士のあまりを算出します. (left % right)
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Mod(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief :  subtractLeft - (multiplyLeft * multiplyRight)を算出します.
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION NegativeMultiplySubtract(ConstVector256 multiplyLeft, ConstVector256 multiplyRight, ConstVector256 subtractLeft) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : multiplyLeft * multiplyRight + addを算出します.
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION MultiplyAdd(ConstVector256 multiplyLeft, ConstVector256 multiplyRight, ConstVector256 add) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ベクトルの各要素同士の逆数を算出します. (1.0f / vector)
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Reciprocal(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : double値を使って各要素に掛けます scale * vector
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Scale(ConstVector256 vector, const double scale) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 二つのベクトルが等しいかどうかを調べます
		*           (Eachは各要素毎に等しいかどうかを調べます.)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION EqualVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION EqualVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION EqualVector4(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION EqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 二つのベクトルが等しくないかを調べます
		*           (Eachは各要素毎に等しいかどうかを調べます.)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualVector4(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION NotEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 二つのベクトルがすべて等しいかどうかを調べます.
		*           比較時には符号なし整数(U32)として比較します.
		*           (Eachは各要素ごとに等しいかどうかを調べます)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION EqualAsIntVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION EqualAsIntVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION EqualAsIntVector4(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION EqualAsIntVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 二つのベクトルが一つでも等しくないかどうかを調べます.
		*           比較時には符号なし整数(U32)として比較します.
		*           (Eachは各要素ごとに等しいかどうかを調べます)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualAsIntVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualAsIntVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NotEqualAsIntVector4(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION NotEqualAsIntVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 一定範囲内で値が等しいかを調べる (left - epsilon <= right <= left + epsilon)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector2   (ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector3   (ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector4   (ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION NearEqualVectorEach(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 左の方が全ての要素において大きいかを調べます (left > right)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterVector2   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterVector3   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterVector4   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION GreaterVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 左の方が全ての要素において大きいor等しいかを調べます (left >= right)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterOrEqualVector2   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterOrEqualVector3   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterOrEqualVector4   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION GreaterOrEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 左の方が全ての要素において小さいかを調べます (left < right)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION LessVector2   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessVector3   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessVector4   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION LessVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 左の方が全ての要素において小さいor等しいかを調べます (left <= right)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION LessOrEqualVector2   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessOrEqualVector3   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessOrEqualVector4   (ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION LessOrEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 指定範囲内にあるかどうか -bounds <= vector <= +bounds
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION InBoundsVector2(ConstVector256 vector, ConstVector256 bounds) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION InBoundsVector3(ConstVector256 vector, ConstVector256 bounds) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION InBoundsVector4(ConstVector256 vector, ConstVector256 bounds) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION InBoundsVectorEach(ConstVector256 vector, ConstVector256 bounds) noexcept;

		#pragma endregion Operator

		#pragma region Bit
		/*----------------------------------------------------------------------
		*  @brief : ビット単位の論理積の演算を行います
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION AndInt(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ビット単位の論理和の演算を行います
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION OrInt(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ビット単位の論理積否定の演算を行います
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION NorInt(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ビット単位の排他的論理和の演算を行います
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION XorInt(ConstVector256 left, ConstVector256 right) noexcept;

		#pragma endregion Bit

		#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : 各要素にマイナスを取ったものを返す.
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Negate(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
		*----------------------------------------------------------------------*/
		__forceinline static double SIMD_CALL_CONVENTION LengthVector2(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION LengthVector3(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION LengthVector4(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2次ノルムの二乗を算出
		*----------------------------------------------------------------------*/
		__forceinline static double SIMD_CALL_CONVENTION LengthSquaredVector2(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION LengthSquaredVector3(ConstVector256 vector) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION LengthSquaredVector4(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 内積を算出 (ただし, 返り値はdoubleではなくVector256型で返されますので, 後々GetXなどで対応してください.)
		*----------------------------------------------------------------------*/
		__forceinline static double SIMD_CALL_CONVENTION DotVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION DotVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static double SIMD_CALL_CONVENTION DotVector4(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 外積を算出 left, rightに垂直なベクトルを返す
		*           2次元の場合は平行四辺形の面積を返すという意味合い
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION CrossVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION CrossVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION CrossVector4(ConstVector256 first, ConstVector256 second, ConstVector256 third) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 入力ベクトルの単位ベクトルを返します
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION NormalizeVector2(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION NormalizeVector3(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION NormalizeVector4(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief     反射ベクトルを算出
		*  @param[in] ConstVector256 : input  (入射ベクトル)
		*  @param[in] ConstVector256 : normal (反射を行う法線ベクトル)
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION ReflectVector2(ConstVector256 input, ConstVector256 normal) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION ReflectVector3(ConstVector256 input, ConstVector256 normal) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION ReflectVector4(ConstVector256 input, ConstVector256 normal) noexcept;

		/*----------------------------------------------------------------------
		*  @brief     屈折ベクトルを算出
		*  @param[in] ConstVector256 : input  (入射ベクトル)
		*  @param[in] ConstVector256 : normal (反射を行う法線ベクトル)
		*  @param[in] const double    : refractionIndex (屈折率)
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION RefractVector2(ConstVector256 input, ConstVector256 normal, const double refractionIndex) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION RefractVector3(ConstVector256 input, ConstVector256 normal, const double refractionIndex) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION RefractVector4(ConstVector256 input, ConstVector256 normal, const double refractionIndex) noexcept;

		__forceinline static Vector256 SIMD_CALL_CONVENTION RefractVector2V(ConstVector256 input, ConstVector256 normal, ConstVector256 refractionIndex) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION RefractVector3V(ConstVector256 input, ConstVector256 normal, ConstVector256 refractionIndex) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION RefractVector4V(ConstVector256 input, ConstVector256 normal, ConstVector256 refractionIndex) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 非常に大きい値か
		*----------------------------------------------------------------------*/
		__forceinline static bool IsInfiniteVector2(ConstVector256 vector) noexcept;
		__forceinline static bool IsInfiniteVector3(ConstVector256 vector) noexcept;
		__forceinline static bool IsInfiniteVector4(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素に根号を取ったものを返します
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Sqrt(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の逆数を取り, それに根号を取ったものを返します
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION ReciprocalSqrt(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のsin, cos, tanを返します [rad]
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Sin(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION Cos(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION Tan(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のarcsin, arccos, arctanを返します
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION ArcSin(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION ArcCos(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION ArcTan(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のsinH, cosH, tanHを返します
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION SinH(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION CosH(ConstVector256 vector) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION TanH(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素に対するsin成分とcos成分を取り出します
		*----------------------------------------------------------------------*/
		__forceinline static void SIMD_CALL_CONVENTION SinCos(ConstVector256 input, Vector256* sin, Vector256* cos) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の2^{vector}を返します
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Exp2(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の10^{vector}を返します
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Exp10(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のe^{vector}を返します
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION ExpE(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のlog2{vector}を返します
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Log2(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のlog10{vector}を返します
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Log10(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のloge{vector}を返します
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION LogE(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のPowを返します
		*           base^{power}
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Pow(ConstVector256 base, ConstVector256 power) noexcept;

		/*----------------------------------------------------------------------
		*  @brief      線形補間を返します
		*  @param[in]  t = 0の時の値
		   @param[in]  t = 1の時の値
		   @param[in]  t : 線形補間の割合
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Lerp(ConstVector256 start, ConstVector256 end, const double t) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION LerpV(ConstVector256 start, ConstVector256 end, ConstVector256 t) noexcept;


		/*----------------------------------------------------------------------
		*  @brief      エルミート補間を返します
		*  @param[in]  始点の位置
		   @param[in]  始点におけるスプラインの接線を表すベクトル
		   @param[in]  終点の位置
		   @param[in]  終点におけるスプラインの接線を表すベクトル
		   @param[in]  t : エルミート補間の割合
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Hermite(ConstVector256 startPosition, ConstVector256 startTangent, ConstVector256 endPosition, ConstVector256 endTangent, const double t) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION HermiteV(ConstVector256 startPosition, ConstVector256 startTangent, ConstVector256 endPosition, ConstVector256 endTangent, ConstVector256 t) noexcept;

		/*----------------------------------------------------------------------
		*  @brief      CatMull-Romスプライン補間の結果を返します。4つの制御点を全て通るスプライン補間
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION CatMullRom(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, ConstVector256 position3, const double t   ) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION CatMullRomV(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, ConstVector256 position3, ConstVector256 t) noexcept;

		/*----------------------------------------------------------------------
		*  @brief      三角形の重心重み位置を算出
		*              (f>=0 && g>=0 && 1-f-g>=0) の場合、ポイントは三角形の Position0>Position1>Position2 の内側にあります>。
                       (f==0 && g>=0 && 1-f-g>=0) の場合、ポイントは Position0>Position2 行目にあります>。
                       (f>=0 && g==0 && 1-f-g>=0) の場合、ポイントは Position0>Position1> 行目にあります。
                       (f>=0 && g>=0 && 1-f-g==0) の場合、ポイントは Position1>Position2 行目にあります>。
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION BaryCentric (ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, const double f, const double g) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION BaryCentricV(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, ConstVector256 f, ConstVector256 g) noexcept;
		/*----------------------------------------------------------------------
		*  @brief : 全ての要素について最小値となる方を選択します
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Min(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素について最大値となる方を選択します
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Max(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 最も近い整数に丸める
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Round(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 切り捨て (指定した数値の端数を切り捨てた整数に丸める。数直線上で0の方向にある次の整数
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Truncate(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 負の方向に丸める
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Floor(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 切り上げ 数直線上で正の方向にある次の整数に丸める
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Ceiling(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 指定範囲内で数値を設定する
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Clamp(ConstVector256 vector, ConstVector256 min, ConstVector256 max) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 1以上には行かないようにする
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Saturate(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 絶対値
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Abs(ConstVector256 vector) noexcept;

		#pragma endregion Math

		#pragma region Angle
		/*----------------------------------------------------------------------
		*  @brief : - pi <= theta <= piの範囲に収めるようにします.
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION ModAngles(ConstVector256 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : - pi <= theta <= piの範囲に収めるようにした上で足し算を行います.
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION AddAngles(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : - pi <= theta <= piの範囲に収めるようにした上で引き算を行います.
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
	*  @brief     ゼロ初期化したVector256を返す関数
	*
	*  @param[in] void
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Zero() noexcept
	{
		return _mm256_setzero_pd(); // 4つの単精度浮動小数点値をクリアする
	}

	/****************************************************************************
	*                       Set
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Set(const double x, const double y, const double z, const double w) noexcept
	*
	*  @brief     double4つからVector256クラスを生成する
	*
	*  @param[in] const double x
	*  @param[in] const double y
	*  @param[in] const double z
	*  @param[in] const double w
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Set(const double x, const double y, const double z, const double w) noexcept
	{
		// 4つの単精度浮動小数点値を設定する
		// リトルエンディアンによる代入です
		return _mm256_set_pd(w, z, y, x);
	}

	/****************************************************************************
	*                       Set
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Set(const double value) noexcept
	*
	*  @brief     double1つからVector256クラスを生成する
	*
	*  @param[in] const double value
	*
	*  @return 　　Vector256
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
	*  @brief     Floatを使ってVectorのXに値を代入する
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] const double x
	*
	*  @return 　　Vector256
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
	*  @brief     Floatを使ってVectorのYに値を代入する
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] const double y
	*
	*  @return 　　Vector256
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
	*  @brief     Floatを使ってVectorのZに値を代入する
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] const double z
	*
	*  @return 　　Vector256
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
	*  @brief     Floatを使ってVectorのWに値を代入する
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] const double w
	*
	*  @return 　　Vector256
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
	*  @brief     Xの要素に基づいて, 全ての要素に同じ値を持ったVector256クラスを作成する
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
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
	*  @brief     Yの要素に基づいて, 全ての要素に同じ値を持ったVector256クラスを作成する
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
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
	*  @brief     Zの要素に基づいて, 全ての要素に同じ値を持ったVector256クラスを作成する
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
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
	*  @brief     Wの要素に基づいて, 全ての要素に同じ値を持ったVector256クラスを作成する
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
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
	*  @brief     全ての要素に1を代入したVector256クラスを作成する
	*
	*  @param[in] void
	*
	*  @return 　　Vector256
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
	*  @brief     Wの要素に基づいて, 全ての要素にINFを持ったVector256クラスを作成する
	*
	*  @param[in] void
	*
	*  @return 　　Vector256
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
	*  @brief     Vector256をもとにDouble配列に代入します
	*
	*  @param[in] double* destination
	*  @param[in] ConstVector256 source
	*
	*  @return 　　Vector256
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
	*  @brief     Vector256をもとにDouble配列に代入します
	*
	*  @param[in] double* destination
	*  @param[in] ConstVector256 source
	*
	*  @return 　　Vector256
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
	*  @brief     Vector256をもとにDouble配列に代入します
	*
	*  @param[in] double* destination
	*  @param[in] ConstVector256 source
	*
	*  @return 　　Vector256
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
	*  @brief     Vector256をもとにFloat配列に代入します
	*
	*  @param[in] double* destination
	*  @param[in] ConstVector256 source
	*
	*  @return 　　Vector256
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
	*  @brief     FPUレジスタに格納されたXの要素を取り出す
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　double
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
	*  @brief     FPUレジスタに格納されたYの要素を取り出す
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　double
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
	*  @brief     FPUレジスタに格納されたZの要素を取り出す
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　double
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
	*  @brief     FPUレジスタに格納されたWの要素を取り出す
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　double
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
	*  @brief     Doubleの配列を使って格納する (source->x, source->y, source->z, source->w)
	*
	*  @param[in] const double* source
	*
	*  @return 　　Vector256
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
	*  @brief     Doubleの配列を使って格納する (source->x, source->y, source->z, source->w)
	*
	*  @param[in] const double* source
	*
	*  @return 　　Vector256
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
	*  @brief     Doubleの配列を使って格納する (source->x, source->y, source->z, source->w)
	*
	*  @param[in] const double* source
	*
	*  @return 　　Vector256
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
	*  @brief     Doubleの配列を使って格納する (source->x, source->y, source->z, source->w)
	*
	*  @param[in] const double* source
	*
	*  @return 　　Vector256
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
	*  @brief     2つのベクトルの各要素同士の和を算出します. (left + right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　Vector256
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
	*  @brief     2つのベクトルの各要素同士の差を算出します. (left - right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　Vector256
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
	*  @brief     2つのベクトルの各要素同士の差を算出します. (left * right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　Vector256
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
	*  @brief     2つのベクトルの各要素同士の差を算出します. (left / right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　Vector256
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
	*  @brief     2つのベクトルの各要素同士のあまりを算出します. (left % right)
	*             left % right = left - right * truncate(left / right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　Vector256
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
	*  @brief     subtractLeft - (multiplyLeft * multiplyRight)を算出します.
	*
	*  @param[in] ConstVector256 multiplyLeft
	*  @param[in] ConstVector256 multiplyRight
	*  @param[in] ConstVector256 subtractLeft
	*
	*  @return 　　Vector256
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
	*  @brief     multiplyLeft * multiplyRight + addを算出します.
	*
	*  @param[in] ConstVector256 multiplyLeft
	*  @param[in] ConstVector256 multiplyRight
	*  @param[in] ConstVector256 add
	*
	*  @return 　　Vector256
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
	*  @brief     2つのベクトルの各要素同士の逆数を算出します. (1.0f / vector)
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
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
	*  @brief     double値を使って各要素に掛けます scale * vector
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] const double scale
	*
	*  @return 　　Vector256
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
	*  @brief     二つの2次元ベクトルが等しいかどうかを調べます
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVector2(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, xそれぞれに対して等しい場合は1を返すベクトル
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_EQ_OQ); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		// w, zは無視するようにする
		return (_mm256_movemask_pd(equalMaskVector) & 0b0011) == 0b0011; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       EqualVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVector3(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     二つの3次元ベクトルが等しいかどうかを調べます
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVector3(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, xそれぞれに対して等しい場合は1を返すベクトル
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_EQ_OQ); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		// wは無視するようにする
		return (_mm256_movemask_pd(equalMaskVector) & 0b0111) == 0b0111; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       EqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVector4(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     二つの4次元ベクトルが等しいかどうかを調べます
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVector4(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, xそれぞれに対して等しい場合は1を返すベクトル
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_EQ_OQ); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		return _mm256_movemask_pd(equalMaskVector) == 0b1111; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       EqualVectorEach
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     二つの4次元ベクトルが各要素ごとに等しいかどうかを調べます
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
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
	*  @brief     二つの2次元ベクトルが等しくないかどうかを調べます
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector2(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, xそれぞれに対して等しくない場合は1を返すベクトル
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_EQ_OQ); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		// w, zは無視するようにする
		return (_mm256_movemask_pd(equalMaskVector) & 0b0011) != 0b0011; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       NotEqualVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     二つの3次元ベクトルが等しくないかどうかを調べます
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector3(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, xそれぞれに対して等しくない場合は1を返すベクトル
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_EQ_OQ); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		// wは無視するようにする
		return (_mm256_movemask_pd(equalMaskVector) & 0b0111) != 0b0111; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       NotEqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     二つの4次元ベクトルが等しくないかどうかを調べます
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, xそれぞれに対して等しくない場合は1を返すベクトル
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_NEQ_OQ); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		return _mm256_movemask_pd(equalMaskVector) != 0; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       NotEqualVectorEach
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     二つの4次元ベクトルが各要素ごとに等しくないかを調べます
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
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
	*  @brief     二つのベクトルが等しいかどうかを調べます.
	*             比較時には符号なし整数(U32)として扱います.
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
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
	*  @brief     二つのベクトルが等しいかどうかを調べます.
	*             比較時には符号なし整数として扱います.
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
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
	*  @brief     二つのベクトルが等しいかどうかを調べます.
	*             比較時には符号なし整数として扱います.
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
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
	*  @brief     二つのベクトルが等しいかどうかをそれぞれの要素で調べます.
	*             比較時には符号なし整数として扱います.
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
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
	*  @brief     二つのベクトルが一つでも等しくないかどうかを調べます.
	*            比較時には符号なし整数(U32)として比較します.
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
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
	*  @brief     二つの3次元ベクトルが等しくないかどうかを調べます
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
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
	*  @brief     二つの4次元ベクトルが等しくないかどうかを調べます
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
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
	*  @brief     二つの4次元ベクトルが各要素ごとに等しくないかを調べます
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
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
	*  @brief     一定範囲内で値が等しいかを調べる (left - epsilon <= right <= left + epsilon)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*  @param[in] ConstVector256 epsilon
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::NearEqualVector2(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
	{
		// 差分の導出
		Vector256 delta = _mm256_sub_pd(left, right);

		// 絶対値の差分の取得
		Vector256 result = _mm256_setzero_pd();
		result = _mm256_sub_pd(result, delta);
		result = _mm256_max_pd(result, delta);
		result = _mm256_cmp_pd(result, epsilon, _CMP_LE_OQ);

		// zとwは無視して, xとyに着目する
		return ((_mm256_movemask_pd(result) & 0b0011) == 0b0011);
	}

	/****************************************************************************
	*                       NearEqualVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector3(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
	*
	*  @brief     一定範囲内で値が等しいかを調べる (left - epsilon <= right <= left + epsilon)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*  @param[in] ConstVector256 epsilon
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::NearEqualVector3(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
	{
		// 差分の導出
		Vector256 delta = _mm256_sub_pd(left, right);

		// 絶対値の差分の取得
		Vector256 result = _mm256_setzero_pd();
		result = _mm256_sub_pd(result, delta);
		result = _mm256_max_pd(result, delta);
		result = _mm256_cmp_pd(result, epsilon, _CMP_LE_OQ);

		// zとwは無視して, xとyに着目する
		return ((_mm256_movemask_pd(result) & 0b0111) == 0b0111);
	}

	/****************************************************************************
	*                       NearEqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector4(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
	*
	*  @brief     一定範囲内で値が等しいかを調べる (left - epsilon <= right <= left + epsilon)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*  @param[in] ConstVector256 epsilon
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::NearEqualVector4(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
	{
		// 差分の導出
		Vector256 delta = _mm256_sub_pd(left, right);

		// 絶対値の差分の取得
		Vector256 result = _mm256_setzero_pd();
		result = _mm256_sub_pd(result, delta);
		result = _mm256_max_pd(result, delta);
		result = _mm256_cmp_pd(result, epsilon, _CMP_LE_OQ);

		// zとwは無視して, xとyに着目する
		return ((_mm256_movemask_pd(result) & 0b1111) == 0b1111);
	}

	/****************************************************************************
	*                       NearEqualVectorEach
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVectorEach(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
	*
	*  @brief     一定範囲内で値が等しいかを調べる (left - epsilon <= right <= left + epsilon)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*  @param[in] ConstVector256 epsilon
	*
	*  @return 　　bool
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NearEqualVectorEach(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
	{
		// 差分
		Vector256 delta = _mm256_sub_pd(left, right);

		// 絶対値の取得
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
	*  @brief     Vector2型において左の方が全ての要素において大きいかを調べます (left > right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterVector2(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft > rightの場合は1を返すベクトル
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_GT_OQ); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		// w, zは無視するようにする
		return (_mm256_movemask_pd(equalMaskVector) & 0b0011) == 0b0011; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       GreaterVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterVector3(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector3型において左の方が全ての要素において大きいかを調べます (left > right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterVector3(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft > rightの場合は1を返すベクトル
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_GT_OQ); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		// wは無視するようにする
		return (_mm256_movemask_pd(equalMaskVector) & 0b0111) == 0b0111; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       GreaterVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterVector4(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector4型において左の方が全ての要素において大きいかを調べます (left > right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterVector4(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft > right場合は1を返すベクトル
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_GT_OQ); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		return _mm256_movemask_pd(equalMaskVector) == 0b1111; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       GreaterVectorEach
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterVector4(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector4型において左の方が全ての要素において大きいかを調べます (left > right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　Vector256
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
	*  @brief     Vector2型において左の方が全ての要素において大きいor等しいかを調べます (left >= right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVector2(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft >= right場合は1を返すベクトル
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_GE_OQ); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		// w, zは無視するようにする
		return (_mm256_movemask_pd(equalMaskVector) & 0b0011) == 0b0011; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       GreaterVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVector3(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector3型において左の方が全ての要素において大きいor等しいかを調べます (left >= right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVector3(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft >= rightの場合は1を返すベクトル
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_GE_OQ); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		// wは無視するようにする
		return (_mm256_movemask_pd(equalMaskVector) & 0b0111) == 0b0111; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       GreaterOrEqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector4型において左の方が全ての要素において大きいor等しいかを調べます (left >= right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft >= rightの場合は1を返すベクトル
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_GE_OQ); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		return _mm256_movemask_pd(equalMaskVector) == 0b1111; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       GreaterOrEqualVectorEach
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector4型において左の方が全ての要素において大きいor等しいかを調べます (left >= right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　Vector256
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
	*  @brief     Vector2型において左の方が全ての要素において小さいかを調べます (left < right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::LessVector2(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft < rightの場合は1を返すベクトル
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_LT_OQ); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		// w, zは無視するようにする
		return (_mm256_movemask_pd(equalMaskVector) & 0b0011) == 0b0011; // 全正解で0b1111というビットフラグが成立

	}

	/****************************************************************************
	*                       LessVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessVector3(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector3型において左の方が全ての要素において小さいかを調べます (left < right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::LessVector3(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft < rightの場合は1を返すベクトル
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_LT_OQ); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		// wは無視するようにする
		return (_mm256_movemask_pd(equalMaskVector) & 0b0111) == 0b0111; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       LessVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessVector4(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector4型において左の方が全ての要素において小さいかを調べます (left < right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::LessVector4(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft < rightの場合は1を返すベクトル
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_LT_OQ); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		return _mm256_movemask_pd(equalMaskVector) == 0b1111; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       LessVectorEach
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LessVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector4型において左の方が全ての要素において小さいかを調べます (left < right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
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
	*  @brief     Vector2型において左の方が全ての要素において小さいor等しいかを調べます (left <= right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVector2(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft <= rightの場合は1を返すベクトル
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_LE_OQ); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		// w, zは無視するようにする
		return (_mm256_movemask_pd(equalMaskVector) & 0b0011) == 0b0011; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       LessOrEqualVector3
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVector3(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector3型において左の方が全ての要素において小さいor等しいかを調べます (left <= right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVector3(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft <= rightの場合は1を返すベクトル
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_LE_OQ); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		// wは無視するようにする
		return (_mm256_movemask_pd(equalMaskVector) & 0b0111) == 0b0111; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       LessOrEqualVector4
	****************************************************************************/
	/* @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector4型において左の方が全ての要素において小さいor等しいかを調べます (left <= right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
	{
		// w, z, y, xそれぞれに対してleft <= rightの場合は1を返すベクトル
		Vector256 equalMaskVector = _mm256_cmp_pd(left, right, _CMP_LE_OQ); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		return _mm256_movemask_pd(equalMaskVector) == 0b0011; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       LessVectorEach
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     Vector4型において左の方が全ての要素において小さいor等しいかを調べます (left <= right)
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　bool
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
	*  @brief     全ての要素を調べて指定範囲内にあるかどうか -bounds <= vector <= +bounds
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] ConstVector256 bounds 
	*
	*  @return 　　bool
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
	*  @brief     全ての要素を調べて指定範囲内にあるかどうか -bounds <= vector <= +bounds
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] ConstVector256 bounds
	*
	*  @return 　　bool
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
	*  @brief     全ての要素を調べて指定範囲内にあるかどうか -bounds <= vector <= +bounds
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] ConstVector256 bounds
	*
	*  @return 　　bool
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
	*  @brief     指定範囲内にあるかどうかを各要素で比較します -bounds <= vector <= +bounds
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] ConstVector256 bounds
	*
	*  @return 　　bool
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
	*  @brief      各要素にマイナスを取ったものを返す.
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
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
	*  @brief     2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　double
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
	*  @brief     2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　double
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
	*  @brief     2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　double
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
	*  @brief     2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　double
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
	*  @brief     2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　double
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
	*  @brief     2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　double
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
	*  @brief     内積を算出
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::DotVector2(ConstVector256 left, ConstVector256 right) noexcept
	{
		// 要素ごとの積を計算 
		Vector256 multiply = _mm256_mul_pd(left, right);

		// z以降の計算は考慮しないため, yの結果を格納
		Vector256 y = _mm256_permute4x64_pd(multiply, _MM_SHUFFLE(1, 1, 1, 1));

		// x成分のみの結果に着目する.  
		y.m256d_f64[0] = multiply.m256d_f64[0] + y.m256d_f64[0];
		return _mm256_cvtsd_f64(y);
	}

	/****************************************************************************
	*                      DotVector3
	****************************************************************************/
	/* @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::DotVector3(ConstVector256 left, ConstVector256 right)
	*
	*  @brief     内積を算出
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::DotVector3(ConstVector256 left, ConstVector256 right) noexcept
	{
		// 要素ごとの積を計算 
		Vector256 multiply = _mm256_mul_pd(left, right);

		// x=Dot.D64[1], y=Dot.D64[2]
		Vector256 temp = _mm256_permute4x64_pd(multiply, _MM_SHUFFLE(2, 1, 2, 1));

		// result.D64[0] = x + y 256 bitに対応する_mm_add_ssがないため
		multiply.m256d_f64[0] = multiply.m256d_f64[0] + temp.m256d_f64[0];

		// x=multiply.D64[2]
		temp = _mm256_permute4x64_pd(temp, _MM_SHUFFLE(1, 1, 1, 1));

		// Result.F32[0] = (x+y)+z
		temp.m256d_f64[0] = multiply.m256d_f64[0] + temp.m256d_f64[0];

		// x成分のみの結果に着目する.  
		return _mm256_cvtsd_f64(_mm256_permute4x64_pd(temp, _MM_SHUFFLE(0, 0, 0, 0)));
	}

	/****************************************************************************
	*                      DotVector4
	****************************************************************************/
	/* @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::DotVector4(ConstVector256 left, ConstVector256 right)
	*
	*  @brief     内積を算出
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　double
	*****************************************************************************/
	inline double SIMD_CALL_CONVENTION Vector256Utility::DotVector4(ConstVector256 left, ConstVector256 right) noexcept
	{
		// 要素ごとの積を計算 
		Vector256 temp2 = right;
		Vector256 temp = _mm256_mul_pd(left, temp2);

		temp2 = _mm256_permute4x64_pd(temp, _MM_SHUFFLE(3, 2, 3, 2));

		temp2 = _mm256_add_pd(temp2, temp);

		temp = _mm256_permute4x64_pd(temp2, _MM_SHUFFLE(1, 1, 1, 1));

		temp = _mm256_add_pd(temp, temp2);

		// x成分のみの結果に着目する.  
		return _mm256_cvtsd_f64(_mm256_permute4x64_pd(temp, _MM_SHUFFLE(0, 0, 0, 0)));
	}

	/****************************************************************************
	*                      CrossVector2
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CrossVector2(ConstVector256 left, ConstVector256 right)
	*
	*  @brief     平行四辺形の面積を算出する
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　Vector256ではあるが, 実質double
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CrossVector2(ConstVector256 left, ConstVector256 right) noexcept
	{
		// [ left.x*right.y - left.y*right.x, left.x*right.y - left.y*right.x ]
		// rightのx, yを入れ替える (w,zは計算上使用しない)
		Vector256 result = _mm256_permute4x64_pd(right, _MM_SHUFFLE(0, 1, 0, 1));

		// leftと入れ替え後のrightの各要素を乗算する
		result = _mm256_mul_pd(result, left);

		// yを抽出する
		Vector256 y = _mm256_permute4x64_pd(result, _MM_SHUFFLE(1, 1, 1, 1));

		// 値を減算する (ssに対応する関数がない)
		result.m256d_f64[0] = result.m256d_f64[0] - y.m256d_f64[0];

		// x成分のみを取り出して結果とする
		return _mm256_permute4x64_pd(result, _MM_SHUFFLE(0, 0, 0, 0));
	}

	/****************************************************************************
	*                      CrossVector3
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CrossVector3(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     外積を算出 left, rightに垂直なベクトルを返す
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CrossVector3(ConstVector256 left, ConstVector256 right) noexcept
	{
		// [ V1.y*V2.z - V1.z*V2.y, V1.z*V2.x - V1.x*V2.z, V1.x*V2.y - V1.y*V2.x ]
		
		// leftの要素を(y1, z1, x1, w1)に順に並べ替える
		Vector256 temp1 = _mm256_permute4x64_pd(left, _MM_SHUFFLE(3, 0, 2, 1)); 

		// rightの要素を(z2, x2. y2, w2)の順に並べ替える
		Vector256 temp2 = _mm256_permute4x64_pd(right, _MM_SHUFFLE(3, 1, 0, 2));

		// 一時ベクトルの要素ごとの乗算
		Vector256 result = _mm256_mul_pd(temp1, temp2);

		// z1, x1, y1, w1の順にtemp1を並べ替える
		temp1 = _mm256_permute4x64_pd(temp1, _MM_SHUFFLE(3, 0, 2, 1));
		
		// y2, z2, x2, w2の順にtemp2を並べ替える
		temp2 = _mm256_permute4x64_pd(temp2, _MM_SHUFFLE(3, 1, 0, 2));

		result = NegativeMultiplySubtract(temp1, temp2, result);

		// wの要素をゼロに設定
		return _mm256_and_pd(result, VECTOR_256U_MASK_XYZ);
	}

	/****************************************************************************
	*                      CrossVector4
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CrossVector4(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     外積を算出 left, rightに垂直なベクトルを返す
	*
	*  @param[in] ConstVector256 first
	*  @param[in] ConstVector256 second
	*  @param[in] ConstVector256 third
	*
	*  @return 　　Vector256
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
	*  @brief     入力ベクトルの単位ベクトルを返します
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NormalizeVector2(ConstVector256 vector) noexcept
	{
		// 1次norm計算
		Vector256 squareLength = _mm256_mul_pd(vector, vector);
		Vector256 temp = _mm256_permute4x64_pd(squareLength, _MM_SHUFFLE(1, 1, 1, 1));
		squareLength   = _mm256_add_pd(squareLength, temp);
		squareLength   = _mm256_permute4x64_pd(squareLength, _MM_SHUFFLE(0, 0, 0, 0));
		Vector256 norm = _mm256_sqrt_pd(squareLength);

		// 0で割られる可能性があるかどうかを調べる. 値を持っていたら1
		Vector256 zeroMask = _mm256_setzero_pd();
		zeroMask = _mm256_cmp_pd(zeroMask, norm, _CMP_NEQ_OQ);

		// 無限大でない場合は対応する要素が1になるマスクの作成
		Vector256 infinityMask = _mm256_cmp_pd(squareLength, VECTOR_256D_INFINITY, _CMP_NEQ_OQ);

		// 正規化の実行
		Vector256 result = _mm256_div_pd(vector, norm);

		// 0付近で割られて無限大になったものに対しては0に設定
		result = _mm256_and_pd(result, zeroMask);

		// 無限大かに基づいてqnanまたはresultを選択する
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
	*  @brief     入力ベクトルの単位ベクトルを返します
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NormalizeVector3(ConstVector256 vector) noexcept
	{
		// x, y, zに対するドット積の計算
		// x*x, y*y, z*z, w*w
		Vector256 squareLength = _mm256_mul_pd(vector, vector);

		// z*z, y*y, z*z, y*y
		Vector256 temp         = _mm256_permute4x64_pd(squareLength, _MM_SHUFFLE(2, 1, 2, 1)); 

		// x*x + z*z, y*y, z*z, w*w
		squareLength .m256d_f64[0] = squareLength.m256d_f64[0] + temp.m256d_f64[0];

		// 全てy*y
		temp = _mm256_permute4x64_pd(temp, _MM_SHUFFLE(1, 1, 1, 1));

		// 1要素目がx*x + y*y + z*z
		squareLength.m256d_f64[0] = squareLength.m256d_f64[0] +  temp.m256d_f64[0];
		// 全ての要素がドット積
		squareLength = _mm256_permute4x64_pd(squareLength, _MM_SHUFFLE(0, 0, 0, 0));

		// 1次ノルムの計算
		Vector256 norm = _mm256_sqrt_pd(squareLength);

		// 0で割られる可能性があるかどうかを調べる. 値を持っていたら1
		Vector256 zeroMask = _mm256_setzero_pd();
		zeroMask = _mm256_cmp_pd(zeroMask, norm, _CMP_NEQ_OQ);

		// 無限大でない場合は対応する要素が1になるマスクの作成
		Vector256 infinityMask = _mm256_cmp_pd(squareLength, VECTOR_256D_INFINITY, _CMP_NEQ_OQ);

		// 正規化の実行
		Vector256 result = _mm256_div_pd(vector, norm);

		// 0付近で割られて無限大になったものに対しては0に設定
		result = _mm256_and_pd(result, zeroMask);

		// 無限大かに基づいてqnanまたはresultを選択する
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
	*  @brief     入力ベクトルの単位ベクトルを返します
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NormalizeVector4(ConstVector256 vector) noexcept
	{
		// 全ての要素が4要素の二乗和
		Vector256 squareLength = _mm256_set1_pd(LengthSquaredVector4(vector));

		// 2次ノルムの計算
		Vector256 norm = _mm256_sqrt_pd(squareLength);

		// 0で割られる可能性があるかどうかを調べる. 値を持っていたら1
		Vector256 zeroMask = _mm256_setzero_pd();
		zeroMask = _mm256_cmp_pd(zeroMask, norm, _CMP_NEQ_OQ);

		// 無限大でない場合は対応する要素が1になるマスクの作成
		Vector256 infinityMask = _mm256_cmp_pd(squareLength, VECTOR_256D_INFINITY, _CMP_NEQ_OQ);

		// 正規化の実行
		Vector256 result = _mm256_div_pd(vector, norm);

		// 0付近で割られて無限大になったものに対しては0に設定
		result = _mm256_and_pd(result, zeroMask);

		// 無限大かに基づいてqnanまたはresultを選択する
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
	*  @brief     反射ベクトルを算出 (input - (2 * dot(input, normal)) * normal)
	*
	*  @param[in] ConstVector256 : input  (入射ベクトル)
	*  @param[in] ConstVector256 : normal (反射を行う法線ベクトル)
	*
	*  @return 　　Vector256
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
	*  @brief     反射ベクトルを算出 (input - (2 * dot(input, normal)) * normal)
	*
	*  @param[in] ConstVector256 : input  (入射ベクトル)
	*  @param[in] ConstVector256 : normal (反射を行う法線ベクトル)
	*
	*  @return 　　Vector256
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
	*  @brief     反射ベクトルを算出 (input - (2 * dot(input, normal)) * normal)
	*
	*  @param[in] ConstVector256 : input  (入射ベクトル)
	*  @param[in] ConstVector256 : normal (反射を行う法線ベクトル)
	*
	*  @return 　　Vector256
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
	*  @brief     屈折ベクトルを算出
	*
	*  @param[in] ConstVector256 : input  (入射ベクトル)
	*  @param[in] ConstVector256 : normal (屈折を行う法線ベクトル)
	*  @param[in]  const double   : refractionIndex (屈折率)
	*
	*  @return 　　Vector256
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
	*  @brief     屈折ベクトルを算出
	*
	*  @param[in] ConstVector256 : input  (入射ベクトル)
	*  @param[in] ConstVector256 : normal (屈折を行う法線ベクトル)
	*  @param[in]  const double   : refractionIndex (屈折率)
	*
	*  @return 　　Vector256
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
	*  @brief     屈折ベクトルを算出
	*
	*  @param[in] ConstVector256 : input  (入射ベクトル)
	*  @param[in] ConstVector256 : normal (屈折を行う法線ベクトル)
	*  @param[in]  const double   : refractionIndex (屈折率)
	*
	*  @return 　　Vector256
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
	*  @brief     屈折ベクトルを算出 (異方性がある場合)
	*
	*  @param[in] ConstVector256 : input  (入射ベクトル)
	*  @param[in] ConstVector256 : normal (屈折を行う法線ベクトル)
	*  @param[in]  const double   : refractionIndex (屈折率)
	*
	*  @return 　　Vector256
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

		// 結果が0以下であれば, sqrtは失敗ということでzeroに置き換える
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
	*  @brief     屈折ベクトルを算出 (異方性がある場合)
	*
	*  @param[in] ConstVector256 : input  (入射ベクトル)
	*  @param[in] ConstVector256 : normal (屈折を行う法線ベクトル)
	*  @param[in]  const double   : refractionIndex (屈折率)
	*
	*  @return 　　Vector256
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
	*  @brief     屈折ベクトルを算出 (異方性がある場合)
	*
	*  @param[in] ConstVector256 : input  (入射ベクトル)
	*  @param[in] ConstVector256 : normal (屈折を行う法線ベクトル)
	*  @param[in]  const double   : refractionIndex (屈折率)
	*
	*  @return 　　Vector256
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
	*  @brief     非常に大きい値か
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline bool Vector256Utility::IsInfiniteVector2(ConstVector256 vector) noexcept
	{
		// 符号ビットをマスクオフする
		Vector256 temp = _mm256_and_pd(vector, VECTOR_256U_MASK_ABS);
		// 無限大と比較
		temp = _mm256_cmp_pd(temp, VECTOR_256D_INFINITY, _CMP_EQ_OQ);
		// どれかが無限大であれば、符号は真である。
		return (_mm256_movemask_pd(temp) & 0b0011) != 0;
	}

	/****************************************************************************
	*                      IsInfiniteVector3
	****************************************************************************/
	/* @fn        inline bool Vector256Utility::IsInfiniteVector3(ConstVector256 vector) noexcept
	*
	*  @brief     非常に大きい値か
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline bool Vector256Utility::IsInfiniteVector3(ConstVector256 vector) noexcept
	{
		// 符号ビットをマスクオフする
		Vector256 temp = _mm256_and_pd(vector, VECTOR_256U_MASK_ABS);
		// 無限大と比較
		temp = _mm256_cmp_pd(temp, VECTOR_256D_INFINITY, _CMP_EQ_OQ);
		// どれかが無限大であれば、符号は真である。
		return (_mm256_movemask_pd(temp) & 0b0111) != 0;
	}

	/****************************************************************************
	*                      IsInfiniteVector4
	****************************************************************************/
	/* @fn        inline bool Vector256Utility::IsInfiniteVector4(ConstVector256 vector) noexcept
	*
	*  @brief     非常に大きい値か
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline bool Vector256Utility::IsInfiniteVector4(ConstVector256 vector) noexcept
	{
		// 符号ビットをマスクオフする
		Vector256 temp = _mm256_and_pd(vector, VECTOR_256U_MASK_ABS);
		// 無限大と比較
		temp = _mm256_cmp_pd(temp, VECTOR_256D_INFINITY, _CMP_EQ_OQ);
		// どれかが無限大であれば、符号は真である。
		return (_mm256_movemask_pd(temp) != 0);
	}

	/****************************************************************************
	*                      Sqrt
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Sqrt(ConstVector256 vector) noexcept
	*
	*  @brief      全ての要素に根号を取ったものを返します
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
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
	*  @brief       全ての要素の逆数を取り, それに根号を取ったものを返します
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
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
	*  @brief     全ての要素の2^{vector}を返します
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
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
	*  @brief      全ての要素の10^{vector}を返します
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
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
	*  @brief     全ての要素のe^{vector}を返します
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
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
	*  @brief     全ての要素のlog2{vector}を返します
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
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
	*  @brief     全ての要素のlog10{vector}を返します
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
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
	*  @brief     全ての要素のloge{vector}を返します
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
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
	*  @brief     全ての要素のPowを返します  base^{power}
	*
	*  @param[in] ConstVector256 : base
	*  @param[in] ConstVector256 : power
	*
	*  @return 　　Vector256
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
	*  @brief     全ての要素にSinを取ったものを返します[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
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
	*  @brief     全ての要素にCosを取ったものを返します
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
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
	*  @brief     全ての要素にTanを取ったものを返します
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
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
	*  @brief     全ての要素に対するsin成分とcos成分を取り出します 
	*
	*  @param[in] ConstVector256 : input
	*  @param[out] Vector256* sin
	*  @param[out] Vector256* cos
	*
	*  @return 　　void
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
	*  @brief     全ての要素にSinHを取ったものを返します[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SinH(ConstVector256 vector) noexcept
	{
		#if PLATFORM_CPU_INSTRUCTION_SVML
			return _mm256_sinh_pd(vector);
		#else
			// 自然対数の底を2からeに変換
			static const Vector256 scale = { {  1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f } } ; // 1.0f / ln(2.0f)

			// sinh(x) = (e^x + e^-x) / 2 - 1として計算
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
	*  @brief     全ての要素にCosHを取ったものを返します[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
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
	*  @brief     全ての要素にTanHを取ったものを返します[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
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
	*  @brief     全ての要素にArcSinを取ったものを返します[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
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
	*  @brief     全ての要素にArcSinを取ったものを返します[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
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
	*  @brief     全ての要素にArcSinを取ったものを返します[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
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
	*  @brief     線形補間を返します
	*
	*  @param[in] ConstVector256  t = 0の時の値
	*  @param[in] ConstVector256  t = 1の時の値
	*  @param[in] double  t : 線形補間の割合
	*
	*  @return 　　Vector
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
	*  @brief     線形補間を返します
	*
	*  @param[in] ConstVector256  t = 0の時の値
	*  @param[in] ConstVector256  t = 1の時の値
	*  @param[in] ConstVector256  t : 線形補間の割合
	*
	*  @return 　　Vector
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
	*  @brief     エルミート補間を返します
	*
	*  @param[in]  始点の位置
	   @param[in]  始点におけるスプラインの接線を表すベクトル
	   @param[in]  終点の位置
	   @param[in]  終点におけるスプラインの接線を表すベクトル
	   @param[in]  t : エルミート補間の割合
	*
	*  @return 　　Vector256
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
	*  @brief     エルミート補間を返します
	*
	*  @param[in]  始点の位置
	   @param[in]  始点におけるスプラインの接線を表すベクトル
	   @param[in]  終点の位置
	   @param[in]  終点におけるスプラインの接線を表すベクトル
	   @param[in]  t : エルミート補間の割合
	*
	*  @return 　　Vector256
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
	*  @brief     CatMull-Romスプライン補間の結果を返します。4つの制御点を全て通るスプライン補間
	*
	*  @param[in]  制御点1
	   @param[in]  制御点2
	   @param[in]  制御点3
	   @param[in]  制御点4
	   @param[in]  t : 補間の割合
	*
	*  @return 　　Vector256
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
	*  @brief     CatMull-Romスプライン補間の結果を返します。4つの制御点を全て通るスプライン補間
	*
	*  @param[in]  制御点1
	   @param[in]  制御点2
	   @param[in]  制御点3
	   @param[in]  制御点4
	   @param[in]  t : 補間の割合
	*
	*  @return 　　Vector256
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
	*  @brief     BaryCentric 三角形の重心重み位置を算出
	*              (f>=0 && g>=0 && 1-f-g>=0) の場合、ポイントは三角形の Position0>Position1>Position2 の内側にあります>。
				   (f==0 && g>=0 && 1-f-g>=0) の場合、ポイントは Position0>Position2 行目にあります>。
				   (f>=0 && g==0 && 1-f-g>=0) の場合、ポイントは Position0>Position1> 行目にあります。
				   (f>=0 && g>=0 && 1-f-g==0) の場合、ポイントは Position1>Position2 行目にあります>。
	*
	*  @return 　　Vector256
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
	*  @brief     BaryCentric 三角形の重心重み位置を算出
	*              (f>=0 && g>=0 && 1-f-g>=0) の場合、ポイントは三角形の Position0>Position1>Position2 の内側にあります>。
				   (f==0 && g>=0 && 1-f-g>=0) の場合、ポイントは Position0>Position2 行目にあります>。
				   (f>=0 && g==0 && 1-f-g>=0) の場合、ポイントは Position0>Position1> 行目にあります。
				   (f>=0 && g>=0 && 1-f-g==0) の場合、ポイントは Position1>Position2 行目にあります>。
	*
	*  @return 　　Vector256
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
	*  @brief     全ての要素について最小値となる方を選択します
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　Vector
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
	*  @brief     全ての要素について最大値となる方を選択します
	*
	*  @param[in] ConstVector256 left
	*  @param[in] ConstVector256 right
	*
	*  @return 　　Vector256
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
	*  @brief     最も近い整数に丸める
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
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
	*  @brief     切り捨て (指定した数値の端数を切り捨てた整数に丸める。数直線上で0の方向にある次の整数
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
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
	*  @brief     負の方向に丸める
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
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
	*  @brief     切り上げ 数直線上で正の方向にある次の整数に丸める
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
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
	*  @brief     指定範囲内で数値を設定する
	*
	*  @param[in] ConstVector256 vector
	*  @param[in] ConstVector256 min
	*  @param[in] ConstVector256 max
	*
	*  @return 　　Vector
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
	*  @brief     1以上には行かないようにする
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector
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
	*  @brief     絶対値
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector
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
	*  @brief      - pi <= theta <= piの範囲に収めるようにします.
	*
	*  @param[in] ConstVector256 angles : 角度rad
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ModAngles(ConstVector256 angles) noexcept
	{
		// PI <= angle < PI となるように、与えられた角度の範囲のモジュロ演算を行う
		Vector256 result = _mm256_mul_pd(angles, VECTOR_256D_RECIPROCAL_2PI);

		// 丸め処理が複雑なため、インライン関数を使用する。
		result = Round(result);
		return NegativeMultiplySubtract(result, VECTOR_256D_2PI, angles);
	}

	/****************************************************************************
	*                       AddAngles
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::AddAngles(ConstVector256 left, ConstVector256 right) noexcept
	*
	*  @brief     - pi <= theta <= piの範囲に収めるようにした上で足し算を行います.
	*
	*  @param[in] ConstVector256 left angles : 角度rad
	*  @param[in] ConstVector256 right angles : 角度rad
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::AddAngles(ConstVector256 left, ConstVector256 right) noexcept
	{
		// 角度の純粋な差分
		Vector256 result = _mm256_add_pd(left, right);

		// -Piよりも小さいかを判定する
		Vector256 offset = _mm256_cmp_pd(result, VECTOR_256D_NEGATIVE_PI, _CMP_LT_OQ);
		offset = _mm256_and_pd(offset, VECTOR_256D_2PI);

		// -Piよりも全ての要素で小さい場合, 2Piを加える
		result = _mm256_add_pd(result, offset);

		// Piよりも大きくなってしまっているか
		offset = _mm256_cmp_pd(result, VECTOR_256D_PI, _CMP_GE_OQ);
		offset = _mm256_and_pd(offset, VECTOR_256D_2PI);

		// piよりも大きい場合は2piを全ての要素で引く
		result = _mm256_sub_pd(result, offset);
		return result;
	}

	/****************************************************************************
	*                       SubtractAngles
	****************************************************************************/
	/* @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ModAngles(ConstVector256 vector)
	*
	*  @brief     - pi <= theta <= piの範囲に収めるようにした上で引き算を行います.
	*
	*  @param[in] ConstVector256 left angles : 角度rad
	*  @param[in] ConstVector256 right angles : 角度rad
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SubtractAngles(ConstVector256 left, ConstVector256 right) noexcept
	{
		// 角度の純粋な差分
		Vector256 result = _mm256_sub_pd(left, right);

		// -Piよりも小さいかを判定する
		Vector256 offset = _mm256_cmp_pd(result, VECTOR_256D_NEGATIVE_PI, _CMP_LT_OQ);
		offset           = _mm256_and_pd(offset, VECTOR_256D_2PI);

		// -Piよりも全ての要素で小さい場合, 2Piを加える
		result = _mm256_add_pd(result, offset);

		// Piよりも大きくなってしまっているか
		offset = _mm256_cmp_pd(result, VECTOR_256D_PI, _CMP_GE_OQ);
		offset = _mm256_and_pd(offset, VECTOR_256D_2PI);

		// piよりも大きい場合は2piを全ての要素で引く
		result = _mm256_sub_pd(result, offset);
		return result;
	}
	#pragma endregion Angle
#pragma endregion Implement
}


#endif
#endif