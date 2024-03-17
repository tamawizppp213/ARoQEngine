//////////////////////////////////////////////////////////////////////////////////
///             @file   GMVector3F.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/03 3:58:44
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_VECTOR3F_HPP
#define GM_VECTOR3F_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Simd/Include/GMSimdMacros.hpp"
#include SIMD_COMPILED_HEADER(GameUtility/Math/Private/Simd/Include, GMSimdVector128)

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
#define VECTOR128 gm::simd::non::Vector128
#elif PLATFORM_CPU_INSTRUCTION_NEON
#define VECTOR128 gm::simd::neon::Vector128 
#else
#define VECTOR128 gm::simd::sse::Vector128
#endif
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	struct Vector3f;
	struct Float3;

	/****************************************************************************
	*				  			   GMVector3
	*************************************************************************//**
	*  @class     GMVector3
	*  @brief     SIMD演算用のVector3クラスです. アラインメントを行っているため, データを保持する場合はFloat3を使用してください
	*****************************************************************************/
	struct Float3
	{
	public:
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		float x;
		float y;
		float z;

		__forceinline bool operator ==(const Float3& vector) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::EqualVector3(*this, vector); }
		__forceinline bool operator !=(const Float3& vector) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::NotEqualVector3(*this, vector); }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor
		Float3() : x(0), y(0), z(0) {};

		// @brief : 一つの要素で初期化
		Float3(const float ix) : x(ix), y(ix), z(ix) {};

		// @brief : 全ての要素で初期化
		Float3(const float ix, const float iy, const float iz) : x(ix), y(iy), z(iz) {};

		// @brief : 配列を使って初期化
		explicit Float3(_In_reads_(4) const float* pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]) {};

		// @brief : copy constructor
		Float3(const Float3&) = default;

		Float3& operator=(const Float3&) = default;

		// @brief : move constructor
		Float3(Float3&&) = default;

		Float3& operator=(Float3&&) = default;

		operator VECTOR128() const noexcept
		{
			return SIMD_NAME_SPACE::Vector128Utility::LoadFloat3((float*)this);
		}
	};

	/****************************************************************************
	*				  			   GMVector3
	*************************************************************************//**
	*  @class     GMVector3
	*  @brief     SIMD演算用のVector3クラスです. アラインメントを行っているため, データを保持する場合はFloat3を使用してください
	*****************************************************************************/
	struct Vector3f
	{
	public:
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
#pragma region Default Vector
		// @brief : (0, 0, 0, 0)
		static const Vector3f ZERO;

		// @brief : (1, 1, 1, 1)
		static const Vector3f ONE;

		// @brief : (1,0,0,0)
		static const Vector3f XAXIS;

		// @brief : (0,1,0,0)
		static const Vector3f YAXIS;

		// @brief : (0,0,1,0)
		static const Vector3f ZAXIS;

		// @brief : (0,0,0,1)
		static const Vector3f WAXIS;

#pragma endregion Default Vector

		/****************************************************************************
		**                Public Function
		*****************************************************************************/

#pragma region Getter
		/*----------------------------------------------------------------------
		*  @brief : FPUレジスタに格納されたX(, Y, Z, W)等の要素を取り出す
		/*----------------------------------------------------------------------*/
		__forceinline gu::float32 GetX() const { return SIMD_NAME_SPACE::Vector128Utility::GetX(_vector); }
		__forceinline gu::float32 GetY() const { return SIMD_NAME_SPACE::Vector128Utility::GetY(_vector); }
		__forceinline gu::float32 GetZ() const { return SIMD_NAME_SPACE::Vector128Utility::GetZ(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : Float3に変換する
		/*----------------------------------------------------------------------*/
		__forceinline Float3 ToFloat3()
		{
			Float3 value = {};
			SIMD_NAME_SPACE::Vector128Utility::StoreFloat3((float*)&value, _vector);
			return value;
		}

#pragma endregion Getter

#pragma region Setter
		/*----------------------------------------------------------------------
		*  @brief : Floatを使ってVectorのX(, Y, Z, W)要素に値を代入する
		/*----------------------------------------------------------------------*/
		__forceinline void SetX(const float x) { _vector = SIMD_NAME_SPACE::Vector128Utility::SetX(_vector, x); }
		__forceinline void SetY(const float y) { _vector = SIMD_NAME_SPACE::Vector128Utility::SetY(_vector, y); }
		__forceinline void SetZ(const float z) { _vector = SIMD_NAME_SPACE::Vector128Utility::SetZ(_vector, z); }
#pragma endregion Setter

#pragma region Operator Function
		// @brief : 加算
		__forceinline Vector3f operator+  (const Vector3f& right) const { return Vector3f(SIMD_NAME_SPACE::Vector128Utility::Add(_vector, right._vector)); }

		// @brief : 減算
		__forceinline Vector3f operator- (const Vector3f& right) const { return Vector3f(SIMD_NAME_SPACE::Vector128Utility::Subtract(_vector, right._vector)); }

		// @brief : 乗算
		__forceinline Vector3f operator* (const Vector3f& right) const { return Vector3f(SIMD_NAME_SPACE::Vector128Utility::Multiply(_vector, right._vector)); }

		// @brief : 除算
		__forceinline Vector3f operator/ (const Vector3f& right) const { return Vector3f(SIMD_NAME_SPACE::Vector128Utility::Vector128Utility::Divide(_vector, right._vector)); }

		// @brief : 余り
		__forceinline Vector3f operator% (const Vector3f& right) const { return Vector3f(SIMD_NAME_SPACE::Vector128Utility::Vector128Utility::Mod(_vector, right._vector)); }

		// @brief : スカラー倍
		__forceinline        Vector3f operator* (const float right) const { return Vector3f(SIMD_NAME_SPACE::Vector128Utility::Scale(_vector, right)); }
		__forceinline friend Vector3f operator* (const float left, const Vector3f right) { return Vector3f(SIMD_NAME_SPACE::Vector128Utility::Scale(right._vector, left)); }

		// @brief : スカラーで除算
		__forceinline        Vector3f operator/ (const float right) const { return Vector3f(SIMD_NAME_SPACE::Vector128Utility::Scale(_vector, 1.0f / right)); }
		__forceinline friend Vector3f operator/ (const float left, const Vector3f right) { return Vector3f(SIMD_NAME_SPACE::Vector128Utility::Scale(right._vector, 1.0f / left)); }

		// @brief : 加算 (_vectorではなく, *thisを使って足すのはsimd::Vector128がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector3f& operator +=(const Vector3f& right) { *this = *this + right;  return *this; }

		// @brief : 減算 (_vectorではなく, *thisを使って引くのはsimd::Vector128がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector3f& operator -=(const Vector3f& right) { *this = *this - right; return *this; }

		// @brief : 乗算 (_vectorではなく, *thisを使ってかけるのはsimd::Vector128がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector3f& operator *=(const Vector3f& right) { *this = *this * right; return *this; }

		// @brief : 除算 (_vectorではなく, *thisを使って割るのはsimd::Vector128がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector3f& operator /=(const Vector3f& right) { *this = *this / right; return *this; }

		// @brief : 全ての要素が等しいか
		__forceinline bool operator == (const Vector3f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::EqualVector3(_vector, right._vector); }

		// @brief : 一つでも要素が異なるか
		__forceinline bool operator != (const Vector3f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::NotEqualVector3(_vector, right._vector); }

		// @brief : 全ての要素で大きいか
		__forceinline bool operator >  (const Vector3f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::GreaterVector3(_vector, right._vector); }

		// @brief : 全ての要素で大きいor等しいか
		__forceinline bool operator >=  (const Vector3f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::GreaterOrEqualVector3(_vector, right._vector); }

		// @brief : 全ての要素で小さいか
		__forceinline bool operator <  (const Vector3f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::LessVector3(_vector, right._vector); }

		// @brief : 全ての要素で大きいか
		__forceinline bool operator <= (const Vector3f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::LessOrEqualVector3(_vector, right._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 指定範囲内にあるかどうか -bounds <= vector <= +bounds
		/*----------------------------------------------------------------------*/
		__forceinline bool InBounds(const Vector3f& bounds) noexcept { return SIMD_NAME_SPACE::Vector128Utility::InBoundsVector3(_vector, bounds._vector); }

		__forceinline operator VECTOR128() const { return _vector; }

#pragma endregion Operator Function

#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルの各要素同士の逆数を算出します. (1.0f / vector)
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Reciprocal() const { return SIMD_NAME_SPACE::Vector128Utility::Reciprocal(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 各要素にマイナスを取ったものを返す.
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Negate() const { return SIMD_NAME_SPACE::Vector128Utility::Negate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 1次ノルム(各要素の絶対値の和におけるベクトルの大きさ)を算出
		/*----------------------------------------------------------------------*/
		__forceinline float Norm() const { return SIMD_NAME_SPACE::Vector128Utility::NormVector3(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
		/*----------------------------------------------------------------------*/
		__forceinline float NormSquared() const { return SIMD_NAME_SPACE::Vector128Utility::NormSquaredVector3(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 内積を算出
		/*----------------------------------------------------------------------*/
		__forceinline float Dot(const Vector3f& right) const { return SIMD_NAME_SPACE::Vector128Utility::DotVector3(_vector, right._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 外積を算出 second, thirdに垂直なベクトルを返す
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Cross(const Vector3f& second) const { return SIMD_NAME_SPACE::Vector128Utility::CrossVector3(_vector, second._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 入力ベクトルの単位ベクトルを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Normalize() const { return SIMD_NAME_SPACE::Vector128Utility::NormalizeVector3(_vector); }

		/*----------------------------------------------------------------------
		*  @brief     反射ベクトルを算出
		*  @param[in] const Vector128&f : input  (入射ベクトル)
		*  @param[in] const Vector128&f : normal (反射を行う法線ベクトル)
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Reflect(const Vector3f& normal) const { return SIMD_NAME_SPACE::Vector128Utility::ReflectVector3(_vector, normal._vector); }

		/*----------------------------------------------------------------------
		*  @brief     屈折ベクトルを算出
		*  @param[in] const Vector3f& : input  (入射ベクトル)
		*  @param[in] const Vector3f& : normal (反射を行う法線ベクトル)
		*  @param[in] const float    : refractionIndex (屈折率)
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Refract(const Vector3f& normal, const float     refractionIndex) noexcept { return SIMD_NAME_SPACE::Vector128Utility::RefractVector3(_vector, normal._vector, refractionIndex); }
		__forceinline Vector3f Refract(const Vector3f& normal, const Vector3f& refractionIndex) noexcept { return SIMD_NAME_SPACE::Vector128Utility::RefractVector3V(_vector, normal._vector, refractionIndex._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 非常に大きい値か
		/*----------------------------------------------------------------------*/
		__forceinline bool IsInfinite() const { return SIMD_NAME_SPACE::Vector128Utility::IsInfiniteVector3(_vector); }

		/*----------------------------------------------------------------------
		*  @brief :  ゼロかどうか
		/*----------------------------------------------------------------------*/
		__forceinline bool IsZero() const { return ZERO == _vector; }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素に根号を取ったものを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Sqrt() const { return SIMD_NAME_SPACE::Vector128Utility::Sqrt(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の逆数を取り, それに根号を取ったものを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f ReciprocalSqrt() const { return SIMD_NAME_SPACE::Vector128Utility::ReciprocalSqrt(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のsin, cos, tanを返します [rad]
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Sin() const { return SIMD_NAME_SPACE::Vector128Utility::Sin(_vector); }
		__forceinline Vector3f Cos() const { return SIMD_NAME_SPACE::Vector128Utility::Cos(_vector); }
		__forceinline Vector3f Tan() const { return SIMD_NAME_SPACE::Vector128Utility::Tan(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のarcsin, arccos, arctanを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f ArcSin() const { return SIMD_NAME_SPACE::Vector128Utility::ArcSin(_vector); }
		__forceinline Vector3f ArcCos() const { return SIMD_NAME_SPACE::Vector128Utility::ArcCos(_vector); }
		__forceinline Vector3f ArcTan() const { return SIMD_NAME_SPACE::Vector128Utility::ArcTan(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のsinH, cosH, tanHを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f SinH() const { return SIMD_NAME_SPACE::Vector128Utility::SinH(_vector); }
		__forceinline Vector3f CosH() const { return SIMD_NAME_SPACE::Vector128Utility::CosH(_vector); }
		__forceinline Vector3f TanH() const { return SIMD_NAME_SPACE::Vector128Utility::TanH(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素に対するsin成分とcos成分を取り出します
		/*----------------------------------------------------------------------*/
		__forceinline void SinCos(Vector3f& sin, Vector3f& cos) noexcept
		{
			SIMD_NAME_SPACE::Vector128Utility::SinCos(_vector, &sin._vector, &cos._vector);
		}

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の2^{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Exp2() const { return SIMD_NAME_SPACE::Vector128Utility::Exp2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の10^{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Exp10() const { return SIMD_NAME_SPACE::Vector128Utility::Exp10(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のe^{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f ExpE() const { return SIMD_NAME_SPACE::Vector128Utility::ExpE(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のlog2{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Log2() const { return SIMD_NAME_SPACE::Vector128Utility::Log2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のlog10{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Log10() const { return SIMD_NAME_SPACE::Vector128Utility::Log10(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のloge{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f LogE() const { return SIMD_NAME_SPACE::Vector128Utility::LogE(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のPowを返します
		*           base^{power}
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Pow(const Vector3f& power) const { return SIMD_NAME_SPACE::Vector128Utility::Pow(_vector, power._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 最も近い整数に丸める
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Round() const { return SIMD_NAME_SPACE::Vector128Utility::Round(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 切り捨て (指定した数値の端数を切り捨てた整数に丸める。数直線上で0の方向にある次の整数
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Truncate() const { return SIMD_NAME_SPACE::Vector128Utility::Truncate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 負の方向に丸める
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Floor() const { return SIMD_NAME_SPACE::Vector128Utility::Floor(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 切り上げ 数直線上で正の方向にある次の整数に丸める
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Ceiling() const { return SIMD_NAME_SPACE::Vector128Utility::Ceiling(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 指定範囲内で数値を設定する
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Clamp(const Vector3f& min, const Vector3f& max) const { return SIMD_NAME_SPACE::Vector128Utility::Clamp(_vector, min._vector, max._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 1以上には行かないようにする
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Saturate() const { return SIMD_NAME_SPACE::Vector128Utility::Saturate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 絶対値
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Abs() noexcept { return SIMD_NAME_SPACE::Vector128Utility::Abs(_vector); }

#pragma endregion Math
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor
		Vector3f() { _vector = SIMD_NAME_SPACE::Vector128Utility::Zero(); }

		// @brief : Float型で生成するコンストラクタ
		Vector3f(const gu::float32 x, const gu::float32 y, const gu::float32 z)
		{
			_vector = SIMD_NAME_SPACE::Vector128Utility::Set(x, y, z, 0.0f);
		}

		// @brief : Vector128を直接代入するコンストラクタ
		Vector3f(const VECTOR128 vector) : _vector(vector) {};

		// @brief : Float3型で生成するコンストラクタ
		Vector3f(const Float3& vector) { _vector = SIMD_NAME_SPACE::Vector128Utility::LoadFloat3(&vector.x); }

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VECTOR128 _vector;
	};

	/*----------------------------------------------------------------------
	*  @brief : 内積を算出 (ただし, 返り値はfloatではなくVector128型で返されますので, 後々GetXなどで対応してください.)
	/*----------------------------------------------------------------------*/
	__forceinline float Dot(const Vector3f& left, const Vector3f& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::DotVector3(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : 外積を算出 first, second, thirdに垂直なベクトルを返す
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Cross(const Vector3f& first, const Vector3f& second) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::CrossVector3(first, second);
	}

	/*----------------------------------------------------------------------
	*  @brief : 入力ベクトルの単位ベクトルを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Normalize(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::NormalizeVector3(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief     反射ベクトルを算出
	*  @param[in] const Vector3f& : input  (入射ベクトル)
	*  @param[in] const Vector3f& : normal (反射を行う法線ベクトル)
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Reflect(const Vector3f& input, const Vector3f& normal) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ReflectVector3(input, normal);
	}

	/*----------------------------------------------------------------------
	*  @brief     屈折ベクトルを算出
	*  @param[in] const Vector3f& : input  (入射ベクトル)
	*  @param[in] const Vector3f& : normal (反射を行う法線ベクトル)
	*  @param[in] const float    : refractionIndex (屈折率)
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Refract(const Vector3f& input, const Vector3f& normal, const float refractionIndex) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::RefractVector3(input, normal, refractionIndex);
	}

	__forceinline Vector3f RefractV(const Vector3f& input, const Vector3f& normal, const Vector3f& refractionIndex) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::RefractVector3V(input, normal, refractionIndex);
	}

	/*----------------------------------------------------------------------
	*  @brief : 非常に大きい値か
	/*----------------------------------------------------------------------*/
	__forceinline bool IsInfinite(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::IsInfiniteVector3(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素に根号を取ったものを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Sqrt(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Sqrt(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素の逆数を取り, それに根号を取ったものを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f ReciprocalSqrt(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ReciprocalSqrt(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のsin, cos, tanを返します [rad]
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Sin(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Sin(vector);
	}

	__forceinline Vector3f Cos(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Cos(vector);
	}

	__forceinline Vector3f Tan(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Tan(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のarcsin, arccos, arctanを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f ArcSin(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ArcSin(vector);
	}

	__forceinline Vector3f ArcCos(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ArcCos(vector);
	}

	__forceinline Vector3f ArcTan(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ArcTan(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のsinH, cosH, tanHを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f SinH(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::SinH(vector);
	}

	__forceinline Vector3f CosH(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::CosH(vector);
	}

	__forceinline Vector3f TanH(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::TanH(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素の2^{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Exp2(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Exp2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素の10^{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Exp10(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Exp10(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のe^{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f ExpE(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ExpE(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のlog2{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Log2(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Log2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のlog10{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Log10(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Log10(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のloge{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f LogE(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::LogE(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のPowを返します
	*           base^{power}
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Pow(const Vector3f& base, const Vector3f& power) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Pow(base, power);
	}

	/*----------------------------------------------------------------------
	*  @brief      線形補間を返します
	*  @param[in]  t = 0の時の値
	   @param[in]  t = 1の時の値
	   @param[in]  t : 線形補間の割合
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Lerp(const Vector3f& start, const Vector3f& end, const float t) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Lerp(start, end, t);
	}

	__forceinline Vector3f LerpV(const Vector3f& start, const Vector3f& end, const Vector3f& t) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::LerpV(start, end, t);
	}


	/*----------------------------------------------------------------------
	*  @brief      エルミート補間を返します
	*  @param[in]  始点の位置
	   @param[in]  始点におけるスプラインの接線を表すベクトル
	   @param[in]  終点の位置
	   @param[in]  終点におけるスプラインの接線を表すベクトル
	   @param[in]  t : エルミート補間の割合
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Hermite(const Vector3f& startPosition, const Vector3f& startTangent, const Vector3f& endPosition, const Vector3f& endTangent, const float t) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Hermite(startPosition, startTangent, endPosition, endTangent, t);
	}

	__forceinline Vector3f HermiteV(const Vector3f& startPosition, const Vector3f& startTangent, const Vector3f& endPosition, const Vector3f& endTangent, const Vector3f& t) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::HermiteV(startPosition, startTangent, endPosition, endTangent, t);
	}

	/*----------------------------------------------------------------------
	*  @brief      CatMull-Romスプライン補間の結果を返します。4つの制御点を全て通るスプライン補間
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f CatMullRom(const Vector3f& position0, const Vector3f& position1, const Vector3f& position2, const Vector3f& position3, const float t) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::CatMullRom(position0, position1, position2, position3, t);
	}

	__forceinline Vector3f CatMullRomV(const Vector3f& position0, const Vector3f& position1, const Vector3f& position2, const Vector3f& position3, const Vector3f& t) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::CatMullRomV(position0, position1, position2, position3, t);
	}

	/*----------------------------------------------------------------------
	*  @brief      三角形の重心重み位置を算出
	*              (f>=0 && g>=0 && 1-f-g>=0) の場合、ポイントは三角形の Position0>Position1>Position2 の内側にあります>。
				   (f==0 && g>=0 && 1-f-g>=0) の場合、ポイントは Position0>Position2 行目にあります>。
				   (f>=0 && g==0 && 1-f-g>=0) の場合、ポイントは Position0>Position1> 行目にあります。
				   (f>=0 && g>=0 && 1-f-g==0) の場合、ポイントは Position1>Position2 行目にあります>。
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f BaryCentric(const Vector3f& position0, const Vector3f& position1, const Vector3f& position2, const float f, const float g) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::BaryCentric(position0, position1, position2, f, g);
	}

	__forceinline Vector3f BaryCentricV(const Vector3f& position0, const Vector3f& position1, const Vector3f& position2, const Vector3f& f, const Vector3f& g) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::BaryCentricV(position0, position1, position2, f, g);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素について最小値となる方を選択します
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Min(const Vector3f& left, const Vector3f& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Min(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素について最大値となる方を選択します
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Max(const Vector3f& left, const Vector3f& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Max(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : 最も近い整数に丸める
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Round(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Round(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 切り捨て (指定した数値の端数を切り捨てた整数に丸める。数直線上で0の方向にある次の整数
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Truncate(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Truncate(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 負の方向に丸める
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Floor(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Floor(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 切り上げ 数直線上で正の方向にある次の整数に丸める
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Ceiling(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Ceiling(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 指定範囲内で数値を設定する
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Clamp(const Vector3f& vector, const Vector3f& min, const Vector3f& max) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Clamp(vector, min, max);
	}

	/*----------------------------------------------------------------------
	*  @brief : 1以上には行かないようにする
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Saturate(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Saturate(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 絶対値
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Abs(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Abs(vector);
	}
}
#endif