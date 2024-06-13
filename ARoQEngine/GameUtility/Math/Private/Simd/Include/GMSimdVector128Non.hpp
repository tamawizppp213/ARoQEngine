//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdVectorNonSimd.hpp
///             @brief  Simdを使わない演算
///             @author toide
///             @date   2024/03/11 2:26:07
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_VECTOR_128_NON_HPP
#define GM_SIMD_VECTOR_128_NON_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMSimdMacros.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)

#include "GameUtility/Base/Include/GUAssert.hpp"
#include <math.h>
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gm::simd::non
{
	struct __vector128
	{
		union
		{
			gu::float32 F32[4];
			gu::uint32  U32[4];
		};
	};
	using Vector128 = __vector128;

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
		inline operator const float*() const noexcept { return F;}
	};

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

		inline operator Vector128()    const noexcept { return V; }
	};

	#define IS_NAN(x)  ((*(const gu::uint32*)&(x) & 0x7F800000) == 0x7F800000 && (*(const gu::uint32*)&(x) & 0x7FFFFF) != 0)
	#define IS_INF(x)  ((*(const gu::uint32*)&(x) & 0x7FFFFFFF) == 0x7F800000)

	constexpr float GM_PI_FLOAT        = 3.141592654f;
	constexpr float GM_2PI_FLOAT       = 6.283185307f;
	constexpr float GM_1_DIV_PI_FLOAT  = 0.318309886f;
	constexpr float GM_1_DIV_2PI_FLOAT = 0.159154943f;
	constexpr float GM_PI_DIV2_FLOAT   = 1.570796327f;
	constexpr float GM_PI_DIV4_FLOAT   = 0.785398163f;
	constexpr Vector128f VECTOR_128F_ONE  = { { {  1.0f, 1.0f, 1.0f, 1.0f  } } };
	constexpr Vector128f VECTOR_128F_ZERO = { { {  0.0f, 0.0f, 0.0f, 0.0f  } } };
	constexpr Vector128f VECTOR_128F_HALF_PI        = { { {  GM_PI_DIV2_FLOAT  , GM_PI_DIV2_FLOAT  , GM_PI_DIV2_FLOAT , GM_PI_DIV2_FLOAT   } } };
	constexpr Vector128f VECTOR_128F_PI             = { { {  GM_PI_FLOAT       , GM_PI_FLOAT       , GM_PI_FLOAT , GM_PI_FLOAT  } } };
	constexpr Vector128f VECTOR_128F_NEGATIVE_PI    = { { {  -GM_PI_FLOAT       , -GM_PI_FLOAT       , -GM_PI_FLOAT , -GM_PI_FLOAT  } } };
	constexpr Vector128f VECTOR_128F_NEGATIVE_2PI   = { { {  -GM_2PI_FLOAT       , -GM_2PI_FLOAT       , -GM_2PI_FLOAT , -GM_2PI_FLOAT  } } };
	constexpr Vector128f VECTOR_128F_RECIPROCAL_PI  = { { {  GM_1_DIV_PI_FLOAT , GM_1_DIV_PI_FLOAT , GM_1_DIV_PI_FLOAT , GM_1_DIV_PI_FLOAT   } } };
	constexpr Vector128f VECTOR_128F_RECIPROCAL_2PI = { { {  GM_1_DIV_2PI_FLOAT, GM_1_DIV_2PI_FLOAT, GM_1_DIV_2PI_FLOAT, GM_1_DIV_2PI_FLOAT   } } };
	constexpr Vector128f VECTOR_128F_2PI            = { { { GM_2PI_FLOAT      , GM_2PI_FLOAT      , GM_2PI_FLOAT, GM_2PI_FLOAT} } };
	
	/****************************************************************************
	*				  			  Vector128Utility
	****************************************************************************/
	/* @class     Vector128Utility
	*  @brief     Simdを使わない演算
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
		__forceinline static Vector128 SIMD_CALL_CONVENTION LoadFloat(const float* source) noexcept;
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
		*  @brief : 2つのベクトルの各要素同士の逆数を算出します. (1.0f / vector)
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
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector2(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector3(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept;
		__forceinline static bool      SIMD_CALL_CONVENTION NearEqualVector4(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept;
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
		*  @brief : 1次ノルム(各要素の絶対値の和におけるベクトルの大きさ)を算出
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION Negate(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 1次ノルム(各要素の絶対値の和におけるベクトルの大きさ)を算出
		/*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION LengthVector2(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION LengthVector3(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION LengthVector4(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
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

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のsinH, cosH, tanHを返します
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION SinH(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION CosH(ConstVector128 vector) noexcept;
		__forceinline static Vector128 SIMD_CALL_CONVENTION TanH(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素に対するsin成分とcos成分を取り出します
		/*----------------------------------------------------------------------*/
		__forceinline static Vector128 SIMD_CALL_CONVENTION SinCos(ConstVector128 input, Vector128* sin, Vector128* cos) noexcept;

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
	#pragma region Setter
	/****************************************************************************
	*                       Zero
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Zero()noexcept
	*
	*  @brief     ゼロ初期化したVector128を返す関数
	*
	*  @param[in] void
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Zero() noexcept
	{
		Vector128f result = { { { 0.0f, 0.0f, 0.0f, 0.0f } } };
		return result.V;
	}

	/****************************************************************************
	*                      TrueIntMask
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::TrueIntMask() noexcept
	*
	*  @brief     全てのビットがtrue (1)に設定されているVector128を返す
	*
	*  @param[in] void
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::TrueIntMask() noexcept
	{
		Vector128f result = { { { 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU  } } };
		return result.V;
	}

	/****************************************************************************
	*                      FalseIntMask
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::FalseIntMask() noexcept
	*
	*  @brief     全てのビットがfalse (0)に設定されているVector128を返す
	*
	*  @param[in] void
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::FalseIntMask() noexcept
	{
		Vector128f result = { { { 0.0f, 0.0f, 0.0f, 0.0f  } } };
		return result.V;
	}

	/****************************************************************************
	*                       Set
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Set(const float x, const float y, const float z, const float w) noexcept
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
		Vector128f result = { { { x, y, z, w } } };
		return result.V;
	}

	/****************************************************************************
	*                       Set
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Set(const float value) noexcept
	*
	*  @brief     float1つからVector128クラスを生成する
	*
	*  @param[in] const float value
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Set(const float value) noexcept
	{
		Vector128f result = { { { value, value, value, value } } };
		return result.V;
	}

	/****************************************************************************
	*                       SetInt
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Set(const float x, const float y, const float z, const float w) noexcept
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
		Vector128f result = { { { x, y, z, w } } };
		return result.V;
	}

	/****************************************************************************
	*                       SetX
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION SetX(ConstVector128 vector, const float x) noexcept
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
		Vector128f result = { { { x, vector.F32[1], vector.F32[2], vector.F32[3] } } };
		return result.V;
	}

	/****************************************************************************
	*                       SetY
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION SetY(ConstVector128 vector, const float y) noexcept
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
		Vector128f result = { { { vector.F32[0], y, vector.F32[2], vector.F32[3]}} };
		return result.V;
	}

	/****************************************************************************
	*                       SetZ
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION SetZ(ConstVector128 vector, const float z) noexcept
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
		Vector128f result = { { { vector.F32[0], vector.F32[1], z, vector.F32[3]}} };
		return result.V;
	}

	/****************************************************************************
	*                       SetW
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION SetW(ConstVector128 vector, const float w) noexcept
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
		Vector128f result = { { { vector.F32[0], vector.F32[1], vector.F32[2], w}} };
		return result.V;
	}

	/****************************************************************************
	*                       StoreFloat
	****************************************************************************/
	*  @fn        inline void SIMD_CALL_CONVENTION Vector128Utility::StoreFloat(float* destination, ConstVector128 source) noexcept
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
		*destination = GetX(source);
	}

	/****************************************************************************
	*                       StoreFloat2
	****************************************************************************/
	*  @fn        inline void SIMD_CALL_CONVENTION Vector128Utility::StoreFloat2(float* destination, ConstVector128 source) noexcept
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
		destination[0] = source.F32[0];
		destination[1] = source.F32[1];
	}

	/****************************************************************************
	*                       StoreFloat3
	****************************************************************************/
	*  @fn        inline void SIMD_CALL_CONVENTION Vector128Utility::StoreFloat3(float* destination, ConstVector128 source) noexcept
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
		destination[0] = source.F32[0];
		destination[1] = source.F32[1];
		destination[2] = source.F32[2];
	}

	/****************************************************************************
	*                       StoreFloat4
	****************************************************************************/
	*  @fn        inline void SIMD_CALL_CONVENTION Vector128Utility::StoreFloat4(float* destination, ConstVector128 source) noexcept
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
		destination[0] = source.F32[0];
		destination[1] = source.F32[1];
		destination[2] = source.F32[2];
		destination[3] = source.F32[3];
	}

	/****************************************************************************
	*                       SplatX
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatX(ConstVector128 vector) noexcept
	*
	*  @brief     Xの要素に基づいて, 全ての要素に同じ値を持ったVector128クラスを作成する
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatX(ConstVector128 vector) noexcept
	{
		Vector128f result = { { { vector.F32[0], vector.F32[0], vector.F32[0],vector.F32[0]}} };
		return result.V;
	}

	/****************************************************************************
	*                       SplatY
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatY(ConstVector128 vector) noexcept
	*
	*  @brief     Yの要素に基づいて, 全ての要素に同じ値を持ったVector128クラスを作成する
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatY(ConstVector128 vector) noexcept
	{
		Vector128f result = { { { vector.F32[1], vector.F32[1], vector.F32[1],vector.F32[1]} } };
		return result.V;
	}

	/****************************************************************************
	*                       SplatZ
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatZ(ConstVector128 vector) noexcept
	*
	*  @brief     Zの要素に基づいて, 全ての要素に同じ値を持ったVector128クラスを作成する
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatZ(ConstVector128 vector) noexcept
	{
		Vector128f result = { { { vector.F32[2], vector.F32[2], vector.F32[2],vector.F32[2]} } };
		return result.V;
	}

	/****************************************************************************
	*                       SplatW
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatZ(ConstVector128 vector) noexcept
	*
	*  @brief     Wの要素に基づいて, 全ての要素に同じ値を持ったVector128クラスを作成する
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatW(ConstVector128 vector) noexcept
	{
		Vector128f result = { { { vector.F32[3], vector.F32[3], vector.F32[3],vector.F32[3]} } };
		return result.V;
	}

	/****************************************************************************
	*                       SplatOne
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatOne(ConstVector128 vector) noexcept
	*
	*  @brief     全ての要素に1を代入したVector128クラスを作成する
	*
	*  @param[in] void
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatOne() noexcept
	{
		Vector128f result = { { { 1.0f, 1.0f, 1.0f, 1.0f } } };
		return result.V;
	}

	/****************************************************************************
	*                       SplatInfinity
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatInfinity() noexcept
	*
	*  @brief     Wの要素に基づいて, 全ての要素にINFを持ったVector128クラスを作成する
	*
	*  @param[in] void
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatInfinity() noexcept
	{
		Vector128f result = { { { 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 } } };
		return result.V;
	}

	/****************************************************************************
	*                       SplatEpsilon
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatEpsilon() noexcept
	*
	*  @brief     Wの要素に基づいて, 全ての要素に1.192092896e-7fを持ったVector128クラスを作成する
	*
	*  @param[in] void
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatEpsilon() noexcept
	{
		Vector128f result = { { { 0x34000000, 0x34000000, 0x34000000, 0x34000000 } } };
		return result.V;
	}

	/****************************************************************************
	*                       SplatSignMask
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatSignMask() noexcept
	*
	*  @brief     Wの要素に基づいて, 全ての要素に-0.0f (0x80000000)を持ったVector128クラスを作成する
	*
	*  @param[in] void
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SplatSignMask() noexcept
	{
		Vector128f result = { { { 0x80000000U, 0x80000000U, 0x80000000U, 0x80000000U } } };
		return result.V;
	}

	/****************************************************************************
	*                       Swizzle
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Swizzle(ConstVector128 vector,
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
		Check((xIndex < 4) && (yIndex < 4) && (zIndex < 4) && (wIndex < 4));

		Vector128f result = { { { vector.F32[xIndex], vector.F32[yIndex], vector.F32[zIndex], vector.F32[wIndex]}}};
		return result.V;
	}

	/****************************************************************************
	*                       Permute
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Permute(ConstVector128 a, ConstVector128 b,
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
		Check((xIndex < 7) && (yIndex < 7) && (zIndex < 7) && (wIndex < 7));

		const gu::uint32* pointer[2];
		pointer[0] = reinterpret_cast<const gu::uint32*>(&a);
		pointer[1] = reinterpret_cast<const gu::uint32* > (&b);

		Vector128 result;
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
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Select(ConstVector128 left, ConstVector128 right, ConstVector128 control) noexcept
	*
	*  @brief     2つのベクトルから要素を抽出し, 新しいベクトルを作成する
	*             control vectorが0の場合はleftから, 非0の場合はrightからそれぞれ抽出が行われます
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Select(ConstVector128 left, ConstVector128 right, ConstVector128 control) noexcept
	{
		Vector128f result = { { { 
				(left.U32[0] & ~control.U32[0]) | (right.U32[0] & ~control.U32[0]),
				(left.U32[1] & ~control.U32[1]) | (right.U32[1] & ~control.U32[1]),
				(left.U32[2] & ~control.U32[2]) | (right.U32[2] & ~control.U32[2]),
				(left.U32[3] & ~control.U32[3]) | (right.U32[3] & ~control.U32[3]),
			} } };
		return result.V;
	}
	#pragma endregion Setter

	#pragma region Getter
	/****************************************************************************
	*                       GetX
	****************************************************************************/
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::GetX(ConstVector128 vector) noexcept
	*
	*  @brief     FPUレジスタに格納されたXの要素を取り出す
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::GetX(ConstVector128 vector) noexcept
	{
		return vector.F32[0];
	}

	/****************************************************************************
	*                       GetY
	****************************************************************************/
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::GetY(ConstVector128 vector) noexcept
	*
	*  @brief     FPUレジスタに格納されたYの要素を取り出す
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::GetY(ConstVector128 vector) noexcept
	{
		return vector.F32[1];
	}

	/****************************************************************************
	*                       GetZ
	****************************************************************************/
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::GetZ(ConstVector128 vector) noexcept
	*
	*  @brief     FPUレジスタに格納されたZの要素を取り出す
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::GetZ(ConstVector128 vector) noexcept
	{
		return vector.F32[2];
	}

	/****************************************************************************
	*                       GetW
	****************************************************************************/
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::GetZ(ConstVector128 vector) noexcept
	*
	*  @brief     FPUレジスタに格納されたWの要素を取り出す
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::GetW(ConstVector128 vector) noexcept
	{
		return vector.F32[3];
	}

	/****************************************************************************
	*                       LoadFloat
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LoadFloat(const float* source) noexcept
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
		Vector128f result = { { { source[0], 0.0f, 0.0f, 0.0f } } };
		return result.V;
	}

	/****************************************************************************
	*                       LoadFloat2
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LoadFloat2(const float* source) noexcept
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
		Vector128f result = { { { source[0], source[1], 0.0f, 0.0f}}};
		return result.V;
	}

	/****************************************************************************
	*                       LoadFloat3
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LoadFloat3(const float* source) noexcept
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
		Vector128f result = { { { source[0], source[1], source[2], 0.0f}}};
		return result.V;
	}

	/****************************************************************************
	*                       LoadFloat3
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LoadFloat3(const float* source) noexcept
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
		Vector128f result = { { { source[0], source[1], source[2], source[3]}}};
		return result.V;
	}

	#pragma endregion Getter

	#pragma region Operator
	/****************************************************************************
	*                       Add
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Add(ConstVector128 left, ConstVector128 right) noexcept
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
		Vector128f result =
		{ { {
			left.F32[0] + right.F32[0],
			left.F32[1] + right.F32[1],
			left.F32[2] + right.F32[2],
			left.F32[3] + right.F32[3]
		} } };
		return result.V;
	}

	/****************************************************************************
	*                       Subtract
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Subtract(ConstVector128 left, ConstVector128 right) noexcept
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
		Vector128f result =
		{ { {
			left.F32[0] - right.F32[0],
			left.F32[1] - right.F32[1],
			left.F32[2] - right.F32[2],
			left.F32[3] - right.F32[3]
		} } };
		return result.V;
	}

	/****************************************************************************
	*                       Multiply
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Multiply(ConstVector128 left, ConstVector128 right) noexcept
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
		Vector128f result =
		{ { {
			left.F32[0] * right.F32[0],
			left.F32[1] * right.F32[1],
			left.F32[2] * right.F32[2],
			left.F32[3] * right.F32[3]
		} } };
		return result.V;
	}

	/****************************************************************************
	*                       Divide
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Divide(ConstVector128 left, ConstVector128 right) noexcept
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
		Vector128f result =
		{ { {
			left.F32[0] / right.F32[0],
			left.F32[1] / right.F32[1],
			left.F32[2] / right.F32[2],
			left.F32[3] / right.F32[3]
		} } };
		return result.V;
	}

	/****************************************************************************
	*                       Mod
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Mod(ConstVector128 left, ConstVector128 right) noexcept
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
		Vector128 quotient = Divide(left, right);
		quotient = Truncate(quotient);

		return NegativeMultiplySubtract(right, quotient, left); // left - result * right
	}

	/****************************************************************************
	*                      NegativeMultiplySubstract
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NegativeMultiplySubtract(ConstVector128 first, ConstVector128 second, ConstVector128 third)
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
		Vector128f result =
		{ { {
			subtractLeft.F32[0] - (multiplyLeft.F32[0] * multiplyRight.F32[0]),
			subtractLeft.F32[1] - (multiplyLeft.F32[1] * multiplyRight.F32[1]),
			subtractLeft.F32[2] - (multiplyLeft.F32[2] * multiplyRight.F32[2]),
			subtractLeft.F32[3] - (multiplyLeft.F32[3] * multiplyRight.F32[3])
		} } };
		return result.V;
	}

	/****************************************************************************
	*                      MultiplyAdd
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::MultiplyAdd(ConstVector128 first, ConstVector128 second, ConstVector128 third)
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
		Vector128f result =
		{ { {
			add.F32[0] + (multiplyLeft.F32[0] * multiplyRight.F32[0]),
			add.F32[1] + (multiplyLeft.F32[1] * multiplyRight.F32[1]),
			add.F32[2] + (multiplyLeft.F32[2] * multiplyRight.F32[2]),
			add.F32[3] + (multiplyLeft.F32[3] * multiplyRight.F32[3])
		} } };
		return result.V;
	}

	/****************************************************************************
	*                       Reciprocal
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Reciprocal(ConstVector128 vector) noexcept
	*
	*  @brief     2つのベクトルの各要素同士の逆数を算出します. (1.0f / vector)
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Reciprocal(ConstVector128 vector) noexcept
	{
		Vector128f result =
		{ { {
			1.0f / vector.F32[0],
			1.0f / vector.F32[1],
			1.0f / vector.F32[2],
			1.0f / vector.F32[3],
		} } };
		return result.V;
	}

	/****************************************************************************
	*                       Scale
	****************************************************************************/
	*  @fn       inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Scale(ConstVector128 vector, const float scale) noexcept
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
		Vector128f result =
		{ { {
			vector.F32[0] * scale,
			vector.F32[1] * scale,
			vector.F32[2] * scale,
			vector.F32[3] * scale
		} } };
		return result.V;
	}

	/****************************************************************************
	*                       EqualVector2
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualVector2(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.F32[0] == right.F32[0]) && (left.F32[1] == right.F32[1]);
	}

	/****************************************************************************
	*                       EqualVector3
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualVector3(ConstVector128 left, ConstVector128 right) noexcept
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
		return(left.F32[0] == right.F32[0]) && (left.F32[1] == right.F32[1]) && (left.F32[2] == right.F32[2]);
	}

	/****************************************************************************
	*                       EqualVector4
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualVector4(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.F32[0] == right.F32[0]) && (left.F32[1] == right.F32[1]) && (left.F32[2] == right.F32[2]) && (left.F32[3] == right.F32[3]);
	}

	/****************************************************************************
	*                       EqualVectorEach
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept
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
		Vector128u result = { { {
				(left.F32[0] == right.F32[0]) ? 0xFFFFFFFF : 0,
				(left.F32[1] == right.F32[1]) ? 0xFFFFFFFF : 0,
				(left.F32[2] == right.F32[2]) ? 0xFFFFFFFF : 0,
				(left.F32[3] == right.F32[3]) ? 0xFFFFFFFF : 0
			} } };
		return result.V;
	}

	/****************************************************************************
	*                       NotEqualVector2
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector2(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.F32[0] != right.F32[0]) || (left.F32[1] != right.F32[1]);
	}

	/****************************************************************************
	*                       NotEqualVector3
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector3(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.F32[0] != right.F32[0]) || (left.F32[1] != right.F32[1]) || (left.F32[2] != right.F32[2]);
	}

	/****************************************************************************
	*                       NotEqualVector4
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector4(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.F32[0] != right.F32[0]) || (left.F32[1] != right.F32[1]) || (left.F32[2] != right.F32[2]) || (left.F32[3] != right.F32[3]);
	}

	/****************************************************************************
	*                       NotEqualVectorEach
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept
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
		Vector128u result = { { {
				(left.F32[0] != right.F32[0]) ? 0xFFFFFFFF : 0,
				(left.F32[1] != right.F32[1]) ? 0xFFFFFFFF : 0,
				(left.F32[2] != right.F32[2]) ? 0xFFFFFFFF : 0,
				(left.F32[3] != right.F32[3]) ? 0xFFFFFFFF : 0
			} } };
		return result.V;
	}

	/****************************************************************************
	*                       EqualAsIntVector2
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualAsIntVector2(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.U32[0] == right.U32[0]) && (left.U32[1] == right.U32[1]);
	}

	/****************************************************************************
	*                       EqualAsIntVector3
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualAsIntVector3(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.U32[0] == right.U32[0]) && (left.U32[1] == right.U32[1]) && (left.U32[2] == right.U32[2]);
	}

	/****************************************************************************
	*                       EqualAsIntVector4
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualAsIntVector4(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.U32[0] == right.U32[0]) && (left.U32[1] == right.U32[1]) && (left.U32[2] == right.U32[2]) && (left.U32[3] == right.U32[3]);
	}

	/****************************************************************************
	*                       EqualAsIntVectorEach
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::EqualAsIntVectorEach(ConstVector128 left, ConstVector128 right) noexcept
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
		Vector128u result = { { {
				(left.U32[0] == right.U32[0]) ? 0xFFFFFFFF : 0,
				(left.U32[1] == right.U32[1]) ? 0xFFFFFFFF : 0,
				(left.U32[2] == right.U32[2]) ? 0xFFFFFFFF : 0,
				(left.U32[3] == right.U32[3]) ? 0xFFFFFFFF : 0
			} } };
		return result.V;
	}

	/****************************************************************************
	*                       NotEqualVector2
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualAsIntVector2(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.U32[0] != right.U32[0]) || (left.U32[1] != right.U32[1]);
	}

	/****************************************************************************
	*                       NotEqualVector3
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualAsIntVector3(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.U32[0] != right.U32[0]) || (left.U32[1] != right.U32[1]) || (left.U32[2] != right.U32[2]);
	}

	/****************************************************************************
	*                       NotEqualVector4
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector4(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.U32[0] != right.U32[0]) || (left.U32[1] != right.U32[1]) || (left.U32[2] != right.U32[2]) || (left.U32[3] != right.U32[3]);
	}

	/****************************************************************************
	*                       NotEqualVector
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector(ConstVector128 left, ConstVector128 right) noexcept
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
		Vector128u result = { { {
				(left.U32[0] != right.U32[0]) ? 0xFFFFFFFF : 0,
				(left.U32[1] != right.U32[1]) ? 0xFFFFFFFF : 0,
				(left.U32[2] != right.U32[2]) ? 0xFFFFFFFF : 0,
				(left.U32[3] != right.U32[3]) ? 0xFFFFFFFF : 0
			} } };
		return result.V;
	}

	/****************************************************************************
	*                       NearEqualVector2
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector4(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept
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
		const float deltaX = fabsf(left.F32[0] - right.F32[0]);
		const float deltaY = fabsf(left.F32[1] - right.F32[1]);
		return ((deltaX <= epsilon.F32[0]) && (deltaY <= epsilon.F32[1]));
	}

	/****************************************************************************
	*                       NearEqualVector3
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector3(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept
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
		const float deltaX = fabsf(left.F32[0] - right.F32[0]);
		const float deltaY = fabsf(left.F32[1] - right.F32[1]);
		const float deltaZ = fabsf(left.F32[2] - right.F32[2]);
		return ((deltaX <= epsilon.F32[0]) && (deltaY <= epsilon.F32[1]) && (deltaZ <= epsilon.F32[2]));
	}

	/****************************************************************************
	*                       NearEqualVector4
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVector4(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept
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
		const float deltaX = fabsf(left.F32[0] - right.F32[0]);
		const float deltaY = fabsf(left.F32[1] - right.F32[1]);
		const float deltaZ = fabsf(left.F32[2] - right.F32[2]);
		const float deltaW = fabsf(left.F32[3] - right.F32[3]);
		return ((deltaX <= epsilon.F32[0]) && (deltaY <= epsilon.F32[1]) && (deltaZ <= epsilon.F32[2]) && (deltaW <= epsilon.F32[3]));
	}

	/****************************************************************************
	*                       NearEqualVectorEach
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::NotEqualVectorEach(ConstVector128 left, ConstVector128 right, ConstVector128 epsilon) noexcept
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
		const float deltaX = fabsf(left.F32[0] - right.F32[0]);
		const float deltaY = fabsf(left.F32[1] - right.F32[1]);
		const float deltaZ = fabsf(left.F32[2] - right.F32[2]);
		const float deltaW = fabsf(left.F32[3] - right.F32[3]);

		Vector128u control = { { {
				deltaX <= epsilon.F32[0] ? 0xFFFFFFFFU : 0,
				deltaY <= epsilon.F32[1] ? 0xFFFFFFFFU : 0,
				deltaZ <= epsilon.F32[2] ? 0xFFFFFFFFU : 0,
				deltaW <= epsilon.F32[3] ? 0xFFFFFFFFU : 0
			} } };

		return control.V;
	}

	/****************************************************************************
	*                       GreaterVector2
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterVector2(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.F32[0] > right.F32[0]) && (left.F32[1] > right.F32[1]);
	}

	/****************************************************************************
	*                       GreaterVector3
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterVector3(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.F32[0] > right.F32[0]) && (left.F32[1] > right.F32[1]) && (left.F32[2] > right.F32[2]);
	}

	/****************************************************************************
	*                       GreaterVector4
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterVector4(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.F32[0] > right.F32[0]) && (left.F32[1] > right.F32[1]) && (left.F32[2] > right.F32[2]) && (left.F32[3] > right.F32[3]);
	}

	/****************************************************************************
	*                       GreaterVectorEach
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::GreaterVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector4型において左の方が全ての要素において大きいかを調べます (left > right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::GreaterVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	{
		Vector128u control = { { {
				left.F32[0] > right.F32[0] ? 0xFFFFFFFFU : 0,
				left.F32[1] > right.F32[1] ? 0xFFFFFFFFU : 0,
				left.F32[2] > right.F32[2] ? 0xFFFFFFFFU : 0,
				left.F32[3] > right.F32[3] ? 0xFFFFFFFFU : 0
			} } };

		return control.V;
	}


	/****************************************************************************
	*                       GreaterOrEqualVector2
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterOrEqualVector2(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.F32[0] >= right.F32[0]) && (left.F32[1] >= right.F32[1]);
	}

	/****************************************************************************
	*                       GreaterVector3
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterOrEqualVector3(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.F32[0] >= right.F32[0]) && (left.F32[1] >= right.F32[1]) && (left.F32[2] >= right.F32[2]);
	}

	/****************************************************************************
	*                       GreaterOrEqualVector4
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::GreaterOrEqualVector4(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.F32[0] >= right.F32[0]) && (left.F32[1] >= right.F32[1]) && (left.F32[2] >= right.F32[2]) && (left.F32[3] >= right.F32[3]);
	}

	/****************************************************************************
	*                       GreaterOrEqualVectorEach
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::GreaterOrEqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector4型において左の方が全ての要素において大きいor等しいかを調べます (left >= right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::GreaterOrEqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	{
		Vector128u control = { { {
				left.F32[0] >= right.F32[0] ? 0xFFFFFFFFU : 0,
				left.F32[1] >= right.F32[1] ? 0xFFFFFFFFU : 0,
				left.F32[2] >= right.F32[2] ? 0xFFFFFFFFU : 0,
				left.F32[3] >= right.F32[3] ? 0xFFFFFFFFU : 0
			} } };

		return control.V;
	}


	/****************************************************************************
	*                       LessVector2
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::LessVector2(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.F32[0] < right.F32[0]) && (left.F32[1] < right.F32[1]);
	}

	/****************************************************************************
	*                       LessVector3
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::LessVector3(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.F32[0] < right.F32[0]) && (left.F32[1] < right.F32[1]) && (left.F32[2] < right.F32[2]);
	}

	/****************************************************************************
	*                       LessVector4
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::LessVector4(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.F32[0] < right.F32[0]) && (left.F32[1] < right.F32[1]) && (left.F32[2] < right.F32[2]) && (left.F32[3] < right.F32[3]);
	}

	/****************************************************************************
	*                       LessVectorEach
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LessOrEqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	*
	*  @brief     Vector4型において左の方が全ての要素において小さいor等しいかを調べます (left <= right)
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　bool
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LessVectorEach(ConstVector128 left, ConstVector128 right) noexcept
	{
		Vector128u control = { { {
				left.F32[0] < right.F32[0] ? 0xFFFFFFFFU : 0,
				left.F32[1] < right.F32[1] ? 0xFFFFFFFFU : 0,
				left.F32[2] < right.F32[2] ? 0xFFFFFFFFU : 0,
				left.F32[3] < right.F32[3] ? 0xFFFFFFFFU : 0
			} } };

		return control.V;
	}


	/****************************************************************************
	*                       LessOrEqualVector2
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::LessOrEqualVector2(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.F32[0] <= right.F32[0]) && (left.F32[1] <= right.F32[1]);
	}

	/****************************************************************************
	*                       LessOrEqualVector3
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::LessOrEqualVector3(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.F32[0] <= right.F32[0]) && (left.F32[1] <= right.F32[1]) && (left.F32[2] <= right.F32[2]);
	}

	/****************************************************************************
	*                       LessOrEqualVector4
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::LessOrEqualVector4(ConstVector128 left, ConstVector128 right) noexcept
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
		return (left.F32[0] <= right.F32[0]) && (left.F32[1] <= right.F32[1]) && (left.F32[2] <= right.F32[2]) && (left.F32[3] <= right.F32[3]);
	}

	/****************************************************************************
	*                       LessVectorEach
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LessOrEqualVectorEach(ConstVector128 left, ConstVector128 right) noexcept
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
		Vector128u control = { { {
				left.F32[0] <= right.F32[0] ? 0xFFFFFFFFU : 0,
				left.F32[1] <= right.F32[1] ? 0xFFFFFFFFU : 0,
				left.F32[2] <= right.F32[2] ? 0xFFFFFFFFU : 0,
				left.F32[3] <= right.F32[3] ? 0xFFFFFFFFU : 0
			} } };

		return control.V;
	}

	/****************************************************************************
	*                       InBoundsVector2
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::InBoundsVector2(ConstVector128 vector, ConstVector128 bounds) noexcept
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
		return (
			(vector.F32[0] <= bounds.F32[0] && vector.F32[0] >= -bounds.F32[0]) &&
			(vector.F32[1] <= bounds.F32[1] && vector.F32[1] >= -bounds.F32[1]));
	}

	/****************************************************************************
	*                       InBoundsVector3
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::InBoundsVector3(ConstVector128 vector, ConstVector128 bounds) noexcept
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
		return (
			(vector.F32[0] <= bounds.F32[0] && vector.F32[0] >= -bounds.F32[0]) &&
			(vector.F32[1] <= bounds.F32[1] && vector.F32[1] >= -bounds.F32[1]) &&
			(vector.F32[2] <= bounds.F32[2] && vector.F32[2] >= -bounds.F32[2]));

	}

	/****************************************************************************
	*                       InBoundsVector4
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::InBoundsVector4(ConstVector128 vector, ConstVector128 bounds) noexcept
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
		return (
			(vector.F32[0] <= bounds.F32[0] && vector.F32[0] >= -bounds.F32[0]) &&
			(vector.F32[1] <= bounds.F32[1] && vector.F32[1] >= -bounds.F32[1]) &&
			(vector.F32[2] <= bounds.F32[2] && vector.F32[2] >= -bounds.F32[2]) &&
			(vector.F32[3] <= bounds.F32[3] && vector.F32[3] >= -bounds.F32[3]));
	}

	/****************************************************************************
	*                       InBoundsVectorEach
	****************************************************************************/
	*  @fn        inline bool SIMD_CALL_CONVENTION Vector128Utility::InBoundsVectorEach(ConstVector128 vector, ConstVector128 bounds) noexcept
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
		Vector128u control = { { {
			(vector.F32[0] <= bounds.F32[0] && vector.F32[0] >= -bounds.F32[0]) ? 0xFFFFFFFF : 0,
			(vector.F32[1] <= bounds.F32[1] && vector.F32[1] >= -bounds.F32[1]) ? 0xFFFFFFFF : 0,
			(vector.F32[2] <= bounds.F32[2] && vector.F32[2] >= -bounds.F32[2]) ? 0xFFFFFFFF : 0,
			(vector.F32[3] <= bounds.F32[3] && vector.F32[3] >= -bounds.F32[3]) ? 0xFFFFFFFF : 0,
		} } };
		return control.V;
	}

	#pragma endregion Operator

	#pragma region Bit
	/****************************************************************************
	*                       AndInt
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::AndInt(ConstVector128 left, ConstVector128 right) noexcept
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
		Vector128u result = { { {
		   left.U32[0] & right.U32[0],
		   left.U32[1] & right.U32[1],
		   left.U32[2] & right.U32[2],
		   left.U32[3] & right.U32[3]
	   } } };
		return result.V;
	}

	/****************************************************************************
	*                        OrInt
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::AndInt(ConstVector128 left, ConstVector128 right) noexcept
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
		Vector128u result = { { {
		   left.U32[0] | right.U32[0],
		   left.U32[1] | right.U32[1],
		   left.U32[2] | right.U32[2],
		   left.U32[3] | right.U32[3]
	   } } };
		return result.V;
	}

	/****************************************************************************
	*                        NorInt
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NorInt(ConstVector128 left, ConstVector128 right) noexcept
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
		Vector128u result = { { {
		   ~(left.U32[0] | right.U32[0]),
		   ~(left.U32[1] & right.U32[1]),
		   ~(left.U32[2] & right.U32[2]),
		   ~(left.U32[3] & right.U32[3])
	   } } };
		return result.V;
	}

	/****************************************************************************
	*                        XorInt
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::AndInt(ConstVector128 left, ConstVector128 right) noexcept
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
		Vector128u result = { { {
		   left.U32[0] ^ right.U32[0],
		   left.U32[1] ^ right.U32[1],
		   left.U32[2] ^ right.U32[2],
		   left.U32[3] ^ right.U32[3]
	   } } };
		return result.V;
	}

	#pragma endregion Bit

	#pragma region Math
	/****************************************************************************
	*                       Negate
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Negate(ConstVector128 vector) noexcept
	*
	*  @brief     Vectorの全ての要素の符号を判定します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Negate(ConstVector128 vector) noexcept
	{
		Vector128f Result = { { {
			-vector.F32[0],
			-vector.F32[1],
			-vector.F32[2],
			-vector.F32[3],
		} } };
		return Result.V;
	}

	/****************************************************************************
	*                      LengthVector2
	****************************************************************************/
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::LengthVector2(ConstVector128 vector) noexcept
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
		return sqrtf(LengthSquaredVector2(vector));
	}

	/****************************************************************************
	*                      LengthVector3
	****************************************************************************/
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::LengthVector3(ConstVector128 vector) noexcept
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
		return sqrtf(LengthSquaredVector3(vector));
	}

	/****************************************************************************
	*                      LengthVector4
	****************************************************************************/
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::LengthVector4(ConstVector128 vector) noexcept
	*
	*  @brief    2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::LengthVector4(ConstVector128 vector) noexcept
	{
		return sqrtf(LengthSquaredVector4(vector));
	}

	/****************************************************************************
	*                      LengthSquaredVector2
	****************************************************************************/
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::LengthSquaredVector2(ConstVector128 vector) noexcept
	*
	*  @brief     2次ノルムの二乗を算出
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
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::LengthSquaredVector3(ConstVector128 vector) noexcept
	*
	*  @brief     2次ノルムの二乗を算出
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
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::LengthSquaredVector4(ConstVector128 vector) noexcept
	*
	*  @brief     2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
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
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector2(ConstVector128 left, ConstVector128 right)
	*
	*  @brief     内積を算出
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector2(ConstVector128 left, ConstVector128 right)
	{
		return left.F32[0] * right.F32[0] + left.F32[1] * right.F32[1];
	}

	/****************************************************************************
	*                      DotVector3
	****************************************************************************/
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector3(ConstVector128 left, ConstVector128 right)
	*
	*  @brief     内積を算出
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector3(ConstVector128 left, ConstVector128 right)
	{
		return left.F32[0] * right.F32[0] + left.F32[1] * right.F32[1] + left.F32[2] * right.F32[2];
	}

	/****************************************************************************
	*                      DotVector4
	****************************************************************************/
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector4(ConstVector128 left, ConstVector128 right)
	*
	*  @brief     内積を算出
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector4(ConstVector128 left, ConstVector128 right)
	{
		return left.F32[0] * right.F32[0] + left.F32[1] * right.F32[1] + left.F32[2] * right.F32[2] + left.F32[3] * right.F32[3];
	}

	/****************************************************************************
	*                      CrossVector2
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CrossVector2(ConstVector128 left, ConstVector128 right)
	*
	*  @brief     平行四辺形の面積を算出する
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　Vector128ではあるが, 実質float
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CrossVector2(ConstVector128 left, ConstVector128 right)
	{
		// [ left.x*right.y - left.y*right.x, left.x*right.y - left.y*right.x ]
		return Set((left.F32[0] * right.F32[1]) - (left.F32[1] * right.F32[0]));
	}

	/****************************************************************************
	*                      CrossVector3
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CrossVector3(ConstVector128 left, ConstVector128 right) noexcept
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
		
		Vector128f result = { { {
			(left.F32[1] * right.F32[2]) - (left.F32[2] * right.F32[1]),
			(left.F32[2] * right.F32[0]) - (left.F32[0] * right.F32[2]),
			(left.F32[0] * right.F32[1]) - (left.F32[1] * right.F32[0]),
			0.0f
		} } };
		return result.V;
	}

	/****************************************************************************
	*                      CrossVector4
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CrossVector4(ConstVector128 left, ConstVector128 right) noexcept
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

		Vector128f result = { { {
			(((second.F32[2] * third.F32[3]) - (second.F32[3] * third.F32[2])) * first.F32[1]) - (((second.F32[1] * third.F32[3]) - (second.F32[3] * third.F32[1])) * first.F32[2]) + (((second.F32[1] * third.F32[2]) - (second.F32[2] * third.F32[1])) * first.F32[3]),
			(((second.F32[3] * third.F32[2]) - (second.F32[2] * third.F32[3])) * first.F32[0]) - (((second.F32[3] * third.F32[0]) - (second.F32[0] * third.F32[3])) * first.F32[2]) + (((second.F32[2] * third.F32[0]) - (second.F32[0] * third.F32[2])) * first.F32[3]),
			(((second.F32[1] * third.F32[3]) - (second.F32[3] * third.F32[1])) * first.F32[0]) - (((second.F32[0] * third.F32[3]) - (second.F32[3] * third.F32[0])) * first.F32[1]) + (((second.F32[0] * third.F32[1]) - (second.F32[1] * third.F32[0])) * first.F32[3]),
			(((second.F32[2] * third.F32[1]) - (second.F32[1] * third.F32[2])) * first.F32[0]) - (((second.F32[2] * third.F32[0]) - (second.F32[0] * third.F32[2])) * first.F32[1]) + (((second.F32[1] * third.F32[0]) - (second.F32[0] * third.F32[1])) * first.F32[2]),
		} } };
		return result.V;
	}

	/****************************************************************************
	*                      NormalizeVector2
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector2(ConstVector128 vector) noexcept
	*
	*  @brief     入力ベクトルの単位ベクトルを返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector2(ConstVector128 vector) noexcept
	{
		const float norm = NormVector2(vector);
		const float reciprocalNorm = norm > 0 ? 1.0f / norm : 0.0f;

		Vector128f result = { { {
				vector.F32[0] * reciprocalNorm,
				vector.F32[1] * reciprocalNorm,
				vector.F32[2] * reciprocalNorm,
				vector.F32[3] * reciprocalNorm
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      NormalizeVector3
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector3(ConstVector128 vector) noexcept
	*
	*  @brief     入力ベクトルの単位ベクトルを返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector3(ConstVector128 vector) noexcept
	{
		const float norm = NormVector3(vector);
		const float reciprocalNorm = norm > 0 ? 1.0f / norm : 0.0f;

		Vector128f result = { { {
				vector.F32[0] * reciprocalNorm,
				vector.F32[1] * reciprocalNorm,
				vector.F32[2] * reciprocalNorm,
				vector.F32[3] * reciprocalNorm
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      NormalizeVector4
	****************************************************************************/ 
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector4(ConstVector128 vector) noexcept
	*
	*  @brief     入力ベクトルの単位ベクトルを返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector4(ConstVector128 vector) noexcept
	{
		const float norm = NormVector4(vector);
		const float reciprocalNorm = norm > 0 ? 1.0f / norm : 0.0f;

		Vector128f result = { { {
				vector.F32[0] * reciprocalNorm,
				vector.F32[1] * reciprocalNorm,
				vector.F32[2] * reciprocalNorm,
				vector.F32[3] * reciprocalNorm
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      ReflectVector2
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ReflectVector2(ConstVector128 input, ConstVector128 normal) noexcept
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
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ReflectVector4(ConstVector128 input, ConstVector128 normal) noexcept
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
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ReflectVector4(ConstVector128 input, ConstVector128 normal)
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
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RefractVector2(ConstVector128 input, ConstVector128 normal, const float refractionIndex)
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
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RefractVector3(ConstVector128 input, ConstVector128 normal, const float refractionIndex)
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
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RefractVector4(ConstVector128 input, ConstVector128 normal, const float refractionIndex)
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
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RefractVector2(ConstVector128 input, ConstVector128 normal, const float refractionIndex)
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

		const float inputDotNormal = (input.F32[0] * normal.F32[0]) + (input.F32[1] * normal.F32[1]);
		// R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
		float refractionY = 1.0f - (inputDotNormal * inputDotNormal);
		float refractionX = 1.0f - (refractionY * refractionIndex.F32[0] * refractionIndex.F32[0]);
		refractionY = 1.0f - (refractionY * refractionIndex.F32[1] * refractionIndex.F32[1]);
		if (refractionX >= 0.0f)
		{
			refractionX = (refractionIndex.F32[0] * input.F32[0]) - (normal.F32[0] * ((refractionIndex.F32[0] * inputDotNormal) + sqrtf(refractionX)));
		}
		else
		{
			refractionX = 0.0f;
		}
		if (refractionY >= 0.0f)
		{
			refractionY = (refractionIndex.F32[1] * input.F32[1]) - (normal.F32[1] * ((refractionIndex.F32[1] * inputDotNormal) + sqrtf(refractionY)));
		}
		else
		{
			refractionY = 0.0f;
		}

		Vector128 result = {};
		result.F32[0] = refractionX;
		result.F32[1] = refractionY;
		result.F32[2] = 0.0f;
		result.F32[3] = 0.0f;
		return result;
	}

	/****************************************************************************
	*                      RefractVector3
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RefractVector3(ConstVector128 input, ConstVector128 normal, const float refractionIndex)
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
		Vector128 r  = NegativeMultiplySubtract(inputDotNormal, inputDotNormal, VECTOR_128F_ONE.V);
		r = Multiply(r, refractionIndex);
		r = NegativeMultiplySubtract(r, refractionIndex, VECTOR_128F_ONE.V);

		Vector128 result = {};
		if (LessOrEqualVector3(r, VECTOR_128F_ZERO.V))
		{
			// Total internal reflection
			result = VECTOR_128F_ZERO;
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
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::RefractVector4(ConstVector128 input, ConstVector128 normal, const float refractionIndex)
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
		Vector128 r  = NegativeMultiplySubtract(inputDotNormal, inputDotNormal, VECTOR_128F_ONE.V);
		r = Multiply(r, refractionIndex);
		r = NegativeMultiplySubtract(r, refractionIndex, VECTOR_128F_ONE.V);

		Vector128 result = {};

		if (LessOrEqualVector4(r, VECTOR_128F_ZERO))
		{
			// Total internal reflection
			result = VECTOR_128F_ZERO;
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
	*                      IsInfiniteVector2
	****************************************************************************/
	*  @fn        inline bool Vector128Utility::IsInfiniteVector2(ConstVector128 vector) noexcept
	*
	*  @brief     非常に大きい値か
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline bool Vector128Utility::IsInfiniteVector2(ConstVector128 vector) noexcept
	{
		return IS_INF(vector.F32[0]) || IS_INF(vector.F32[1]);
	}

	/****************************************************************************
	*                      IsInfiniteVector3
	****************************************************************************/
	*  @fn        inline bool Vector128Utility::IsInfiniteVector3(ConstVector128 vector) noexcept
	*
	*  @brief     非常に大きい値か
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline bool Vector128Utility::IsInfiniteVector3(ConstVector128 vector) noexcept
	{
		return IS_INF(vector.F32[0]) || IS_INF(vector.F32[1]) || IS_INF(vector.F32[2]);
	}

	/****************************************************************************
	*                      IsInfiniteVector4
	****************************************************************************/
	*  @fn        inline bool Vector128Utility::IsInfiniteVector4(ConstVector128 vector) noexcept
	*
	*  @brief     非常に大きい値か
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline bool Vector128Utility::IsInfiniteVector4(ConstVector128 vector) noexcept
	{
		return IS_INF(vector.F32[0]) || IS_INF(vector.F32[1]) || IS_INF(vector.F32[2]) || IS_INF(vector.F32[3]);
	}

	/****************************************************************************
	*                      Sqrt
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Sqrt(ConstVector128 vector) noexcept
	*
	*  @brief      全ての要素に根号を取ったものを返します
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Sqrt(ConstVector128 vector) noexcept
	{
		Vector128f result = { { {
				sqrtf(vector.F32[0]),
				sqrtf(vector.F32[1]),
				sqrtf(vector.F32[2]),
				sqrtf(vector.F32[3]),
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      ReciprocalSqrt
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ReciprocalSqrt(ConstVector128 vector) noexcept
	*
	*  @brief       全ての要素の逆数を取り, それに根号を取ったものを返します
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ReciprocalSqrt(ConstVector128 vector) noexcept
	{
		Vector128f result = { { {
				1.0f / sqrtf(vector.F32[0]),
				1.0f / sqrtf(vector.F32[1]),
				1.0f / sqrtf(vector.F32[2]),
				1.0f / sqrtf(vector.F32[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      Exp10
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Exp10(ConstVector128 vector) noexcept
	*
	*  @brief     全ての要素の2^{vector}を返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Exp2(ConstVector128 vector) noexcept
	{
		Vector128f result = { { {
				powf(2.0f, vector.F32[0]),
				powf(2.0f, vector.F32[1]),
				powf(2.0f, vector.F32[2]),
				powf(2.0f, vector.F32[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      Exp10
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Exp10(ConstVector128 vector) noexcept
	*
	*  @brief      全ての要素の10^{vector}を返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Exp10(ConstVector128 vector) noexcept
	{
		Vector128f result = { { {
				powf(10.0f, vector.F32[0]),
				powf(10.0f, vector.F32[1]),
				powf(10.0f, vector.F32[2]),
				powf(10.0f, vector.F32[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      ExpE
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ExpE(ConstVector128 vector) noexcept
	*
	*  @brief     全ての要素のe^{vector}を返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ExpE(ConstVector128 vector) noexcept
	{
		Vector128f result = { { {
				expf(vector.F32[0]),
				expf(vector.F32[1]),
				expf(vector.F32[2]),
				expf(vector.F32[3])
			} } };

		return result.V;
	}
	/****************************************************************************
	*                      Log2
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Log2(ConstVector128 vector) noexcept
	*
	*  @brief     全ての要素のlog2{vector}を返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Log2(ConstVector128 vector) noexcept
	{
		const float scale = 1.4426950f; // (1.0f / logf(2.0f));

		Vector128f result = { { {
				logf(vector.F32[0]),
				logf(vector.F32[1]),
				logf(vector.F32[2]),
				logf(vector.F32[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      Log10
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Log10(ConstVector128 vector) noexcept
	*
	*  @brief     全ての要素のlog10{vector}を返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Log10(ConstVector128 vector) noexcept
	{
		Vector128f result = { { {
				log10f(vector.F32[0]),
				log10f(vector.F32[1]),
				log10f(vector.F32[2]),
				log10f(vector.F32[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      LogE
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LogE(ConstVector128 vector) noexcept
	*
	*  @brief     全ての要素のloge{vector}を返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LogE(ConstVector128 vector) noexcept
	{
		Vector128f result = { { {
				logf(vector.F32[0]),
				logf(vector.F32[1]),
				logf(vector.F32[2]),
				logf(vector.F32[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      Pow
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Pow(ConstVector128 base, ConstVector128 power)
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
		Vector128f result = { { {
				powf(base.F32[0], power.F32[0]),
				powf(base.F32[1], power.F32[1]),
				powf(base.F32[2], power.F32[2]),
				powf(base.F32[3], power.F32[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      Sin
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Sin(ConstVector128 vector) noexcept
	*
	*  @brief     全ての要素にSinを取ったものを返します[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Sin(ConstVector128 vector) noexcept
	{
		Vector128f result = { { {
				sinf(vector.F32[0]),
				sinf(vector.F32[1]),
				sinf(vector.F32[2]),
				sinf(vector.F32[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      Cos
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Cos(ConstVector128 vector) noexcepts
	*
	*  @brief     全ての要素にCosを取ったものを返します
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Cos(ConstVector128 vector) noexcept
	{
		Vector128f result = { { {
				cosf(vector.F32[0]),
				cosf(vector.F32[1]),
				cosf(vector.F32[2]),
				cosf(vector.F32[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      Tan
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Tan(ConstVector128 vector) noexcept
	*
	*  @brief     全ての要素にTanを取ったものを返します
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Tan(ConstVector128 vector) noexcept
	{
		Vector128f result = { { {
				tanf(vector.F32[0]),
				tanf(vector.F32[1]),
				tanf(vector.F32[2]),
				tanf(vector.F32[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      SinCos
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SinCos(ConstVector128 input, Vector128* sin, Vector128* cos) noexcept
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
		Check(sin != nullptr);
		Check(cos != nullptr);

		Vector128f sinResult = { { {
				sinf(input.F32[0]),
				sinf(input.F32[1]),
				sinf(input.F32[2]),
				sinf(input.F32[3])
			} } };

		Vector128f cosResult = { { {
				cosf(input.F32[0]),
				cosf(input.F32[1]),
				cosf(input.F32[2]),
				cosf(input.F32[3])
			} } };

		*sin = sinResult.V;
		*cos = cosResult.V;
	}

	/****************************************************************************
	*                      SinH
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SinH(ConstVector128 vector) noexcept
	*
	*  @brief     全ての要素にSinHを取ったものを返します[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::SinH(ConstVector128 vector) noexcept
	{
		Vector128f result = { { {
				sinhf(vector.F32[0]),
				sinhf(vector.F32[1]),
				sinhf(vector.F32[2]),
				sinhf(vector.F32[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      CosH
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CosH(ConstVector128 vector) noexcept
	*
	*  @brief     全ての要素にCosHを取ったものを返します[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CosH(ConstVector128 vector) noexcept
	{
		Vector128f result = { { {
				coshf(vector.F32[0]),
				coshf(vector.F32[1]),
				coshf(vector.F32[2]),
				coshf(vector.F32[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      TanH
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::TanH(ConstVector128 vector) noexcept
	*
	*  @brief     全ての要素にTanHを取ったものを返します[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::TanH(ConstVector128 vector) noexcept
	{
		Vector128f result = { { {
				tanhf(vector.F32[0]),
				tanhf(vector.F32[1]),
				tanhf(vector.F32[2]),
				tanhf(vector.F32[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      ArcSin
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ArcSin(ConstVector128 vector) noexcept
	*
	*  @brief     全ての要素にArcSinを取ったものを返します[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ArcSin(ConstVector128 vector) noexcept
	{
		Vector128f result = { { {
				asinf(vector.F32[0]),
				asinf(vector.F32[1]),
				asinf(vector.F32[2]),
				asinf(vector.F32[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      ArcCos
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ArcCos(ConstVector128 vector) noexcept
	*
	*  @brief     全ての要素にArcSinを取ったものを返します[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ArcCos(ConstVector128 vector) noexcept
	{
		Vector128f result = { { {
				acosf(vector.F32[0]),
				acosf(vector.F32[1]),
				acosf(vector.F32[2]),
				acosf(vector.F32[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      ArcTan
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ArcCos(ConstVector128 vector) noexcept
	*
	*  @brief     全ての要素にArcSinを取ったものを返します[rad]
	*
	*  @param[in] ConstVector128 : vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ArcTan(ConstVector128 vector) noexcept
	{
		Vector128f result = { { {
				atanf(vector.F32[0]),
				atanf(vector.F32[1]),
				atanf(vector.F32[2]),
				atanf(vector.F32[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                       Lerp
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Lerp(ConstVector128 start, ConstVector128 end, const float t) noexcept
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
		Vector128 scale  = Set(t);
		Vector128 length = Subtract(end, start);
		return MultiplyAdd(length, scale, start);
	}

	/****************************************************************************
	*                       LerpV
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LerpV(ConstVector128 start, ConstVector128 end, const float t) noexcept
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
		Vector128 length = Subtract(end, start);
		return MultiplyAdd(length, t, start);
	}

	/****************************************************************************
	*                       Hermite
	****************************************************************************/
	*  @fn        __forceinline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Hermite(ConstVector128 startPosition, ConstVector128 startTangent, ConstVector128 endPosition, ConstVector128 endTangent, const float t) noexcept
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

		Vector128 p0 = Set(2.0f * t3 - 3.0f * t2 + 1.0f);
		Vector128 t0 = Set(t3 - 2.0f * t2 + t);
		Vector128 p1 = Set(-2.0f * t3 + 3.0f * t2);
		Vector128 t1 = Set(t3 - t2);
		
		Vector128 result = Multiply(p0, startPosition);
		result = MultiplyAdd(startTangent, t0, result);
		result = MultiplyAdd(endPosition, p1, result);
		result = MultiplyAdd(endTangent , t1, result);
		return result;
	}

	/****************************************************************************
	*                       HermiteV
	****************************************************************************/
	*  @fn        __forceinline Vector128 SIMD_CALL_CONVENTION Vector128Utility::HermiteV(ConstVector128 startPosition, ConstVector128 startTangent, ConstVector128 endPosition, ConstVector128 endTangent, const float t) noexcept
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
		Vector128 t2 = Multiply(t, t);
		Vector128 t3 = Multiply(t, t2);

		Vector128 p0 = Set(2.0f * t3.F32[0] - 3.0f * t2.F32[0] + 1.0f);
		Vector128 t0 = Set(t3.F32[1] - 2.0f * t2.F32[1] + t.F32[1]);
		Vector128 p1 = Set(-2.0f * t3.F32[2] + 3.0f * t2.F32[2]);
		Vector128 t1 = Set(t3.F32[3] - t2.F32[3]);

		Vector128 result = Multiply(p0, startPosition);
		result = MultiplyAdd(t0, startTangent, result);
		result = MultiplyAdd(p1, endPosition, result);
		result = MultiplyAdd(t1, endTangent, result);

		return result;
	}

	/****************************************************************************
	*                       CatMullRom
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CatMullRom(ConstVector128 position0, ConstVector128 position1, ConstVector128 position2, ConstVector128 position3, const float t) noexcept
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

		Vector128 p0 = Set((-t3 + 2.0f * t2 - t) * 0.5f);
		Vector128 p1 = Set((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
		Vector128 p2 = Set((-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
		Vector128 p3 = Set((t3 - t2) * 0.5f);

		Vector128 result = Multiply(p0, position0);
		result = MultiplyAdd(p1, position1, result);
		result = MultiplyAdd(p2, position2, result);
		result = MultiplyAdd(p3, position3, result);

		return result;
	}

	/****************************************************************************
	*                       CatMullRom
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::CatMullRom(ConstVector128 position0, ConstVector128 position1, ConstVector128 position2, ConstVector128 position3, const float t) noexcept
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
		const float fx = t.F32[0];
		const float fy = t.F32[1];
		const float fz = t.F32[2];
		const float fw = t.F32[3];
		Vector128f result = { { {
				0.5f * ((-fx * fx * fx + 2 * fx * fx - fx) * position0.F32[0]
				+ (3 * fx * fx * fx - 5 * fx * fx + 2) * position1.F32[0]
				+ (-3 * fx * fx * fx + 4 * fx * fx + fx) * position2.F32[0]
				+ (fx * fx * fx - fx * fx) * position3.F32[0]),

				0.5f * ((-fy * fy * fy + 2 * fy * fy - fy) * position0.F32[1]
				+ (3 * fy * fy * fy - 5 * fy * fy + 2) * position1.F32[1]
				+ (-3 * fy * fy * fy + 4 * fy * fy + fy) * position2.F32[1]
				+ (fy * fy * fy - fy * fy) * position3.F32[1]),

				0.5f * ((-fz * fz * fz + 2 * fz * fz - fz) * position0.F32[2]
				+ (3 * fz * fz * fz - 5 * fz * fz + 2) * position1.F32[2]
				+ (-3 * fz * fz * fz + 4 * fz * fz + fz) * position2.F32[2]
				+ (fz * fz * fz - fz * fz) * position3.F32[2]),

				0.5f * ((-fw * fw * fw + 2 * fw * fw - fw) * position0.F32[3]
				+ (3 * fw * fw * fw - 5 * fw * fw + 2) * position1.F32[3]
				+ (-3 * fw * fw * fw + 4 * fw * fw + fw) * position2.F32[3]
				+ (fw * fw * fw - fw * fw) * position3.F32[3])
			} } };
		return result.V;
	}

	/****************************************************************************
	*                      BaryCentric
	****************************************************************************/
	*  @fn        __forceinline Vector128 SIMD_CALL_CONVENTION Vector128Utility::BaryCentric(ConstVector128 position0, ConstVector128 position1, ConstVector128 position2, const float f, const float g) noexcept
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
		Vector128 p10    = Subtract(position1, position0);
		Vector128 scaleF = Set(f);

		Vector128 p20    = Subtract(position2, position0);
		Vector128 scaleG = Set(g);

		Vector128 result = MultiplyAdd(p10, scaleF, position0);
		result = MultiplyAdd(p20, scaleG, result);

		return result;
	}

	/****************************************************************************
	*                      BaryCentricV
	****************************************************************************/
	*  @fn        __forceinline Vector128 SIMD_CALL_CONVENTION Vector128Utility::BaryCentricV(ConstVector128 position0, ConstVector128 position1, ConstVector128 position2, const float f, const float g) noexcept
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
		Vector128 p10 = Subtract(position1, position0);
		Vector128 p20 = Subtract(position2, position0);

		Vector128 result = MultiplyAdd(p10, f, position0);
		result = MultiplyAdd(p20, g, result);

		return result;
	}

	/****************************************************************************
	*                       Min
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Min(ConstVector128 left, ConstVector128 right) noexcept
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
		Vector128f result = { { {
				left.F32[0] < right.F32[0] ? left.F32[0] : right.F32[0],
				left.F32[1] < right.F32[1] ? left.F32[1] : right.F32[1],
				left.F32[2] < right.F32[2] ? left.F32[2] : right.F32[2],
				left.F32[3] < right.F32[3] ? left.F32[3] : right.F32[3],
			} } };

		return result.V;
	}

	/****************************************************************************
	*                       Max
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Max(ConstVector128 left, ConstVector128 right) noexcept
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
		Vector128f result = { { {
				left.F32[0] > right.F32[0] ? left.F32[0] : right.F32[0],
				left.F32[1] > right.F32[1] ? left.F32[1] : right.F32[1],
				left.F32[2] > right.F32[2] ? left.F32[2] : right.F32[2],
				left.F32[3] > right.F32[3] ? left.F32[3] : right.F32[3],
			} } };

		return result.V;
	}

	namespace internal
	{
		inline float round_to_nearest(float x) noexcept
		{
			float i = floorf(x);
			x -= i;
			if (x < 0.5f)
				return i;
			if (x > 0.5f)
				return i + 1.f;

			float int_part;
			(void)modff(i / 2.f, &int_part);
			if ((2.f * int_part) == i)
			{
				return i;
			}

			return i + 1.f;
		}
	}

	/****************************************************************************
	*                       Round
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Round(ConstVector128 vector) noexcept
	*
	*  @brief     最も近い整数に丸める
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Round(ConstVector128 vector) noexcept
	{
		Vector128f result = { { {
				internal::round_to_nearest(vector.F32[0]),
				internal::round_to_nearest(vector.F32[1]),
				internal::round_to_nearest(vector.F32[2]),
				internal::round_to_nearest(vector.F32[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                       Truncate
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Truncate(ConstVector128 vector) noexcept
	*
	*  @brief     切り捨て (指定した数値の端数を切り捨てた整数に丸める。数直線上で0の方向にある次の整数
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Truncate(ConstVector128 vector) noexcept
	{
		Vector128 result = {};

		for (gu::uint32 i = 0; i < 4; i++)
		{
			if (IS_NAN(vector.F32[i]))
			{
				result.U32[i] = 0x7FC00000;
			}
			else if (fabsf(vector.F32[i]) < 8388608.0f)
			{
				result.F32[i] = static_cast<float>(static_cast<gu::int32>(vector.F32[i]));
			}
			else
			{
				result.F32[i] = vector.F32[i];
			}
		}
		return result;
	}

	/****************************************************************************
	*                       Floor
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Floor(ConstVector128 vector) noexcept
	*
	*  @brief     負の方向に丸める
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Floor(ConstVector128 vector) noexcept
	{
		Vector128f result = { { {
				floorf(vector.F32[0]),
				floorf(vector.F32[1]),
				floorf(vector.F32[2]),
				floorf(vector.F32[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                      Ceiling
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Ceiling(ConstVector128 vector) noexcept
	*
	*  @brief     切り上げ 数直線上で正の方向にある次の整数に丸める
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Ceiling(ConstVector128 vector) noexcept
	{
		Vector128f result = { { {
				ceilf(vector.F32[0]),
				ceilf(vector.F32[1]),
				ceilf(vector.F32[2]),
				ceilf(vector.F32[3])
			} } };

		return result.V;
	}

	/****************************************************************************
	*                       Clamp
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Clamp(ConstVector128 vector, ConstVector128 min, ConstVector128 max) noexcept
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
		Vector128 result = Max(min, vector);
		return Min(max, result);
	}

	/****************************************************************************
	*                       Saturate
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Saturate(ConstVector128 vector)
	*
	*  @brief     1以上には行かないようにする
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Saturate(ConstVector128 vector) noexcept
	{
		return Clamp(vector, VECTOR_128F_ZERO, VECTOR_128F_ONE);
	}

	/****************************************************************************
	*                       Abs
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Saturate(ConstVector128 vector)
	*
	*  @brief     絶対値
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Abs(ConstVector128 vector) noexcept
	{
		Vector128f result = { { {
				fabsf(vector.F32[0]),
				fabsf(vector.F32[1]),
				fabsf(vector.F32[2]),
				fabsf(vector.F32[3])
			} } };

		return result.V;
	}

	#pragma endregion Math

	#pragma region Angle
	/****************************************************************************
	*                       ModAngles
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ModAngles(ConstVector128 vector)
	*
	*  @brief      - pi <= theta <= piの範囲に収めるようにします.
	*
	*  @param[in] ConstVector128 angles : 角度rad
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ModAngles(ConstVector128 angles) noexcept
	{
		Vector128 vector = Multiply(angles, VECTOR_128F_RECIPROCAL_2PI.V);
		vector = Round(vector);
		return NegativeMultiplySubtract(VECTOR_128F_2PI.V, vector, angles);
	}

	/****************************************************************************
	*                       AddAngles
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::AddAngles(ConstVector128 left, ConstVector128 right) noexcept
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
		Vector128 result = Add(left, right);
		Vector128 mask   = LessVectorEach(result, VECTOR_128F_NEGATIVE_PI.V);
		Vector128 offset = Select(VECTOR_128F_ZERO, VECTOR_128F_2PI.V, mask);

		mask = GreaterOrEqualVectorEach(result, VECTOR_128F_PI.V);
		offset = Select(offset, VECTOR_128F_NEGATIVE_2PI.V, mask);

		return Add(result, offset);
	}

	/****************************************************************************
	*                       SubtractAngles
	****************************************************************************/
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::ModAngles(ConstVector128 vector)
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
		Vector128 result = Subtract(left, right);
		Vector128 mask   = LessVectorEach(result, VECTOR_128F_NEGATIVE_PI.V);
		Vector128 offset = Select(VECTOR_128F_ZERO, VECTOR_128F_2PI.V, mask);

		mask = GreaterOrEqualVectorEach(result, VECTOR_128F_PI.V);
		offset = Select(offset, VECTOR_128F_NEGATIVE_2PI.V, mask);

		return Add(result, offset);
	}
	#pragma endregion Angle
#pragma endregion Implement
}

#endif
#endif