//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdQuaternion128SSE.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/22 2:52:20
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_QUATERNION_128_SSE_HPP
#define GM_SIMD_QUATERNION_128_SSE_HPP


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
	using Quaternion128 = Vector128;

	/****************************************************************************
	*				  			   GMSimdQuaternion128SSE
	****************************************************************************/
	/* @class     GMSimdQuaternion128SSE
	*  @brief     実部がwで虚部がx,y,zです. 
	*****************************************************************************/
	class Quaternion128Utility
	{
	public:
		#if ( defined(_M_IX86) || defined(_M_ARM) || defined(_M_ARM64) || USE_VECTOR_CALL || __i386__ || __arm__ || __aarch64__ )
		using ConstVector128     = const Vector128;	
		using ConstQuaternion128 = const Quaternion128;
		#else
		using ConstVector128 = const Vector128&;
		using ConstQuaternion128 = const Quaternion128&;
		#endif

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		#pragma region Setter
		/*----------------------------------------------------------------------
		*  @brief : 単位クォータニオンを取得
		*----------------------------------------------------------------------*/
		__forceinline static Quaternion128 SIMD_CALL_CONVENTION Identity() noexcept;
		#pragma endregion Setter
		#pragma region Operator
		
		/*----------------------------------------------------------------------
		*  @brief : 掛け算 left * right (directXMathの使い方とは引数の順番が逆なので注意 (q1, q2 -> q2 * q1))
		*           固定の回転軸で回転させたい場合は leftに回転軸と回転度合いのクォータニオン, オブジェクトの姿勢をrightに設定する
		*           ex ) worldY軸中心に30度回転みたいな状況であれば left : worldYQuaternion30 right : objectRotation 
		*----------------------------------------------------------------------*/
		__forceinline static Quaternion128 SIMD_CALL_CONVENTION Multiply(ConstQuaternion128 left, ConstQuaternion128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 両者のクォータニオンが等しいか
		*----------------------------------------------------------------------*/
		__forceinline static bool SIMD_CALL_CONVENTION Equal(ConstQuaternion128 left, ConstQuaternion128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 両者のクォータニオンが等しくないか
		*----------------------------------------------------------------------*/
		__forceinline static bool SIMD_CALL_CONVENTION NotEqual(ConstQuaternion128 left, ConstQuaternion128 right) noexcept;

		#pragma endregion Operator

		#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : 単位クォータニオンか
		*----------------------------------------------------------------------*/
		__forceinline static bool SIMD_CALL_CONVENTION IsIdentity(ConstQuaternion128 quaternion) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 内積を算出します. 通常のベクトル同様, 互いのベクトルのなす角度を導出したいときに使用します
		*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION Dot(ConstQuaternion128 left, ConstQuaternion128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
		*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION Length(ConstQuaternion128 quaternion) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 2次ノルムの二乗を算出
		*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION LengthSquare(ConstQuaternion128 quaternion) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : クォータニオンの正規化を行います
		*----------------------------------------------------------------------*/
		__forceinline static Quaternion128 SIMD_CALL_CONVENTION Normalize(ConstQuaternion128 quaternion) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 共役なクォータニオンを返します. 共役なクォータニオンは、逆向きの回転を表現します.
		*----------------------------------------------------------------------*/
		__forceinline static Quaternion128 SIMD_CALL_CONVENTION Conjugate(ConstQuaternion128 quaternion) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : クォータニオンの逆関数を渡します
		*----------------------------------------------------------------------*/
		__forceinline static Quaternion128 SIMD_CALL_CONVENTION Inverse(ConstQuaternion128 quaternion) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素にLogを適用します
		*----------------------------------------------------------------------*/
		__forceinline static Quaternion128 SIMD_CALL_CONVENTION Log(ConstQuaternion128 quaternion) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素にExpを適用します
		*----------------------------------------------------------------------*/
		__forceinline static Quaternion128 SIMD_CALL_CONVENTION Exp(ConstQuaternion128 quaternion) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 通常の4次元ベクトルのように線形補間を掛けます. Slerpよりは精度が粗いですが, 省コストです
		*----------------------------------------------------------------------*/
		__forceinline static Quaternion128 SIMD_CALL_CONVENTION Lerp(ConstQuaternion128 start, ConstQuaternion128 end, const float t) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 通常の4次元ベクトルのように線形補間を掛けます. tは単純な4次元ベクトルです
		*----------------------------------------------------------------------*/
		__forceinline static Quaternion128 SIMD_CALL_CONVENTION LerpV(ConstQuaternion128 start, ConstQuaternion128 end, ConstVector128 t) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 球面補間です
		*----------------------------------------------------------------------*/
		__forceinline static Quaternion128 SIMD_CALL_CONVENTION Slerp(ConstQuaternion128 start, ConstQuaternion128 end, const float t) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 球面補間です
		*----------------------------------------------------------------------*/
		__forceinline static Quaternion128 SIMD_CALL_CONVENTION SlerpV(ConstQuaternion128 start, ConstQuaternion128 end, ConstVector128 t) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : Roll pitch yawを使って回転を行います
		*----------------------------------------------------------------------*/
		__forceinline static Quaternion128 SIMD_CALL_CONVENTION RotationRollPitchYaw(const float roll, const float pitch, const float yaw) noexcept;
		__forceinline static Quaternion128 SIMD_CALL_CONVENTION RotationRollPitchYaw(ConstVector128 rollPitchYaw) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ある法線ベクトル軸を中心に回転を行います
		*----------------------------------------------------------------------*/
		__forceinline static Quaternion128 SIMD_CALL_CONVENTION  RotationNormal(ConstVector128 normalAxis, const float angle) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : ある軸を中心に回転を行います
		*----------------------------------------------------------------------*/
		__forceinline static Quaternion128 SIMD_CALL_CONVENTION RotationAxis(ConstVector128 axis, const float angle) noexcept;

		#pragma endregion Math
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
	/*----------------------------------------------------------------------
	*  @brief : 単位クォータニオンを渡す
	*----------------------------------------------------------------------*/
	inline Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::Identity() noexcept
	{
		return VECTOR_128F_IDENTITY_R3.V;
	}

	#pragma endregion Setter
	#pragma region Operator
	/*----------------------------------------------------------------------
	*  @brief : 掛け算 (～が先になるため、A -> B -> Cの順番で実行したければ)
	*----------------------------------------------------------------------*/
	inline Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::Multiply(ConstQuaternion128 left, ConstQuaternion128 right) noexcept
	{
		// Returns the product left*right (which is the concatenation of a rotation right followed by the rotation left)

		// [ (left.w * right.x) + (left.x * right.w) + (left.y * right.z) - (left.z * right.y),
		//   (left.w * right.y) - (left.x * right.z) + (left.y * right.w) + (left.z * right.x),
		//   (left.w * right.z) + (left.x * right.y) - (left.y * right.x) + (left.z * right.w),
		//   (left.w * right.w) - (left.x * right.x) - (left.y * right.y) - (left.z * right.z) ]

		static const Vector128f ControlWZYX = { { { 1.0f, -1.0f, 1.0f, -1.0f } } };
		static const Vector128f ControlZWXY = { { { 1.0f, 1.0f, -1.0f, -1.0f } } };
		static const Vector128f ControlYXWZ = { { { -1.0f, 1.0f, 1.0f, -1.0f } } };
		// Copy to SSE registers and use as few as possible for x86
		Quaternion128 leftX = left;
		Quaternion128 leftY = left;
		Quaternion128 leftZ = left;
		Quaternion128 vResult = left;
		// Splat with one instruction
		vResult = PERMUTE_PS(vResult, _MM_SHUFFLE(3, 3, 3, 3));
		leftX =   PERMUTE_PS(leftX, _MM_SHUFFLE(0, 0, 0, 0));
		leftY = PERMUTE_PS(leftY, _MM_SHUFFLE(1, 1, 1, 1));
		leftZ = PERMUTE_PS(leftZ, _MM_SHUFFLE(2, 2, 2, 2));
		// Retire right and perform right*leftW
		vResult = _mm_mul_ps(vResult, right);
		Quaternion128 rightShuffle = right;
		// Shuffle the copies of right
		rightShuffle = PERMUTE_PS(rightShuffle, _MM_SHUFFLE(0, 1, 2, 3));
		// Mul by rightWZYX
		leftX = _mm_mul_ps(leftX, rightShuffle);
		rightShuffle = PERMUTE_PS(rightShuffle, _MM_SHUFFLE(2, 3, 0, 1));
		// Flip the signs on y and z
		vResult = Vector128Utility::MultiplyAdd(leftX, ControlWZYX, vResult);
		// Mul by rightZWXY
		leftY = _mm_mul_ps(leftY, rightShuffle);
		rightShuffle = PERMUTE_PS(rightShuffle, _MM_SHUFFLE(0, 1, 2, 3));
		// Flip the signs on z and w
		leftY = _mm_mul_ps(leftY, ControlZWXY);
		// Mul by rightYXWZ
		leftZ = _mm_mul_ps(leftZ, rightShuffle);
		// Flip the signs on x and w
		leftY = Vector128Utility::MultiplyAdd(leftZ, ControlYXWZ, leftY);
		vResult = _mm_add_ps(vResult, leftY);
		return vResult;
	}

	/*----------------------------------------------------------------------
	*  @brief : 両者のクォータニオンが等しいか
	*----------------------------------------------------------------------*/
	inline bool SIMD_CALL_CONVENTION Quaternion128Utility::Equal(ConstQuaternion128 left, ConstQuaternion128 right) noexcept
	{
		return Vector128Utility::EqualVector4(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : 両者のクォータニオンが等しくないか
	*----------------------------------------------------------------------*/
	inline bool SIMD_CALL_CONVENTION Quaternion128Utility::NotEqual(ConstQuaternion128 left, ConstQuaternion128 right) noexcept
	{
		return Vector128Utility::NotEqualVector4(left, right);
	}

	#pragma endregion Operator

	#pragma region Math
	/****************************************************************************
	*                       IsIdentity
	****************************************************************************/
	/* @fn        bool SIMD_CALL_CONVENTION Quaternion128Utility::IsIdentity(ConstQuaternion128 quaternion) noexcept
	*
	*  @brief     単位クォータニオンか
	*
	*  @param[in] ConstQuaternion128 quaternion
	*
	*  @return 　　float
	*****************************************************************************/
	inline bool SIMD_CALL_CONVENTION Quaternion128Utility::IsIdentity(ConstQuaternion128 quaternion) noexcept
	{
		return Vector128Utility::EqualVector4(quaternion, VECTOR_128F_IDENTITY_R3.V);
	}

	/****************************************************************************
	*                       Dot
	****************************************************************************/
	/* @fn        float SIMD_CALL_CONVENTION Quaternion128Utility::Dot(ConstQuaternion128 left, ConstQuaternion128 right) noexcept
	*
	*  @brief      内積を算出します. 通常のベクトル同様, 互いのベクトルのなす角度を導出したいときに使用します
	*
	*  @param[in] ConstQuaternion128 quaternion
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Quaternion128Utility::Dot(ConstQuaternion128 left, ConstQuaternion128 right) noexcept
	{
		return Vector128Utility::DotVector4(left, right);
	}

	/****************************************************************************
	*                       Length
	****************************************************************************/
	/* @fn        float SIMD_CALL_CONVENTION Quaternion128Utility::Length(ConstQuaternion128 quaternion) noexcept
	*
	*  @brief     2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
	*
	*  @param[in] ConstQuaternion128 quaternion
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Quaternion128Utility::Length(ConstQuaternion128 quaternion) noexcept
	{
		return Vector128Utility::LengthVector4(quaternion);
	}

	/****************************************************************************
	*                       LengthSquare
	****************************************************************************/
	/* @fn        float SIMD_CALL_CONVENTION Quaternion128Utility::LengthSquare(ConstQuaternion128 quaternion) noexcept
	*
	*  @brief     2次ノルムの二乗を算出
	*
	*  @param[in] ConstQuaternion128 quaternion
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Quaternion128Utility::LengthSquare(ConstQuaternion128 quaternion) noexcept
	{
		return Vector128Utility::LengthSquaredVector4(quaternion);
	}

	/****************************************************************************
	*                       Normalize
	****************************************************************************/
	/* @fn        Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::Normalize(ConstQuaternion128 quaternion) noexcept
	*
	*  @brief      クォータニオンの正規化を行います
	*
	*  @param[in] ConstQuaternion128 quaternion
	*
	*  @return 　　Quaternion128
	*****************************************************************************/
	inline Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::Normalize(ConstQuaternion128 quaternion) noexcept
	{
		return Vector128Utility::NormalizeVector4(quaternion);
	}

	/****************************************************************************
	*                       Conjugate
	****************************************************************************/
	/* @fn        Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::Conjugate(ConstQuaternion128 quaternion) noexcept
	*
	*  @brief     共役なクォータニオンを返します. 共役なクォータニオンは、逆向きの回転を表現します
	*
	*  @param[in] ConstQuaternion128 quaternion
	*
	*  @return 　　Quaternion128
	*****************************************************************************/
	inline Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::Conjugate(ConstQuaternion128 quaternion) noexcept
	{
		static const Vector128f NegativeOne3 = { { { -1.0f, -1.0f, -1.0f, 1.0f } } };
		return _mm_mul_ps(quaternion, NegativeOne3.V);
	}

	/****************************************************************************
	*                       Inverse
	****************************************************************************/
	/* @fn        Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::Inverse(ConstQuaternion128 quaternion) noexcept
	*
	*  @brief     クォータニオンの逆関数を渡します
	*
	*  @param[in] ConstQuaternion128 quaternion
	*
	*  @return 　　Quaternion128
	*****************************************************************************/
	inline Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::Inverse(ConstQuaternion128 quaternion) noexcept
	{
		// 正規化を実行するために使用
		Vector128 normSquared = Vector128Utility::Set(Vector128Utility::LengthSquaredVector4(quaternion));
		
		// 共役なクォータニオン取得
		Quaternion128 conjugate = Conjugate(quaternion);

		// 長さが非常に小さい場合の対応
		Vector128 control = Vector128Utility::LessOrEqualVectorEach(normSquared, VECTOR_128F_EPSILON.V);

		// normalize
		Vector128 result = Vector128Utility::Divide(conjugate, normSquared);

		result = Vector128Utility::Select(result, VECTOR_128F_ZERO.V, control);

		return result;
	}

	/****************************************************************************
	*                       Log
	****************************************************************************/
	/* @fn        Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::Log(ConstQuaternion128 quaternion) noexcept
	*
	*  @brief     全ての要素にLogを適用します
	*
	*  @param[in] ConstQuaternion128 quaternion
	*
	*  @return 　　Quaternion128
	*****************************************************************************/
	inline Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::Log(ConstQuaternion128 quaternion) noexcept
	{
		static const Vector128f OneMinusEpsilon = { { { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f } } };

		Vector128 QW = Vector128Utility::SplatW(quaternion);
		Vector128 Q0 = Vector128Utility::Select(VECTOR_128F_SELECT_1110.V, quaternion, VECTOR_128F_SELECT_1110.V);

		Vector128 ControlW = Vector128Utility::InBoundsVectorEach(QW, OneMinusEpsilon.V);

		Vector128 Theta    = Vector128Utility::ArcCos(QW);
		Vector128 SinTheta = Vector128Utility::Sin(Theta);

		Vector128 S = Vector128Utility::Divide(Theta, SinTheta);

		Vector128 Result = Vector128Utility::Multiply(Q0, S);
		Result = Vector128Utility::Select(Q0, Result, ControlW);

		return Result;
	}

	/****************************************************************************
	*                       Exp
	****************************************************************************/
	/* @fn        Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::Exp(ConstQuaternion128 quaternion) noexcept
	*
	*  @brief     全ての要素にExpを適用します
	*
	*  @param[in] ConstQuaternion128 quaternion
	*
	*  @return 　　Quaternion128
	*****************************************************************************/
	inline Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::Exp(ConstQuaternion128 quaternion) noexcept
	{
		Vector128 Theta = Vector128Utility::Set(Vector128Utility::LengthVector3(quaternion));

		Vector128 SinTheta, CosTheta;
		Vector128Utility::SinCos(Theta, &SinTheta, &CosTheta);

		Vector128 S = Vector128Utility::Divide(SinTheta, Theta);

		Vector128 Result = Vector128Utility::Multiply(quaternion, S);

		const Vector128 Zero = Vector128Utility::Zero();
		Vector128 Control    = Vector128Utility::NearEqualVectorEach(Theta, Zero, VECTOR_128F_EPSILON.V);
		Result = Vector128Utility::Select(Result, quaternion, Control);

		Result = Vector128Utility::Select(CosTheta, Result, VECTOR_128F_SELECT_1110.V);

		return Result;
	}

	/****************************************************************************
	*                       Lerp
	****************************************************************************/
	/* @fn        Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::Lerp(ConstQuaternion128 start, ConstQuaternion128 end, const float t) noexcept
	*
	*  @brief     通常の4次元ベクトルのように線形補間を掛けます. Slerpよりは精度が粗いですが, 省コストです
	*
	*  @param[in] ConstQuaternion128 開始時の姿勢
	*  @param[in] ConstQuaternion128 終了時の姿勢,
	*  @param[in] ConstVector128 t 割合
	*
	*  @return 　　Quaternion128
	*****************************************************************************/
	inline Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::Lerp(ConstQuaternion128 start, ConstQuaternion128 end, const float t) noexcept
	{
		return Vector128Utility::Lerp(start, end, t);
	}

	/****************************************************************************
	*                       LerpV
	****************************************************************************/
	/* @fn        Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::LerpV(ConstQuaternion128 start, ConstQuaternion128 end, ConstVector128 t) noexcept
	*
	*  @brief     通常の4次元ベクトルのように線形補間を掛けます. tは単純な4次元ベクトルです
	*
	*  @param[in] ConstQuaternion128 開始時の姿勢
	*  @param[in] ConstQuaternion128 終了時の姿勢,
	*  @param[in] ConstVector128 t 割合
	*
	*  @return 　　Quaternion128
	*****************************************************************************/
	inline Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::LerpV(ConstQuaternion128 start, ConstQuaternion128 end, ConstVector128 t) noexcept
	{
		return Vector128Utility::LerpV(start, end, t);
	}

\
	/****************************************************************************
	*                       Slerp
	****************************************************************************/
	/* @fn        Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::SlerpV(ConstQuaternion128 start, ConstQuaternion128 end, const float t) noexcept
	*
	*  @brief     球面補間です
	*
	*  @param[in] ConstQuaternion128 開始時の姿勢
	*  @param[in] ConstQuaternion128 終了時の姿勢,
	*  @param[in] const float t 割合
	*
	*  @return 　　Quaternion128
	*****************************************************************************/
	inline Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::Slerp(ConstQuaternion128 start, ConstQuaternion128 end, const float t) noexcept
	{
		return SlerpV(start, end, Vector128Utility::Set(t));
	}

	/****************************************************************************
	*                       SlerpV
	****************************************************************************/
	/* @fn        Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::SlerpV(ConstQuaternion128 start, ConstQuaternion128 end, ConstVector128 t) noexcept
	*
	*  @brief     球面補間です
	*
	*  @param[in] ConstQuaternion128 開始時の姿勢
	*  @param[in] ConstQuaternion128 終了時の姿勢, 
	*  @param[in] ConstVector128 t 割合
	*
	*  @return 　　Quaternion128
	*****************************************************************************/
	inline Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::SlerpV(ConstQuaternion128 start, ConstQuaternion128 end, ConstVector128 t) noexcept
	{
		Check((Vector128Utility::GetY(t) == Vector128Utility::GetX(t)) && (Vector128Utility::GetZ(t) == Vector128Utility::GetX(t)) && (Vector128Utility::GetW(t) == Vector128Utility::GetX(t)));

		static const Vector128f OneMinusEpsilon = { { { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f } } };
		static const Vector128u SignMask2       = { { { 0x80000000, 0x00000000, 0x00000000, 0x00000000 } } };

		Vector128 CosOmega = Vector128Utility::Set(Quaternion128Utility::Dot(start, end));

		const Vector128 Zero = Vector128Utility::Zero();
		Vector128 Control    = Vector128Utility::LessVectorEach(CosOmega, Zero);
		Vector128 Sign = Vector128Utility::Select(VECTOR_128F_ONE.V, VECTOR_128F_NEGATIVE_ONE.V, Control);

		CosOmega = _mm_mul_ps(CosOmega, Sign);

		Control = Vector128Utility::LessVectorEach(CosOmega, OneMinusEpsilon);

		Vector128 SinOmega = _mm_mul_ps(CosOmega, CosOmega);
		SinOmega = _mm_sub_ps(VECTOR_128F_ONE.V, SinOmega);
		SinOmega = _mm_sqrt_ps(SinOmega);

		Vector128 Omega = Vector128Utility::ArcTan2(SinOmega, CosOmega);

		Vector128 V01 = PERMUTE_PS(t, _MM_SHUFFLE(2, 3, 0, 1));
		V01 = _mm_and_ps(V01, VECTOR_128U_MASK_XY.V);
		V01 = _mm_xor_ps(V01, SignMask2);
		V01 = _mm_add_ps(VECTOR_128F_IDENTITY_R0.V, V01);

		Vector128 S0 = _mm_mul_ps(V01, Omega);
		S0 = Vector128Utility::Sin(S0);
		S0 = _mm_div_ps(S0, SinOmega);

		S0 = Vector128Utility::Select(V01, S0, Control);

		Vector128 S1 = Vector128Utility::SplatY(S0);
		S0 = Vector128Utility::SplatX(S0);

		S1 = _mm_mul_ps(S1, Sign);
		Vector128 Result = _mm_mul_ps(start, S0);
		S1 = _mm_mul_ps(S1, end);
		Result = _mm_add_ps(Result, S1);
		return Result;
	}

	/****************************************************************************
	*                       RotationRollPitchYaw
	****************************************************************************/
	/* @fn        Vector128 SIMD_CALL_CONVENTION Quaternion128Utility::RotationRollPitchYaw(ConstVector128 rollPitchYaw) noexcept
	*
	*  @brief     Roll -> pitch -> yawを使って回転を行います
	*
	*  @param[in] const float roll [rad]
	*  @param[in] const float pitch [rad]
	*  @param[in] const float yaw [rad]
	*
	*  @return 　　Quaternion128
	*****************************************************************************/
	inline Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::RotationRollPitchYaw(const float roll, const float pitch, const float yaw) noexcept
	{
		return RotationRollPitchYaw(Vector128Utility::Set(roll, pitch, yaw, 0));
	}

	/****************************************************************************
	*                       RotationRollPitchYaw
	****************************************************************************/
	/* @fn        Vector128 SIMD_CALL_CONVENTION Quaternion128Utility::RotationRollPitchYaw(ConstVector128 rollPitchYaw) noexcept
	*
	*  @brief     Roll -> pitch -> yawを使って回転を行います
	*
	*  @param[in] ConstVector128 roll pitch yaw , 0のベクトル
	*
	*  @return 　　Quaternion128
	*****************************************************************************/
	inline Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::RotationRollPitchYaw(ConstVector128 rollPitchYaw) noexcept
	{
		static const Vector128f sign = { { { 1.0f, -1.0f, -1.0f, 1.0f } } };

		Vector128 halfAngles = Vector128Utility::Multiply(rollPitchYaw, VECTOR_128F_ONE_HALF.V);

		Vector128 sinAngles = {}, cosAngles = {};
		Vector128Utility::SinCos(halfAngles, &sinAngles, &cosAngles);

		Vector128 R0 = VectorPermute<GM_PERMUTE_0X, GM_PERMUTE_1X, GM_PERMUTE_1X, GM_PERMUTE_1X>(sinAngles, cosAngles);
		Vector128 P0 = VectorPermute<GM_PERMUTE_1Y, GM_PERMUTE_0Y, GM_PERMUTE_1Y, GM_PERMUTE_1Y>(sinAngles, cosAngles);
		Vector128 Y0 = VectorPermute<GM_PERMUTE_1Z, GM_PERMUTE_1Z, GM_PERMUTE_0Z, GM_PERMUTE_1Z>(sinAngles, cosAngles);
		Vector128 R1 = VectorPermute<GM_PERMUTE_0X, GM_PERMUTE_1X, GM_PERMUTE_1X, GM_PERMUTE_1X>(cosAngles, sinAngles);
		Vector128 P1 = VectorPermute<GM_PERMUTE_1Y, GM_PERMUTE_0Y, GM_PERMUTE_1Y, GM_PERMUTE_1Y>(cosAngles, sinAngles);
		Vector128 Y1 = VectorPermute<GM_PERMUTE_1Z, GM_PERMUTE_1Z, GM_PERMUTE_0Z, GM_PERMUTE_1Z>(cosAngles, sinAngles);

		Vector128 right = Vector128Utility::Multiply(P1, sign.V);
		Vector128 Q0 = Vector128Utility::Multiply(P0, Y0);
		right = Vector128Utility::Multiply(right, Y1);
		Q0 = Vector128Utility::Multiply(Q0, R0);
		Vector128 Q = Vector128Utility::MultiplyAdd(right, R1, Q0);

		return Q;
	}

	/****************************************************************************
	*                       RotationNormal
	****************************************************************************/
	/* @fn        Vector128 SIMD_CALL_CONVENTION  Quaternion128Utility::RotationNormal(ConstVector128 normalAxis, const float angle) noexcept
	*
	*  @brief     ある法線ベクトルを中心に回転を行います
	*
	*  @param[in] ConstVector128 Vector3型のAxis
	*  @param[in] const float radianによる角度
	*
	*  @return 　　Quaternion128
	*****************************************************************************/
	inline Quaternion128 SIMD_CALL_CONVENTION  Quaternion128Utility::RotationNormal(ConstVector128 normalAxis, const float angle) noexcept
	{
		// 3次元ベクトルとして取得する (w = 0)
		Vector128 normal = _mm_and_ps(normalAxis, VECTOR_128F_MASK_XYZ.V);
		
		// クォータニオンの回転軸としたいので, w = 1に設定する
		normal = _mm_or_ps(normal, VECTOR_128F_IDENTITY_R3.V);

		// クォータニオンは 基本theta / 2として定義するため, そのScale調整
		Vector128 scale = _mm_set_ps1(0.5f * angle);

		// theta / 2によるsin成分とcos成分を取得する
		Vector128 sin = {};
		Vector128 cos = {};
		Vector128Utility::SinCos(scale, &sin, &cos);

		// クォータニオンのxyz成分を取り出す
		scale = _mm_and_ps(sin, VECTOR_128F_MASK_XYZ.V); // scaleの意味合いはもうない
		
		// クォータニオンのw成分を取り出す
		cos   = _mm_and_ps(cos, VECTOR_128U_MASK_W.V);
		
		// クォータニオンとして全体を取得
		scale = _mm_or_ps(scale, cos);

		// 軸方向に回転
		return _mm_mul_ps(normalAxis, scale); 
	}

	/****************************************************************************
	*                       RotationAxis
	****************************************************************************/
	/* @fn        Vector128 SIMD_CALL_CONVENTION Quaternion128Utility::RotationAxis(ConstVector128 axis, const float angle) noexcept
	*
	*  @brief     ある軸を中心に回転を行います
	*
	*  @param[in] ConstVector128 Vector3型のAxis
	*  @param[in] const float radianによる角度
	*
	*  @return 　　Quaternion128
	*****************************************************************************/
	inline Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::RotationAxis(ConstVector128 axis, const float angle) noexcept
	{
		Check(!Vector128Utility::EqualVector3(axis, VECTOR_128F_ZERO.V));
		Check(!Vector128Utility::IsInfiniteVector3(axis));

		Quaternion128 normal = Vector128Utility::NormalizeVector3(axis);
		
		return RotationNormal(normal, angle);
	}

	#pragma endregion Math
#pragma endregion Implement
}
#endif
#endif