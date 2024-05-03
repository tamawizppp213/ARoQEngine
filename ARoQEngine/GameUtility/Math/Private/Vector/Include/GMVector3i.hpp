//////////////////////////////////////////////////////////////////////////////////
///  @file   GMVector3i.hpp
///  @brief  Int型のベクトル3クラスです. ほかのVector3f, 3dと異なりサイズが不定のため, @n
///          Simdを使用しない方針を採用しています. @n
///          また, Float3のようなデータ構造と計算部分を分離する構造もアラインメント調整などが不要なため, @n
///          Int3型などの導入も不要と判断いたしました.
///  @author toide
///  @date   2024/04/21 19:02:30
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_VECTOR3I_HPP
#define GM_VECTOR3I_HPP

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
	*				  			   Vector3i
	*************************************************************************//**
	/*  @struct Vector3i
	*   @brief  Int型のベクトル3クラスです. ほかのVector3f, 3dと異なりサイズが不定のため, @n
	*           Simdを使用しない方針を採用しています. そのため, 計算速度面は遅いので考慮が必要です. @n
	*           また, Float3のようなデータ構造と計算部分を分離する構造もアラインメント調整などが不要なため,@n
	*           Int3型などの導入も不要と判断いたしました.  
	*****************************************************************************/
	template<typename IntType>
	struct Vector3i
	{
	public:
		#pragma region Public Property
		IntType x;  //!< 第1要素
		IntType y;  //!< 第2要素
		IntType z;  //!< 第3要素

		/*!**********************************************************************
		*  @brief     Zeroベクトルを返します
		*************************************************************************/
		static Vector3i Zero() { return Vector3i(0, 0, 0); }

		/*!**********************************************************************
		*  @brief     Oneベクトルを返します
		*************************************************************************/
		static Vector3i One()  { return Vector3i(1, 1, 1); }

		/*!**********************************************************************
		*  @brief     XAxisベクトルを返します
		*************************************************************************/
		static Vector3i XAxis() { return Vector3i(1, 0, 0); }

		/*!**********************************************************************
		*  @brief     YAxisベクトルを返します
		*************************************************************************/
		static Vector3i YAxis() { return Vector3i(0, 1, 0); }

		/*!**********************************************************************
		*  @brief     ZAxisベクトルを返します
		*************************************************************************/
		static Vector3i ZAxis() { return Vector3i(0, 0, 1); }

		#pragma endregion 

		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     0ベクトルかどうかを返します
		*************************************************************************/
		__forceinline bool IsZero() const { return *this == Vector3i(0, 0, 0); }

		#pragma endregion 

		#pragma region Public Operator 
		/*! @brief 加算*/
		__forceinline Vector3i operator+ (const Vector3i& right) const { return Vector3i(x + right.x, y + right.y, z + right.z); }

		/*! @brief 減算*/
		__forceinline Vector3i operator- (const Vector3i& right) const { return Vector3i(x - right.x, y - right.y, z - right.z); }

		/*! @brief 乗算*/
		__forceinline Vector3i operator* (const Vector3i& right) const { return Vector3i(x * right.x, y * right.y, z * right.z); }

		/*! @brief 除算*/
		__forceinline Vector3i operator/ (const Vector3i& right) const { return Vector3i(x / right.x, y / right.y, z / right.z); }

		/*! @brief 余り*/
		__forceinline Vector3i operator% (const Vector3i& right) const { return Vector3i(x % right.x, y % right.y, z % right.z); }

		/*! @brief スカラー倍*/
		__forceinline Vector3i operator* (const IntType right) const { return Vector3i(x * right, y * right, z * right); }

		/*! @brief スカラー倍*/
		__forceinline friend Vector3i operator* (const IntType left, const Vector3i& right) { return Vector3i(left * right.x, left * right.y, left * right.z); }

		/*! @brief スカラーで除算*/
		__forceinline Vector3i operator/ (const IntType right) const { return Vector3i(x / right, y / right, z / right); }

		/*! @brief スカラーで除算*/
		__forceinline friend Vector3i operator/ (const IntType left, const Vector3i& right) { return Vector3i(left / right.x, left / right.y, left / right.z); }

		/*! @brief 加算*/
		__forceinline Vector3i& operator +=(const Vector3i& right) { *this = *this + right; return *this; }

		/*! @brief 減算*/
		__forceinline Vector3i& operator -=(const Vector3i& right) { *this = *this - right; return *this; }

		/*! @brief 乗算*/
		__forceinline Vector3i& operator *=(const Vector3i& right) { *this = *this * right; return *this; }

		/*! @brief 除算*/
		__forceinline Vector3i& operator /=(const Vector3i& right) { *this = *this / right; return *this; }

		/*! @brief 右シフト*/
		__forceinline Vector3i operator >> (const IntType right) const { return Vector3i(x >> right, y >> right, z >> right); }

		/*! @brief 左シフト*/
		__forceinline Vector3i operator << (const IntType right) const { return Vector3i(x << right, y << right, z << right); }

		/*! @brief bit and */
		__forceinline Vector3i operator &(const IntType right) const { return Vector3i(x & right, y & right, z & right); }

		/*! @brief bit or */
		__forceinline Vector3i operator |(const IntType right) const { return Vector3i(x | right, y | right, z | right); }

		/*! @brief bit xor*/
		__forceinline Vector3i operator ^(const IntType right) const { return Vector3i(x ^ right, y ^ right, z ^ right); }

		/*! @brief 全ての要素が等しいか*/
		__forceinline bool operator ==(const Vector3i& right) const noexcept
		{
			return x == right.x && y == right.y && z == right.z;
		}

		/*! @brief 一つでも要素が異なるか*/
		__forceinline bool operator !=(const Vector3i& right) const noexcept
		{
			return x != right.x || y != right.y || z != right.z;
		}

		/*! @brief 全ての要素で大きいか*/
		__forceinline bool operator > (const Vector3i& right) const noexcept
		{
			return x > right.x && y > right.y && z > right.z;
		}

		/*! @brief 全ての要素で大きい or 等しいか*/
		__forceinline bool operator >= (const Vector3i& right) const noexcept
		{
			return x >= right.x && y >= right.y && z >= right.z;
		}

		/*! @brief 全ての要素で大きいか*/
		__forceinline bool operator < (const Vector3i& right) const noexcept
		{
			return x < right.x && y < right.y && z < right.z;
		}

		/*! @brief 全ての要素で大きい or 等しいか*/
		__forceinline bool operator <= (const Vector3i& right) const noexcept
		{
			return x <= right.x && y <= right.y && z <= right.z;
		}

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		Vector3i() : x(0), y(0), z(0) {};

		/*! @brief 一つの要素で初期化*/
		Vector3i(const IntType value) : x(value), y(value), z(value) {};

		/*! @brief 全ての要素で初期化*/
		Vector3i(const IntType ix, const IntType iy, const IntType iz) : x(ix), y(iy), z(iz) {};

		/*! @brief 配列を使って初期化*/
		explicit Vector3i(const IntType* array) noexcept : x(array[0]), y(array[1]), z(array[2]) {};

		/*! @brief コピーコンストラクタ*/
		Vector3i(const Vector3i<IntType>&) = default;

		/*! @brief コピーコンストラクタ*/
		Vector3i& operator=(const Vector3i<IntType>&) = default;

		/*! @brief ムーブコンストラクタ*/
		Vector3i(Vector3i<IntType>&&) = default;

		/*! @brief ムーブコンストラクタ*/
		Vector3i& operator=(Vector3i<IntType>&&) = default;
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Property

		#pragma endregion 

	};
}

#endif