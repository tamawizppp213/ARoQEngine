//////////////////////////////////////////////////////////////////////////////////
///  @file   GMVector4i.hpp
///  @brief  Int�^�̃x�N�g��4�N���X�ł�. �ق���Vector4f, 4d�ƈقȂ�T�C�Y���s��̂���, @n
///          Simd���g�p���Ȃ����j���̗p���Ă��܂�. @n
///          �܂�, Float4�̂悤�ȃf�[�^�\���ƌv�Z�����𕪗�����\�����A���C�������g�����Ȃǂ��s�v�Ȃ���, @n
///          Int4�^�Ȃǂ̓������s�v�Ɣ��f�������܂���.
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
	*   @brief  Int�^�̃x�N�g��4�N���X�ł�. �ق���Vector4f, 4d�ƈقȂ�T�C�Y���s��̂���, @n
	*           Simd���g�p���Ȃ����j���̗p���Ă��܂�. ���̂���, �v�Z���x�ʂ͒x���̂ōl�����K�v�ł�. @n
	*           �܂�, Float4�̂悤�ȃf�[�^�\���ƌv�Z�����𕪗�����\�����A���C�������g�����Ȃǂ��s�v�Ȃ���,@n
	*           Int4�^�Ȃǂ̓������s�v�Ɣ��f�������܂���.  
	*****************************************************************************/
	template<typename IntType>
	struct Vector4i
	{
	public:
		#pragma region Public Member Variables
		IntType x;  //!< ��1�v�f
		IntType y;  //!< ��2�v�f
		IntType z;  //!< ��3�v�f
		IntType w;  //!< ��4�v�f

		/*!**********************************************************************
		*  @brief     Zero�x�N�g����Ԃ��܂�
		*************************************************************************/
		static Vector4i Zero() { return Vector4i(0, 0, 0, 0); }

		/*!**********************************************************************
		*  @brief     One�x�N�g����Ԃ��܂�
		*************************************************************************/
		static Vector4i One()  { return Vector4i(1, 1, 1, 1); }

		/*!**********************************************************************
		*  @brief     XAxis�x�N�g����Ԃ��܂�
		*************************************************************************/
		static Vector4i XAxis() { return Vector4i(1, 0, 0, 0); }

		/*!**********************************************************************
		*  @brief     YAxis�x�N�g����Ԃ��܂�
		*************************************************************************/
		static Vector4i YAxis() { return Vector4i(0, 1, 0, 0); }

		/*!**********************************************************************
		*  @brief     ZAxis�x�N�g����Ԃ��܂�
		*************************************************************************/
		static Vector4i ZAxis() { return Vector4i(0, 0, 1, 0); }

		/*!**********************************************************************
		*  @brief     WAxis�x�N�g����Ԃ��܂�
		*************************************************************************/
		static Vector4i WAxis() { return Vector4i(0, 0, 0, 1); }

		#pragma endregion 

		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     0�x�N�g�����ǂ�����Ԃ��܂�
		*************************************************************************/
		__forceinline bool IsZero() { return *this == Vector4i(0, 0, 0, 0); }

		#pragma endregion 

		#pragma region Public Operator 
		/*! @brief ���Z*/
		__forceinline Vector4i operator+ (const Vector4i& right) const { return Vector4i(x + right.x, y + right.y, z + right.z, w + right.w); }

		/*! @brief ���Z*/
		__forceinline Vector4i operator- (const Vector4i& right) const { return Vector4i(x - right.x, y - right.y, z - right.z, w - right.w); }

		/*! @brief ��Z*/
		__forceinline Vector4i operator* (const Vector4i& right) const { return Vector4i(x * right.x, y * right.y, z * right.z, w * right.w); }

		/*! @brief ���Z*/
		__forceinline Vector4i operator/ (const Vector4i& right) const { return Vector4i(x / right.x, y / right.y, z / right.z, w / right.w); }

		/*! @brief �]��*/
		__forceinline Vector4i operator% (const Vector4i& right) const { return Vector4i(x % right.x, y % right.y, z % right.z, w % right.w); }

		/*! @brief �X�J���[�{*/
		__forceinline Vector4i operator* (const IntType right) const { return Vector4i(x * right, y * right, z * right, w * right); }

		/*! @brief �X�J���[�{*/
		__forceinline friend Vector4i operator* (const IntType left, const Vector4i& right) { return Vector4i(left * right.x, left * right.y, left * right.z, left * right.w); }

		/*! @brief �X�J���[�ŏ��Z*/
		__forceinline Vector4i operator/ (const IntType right) const { return Vector4i(x / right, y / right, z / right, w / right); }

		/*! @brief �X�J���[�ŏ��Z*/
		__forceinline friend Vector4i operator/ (const IntType left, const Vector4i& right) { return Vector4i(left / right.x, left / right.y, left / right.z, left / right.w); }

		/*! @brief ���Z*/
		__forceinline Vector4i& operator +=(const Vector4i& right) { *this = *this + right; return *this; }

		/*! @brief ���Z*/
		__forceinline Vector4i& operator -=(const Vector4i& right) { *this = *this - right; return *this; }

		/*! @brief ��Z*/
		__forceinline Vector4i& operator *=(const Vector4i& right) { *this = *this * right; return *this; }

		/*! @brief ���Z*/
		__forceinline Vector4i& operator /=(const Vector4i& right) { *this = *this / right; return *this; }

		/*! @brief �S�Ă̗v�f����������*/
		__forceinline bool operator ==(const Vector4i& right) const noexcept
		{
			return x == right.x && y == right.y && z == right.z && w == right.w;
		}

		/*! @brief ��ł��v�f���قȂ邩*/
		__forceinline bool operator !=(const Vector4i& right) const noexcept
		{
			return x != right.x || y != right.y || z != right.z || w != right.w;
		}

		/*! @brief �S�Ă̗v�f�ő傫����*/
		__forceinline bool operator > (const Vector4i& right) const noexcept
		{
			return x > right.x && y > right.y && z > right.z && w > right.w;
		}

		/*! @brief �S�Ă̗v�f�ő傫�� or ��������*/
		__forceinline bool operator >= (const Vector4i& right) const noexcept
		{
			return x >= right.x && y >= right.y && z >= right.z && w >= right.w;
		}

		/*! @brief �S�Ă̗v�f�ő傫����*/
		__forceinline bool operator < (const Vector4i& right) const noexcept
		{
			return x < right.x && y < right.y && z < right.z && w < right.w;
		}

		/*! @brief �S�Ă̗v�f�ő傫�� or ��������*/
		__forceinline bool operator <= (const Vector4i& right) const noexcept
		{
			return x <= right.x && y <= right.y && z <= right.z && w <= right.w;
		}

		/*! @brief �E�V�t�g*/
		__forceinline Vector4i operator >> (const IntType right) const { return Vector4i(x >> right, y >> right, z >> right, w >> right); }

		/*! @brief ���V�t�g*/
		__forceinline Vector4i operator << (const IntType right) const { return Vector4i(x << right, y << right, z << right, w << right); }

		/*! @brief bit and */
		__forceinline Vector4i operator &(const IntType right) const { return Vector4i(x & right, y & right, z & right, w & right); }

		/*! @brief bit or */
		__forceinline Vector4i operator |(const IntType right) const { return Vector4i(x | right, y | right, z | right, w | right); }

		/*! @brief bit xor*/
		__forceinline Vector4i operator ^(const IntType right) const { return Vector4i(x ^ right, y ^ right, z ^ right, w ^ right); }

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		Vector4i() : x(0), y(0), z(0), w(0) {};

		/*! @brief ��̗v�f�ŏ�����*/
		Vector4i(const IntType value) : x(value), y(value), z(value), w(value) {};

		/*! @brief �S�Ă̗v�f�ŏ�����*/
		Vector4i(const IntType ix, const IntType iy, const IntType iz, const IntType iw) : x(ix), y(iy), z(iz), w(iw) {};

		/*! @brief �z����g���ď�����*/
		explicit Vector4i(const IntType* array) noexcept : x(array[0]), y(array[1]), z(array[2]), w(array[3]) {};

		/*! @brief �R�s�[�R���X�g���N�^*/
		Vector4i(const Vector4i<IntType>&) = default;

		/*! @brief �R�s�[�R���X�g���N�^*/
		Vector4i& operator=(const Vector4i<IntType>&) = default;

		/*! @brief ���[�u�R���X�g���N�^*/
		Vector4i(Vector4i<IntType>&&) = default;

		/*! @brief ���[�u�R���X�g���N�^*/
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