//////////////////////////////////////////////////////////////////////////////////
///             @file   GMVector3F.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/03 3:58:44
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_VECTOR3D_HPP
#define GM_VECTOR3D_HPP

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
	struct Vector3d;
	struct Double3;

	/****************************************************************************
	*				  			   GMVector4
	****************************************************************************/
	/* @class     GMVector4
	*  @brief     SIMD演算用のVector4クラスです. アラインメントを行っているため, データを保持する場合はDouble3を使用してください
	*****************************************************************************/
	struct Double3
	{
	public:
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		double x;
		double y;
		double z;

		__forceinline bool operator ==(const Double3& vector) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::EqualVector4(*this, vector); }
		__forceinline bool operator !=(const Double3& vector) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::NotEqualVector4(*this, vector); }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor
		Double3() : x(0), y(0), z(0){};

		// @brief : 一つの要素で初期化
		Double3(const double ix) : x(ix), y(ix), z(ix) {};

		// @brief : 全ての要素で初期化
		Double3(const double ix, const double iy, const double iz) : x(ix), y(iy), z(iz) {};

		// @brief : 配列を使って初期化
		explicit Double3(_In_reads_(4) const double* pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]){};

		// @brief : copy constructor
		Double3(const Double3&) = default;

		Double3& operator=(const Double3&) = default;

		// @brief : move constructor
		Double3(Double3&&) = default;

		Double3& operator=(Double3&&) = default;

		operator VECTOR256() const noexcept
		{
			return SIMD_NAME_SPACE::Vector256Utility::LoadDouble3((double*)this);
		}
	};

	/****************************************************************************
	*				  			   GMVector4
	****************************************************************************/
	/* @class     GMVector4
	*  @brief     SIMD演算用のVector4クラスです. アラインメントを行っているため, データを保持する場合はDouble3を使用してください
	*****************************************************************************/
	struct Vector3d
	{
	public:
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
#pragma region Default Vector
		// @brief : (0, 0, 0, 0)
		static const Vector3d ZERO;

		// @brief : (1, 1, 1, 1)
		static const Vector3d ONE;

		// @brief : (1,0,0,0)
		static const Vector3d X_AXIS;

		// @brief : (0,1,0,0)
		static const Vector3d Y_AXIS;

		// @brief : (0,0,1,0)
		static const Vector3d Z_AXIS;


#pragma endregion Default Vector

		/****************************************************************************
		**                Public Function
		*****************************************************************************/

#pragma region Getter
		/*----------------------------------------------------------------------
		*  @brief : FPUレジスタに格納されたX(, Y, Z, W)等の要素を取り出す
		*----------------------------------------------------------------------*/
		__forceinline gu::double64 GetX() const { return SIMD_NAME_SPACE::Vector256Utility::GetX(_vector); }
		__forceinline gu::double64 GetY() const { return SIMD_NAME_SPACE::Vector256Utility::GetY(_vector); }
		__forceinline gu::double64 GetZ() const { return SIMD_NAME_SPACE::Vector256Utility::GetZ(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : Double3に変換する
		*----------------------------------------------------------------------*/
		__forceinline Double3 ToDouble3()
		{
			Double3 value = {};
			SIMD_NAME_SPACE::Vector256Utility::StoreDouble3((double*)&value, _vector);
			return value;
		}

#pragma endregion Getter

#pragma region Setter
		/*----------------------------------------------------------------------
		*  @brief : Floatを使ってVectorのX(, Y, Z, W)要素に値を代入する
		*----------------------------------------------------------------------*/
		__forceinline void SetX(const double x) { _vector = SIMD_NAME_SPACE::Vector256Utility::SetX(_vector, x); }
		__forceinline void SetY(const double y) { _vector = SIMD_NAME_SPACE::Vector256Utility::SetY(_vector, y); }
		__forceinline void SetZ(const double z) { _vector = SIMD_NAME_SPACE::Vector256Utility::SetZ(_vector, z); }
		
#pragma endregion Setter

#pragma region Operator Function
		// @brief : 加算
		__forceinline Vector3d operator+  (const Vector3d& right) const { return Vector3d(SIMD_NAME_SPACE::Vector256Utility::Add(_vector, right._vector)); }

		// @brief : 減算
		__forceinline Vector3d operator- (const Vector3d& right) const { return Vector3d(SIMD_NAME_SPACE::Vector256Utility::Subtract(_vector, right._vector)); }

		// @brief : 乗算
		__forceinline Vector3d operator* (const Vector3d& right) const { return Vector3d(SIMD_NAME_SPACE::Vector256Utility::Multiply(_vector, right._vector)); }

		// @brief : 除算
		__forceinline Vector3d operator/ (const Vector3d& right) const { return Vector3d(SIMD_NAME_SPACE::Vector256Utility::Vector256Utility::Divide(_vector, right._vector)); }

		// @brief : 余り
		__forceinline Vector3d operator% (const Vector3d& right) const { return Vector3d(SIMD_NAME_SPACE::Vector256Utility::Vector256Utility::Mod(_vector, right._vector)); }

		// @brief : 内積
		__forceinline double operator| (const Vector3d& right) const { return SIMD_NAME_SPACE::Vector256Utility::DotVector3(_vector, right._vector); }

		// @brief : スカラー倍
		__forceinline        Vector3d operator* (const double right) const { return Vector3d(SIMD_NAME_SPACE::Vector256Utility::Scale(_vector, right)); }
		__forceinline friend Vector3d operator* (const double left, const Vector3d right) { return Vector3d(SIMD_NAME_SPACE::Vector256Utility::Scale(right._vector, left)); }

		// @brief : スカラーで除算
		__forceinline        Vector3d operator/ (const double right) const { return Vector3d(SIMD_NAME_SPACE::Vector256Utility::Scale(_vector, 1.0f / right)); }
		__forceinline friend Vector3d operator/ (const double left, const Vector3d right) { return Vector3d(SIMD_NAME_SPACE::Vector256Utility::Scale(right._vector, 1.0f / left)); }

		// @brief : 加算 (_vectorではなく, *thisを使って足すのはsimd::Vector256がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector3d& operator +=(const Vector3d& right) { *this = *this + right;  return *this; }

		// @brief : 減算 (_vectorではなく, *thisを使って引くのはsimd::Vector256がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector3d& operator -=(const Vector3d& right) { *this = *this - right; return *this; }

		// @brief : 乗算 (_vectorではなく, *thisを使ってかけるのはsimd::Vector256がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector3d& operator *=(const Vector3d& right) { *this = *this * right; return *this; }

		// @brief : 除算 (_vectorではなく, *thisを使って割るのはsimd::Vector256がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector3d& operator /=(const Vector3d& right) { *this = *this / right; return *this; }

		// @brief : 全ての要素が等しいか
		__forceinline bool operator == (const Vector3d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::EqualVector4(_vector, right._vector); }

		// @brief : 一つでも要素が異なるか
		__forceinline bool operator != (const Vector3d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::NotEqualVector4(_vector, right._vector); }

		// @brief : 全ての要素で大きいか
		__forceinline bool operator >  (const Vector3d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::GreaterVector4(_vector, right._vector); }

		// @brief : 全ての要素で大きいor等しいか
		__forceinline bool operator >=  (const Vector3d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::GreaterOrEqualVector4(_vector, right._vector); }

		// @brief : 全ての要素で小さいか
		__forceinline bool operator <  (const Vector3d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::LessVector4(_vector, right._vector); }

		// @brief : 全ての要素で大きいか
		__forceinline bool operator <= (const Vector3d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::LessOrEqualVector4(_vector, right._vector); }

		// @brief : 直接要素にアクセスします
		__forceinline       double& operator[](const gu::uint32 index)       noexcept { return _vector.m256d_f64[index]; }
		__forceinline const double& operator[](const gu::uint32 index) const noexcept { return _vector.m256d_f64[index]; }

		/*----------------------------------------------------------------------
		*  @brief : 指定範囲内にあるかどうか -bounds <= vector <= +bounds
		*----------------------------------------------------------------------*/
		__forceinline bool InBounds(const Vector3d& bounds) noexcept { return SIMD_NAME_SPACE::Vector256Utility::InBoundsVector4(_vector, bounds._vector); }

		__forceinline operator VECTOR256() const { return _vector; }

#pragma endregion Operator Function

#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルの各要素同士の逆数を算出します. (1.0f / vector)
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Reciprocal() const { return SIMD_NAME_SPACE::Vector256Utility::Reciprocal(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 各要素にマイナスを取ったものを返す.
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Negate() const { return SIMD_NAME_SPACE::Vector256Utility::Negate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
		*----------------------------------------------------------------------*/
		__forceinline double Length() const { return SIMD_NAME_SPACE::Vector256Utility::LengthVector3(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 2次ノルムの二乗を算出
		*----------------------------------------------------------------------*/
		__forceinline double LengthSquared() const { return SIMD_NAME_SPACE::Vector256Utility::LengthSquaredVector3(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 内積を算出
		*----------------------------------------------------------------------*/
		__forceinline double Dot(const Vector3d& right) const { return SIMD_NAME_SPACE::Vector256Utility::DotVector3(_vector, right._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 外積を算出 secondに垂直なベクトルを返す
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Cross(const Vector3d& second) const { return SIMD_NAME_SPACE::Vector256Utility::CrossVector3(_vector, second._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 入力ベクトルの単位ベクトルを返します
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Normalize() const { return SIMD_NAME_SPACE::Vector256Utility::NormalizeVector3(_vector); }

		/*----------------------------------------------------------------------
		*  @brief     反射ベクトルを算出
		*  @param[in] const Vector256&f : input  (入射ベクトル)
		*  @param[in] const Vector256&f : normal (反射を行う法線ベクトル)
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Reflect(const Vector3d& normal) const { return SIMD_NAME_SPACE::Vector256Utility::ReflectVector3(_vector, normal._vector); }

		/*----------------------------------------------------------------------
		*  @brief     屈折ベクトルを算出
		*  @param[in] const Vector3d& : input  (入射ベクトル)
		*  @param[in] const Vector3d& : normal (反射を行う法線ベクトル)
		*  @param[in] const double    : refractionIndex (屈折率)
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Refract(const Vector3d& normal, const double     refractionIndex) noexcept { return SIMD_NAME_SPACE::Vector256Utility::RefractVector3(_vector, normal._vector, refractionIndex); }
		__forceinline Vector3d Refract(const Vector3d& normal, const Vector3d& refractionIndex) noexcept { return SIMD_NAME_SPACE::Vector256Utility::RefractVector3V(_vector, normal._vector, refractionIndex._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 非常に大きい値か
		*----------------------------------------------------------------------*/
		__forceinline bool IsInfinite() const { return SIMD_NAME_SPACE::Vector256Utility::IsInfiniteVector3(_vector); }

		/*----------------------------------------------------------------------
		*  @brief :  ゼロかどうか
		*----------------------------------------------------------------------*/
		__forceinline bool IsZero() const { return ZERO == _vector; }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素に根号を取ったものを返します
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Sqrt() const { return SIMD_NAME_SPACE::Vector256Utility::Sqrt(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の逆数を取り, それに根号を取ったものを返します
		*----------------------------------------------------------------------*/
		__forceinline Vector3d ReciprocalSqrt() const { return SIMD_NAME_SPACE::Vector256Utility::ReciprocalSqrt(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のsin, cos, tanを返します [rad]
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Sin() const { return SIMD_NAME_SPACE::Vector256Utility::Sin(_vector); }
		__forceinline Vector3d Cos() const { return SIMD_NAME_SPACE::Vector256Utility::Cos(_vector); }
		__forceinline Vector3d Tan() const { return SIMD_NAME_SPACE::Vector256Utility::Tan(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のarcsin, arccos, arctanを返します
		*----------------------------------------------------------------------*/
		__forceinline Vector3d ArcSin() const { return SIMD_NAME_SPACE::Vector256Utility::ArcSin(_vector); }
		__forceinline Vector3d ArcCos() const { return SIMD_NAME_SPACE::Vector256Utility::ArcCos(_vector); }
		__forceinline Vector3d ArcTan() const { return SIMD_NAME_SPACE::Vector256Utility::ArcTan(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のsinH, cosH, tanHを返します
		*----------------------------------------------------------------------*/
		__forceinline Vector3d SinH() const { return SIMD_NAME_SPACE::Vector256Utility::SinH(_vector); }
		__forceinline Vector3d CosH() const { return SIMD_NAME_SPACE::Vector256Utility::CosH(_vector); }
		__forceinline Vector3d TanH() const { return SIMD_NAME_SPACE::Vector256Utility::TanH(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素に対するsin成分とcos成分を取り出します
		*----------------------------------------------------------------------*/
		__forceinline void SinCos(Vector3d& sin, Vector3d& cos) noexcept
		{
			SIMD_NAME_SPACE::Vector256Utility::SinCos(_vector, &sin._vector, &cos._vector);
		}

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の2^{vector}を返します
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Exp2() const { return SIMD_NAME_SPACE::Vector256Utility::Exp2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の10^{vector}を返します
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Exp10() const { return SIMD_NAME_SPACE::Vector256Utility::Exp10(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のe^{vector}を返します
		*----------------------------------------------------------------------*/
		__forceinline Vector3d ExpE() const { return SIMD_NAME_SPACE::Vector256Utility::ExpE(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のlog2{vector}を返します
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Log2() const { return SIMD_NAME_SPACE::Vector256Utility::Log2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のlog10{vector}を返します
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Log10() const { return SIMD_NAME_SPACE::Vector256Utility::Log10(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のloge{vector}を返します
		*----------------------------------------------------------------------*/
		__forceinline Vector3d LogE() const { return SIMD_NAME_SPACE::Vector256Utility::LogE(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のPowを返します
		*           base^{power}
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Pow(const Vector3d& power) const { return SIMD_NAME_SPACE::Vector256Utility::Pow(_vector, power._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 最も近い整数に丸める
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Round() const { return SIMD_NAME_SPACE::Vector256Utility::Round(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 切り捨て (指定した数値の端数を切り捨てた整数に丸める。数直線上で0の方向にある次の整数
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Truncate() const { return SIMD_NAME_SPACE::Vector256Utility::Truncate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 負の方向に丸める
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Floor() const { return SIMD_NAME_SPACE::Vector256Utility::Floor(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 切り上げ 数直線上で正の方向にある次の整数に丸める
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Ceiling() const { return SIMD_NAME_SPACE::Vector256Utility::Ceiling(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 指定範囲内で数値を設定する
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Clamp(const Vector3d& min, const Vector3d& max) const { return SIMD_NAME_SPACE::Vector256Utility::Clamp(_vector, min._vector, max._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 1以上には行かないようにする
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Saturate() const { return SIMD_NAME_SPACE::Vector256Utility::Saturate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 絶対値
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Abs() const { return SIMD_NAME_SPACE::Vector256Utility::Abs(_vector); }

#pragma endregion Math
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor
		Vector3d() { _vector = SIMD_NAME_SPACE::Vector256Utility::Zero(); }

		// @brief : Float型で生成するコンストラクタ
		Vector3d(const gu::double64 x, const gu::double64 y, const gu::double64 z)
		{
			_vector = SIMD_NAME_SPACE::Vector256Utility::Set(x, y, z, 0.0);
		}

		// @brief : Vector256を直接代入するコンストラクタ
		Vector3d(const VECTOR256 vector) : _vector(vector) {};

		// @brief : Double3型で生成するコンストラクタ
		Vector3d(const Double3& vector) { _vector = SIMD_NAME_SPACE::Vector256Utility::LoadDouble3(&vector.x); }

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		VECTOR256 _vector;
	};

	/*----------------------------------------------------------------------
	*  @brief : 内積を算出 (ただし, 返り値はdoubleではなくVector256型で返されますので, 後々GetXなどで対応してください.)
	*----------------------------------------------------------------------*/
	__forceinline double Dot(const Vector3d& left, const Vector3d& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::DotVector3(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : 外積を算出 first, second, thirdに垂直なベクトルを返す
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Cross(const Vector3d& first, const Vector3d& second) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::CrossVector3(first, second);
	}

	/*----------------------------------------------------------------------
	*  @brief : 入力ベクトルの単位ベクトルを返します
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Normalize(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::NormalizeVector3(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief     反射ベクトルを算出
	*  @param[in] const Vector3d& : input  (入射ベクトル)
	*  @param[in] const Vector3d& : normal (反射を行う法線ベクトル)
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Reflect(const Vector3d& input, const Vector3d& normal) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ReflectVector3(input, normal);
	}

	/*----------------------------------------------------------------------
	*  @brief     屈折ベクトルを算出
	*  @param[in] const Vector3d& : input  (入射ベクトル)
	*  @param[in] const Vector3d& : normal (反射を行う法線ベクトル)
	*  @param[in] const double    : refractionIndex (屈折率)
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Refract(const Vector3d& input, const Vector3d& normal, const double refractionIndex) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::RefractVector3(input, normal, refractionIndex);
	}

	__forceinline Vector3d RefractV(const Vector3d& input, const Vector3d& normal, const Vector3d& refractionIndex) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::RefractVector3V(input, normal, refractionIndex);
	}

	/*----------------------------------------------------------------------
	*  @brief : 非常に大きい値か
	*----------------------------------------------------------------------*/
	__forceinline bool IsInfinite(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::IsInfiniteVector3(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素に根号を取ったものを返します
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Sqrt(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Sqrt(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素の逆数を取り, それに根号を取ったものを返します
	*----------------------------------------------------------------------*/
	__forceinline Vector3d ReciprocalSqrt(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ReciprocalSqrt(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のsin, cos, tanを返します [rad]
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Sin(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Sin(vector);
	}

	__forceinline Vector3d Cos(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Cos(vector);
	}

	__forceinline Vector3d Tan(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Tan(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のarcsin, arccos, arctanを返します
	*----------------------------------------------------------------------*/
	__forceinline Vector3d ArcSin(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ArcSin(vector);
	}

	__forceinline Vector3d ArcCos(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ArcCos(vector);
	}

	__forceinline Vector3d ArcTan(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ArcTan(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のsinH, cosH, tanHを返します
	*----------------------------------------------------------------------*/
	__forceinline Vector3d SinH(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::SinH(vector);
	}

	__forceinline Vector3d CosH(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::CosH(vector);
	}

	__forceinline Vector3d TanH(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::TanH(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素の2^{vector}を返します
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Exp2(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Exp2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素の10^{vector}を返します
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Exp10(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Exp10(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のe^{vector}を返します
	*----------------------------------------------------------------------*/
	__forceinline Vector3d ExpE(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ExpE(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のlog2{vector}を返します
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Log2(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Log2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のlog10{vector}を返します
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Log10(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Log10(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のloge{vector}を返します
	*----------------------------------------------------------------------*/
	__forceinline Vector3d LogE(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::LogE(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のPowを返します
	*           base^{power}
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Pow(const Vector3d& base, const Vector3d& power) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Pow(base, power);
	}

	/*----------------------------------------------------------------------
	*  @brief      線形補間を返します
	*  @param[in]  t = 0の時の値
	   @param[in]  t = 1の時の値
	   @param[in]  t : 線形補間の割合
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Lerp(const Vector3d& start, const Vector3d& end, const double t) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Lerp(start, end, t);
	}

	__forceinline Vector3d LerpV(const Vector3d& start, const Vector3d& end, const Vector3d& t) noexcept
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
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Hermite(const Vector3d& startPosition, const Vector3d& startTangent, const Vector3d& endPosition, const Vector3d& endTangent, const double t) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Hermite(startPosition, startTangent, endPosition, endTangent, t);
	}

	__forceinline Vector3d HermiteV(const Vector3d& startPosition, const Vector3d& startTangent, const Vector3d& endPosition, const Vector3d& endTangent, const Vector3d& t) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::HermiteV(startPosition, startTangent, endPosition, endTangent, t);
	}

	/*----------------------------------------------------------------------
	*  @brief      CatMull-Romスプライン補間の結果を返します。4つの制御点を全て通るスプライン補間
	*----------------------------------------------------------------------*/
	__forceinline Vector3d CatMullRom(const Vector3d& position0, const Vector3d& position1, const Vector3d& position2, const Vector3d& position3, const double t) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::CatMullRom(position0, position1, position2, position3, t);
	}

	__forceinline Vector3d CatMullRomV(const Vector3d& position0, const Vector3d& position1, const Vector3d& position2, const Vector3d& position3, const Vector3d& t) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::CatMullRomV(position0, position1, position2, position3, t);
	}

	/*----------------------------------------------------------------------
	*  @brief      三角形の重心重み位置を算出
	*              (f>=0 && g>=0 && 1-f-g>=0) の場合、ポイントは三角形の Position0>Position1>Position2 の内側にあります>。
				   (f==0 && g>=0 && 1-f-g>=0) の場合、ポイントは Position0>Position2 行目にあります>。
				   (f>=0 && g==0 && 1-f-g>=0) の場合、ポイントは Position0>Position1> 行目にあります。
				   (f>=0 && g>=0 && 1-f-g==0) の場合、ポイントは Position1>Position2 行目にあります>。
	*----------------------------------------------------------------------*/
	__forceinline Vector3d BaryCentric(const Vector3d& position0, const Vector3d& position1, const Vector3d& position2, const double f, const double g) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::BaryCentric(position0, position1, position2, f, g);
	}

	__forceinline Vector3d BaryCentricV(const Vector3d& position0, const Vector3d& position1, const Vector3d& position2, const Vector3d& f, const Vector3d& g) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::BaryCentricV(position0, position1, position2, f, g);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素について最小値となる方を選択します
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Min(const Vector3d& left, const Vector3d& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Min(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素について最大値となる方を選択します
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Max(const Vector3d& left, const Vector3d& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Max(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : 最も近い整数に丸める
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Round(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Round(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 切り捨て (指定した数値の端数を切り捨てた整数に丸める。数直線上で0の方向にある次の整数
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Truncate(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Truncate(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 負の方向に丸める
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Floor(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Floor(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 切り上げ 数直線上で正の方向にある次の整数に丸める
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Ceiling(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Ceiling(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 指定範囲内で数値を設定する
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Clamp(const Vector3d& vector, const Vector3d& min, const Vector3d& max) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Clamp(vector, min, max);
	}

	/*----------------------------------------------------------------------
	*  @brief : 1以上には行かないようにする
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Saturate(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Saturate(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 絶対値
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Abs(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Abs(vector);
	}
}
#endif