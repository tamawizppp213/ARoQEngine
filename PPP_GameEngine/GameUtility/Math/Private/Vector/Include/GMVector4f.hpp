//////////////////////////////////////////////////////////////////////////////////
///             @file   GMVector3F.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/03 3:58:44
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_VECTOR4F_HPP
#define GM_VECTOR4F_HPP

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
	struct Vector4f;
	struct Float4;

	/****************************************************************************
	*				  			   GMVector4
	*************************************************************************//**
	*  @class     GMVector4
	*  @brief     SIMD演算用のVector4クラスです. アラインメントを行っているため, データを保持する場合はFloat4を使用してください
	*****************************************************************************/
	struct Float4
	{
	public:
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		float x;
		float y;
		float z;
		float w;

		__forceinline bool operator ==(const Float4& vector) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::EqualVector4(*this, vector); }
		__forceinline bool operator !=(const Float4& vector) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::NotEqualVector4(*this, vector); }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor
		Float4() : x(0), y(0), z(0), w(0) {};

		// @brief : 一つの要素で初期化
		Float4(const float ix) : x(ix), y(ix), z(ix), w(ix) {};

		// @brief : 全ての要素で初期化
		Float4(const float ix, const float iy, const float iz, const float iw) : x(ix), y(iy), z(iz), w(iw) {};

		// @brief : 配列を使って初期化
		explicit Float4(_In_reads_(4) const float* pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {};

		// @brief : copy constructor
		Float4(const Float4&) = default;

		Float4& operator=(const Float4&) = default;

		// @brief : move constructor
		Float4(Float4&&) = default;

		Float4& operator=(Float4&&) = default;

		operator VECTOR128() const noexcept
		{
			return SIMD_NAME_SPACE::Vector128Utility::LoadFloat4((float*)this);
		}
	};

	/****************************************************************************
	*				  			   GMVector4
	*************************************************************************//**
	*  @class     GMVector4
	*  @brief     SIMD演算用のVector4クラスです. アラインメントを行っているため, データを保持する場合はFloat4を使用してください
	*****************************************************************************/
	struct Vector4f
	{
	public:
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		#pragma region Default Vector
		// @brief : (0, 0, 0, 0)
		static const Vector4f ZERO;

		// @brief : (1, 1, 1, 1)
		static const Vector4f ONE;

		// @brief : (1,0,0,0)
		static const Vector4f XAXIS;

		// @brief : (0,1,0,0)
		static const Vector4f YAXIS;

		// @brief : (0,0,1,0)
		static const Vector4f ZAXIS;

		// @brief : (0,0,0,1)
		static const Vector4f WAXIS;

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
		__forceinline gu::float32 GetW() const { return SIMD_NAME_SPACE::Vector128Utility::GetW(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : Float4に変換する
		/*----------------------------------------------------------------------*/
		__forceinline Float4 ToFloat4()
		{
			Float4 value = {};
			SIMD_NAME_SPACE::Vector128Utility::StoreFloat4((float*)&value, _vector);
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
		__forceinline void SetW(const float w) { _vector = SIMD_NAME_SPACE::Vector128Utility::SetW(_vector, w); }
		#pragma endregion Setter

		#pragma region Operator Function
		// @brief : 加算
		__forceinline Vector4f operator+  (const Vector4f& right) const { return Vector4f(SIMD_NAME_SPACE::Vector128Utility::Add(_vector, right._vector)); }
		
		// @brief : 減算
		__forceinline Vector4f operator- (const Vector4f& right) const { return Vector4f(SIMD_NAME_SPACE::Vector128Utility::Subtract(_vector, right._vector)); }

		// @brief : 乗算
		__forceinline Vector4f operator* (const Vector4f& right) const { return Vector4f(SIMD_NAME_SPACE::Vector128Utility::Multiply(_vector, right._vector)); }

		// @brief : 除算
		__forceinline Vector4f operator/ (const Vector4f& right) const { return Vector4f(SIMD_NAME_SPACE::Vector128Utility::Vector128Utility::Divide(_vector, right._vector)); }

		// @brief : 余り
		__forceinline Vector4f operator% (const Vector4f& right) const { return Vector4f(SIMD_NAME_SPACE::Vector128Utility::Vector128Utility::Mod(_vector, right._vector)); }

		// @brief : スカラー倍
		__forceinline        Vector4f operator* (const float right) const { return Vector4f(SIMD_NAME_SPACE::Vector128Utility::Scale(_vector, right)); }
		__forceinline friend Vector4f operator* (const float left, const Vector4f right) { return Vector4f(SIMD_NAME_SPACE::Vector128Utility::Scale(right._vector, left)); }

		// @brief : スカラーで除算
		__forceinline        Vector4f operator/ (const float right) const                { return Vector4f(SIMD_NAME_SPACE::Vector128Utility::Scale(_vector, 1.0f / right)); }
		__forceinline friend Vector4f operator/ (const float left, const Vector4f right) { return Vector4f(SIMD_NAME_SPACE::Vector128Utility::Scale(right._vector, 1.0f / left)); }

		// @brief : 加算 (_vectorではなく, *thisを使って足すのはsimd::Vector128がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector4f& operator +=(const Vector4f& right) { *this = *this + right;  return *this; }

		// @brief : 減算 (_vectorではなく, *thisを使って引くのはsimd::Vector128がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector4f& operator -=(const Vector4f& right) { *this = *this - right; return *this; }

		// @brief : 乗算 (_vectorではなく, *thisを使ってかけるのはsimd::Vector128がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector4f& operator *=(const Vector4f& right) { *this = *this * right; return *this; }

		// @brief : 除算 (_vectorではなく, *thisを使って割るのはsimd::Vector128がマクロに応じて変わるため, ラップする必要がある)
		__forceinline Vector4f& operator /=(const Vector4f& right) { *this = *this / right; return *this; }

		// @brief : 全ての要素が等しいか
		__forceinline bool operator == (const Vector4f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::EqualVector4(_vector, right._vector); }

		// @brief : 一つでも要素が異なるか
		__forceinline bool operator != (const Vector4f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::NotEqualVector4(_vector, right._vector); }
		
		// @brief : 全ての要素で大きいか
		__forceinline bool operator >  (const Vector4f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::GreaterVector4(_vector, right._vector); }

		// @brief : 全ての要素で大きいor等しいか
		__forceinline bool operator >=  (const Vector4f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::GreaterOrEqualVector4(_vector, right._vector); }
		
		// @brief : 全ての要素で小さいか
		__forceinline bool operator <  (const Vector4f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::LessVector4(_vector, right._vector); }

		// @brief : 全ての要素で大きいか
		__forceinline bool operator <= (const Vector4f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::LessOrEqualVector4(_vector, right._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 指定範囲内にあるかどうか -bounds <= vector <= +bounds
		/*----------------------------------------------------------------------*/
		__forceinline bool InBounds(const Vector4f& bounds) noexcept { return SIMD_NAME_SPACE::Vector128Utility::InBoundsVector4(_vector, bounds._vector); }
		
		__forceinline operator VECTOR128() const { return _vector; }

		#pragma endregion Operator Function

		#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : 2つのベクトルの各要素同士の逆数を算出します. (1.0f / vector)
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Reciprocal() const { return SIMD_NAME_SPACE::Vector128Utility::Reciprocal(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 各要素にマイナスを取ったものを返す.
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Negate() const { return SIMD_NAME_SPACE::Vector128Utility::Negate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 1次ノルム(各要素の絶対値の和におけるベクトルの大きさ)を算出
		/*----------------------------------------------------------------------*/
		__forceinline float Norm() const { return SIMD_NAME_SPACE::Vector128Utility::NormVector4(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
		/*----------------------------------------------------------------------*/
		__forceinline float NormSquared() const { return SIMD_NAME_SPACE::Vector128Utility::NormSquaredVector4(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 内積を算出 
		/*----------------------------------------------------------------------*/
		__forceinline float Dot(const Vector4f& right) const { return SIMD_NAME_SPACE::Vector128Utility::DotVector4(_vector, right._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 外積を算出 second, thirdに垂直なベクトルを返す
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Cross(const Vector4f& second, const Vector4f& third) const { return SIMD_NAME_SPACE::Vector128Utility::CrossVector4(_vector, second._vector, third._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 入力ベクトルの単位ベクトルを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Normalize() const { return SIMD_NAME_SPACE::Vector128Utility::NormalizeVector4(_vector); }

		/*----------------------------------------------------------------------
		*  @brief     反射ベクトルを算出
		*  @param[in] const Vector128&f : input  (入射ベクトル)
		*  @param[in] const Vector128&f : normal (反射を行う法線ベクトル)
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Reflect(const Vector4f& normal) const { return SIMD_NAME_SPACE::Vector128Utility::ReflectVector4(_vector, normal._vector); }

		/*----------------------------------------------------------------------
		*  @brief     屈折ベクトルを算出
		*  @param[in] const Vector4f& : input  (入射ベクトル)
		*  @param[in] const Vector4f& : normal (反射を行う法線ベクトル)
		*  @param[in] const float    : refractionIndex (屈折率)
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Refract(const Vector4f& normal, const float     refractionIndex) noexcept { return SIMD_NAME_SPACE::Vector128Utility::RefractVector4(_vector, normal._vector, refractionIndex); }
		__forceinline Vector4f Refract(const Vector4f& normal, const Vector4f& refractionIndex) noexcept { return SIMD_NAME_SPACE::Vector128Utility::RefractVector4V(_vector, normal._vector, refractionIndex._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 非常に大きい値か
		/*----------------------------------------------------------------------*/
		__forceinline bool IsInfinite() const { return SIMD_NAME_SPACE::Vector128Utility::IsInfiniteVector4(_vector); }

		/*----------------------------------------------------------------------
		*  @brief :  ゼロかどうか
		/*----------------------------------------------------------------------*/
		__forceinline bool IsZero() const { return ZERO == _vector; }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素に根号を取ったものを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Sqrt() const { return SIMD_NAME_SPACE::Vector128Utility::Sqrt(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の逆数を取り, それに根号を取ったものを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f ReciprocalSqrt() const { return SIMD_NAME_SPACE::Vector128Utility::ReciprocalSqrt(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のsin, cos, tanを返します [rad]
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Sin() const { return SIMD_NAME_SPACE::Vector128Utility::Sin(_vector); }
		__forceinline Vector4f Cos() const { return SIMD_NAME_SPACE::Vector128Utility::Cos(_vector); }
		__forceinline Vector4f Tan() const { return SIMD_NAME_SPACE::Vector128Utility::Tan(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のarcsin, arccos, arctanを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f ArcSin() const { return SIMD_NAME_SPACE::Vector128Utility::ArcSin(_vector); }
		__forceinline Vector4f ArcCos() const { return SIMD_NAME_SPACE::Vector128Utility::ArcCos(_vector); }
		__forceinline Vector4f ArcTan() const { return SIMD_NAME_SPACE::Vector128Utility::ArcTan(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のsinH, cosH, tanHを返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f SinH() const { return SIMD_NAME_SPACE::Vector128Utility::SinH(_vector); }
		__forceinline Vector4f CosH() const { return SIMD_NAME_SPACE::Vector128Utility::CosH(_vector); }
		__forceinline Vector4f TanH() const { return SIMD_NAME_SPACE::Vector128Utility::TanH(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素に対するsin成分とcos成分を取り出します
		/*----------------------------------------------------------------------*/
		__forceinline void SinCos(Vector4f& sin, Vector4f& cos) noexcept
		{
			SIMD_NAME_SPACE::Vector128Utility::SinCos(_vector, &sin._vector, &cos._vector);
		}

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の2^{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Exp2() const { return SIMD_NAME_SPACE::Vector128Utility::Exp2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素の10^{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Exp10() const { return SIMD_NAME_SPACE::Vector128Utility::Exp10(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のe^{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f ExpE() const { return SIMD_NAME_SPACE::Vector128Utility::ExpE(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のlog2{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Log2() const { return SIMD_NAME_SPACE::Vector128Utility::Log2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のlog10{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Log10() const { return SIMD_NAME_SPACE::Vector128Utility::Log10(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のloge{vector}を返します
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f LogE() const { return SIMD_NAME_SPACE::Vector128Utility::LogE(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 全ての要素のPowを返します
		*           base^{power}
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Pow(const Vector4f& power) const { return SIMD_NAME_SPACE::Vector128Utility::Pow(_vector, power._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 最も近い整数に丸める
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Round() const { return SIMD_NAME_SPACE::Vector128Utility::Round(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 切り捨て (指定した数値の端数を切り捨てた整数に丸める。数直線上で0の方向にある次の整数
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Truncate() const { return SIMD_NAME_SPACE::Vector128Utility::Truncate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 負の方向に丸める
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Floor() const { return SIMD_NAME_SPACE::Vector128Utility::Floor(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 切り上げ 数直線上で正の方向にある次の整数に丸める
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Ceiling() const { return SIMD_NAME_SPACE::Vector128Utility::Ceiling(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 指定範囲内で数値を設定する
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Clamp(const Vector4f& min, const Vector4f& max) const { return SIMD_NAME_SPACE::Vector128Utility::Clamp(_vector, min._vector, max._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 1以上には行かないようにする
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Saturate() const { return SIMD_NAME_SPACE::Vector128Utility::Saturate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 絶対値
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Abs() noexcept { return SIMD_NAME_SPACE::Vector128Utility::Abs(_vector); }

		#pragma endregion Math
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor
		Vector4f() { _vector = SIMD_NAME_SPACE::Vector128Utility::Zero(); }

		// @brief : Float型で生成するコンストラクタ
		Vector4f(const gu::float32 x, const gu::float32 y, const gu::float32 z, const gu::float32 w) 
		{
			_vector = SIMD_NAME_SPACE::Vector128Utility::Set(x, y, z, w);
		}

		// @brief : Vector128を直接代入するコンストラクタ
		Vector4f(const VECTOR128 vector) : _vector(vector) {};

		// @brief : Float4型で生成するコンストラクタ
		Vector4f(const Float4& vector) { _vector = SIMD_NAME_SPACE::Vector128Utility::LoadFloat4(&vector.x); }

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
	__forceinline float Dot(const Vector4f& left, const Vector4f& right) noexcept 
	{ 
		return SIMD_NAME_SPACE::Vector128Utility::DotVector4(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : 外積を算出 first, second, thirdに垂直なベクトルを返す
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Cross(const Vector4f& first, const Vector4f& second, const Vector4f& third) noexcept 
	{ 
		return SIMD_NAME_SPACE::Vector128Utility::CrossVector4(first, second, third);
	}

	/*----------------------------------------------------------------------
	*  @brief : 入力ベクトルの単位ベクトルを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Normalize(const Vector4f& vector) noexcept 
	{ 
		return SIMD_NAME_SPACE::Vector128Utility::NormalizeVector4(vector); 
	}

	/*----------------------------------------------------------------------
	*  @brief     反射ベクトルを算出
	*  @param[in] const Vector4f& : input  (入射ベクトル)
	*  @param[in] const Vector4f& : normal (反射を行う法線ベクトル)
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Reflect(const Vector4f& input, const Vector4f& normal) noexcept 
	{ 
		return SIMD_NAME_SPACE::Vector128Utility::ReflectVector4(input, normal); 
	}

	/*----------------------------------------------------------------------
	*  @brief     屈折ベクトルを算出
	*  @param[in] const Vector4f& : input  (入射ベクトル)
	*  @param[in] const Vector4f& : normal (反射を行う法線ベクトル)
	*  @param[in] const float    : refractionIndex (屈折率)
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Refract(const Vector4f& input, const Vector4f& normal, const float refractionIndex) noexcept 
	{
		return SIMD_NAME_SPACE::Vector128Utility::RefractVector4(input, normal, refractionIndex); 
	}

	__forceinline Vector4f RefractV(const Vector4f& input, const Vector4f& normal, const Vector4f& refractionIndex) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::RefractVector4V(input, normal, refractionIndex);
	}

	/*----------------------------------------------------------------------
	*  @brief : 非常に大きい値か
	/*----------------------------------------------------------------------*/
	__forceinline bool IsInfinite(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::IsInfiniteVector4(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素に根号を取ったものを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Sqrt(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Sqrt(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素の逆数を取り, それに根号を取ったものを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f ReciprocalSqrt(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ReciprocalSqrt(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のsin, cos, tanを返します [rad]
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Sin(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Sin(vector);
	}

	__forceinline Vector4f Cos(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Cos(vector);
	}

	__forceinline Vector4f Tan(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Tan(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のarcsin, arccos, arctanを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f ArcSin(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ArcSin(vector);
	}

	__forceinline Vector4f ArcCos(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ArcCos(vector);
	}

	__forceinline Vector4f ArcTan(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ArcTan(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のsinH, cosH, tanHを返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f SinH(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::SinH(vector);
	}

	__forceinline Vector4f CosH(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::CosH(vector);
	}

	__forceinline Vector4f TanH(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::TanH(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素の2^{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Exp2(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Exp2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素の10^{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Exp10(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Exp10(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のe^{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f ExpE(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ExpE(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のlog2{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Log2(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Log2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のlog10{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Log10(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Log10(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のloge{vector}を返します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f LogE(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::LogE(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素のPowを返します
	*           base^{power}
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Pow(const Vector4f& base, const Vector4f& power) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Pow(base, power);
	}

	/*----------------------------------------------------------------------
	*  @brief      線形補間を返します
	*  @param[in]  t = 0の時の値
	   @param[in]  t = 1の時の値
	   @param[in]  t : 線形補間の割合
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Lerp(const Vector4f& start, const Vector4f& end, const float t) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Lerp(start, end, t);
	}

	__forceinline Vector4f LerpV(const Vector4f& start, const Vector4f& end, const Vector4f& t) noexcept
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
	__forceinline Vector4f Hermite(const Vector4f& startPosition, const Vector4f& startTangent, const Vector4f& endPosition, const Vector4f& endTangent, const float t) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Hermite(startPosition, startTangent, endPosition, endTangent, t);
	}

	__forceinline Vector4f HermiteV(const Vector4f& startPosition, const Vector4f& startTangent, const Vector4f& endPosition, const Vector4f& endTangent, const Vector4f& t) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::HermiteV(startPosition, startTangent, endPosition, endTangent, t);
	}

	/*----------------------------------------------------------------------
	*  @brief      CatMull-Romスプライン補間の結果を返します。4つの制御点を全て通るスプライン補間
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f CatMullRom(const Vector4f& position0, const Vector4f& position1, const Vector4f& position2, const Vector4f& position3, const float t) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::CatMullRom(position0, position1, position2, position3, t);
	}

	__forceinline Vector4f CatMullRomV(const Vector4f& position0, const Vector4f& position1, const Vector4f& position2, const Vector4f& position3, const Vector4f& t) noexcept
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
	__forceinline Vector4f BaryCentric(const Vector4f& position0, const Vector4f& position1, const Vector4f& position2, const float f, const float g) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::BaryCentric(position0, position1, position2, f, g);
	}

	__forceinline Vector4f BaryCentricV(const Vector4f& position0, const Vector4f& position1, const Vector4f& position2, const Vector4f& f, const Vector4f& g) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::BaryCentricV(position0, position1, position2, f, g);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素について最小値となる方を選択します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Min(const Vector4f& left, const Vector4f& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Min(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : 全ての要素について最大値となる方を選択します
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Max(const Vector4f& left, const Vector4f& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Max(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : 最も近い整数に丸める
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Round(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Round(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 切り捨て (指定した数値の端数を切り捨てた整数に丸める。数直線上で0の方向にある次の整数
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Truncate(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Truncate(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 負の方向に丸める
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Floor(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Floor(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 切り上げ 数直線上で正の方向にある次の整数に丸める
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Ceiling(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Ceiling(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 指定範囲内で数値を設定する
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Clamp(const Vector4f& vector, const Vector4f& min, const Vector4f& max) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Clamp(vector, min, max);
	}

	/*----------------------------------------------------------------------
	*  @brief : 1以上には行かないようにする
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Saturate(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Saturate(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : 絶対値
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Abs(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Abs(vector);
	}
}
#endif