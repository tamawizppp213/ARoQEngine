//////////////////////////////////////////////////////////////////////////////////
///  @file   GMVector3i.hpp
///  @brief  Int�^�̃x�N�g��3�N���X�ł�. �ق���Vector3f, 3d�ƈقȂ�T�C�Y���s��̂���, @n
///          Simd���g�p���Ȃ����j���̗p���Ă��܂�. @n
///          �܂�, Float3�̂悤�ȃf�[�^�\���ƌv�Z�����𕪗�����\�����A���C�������g�����Ȃǂ��s�v�Ȃ���, @n
///          Int3�^�Ȃǂ̓������s�v�Ɣ��f�������܂���.
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
	*   @brief  Int�^�̃x�N�g��3�N���X�ł�. �ق���Vector3f, 3d�ƈقȂ�T�C�Y���s��̂���, @n
	*           Simd���g�p���Ȃ����j���̗p���Ă��܂�. ���̂���, �v�Z���x�ʂ͒x���̂ōl�����K�v�ł�. @n
	*           �܂�, Float3�̂悤�ȃf�[�^�\���ƌv�Z�����𕪗�����\�����A���C�������g�����Ȃǂ��s�v�Ȃ���,@n
	*           Int3�^�Ȃǂ̓������s�v�Ɣ��f�������܂���.  
	*****************************************************************************/
	template<typename IntType>
	struct Vector3i
	{
	public:
		#pragma region Public Property
		IntType x;  //!< ��1�v�f
		IntType y;  //!< ��2�v�f
		IntType z;  //!< ��3�v�f

		/*!**********************************************************************
		*  @brief     Zero�x�N�g����Ԃ��܂�
		*************************************************************************/
		static Vector3i Zero() { return Vector3i(0, 0, 0); }

		/*!**********************************************************************
		*  @brief     One�x�N�g����Ԃ��܂�
		*************************************************************************/
		static Vector3i One()  { return Vector3i(1, 1, 1); }

		/*!**********************************************************************
		*  @brief     XAxis�x�N�g����Ԃ��܂�
		*************************************************************************/
		static Vector3i XAxis() { return Vector3i(1, 0, 0); }

		/*!**********************************************************************
		*  @brief     YAxis�x�N�g����Ԃ��܂�
		*************************************************************************/
		static Vector3i YAxis() { return Vector3i(0, 1, 0); }

		/*!**********************************************************************
		*  @brief     ZAxis�x�N�g����Ԃ��܂�
		*************************************************************************/
		static Vector3i ZAxis() { return Vector3i(0, 0, 1); }

		#pragma endregion 

		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     0�x�N�g�����ǂ�����Ԃ��܂�
		*************************************************************************/
		__forceinline bool IsZero() const { return *this == Vector3i(0, 0, 0); }

		#pragma endregion 

		#pragma region Public Operator 
		/*! @brief ���Z*/
		__forceinline Vector3i operator+ (const Vector3i& right) const { return Vector3i(x + right.x, y + right.y, z + right.z); }

		/*! @brief ���Z*/
		__forceinline Vector3i operator- (const Vector3i& right) const { return Vector3i(x - right.x, y - right.y, z - right.z); }

		/*! @brief ��Z*/
		__forceinline Vector3i operator* (const Vector3i& right) const { return Vector3i(x * right.x, y * right.y, z * right.z); }

		/*! @brief ���Z*/
		__forceinline Vector3i operator/ (const Vector3i& right) const { return Vector3i(x / right.x, y / right.y, z / right.z); }

		/*! @brief �]��*/
		__forceinline Vector3i operator% (const Vector3i& right) const { return Vector3i(x % right.x, y % right.y, z % right.z); }

		/*! @brief �X�J���[�{*/
		__forceinline Vector3i operator* (const IntType right) const { return Vector3i(x * right, y * right, z * right); }

		/*! @brief �X�J���[�{*/
		__forceinline friend Vector3i operator* (const IntType left, const Vector3i& right) { return Vector3i(left * right.x, left * right.y, left * right.z); }

		/*! @brief �X�J���[�ŏ��Z*/
		__forceinline Vector3i operator/ (const IntType right) const { return Vector3i(x / right, y / right, z / right); }

		/*! @brief �X�J���[�ŏ��Z*/
		__forceinline friend Vector3i operator/ (const IntType left, const Vector3i& right) { return Vector3i(left / right.x, left / right.y, left / right.z); }

		/*! @brief ���Z*/
		__forceinline Vector3i& operator +=(const Vector3i& right) { *this = *this + right; return *this; }

		/*! @brief ���Z*/
		__forceinline Vector3i& operator -=(const Vector3i& right) { *this = *this - right; return *this; }

		/*! @brief ��Z*/
		__forceinline Vector3i& operator *=(const Vector3i& right) { *this = *this * right; return *this; }

		/*! @brief ���Z*/
		__forceinline Vector3i& operator /=(const Vector3i& right) { *this = *this / right; return *this; }

		/*! @brief �E�V�t�g*/
		__forceinline Vector3i operator >> (const IntType right) const { return Vector3i(x >> right, y >> right, z >> right); }

		/*! @brief ���V�t�g*/
		__forceinline Vector3i operator << (const IntType right) const { return Vector3i(x << right, y << right, z << right); }

		/*! @brief bit and */
		__forceinline Vector3i operator &(const IntType right) const { return Vector3i(x & right, y & right, z & right); }

		/*! @brief bit or */
		__forceinline Vector3i operator |(const IntType right) const { return Vector3i(x | right, y | right, z | right); }

		/*! @brief bit xor*/
		__forceinline Vector3i operator ^(const IntType right) const { return Vector3i(x ^ right, y ^ right, z ^ right); }

		/*! @brief �S�Ă̗v�f����������*/
		__forceinline bool operator ==(const Vector3i& right) const noexcept
		{
			return x == right.x && y == right.y && z == right.z;
		}

		/*! @brief ��ł��v�f���قȂ邩*/
		__forceinline bool operator !=(const Vector3i& right) const noexcept
		{
			return x != right.x || y != right.y || z != right.z;
		}

		/*! @brief �S�Ă̗v�f�ő傫����*/
		__forceinline bool operator > (const Vector3i& right) const noexcept
		{
			return x > right.x && y > right.y && z > right.z;
		}

		/*! @brief �S�Ă̗v�f�ő傫�� or ��������*/
		__forceinline bool operator >= (const Vector3i& right) const noexcept
		{
			return x >= right.x && y >= right.y && z >= right.z;
		}

		/*! @brief �S�Ă̗v�f�ő傫����*/
		__forceinline bool operator < (const Vector3i& right) const noexcept
		{
			return x < right.x && y < right.y && z < right.z;
		}

		/*! @brief �S�Ă̗v�f�ő傫�� or ��������*/
		__forceinline bool operator <= (const Vector3i& right) const noexcept
		{
			return x <= right.x && y <= right.y && z <= right.z;
		}

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		Vector3i() : x(0), y(0), z(0) {};

		/*! @brief ��̗v�f�ŏ�����*/
		Vector3i(const IntType value) : x(value), y(value), z(value) {};

		/*! @brief �S�Ă̗v�f�ŏ�����*/
		Vector3i(const IntType ix, const IntType iy, const IntType iz) : x(ix), y(iy), z(iz) {};

		/*! @brief �z����g���ď�����*/
		explicit Vector3i(const IntType* array) noexcept : x(array[0]), y(array[1]), z(array[2]) {};

		/*! @brief �R�s�[�R���X�g���N�^*/
		Vector3i(const Vector3i<IntType>&) = default;

		/*! @brief �R�s�[�R���X�g���N�^*/
		Vector3i& operator=(const Vector3i<IntType>&) = default;

		/*! @brief ���[�u�R���X�g���N�^*/
		Vector3i(Vector3i<IntType>&&) = default;

		/*! @brief ���[�u�R���X�g���N�^*/
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