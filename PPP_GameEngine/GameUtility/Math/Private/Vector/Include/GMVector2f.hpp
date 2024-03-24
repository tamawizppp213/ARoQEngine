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
	*  @brief     SIMD���Z�p��Vector2�N���X�ł�. �A���C�������g���s���Ă��邽��, �f�[�^��ێ�����ꍇ��Float2���g�p���Ă�������
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

		// @brief : ��̗v�f�ŏ�����
		Float2(const float ix) : x(ix), y(ix) {};

		// @brief : �S�Ă̗v�f�ŏ�����
		Float2(const float ix, const float iy) : x(ix), y(iy) {};

		// @brief : �z����g���ď�����
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
	*  @brief     SIMD���Z�p��Vector2�N���X�ł�. �A���C�������g���s���Ă��邽��, �f�[�^��ێ�����ꍇ��Float2���g�p���Ă�������
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
		*  @brief : FPU���W�X�^�Ɋi�[���ꂽX(, Y, Z, W)���̗v�f�����o��
		/*----------------------------------------------------------------------*/
		__forceinline gu::float32 GetX() const { return SIMD_NAME_SPACE::Vector128Utility::GetX(_vector); }
		__forceinline gu::float32 GetY() const { return SIMD_NAME_SPACE::Vector128Utility::GetY(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : Simd vector
		/*----------------------------------------------------------------------*/
		__forceinline VECTOR128 GetSimdVector() const { return _vector; }

		/*----------------------------------------------------------------------
		*  @brief : Float2�ɕϊ�����
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
		*  @brief : Float���g����Vector��X(, Y, Z, W)�v�f�ɒl��������
		/*----------------------------------------------------------------------*/
		__forceinline void SetX(const float x) { _vector = SIMD_NAME_SPACE::Vector128Utility::SetX(_vector, x); }
		__forceinline void SetY(const float y) { _vector = SIMD_NAME_SPACE::Vector128Utility::SetY(_vector, y); }
#pragma endregion Setter

#pragma region Operator Function
		// @brief : ���Z
		__forceinline Vector2f operator+  (const Vector2f& right) const { return Vector2f(SIMD_NAME_SPACE::Vector128Utility::Add(_vector, right._vector)); }

		// @brief : ���Z
		__forceinline Vector2f operator- (const Vector2f& right) const { return Vector2f(SIMD_NAME_SPACE::Vector128Utility::Subtract(_vector, right._vector)); }

		// @brief : ��Z
		__forceinline Vector2f operator* (const Vector2f& right) const { return Vector2f(SIMD_NAME_SPACE::Vector128Utility::Multiply(_vector, right._vector)); }

		// @brief : ���Z
		__forceinline Vector2f operator/ (const Vector2f& right) const { return Vector2f(SIMD_NAME_SPACE::Vector128Utility::Vector128Utility::Divide(_vector, right._vector)); }

		// @brief : �]��
		__forceinline Vector2f operator% (const Vector2f& right) const { return Vector2f(SIMD_NAME_SPACE::Vector128Utility::Vector128Utility::Mod(_vector, right._vector)); }

		// @brief : �X�J���[�{
		__forceinline        Vector2f operator* (const float right) const { return Vector2f(SIMD_NAME_SPACE::Vector128Utility::Scale(_vector, right)); }
		__forceinline friend Vector2f operator* (const float left, const Vector2f right) { return Vector2f(SIMD_NAME_SPACE::Vector128Utility::Scale(right._vector, left)); }

		// @brief : �X�J���[�ŏ��Z
		__forceinline        Vector2f operator/ (const float right) const { return Vector2f(SIMD_NAME_SPACE::Vector128Utility::Scale(_vector, 1.0f / right)); }
		__forceinline friend Vector2f operator/ (const float left, const Vector2f right) { return Vector2f(SIMD_NAME_SPACE::Vector128Utility::Scale(right._vector, 1.0f / left)); }

		// @brief : ���Z (_vector�ł͂Ȃ�, *this���g���đ����̂�simd::Vector128���}�N���ɉ����ĕς�邽��, ���b�v����K�v������)
		__forceinline Vector2f& operator +=(const Vector2f& right) { *this = *this + right;  return *this; }

		// @brief : ���Z (_vector�ł͂Ȃ�, *this���g���Ĉ����̂�simd::Vector128���}�N���ɉ����ĕς�邽��, ���b�v����K�v������)
		__forceinline Vector2f& operator -=(const Vector2f& right) { *this = *this - right; return *this; }

		// @brief : ��Z (_vector�ł͂Ȃ�, *this���g���Ă�����̂�simd::Vector128���}�N���ɉ����ĕς�邽��, ���b�v����K�v������)
		__forceinline Vector2f& operator *=(const Vector2f& right) { *this = *this * right; return *this; }

		// @brief : ���Z (_vector�ł͂Ȃ�, *this���g���Ċ���̂�simd::Vector128���}�N���ɉ����ĕς�邽��, ���b�v����K�v������)
		__forceinline Vector2f& operator /=(const Vector2f& right) { *this = *this / right; return *this; }

		// @brief : �S�Ă̗v�f����������
		__forceinline bool operator == (const Vector2f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::EqualVector2(_vector, right._vector); }

		// @brief : ��ł��v�f���قȂ邩
		__forceinline bool operator != (const Vector2f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::NotEqualVector2(_vector, right._vector); }

		// @brief : �S�Ă̗v�f�ő傫����
		__forceinline bool operator >  (const Vector2f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::GreaterVector2(_vector, right._vector); }

		// @brief : �S�Ă̗v�f�ő傫��or��������
		__forceinline bool operator >=  (const Vector2f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::GreaterOrEqualVector2(_vector, right._vector); }

		// @brief : �S�Ă̗v�f�ŏ�������
		__forceinline bool operator <  (const Vector2f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::LessVector2(_vector, right._vector); }

		// @brief : �S�Ă̗v�f�ő傫����
		__forceinline bool operator <= (const Vector2f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::LessOrEqualVector2(_vector, right._vector); }

		// @brief : ���ڗv�f�ɃA�N�Z�X���܂�
		__forceinline       float& operator[](const gu::uint32 index) noexcept       { return _vector.m128_f32[index]; }
		__forceinline const float& operator[](const gu::uint32 index) const noexcept { return _vector.m128_f32[index]; }

		/*----------------------------------------------------------------------
		*  @brief : �w��͈͓��ɂ��邩�ǂ��� -bounds <= vector <= +bounds
		/*----------------------------------------------------------------------*/
		__forceinline bool InBounds(const Vector2f& bounds) noexcept { return SIMD_NAME_SPACE::Vector128Utility::InBoundsVector2(_vector, bounds._vector); }

		__forceinline operator VECTOR128() const { return _vector; }

#pragma endregion Operator Function

#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g���̊e�v�f���m�̋t�����Z�o���܂�. (1.0f / vector)
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Reciprocal() const { return SIMD_NAME_SPACE::Vector128Utility::Reciprocal(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �e�v�f�Ƀ}�C�i�X����������̂�Ԃ�.
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Negate() const { return SIMD_NAME_SPACE::Vector128Utility::Negate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 2���m����(�e�v�f��2��a�����[�g��������Ƃ��̃x�N�g���̑傫��)���Z�o
		/*----------------------------------------------------------------------*/
		__forceinline float Length() const { return SIMD_NAME_SPACE::Vector128Utility::LengthVector2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 2���m�����̓����Z�o
		/*----------------------------------------------------------------------*/
		__forceinline float LengthSquared() const { return SIMD_NAME_SPACE::Vector128Utility::LengthSquaredVector2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : ���ς��Z�o
		/*----------------------------------------------------------------------*/
		__forceinline float Dot(const Vector2f& right) const { return SIMD_NAME_SPACE::Vector128Utility::DotVector2(_vector, right._vector); }

		/*----------------------------------------------------------------------
		*  @brief : �O�ς��Z�o second, third�ɐ����ȃx�N�g����Ԃ�
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Cross(const Vector2f& second) const { return SIMD_NAME_SPACE::Vector128Utility::CrossVector2(_vector, second._vector); }

		/*----------------------------------------------------------------------
		*  @brief : ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Normalize() const { return SIMD_NAME_SPACE::Vector128Utility::NormalizeVector2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief     ���˃x�N�g�����Z�o
		*  @param[in] const Vector128&f : input  (���˃x�N�g��)
		*  @param[in] const Vector128&f : normal (���˂��s���@���x�N�g��)
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Reflect(const Vector2f& normal) const { return SIMD_NAME_SPACE::Vector128Utility::ReflectVector2(_vector, normal._vector); }

		/*----------------------------------------------------------------------
		*  @brief     ���܃x�N�g�����Z�o
		*  @param[in] const Vector2f& : input  (���˃x�N�g��)
		*  @param[in] const Vector2f& : normal (���˂��s���@���x�N�g��)
		*  @param[in] const float    : refractionIndex (���ܗ�)
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Refract(const Vector2f& normal, const float     refractionIndex) noexcept { return SIMD_NAME_SPACE::Vector128Utility::RefractVector2(_vector, normal._vector, refractionIndex); }
		__forceinline Vector2f Refract(const Vector2f& normal, const Vector2f& refractionIndex) noexcept { return SIMD_NAME_SPACE::Vector128Utility::RefractVector2V(_vector, normal._vector, refractionIndex._vector); }

		/*----------------------------------------------------------------------
		*  @brief : ���ɑ傫���l��
		/*----------------------------------------------------------------------*/
		__forceinline bool IsInfinite() const { return SIMD_NAME_SPACE::Vector128Utility::IsInfiniteVector2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief :  �[�����ǂ���
		/*----------------------------------------------------------------------*/
		__forceinline bool IsZero() const { return ZERO == _vector; }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�ɍ�������������̂�Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Sqrt() const { return SIMD_NAME_SPACE::Vector128Utility::Sqrt(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�̋t�������, ����ɍ�������������̂�Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f ReciprocalSqrt() const { return SIMD_NAME_SPACE::Vector128Utility::ReciprocalSqrt(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��sin, cos, tan��Ԃ��܂� [rad]
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Sin() const { return SIMD_NAME_SPACE::Vector128Utility::Sin(_vector); }
		__forceinline Vector2f Cos() const { return SIMD_NAME_SPACE::Vector128Utility::Cos(_vector); }
		__forceinline Vector2f Tan() const { return SIMD_NAME_SPACE::Vector128Utility::Tan(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��arcsin, arccos, arctan��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f ArcSin() const { return SIMD_NAME_SPACE::Vector128Utility::ArcSin(_vector); }
		__forceinline Vector2f ArcCos() const { return SIMD_NAME_SPACE::Vector128Utility::ArcCos(_vector); }
		__forceinline Vector2f ArcTan() const { return SIMD_NAME_SPACE::Vector128Utility::ArcTan(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��sinH, cosH, tanH��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f SinH() const { return SIMD_NAME_SPACE::Vector128Utility::SinH(_vector); }
		__forceinline Vector2f CosH() const { return SIMD_NAME_SPACE::Vector128Utility::CosH(_vector); }
		__forceinline Vector2f TanH() const { return SIMD_NAME_SPACE::Vector128Utility::TanH(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�ɑ΂���sin������cos���������o���܂�
		/*----------------------------------------------------------------------*/
		__forceinline void SinCos(Vector2f& sin, Vector2f& cos) noexcept
		{
			SIMD_NAME_SPACE::Vector128Utility::SinCos(_vector, &sin._vector, &cos._vector);
		}

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��2^{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Exp2() const { return SIMD_NAME_SPACE::Vector128Utility::Exp2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��10^{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Exp10() const { return SIMD_NAME_SPACE::Vector128Utility::Exp10(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��e^{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f ExpE() const { return SIMD_NAME_SPACE::Vector128Utility::ExpE(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��log2{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Log2() const { return SIMD_NAME_SPACE::Vector128Utility::Log2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��log10{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Log10() const { return SIMD_NAME_SPACE::Vector128Utility::Log10(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��loge{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f LogE() const { return SIMD_NAME_SPACE::Vector128Utility::LogE(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��Pow��Ԃ��܂�
		*           base^{power}
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Pow(const Vector2f& power) const { return SIMD_NAME_SPACE::Vector128Utility::Pow(_vector, power._vector); }

		/*----------------------------------------------------------------------
		*  @brief : �ł��߂������Ɋۂ߂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Round() const { return SIMD_NAME_SPACE::Vector128Utility::Round(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �؂�̂� (�w�肵�����l�̒[����؂�̂Ă������Ɋۂ߂�B���������0�̕����ɂ��鎟�̐���
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Truncate() const { return SIMD_NAME_SPACE::Vector128Utility::Truncate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : ���̕����Ɋۂ߂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Floor() const { return SIMD_NAME_SPACE::Vector128Utility::Floor(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �؂�グ ��������Ő��̕����ɂ��鎟�̐����Ɋۂ߂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Ceiling() const { return SIMD_NAME_SPACE::Vector128Utility::Ceiling(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �w��͈͓��Ő��l��ݒ肷��
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Clamp(const Vector2f& min, const Vector2f& max) const { return SIMD_NAME_SPACE::Vector128Utility::Clamp(_vector, min._vector, max._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 1�ȏ�ɂ͍s���Ȃ��悤�ɂ���
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Saturate() const { return SIMD_NAME_SPACE::Vector128Utility::Saturate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : ��Βl
		/*----------------------------------------------------------------------*/
		__forceinline Vector2f Abs() const { return SIMD_NAME_SPACE::Vector128Utility::Abs(_vector); }

#pragma endregion Math
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor
		Vector2f() { _vector = SIMD_NAME_SPACE::Vector128Utility::Zero(); }

		// @brief : Float�^�Ő�������R���X�g���N�^
		Vector2f(const gu::float32 x, const gu::float32 y)
		{
			_vector = SIMD_NAME_SPACE::Vector128Utility::Set(x, y, 0.0f, 0.0f);
		}

		// @brief : Vector128�𒼐ڑ������R���X�g���N�^
		Vector2f(const VECTOR128 vector) : _vector(vector) {};

		// @brief : Float2�^�Ő�������R���X�g���N�^
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
	*  @brief : ���ς��Z�o (������, �Ԃ�l��float�ł͂Ȃ�Vector128�^�ŕԂ���܂��̂�, ��XGetX�ȂǂőΉ����Ă�������.)
	/*----------------------------------------------------------------------*/
	__forceinline float Dot(const Vector2f& left, const Vector2f& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::DotVector2(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : �O�ς��Z�o first, second, third�ɐ����ȃx�N�g����Ԃ�
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Cross(const Vector2f& first, const Vector2f& second) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::CrossVector2(first, second);
	}

	/*----------------------------------------------------------------------
	*  @brief : ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Normalize(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::NormalizeVector2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief     ���˃x�N�g�����Z�o
	*  @param[in] const Vector2f& : input  (���˃x�N�g��)
	*  @param[in] const Vector2f& : normal (���˂��s���@���x�N�g��)
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Reflect(const Vector2f& input, const Vector2f& normal) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ReflectVector2(input, normal);
	}

	/*----------------------------------------------------------------------
	*  @brief     ���܃x�N�g�����Z�o
	*  @param[in] const Vector2f& : input  (���˃x�N�g��)
	*  @param[in] const Vector2f& : normal (���˂��s���@���x�N�g��)
	*  @param[in] const float    : refractionIndex (���ܗ�)
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
	*  @brief : ���ɑ傫���l��
	/*----------------------------------------------------------------------*/
	__forceinline bool IsInfinite(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::IsInfiniteVector2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f�ɍ�������������̂�Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Sqrt(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Sqrt(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f�̋t�������, ����ɍ�������������̂�Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f ReciprocalSqrt(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ReciprocalSqrt(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��sin, cos, tan��Ԃ��܂� [rad]
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
	*  @brief : �S�Ă̗v�f��arcsin, arccos, arctan��Ԃ��܂�
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
	*  @brief : �S�Ă̗v�f��sinH, cosH, tanH��Ԃ��܂�
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
	*  @brief : �S�Ă̗v�f��2^{vector}��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Exp2(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Exp2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��10^{vector}��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Exp10(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Exp10(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��e^{vector}��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f ExpE(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ExpE(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��log2{vector}��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Log2(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Log2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��log10{vector}��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Log10(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Log10(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��loge{vector}��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f LogE(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::LogE(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��Pow��Ԃ��܂�
	*           base^{power}
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Pow(const Vector2f& base, const Vector2f& power) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Pow(base, power);
	}

	/*----------------------------------------------------------------------
	*  @brief      ���`��Ԃ�Ԃ��܂�
	*  @param[in]  t = 0�̎��̒l
	   @param[in]  t = 1�̎��̒l
	   @param[in]  t : ���`��Ԃ̊���
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
	*  @brief      �G���~�[�g��Ԃ�Ԃ��܂�
	*  @param[in]  �n�_�̈ʒu
	   @param[in]  �n�_�ɂ�����X�v���C���̐ڐ���\���x�N�g��
	   @param[in]  �I�_�̈ʒu
	   @param[in]  �I�_�ɂ�����X�v���C���̐ڐ���\���x�N�g��
	   @param[in]  t : �G���~�[�g��Ԃ̊���
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
	*  @brief      CatMull-Rom�X�v���C����Ԃ̌��ʂ�Ԃ��܂��B4�̐���_��S�Ēʂ�X�v���C�����
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
	*  @brief      �O�p�`�̏d�S�d�݈ʒu���Z�o
	*              (f>=0 && g>=0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�͎O�p�`�� Position0>Position1>Position2 �̓����ɂ���܂�>�B
				   (f==0 && g>=0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�� Position0>Position2 �s�ڂɂ���܂�>�B
				   (f>=0 && g==0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�� Position0>Position1> �s�ڂɂ���܂��B
				   (f>=0 && g>=0 && 1-f-g==0) �̏ꍇ�A�|�C���g�� Position1>Position2 �s�ڂɂ���܂�>�B
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
	*  @brief : �S�Ă̗v�f�ɂ��čŏ��l�ƂȂ����I�����܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Min(const Vector2f& left, const Vector2f& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Min(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f�ɂ��čő�l�ƂȂ����I�����܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Max(const Vector2f& left, const Vector2f& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Max(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : �ł��߂������Ɋۂ߂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Round(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Round(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �؂�̂� (�w�肵�����l�̒[����؂�̂Ă������Ɋۂ߂�B���������0�̕����ɂ��鎟�̐���
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Truncate(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Truncate(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : ���̕����Ɋۂ߂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Floor(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Floor(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �؂�グ ��������Ő��̕����ɂ��鎟�̐����Ɋۂ߂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Ceiling(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Ceiling(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �w��͈͓��Ő��l��ݒ肷��
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Clamp(const Vector2f& vector, const Vector2f& min, const Vector2f& max) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Clamp(vector, min, max);
	}

	/*----------------------------------------------------------------------
	*  @brief : 1�ȏ�ɂ͍s���Ȃ��悤�ɂ���
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Saturate(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Saturate(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : ��Βl
	/*----------------------------------------------------------------------*/
	__forceinline Vector2f Abs(const Vector2f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Abs(vector);
	}
}
#endif