//////////////////////////////////////////////////////////////////////////////////
///             @file   GMQuaternionf.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/23 11:46:55
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_QUATERNIONF_HPP
#define GM_QUATERNIONF_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Simd/Include/GMSimdMacros.hpp"
#include SIMD_COMPILED_HEADER(GameUtility/Math/Private/Simd/Include, GMSimdQuaternion128)

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
	#define QUATERNION128 gm::simd::non::Quaternion128
#elif PLATFORM_CPU_INSTRUCTION_NEON
	#define QUATERNION128 gm::simd::neon::Quaternion128
#else
	#define QUATERNION128 gm::simd::sse::Quaternion128
#endif
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gm
{
	struct Vector3f;
	struct Vector4f;

	/****************************************************************************
	*				  			   GMQuaternionf
	****************************************************************************/
	/* @class     GMQuaternionf
	*  @brief     temp
	*****************************************************************************/
	struct QuaternionF
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : 単位クォータニオンか
		*----------------------------------------------------------------------*/
		__forceinline bool IsIdentity() const 
		{ 
			return SIMD_NAME_SPACE::Quaternion128Utility::IsIdentity(_quaternion);
		}

		/*----------------------------------------------------------------------
		*  @brief : 回転角を取得します
		*----------------------------------------------------------------------*/
		float Angle() const;

		/*----------------------------------------------------------------------
		*  @brief : 内積を算出します. 通常のベクトル同様, 互いのベクトルのなす角度を導出したいときに使用します
		*----------------------------------------------------------------------*/
		__forceinline float Dot(const QuaternionF& right) const
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::Dot(_quaternion, right._quaternion);
		}

		/*----------------------------------------------------------------------
		*  @brief : 2次ノルム (全ての要素の2乗和)を取得します
		*----------------------------------------------------------------------*/
		__forceinline float Length() const
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::Length(_quaternion);
		}

		/*----------------------------------------------------------------------
		*  @brief : 2次ノルムに二乗を取得します
		*----------------------------------------------------------------------*/
		__forceinline float LengthSquare() const
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::LengthSquare(_quaternion);
		}

		/*----------------------------------------------------------------------
		*  @brief : クォータニオンの正規化を行います
		*----------------------------------------------------------------------*/
		__forceinline QuaternionF Normalize() const
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::Normalize(_quaternion);
		}

		/*----------------------------------------------------------------------
		*  @brief : 共役なクォータニオンを返します. 共役なクォータニオンは、逆向きの回転を表現します.
		*----------------------------------------------------------------------*/
		__forceinline QuaternionF Conjugate() const
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::Conjugate(_quaternion);
		}

		/*----------------------------------------------------------------------
		*  @brief : クォータニオンの逆関数を渡します
		*----------------------------------------------------------------------*/
		__forceinline QuaternionF Inverse() const
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::Inverse(_quaternion);
		}

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素にLogを適用します
		*----------------------------------------------------------------------*/
		__forceinline QuaternionF SIMD_CALL_CONVENTION Log() const
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::Log(_quaternion);
		}

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素にExpを適用します
		*----------------------------------------------------------------------*/
		__forceinline QuaternionF SIMD_CALL_CONVENTION Exp() const
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::Exp(_quaternion);
		}

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		#pragma region Operator
		/*----------------------------------------------------------------------
		*  @brief : 掛け算 left * right (directXMathの使い方とは引数の順番が逆なので注意 (q1, q2 -> q2 * q1))
		*           固定の回転軸で回転させたい場合は leftに回転軸と回転度合いのクォータニオン, オブジェクトの姿勢をrightに設定する
		*           ex ) worldY軸中心に30度回転みたいな状況であれば left : worldYQuaternion30 right : objectRotation
		*----------------------------------------------------------------------*/
		__forceinline QuaternionF operator* (const QuaternionF& right)
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::Multiply(_quaternion, right._quaternion);
		}

		// @brief : 掛け算 (this = rotateQuaternion * this)
		__forceinline QuaternionF& operator*=(const QuaternionF& rotateQuaternion)
		{
			this->_quaternion = SIMD_NAME_SPACE::Quaternion128Utility::Multiply(rotateQuaternion, _quaternion);
			return *this;
		}

		// @brief : 両者のクォータニオンが等しいか
		__forceinline bool operator ==(const QuaternionF& right) const noexcept 
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::Equal(_quaternion, right._quaternion);
		}

		// @brief : 両者のクォータニオンが等しくないか
		__forceinline bool operator !=(const QuaternionF& right) const noexcept
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::NotEqual(_quaternion, right._quaternion);
		}

		__forceinline float& operator[](const gu::uint32 index) noexcept
		{
			return _quaternion.m128_f32[index];
		}

		__forceinline const float& operator[](const gu::uint32 index) const noexcept 
		{
			return _quaternion.m128_f32[index];
		}

		#pragma endregion Operator

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor
		QuaternionF() 
		{
			_quaternion = SIMD_NAME_SPACE::Quaternion128Utility::Identity(); 
		};

		// @brief : 回転軸と回転量で生成するコンストラクタ
		QuaternionF(const Vector3f& axis, const float radian);

		// @brief : Quaternion128を直接代入するコンストラクタ
		QuaternionF(const QUATERNION128 quaternion) : _quaternion(quaternion) {};

		// @brief : roll pitch yawをもとに回転量を制御するコンストラクタ
		QuaternionF(const float roll, const float pitch, const float yaw) 
		{
			_quaternion = SIMD_NAME_SPACE::Quaternion128Utility::RotationRollPitchYaw(roll, pitch, yaw); 
		}

		// @brief : roll Pitch Yaw, 0というベクトル型で指定するコンストラクタ
		QuaternionF(const Vector3f& rollPitchYaw);

		// @brief : Vector4型で直接クォータニオンを生成するコンストラクタ
		QuaternionF(const Vector4f& vector);

		// @brief : Copy constructor
		QuaternionF(const QuaternionF&) = default;
		QuaternionF& operator=(const QuaternionF& right)
		{
			_quaternion = right._quaternion; return *this;
		}

		// @brief : Move constructor
		QuaternionF(QuaternionF&&) = default;
		QuaternionF& operator=(QuaternionF&&) = default;

		operator QUATERNION128() const noexcept
		{
			return SIMD_NAME_SPACE::Vector128Utility::LoadFloat4((float*)this);
		}
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		QUATERNION128 _quaternion;
	};

	/*----------------------------------------------------------------------
	*  @brief : Roll pitch yawを使って回転を行います
	*----------------------------------------------------------------------*/
	__forceinline QuaternionF RotationRollPitchYawQuaternion(const float roll, const float pitch, const float yaw) noexcept
	{
		return SIMD_NAME_SPACE::Quaternion128Utility::RotationRollPitchYaw(roll, pitch, yaw);
	}

	__forceinline QuaternionF RotationRollPitchYawQuaternion(const Vector3f& rollPitchYaw) noexcept;

	/*----------------------------------------------------------------------
	*  @brief : ある法線ベクトル軸を中心に回転を行います
	*----------------------------------------------------------------------*/
	__forceinline QuaternionF RotationNormalQuaternion(const Vector3f& normalAxis, const float angle) noexcept;

	/*----------------------------------------------------------------------
	*  @brief : ある軸を中心に回転を行います
	*----------------------------------------------------------------------*/
	__forceinline QuaternionF RotationAxisQuaternion(const Vector3f& axis, const float angle) noexcept;

	/*----------------------------------------------------------------------
	*  @brief : 通常の4次元ベクトルのように線形補間を掛けます. Slerpよりは精度が粗いですが, 省コストです
	*----------------------------------------------------------------------*/
	__forceinline QuaternionF Lerp(const QuaternionF& start, const QuaternionF& end, const float t) noexcept
	{
		return SIMD_NAME_SPACE::Quaternion128Utility::Lerp(start, end, t);
	}

	/*----------------------------------------------------------------------
	*  @brief : 球面補間です
	*----------------------------------------------------------------------*/
	__forceinline QuaternionF Slerp(const QuaternionF& start, const QuaternionF& end, const float t) noexcept
	{
		return SIMD_NAME_SPACE::Quaternion128Utility::Slerp(start, end, t);
	}

}

#endif