//////////////////////////////////////////////////////////////////////////////////
///  @file   GMVector4i.hpp
///  @brief  Int型のベクトル4クラスです. ほかのVector4f, 4dと異なりサイズが不定のため, @n
///          Simdを使用しない方針を採用しています. @n
///          また, Float4のようなデータ構造と計算部分を分離する構造もアラインメント調整などが不要なため, @n
///          Int4型などの導入も不要と判断いたしました.
///  @author toide
///  @date   2024/04/21 19:02:30
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_VECTOR4I_HPP
#define GM_VECTOR4I_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gm
{
	/****************************************************************************
	*				  			   Vector4i
	*************************************************************************//**
	/*  @struct Vector4i
	*   @brief  Int型のベクトル4クラスです. ほかのVector4f, 4dと異なりサイズが不定のため, @n
	*           Simdを使用しない方針を採用しています. そのため, 計算速度面は遅いので考慮が必要です. @n
	*           また, Float4のようなデータ構造と計算部分を分離する構造もアラインメント調整などが不要なため,@n
	*           Int4型などの導入も不要と判断いたしました.  
	*****************************************************************************/
	template<typename IntType>
	struct Vector4i
	{
	public:
		#pragma region Public Member Variables
		IntType x;  //!< 第1要素
		IntType y;  //!< 第2要素
		IntType z;  //!< 第3要素
		IntType w;  //!< 第4要素

		/*!**********************************************************************
		*  @brief     Zeroベクトルを返します
		*************************************************************************/
		static Vector4i Zero() { return Vector4i(0, 0, 0, 0); }

		/*!**********************************************************************
		*  @brief     Oneベクトルを返します
		*************************************************************************/
		static Vector4i One()  { return Vector4i(1, 1, 1, 1); }

		/*!**********************************************************************
		*  @brief     XAxisベクトルを返します
		*************************************************************************/
		static Vector4i XAxis() { return Vector4i(1, 0, 0, 0); }

		/*!**********************************************************************
		*  @brief     YAxisベクトルを返します
		*************************************************************************/
		static Vector4i YAxis() { return Vector4i(0, 1, 0, 0); }

		/*!**********************************************************************
		*  @brief     ZAxisベクトルを返します
		*************************************************************************/
		static Vector4i ZAxis() { return Vector4i(0, 0, 1, 0); }

		/*!**********************************************************************
		*  @brief     WAxisベクトルを返します
		*************************************************************************/
		static Vector4i WAxis() { return Vector4i(0, 0, 0, 1); }

		#pragma endregion 

		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     0ベクトルかどうかを返します
		*************************************************************************/
		__forceinline bool IsZero() { return *this == Vector4i(0, 0, 0, 0); }

		#pragma endregion 

		#pragma region Public Operator 
		/*! @brief 加算*/
		__forceinline Vector4i operator+ (const Vector4i& right) const { return Vector4i(x + right.x, y + right.y, z + right.z, w + right.w); }

		/*! @brief 減算*/
		__forceinline Vector4i operator- (const Vector4i& right) const { return Vector4i(x - right.x, y - right.y, z - right.z, w - right.w); }

		/*! @brief 乗算*/
		__forceinline Vector4i operator* (const Vector4i& right) const { return Vector4i(x * right.x, y * right.y, z * right.z, w * right.w); }

		/*! @brief 除算*/
		__forceinline Vector4i operator/ (const Vector4i& right) const { return Vector4i(x / right.x, y / right.y, z / right.z, w / right.w); }

		/*! @brief 余り*/
		__forceinline Vector4i operator% (const Vector4i& right) const { return Vector4i(x % right.x, y % right.y, z % right.z, w % right.w); }

		/*! @brief スカラー倍*/
		__forceinline Vector4i operator* (const IntType right) const { return Vector4i(x * right, y * right, z * right, w * right); }

		/*! @brief スカラー倍*/
		__forceinline friend Vector4i operator* (const IntType left, const Vector4i& right) { return Vector4i(left * right.x, left * right.y, left * right.z, left * right.w); }

		/*! @brief スカラーで除算*/
		__forceinline Vector4i operator/ (const IntType right) const { return Vector4i(x / right, y / right, z / right, w / right); }

		/*! @brief スカラーで除算*/
		__forceinline friend Vector4i operator/ (const IntType left, const Vector4i& right) { return Vector4i(left / right.x, left / right.y, left / right.z, left / right.w); }

		/*! @brief 加算*/
		__forceinline Vector4i& operator +=(const Vector4i& right) { *this = *this + right; return *this; }

		/*! @brief 減算*/
		__forceinline Vector4i& operator -=(const Vector4i& right) { *this = *this - right; return *this; }

		/*! @brief 乗算*/
		__forceinline Vector4i& operator *=(const Vector4i& right) { *this = *this * right; return *this; }

		/*! @brief 除算*/
		__forceinline Vector4i& operator /=(const Vector4i& right) { *this = *this / right; return *this; }

		/*! @brief 全ての要素が等しいか*/
		__forceinline bool operator ==(const Vector4i& right) const noexcept
		{
			return x == right.x && y == right.y && z == right.z && w == right.w;
		}

		/*! @brief 一つでも要素が異なるか*/
		__forceinline bool operator !=(const Vector4i& right) const noexcept
		{
			return x != right.x || y != right.y || z != right.z || w != right.w;
		}

		/*! @brief 全ての要素で大きいか*/
		__forceinline bool operator > (const Vector4i& right) const noexcept
		{
			return x > right.x && y > right.y && z > right.z && w > right.w;
		}

		/*! @brief 全ての要素で大きい or 等しいか*/
		__forceinline bool operator >= (const Vector4i& right) const noexcept
		{
			return x >= right.x && y >= right.y && z >= right.z && w >= right.w;
		}

		/*! @brief 全ての要素で大きいか*/
		__forceinline bool operator < (const Vector4i& right) const noexcept
		{
			return x < right.x && y < right.y && z < right.z && w < right.w;
		}

		/*! @brief 全ての要素で大きい or 等しいか*/
		__forceinline bool operator <= (const Vector4i& right) const noexcept
		{
			return x <= right.x && y <= right.y && z <= right.z && w <= right.w;
		}

		/*! @brief 右シフト*/
		__forceinline Vector4i operator >> (const IntType right) const { return Vector4i(x >> right, y >> right, z >> right, w >> right); }

		/*! @brief 左シフト*/
		__forceinline Vector4i operator << (const IntType right) const { return Vector4i(x << right, y << right, z << right, w << right); }

		/*! @brief bit and */
		__forceinline Vector4i operator &(const IntType right) const { return Vector4i(x & right, y & right, z & right, w & right); }

		/*! @brief bit or */
		__forceinline Vector4i operator |(const IntType right) const { return Vector4i(x | right, y | right, z | right, w | right); }

		/*! @brief bit xor*/
		__forceinline Vector4i operator ^(const IntType right) const { return Vector4i(x ^ right, y ^ right, z ^ right, w ^ right); }

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		Vector4i() : x(0), y(0), z(0), w(0) {};

		/*! @brief 一つの要素で初期化*/
		Vector4i(const IntType value) : x(value), y(value), z(value), w(value) {};

		/*! @brief 全ての要素で初期化*/
		Vector4i(const IntType ix, const IntType iy, const IntType iz, const IntType iw) : x(ix), y(iy), z(iz), w(iw) {};

		/*! @brief 配列を使って初期化*/
		explicit Vector4i(const IntType* array) noexcept : x(array[0]), y(array[1]), z(array[2]), w(array[3]) {};

		/*! @brief コピーコンストラクタ*/
		Vector4i(const Vector4i<IntType>&) = default;

		/*! @brief コピーコンストラクタ*/
		Vector4i& operator=(const Vector4i<IntType>&) = default;

		/*! @brief ムーブコンストラクタ*/
		Vector4i(Vector4i<IntType>&&) = default;

		/*! @brief ムーブコンストラクタ*/
		Vector4i& operator=(Vector4i<IntType>&&) = default;
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Member Variables

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Member Variables

		#pragma endregion 

	};
}

#endif