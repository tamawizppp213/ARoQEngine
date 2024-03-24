//////////////////////////////////////////////////////////////////////////////////
///             @file   GMVector3F.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/03 3:58:44
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_VECTOR4D_HPP
#define GM_VECTOR4D_HPP

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
	struct Vector4d;
	struct Double4;

	/****************************************************************************
	*				  			   GMVector4
	*************************************************************************//**
	*  @class     GMVector4
	*  @brief     SIMD演算用のVector4クラスです. アラインメントを行っているため, データを保持する場合はDouble4を使用してください
	*****************************************************************************/
	struct Double4
	{
	public:
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		double x;
		double y;
		double z;
		double w;

		__forceinline bool operator ==(const Double4& vector) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::EqualVector4(*this, vector); }
		__forceinline bool operator !=(const Double4& vector) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::NotEqualVector4(*this, vector); }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor
		Double4() : x(0), y(0), z(0), w(0) {};

		// @brief : 一つの要素で初期化
		Double4(const double ix) : x(ix), y(ix), z(ix), w(ix) {};

		// @brief : 全ての要素で初期化
		Double4(const double ix, const double iy, const double iz, const double iw) : x(ix), y(iy), z(iz), w(iw) {};

		// @brief : 配列を使って初期化
		explicit Double4(_In_reads_(4) const double* pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {};

		// @brief : copy constructor
		Double4(const Double4&) = default;

		Double4& operator=(const Double4&) = default;

		// @brief : move constructor
		Double4(Double4&&) = default;

		Double4& operator=(Double4&&) = default;

		operator VECTOR256() const noexcept
		{
			return SIMD_NAME_SPACE::Vector256Utility::LoadDouble4((double*)this);
		}
	};

	/****************************************************************************
	*				  			   GMVector4
	*************************************************************************//**
	*  @class     GMVector4
	*  @brief     SIMD演算用のVector4クラスです. アラインメントを行っているため, データを保持する場合はDouble4を使用してください
	*****************************************************************************/
	struct Vector4d
	{
	public:
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
#pragma region Default Vector
		// @brief : (0, 0, 0, 0)
		static const Vector4d ZERO;

		// @brief : (1, 1, 1, 1)
		static const Vector4d ONE;

		// @brief : (1,0,0,0)
		static const Vector4d X_AXIS;

		// @brief : (0,1,0,0)
		static const Vector4d Y_AXIS;

		// @brief : (0,0,1,0)
		static const Vector4d Z_AXIS;

		// @brief : (0,0,0,q)
		static const Vector4d W_AXIS;
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
		__forceinline gu::double64 GetZ() const { return SIMD_NAME_SPACE::Vector256Utility::GetZ(_vector); }
		__forceinline gu::double64 GetW() const { return SIMD_NAME_SPACE::Vector256Utility::GetW(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : Double4に変換する
		/*----------------------------------------------------------------------*/
		__forceinline Double4 ToDouble4()
		{
			Double4 value = {};
			SIMD_NAME_SPACE::Vector256Utility::StoreDouble4((double*)&value, _vector);
			return value;
		}

#pragma endregion Getter

#pragma region Setter
		/*----------------------------------------------------------------------
		*  @brief : Floatを使ってVectorのX(, Y, Z, W)要素に値を代入する
		/*----------------------------------------------------------------------*/
		__forceinline void SetX(const double x) { _vector = SIMD_NAME_SPACE::Vector256Utility::SetX(_vector, x); }
		__forceinline void SetY(const double y) { _vector = SIMD_NAME_SPACE::Vector256Utility::SetY(_vector, y); }
		__forceinline void SetZ(const double z) { _vector = SIMD_NAME_SPACE::Vector256Utility::SetZ(_vector, z); }
		__forceinline void SetW(const double w) { _vector = SIMD_NAME_SPACE::Vector256Utility::SetW(_vector, w); }
#pragma endregion Setter

#pragma region Operator Function
		// @brief : 加算
		__forceinline Vector4d operator+  (const Vector4d& right) const { return Vector4d(SIMD_NAME_SPACE::Vector256Utility::Add(_vector, right._vector)); }

		// @brief : 減算
		__forceinline Vector4d operator- (const Vector4d& right) const { return Vector4d(SIMD_NAME_SPACE::Vector256Utility::Subtract(_vector, right._vector)); }

		// @brief : 乗算
		__forceinline Vector4d operator* (const Vector4d& right) const { return Vector4d(SIMD_NAME_SPACE::Vector256Utility::Multiply(_vector, right._vector)); }

		// @brief : 除算
		__forceinline Vector4d operator/ (const Vector4d& right) const { return Vector4d(SIMD_NAME_SPACE::Vector256Utility::Vector256Utility::Divide(_vector, right._vector)); }

		// @brief : 余り
		__forceinline Vector4d operator% (const Vector4d& right) const { return Vector4d(SIMD_NAME_SPACE::Vector256Utility::Vector256Utility::Mod(_vector, right._vector)); }

		// @brief : スカラー倍
		__forceinline        Vector4d operator* (const double right) const { return Vector4d(SIMD_NAME_SPACE::Vector256Utility::Scale(_vector, right)); }
		__forceinline friend Vector4d operator* (const double left, const Vector4d right) { return Vector4d(SIMD_NAME_SPACE::Vector256Utility::Scale(right._vector, left)); }

		// @brief : スカラーで除算
		__forceinline        Vector4d operator/ (const double right) const { return Vector4d(SIMD_NAME_SPACE::Vector256Utility::Scale(_vector, 1.0f / right)); }
		__forceinline friend Vector4d operator/ (const double left, const Vector4d right) { return Vector4d(SIMD_NAME_SPACE::Vector256Utility::Scale(right._vector, 1.0f / left)); }

		// @brief : 加算 (_vectorではなく, *thisを使って足すのはsimd::Vector256がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector4d& operator +=(const Vector4d& right) { *this = *this + right;  return *this; }

		// @brief : 減算 (_vectorではなく, *thisを使って引くのはsimd::Vector256がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector4d& operator -=(const Vector4d& right) { *this = *this - right; return *this; }

		// @brief : 乗算 (_vectorではなく, *thisを使ってかけるのはsimd::Vector256がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector4d& operator *=(const Vector4d& right) { *this = *this * right; return *this; }

		// @brief : 除算 (_vectorではなく, *thisを使って割るのはsimd::Vector256がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector4d& operator /=(const Vector4d& right) { *this = *this / right; return *this; }

		// @brief : 全ての要素が等しいか
		__forceinline bool operator == (const Vector4d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::EqualVector4(_vector, right._vector); }

		// @brief : 一つでも要素が異なるか
		__forceinline bool operator != (const Vector4d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::NotEqualVector4(_vector, right._vector); }

		// @brief : 全ての要素で大きいか
		__forceinline bool operator >  (const Vector4d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::GreaterVector4(_vector, right._vector); }

		// @brief : 全ての要素で大きいor等しいか
		__forceinline bool operator >=  (const Vector4d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::GreaterOrEqualVector4(_vector, right._vector); }

		// @brief : 全ての要素で小さいか
		__forceinline bool operator <  (const Vector4d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::LessVector4(_vector, right._vector); }

		// @brief : 全ての要素で大きいか
		__forceinline bool operator <= (const Vector4d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::LessOrEqualVector4(_vector, right._vector); }

		// @brief : 直接要素にアクセスします
		__forceinline       double& operator[](const gu::uint32 index)       noexcept { return _vector.m256d_f64[index]; }
		__forceinline const double& operator[](const gu::uint32 index) const noexcept { return _vector.m256d_f64[index]; }

		/*----------------------------------------------------------------------
		*  @brief : 指定範囲内にあるかどうか -bounds <= vector <= +bounds
		/*----------------------------------------------------------------------*/
		__forceinline bool InBounds(const Vector4d& bounds) noexcept { return SIMD_NAME_SPACE::Vector256Utility::InBoundsVector4(_vector, bounds._vector); }

		__forceinline operator VECTOR256() const { return _vector; }

#pragma endregion Operator Function

#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルの各要素同士の逆数を算出します. (1.0f / vector)
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d Reciprocal() const { return SIMD_NAME_SPACE::Vector256Utility::Reciprocal(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 各要素にマイナスを取ったものを返す.
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d Negate() const { return SIMD_NAME_SPACE::Vector256Utility::Negate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 1次ノルム(各要素の絶対値の和におけるベクトルの大きさ)を算出
		/*----------------------------------------------------------------------*/
		__forceinline double Norm() const { return SIMD_NAME_SPACE::Vector256Utility::NormVector4(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
		/*----------------------------------------------------------------------*/
		__forceinline double NormSquared() const { return SIMD_NAME_SPACE::Vector256Utility::NormSquaredVector4(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 内積を算出
		/*----------------------------------------------------------------------*/
		__forceinline double Dot(const Vector4d& right) const { return SIMD_NAME_SPACE::Vector256Utility::DotVector4(_vector, right._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 外積を算出 second, thirdに垂直なベクトルを返す
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d Cross(const Vector4d& second, const Vector4d& third) const { return SIMD_NAME_SPACE::Vector256Utility::CrossVector4(_vector, second._vector, third._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 入力ベクトルの単位ベクトルを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d Normalize() const { return SIMD_NAME_SPACE::Vector256Utility::NormalizeVector4(_vector); }

		/*----------------------------------------------------------------------
		*  @brief     反射ベクトルを算出
		*  @param[in] const Vector256&f : input  (入射ベクトル)
		*  @param[in] const Vector256&f : normal (反射を行う法線ベクトル)
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d Reflect(const Vector4d& normal) const { return SIMD_NAME_SPACE::Vector256Utility::ReflectVector4(_vector, normal._vector); }

		/*----------------------------------------------------------------------
		*  @brief     屈折ベクトルを算出
		*  @param[in] const Vector4d& : input  (入射ベクトル)
		*  @param[in] const Vector4d& : normal (反射を行う法線ベクトル)
		*  @param[in] const double    : refractionIndex (屈折率)
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d Refract(const Vector4d& normal, const double     refractionIndex) noexcept { return SIMD_NAME_SPACE::Vector256Utility::RefractVector4(_vector, normal._vector, refractionIndex); }
		__forceinline Vector4d Refract(const Vector4d& normal, const Vector4d& refractionIndex) noexcept { return SIMD_NAME_SPACE::Vector256Utility::RefractVector4V(_vector, normal._vector, refractionIndex._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 非常に大きい値か
		/*----------------------------------------------------------------------*/
		__forceinline bool IsInfinite() const { return SIMD_NAME_SPACE::Vector256Utility::IsInfiniteVector4(_vector); }

		/*----------------------------------------------------------------------
		*  @brief :  ゼロかどうか
		/*----------------------------------------------------------------------*/
		__forceinline bool IsZero() const { return ZERO == _vector; }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素に根号を取ったものを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d Sqrt() const { return SIMD_NAME_SPACE::Vector256Utility::Sqrt(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の逆数を取り, それに根号を取ったものを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d ReciprocalSqrt() const { return SIMD_NAME_SPACE::Vector256Utility::ReciprocalSqrt(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のsin, cos, tanを返します [rad]
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d Sin() const { return SIMD_NAME_SPACE::Vector256Utility::Sin(_vector); }
		__forceinline Vector4d Cos() const { return SIMD_NAME_SPACE::Vector256Utility::Cos(_vector); }
		__forceinline Vector4d Tan() const { return SIMD_NAME_SPACE::Vector256Utility::Tan(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のarcsin, arccos, arctanを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d ArcSin() const { return SIMD_NAME_SPACE::Vector256Utility::ArcSin(_vector); }
		__forceinline Vector4d ArcCos() const { return SIMD_NAME_SPACE::Vector256Utility::ArcCos(_vector); }
		__forceinline Vector4d ArcTan() const { return SIMD_NAME_SPACE::Vector256Utility::ArcTan(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のsinH, cosH, tanHを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d SinH() const { return SIMD_NAME_SPACE::Vector256Utility::SinH(_vector); }
		__forceinline Vector4d CosH() const { return SIMD_NAME_SPACE::Vector256Utility::CosH(_vector); }
		__forceinline Vector4d TanH() const { return SIMD_NAME_SPACE::Vector256Utility::TanH(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素に対するsin成分とcos成分を取り出します
		/*----------------------------------------------------------------------*/
		__forceinline void SinCos(Vector4d& sin, Vector4d& cos) noexcept
		{
			SIMD_NAME_SPACE::Vector256Utility::SinCos(_vector, &sin._vector, &cos._vector);
		}

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の2^{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d Exp2() const { return SIMD_NAME_SPACE::Vector256Utility::Exp2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の10^{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d Exp10() const { return SIMD_NAME_SPACE::Vector256Utility::Exp10(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のe^{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d ExpE() const { return SIMD_NAME_SPACE::Vector256Utility::ExpE(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のlog2{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d Log2() const { return SIMD_NAME_SPACE::Vector256Utility::Log2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のlog10{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d Log10() const { return SIMD_NAME_SPACE::Vector256Utility::Log10(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のloge{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d LogE() const { return SIMD_NAME_SPACE::Vector256Utility::LogE(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のPowを返します
		*           base^{power}
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d Pow(const Vector4d& power) const { return SIMD_NAME_SPACE::Vector256Utility::Pow(_vector, power._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 最も近い整数に丸める
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d Round() const { return SIMD_NAME_SPACE::Vector256Utility::Round(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 切り捨て (指定した数値の端数を切り捨てた整数に丸める。数直線上で0の方向にある次の整数
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d Truncate() const { return SIMD_NAME_SPACE::Vector256Utility::Truncate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 負の方向に丸める
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d Floor() const { return SIMD_NAME_SPACE::Vector256Utility::Floor(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 切り上げ 数直線上で正の方向にある次の整数に丸める
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d Ceiling() const { return SIMD_NAME_SPACE::Vector256Utility::Ceiling(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 指定範囲内で数値を設定する
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d Clamp(const Vector4d& min, const Vector4d& max) const { return SIMD_NAME_SPACE::Vector256Utility::Clamp(_vector, min._vector, max._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 1以上には行かないようにする
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d Saturate() const { return SIMD_NAME_SPACE::Vector256Utility::Saturate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 絶対値
		/*----------------------------------------------------------------------*/
		__forceinline Vector4d Abs() const { return SIMD_NAME_SPACE::Vector256Utility::Abs(_vector); }


#pragma endregion Math
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor
		Vector4d() { _vector = SIMD_NAME_SPACE::Vector256Utility::Zero(); }

		// @brief : Float型で生成するコンストラクタ
		Vector4d(const gu::double64 x, const gu::double64 y, const gu::double64 z, const gu::double64 w)
		{
			_vector = SIMD_NAME_SPACE::Vector256Utility::Set(x, y, z, w);
		}

		// @brief : Vector256を直接代入するコンストラクタ
		Vector4d(const VECTOR256 vector) : _vector(vector) {};

		// @brief : Double4型で生成するコンストラクタ
		Vector4d(const Double4& vector) { _vector = SIMD_NAME_SPACE::Vector256Utility::LoadDouble4(&vector.x); }

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
	__forceinline double Dot(const Vector4d& left, const Vector4d& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::DotVector4(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : 外積を算出 first, second, thirdに垂直なベクトルを返す
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d Cross(const Vector4d& first, const Vector4d& second, const Vector4d& third) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::CrossVector4(first, second, third);
	}

	/*----------------------------------------------------------------------
	*  @brief : 入力ベクトルの単位ベクトルを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d Normalize(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::NormalizeVector4(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief     反射ベクトルを算出
	*  @param[in] const Vector4d& : input  (入射ベクトル)
	*  @param[in] const Vector4d& : normal (反射を行う法線ベクトル)
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d Reflect(const Vector4d& input, const Vector4d& normal) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ReflectVector4(input, normal);
	}

	/*----------------------------------------------------------------------
	*  @brief     屈折ベクトルを算出
	*  @param[in] const Vector4d& : input  (入射ベクトル)
	*  @param[in] const Vector4d& : normal (反射を行う法線ベクトル)
	*  @param[in] const double    : refractionIndex (屈折率)
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d Refract(const Vector4d& input, const Vector4d& normal, const double refractionIndex) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::RefractVector4(input, normal, refractionIndex);
	}

	__forceinline Vector4d RefractV(const Vector4d& input, const Vector4d& normal, const Vector4d& refractionIndex) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::RefractVector4V(input, normal, refractionIndex);
	}

	/*----------------------------------------------------------------------
	*  @brief : 非常に大きい値か
	/*----------------------------------------------------------------------*/
	__forceinline bool IsInfinite(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::IsInfiniteVector4(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素に根号を取ったものを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d Sqrt(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Sqrt(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素の逆数を取り, それに根号を取ったものを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d ReciprocalSqrt(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ReciprocalSqrt(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のsin, cos, tanを返します [rad]
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d Sin(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Sin(vector);
	}

	__forceinline Vector4d Cos(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Cos(vector);
	}

	__forceinline Vector4d Tan(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Tan(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のarcsin, arccos, arctanを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d ArcSin(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ArcSin(vector);
	}

	__forceinline Vector4d ArcCos(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ArcCos(vector);
	}

	__forceinline Vector4d ArcTan(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ArcTan(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のsinH, cosH, tanHを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d SinH(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::SinH(vector);
	}

	__forceinline Vector4d CosH(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::CosH(vector);
	}

	__forceinline Vector4d TanH(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::TanH(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素の2^{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d Exp2(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Exp2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素の10^{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d Exp10(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Exp10(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のe^{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d ExpE(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ExpE(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のlog2{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d Log2(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Log2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のlog10{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d Log10(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Log10(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のloge{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d LogE(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::LogE(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のPowを返します
	*           base^{power}
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d Pow(const Vector4d& base, const Vector4d& power) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Pow(base, power);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素について最小値となる方を選択します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d Min(const Vector4d& left, const Vector4d& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Min(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素について最大値となる方を選択します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d Max(const Vector4d& left, const Vector4d& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Max(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : 最も近い整数に丸める
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d Round(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Round(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 切り捨て (指定した数値の端数を切り捨てた整数に丸める。数直線上で0の方向にある次の整数
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d Truncate(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Truncate(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 負の方向に丸める
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d Floor(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Floor(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 切り上げ 数直線上で正の方向にある次の整数に丸める
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d Ceiling(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Ceiling(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 指定範囲内で数値を設定する
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d Clamp(const Vector4d& vector, const Vector4d& min, const Vector4d& max) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Clamp(vector, min, max);
	}

	/*----------------------------------------------------------------------
	*  @brief : 1以上には行かないようにする
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d Saturate(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Saturate(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 絶対値
	/*----------------------------------------------------------------------*/
	__forceinline Vector4d Abs(const Vector4d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Abs(vector);
	}
}
#endif