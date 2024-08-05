//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdVector256SSE.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/11 2:30:00
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_VECTOR_256_SSE_HPP
#define GM_SIMD_VECTOR_256_SSE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMSimdMacros.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if !PLATFORM_CPU_INSTRUCTION_SSE && !defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
#include "GameUtility/Base/Include/GUAssert.hpp"
#include <intrin.h>
#include <xmmintrin.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm::simd::sse
{
	/*----------------------------------------------------------------------
	*           256 bitの整数型もdouble型も格納可能なベクトルを作成する
	*----------------------------------------------------------------------*/
	ALIGNED_STRUCT(16) Vector256
	{
		union
		{
			double D[4];
			struct 
			{
				__m128d XY;
				__m128d ZW;
			};
		};

		inline operator const double* () const noexcept { return D; }
	};

	GLOBAL_CONST Vector256 VECTOR_256D_ONE            = { {  1.0, 1.0, 1.0, 1.0  } };
	GLOBAL_CONST Vector256 VECTOR_256D_ZERO           = { {  0.0, 0.0, 0.0, 0.0  } };
	GLOBAL_CONST Vector256 VECTOR_256U_MASK_ABS = { { {  ~gu::uint64(0), ~gu::uint64(0), ~gu::uint64(0), gu::uint64(0) } } };
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
		__forceinline static Vector256 SIMD_CALL_CONVENTION SetInt(const gu::uint32 x, const gu::uint32 y, const gu::uint32 z, const gu::uint32 w) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : Doubleを使ってVectorのX(, Y, Z, W)要素に値を代入する
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION SetX(ConstVector256 vector, const double x) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION SetY(ConstVector256 vector, const double y) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION SetZ(ConstVector256 vector, const double z) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION SetW(ConstVector256 vector, const double w) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ベクトルを使ってdouble配列に代入する
		*----------------------------------------------------------------------*/
		__forceinline static void SIMD_CALL_CONVENTION StoreDouble(double* destination, ConstVector256 source) noexcept;
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
			const gu::uint32 xIndex, const gu::uint32 yIndex,
			const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルから要素を抽出し, 新しいベクトルを作成する
		*           0～3 : aからのx, y, z, w
		*           4～7 : bからのx, y, z, w
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION Permute(ConstVector256 a, ConstVector256,
			const gu::uint32 xIndex, const gu::uint32 yIndex,
			const gu::uint32 zIndex, const gu::uint32 wIndex) noexcept;

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
		__forceinline static Vector256 SIMD_CALL_CONVENTION LoadDouble(const double* source) noexcept;
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
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector2(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector3(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector4(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION NearEqualVectorEach(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 左の方が全ての要素において大きいかを調べます (left > right)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterVector4(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION GreaterVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 左の方が全ての要素において大きいor等しいかを調べます (left >= right)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterOrEqualVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterOrEqualVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION GreaterOrEqualVector4(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION GreaterOrEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 左の方が全ての要素において小さいかを調べます (left < right)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION LessVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessVector4(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION LessVectorEach(ConstVector256 left, ConstVector256 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 左の方が全ての要素において小さいor等しいかを調べます (left <= right)
		*----------------------------------------------------------------------*/
		__forceinline static bool      SIMD_CALL_CONVENTION LessOrEqualVector2(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessOrEqualVector3(ConstVector256 left, ConstVector256 right) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION LessOrEqualVector4(ConstVector256 left, ConstVector256 right) noexcept;
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
		*  @brief : 2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
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
		__forceinline static Vector256 SIMD_CALL_CONVENTION CatMullRom(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, ConstVector256 position3, const double t) noexcept;
		__forceinline static Vector256 SIMD_CALL_CONVENTION CatMullRomV(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, ConstVector256 position3, ConstVector256 t) noexcept;

		/*----------------------------------------------------------------------
		*  @brief      三角形の重心重み位置を算出
		*              (f>=0 && g>=0 && 1-f-g>=0) の場合、ポイントは三角形の Position0>Position1>Position2 の内側にあります>。
					   (f==0 && g>=0 && 1-f-g>=0) の場合、ポイントは Position0>Position2 行目にあります>。
					   (f>=0 && g==0 && 1-f-g>=0) の場合、ポイントは Position0>Position1> 行目にあります。
					   (f>=0 && g>=0 && 1-f-g==0) の場合、ポイントは Position1>Position2 行目にあります>。
		*----------------------------------------------------------------------*/
		__forceinline static Vector256 SIMD_CALL_CONVENTION BaryCentric(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, const double f, const double g) noexcept;
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Zero()noexcept
	*
	*  @brief     ゼロ初期化したVector256を返す関数
	*
	*  @param[in] void
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Zero() noexcept
	{
		Vector256 result = {};
		result.XY = _mm_setzero_pd();
		result.ZW = _mm_setzero_pd();
		return result; // 4つの単精度浮動小数点値をクリアする
	}

	/****************************************************************************
	*                       Set
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Set(const double x, const double y, const double z, const double w) noexcept
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
		Vector256 result = {};
		result.XY = _mm_setr_pd(x, y);
		result.ZW = _mm_setr_pd(z, w);
		return result;
	}

	/****************************************************************************
	*                       Set
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Set(const double value) noexcept
	*
	*  @brief     double1つからVector256クラスを生成する
	*
	*  @param[in] const double value
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Set(const double value) noexcept
	{
		Vector256 result = {};
		result.XY = _mm_set1_pd(value);
		result.ZW = result.XY;
		return result;
	}

	/****************************************************************************
	*                       StoreDouble
	****************************************************************************/
	*  @fn        inline void SIMD_CALL_CONVENTION Vector256Utility::StoreDouble(double* destination, ConstVector256 source) noexcept
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
		_mm_store_sd(destination, source.XY);
	}

	/****************************************************************************
	*                       StoreDouble2
	****************************************************************************/
	*  @fn        inline void SIMD_CALL_CONVENTION Vector256Utility::StoreDouble2(double* destination, ConstVector256 source) noexcept
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
		_mm_storeu_pd(destination, source.XY);
	}

	/****************************************************************************
	*                       StoreDouble3
	****************************************************************************/
	*  @fn        inline void SIMD_CALL_CONVENTION Vector256Utility::StoreDouble3(double* destination, ConstVector256 source) noexcept
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
		_mm_storeu_pd(destination, source.XY);
		_mm_store_sd(destination + 2, source.ZW);
	}

	/****************************************************************************
	*                       StoreDouble4
	****************************************************************************/
	*  @fn        inline void SIMD_CALL_CONVENTION Vector256Utility::StoreDouble4(double* destination, ConstVector256 source) noexcept
	*
	*  @brief     Vector256をもとにDouble配列に代入します
	*
	*  @param[in] double* destination
	*  @param[in] ConstVector256 source
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline void SIMD_CALL_CONVENTION Vector256Utility::StoreDouble4(double* destination, ConstVector256 source) noexcept
	{
		Check(destination);
		_mm_storeu_pd(destination    , source.XY);
		_mm_storeu_pd(destination + 2, source.ZW);
	}
	#pragma endregion Setter

	#pragma region Getter
	/****************************************************************************
	*                       LoadDouble
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble2(const double* source) noexcept
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

		Vector256 result = {};
		result.XY = _mm_load1_pd(source);
		result.ZW = result.XY;
		return result;
	}

	/****************************************************************************
	*                       LoadDouble2
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble2(const double* source) noexcept
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

		Vector256 result = {};
		result.XY = _mm_loadu_pd(source);
		result.ZW = result.XY;
		return result;
	}

	/****************************************************************************
	*                       LoadDouble3
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble3(const double* source) noexcept
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

		Vector256 result = {};
		result.XY = _mm_loadu_pd((double*)(source));
		result.ZW = _mm_loadu_pd((double*)(source + 2));
		return result;
	}

	/****************************************************************************
	*                       LoadDouble4
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LoadDouble4(const double* source) noexcept
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

		Vector256 result = {};
		result.XY = _mm_loadu_pd((double*)(source));
		result.ZW = _mm_loadu_pd((double*)(source + 2));
		return result;
	}
	#pragma endregion Getter

	#pragma region Operator
	/****************************************************************************
	*                       Add
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Add(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 result = {};
		result.XY = _mm_add_pd(left.XY, right.XY);
		result.ZW = _mm_add_pd(left.ZW, right.ZW);
		return result;
	}

	/****************************************************************************
	*                       Subtract
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Subtract(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 result = {};
		result.XY = _mm_sub_pd(left.XY, right.XY);
		result.ZW = _mm_sub_pd(left.ZW, right.ZW);
		return result;
	}

	/****************************************************************************
	*                       Multiply
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Multiply(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 result = {};
		result.XY = _mm_mul_pd(left.XY, right.XY);
		result.ZW = _mm_mul_pd(left.ZW, right.ZW);
		return result;
	}

	/****************************************************************************
	*                       Divide
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Divide(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 result = {};
		result.XY = _mm_div_pd(left.XY, right.XY);
		result.ZW = _mm_div_pd(left.ZW, right.ZW);
		return result;
	}

	/****************************************************************************
	*                       Mod
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Mod(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 result = {};
		result.XY = _mm_div_pd(left.XY, right.XY);
		result.ZW = _mm_div_pd(left.ZW, right.ZW);

		result = Truncate(result);
		return NegativeMultiplySubtract(result, right, left); // left - result * right
	}

	/****************************************************************************
	*                      NegativeMultiplySubstract
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NegativeMultiplySubtract(ConstVector256 first, ConstVector256 second, ConstVector256 third)
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::MultiplyAdd(ConstVector256 first, ConstVector256 second, ConstVector256 third)
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Reciprocal(ConstVector256 vector) noexcept
	*
	*  @brief     2つのベクトルの各要素同士の逆数を算出します. (1.0f / vector)
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Reciprocal(ConstVector256 vector) noexcept
	{
		Vector256 result = {};
		result.XY = _mm_div_pd(VECTOR_256D_ONE.XY, vector.XY);
		result.ZW = _mm_div_pd(VECTOR_256D_ONE.ZW, vector.ZW);
		return result;
	}

	/****************************************************************************
	*                       Scale
	****************************************************************************/
	*  @fn       inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Scale(ConstVector256 vector, const double scale) noexcept
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
		Vector256 scaleV = {};
		scaleV.XY = _mm_set1_pd(scale);
		scaleV.ZW = scaleV.XY;

		Vector256 result = {};
		result.XY = _mm_mul_pd(vector.XY, scaleV.XY);
		result.ZW = _mm_mul_pd(vector.ZW, scaleV.ZW);
		return result;
	}

	/****************************************************************************
	*                       EqualVector2
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVector2(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 equalMaskVector = _mm_cmpeq_ps(left, right); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		// w, zは無視するようにする
		return (_mm_movemask_ps(equalMaskVector) & 0b0011) == 0b0011; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       EqualVector3
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVector3(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 equalMaskVector = _mm_cmpeq_ps(left, right); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		// wは無視するようにする
		return (_mm_movemask_ps(equalMaskVector) & 0b0111) == 0b0111; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       EqualVector4
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVector4(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 notEqualMaskVector = _mm_cmpeq_ps(left, right); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		return (_mm_movemask_ps(notEqualMaskVector) & 0b1111) == 0b1111; // 全正解で0b1111というビットフラグが成立
	}

	/****************************************************************************
	*                       EqualVectorEach
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 result = {};
		result.XY = _mm_cmpeq_pd(left.XY, right.XY);
		result.ZW = _mm_cmpeq_pd(left.ZW, right.ZW);
		return result;
	}

	/****************************************************************************
	*                       NotEqualVector2
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector2(ConstVector256 left, ConstVector256 right) noexcept
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
		// w, z, y, xそれぞれに対して等しい場合は1を返すベクトル
		Vector256 equalMaskVector = _mm_cmpeq_ps(left, right); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		// 一個でもフラグが立っていなかったら等しくない判定
		// ただしw, zは無視
		return (_mm_movemask_ps(equalMaskVector) & 0b0011) != 0b0011;
	}

	/****************************************************************************
	*                       NotEqualVector3
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
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
		// w, z, y, xそれぞれに対して等しい場合は1を返すベクトル
		Vector256 equalMaskVector = _mm_cmpeq_ps(left, right); // 全正解なら{1, 1, 1, 1}, wだけ異なるなら{0, 1, 1, 1}

		// 一個でもフラグが立っていなかったら等しくない判定
		// ただしwは無視
		return (_mm_movemask_ps(equalMaskVector) & 0b0111) != 0b0111;
	}

	/****************************************************************************
	*                       NotEqualVector4
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 equalMaskVector = _mm_cmpneq_ps(left, right); // 全不正解なら{1, 1, 1, 1}, wだけ正しいなら{0, 1, 1, 1}

		return (_mm_movemask_ps(equalMaskVector)) != 0;
	}

	/****************************************************************************
	*                       NotEqualVectorEach
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 result = {};
		result.XY = _mm_cmpneq_pd(left.XY, right.XY);
		result.ZW = _mm_cmpneq_pd(left.ZW, right.ZW);
		return result;
	}

	/****************************************************************************
	*                       EqualAsIntVector2
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualAsIntVector2(ConstVector256 left, ConstVector256 right) noexcept
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
		__m128i temp = _mm_cmpeq_epi32(_mm_castps_si128(left), _mm_castps_si128(right));
		return (_mm_movemask_ps(_mm_castsi128_ps(temp)) & 0b0011) == 0b0011;
	}

	/****************************************************************************
	*                       EqualAsIntVector3
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualAsIntVector3(ConstVector256 left, ConstVector256 right) noexcept
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
		__m128i temp = _mm_cmpeq_epi32(_mm_castps_si128(left), _mm_castps_si128(right));
		return (_mm_movemask_ps(_mm_castsi128_ps(temp)) & 0b0111) == 0b0111;
	}

	/****************************************************************************
	*                       EqualAsIntVector4
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualAsIntVector4(ConstVector256 left, ConstVector256 right) noexcept
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
		__m128i temp = _mm_cmpeq_epi32(_mm_castps_si128(left), _mm_castps_si128(right));
		return _mm_movemask_ps(_mm_castsi128_ps(temp)) == 0b1111;
	}

	/****************************************************************************
	*                       EqualAsIntVectorEach
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::EqualAsIntVectorEach(ConstVector256 left, ConstVector256 right) noexcept
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
		__m128i result = _mm_cmpeq_epi32(_mm_castps_si128(left), _mm_castps_si128(right));
		return _mm_castsi128_ps(result);
	}

	/****************************************************************************
	*                       NotEqualVector2
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualAsIntVector2(ConstVector256 left, ConstVector256 right) noexcept
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
		__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(left), _mm_castps_si128(right));
		return ((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 0b0011) != 0b0011);
	}

	/****************************************************************************
	*                       NotEqualVector3
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualAsIntVector3(ConstVector256 left, ConstVector256 right) noexcept
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
		__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(left), _mm_castps_si128(right));
		return ((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 0b0111) != 0b0111);
	}

	/****************************************************************************
	*                       NotEqualVector4
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
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
		__m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(left), _mm_castps_si128(right));
		return (_mm_movemask_ps(_mm_castsi128_ps(vTemp)) != 0b1111);
	}

	/****************************************************************************
	*                       NotEqualVector
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector(ConstVector256 left, ConstVector256 right) noexcept
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
		__m128i v = _mm_cmpeq_epi32(_mm_castps_si128(left), _mm_castps_si128(right));
		return _mm_xor_ps(_mm_castsi128_ps(v), VECTOR_128U_NEGATIVE_ONE_MASK);
	}

	/****************************************************************************
	*                       NearEqualVector2
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector4(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
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
		Vector256 delta = _mm_sub_ps(left, right);

		// 絶対値の差分の取得
		Vector256 result = _mm_setzero_ps();
		result = _mm_sub_ps(result, delta);
		result = _mm_max_ps(result, delta);
		result = _mm_cmple_ps(result, epsilon);

		// zとwは無視して, xとyに着目する
		return ((_mm_movemask_ps(result) & 0b0011) == 0b0011);
	}

	/****************************************************************************
	*                       NearEqualVector3
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector3(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
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
		Vector256 delta = _mm_sub_ps(left, right);

		// 絶対値の差分の取得
		Vector256 result = _mm_setzero_ps();
		result = _mm_sub_ps(result, delta);
		result = _mm_max_ps(result, delta);
		result = _mm_cmple_ps(result, epsilon);

		// zとwは無視して, xとyに着目する
		return ((_mm_movemask_ps(result) & 0b0111) == 0b0111);
	}

	/****************************************************************************
	*                       NearEqualVector4
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVector4(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
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
		Vector256 delta = _mm_sub_ps(left, right);

		// 絶対値の差分の取得
		Vector256 result = _mm_setzero_ps();
		result = _mm_sub_ps(result, delta);
		result = _mm_max_ps(result, delta);
		result = _mm_cmple_ps(result, epsilon);

		// zとwは無視して, xとyに着目する
		return ((_mm_movemask_ps(result) & 0b1111) == 0b1111);
	}

	/****************************************************************************
	*                       NearEqualVectorEach
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::NotEqualVectorEach(ConstVector256 left, ConstVector256 right, ConstVector256 epsilon) noexcept
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
		Vector256 delta = _mm_sub_ps(left, right);

		// 絶対値の取得
		Vector256 temp = _mm_setzero_ps();
		temp = _mm_sub_ps(temp, delta);
		temp = _mm_max_ps(temp, delta);
		temp = _mm_cmple_ps(temp, epsilon);
		return temp;
	}

	/****************************************************************************
	*                       GreaterVector2
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterVector2(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 equalMaskVector = _mm_cmpgt_ps(left, right); // 全て大きいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		// w, zは無視する
		return (_mm_movemask_ps(equalMaskVector) & 0b0011) == 0b0011;
	}

	/****************************************************************************
	*                       GreaterVector3
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterVector3(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 equalMaskVector = _mm_cmpgt_ps(left, right); // 全て大きいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		// wは無視する
		return (_mm_movemask_ps(equalMaskVector) & 0b0111) == 0b0111;
	}

	/****************************************************************************
	*                       GreaterVector4
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterVector4(ConstVector256 left, ConstVector256 right) noexcept
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
		// w, z, y, xそれぞれに対してleft > rightの場合は1を返すベクトル
		Vector256 equalMaskVector = _mm_cmpgt_ps(left, right); // 全て大きいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		return _mm_movemask_ps(equalMaskVector) == 0b1111;
	}

	/****************************************************************************
	*                       GreaterVectorEach
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterVector4(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 result = {};
		result.XY = _mm_cmpgt_pd(left.XY, right.XY);
		result.ZW = _mm_cmpgt_pd(left.ZW, right.ZW);
		return result;
	}


	/****************************************************************************
	*                       GreaterOrEqualVector2
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVector2(ConstVector256 left, ConstVector256 right) noexcept
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
		// w, z, y, xそれぞれに対してleft > rightの場合は1を返すベクトル
		Vector256 equalMaskVector = _mm_cmpge_ps(left, right); // 全て大きいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		return (_mm_movemask_ps(equalMaskVector) & 0b0011) == 0b0011;
	}

	/****************************************************************************
	*                       GreaterVector3
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVector3(ConstVector256 left, ConstVector256 right) noexcept
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
		// w, z, y, xそれぞれに対してleft > rightの場合は1を返すベクトル
		Vector256 equalMaskVector = _mm_cmpge_ps(left, right); // 全て大きいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		return (_mm_movemask_ps(equalMaskVector) & 0b0111) == 0b0111;
	}

	/****************************************************************************
	*                       GreaterOrEqualVector4
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 equalMaskVector = _mm_cmpge_ps(left, right); // 全て大きいor等しいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		return _mm_movemask_ps(equalMaskVector) == 0b1111;
	}

	/****************************************************************************
	*                       GreaterOrEqualVectorEach
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::GreaterOrEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 result = {};
		result.XY = _mm_cmpge_pd(left.XY, right.XY);
		result.ZW = _mm_cmpge_pd(left.ZW, right.ZW);
		return result;
	}

	/****************************************************************************
	*                       LessVector2
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessVector2(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 equalMaskVector = _mm_cmplt_ps(left, right); // 全て小さいなら{1, 1, 1, 1}, wだけ大きいなら{0, 1, 1, 1}

		return (_mm_movemask_ps(equalMaskVector) & 0b0011) == 0b0011;
	}

	/****************************************************************************
	*                       LessVector3
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessVector3(ConstVector256 left, ConstVector256 right) noexcept
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
		// w, z, y, xそれぞれに対してleft > rightの場合は1を返すベクトル
		Vector256 equalMaskVector = _mm_cmplt_ps(left, right); // 全て大きいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		// wは無視する
		return (_mm_movemask_ps(equalMaskVector) & 0b0111) == 0b0111;
	}

	/****************************************************************************
	*                       LessVector4
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessVector4(ConstVector256 left, ConstVector256 right) noexcept
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
		// w, z, y, xそれぞれに対してleft > rightの場合は1を返すベクトル
		Vector256 equalMaskVector = _mm_cmplt_ps(left, right); // 全て大きいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		return _mm_movemask_ps(equalMaskVector) == 0b1111;
	}

	/****************************************************************************
	*                       LessVectorEach
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LessVectorEach(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 result = {};
		result.XY = _mm_cmplt_pd(left.XY, right.XY);
		result.ZW = _mm_cmplt_pd(left.ZW, right.ZW);
		return result;
	}

	/****************************************************************************
	*                       LessOrEqualVector2
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVector2(ConstVector256 left, ConstVector256 right) noexcept
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
		// w, z, y, xそれぞれに対してleft > rightの場合は1を返すベクトル
		Vector256 equalMaskVector = _mm_cmple_ps(left, right); // 全て大きいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		return (_mm_movemask_ps(equalMaskVector) & 0b0011) == 0b0011;
	}

	/****************************************************************************
	*                       LessOrEqualVector3
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVector3(ConstVector256 left, ConstVector256 right) noexcept
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
		// w, z, y, xそれぞれに対してleft > rightの場合は1を返すベクトル
		Vector256 equalMaskVector = _mm_cmple_ps(left, right); // 全て大きいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		return (_mm_movemask_ps(equalMaskVector) & 0b0111) == 0b0111;
	}

	/****************************************************************************
	*                       LessOrEqualVector4
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVector4(ConstVector256 left, ConstVector256 right) noexcept
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
		// w, z, y, xそれぞれに対してleft >= rightの場合は1を返すベクトル
		Vector256 equalMaskVector = _mm_cmple_ps(left, right); // 全て大きいor等しいなら{1, 1, 1, 1}, wだけ小さいなら{0, 1, 1, 1}

		return _mm_movemask_ps(equalMaskVector) == 0b1111;
	}

	/****************************************************************************
	*                       LessVectorEach
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LessOrEqualVectorEach(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 result = {};
		result.XY = _mm_cmple_pd(left.XY, right.XY);
		result.ZW = _mm_cmple_pd(left.ZW, right.ZW);
		return result;
	}

	/****************************************************************************
	*                       InBoundsVector2
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::InBoundsVector2(ConstVector256 vector, ConstVector256 bounds) noexcept
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
		Vector256 lessOrEqual = _mm_cmple_ps(vector, bounds);
		Vector256 negateBound = _mm_mul_ps(bounds, VECTOR_128F_NEGATIVE_ONE);
		negateBound = _mm_cmple_ps(negateBound, vector); // greater or equal
		lessOrEqual = _mm_and_ps(lessOrEqual, negateBound);
		return (_mm_movemask_ps(lessOrEqual) & 0b0011) == 0b0011;
	}

	/****************************************************************************
	*                       InBoundsVector3
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::InBoundsVector3(ConstVector256 vector, ConstVector256 bounds) noexcept
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
		Vector256 lessOrEqual = _mm_cmple_ps(vector, bounds);
		Vector256 negateBound = _mm_mul_ps(bounds, VECTOR_128F_NEGATIVE_ONE);
		negateBound = _mm_cmple_ps(negateBound, vector); // greater or equal
		lessOrEqual = _mm_and_ps(lessOrEqual, negateBound);
		return (_mm_movemask_ps(lessOrEqual) & 0b0111) == 0b0111;

	}

	/****************************************************************************
	*                       InBoundsVector4
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::InBoundsVector4(ConstVector256 vector, ConstVector256 bounds) noexcept
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
		Vector256 lessOrEqual = _mm_cmple_ps(vector, bounds);
		Vector256 negateBound = _mm_mul_ps(bounds, VECTOR_128F_NEGATIVE_ONE);
		negateBound = _mm_cmple_ps(negateBound, vector); // greater or equal
		lessOrEqual = _mm_and_ps(lessOrEqual, negateBound);
		return _mm_movemask_ps(lessOrEqual) == 0b1111;
	}

	/****************************************************************************
	*                       InBoundsVectorEach
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector256Utility::InBoundsVectorEach(ConstVector256 vector, ConstVector256 bounds) noexcept
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
		Vector256 lessOrEqual = _mm_cmple_ps(vector, bounds);
		Vector256 negateBound = _mm_mul_ps(bounds, VECTOR_128F_NEGATIVE_ONE);
		negateBound = _mm_cmple_ps(negateBound, vector); // greater or equal
		lessOrEqual = _mm_and_ps(lessOrEqual, negateBound);
		return lessOrEqual;
	}

	#pragma endregion Operator

	#pragma region Math
	/****************************************************************************
	*                       Negate
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Negate(ConstVector256 vector) noexcept
	*
	*  @brief     Vectorの全ての要素の符号を判定します
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Negate(ConstVector256 vector) noexcept
	{
		Vector256 result = {};
		result.XY = _mm_sub_pd(_mm_setzero_pd(), vector.XY);
		result.ZW = _mm_sub_pd(_mm_setzero_pd(), vector.ZW);
		return result;
	}

	/****************************************************************************
	*                      LengthVector2
	****************************************************************************/
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::LengthVector2(ConstVector256 vector) noexcept
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
		// Dot積の計算
		Vector256 squareLength = _mm_mul_ps(vector, vector);

		// yのみのベクトルを取得
		Vector256 temp = PERMUTE_PS(squareLength, _MM_SHUFFLE(1, 1, 1, 1));

		// x + y
		squareLength = _mm_add_ss(squareLength, temp);
		squareLength = PERMUTE_PS(squareLength, _MM_SHUFFLE(0, 0, 0, 0));
		squareLength = _mm_sqrt_ps(squareLength);
		return _mm_cvtss_f32(squareLength);
	}

	/****************************************************************************
	*                      LengthVector3
	****************************************************************************/
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::LengthVector3(ConstVector256 vector) noexcept
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
		// Dot積の計算
		Vector256 squareLength = _mm_mul_ps(vector, vector);

		// zとyを要素にもつベクトル作成
		Vector256 temp = PERMUTE_PS(squareLength, _MM_SHUFFLE(1, 2, 1, 2));

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
	*                      LengthVector4
	****************************************************************************/
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::LengthVector4(ConstVector256 vector) noexcept
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
		// Dot積の計算
		Vector256 squareLength = _mm_mul_ps(vector, vector);

		// z, wのベクトルを取得
		Vector256 temp = PERMUTE_PS(squareLength, _MM_SHUFFLE(3, 2, 3, 2));

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
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::LengthSquaredVector2(ConstVector256 vector) noexcept
	*
	*  @brief     2次ノルムの二乗を算出
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
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::NormSquaredVector3(ConstVector256 vector) noexcept
	*
	*  @brief     2次ノルムの二乗を算出
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
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::LengthSquaredVector4(ConstVector256 vector) noexcept
	*
	*  @brief     2次ノルムの二乗を算出
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
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::DotVector2(ConstVector256 left, ConstVector256 right)
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
		Vector256 multiply = _mm_mul_ps(left, right);

		// z以降の計算は考慮しないため, yの結果を格納
		Vector256 y = PERMUTE_PS(multiply, _MM_SHUFFLE(1, 1, 1, 1));

		// x成分のみの結果に着目する.  
		return _mm_cvtss_f32(_mm_add_ss(multiply, y));
	}

	/****************************************************************************
	*                      DotVector3
	****************************************************************************/
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::DotVector3(ConstVector256 left, ConstVector256 right)
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
		Vector256 multiply = _mm_mul_ps(left, right);

		// x=Dot.F32[1], y=Dot.F32[2]
		Vector256 temp = PERMUTE_PS(multiply, _MM_SHUFFLE(2, 1, 2, 1));

		// result.F32[0] = x + y
		multiply = _mm_add_ss(multiply, temp);

		// x=multiply.F32[2]
		temp = PERMUTE_PS(multiply, _MM_SHUFFLE(1, 1, 1, 1));

		// Result.F32[0] = (x+y)+z
		temp = _mm_add_ss(multiply, temp);

		// x成分のみの結果に着目する.  
		return _mm_cvtss_f32(PERMUTE_PS(multiply, _MM_SHUFFLE(0, 0, 0, 0)));
	}

	/****************************************************************************
	*                      DotVector4
	****************************************************************************/
	*  @fn        inline double SIMD_CALL_CONVENTION Vector256Utility::DotVector4(ConstVector256 left, ConstVector256 right)
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
		Vector256 temp = _mm_mul_ps(left, temp2);

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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CrossVector2(ConstVector256 left, ConstVector256 right)
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
		Vector256 result = PERMUTE_PS(right, _MM_SHUFFLE(0, 1, 0, 1));

		// leftと入れ替え後のrightの各要素を乗算する
		result = _mm_mul_ps(result, left);

		// yを抽出する
		Vector256 y = PERMUTE_PS(result, _MM_SHUFFLE(1, 1, 1, 1));

		// 値を減算する
		result = _mm_sub_ss(result, y);

		// x成分のみを取り出して結果とする
		return PERMUTE_PS(result, _MM_SHUFFLE(0, 0, 0, 0));
	}

	/****************************************************************************
	*                      CrossVector3
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CrossVector3(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 temp1 = PERMUTE_PS(left, _MM_SHUFFLE(3, 0, 2, 1)); 

		// rightの要素を(z2, x2. y2, w2)の順に並べ替える
		Vector256 temp2 = PERMUTE_PS(right, _MM_SHUFFLE(3, 1, 0, 2));

		// 一時ベクトルの要素ごとの乗算
		Vector256 result = _mm_mul_ps(temp1, temp2);

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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CrossVector4(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 result = PERMUTE_PS(second, _MM_SHUFFLE(2, 1, 3, 2));
		Vector256 temp3  = PERMUTE_PS(third, _MM_SHUFFLE(1, 3, 2, 3));
		result = _mm_mul_ps(result, temp3);

		// - second.wzwy * third.zwyz
		Vector256 temp2 = PERMUTE_PS(second, _MM_SHUFFLE(1, 3, 2, 3));
		temp3  = PERMUTE_PS(temp3, _MM_SHUFFLE(1, 3, 0, 1));
		result = NegativeMultiplySubtract(temp2, temp3, result);

		// term1 * first.yxxx
		Vector256 temp1 = PERMUTE_PS(first, _MM_SHUFFLE(0, 0, 0, 1));
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NormalizeVector2(ConstVector256 vector) noexcept
	*
	*  @brief     入力ベクトルの単位ベクトルを返します
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NormalizeVector2(ConstVector256 vector) noexcept
	{
		// 2次ノルム計算計算
		Vector256 squareLength = _mm_mul_ps(vector, vector);
		Vector256 temp = PERMUTE_PS(squareLength, _MM_SHUFFLE(1, 1, 1, 1));
		squareLength   = _mm_add_ps(squareLength, temp);
		squareLength   = PERMUTE_PS(squareLength, _MM_SHUFFLE(0, 0, 0, 0));
		Vector256 norm = _mm_sqrt_ps(squareLength);

		// 0で割られる可能性があるかどうかを調べる. 値を持っていたら1
		Vector256 zeroMask = _mm_setzero_ps();
		zeroMask = _mm_cmpneq_ps(zeroMask, norm);

		// 無限大でない場合は対応する要素が1になるマスクの作成
		Vector256 infinityMask = _mm_cmpneq_ps(squareLength, VECTOR_128F_INFINITY);

		// 正規化の実行
		Vector256 result = _mm_div_ps(vector, norm);

		// 0付近で割られて無限大になったものに対しては0に設定
		result = _mm_and_ps(result, zeroMask);

		// 無限大かに基づいてqnanまたはresultを選択する
		Vector256 temp1 = _mm_andnot_ps(infinityMask, VECTOR_128F_QNAN);
		Vector256 temp2 = _mm_and_ps(result, infinityMask);
		result = _mm_or_ps(temp1, temp2);
		return result;
	}

	/****************************************************************************
	*                      NormalizeVector3
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NormalizeVector3(ConstVector256 vector) noexcept
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
		Vector256 squareLength = _mm_mul_ps(vector, vector);

		// z*z, y*y, z*z, y*y
		Vector256 temp         = PERMUTE_PS(squareLength, _MM_SHUFFLE(2, 1, 2, 1)); 

		// x*x + z*z, y*y, z*z, w*w
		squareLength           = _mm_add_ss(squareLength, temp);

		// 全てy*y
		temp = PERMUTE_PS(temp, _MM_SHUFFLE(1, 1, 1, 1));

		// 1要素目がx*x + y*y + z*z
		squareLength = _mm_add_ss(squareLength, temp);
		// 全ての要素がドット積
		squareLength = PERMUTE_PS(squareLength, _MM_SHUFFLE(0, 0, 0, 0));

		// 2次ノルムの計算
		Vector256 norm = _mm_sqrt_ps(squareLength);

		// 0で割られる可能性があるかどうかを調べる. 値を持っていたら1
		Vector256 zeroMask = _mm_setzero_ps();
		zeroMask = _mm_cmpneq_ps(zeroMask, norm);

		// 無限大でない場合は対応する要素が1になるマスクの作成
		Vector256 infinityMask = _mm_cmpneq_ps(squareLength, VECTOR_128F_INFINITY);

		// 正規化の実行
		Vector256 result = _mm_div_ps(vector, norm);

		// 0付近で割られて無限大になったものに対しては0に設定
		result = _mm_and_ps(result, zeroMask);

		// 無限大かに基づいてqnanまたはresultを選択する
		Vector256 temp1 = _mm_andnot_ps(infinityMask, VECTOR_128F_QNAN);
		Vector256 temp2 = _mm_and_ps(result, infinityMask);
		result = _mm_or_ps(temp1, temp2);
		return result;
	}

	/****************************************************************************
	*                      NormalizeVector4
	****************************************************************************/ 
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NormalizeVector4(ConstVector256 vector) noexcept
	*
	*  @brief     入力ベクトルの単位ベクトルを返します
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::NormalizeVector4(ConstVector256 vector) noexcept
	{
		// x, y, z, wに対するドット積の計算
		// x*x, y*y, z*z, w*w
		Vector256 squareLength = _mm_mul_ps(vector, vector);

		// w*w, z*z, w*w, z*z
		Vector256 temp = PERMUTE_PS(squareLength, _MM_SHUFFLE(3, 2, 3, 2));

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

		// 2次ノルムの計算
		Vector256 norm = _mm_sqrt_ps(squareLength);

		// 0で割られる可能性があるかどうかを調べる. 値を持っていたら1
		Vector256 zeroMask = _mm_setzero_ps();
		zeroMask = _mm_cmpneq_ps(zeroMask, norm);

		// 無限大でない場合は対応する要素が1になるマスクの作成
		Vector256 infinityMask = _mm_cmpneq_ps(squareLength, VECTOR_128F_INFINITY);

		// 正規化の実行
		Vector256 result = _mm_div_ps(vector, norm);

		// 0付近で割られて無限大になったものに対しては0に設定
		result = _mm_and_ps(result, zeroMask);

		// 無限大かに基づいてqnanまたはresultを選択する
		Vector256 temp1 = _mm_andnot_ps(infinityMask, VECTOR_128F_QNAN);
		Vector256 temp2 = _mm_and_ps(result, infinityMask);
		result = _mm_or_ps(temp1, temp2);
		return result;
	}

	/****************************************************************************
	*                      ReflectVector2
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ReflectVector2(ConstVector256 input, ConstVector256 normal) noexcept
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ReflectVector4(ConstVector256 input, ConstVector256 normal) noexcept
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ReflectVector4(ConstVector256 input, ConstVector256 normal)
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector2(ConstVector256 input, ConstVector256 normal, const double refractionIndex)
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector3(ConstVector256 input, ConstVector256 normal, const double refractionIndex)
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector4(ConstVector256 input, ConstVector256 normal, const double refractionIndex)
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector2(ConstVector256 input, ConstVector256 normal, const double refractionIndex)
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
		Vector256 temp = NegativeMultiplySubtract(inputDotNormal, inputDotNormal, VECTOR_128F_ONE);
		temp = _mm_mul_ps(temp, refractionIndex);
		temp = NegativeMultiplySubtract(temp, refractionIndex, VECTOR_128F_ONE);

		// 結果が0以下であれば, sqrtは失敗ということでzeroに置き換える
		Vector256 zeroMask = _mm_cmpgt_ps(temp, VECTOR_128F_ZERO);

		// r = refractionIndex * inputDotNormal + sqrt(r)
		temp = _mm_sqrt_ps(temp);
		temp = MultiplyAdd(refractionIndex, inputDotNormal, temp);

		// result = refractionIndex * input - normal * r
		Vector256 result = _mm_mul_ps(refractionIndex, input);
		result = NegativeMultiplySubtract(temp, normal, result);
		result = _mm_and_ps(result, zeroMask);

		return result;
	}

	/****************************************************************************
	*                      RefractVector3
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector3(ConstVector256 input, ConstVector256 normal, const double refractionIndex)
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
		Vector256 r  = NegativeMultiplySubtract(inputDotNormal, inputDotNormal, VECTOR_128F_ONE);
		Vector256 r2 = _mm_mul_ps(refractionIndex, refractionIndex);
		r = NegativeMultiplySubtract(r, r2, VECTOR_128F_ONE);

		Vector256 result = _mm_cmple_ps(r, VECTOR_128F_ZERO);

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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::RefractVector4(ConstVector256 input, ConstVector256 normal, const double refractionIndex)
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
		Vector256 r  = NegativeMultiplySubtract(inputDotNormal, inputDotNormal, VECTOR_128F_ONE);
		Vector256 r2 = _mm_mul_ps(refractionIndex, refractionIndex);
		r = NegativeMultiplySubtract(r, r2, VECTOR_128F_ONE);

		Vector256 result = _mm_cmple_ps(r, VECTOR_128F_ZERO);

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
	*  @fn        inline bool Vector256Utility::IsInfiniteVector2(ConstVector256 vector) noexcept
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
		Vector256 temp = _mm_and_ps(vector, VECTOR_128U_MASK_ABS);
		// 無限大と比較
		temp = _mm_cmpeq_ps(temp, VECTOR_128F_INFINITY);
		// どれかが無限大であれば、符号は真である。
		return (_mm_movemask_ps(temp) & 0b0011) != 0;
	}

	/****************************************************************************
	*                      IsInfiniteVector3
	****************************************************************************/
	*  @fn        inline bool Vector256Utility::IsInfiniteVector3(ConstVector256 vector) noexcept
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
		Vector256 temp = _mm_and_ps(vector, VECTOR_128U_MASK_ABS);
		// 無限大と比較
		temp = _mm_cmpeq_ps(temp, VECTOR_128F_INFINITY);
		// どれかが無限大であれば、符号は真である。
		return (_mm_movemask_ps(temp) & 0b0111) != 0;
	}

	/****************************************************************************
	*                      IsInfiniteVector4
	****************************************************************************/
	*  @fn        inline bool Vector256Utility::IsInfiniteVector4(ConstVector256 vector) noexcept
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
		Vector256 temp = _mm_and_ps(vector, VECTOR_128U_MASK_ABS);
		// 無限大と比較
		temp = _mm_cmpeq_ps(temp, VECTOR_128F_INFINITY);
		// どれかが無限大であれば、符号は真である。
		return (_mm_movemask_ps(temp) != 0);
	}

	/****************************************************************************
	*                      Sqrt
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Sqrt(ConstVector256 vector) noexcept
	*
	*  @brief      全ての要素に根号を取ったものを返します
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Sqrt(ConstVector256 vector) noexcept
	{
		Vector256 result = {};
		result.XY = _mm_sqrt_pd(vector.XY);
		result.ZW = _mm_sqrt_pd(vector.ZW);
		return result;
	}

	/****************************************************************************
	*                      ReciprocalSqrt
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ReciprocalSqrt(ConstVector256 vector) noexcept
	*
	*  @brief       全ての要素の逆数を取り, それに根号を取ったものを返します
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ReciprocalSqrt(ConstVector256 vector) noexcept
	{
		return Divide(VECTOR_256D_ONE, Sqrt(vector));
	}

	/****************************************************************************
	*                      Exp10
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Exp10(ConstVector256 vector) noexcept
	*
	*  @brief     全ての要素の2^{vector}を返します
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Exp2(ConstVector256 vector) noexcept
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Exp10(ConstVector256 vector) noexcept
	*
	*  @brief      全ての要素の10^{vector}を返します
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Exp10(ConstVector256 vector) noexcept
	{
		// Exp10(vector) = Exp2(vector * log2(10))
		Vector256 result = Multiply(VECTOR_128F_LOG_10, vector);
		return Exp2(vector);
	}

	/****************************************************************************
	*                      ExpE
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ExpE(ConstVector256 vector) noexcept
	*
	*  @brief     全ての要素のe^{vector}を返します
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ExpE(ConstVector256 vector) noexcept
	{
		Vector256 result = Multiply(VECTOR_128F_LOG_E, vector);
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
			static const Vector256i g_XM0000FFFF = { { { 0x0000FFFF, 0x0000FFFF, 0x0000FFFF, 0x0000FFFF } } };
			static const Vector256i g_XM000000FF = { { { 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF } } };
			static const Vector256i g_XM0000000F = { { { 0x0000000F, 0x0000000F, 0x0000000F, 0x0000000F } } };
			static const Vector256i g_XM00000003 = { { { 0x00000003, 0x00000003, 0x00000003, 0x00000003 } } };

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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Log2(ConstVector256 vector) noexcept
	*
	*  @brief     全ての要素のlog2{vector}を返します
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Log2(ConstVector256 vector) noexcept
	{
		/*----------------------------------------------------------------------
		*   指数部と仮数部を取り出す.
		*----------------------------------------------------------------------*/
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
		*----------------------------------------------------------------------*/
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Log10(ConstVector256 vector) noexcept
	*
	*  @brief     全ての要素のlog10{vector}を返します
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Log10(ConstVector256 vector) noexcept
	{
		/*----------------------------------------------------------------------
		*   指数部と仮数部を取り出す.
		*----------------------------------------------------------------------*/
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
		*----------------------------------------------------------------------*/
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LogE(ConstVector256 vector) noexcept
	*
	*  @brief     全ての要素のloge{vector}を返します
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LogE(ConstVector256 vector) noexcept
	{
		/*----------------------------------------------------------------------
		*   指数部と仮数部を取り出す.
		*----------------------------------------------------------------------*/
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
		*----------------------------------------------------------------------*/
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Pow(ConstVector256 base, ConstVector256 power)
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
		ALIGNED_DATA(16) double a[4];
		ALIGNED_DATA(16) double b[4];
		_mm_store_ps(a, base);
		_mm_store_ps(b, power);
		Vector256 vResult = _mm_setr_ps(
			powf(a[0], b[0]),
			powf(a[1], b[1]),
			powf(a[2], b[2]),
			powf(a[3], b[3]));
		return vResult;

	}

	/****************************************************************************
	*                      Sin
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Sin(ConstVector256 vector) noexcept
	*
	*  @brief     全ての要素にSinを取ったものを返します[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Sin(ConstVector256 vector) noexcept
	{
		// -PI <= theta <= PIの範囲に収める
		Vector256 normalizedAngle = ModAngles(vector);

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
		const Vector256 sinCoefficient0 = VECTOR_128F_SIN_COEFFICIENTS0;
		const Vector256 sinCoefficient1 = VECTOR_128F_SIN_COEFFICIENTS1;
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Cos(ConstVector256 vector) noexcepts
	*
	*  @brief     全ての要素にCosを取ったものを返します
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Cos(ConstVector256 vector) noexcept
	{
		// -PI <= theta <= PIの範囲に収める
		Vector256 normalizedAngle = ModAngles(vector);

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
		const Vector256 cosCoefficient0 = VECTOR_128F_COS_COEFFICIENTS0;
		const Vector256 cosCoefficient1 = VECTOR_128F_COS_COEFFICIENTS1;
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Tan(ConstVector256 vector) noexcept
	*
	*  @brief     全ての要素にTanを取ったものを返します
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Tan(ConstVector256 vector) noexcept
	{
		static const Vector256f tanCoefficients0 = { { { 1.0f, -4.667168334e-1f, 2.566383229e-2f, -3.118153191e-4f } } };
		static const Vector256f tanCoefficients1 = { { { 4.981943399e-7f, -1.333835001e-1f, 3.424887824e-3f, -1.786170734e-5f } } };
		static const Vector256f tanConstants = { { { 1.570796371f, 6.077100628e-11f, 0.000244140625f, 0.63661977228f /*2 / Pi*/ } } };
		static const Vector256u mask         = { { { 0x1, 0x1, 0x1, 0x1 } } };

		// 定数値の事前準備
		Vector256 twoDivPi = SplatW(tanConstants.V);
		Vector256 zero     = Zero();
		Vector256 c0       = SplatX(tanConstants.V);
		Vector256 c1       = SplatY(tanConstants.V);
		Vector256 epsilon  = SplatZ(tanConstants.V);

		// 2 / PIで乗算し, その結果を四捨五入する. 
		Vector256 va = Multiply(vector, twoDivPi);
		va = Round(va);

		// vaの各要素をc0, c1を使って補正することで, Vの各要素が最も近いPI / 2からどれだけ離れているかを求める
		Vector256 vc = NegativeMultiplySubtract(va, c0, vector);
		Vector256 vb = Abs(va);
		vc = NegativeMultiplySubtract(va, c1, vc);

		// vbはvaが正か負かを判定するために使用する
		reinterpret_cast<__m128i*>(&vb)[0] = _mm_cvttps_epi32(vb);

		// テイラー展開に使用する定数の作成
		Vector256 vc2 = Multiply(vc, vc);
		Vector256 t7 = SplatW(tanCoefficients1.V);
		Vector256 t6 = SplatZ(tanCoefficients1.V);
		Vector256 t4 = SplatX(tanCoefficients1.V);
		Vector256 t3 = SplatW(tanCoefficients0.V);
		Vector256 t5 = SplatY(tanCoefficients1.V);
		Vector256 t2 = SplatZ(tanCoefficients0.V);
		Vector256 t1 = SplatY(tanCoefficients0.V);
		Vector256 t0 = SplatX(tanCoefficients0.V);

		Vector256 isEvenVB = AndInt(vb, mask.V);
		isEvenVB = EqualAsIntVectorEach(isEvenVB, zero);

		Vector256 n = MultiplyAdd(vc2, t7, t6);
		Vector256 d = MultiplyAdd(vc2, t4, t3);
		n = MultiplyAdd(vc2, n, t5);
		d = MultiplyAdd(vc2, d, t2);
		n = Multiply(vc2, n);
		d = MultiplyAdd(vc2, d, t1);
		n = MultiplyAdd(vc, n, vc);
		Vector256 nearZeroVC = InBoundsVectorEach(vc, epsilon);
		d = MultiplyAdd(vc2, d, t0);

		n = Select(n, vc, nearZeroVC);
		d = Select(d, VECTOR_128F_ONE.V, nearZeroVC);

		Vector256 r0 = Negate(n);
		Vector256 r1 = Divide(n, d);
		r0 = Divide(d, r0);

		Vector256 isZero = EqualVectorEach(vector, zero);

		Vector256 result = Select(r0, r1, isEvenVB);

		result = Select(result, zero, isZero);

		return result;

	}

	/****************************************************************************
	*                      SinCos
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SinCos(ConstVector256 input, Vector256* sin, Vector256* cos) noexcept
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
		// -PI <= theta <= PIの範囲に収める
		Vector256 normalizedAngle = ModAngles(input);

		// -PI/2 <= theta < PI/2の時はsin, cosの対称性を使用するsin(y) = sin(x), cos(y) = sign*cos(x).
		Vector256 sign = _mm_and_ps(normalizedAngle, VECTOR_128U_NEGATIVE_ZERO);

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
		const Vector256 sinCoefficients1 = VECTOR_128F_SIN_COEFFICIENTS1;
		__m128 vConstantsB = PERMUTE_PS(sinCoefficients1, _MM_SHUFFLE(0, 0, 0, 0));
		const Vector256 sinCoefficients0 = VECTOR_128F_SIN_COEFFICIENTS0;
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
		const Vector256 cosCoefficients1 = VECTOR_128F_COS_COEFFICIENTS1;
		vConstantsB = PERMUTE_PS(cosCoefficients1, _MM_SHUFFLE(0, 0, 0, 0));
		const Vector256 cosCoefficients0 = VECTOR_128F_COS_COEFFICIENTS0;
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SinH(ConstVector256 vector) noexcept
	*
	*  @brief     全ての要素にSinHを取ったものを返します[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::SinH(ConstVector256 vector) noexcept
	{
		// 自然対数の底を2からeに変換
		static const Vector256f scale = { { { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f } } }; // 1.0f / ln(2.0f)

		// sinh(x) = (e^x + e^-x) / 2 - 1として計算
		Vector256 v1 = MultiplyAdd(vector, scale.V, VECTOR_128F_NEGATIVE_ONE);
		Vector256 v2 = NegativeMultiplySubtract(vector, scale, VECTOR_128F_NEGATIVE_ONE);
		Vector256 e1 = Exp2(vector);
		Vector256 e2 = Exp2(vector);

		return _mm_sub_ps(e1, e2);
	}

	/****************************************************************************
	*                      CosH
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CosH(ConstVector256 vector) noexcept
	*
	*  @brief     全ての要素にCosHを取ったものを返します[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CosH(ConstVector256 vector) noexcept
	{
		// 自然対数の底を2からeに変換
		static const Vector256f scale = { { { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f } } }; // 1.0f / ln(2.0f)

		Vector256 v1 = MultiplyAdd(vector, scale.V, VECTOR_128F_NEGATIVE_ONE);
		Vector256 v2 = NegativeMultiplySubtract(vector, scale.V, VECTOR_128F_NEGATIVE_ONE);
		Vector256 e1 = Exp2(vector);
		Vector256 e2 = Exp2(vector);

		return _mm_add_ps(e1, e2);
	}

	/****************************************************************************
	*                      TanH
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::TanH(ConstVector256 vector) noexcept
	*
	*  @brief     全ての要素にTanHを取ったものを返します[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::TanH(ConstVector256 vector) noexcept
	{
		static const Vector256f scale = { { { 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f } } }; // 2.0f / ln(2.0f)

		Vector256 exponential = _mm_mul_ps(vector, scale.V);
		exponential = Exp2(exponential);
		exponential = MultiplyAdd(exponential, VECTOR_128F_ONE_HALF.V, VECTOR_128F_ONE_HALF.V);
		exponential = _mm_div_ps(VECTOR_128F_ONE.V, exponential);
		return _mm_sub_ps(VECTOR_128F_ONE.V, exponential);
	}

	/****************************************************************************
	*                      ArcSin
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ArcSin(ConstVector256 vector) noexcept
	*
	*  @brief     全ての要素にArcSinを取ったものを返します[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ArcSin(ConstVector256 vector) noexcept
	{
		__m128 isNonNegative  = _mm_cmpge_ps(vector, VECTOR_128F_ZERO); // 0以上であるか
		__m128 mValue         = _mm_sub_ps(VECTOR_128F_ZERO, vector);   // -vector
		__m128 absoluteVector = _mm_max_ps(vector, mValue);             // |vector|

		// (1-|V|)を計算し、負の数のsqrtを避けるためにゼロにクランプする
		__m128 oneMinusValue      = _mm_sub_ps(VECTOR_128F_ONE, absoluteVector);
		__m128 clampOneMinusValue = _mm_max_ps(VECTOR_128F_ZERO, oneMinusValue);
		__m128 rootOneMinusValue  = _mm_sqrt_ps(clampOneMinusValue);

		// テイラー展開を用いてarcsinの近似値を計算する
		const Vector256 arcCoefficients1 = VECTOR_128F_ARC_COEFFICIENTS1;
		__m128 constantsB = PERMUTE_PS(arcCoefficients1, _MM_SHUFFLE(3, 3, 3, 3));
		__m128 constantsA = PERMUTE_PS(arcCoefficients1, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 t0 = MultiplyAdd(constantsB, absoluteVector, constantsA);

		constantsA = PERMUTE_PS(arcCoefficients1, _MM_SHUFFLE(1, 1, 1, 1));
		t0 = MultiplyAdd(t0, absoluteVector, constantsA);

		constantsA = PERMUTE_PS(arcCoefficients1, _MM_SHUFFLE(0, 0, 0, 0));
		t0 = MultiplyAdd(t0, absoluteVector, constantsA);

		const Vector256 arcCoefficients0 = VECTOR_128F_ARC_COEFFICIENTS0;
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ArcCos(ConstVector256 vector) noexcept
	*
	*  @brief     全ての要素にArcSinを取ったものを返します[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ArcCos(ConstVector256 vector) noexcept
	{
		__m128 isNonNegative  = _mm_cmpge_ps(vector, VECTOR_128F_ZERO); // 0以上であるか
		__m128 mValue         = _mm_sub_ps(VECTOR_128F_ZERO, vector);   // -vector
		__m128 absoluteVector = _mm_max_ps(vector, mValue);             // |vector|

		// (1-|V|)を計算し、負の数のsqrtを避けるためにゼロにクランプする
		__m128 oneMinusValue      = _mm_sub_ps(VECTOR_128F_ONE, absoluteVector);
		__m128 clampOneMinusValue = _mm_max_ps(VECTOR_128F_ZERO, oneMinusValue);
		__m128 rootOneMinusValue  = _mm_sqrt_ps(clampOneMinusValue);

		// テイラー展開を用いてarcsinの近似値を計算する
		const Vector256 arcCoefficients1 = VECTOR_128F_ARC_COEFFICIENTS1;
		__m128 constantsB = PERMUTE_PS(arcCoefficients1, _MM_SHUFFLE(3, 3, 3, 3));
		__m128 constantsA = PERMUTE_PS(arcCoefficients1, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 t0 = MultiplyAdd(constantsB, absoluteVector, constantsA);

		constantsA = PERMUTE_PS(arcCoefficients1, _MM_SHUFFLE(1, 1, 1, 1));
		t0 = MultiplyAdd(t0, absoluteVector, constantsA);

		constantsA = PERMUTE_PS(arcCoefficients1, _MM_SHUFFLE(0, 0, 0, 0));
		t0 = MultiplyAdd(t0, absoluteVector, constantsA);

		const Vector256 arcCoefficients0 = VECTOR_128F_ARC_COEFFICIENTS0;
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ArcCos(ConstVector256 vector) noexcept
	*
	*  @brief     全ての要素にArcSinを取ったものを返します[rad]
	*
	*  @param[in] ConstVector256 : vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ArcTan(ConstVector256 vector) noexcept
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
		const Vector256 arcTanCoefficients1 = VECTOR_128F_ATAN_COEFFICIENTS1;
		__m128 constantsB = PERMUTE_PS(arcTanCoefficients1, _MM_SHUFFLE(3, 3, 3, 3));
		__m128 constantsA = PERMUTE_PS(arcTanCoefficients1, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 result = MultiplyAdd(constantsB, x2, constantsA);

		constantsA = PERMUTE_PS(arcTanCoefficients1, _MM_SHUFFLE(1, 1, 1, 1));
		result = MultiplyAdd(result, x2, constantsA);

		constantsA = PERMUTE_PS(arcTanCoefficients1, _MM_SHUFFLE(0, 0, 0, 0));
		result = MultiplyAdd(result, x2, constantsA);

		const Vector256 arcTanCoefficients0 = VECTOR_128F_ATAN_COEFFICIENTS0;
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
	*                       Lerp
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Lerp(ConstVector256 start, ConstVector256 end, const double t) noexcept
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
		Vector256 diff = _mm_sub_ps(end, start);
		Vector256 scale = _mm_set_ps1(t);
		return MultiplyAdd(diff, scale, start);
	}

	/****************************************************************************
	*                       LerpV
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::LerpV(ConstVector256 start, ConstVector256 end, const double t) noexcept
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
		Vector256 diff = _mm_sub_ps(end, start);
		return MultiplyAdd(diff, t, start);
	}

	/****************************************************************************
	*                       Hermite
	****************************************************************************/
	*  @fn        __forceinline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Hermite(ConstVector256 startPosition, ConstVector256 startTangent, ConstVector256 endPosition, ConstVector256 endTangent, const double t) noexcept
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

		Vector256 p0 = _mm_set_ps1(2.0f * t3 - 3.0f * t2 + 1.0f);
		Vector256 t0 = _mm_set_ps1(t3 - 2.0f * t2 + t);
		Vector256 p1 = _mm_set_ps1(-2.0f * t3 + 3.0f * t2);
		Vector256 t1 = _mm_set_ps1(t3 - t2);
		
		Vector256 result = _mm_mul_ps(p0, startPosition);
		result = MultiplyAdd(startTangent, t0, result);
		result = MultiplyAdd(endPosition, p1, result);
		result = MultiplyAdd(endTangent , t1, result);
		return result;
	}

	/****************************************************************************
	*                       HermiteV
	****************************************************************************/
	*  @fn        __forceinline Vector256 SIMD_CALL_CONVENTION Vector256Utility::HermiteV(ConstVector256 startPosition, ConstVector256 startTangent, ConstVector256 endPosition, ConstVector256 endTangent, const double t) noexcept
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
		static const Vector256f CatMulT2 = { { { -3.0f, -2.0f, 3.0f, -1.0f } } };
		static const Vector256f CatMulT3 = { { { 2.0f, 1.0f, -2.0f, 1.0f } } };

		Vector256 t2 = _mm_mul_ps(t, t);
		Vector256 t3 = _mm_mul_ps(t, t2);
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
		Vector256 result = PERMUTE_PS(t3, _MM_SHUFFLE(0, 0, 0, 0));
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CatMullRom(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, ConstVector256 position3, const double t) noexcept
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

		Vector256 p0 = _mm_set_ps1((-t3 + 2.0f * t2 - t) * 0.5f);
		Vector256 p1 = _mm_set_ps1((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
		Vector256 p2 = _mm_set_ps1((-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
		Vector256 p3 = _mm_set_ps1((t3 - t2) * 0.5f);

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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::CatMullRom(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, ConstVector256 position3, const double t) noexcept
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
		static const Vector256f Catmul2 = { { { 2.0f, 2.0f, 2.0f, 2.0f } } };
		static const Vector256f Catmul3 = { { { 3.0f, 3.0f, 3.0f, 3.0f } } };
		static const Vector256f Catmul4 = { { { 4.0f, 4.0f, 4.0f, 4.0f } } };
		static const Vector256f Catmul5 = { { { 5.0f, 5.0f, 5.0f, 5.0f } } };
		// Cache T^2 and T^3
		Vector256 t2 = _mm_mul_ps(t, t);
		Vector256 t3 = _mm_mul_ps(t, t2);
		// Perform the Position0 term
		Vector256 vResult = _mm_add_ps(t2, t2);
		vResult = _mm_sub_ps(vResult, t);
		vResult = _mm_sub_ps(vResult, t3);
		vResult = _mm_mul_ps(vResult, position0);
		// Perform the Position1 term and add
		Vector256 vTemp = _mm_mul_ps(t3, Catmul3);
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
	*  @fn        __forceinline Vector256 SIMD_CALL_CONVENTION Vector256Utility::BaryCentric(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, const double f, const double g) noexcept
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
		Vector256 r1 = _mm_sub_ps(position1, position0);
		Vector256 r2 = _mm_sub_ps(position2, position0);
		Vector256 sf = _mm_set_ps1(f);
		r1 = MultiplyAdd(r1, sf, position0);
		Vector256 sg = _mm_set_ps1(g);
		return MultiplyAdd(r2, sg, r1);
	}

	/****************************************************************************
	*                      BaryCentricV
	****************************************************************************/
	*  @fn        __forceinline Vector256 SIMD_CALL_CONVENTION Vector256Utility::BaryCentricV(ConstVector256 position0, ConstVector256 position1, ConstVector256 position2, const double f, const double g) noexcept
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
		Vector256 r1 = _mm_sub_ps(position1, position0);
		Vector256 r2 = _mm_sub_ps(position2, position0);
		r1 = MultiplyAdd(r1, f, position0);
		return MultiplyAdd(r2, g, r1);
	}

	/****************************************************************************
	*                       Min
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Min(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 result = {};
		result.XY = _mm_min_pd(left.XY, right.XY);
		result.ZW = _mm_min_pd(left.ZW, right.ZW);
		return result;
	}

	/****************************************************************************
	*                       Max
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Max(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 result = {};
		result.XY = _mm_max_pd(left.XY, right.XY);
		result.ZW = _mm_max_pd(left.ZW, right.ZW);
		return result;
	}

	/****************************************************************************
	*                       Round
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Round(ConstVector256 vector) noexcept
	*
	*  @brief     最も近い整数に丸める
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Round(ConstVector256 vector) noexcept
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Truncate(ConstVector256 vector) noexcept
	*
	*  @brief     切り捨て (指定した数値の端数を切り捨てた整数に丸める。数直線上で0の方向にある次の整数
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Truncate(ConstVector256 vector) noexcept
	{
		// NAN, INF, 8388608より大きい数値を扱うには, マスキングを使用する
		// 絶対値を取得
		__m128i test = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128U_MASK_ABS);

		// 8388608より大きいかどうかのテスト (すべての浮動小数点数、NAN、INFなし)
		test = _mm_cmplt_epi32(test, VECTOR_128F_NO_FRACTION);

		// 切り捨てで丸めるために、intに変換してdoubleに戻す。
		__m128i intVector = _mm_cvttps_epi32(vector);

		Vector256 result = _mm_cvtepi32_ps(intVector);

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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Floor(ConstVector256 vector) noexcept
	*
	*  @brief     負の方向に丸める
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Floor(ConstVector256 vector) noexcept
	{
		// NAN, INF, 8388608より大きい数値を扱うには, マスキングを使用する
		// 絶対値を取得
		__m128i test = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128U_MASK_ABS);

		// 8388608より大きいかどうかのテスト (すべての浮動小数点数、NAN、INFなし)
		test = _mm_cmplt_epi32(test, VECTOR_128F_NO_FRACTION);

		// 切り捨てで丸めるために、intに変換してdoubleに戻す。
		__m128i intVector = _mm_cvttps_epi32(vector);

		Vector256 result = _mm_cvtepi32_ps(intVector);

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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Ceiling(ConstVector256 vector) noexcept
	*
	*  @brief     切り上げ 数直線上で正の方向にある次の整数に丸める
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector256
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Ceiling(ConstVector256 vector) noexcept
	{
		// NAN, INF, 8388608より大きい数値を扱うには, マスキングを使用する
		// 絶対値を取得
		__m128i test = _mm_and_si128(_mm_castps_si128(vector), VECTOR_128U_MASK_ABS);

		// 8388608より大きいかどうかのテスト (すべての浮動小数点数、NAN、INFなし)
		test = _mm_cmplt_epi32(test, VECTOR_128F_NO_FRACTION);

		// 切り捨てで丸めるために、intに変換してdoubleに戻す。
		__m128i intVector = _mm_cvttps_epi32(vector);

		Vector256 result = _mm_cvtepi32_ps(intVector);

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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Clamp(ConstVector256 vector, ConstVector256 min, ConstVector256 max) noexcept
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
		Vector256 result = _mm_max_ps(min, vector);
		return _mm_min_ps(max, result);
	}

	/****************************************************************************
	*                       Saturate
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Saturate(ConstVector256 vector)
	*
	*  @brief     1以上には行かないようにする
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Saturate(ConstVector256 vector) noexcept
	{
		Vector256 result = _mm_max_ps(vector, VECTOR_128F_ZERO);
		return _mm_min_ps(result, VECTOR_128F_ONE);
	}

	/****************************************************************************
	*                       Abs
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Saturate(ConstVector256 vector)
	*
	*  @brief     絶対値
	*
	*  @param[in] ConstVector256 vector
	*
	*  @return 　　Vector
	*****************************************************************************/
	inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::Abs(ConstVector256 vector) noexcept
	{
		Vector256 result = _mm_setzero_ps();
		result = _mm_sub_ps(result, vector);
		result = _mm_max_ps(result, vector);
		return result;
	}

	#pragma endregion Math

	#pragma region Angle
	/****************************************************************************
	*                       ModAngles
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ModAngles(ConstVector256 vector)
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
		Vector256 result = _mm_mul_ps(angles, VECTOR_128F_RECIPROCAL_2PI.V);

		// 丸め処理が複雑なため、インライン関数を使用する。
		result = Round(result);
		return NegativeMultiplySubtract(result, VECTOR_128F_2PI, angles);
	}

	/****************************************************************************
	*                       AddAngles
	****************************************************************************/
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::AddAngles(ConstVector256 left, ConstVector256 right) noexcept
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
		Vector256 result = _mm_add_ps(left, right);

		// -Piよりも小さいかを判定する
		Vector256 offset = _mm_cmplt_ps(result, VECTOR_128F_NEGATIVE_PI);
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
	*  @fn        inline Vector256 SIMD_CALL_CONVENTION Vector256Utility::ModAngles(ConstVector256 vector)
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
		Vector256 result = _mm_sub_ps(left, right);

		// -Piよりも小さいかを判定する
		Vector256 offset = _mm_cmplt_ps(result, VECTOR_128F_NEGATIVE_PI);
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

#endif
#endif