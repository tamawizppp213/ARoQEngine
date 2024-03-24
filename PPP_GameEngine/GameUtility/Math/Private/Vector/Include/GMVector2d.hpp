//////////////////////////////////////////////////////////////////////////////////
///             @file   GMVector3F.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/03 3:58:44
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_VECTOR2D_HPP
#define GM_VECTOR2D_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Math/Private/Simd/Include/GMSimdMacros.hpp"
#include SIMD_COMPILED_HEADER(GameUtility/Math/Private/Simd/Include, GMSimdVector256)

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
#define VECTOR256 gm::simd::non::Vector256
#elif PLATFORM_CPU_INSTRUCTION_NEON
#define VECTOR256 gm::simd::neon::Vector256 
#elif PLATFORM_CPU_INSTRUCTION_AVX
#define VECTOR256 gm::simd::avx::Vector256
#else
#define VECTOR256 gm::simd::sse::Vector256
#endif
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	struct Vector2d;
	struct Double2;

	/****************************************************************************
	*				  			   GMVector4
	*************************************************************************//**
	*  @class     GMVector4
	*  @brief     SIMD演算用のVector4クラスです. アラインメントを行っているため, データを保持する場合はDouble2を使用してください
	*****************************************************************************/
	struct Double2
	{
	public:
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		double x;
		double y;

		__forceinline bool operator ==(const Double2& vector) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::EqualVector4(*this, vector); }
		__forceinline bool operator !=(const Double2& vector) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::NotEqualVector4(*this, vector); }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor
		Double2() : x(0), y(0) {};

		// @brief : 一つの要素で初期化
		Double2(const double ix) : x(ix), y(ix){};

		// @brief : 全ての要素で初期化
		Double2(const double ix, const double iy, const double iz, const double iw) : x(ix), y(iy){};

		// @brief : 配列を使って初期化
		explicit Double2(_In_reads_(4) const double* pArray) noexcept : x(pArray[0]), y(pArray[1]) {};

		// @brief : copy constructor
		Double2(const Double2&) = default;

		Double2& operator=(const Double2&) = default;

		// @brief : move constructor
		Double2(Double2&&) = default;

		Double2& operator=(Double2&&) = default;

		operator VECTOR256() const noexcept
		{
			return SIMD_NAME_SPACE::Vector256Utility::LoadDouble2((double*)this);
		}
	};

	/****************************************************************************
	*				  			   GMVector4
	*************************************************************************//**
	*  @class     GMVector4
	*  @brief     SIMD演算用のVector4クラスです. アラインメントを行っているため, データを保持する場合はDouble2を使用してください
	*****************************************************************************/
	struct Vector2d
	{
	public:
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
#pragma region Default Vector
		// @brief : (0, 0, 0, 0)
		static const Vector2d ZERO;

		// @brief : (1, 1, 1, 1)
		static const Vector2d ONE;

		// @brief : (1,0,0,0)
		static const Vector2d X_AXIS;

		// @brief : (0,1,0,0)
		static const Vector2d Y_AXIS;

#pragma endregion Default Vector

		/****************************************************************************
		**                Public Function
		*****************************************************************************/

#pragma region Getter
		/*----------------------------------------------------------------------
		*  @brief : FPUレジスタに格納されたX(, Y, Z, W)等の要素を取り出す
		/*----------------------------------------------------------------------*/
		__forceinline gu::double64 GetX() const { return SIMD_NAME_SPACE::Vector256Utility::GetX(_vector); }
		__forceinline gu::double64 GetY() const { return SIMD_NAME_SPACE::Vector256Utility::GetY(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : Double2に変換する
		/*----------------------------------------------------------------------*/
		__forceinline Double2 ToDouble2()
		{
			Double2 value = {};
			SIMD_NAME_SPACE::Vector256Utility::StoreDouble2((double*)&value, _vector);
			return value;
		}

#pragma endregion Getter

#pragma region Setter
		/*----------------------------------------------------------------------
		*  @brief : Floatを使ってVectorのX(, Y, Z, W)要素に値を代入する
		/*----------------------------------------------------------------------*/
		__forceinline void SetX(const double x) { _vector = SIMD_NAME_SPACE::Vector256Utility::SetX(_vector, x); }
		__forceinline void SetY(const double y) { _vector = SIMD_NAME_SPACE::Vector256Utility::SetY(_vector, y); }
#pragma endregion Setter

#pragma region Operator Function
		// @brief : 加算
		__forceinline Vector2d operator+  (const Vector2d& right) const { return Vector2d(SIMD_NAME_SPACE::Vector256Utility::Add(_vector, right._vector)); }

		// @brief : 減算
		__forceinline Vector2d operator- (const Vector2d& right) const { return Vector2d(SIMD_NAME_SPACE::Vector256Utility::Subtract(_vector, right._vector)); }

		// @brief : 乗算
		__forceinline Vector2d operator* (const Vector2d& right) const { return Vector2d(SIMD_NAME_SPACE::Vector256Utility::Multiply(_vector, right._vector)); }

		// @brief : 除算
		__forceinline Vector2d operator/ (const Vector2d& right) const { return Vector2d(SIMD_NAME_SPACE::Vector256Utility::Vector256Utility::Divide(_vector, right._vector)); }

		// @brief : 余り
		__forceinline Vector2d operator% (const Vector2d& right) const { return Vector2d(SIMD_NAME_SPACE::Vector256Utility::Vector256Utility::Mod(_vector, right._vector)); }

		// @brief : スカラー倍
		__forceinline        Vector2d operator* (const double right) const { return Vector2d(SIMD_NAME_SPACE::Vector256Utility::Scale(_vector, right)); }
		__forceinline friend Vector2d operator* (const double left, const Vector2d right) { return Vector2d(SIMD_NAME_SPACE::Vector256Utility::Scale(right._vector, left)); }

		// @brief : スカラーで除算
		__forceinline        Vector2d operator/ (const double right) const { return Vector2d(SIMD_NAME_SPACE::Vector256Utility::Scale(_vector, 1.0f / right)); }
		__forceinline friend Vector2d operator/ (const double left, const Vector2d right) { return Vector2d(SIMD_NAME_SPACE::Vector256Utility::Scale(right._vector, 1.0f / left)); }

		// @brief : 加算 (_vectorではなく, *thisを使って足すのはsimd::Vector256がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector2d& operator +=(const Vector2d& right) { *this = *this + right;  return *this; }

		// @brief : 減算 (_vectorではなく, *thisを使って引くのはsimd::Vector256がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector2d& operator -=(const Vector2d& right) { *this = *this - right; return *this; }

		// @brief : 乗算 (_vectorではなく, *thisを使ってかけるのはsimd::Vector256がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector2d& operator *=(const Vector2d& right) { *this = *this * right; return *this; }

		// @brief : 除算 (_vectorではなく, *thisを使って割るのはsimd::Vector256がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector2d& operator /=(const Vector2d& right) { *this = *this / right; return *this; }

		// @brief : 全ての要素が等しいか
		__forceinline bool operator == (const Vector2d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::EqualVector4(_vector, right._vector); }

		// @brief : 一つでも要素が異なるか
		__forceinline bool operator != (const Vector2d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::NotEqualVector4(_vector, right._vector); }

		// @brief : 全ての要素で大きいか
		__forceinline bool operator >  (const Vector2d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::GreaterVector4(_vector, right._vector); }

		// @brief : 全ての要素で大きいor等しいか
		__forceinline bool operator >=  (const Vector2d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::GreaterOrEqualVector4(_vector, right._vector); }

		// @brief : 全ての要素で小さいか
		__forceinline bool operator <  (const Vector2d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::LessVector4(_vector, right._vector); }

		// @brief : 全ての要素で大きいか
		__forceinline bool operator <= (const Vector2d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::LessOrEqualVector4(_vector, right._vector); }

		// @brief : 直接要素にアクセスします
		__forceinline       double& operator[](const gu::uint32 index) noexcept       { return _vector.m256d_f64[index]; }
		__forceinline const double& operator[](const gu::uint32 index) const noexcept { return _vector.m256d_f64[index]; }
		/*----------------------------------------------------------------------
		*  @brief : 指定範囲内にあるかどうか -bounds <= vector <= +bounds
		/*----------------------------------------------------------------------*/
		__forceinline bool InBounds(const Vector2d& bounds) noexcept { return SIMD_NAME_SPACE::Vector256Utility::InBoundsVector4(_vector, bounds._vector); }

		__forceinline operator VECTOR256() const { return _vector; }

#pragma endregion Operator Function

#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルの各要素同士の逆数を算出します. (1.0f / vector)
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d Reciprocal() const { return SIMD_NAME_SPACE::Vector256Utility::Reciprocal(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 各要素にマイナスを取ったものを返す.
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d Negate() const { return SIMD_NAME_SPACE::Vector256Utility::Negate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
		/*----------------------------------------------------------------------*/
		__forceinline double Length() const { return SIMD_NAME_SPACE::Vector256Utility::LengthVector2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 2次ノルムの二乗を算出
		/*----------------------------------------------------------------------*/
		__forceinline double LengthSquared() const { return SIMD_NAME_SPACE::Vector256Utility::LengthSquaredVector2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 内積を算出
		/*----------------------------------------------------------------------*/
		__forceinline double Dot(const Vector2d& right) const { return SIMD_NAME_SPACE::Vector256Utility::DotVector2(_vector, right._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 外積を算出 second, thirdに垂直なベクトルを返す
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d Cross(const Vector2d& second) const { return SIMD_NAME_SPACE::Vector256Utility::CrossVector2(_vector, second._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 入力ベクトルの単位ベクトルを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d Normalize() const { return SIMD_NAME_SPACE::Vector256Utility::NormalizeVector2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief     反射ベクトルを算出
		*  @param[in] const Vector256&f : input  (入射ベクトル)
		*  @param[in] const Vector256&f : normal (反射を行う法線ベクトル)
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d Reflect(const Vector2d& normal) const { return SIMD_NAME_SPACE::Vector256Utility::ReflectVector2(_vector, normal._vector); }

		/*----------------------------------------------------------------------
		*  @brief     屈折ベクトルを算出
		*  @param[in] const Vector2d& : input  (入射ベクトル)
		*  @param[in] const Vector2d& : normal (反射を行う法線ベクトル)
		*  @param[in] const double    : refractionIndex (屈折率)
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d Refract(const Vector2d& normal, const double     refractionIndex) noexcept { return SIMD_NAME_SPACE::Vector256Utility::RefractVector2(_vector, normal._vector, refractionIndex); }
		__forceinline Vector2d Refract(const Vector2d& normal, const Vector2d& refractionIndex) noexcept { return SIMD_NAME_SPACE::Vector256Utility::RefractVector2V(_vector, normal._vector, refractionIndex._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 非常に大きい値か
		/*----------------------------------------------------------------------*/
		__forceinline bool IsInfinite() const { return SIMD_NAME_SPACE::Vector256Utility::IsInfiniteVector2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief :  ゼロかどうか
		/*----------------------------------------------------------------------*/
		__forceinline bool IsZero() const { return ZERO == _vector; }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素に根号を取ったものを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d Sqrt() const { return SIMD_NAME_SPACE::Vector256Utility::Sqrt(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の逆数を取り, それに根号を取ったものを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d ReciprocalSqrt() const { return SIMD_NAME_SPACE::Vector256Utility::ReciprocalSqrt(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のsin, cos, tanを返します [rad]
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d Sin() const { return SIMD_NAME_SPACE::Vector256Utility::Sin(_vector); }
		__forceinline Vector2d Cos() const { return SIMD_NAME_SPACE::Vector256Utility::Cos(_vector); }
		__forceinline Vector2d Tan() const { return SIMD_NAME_SPACE::Vector256Utility::Tan(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のarcsin, arccos, arctanを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d ArcSin() const { return SIMD_NAME_SPACE::Vector256Utility::ArcSin(_vector); }
		__forceinline Vector2d ArcCos() const { return SIMD_NAME_SPACE::Vector256Utility::ArcCos(_vector); }
		__forceinline Vector2d ArcTan() const { return SIMD_NAME_SPACE::Vector256Utility::ArcTan(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のsinH, cosH, tanHを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d SinH() const { return SIMD_NAME_SPACE::Vector256Utility::SinH(_vector); }
		__forceinline Vector2d CosH() const { return SIMD_NAME_SPACE::Vector256Utility::CosH(_vector); }
		__forceinline Vector2d TanH() const { return SIMD_NAME_SPACE::Vector256Utility::TanH(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素に対するsin成分とcos成分を取り出します
		/*----------------------------------------------------------------------*/
		__forceinline void SinCos(Vector2d& sin, Vector2d& cos) noexcept
		{
			SIMD_NAME_SPACE::Vector256Utility::SinCos(_vector, &sin._vector, &cos._vector);
		}

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の2^{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d Exp2() const { return SIMD_NAME_SPACE::Vector256Utility::Exp2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の10^{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d Exp10() const { return SIMD_NAME_SPACE::Vector256Utility::Exp10(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のe^{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d ExpE() const { return SIMD_NAME_SPACE::Vector256Utility::ExpE(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のlog2{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d Log2() const { return SIMD_NAME_SPACE::Vector256Utility::Log2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のlog10{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d Log10() const { return SIMD_NAME_SPACE::Vector256Utility::Log10(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のloge{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d LogE() const { return SIMD_NAME_SPACE::Vector256Utility::LogE(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のPowを返します
		*           base^{power}
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d Pow(const Vector2d& power) const { return SIMD_NAME_SPACE::Vector256Utility::Pow(_vector, power._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 最も近い整数に丸める
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d Round() const { return SIMD_NAME_SPACE::Vector256Utility::Round(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 切り捨て (指定した数値の端数を切り捨てた整数に丸める。数直線上で0の方向にある次の整数
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d Truncate() const { return SIMD_NAME_SPACE::Vector256Utility::Truncate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 負の方向に丸める
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d Floor() const { return SIMD_NAME_SPACE::Vector256Utility::Floor(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 切り上げ 数直線上で正の方向にある次の整数に丸める
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d Ceiling() const { return SIMD_NAME_SPACE::Vector256Utility::Ceiling(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 指定範囲内で数値を設定する
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d Clamp(const Vector2d& min, const Vector2d& max) const { return SIMD_NAME_SPACE::Vector256Utility::Clamp(_vector, min._vector, max._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 1以上には行かないようにする
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d Saturate() const { return SIMD_NAME_SPACE::Vector256Utility::Saturate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 絶対値
		/*----------------------------------------------------------------------*/
		__forceinline Vector2d Abs() const { return SIMD_NAME_SPACE::Vector256Utility::Abs(_vector); }

#pragma endregion Math
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor
		Vector2d() { _vector = SIMD_NAME_SPACE::Vector256Utility::Zero(); }

		// @brief : Float型で生成するコンストラクタ
		Vector2d(const gu::double64 x, const gu::double64 y)
		{
			_vector = SIMD_NAME_SPACE::Vector256Utility::Set(x, y, 0.0, 0.0);
		}

		// @brief : Vector256を直接代入するコンストラクタ
		Vector2d(const VECTOR256 vector) : _vector(vector) {};

		// @brief : Double2型で生成するコンストラクタ
		Vector2d(const Double2& vector) { _vector = SIMD_NAME_SPACE::Vector256Utility::LoadDouble2(&vector.x); }

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VECTOR256 _vector;
	};

	/*----------------------------------------------------------------------
	*  @brief : 内積を算出 (ただし, 返り値はdoubleではなくVector256型で返されますので, 後々GetXなどで対応してください.)
	/*----------------------------------------------------------------------*/
	__forceinline double Dot(const Vector2d& left, const Vector2d& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::DotVector2(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : 外積を算出 first, second, thirdに垂直なベクトルを返す
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Cross(const Vector2d& first, const Vector2d& second) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::CrossVector2(first, second);
	}

	/*----------------------------------------------------------------------
	*  @brief : 入力ベクトルの単位ベクトルを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Normalize(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::NormalizeVector2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief     反射ベクトルを算出
	*  @param[in] const Vector2d& : input  (入射ベクトル)
	*  @param[in] const Vector2d& : normal (反射を行う法線ベクトル)
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Reflect(const Vector2d& input, const Vector2d& normal) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ReflectVector2(input, normal);
	}

	/*----------------------------------------------------------------------
	*  @brief     屈折ベクトルを算出
	*  @param[in] const Vector2d& : input  (入射ベクトル)
	*  @param[in] const Vector2d& : normal (反射を行う法線ベクトル)
	*  @param[in] const double    : refractionIndex (屈折率)
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Refract(const Vector2d& input, const Vector2d& normal, const double refractionIndex) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::RefractVector2(input, normal, refractionIndex);
	}

	__forceinline Vector2d RefractV(const Vector2d& input, const Vector2d& normal, const Vector2d& refractionIndex) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::RefractVector2V(input, normal, refractionIndex);
	}

	/*----------------------------------------------------------------------
	*  @brief : 非常に大きい値か
	/*----------------------------------------------------------------------*/
	__forceinline bool IsInfinite(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::IsInfiniteVector4(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素に根号を取ったものを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Sqrt(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Sqrt(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素の逆数を取り, それに根号を取ったものを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d ReciprocalSqrt(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ReciprocalSqrt(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のsin, cos, tanを返します [rad]
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Sin(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Sin(vector);
	}

	__forceinline Vector2d Cos(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Cos(vector);
	}

	__forceinline Vector2d Tan(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Tan(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のarcsin, arccos, arctanを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d ArcSin(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ArcSin(vector);
	}

	__forceinline Vector2d ArcCos(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ArcCos(vector);
	}

	__forceinline Vector2d ArcTan(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ArcTan(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のsinH, cosH, tanHを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d SinH(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::SinH(vector);
	}

	__forceinline Vector2d CosH(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::CosH(vector);
	}

	__forceinline Vector2d TanH(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::TanH(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素の2^{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Exp2(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Exp2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素の10^{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Exp10(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Exp10(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のe^{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d ExpE(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ExpE(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のlog2{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Log2(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Log2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のlog10{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Log10(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Log10(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のloge{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d LogE(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::LogE(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のPowを返します
	*           base^{power}
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Pow(const Vector2d& base, const Vector2d& power) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Pow(base, power);
	}

	/*----------------------------------------------------------------------
	*  @brief      線形補間を返します
	*  @param[in]  t = 0の時の値
	   @param[in]  t = 1の時の値
	   @param[in]  t : 線形補間の割合
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Lerp(const Vector2d& start, const Vector2d& end, const double t) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Lerp(start, end, t);
	}

	__forceinline Vector2d LerpV(const Vector2d& start, const Vector2d& end, const Vector2d& t) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::LerpV(start, end, t);
	}


	/*----------------------------------------------------------------------
	*  @brief      エルミート補間を返します
	*  @param[in]  始点の位置
	   @param[in]  始点におけるスプラインの接線を表すベクトル
	   @param[in]  終点の位置
	   @param[in]  終点におけるスプラインの接線を表すベクトル
	   @param[in]  t : エルミート補間の割合
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Hermite(const Vector2d& startPosition, const Vector2d& startTangent, const Vector2d& endPosition, const Vector2d& endTangent, const double t) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Hermite(startPosition, startTangent, endPosition, endTangent, t);
	}

	__forceinline Vector2d HermiteV(const Vector2d& startPosition, const Vector2d& startTangent, const Vector2d& endPosition, const Vector2d& endTangent, const Vector2d& t) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::HermiteV(startPosition, startTangent, endPosition, endTangent, t);
	}

	/*----------------------------------------------------------------------
	*  @brief      CatMull-Romスプライン補間の結果を返します。4つの制御点を全て通るスプライン補間
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d CatMullRom(const Vector2d& position0, const Vector2d& position1, const Vector2d& position2, const Vector2d& position3, const double t) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::CatMullRom(position0, position1, position2, position3, t);
	}

	__forceinline Vector2d CatMullRomV(const Vector2d& position0, const Vector2d& position1, const Vector2d& position2, const Vector2d& position3, const Vector2d& t) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::CatMullRomV(position0, position1, position2, position3, t);
	}

	/*----------------------------------------------------------------------
	*  @brief      三角形の重心重み位置を算出
	*              (f>=0 && g>=0 && 1-f-g>=0) の場合、ポイントは三角形の Position0>Position1>Position2 の内側にあります>。
				   (f==0 && g>=0 && 1-f-g>=0) の場合、ポイントは Position0>Position2 行目にあります>。
				   (f>=0 && g==0 && 1-f-g>=0) の場合、ポイントは Position0>Position1> 行目にあります。
				   (f>=0 && g>=0 && 1-f-g==0) の場合、ポイントは Position1>Position2 行目にあります>。
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d BaryCentric(const Vector2d& position0, const Vector2d& position1, const Vector2d& position2, const double f, const double g) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::BaryCentric(position0, position1, position2, f, g);
	}

	__forceinline Vector2d BaryCentricV(const Vector2d& position0, const Vector2d& position1, const Vector2d& position2, const Vector2d& f, const Vector2d& g) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::BaryCentricV(position0, position1, position2, f, g);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素について最小値となる方を選択します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Min(const Vector2d& left, const Vector2d& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Min(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素について最大値となる方を選択します
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Max(const Vector2d& left, const Vector2d& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Max(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : 最も近い整数に丸める
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Round(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Round(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 切り捨て (指定した数値の端数を切り捨てた整数に丸める。数直線上で0の方向にある次の整数
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Truncate(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Truncate(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 負の方向に丸める
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Floor(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Floor(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 切り上げ 数直線上で正の方向にある次の整数に丸める
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Ceiling(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Ceiling(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 指定範囲内で数値を設定する
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Clamp(const Vector2d& vector, const Vector2d& min, const Vector2d& max) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Clamp(vector, min, max);
	}

	/*----------------------------------------------------------------------
	*  @brief : 1以上には行かないようにする
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Saturate(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Saturate(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 絶対値
	/*----------------------------------------------------------------------*/
	__forceinline Vector2d Abs(const Vector2d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Abs(vector);
	}
}
#endif