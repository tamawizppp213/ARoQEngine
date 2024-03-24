//////////////////////////////////////////////////////////////////////////////////
///             @file   GMVector2F.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/03 3:58:44
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_VECTOR2F_HPP
#define GM_VECTOR2F_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Math/Private/Simd/Include/GMSimdMacros.hpp"
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
	struct Vector2f;
	struct Float2;

	/****************************************************************************
	*				  			   GMVector2
	*************************************************************************//**
	*  @class     GMVector2
	*  @brief     SIMD演算用のVector2クラスです. アラインメントを行っているため, データを保持する場合はFloat2を使用してください
	*****************************************************************************/
	struct Float2
	{
	public:
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		float x;
		float y;

		__forceinline bool operator ==(const Float2& vector) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::EqualVector2(*this, vector); }
		__forceinline bool operator !=(const Float2& vector) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::NotEqualVector2(*this, vector); }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor
		Float2() : x(0), y(0) {};

		// @brief : 一つの要素で初期化
		Float2(const float ix) : x(ix), y(ix) {};

		// @brief : 全ての要素で初期化
		Float2(const float ix, const float iy) : x(ix), y(iy) {};

		// @brief : 配列を使って初期化
		explicit Float2(_In_reads_(4) const float* pArray) noexcept : x(pArray[0]), y(pArray[1]) {};

		// @brief : copy constructor
		Float2(const Float2&) = default;

		Float2& operator=(const Float2&) = default;

		// @brief : move constructor
		Float2(Float2&&) = default;

		Float2& operator=(Float2&&) = default;

		operator VECTOR128() const noexcept
		{
			return SIMD_NAME_SPACE::Vector128Utility::LoadFloat2((float*)this);
		}
	};

	/****************************************************************************
	*				  			   GMVector2
	*************************************************************************//**
	*  @class     GMVector2
	*  @brief     SIMD演算用のVector2クラスです. アラインメントを行っているため, データを保持する場合はFloat2を使用してください
	*****************************************************************************/
	struct Vector2f
	{
	public:
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
#pragma region Default Vector
		// @brief : (0, 0, 0, 0)
		static const Vector2f ZERO;

		// @brief : (1, 1, 1, 1)
		static const Vector2f ONE;

		// @brief : (1,0,0,0)
		static const Vector2f X_AXIS;

		// @brief : (0,1,0,0)
		static const Vector2f Y_AXIS;

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

		/*----------------------------------------------------------------------
		*  @brief : Simd vector
		/*----------------------------------------------------------------------*/
		__forceinline VECTOR128 GetSimdVector() const { return _vector; }

		/*----------------------------------------------------------------------
		*  @brief : Float2に変換する
		/*----------------------------------------------------------------------*/
		__forceinline Float2 ToFloat2()
		{
			Float2 value = {};
			SIMD_NAME_SPACE::Vector128Utility::StoreFloat2((float*)&value, _vector);
			return value;
		}

#pragma endregion Getter

#pragma region Setter
		/*----------------------------------------------------------------------
		*  @brief : Floatを使ってVectorのX(, Y, Z, W)要素に値を代入する
		/*----------------------------------------------------------------------*/
		__forceinline void SetX(const float x) { _vector = SIMD_NAME_SPACE::Vector128Utility::SetX(_vector, x); }
		__forceinline void SetY(const float y) { _vector = SIMD_NAME_SPACE::Vector128Utility::SetY(_vector, y); }
#pragma endregion Setter

#pragma region Operator Function
		// @brief : 加算
		__forceinline Vector2f operator+  (const Vector2f& right) const { return Vector2f(SIMD_NAME_SPACE::Vector128Utility::Add(_vector, right._vector)); }

		// @brief : 減算
		__forceinline Vector2f operator- (const Vector2f& right) const { return Vector2f(SIMD_NAME_SPACE::Vector128Utility::Subtract(_vector, right._vector)); }

		// @brief : 乗算
		__forceinline Vector2f operator* (const Vector2f& right) const { return Vector2f(SIMD_NAME_SPACE::Vector128Utility::Multiply(_vector, right._vector)); }

		// @brief : 除算
		__forceinline Vector2f operator/ (const Vector2f& right) const { return Vector2f(SIMD_NAME_SPACE::Vector128Utility::Vector128Utility::Divide(_vector, right._vector)); }

		// @brief : 余り
		__forceinline Vector2f operator% (const Vector2f& right) const { return Vector2f(SIMD_NAME_SPACE::Vector128Utility::Vector128Utility::Mod(_vector, right._vector)); }

		// @brief : スカラー倍
		__forceinline        Vector2f operator* (const float right) const { return Vector2f(SIMD_NAME_SPACE::Vector128Utility::Scale(_vector, right)); }
		__forceinline friend Vector2f operator* (const float left, const Vector2f right) { return Vector2f(SIMD_NAME_SPACE::Vector128Utility::Scale(right._vector, left)); }

		// @brief : スカラーで除算
		__forceinline        Vector2f operator/ (const float right) const { return Vector2f(SIMD_NAME_SPACE::Vector128Utility::Scale(_vector, 1.0f / right)); }
		__forceinline friend Vector2f operator/ (const float left, const Vector2f right) { return Vector2f(SIMD_NAME_SPACE::Vector128Utility::Scale(right._vector, 1.0f / left)); }

		// @brief : 加算 (_vectorではなく, *thisを使って足すのはsimd::Vector128がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector2f& operator +=(const Vector2f& right) { *this = *this + right;  return *this; }

		// @brief : 減算 (_vectorではなく, *thisを使って引くのはsimd::Vector128がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector2f& operator -=(const Vector2f& right) { *this = *this - right; return *this; }

		// @brief : 乗算 (_vectorではなく, *thisを使ってかけるのはsimd::Vector128がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector2f& operator *=(const Vector2f& right) { *this = *this * right; return *this; }

		// @brief : 除算 (_vectorではなく, *thisを使って割るのはsimd::Vector128がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector2f& operator /=(const Vector2f& right) { *this = *this / right; return *this; }

		// @brief : 全ての要素が等しいか
		__forceinline bool operator == (const Vector2f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::EqualVector2(_vector, right._vector); }

		// @brief : 一つでも要素が異なるか
		__forceinline bool operator != (const Vector2f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::NotEqualVector2(_vector, right._vector); }

		// @brief : 全ての要素で大きいか
		__forceinline bool operator >  (const Vector2f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::GreaterVector2(_vector, right._vector); }

		// @brief : 全ての要素で大きいor等しいか
		__forceinline bool operator >=  (const Vector2f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::GreaterOrEqualVector2(_vector, right._vector); }

		// @brief : 全ての要素で小さいか
		__forceinline bool operator <  (const Vector2f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::LessVector2(_vector, right._vector); }

		// @brief : 全ての要素で大きいか
		__forceinline bool operator <= (const Vector2f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::LessOrEqualVector2(_vector, right._vector); }

		// @brief : 直接要素にアクセスします
		__forceinline       float& operator[](const gu::uint32 index) noexcept       { return _vector.m128_f32[index]; }
		__forceinline const float& operator[](const gu::uint32 index) const noexcept { return _vector.m128_f32[index]; }

		/*----------------------------------------------------------------------
		*  @brief : 指定範囲内にあるかどうか -bounds <= vector <= +bounds
		/*----------------------------------------------------------------------*/
		__forceinline bool InBounds(const Vector2f& bounds) noexcept { return SIMD_NAME_SPACE::Vector128Utility::InBoundsVector2(_vector, bounds._vector); }

		__forceinline operator VECTOR128() const { return _vector; }

#pragma endregion Operator Function

#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルの各要素同士の逆数を算出します. (1.0f / vector)
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Reciprocal() const { return SIMD_NAME_SPACE::Vector128Utility::Reciprocal(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 各要素にマイナスを取ったものを返す.
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Negate() const { return SIMD_NAME_SPACE::Vector128Utility::Negate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
		/*----------------------------------------------------------------------*/
		__forceinline float Length() const { return SIMD_NAME_SPACE::Vector128Utility::LengthVector2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 2次ノルムの二乗を算出
		/*----------------------------------------------------------------------*/
		__forceinline float LengthSquared() const { return SIMD_NAME_SPACE::Vector128Utility::LengthSquaredVector2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 内積を算出
		/*----------------------------------------------------------------------*/
		__forceinline float Dot(const Vector2f& right) const { return SIMD_NAME_SPACE::Vector128Utility::DotVector2(_vector, right._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 外積を算出 second, thirdに垂直なベクトルを返す
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Cross(const Vector2f& second) const { return SIMD_NAME_SPACE::Vector128Utility::CrossVector2(_vector, second._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 入力ベクトルの単位ベクトルを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Normalize() const { return SIMD_NAME_SPACE::Vector128Utility::NormalizeVector2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief     反射ベクトルを算出
		*  @param[in] const Vector128&f : input  (入射ベクトル)
		*  @param[in] const Vector128&f : normal (反射を行う法線ベクトル)
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Reflect(const Vector2f& normal) const { return SIMD_NAME_SPACE::Vector128Utility::ReflectVector2(_vector, normal._vector); }

		/*----------------------------------------------------------------------
		*  @brief     屈折ベクトルを算出
		*  @param[in] const Vector2f& : input  (入射ベクトル)
		*  @param[in] const Vector2f& : normal (反射を行う法線ベクトル)
		*  @param[in] const float    : refractionIndex (屈折率)
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Refract(const Vector2f& normal, const float     refractionIndex) noexcept { return SIMD_NAME_SPACE::Vector128Utility::RefractVector2(_vector, normal._vector, refractionIndex); }
		__forceinline Vector2f Refract(const Vector2f& normal, const Vector2f& refractionIndex) noexcept { return SIMD_NAME_SPACE::Vector128Utility::RefractVector2V(_vector, normal._vector, refractionIndex._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 非常に大きい値か
		/*----------------------------------------------------------------------*/
		__forceinline bool IsInfinite() const { return SIMD_NAME_SPACE::Vector128Utility::IsInfiniteVector2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief :  ゼロかどうか
		/*----------------------------------------------------------------------*/
		__forceinline bool IsZero() const { return ZERO == _vector; }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素に根号を取ったものを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Sqrt() const { return SIMD_NAME_SPACE::Vector128Utility::Sqrt(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の逆数を取り, それに根号を取ったものを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f ReciprocalSqrt() const { return SIMD_NAME_SPACE::Vector128Utility::ReciprocalSqrt(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のsin, cos, tanを返します [rad]
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Sin() const { return SIMD_NAME_SPACE::Vector128Utility::Sin(_vector); }
		__forceinline Vector2f Cos() const { return SIMD_NAME_SPACE::Vector128Utility::Cos(_vector); }
		__forceinline Vector2f Tan() const { return SIMD_NAME_SPACE::Vector128Utility::Tan(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のarcsin, arccos, arctanを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f ArcSin() const { return SIMD_NAME_SPACE::Vector128Utility::ArcSin(_vector); }
		__forceinline Vector2f ArcCos() const { return SIMD_NAME_SPACE::Vector128Utility::ArcCos(_vector); }
		__forceinline Vector2f ArcTan() const { return SIMD_NAME_SPACE::Vector128Utility::ArcTan(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のsinH, cosH, tanHを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f SinH() const { return SIMD_NAME_SPACE::Vector128Utility::SinH(_vector); }
		__forceinline Vector2f CosH() const { return SIMD_NAME_SPACE::Vector128Utility::CosH(_vector); }
		__forceinline Vector2f TanH() const { return SIMD_NAME_SPACE::Vector128Utility::TanH(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素に対するsin成分とcos成分を取り出します
		/*----------------------------------------------------------------------*/
		__forceinline void SinCos(Vector2f& sin, Vector2f& cos) noexcept
		{
			SIMD_NAME_SPACE::Vector128Utility::SinCos(_vector, &sin._vector, &cos._vector);
		}

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の2^{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Exp2() const { return SIMD_NAME_SPACE::Vector128Utility::Exp2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の10^{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Exp10() const { return SIMD_NAME_SPACE::Vector128Utility::Exp10(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のe^{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f ExpE() const { return SIMD_NAME_SPACE::Vector128Utility::ExpE(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のlog2{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Log2() const { return SIMD_NAME_SPACE::Vector128Utility::Log2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のlog10{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Log10() const { return SIMD_NAME_SPACE::Vector128Utility::Log10(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のloge{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f LogE() const { return SIMD_NAME_SPACE::Vector128Utility::LogE(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のPowを返します
		*           base^{power}
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Pow(const Vector2f& power) const { return SIMD_NAME_SPACE::Vector128Utility::Pow(_vector, power._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 最も近い整数に丸める
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Round() const { return SIMD_NAME_SPACE::Vector128Utility::Round(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 切り捨て (指定した数値の端数を切り捨てた整数に丸める。数直線上で0の方向にある次の整数
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Truncate() const { return SIMD_NAME_SPACE::Vector128Utility::Truncate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 負の方向に丸める
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Floor() const { return SIMD_NAME_SPACE::Vector128Utility::Floor(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 切り上げ 数直線上で正の方向にある次の整数に丸める
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Ceiling() const { return SIMD_NAME_SPACE::Vector128Utility::Ceiling(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 指定範囲内で数値を設定する
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Clamp(const Vector2f& min, const Vector2f& max) const { return SIMD_NAME_SPACE::Vector128Utility::Clamp(_vector, min._vector, max._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 1以上には行かないようにする
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Saturate() const { return SIMD_NAME_SPACE::Vector128Utility::Saturate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 絶対値
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Abs() const { return SIMD_NAME_SPACE::Vector128Utility::Abs(_vector); }

#pragma endregion Math
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor
		Vector2f() { _vector = SIMD_NAME_SPACE::Vector128Utility::Zero(); }

		// @brief : Float型で生成するコンストラクタ
		Vector2f(const gu::float32 x, const gu::float32 y)
		{
			_vector = SIMD_NAME_SPACE::Vector128Utility::Set(x, y, 0.0f, 0.0f);
		}

		// @brief : Vector128を直接代入するコンストラクタ
		Vector2f(const VECTOR128 vector) : _vector(vector) {};

		// @brief : Float2型で生成するコンストラクタ
		Vector2f(const Float2& vector) { _vector = SIMD_NAME_SPACE::Vector128Utility::LoadFloat2(&vector.x); }

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
	__forceinline float Dot(const Vector2f& left, const Vector2f& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::DotVector2(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : 外積を算出 first, second, thirdに垂直なベクトルを返す
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Cross(const Vector2f& first, const Vector2f& second) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::CrossVector2(first, second);
	}

	/*----------------------------------------------------------------------
	*  @brief : 入力ベクトルの単位ベクトルを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Normalize(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::NormalizeVector2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief     反射ベクトルを算出
	*  @param[in] const Vector2f& : input  (入射ベクトル)
	*  @param[in] const Vector2f& : normal (反射を行う法線ベクトル)
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Reflect(const Vector2f& input, const Vector2f& normal) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ReflectVector2(input, normal);
	}

	/*----------------------------------------------------------------------
	*  @brief     屈折ベクトルを算出
	*  @param[in] const Vector2f& : input  (入射ベクトル)
	*  @param[in] const Vector2f& : normal (反射を行う法線ベクトル)
	*  @param[in] const float    : refractionIndex (屈折率)
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Refract(const Vector2f& input, const Vector2f& normal, const float refractionIndex) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::RefractVector2(input, normal, refractionIndex);
	}

	__forceinline Vector2f RefractV(const Vector2f& input, const Vector2f& normal, const Vector2f& refractionIndex) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::RefractVector2V(input, normal, refractionIndex);
	}

	/*----------------------------------------------------------------------
	*  @brief : 非常に大きい値か
	/*----------------------------------------------------------------------*/
	__forceinline bool IsInfinite(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::IsInfiniteVector2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素に根号を取ったものを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Sqrt(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Sqrt(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素の逆数を取り, それに根号を取ったものを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f ReciprocalSqrt(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ReciprocalSqrt(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のsin, cos, tanを返します [rad]
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Sin(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Sin(vector);
	}

	__forceinline Vector2f Cos(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Cos(vector);
	}

	__forceinline Vector2f Tan(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Tan(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のarcsin, arccos, arctanを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f ArcSin(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ArcSin(vector);
	}

	__forceinline Vector2f ArcCos(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ArcCos(vector);
	}

	__forceinline Vector2f ArcTan(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ArcTan(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のsinH, cosH, tanHを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f SinH(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::SinH(vector);
	}

	__forceinline Vector2f CosH(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::CosH(vector);
	}

	__forceinline Vector2f TanH(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::TanH(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素の2^{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Exp2(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Exp2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素の10^{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Exp10(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Exp10(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のe^{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f ExpE(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ExpE(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のlog2{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Log2(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Log2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のlog10{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Log10(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Log10(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のloge{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f LogE(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::LogE(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のPowを返します
	*           base^{power}
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Pow(const Vector2f& base, const Vector2f& power) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Pow(base, power);
	}

	/*----------------------------------------------------------------------
	*  @brief      線形補間を返します
	*  @param[in]  t = 0の時の値
	   @param[in]  t = 1の時の値
	   @param[in]  t : 線形補間の割合
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Lerp(const Vector2f& start, const Vector2f& end, const float t) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Lerp(start, end, t);
	}

	__forceinline Vector2f LerpV(const Vector2f& start, const Vector2f& end, const Vector2f& t) noexcept
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
	__forceinline Vector2f Hermite(const Vector2f& startPosition, const Vector2f& startTangent, const Vector2f& endPosition, const Vector2f& endTangent, const float t) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Hermite(startPosition, startTangent, endPosition, endTangent, t);
	}

	__forceinline Vector2f HermiteV(const Vector2f& startPosition, const Vector2f& startTangent, const Vector2f& endPosition, const Vector2f& endTangent, const Vector2f& t) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::HermiteV(startPosition, startTangent, endPosition, endTangent, t);
	}

	/*----------------------------------------------------------------------
	*  @brief      CatMull-Romスプライン補間の結果を返します。4つの制御点を全て通るスプライン補間
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f CatMullRom(const Vector2f& position0, const Vector2f& position1, const Vector2f& position2, const Vector2f& position3, const float t) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::CatMullRom(position0, position1, position2, position3, t);
	}

	__forceinline Vector2f CatMullRomV(const Vector2f& position0, const Vector2f& position1, const Vector2f& position2, const Vector2f& position3, const Vector2f& t) noexcept
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
	__forceinline Vector2f BaryCentric(const Vector2f& position0, const Vector2f& position1, const Vector2f& position2, const float f, const float g) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::BaryCentric(position0, position1, position2, f, g);
	}

	__forceinline Vector2f BaryCentricV(const Vector2f& position0, const Vector2f& position1, const Vector2f& position2, const Vector2f& f, const Vector2f& g) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::BaryCentricV(position0, position1, position2, f, g);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素について最小値となる方を選択します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Min(const Vector2f& left, const Vector2f& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Min(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素について最大値となる方を選択します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Max(const Vector2f& left, const Vector2f& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Max(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : 最も近い整数に丸める
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Round(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Round(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 切り捨て (指定した数値の端数を切り捨てた整数に丸める。数直線上で0の方向にある次の整数
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Truncate(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Truncate(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 負の方向に丸める
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Floor(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Floor(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 切り上げ 数直線上で正の方向にある次の整数に丸める
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Ceiling(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Ceiling(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 指定範囲内で数値を設定する
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Clamp(const Vector2f& vector, const Vector2f& min, const Vector2f& max) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Clamp(vector, min, max);
	}

	/*----------------------------------------------------------------------
	*  @brief : 1以上には行かないようにする
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Saturate(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Saturate(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 絶対値
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Abs(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Abs(vector);
	}
}
#endif