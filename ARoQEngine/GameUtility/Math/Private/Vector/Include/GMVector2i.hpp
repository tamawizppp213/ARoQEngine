//////////////////////////////////////////////////////////////////////////////////
///  @file   GMVector2i.hpp
///  @brief  Int�^�̃x�N�g��2�N���X�ł�. �ق���Vector2f, 2d�ƈقȂ�T�C�Y���s��̂���, @n
///          Simd���g�p���Ȃ����j���̗p���Ă��܂�. @n
///          �܂�, Float2�̂悤�ȃf�[�^�\���ƌv�Z�����𕪗�����\�����A���C�������g�����Ȃǂ��s�v�Ȃ���, @n
///          Int2�^�Ȃǂ̓������s�v�Ɣ��f�������܂���.
///  @author toide
///  @date   2024/04/21 19:02:30
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_VECTOR2I_HPP
#define GM_VECTOR2I_HPP

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
	*				  			   Vector2i
	****************************************************************************/
	/*  @struct Vector2i
	*   @brief  Int�^�̃x�N�g��3�N���X�ł�. �ق���Vector3f, 3d�ƈقȂ�T�C�Y���s��̂���, @n
	*           Simd���g�p���Ȃ����j���̗p���Ă��܂�. ���̂���, �v�Z���x�ʂ͒x���̂ōl�����K�v�ł�. @n
	*           �܂�, Float3�̂悤�ȃf�[�^�\���ƌv�Z�����𕪗�����\�����A���C�������g�����Ȃǂ��s�v�Ȃ���,@n
	*           Int3�^�Ȃǂ̓������s�v�Ɣ��f�������܂���.  
	*****************************************************************************/
	template<typename IntType>
	struct Vector2i
	{
	public:
		#pragma region Public Property
		IntType x;  //!< ��1�v�f
		IntType y;  //!< ��2�v�f

		/*!**********************************************************************
		*  @brief     Zero�x�N�g����Ԃ��܂�
		*************************************************************************/
		static Vector2i Zero() { return Vector2i(0, 0); }

		/*!**********************************************************************
		*  @brief     One�x�N�g����Ԃ��܂�
		*************************************************************************/
		static Vector2i One()  { return Vector2i(1, 1); }

		/*!**********************************************************************
		*  @brief     XAxis�x�N�g����Ԃ��܂�
		*************************************************************************/
		static Vector2i XAxis() { return Vector2i(1, 0); }

		/*!**********************************************************************
		*  @brief     YAxis�x�N�g����Ԃ��܂�
		*************************************************************************/
		static Vector2i YAxis() { return Vector2i(0, 1); }

		#pragma endregion 

		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     0�x�N�g�����ǂ�����Ԃ��܂�
		*************************************************************************/
		__forceinline bool IsZero() { return *this == Vector2i(0, 0); }

		#pragma endregion 

		#pragma region Public Operator 
		/*! @brief ���Z*/
		__forceinline Vector2i operator+ (const Vector2i& right) const { return Vector2i(x + right.x, y + right.y); }

		/*! @brief ���Z*/
		__forceinline Vector2i operator- (const Vector2i& right) const { return Vector2i(x - right.x, y - right.y); }

		/*! @brief ��Z*/
		__forceinline Vector2i operator* (const Vector2i& right) const { return Vector2i(x * right.x, y * right.y); }

		/*! @brief ���Z*/
		__forceinline Vector2i operator/ (const Vector2i& right) const { return Vector2i(x / right.x, y / right.y); }

		/*! @brief �]��*/
		__forceinline Vector2i operator% (const Vector2i& right) const { return Vector2i(x % right.x, y % right.y); }

		/*! @brief �X�J���[�{*/
		__forceinline Vector2i operator* (const IntType right) const { return Vector2i(x * right, y * right); }

		/*! @brief �X�J���[�{*/
		__forceinline friend Vector2i operator* (const IntType left, const Vector2i& right) { return Vector2i(left * right.x, left * right.y); }

		/*! @brief �X�J���[�ŏ��Z*/
		__forceinline Vector2i operator/ (const IntType right) const { return Vector2i(x / right, y / right); }

		/*! @brief �X�J���[�ŏ��Z*/
		__forceinline friend Vector2i operator/ (const IntType left, const Vector2i& right) { return Vector2i(left / right.x, left / right.y); }

		/*! @brief ���Z*/
		__forceinline Vector2i& operator +=(const Vector2i& right) { *this = *this + right; return *this; }

		/*! @brief ���Z*/
		__forceinline Vector2i& operator -=(const Vector2i& right) { *this = *this - right; return *this; }

		/*! @brief ��Z*/
		__forceinline Vector2i& operator *=(const Vector2i& right) { *this = *this * right; return *this; }

		/*! @brief ���Z*/
		__forceinline Vector2i& operator /=(const Vector2i& right) { *this = *this / right; return *this; }

		/*! @brief �S�Ă̗v�f����������*/
		__forceinline bool operator ==(const Vector2i& right) const noexcept
		{
			return x == right.x && y == right.y;
		}

		/*! @brief ��ł��v�f���قȂ邩*/
		__forceinline bool operator !=(const Vector2i& right) const noexcept
		{
			return x != right.x || y != right.y;
		}

		/*! @brief �S�Ă̗v�f�ő傫����*/
		__forceinline bool operator > (const Vector2i& right) const noexcept
		{
			return x > right.x && y > right.y;
		}

		/*! @brief �S�Ă̗v�f�ő傫�� or ��������*/
		__forceinline bool operator >= (const Vector2i& right) const noexcept
		{
			return x >= right.x && y >= right.y;
		}

		/*! @brief �S�Ă̗v�f�ő傫����*/
		__forceinline bool operator < (const Vector2i& right) const noexcept
		{
			return x < right.x && y < right.y;
		}

		/*! @brief �S�Ă̗v�f�ő傫�� or ��������*/
		__forceinline bool operator <= (const Vector2i& right) const noexcept
		{
			return x <= right.x && y <= right.y;
		}

		/*! @brief �E�V�t�g*/
		__forceinline Vector2i operator >> (const IntType right) const { return Vector2i(x >> right, y >> right); }

		/*! @brief ���V�t�g*/
		__forceinline Vector2i operator << (const IntType right) const { return Vector2i(x << right, y << right); }

		/*! @brief bit and */
		__forceinline Vector2i operator &(const IntType right) const { return Vector2i(x & right, y & right); }

		/*! @brief bit or */
		__forceinline Vector2i operator |(const IntType right) const { return Vector2i(x | right, y | right); }

		/*! @brief bit xor*/
		__forceinline Vector2i operator ^(const IntType right) const { return Vector2i(x ^ right, y ^ right); }
		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		Vector2i() : x(0), y(0) {};

		/*! @brief ��̗v�f�ŏ�����*/
		Vector2i(const IntType value) : x(value), y(value) {};

		/*! @brief �S�Ă̗v�f�ŏ�����*/
		Vector2i(const IntType ix, const IntType iy, const IntType iz) : x(ix), y(iy){};

		/*! @brief �z����g���ď�����*/
		explicit Vector2i(const IntType* array) noexcept : x(array[0]), y(array[1]) {};

		/*! @brief �R�s�[�R���X�g���N�^*/
		Vector2i(const Vector2i<IntType>&) = default;

		/*! @brief �R�s�[�R���X�g���N�^*/
		Vector2i& operator=(const Vector2i<IntType>&) = default;

		/*! @brief ���[�u�R���X�g���N�^*/
		Vector2i(Vector2i<IntType>&&) = default;

		/*! @brief ���[�u�R���X�g���N�^*/
		Vector2i& operator=(Vector2i<IntType>&&) = default;
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