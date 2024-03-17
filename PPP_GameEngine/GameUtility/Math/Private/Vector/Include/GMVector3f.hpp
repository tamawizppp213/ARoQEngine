//////////////////////////////////////////////////////////////////////////////////
///             @file   GMVector3F.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/03 3:58:44
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_VECTOR3F_HPP
#define GM_VECTOR3F_HPP

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
	struct Vector3f;
	struct Float3;

	/****************************************************************************
	*				  			   GMVector3
	*************************************************************************//**
	*  @class     GMVector3
	*  @brief     SIMD���Z�p��Vector3�N���X�ł�. �A���C�������g���s���Ă��邽��, �f�[�^��ێ�����ꍇ��Float3���g�p���Ă�������
	*****************************************************************************/
	struct Float3
	{
	public:
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		float x;
		float y;
		float z;

		__forceinline bool operator ==(const Float3& vector) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::EqualVector3(*this, vector); }
		__forceinline bool operator !=(const Float3& vector) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::NotEqualVector3(*this, vector); }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor
		Float3() : x(0), y(0), z(0) {};

		// @brief : ��̗v�f�ŏ�����
		Float3(const float ix) : x(ix), y(ix), z(ix) {};

		// @brief : �S�Ă̗v�f�ŏ�����
		Float3(const float ix, const float iy, const float iz) : x(ix), y(iy), z(iz) {};

		// @brief : �z����g���ď�����
		explicit Float3(_In_reads_(4) const float* pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]) {};

		// @brief : copy constructor
		Float3(const Float3&) = default;

		Float3& operator=(const Float3&) = default;

		// @brief : move constructor
		Float3(Float3&&) = default;

		Float3& operator=(Float3&&) = default;

		operator VECTOR128() const noexcept
		{
			return SIMD_NAME_SPACE::Vector128Utility::LoadFloat3((float*)this);
		}
	};

	/****************************************************************************
	*				  			   GMVector3
	*************************************************************************//**
	*  @class     GMVector3
	*  @brief     SIMD���Z�p��Vector3�N���X�ł�. �A���C�������g���s���Ă��邽��, �f�[�^��ێ�����ꍇ��Float3���g�p���Ă�������
	*****************************************************************************/
	struct Vector3f
	{
	public:
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
#pragma region Default Vector
		// @brief : (0, 0, 0, 0)
		static const Vector3f ZERO;

		// @brief : (1, 1, 1, 1)
		static const Vector3f ONE;

		// @brief : (1,0,0,0)
		static const Vector3f XAXIS;

		// @brief : (0,1,0,0)
		static const Vector3f YAXIS;

		// @brief : (0,0,1,0)
		static const Vector3f ZAXIS;

		// @brief : (0,0,0,1)
		static const Vector3f WAXIS;

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
		__forceinline gu::float32 GetZ() const { return SIMD_NAME_SPACE::Vector128Utility::GetZ(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : Float3�ɕϊ�����
		/*----------------------------------------------------------------------*/
		__forceinline Float3 ToFloat3()
		{
			Float3 value = {};
			SIMD_NAME_SPACE::Vector128Utility::StoreFloat3((float*)&value, _vector);
			return value;
		}

#pragma endregion Getter

#pragma region Setter
		/*----------------------------------------------------------------------
		*  @brief : Float���g����Vector��X(, Y, Z, W)�v�f�ɒl��������
		/*----------------------------------------------------------------------*/
		__forceinline void SetX(const float x) { _vector = SIMD_NAME_SPACE::Vector128Utility::SetX(_vector, x); }
		__forceinline void SetY(const float y) { _vector = SIMD_NAME_SPACE::Vector128Utility::SetY(_vector, y); }
		__forceinline void SetZ(const float z) { _vector = SIMD_NAME_SPACE::Vector128Utility::SetZ(_vector, z); }
#pragma endregion Setter

#pragma region Operator Function
		// @brief : ���Z
		__forceinline Vector3f operator+  (const Vector3f& right) const { return Vector3f(SIMD_NAME_SPACE::Vector128Utility::Add(_vector, right._vector)); }

		// @brief : ���Z
		__forceinline Vector3f operator- (const Vector3f& right) const { return Vector3f(SIMD_NAME_SPACE::Vector128Utility::Subtract(_vector, right._vector)); }

		// @brief : ��Z
		__forceinline Vector3f operator* (const Vector3f& right) const { return Vector3f(SIMD_NAME_SPACE::Vector128Utility::Multiply(_vector, right._vector)); }

		// @brief : ���Z
		__forceinline Vector3f operator/ (const Vector3f& right) const { return Vector3f(SIMD_NAME_SPACE::Vector128Utility::Vector128Utility::Divide(_vector, right._vector)); }

		// @brief : �]��
		__forceinline Vector3f operator% (const Vector3f& right) const { return Vector3f(SIMD_NAME_SPACE::Vector128Utility::Vector128Utility::Mod(_vector, right._vector)); }

		// @brief : �X�J���[�{
		__forceinline        Vector3f operator* (const float right) const { return Vector3f(SIMD_NAME_SPACE::Vector128Utility::Scale(_vector, right)); }
		__forceinline friend Vector3f operator* (const float left, const Vector3f right) { return Vector3f(SIMD_NAME_SPACE::Vector128Utility::Scale(right._vector, left)); }

		// @brief : �X�J���[�ŏ��Z
		__forceinline        Vector3f operator/ (const float right) const { return Vector3f(SIMD_NAME_SPACE::Vector128Utility::Scale(_vector, 1.0f / right)); }
		__forceinline friend Vector3f operator/ (const float left, const Vector3f right) { return Vector3f(SIMD_NAME_SPACE::Vector128Utility::Scale(right._vector, 1.0f / left)); }

		// @brief : ���Z (_vector�ł͂Ȃ�, *this���g���đ����̂�simd::Vector128���}�N���ɉ����ĕς�邽��, ���b�v����K�v������)
		__forceinline Vector3f& operator +=(const Vector3f& right) { *this = *this + right;  return *this; }

		// @brief : ���Z (_vector�ł͂Ȃ�, *this���g���Ĉ����̂�simd::Vector128���}�N���ɉ����ĕς�邽��, ���b�v����K�v������)
		__forceinline Vector3f& operator -=(const Vector3f& right) { *this = *this - right; return *this; }

		// @brief : ��Z (_vector�ł͂Ȃ�, *this���g���Ă�����̂�simd::Vector128���}�N���ɉ����ĕς�邽��, ���b�v����K�v������)
		__forceinline Vector3f& operator *=(const Vector3f& right) { *this = *this * right; return *this; }

		// @brief : ���Z (_vector�ł͂Ȃ�, *this���g���Ċ���̂�simd::Vector128���}�N���ɉ����ĕς�邽��, ���b�v����K�v������)
		__forceinline Vector3f& operator /=(const Vector3f& right) { *this = *this / right; return *this; }

		// @brief : �S�Ă̗v�f����������
		__forceinline bool operator == (const Vector3f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::EqualVector3(_vector, right._vector); }

		// @brief : ��ł��v�f���قȂ邩
		__forceinline bool operator != (const Vector3f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::NotEqualVector3(_vector, right._vector); }

		// @brief : �S�Ă̗v�f�ő傫����
		__forceinline bool operator >  (const Vector3f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::GreaterVector3(_vector, right._vector); }

		// @brief : �S�Ă̗v�f�ő傫��or��������
		__forceinline bool operator >=  (const Vector3f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::GreaterOrEqualVector3(_vector, right._vector); }

		// @brief : �S�Ă̗v�f�ŏ�������
		__forceinline bool operator <  (const Vector3f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::LessVector3(_vector, right._vector); }

		// @brief : �S�Ă̗v�f�ő傫����
		__forceinline bool operator <= (const Vector3f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::LessOrEqualVector3(_vector, right._vector); }

		/*----------------------------------------------------------------------
		*  @brief : �w��͈͓��ɂ��邩�ǂ��� -bounds <= vector <= +bounds
		/*----------------------------------------------------------------------*/
		__forceinline bool InBounds(const Vector3f& bounds) noexcept { return SIMD_NAME_SPACE::Vector128Utility::InBoundsVector3(_vector, bounds._vector); }

		__forceinline operator VECTOR128() const { return _vector; }

#pragma endregion Operator Function

#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g���̊e�v�f���m�̋t�����Z�o���܂�. (1.0f / vector)
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Reciprocal() const { return SIMD_NAME_SPACE::Vector128Utility::Reciprocal(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �e�v�f�Ƀ}�C�i�X����������̂�Ԃ�.
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Negate() const { return SIMD_NAME_SPACE::Vector128Utility::Negate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 1���m����(�e�v�f�̐�Βl�̘a�ɂ�����x�N�g���̑傫��)���Z�o
		/*----------------------------------------------------------------------*/
		__forceinline float Norm() const { return SIMD_NAME_SPACE::Vector128Utility::NormVector3(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 2���m����(�e�v�f��2��a�����[�g��������Ƃ��̃x�N�g���̑傫��)���Z�o
		/*----------------------------------------------------------------------*/
		__forceinline float NormSquared() const { return SIMD_NAME_SPACE::Vector128Utility::NormSquaredVector3(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : ���ς��Z�o
		/*----------------------------------------------------------------------*/
		__forceinline float Dot(const Vector3f& right) const { return SIMD_NAME_SPACE::Vector128Utility::DotVector3(_vector, right._vector); }

		/*----------------------------------------------------------------------
		*  @brief : �O�ς��Z�o second, third�ɐ����ȃx�N�g����Ԃ�
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Cross(const Vector3f& second) const { return SIMD_NAME_SPACE::Vector128Utility::CrossVector3(_vector, second._vector); }

		/*----------------------------------------------------------------------
		*  @brief : ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Normalize() const { return SIMD_NAME_SPACE::Vector128Utility::NormalizeVector3(_vector); }

		/*----------------------------------------------------------------------
		*  @brief     ���˃x�N�g�����Z�o
		*  @param[in] const Vector128&f : input  (���˃x�N�g��)
		*  @param[in] const Vector128&f : normal (���˂��s���@���x�N�g��)
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Reflect(const Vector3f& normal) const { return SIMD_NAME_SPACE::Vector128Utility::ReflectVector3(_vector, normal._vector); }

		/*----------------------------------------------------------------------
		*  @brief     ���܃x�N�g�����Z�o
		*  @param[in] const Vector3f& : input  (���˃x�N�g��)
		*  @param[in] const Vector3f& : normal (���˂��s���@���x�N�g��)
		*  @param[in] const float    : refractionIndex (���ܗ�)
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Refract(const Vector3f& normal, const float     refractionIndex) noexcept { return SIMD_NAME_SPACE::Vector128Utility::RefractVector3(_vector, normal._vector, refractionIndex); }
		__forceinline Vector3f Refract(const Vector3f& normal, const Vector3f& refractionIndex) noexcept { return SIMD_NAME_SPACE::Vector128Utility::RefractVector3V(_vector, normal._vector, refractionIndex._vector); }

		/*----------------------------------------------------------------------
		*  @brief : ���ɑ傫���l��
		/*----------------------------------------------------------------------*/
		__forceinline bool IsInfinite() const { return SIMD_NAME_SPACE::Vector128Utility::IsInfiniteVector3(_vector); }

		/*----------------------------------------------------------------------
		*  @brief :  �[�����ǂ���
		/*----------------------------------------------------------------------*/
		__forceinline bool IsZero() const { return ZERO == _vector; }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�ɍ�������������̂�Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Sqrt() const { return SIMD_NAME_SPACE::Vector128Utility::Sqrt(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�̋t�������, ����ɍ�������������̂�Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f ReciprocalSqrt() const { return SIMD_NAME_SPACE::Vector128Utility::ReciprocalSqrt(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��sin, cos, tan��Ԃ��܂� [rad]
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Sin() const { return SIMD_NAME_SPACE::Vector128Utility::Sin(_vector); }
		__forceinline Vector3f Cos() const { return SIMD_NAME_SPACE::Vector128Utility::Cos(_vector); }
		__forceinline Vector3f Tan() const { return SIMD_NAME_SPACE::Vector128Utility::Tan(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��arcsin, arccos, arctan��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f ArcSin() const { return SIMD_NAME_SPACE::Vector128Utility::ArcSin(_vector); }
		__forceinline Vector3f ArcCos() const { return SIMD_NAME_SPACE::Vector128Utility::ArcCos(_vector); }
		__forceinline Vector3f ArcTan() const { return SIMD_NAME_SPACE::Vector128Utility::ArcTan(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��sinH, cosH, tanH��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f SinH() const { return SIMD_NAME_SPACE::Vector128Utility::SinH(_vector); }
		__forceinline Vector3f CosH() const { return SIMD_NAME_SPACE::Vector128Utility::CosH(_vector); }
		__forceinline Vector3f TanH() const { return SIMD_NAME_SPACE::Vector128Utility::TanH(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�ɑ΂���sin������cos���������o���܂�
		/*----------------------------------------------------------------------*/
		__forceinline void SinCos(Vector3f& sin, Vector3f& cos) noexcept
		{
			SIMD_NAME_SPACE::Vector128Utility::SinCos(_vector, &sin._vector, &cos._vector);
		}

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��2^{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Exp2() const { return SIMD_NAME_SPACE::Vector128Utility::Exp2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��10^{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Exp10() const { return SIMD_NAME_SPACE::Vector128Utility::Exp10(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��e^{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f ExpE() const { return SIMD_NAME_SPACE::Vector128Utility::ExpE(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��log2{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Log2() const { return SIMD_NAME_SPACE::Vector128Utility::Log2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��log10{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Log10() const { return SIMD_NAME_SPACE::Vector128Utility::Log10(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��loge{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f LogE() const { return SIMD_NAME_SPACE::Vector128Utility::LogE(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��Pow��Ԃ��܂�
		*           base^{power}
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Pow(const Vector3f& power) const { return SIMD_NAME_SPACE::Vector128Utility::Pow(_vector, power._vector); }

		/*----------------------------------------------------------------------
		*  @brief : �ł��߂������Ɋۂ߂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Round() const { return SIMD_NAME_SPACE::Vector128Utility::Round(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �؂�̂� (�w�肵�����l�̒[����؂�̂Ă������Ɋۂ߂�B���������0�̕����ɂ��鎟�̐���
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Truncate() const { return SIMD_NAME_SPACE::Vector128Utility::Truncate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : ���̕����Ɋۂ߂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Floor() const { return SIMD_NAME_SPACE::Vector128Utility::Floor(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �؂�グ ��������Ő��̕����ɂ��鎟�̐����Ɋۂ߂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Ceiling() const { return SIMD_NAME_SPACE::Vector128Utility::Ceiling(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �w��͈͓��Ő��l��ݒ肷��
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Clamp(const Vector3f& min, const Vector3f& max) const { return SIMD_NAME_SPACE::Vector128Utility::Clamp(_vector, min._vector, max._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 1�ȏ�ɂ͍s���Ȃ��悤�ɂ���
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Saturate() const { return SIMD_NAME_SPACE::Vector128Utility::Saturate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : ��Βl
		/*----------------------------------------------------------------------*/
		__forceinline Vector3f Abs() noexcept { return SIMD_NAME_SPACE::Vector128Utility::Abs(_vector); }

#pragma endregion Math
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor
		Vector3f() { _vector = SIMD_NAME_SPACE::Vector128Utility::Zero(); }

		// @brief : Float�^�Ő�������R���X�g���N�^
		Vector3f(const gu::float32 x, const gu::float32 y, const gu::float32 z)
		{
			_vector = SIMD_NAME_SPACE::Vector128Utility::Set(x, y, z, 0.0f);
		}

		// @brief : Vector128�𒼐ڑ������R���X�g���N�^
		Vector3f(const VECTOR128 vector) : _vector(vector) {};

		// @brief : Float3�^�Ő�������R���X�g���N�^
		Vector3f(const Float3& vector) { _vector = SIMD_NAME_SPACE::Vector128Utility::LoadFloat3(&vector.x); }

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
	__forceinline float Dot(const Vector3f& left, const Vector3f& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::DotVector3(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : �O�ς��Z�o first, second, third�ɐ����ȃx�N�g����Ԃ�
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Cross(const Vector3f& first, const Vector3f& second) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::CrossVector3(first, second);
	}

	/*----------------------------------------------------------------------
	*  @brief : ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Normalize(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::NormalizeVector3(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief     ���˃x�N�g�����Z�o
	*  @param[in] const Vector3f& : input  (���˃x�N�g��)
	*  @param[in] const Vector3f& : normal (���˂��s���@���x�N�g��)
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Reflect(const Vector3f& input, const Vector3f& normal) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ReflectVector3(input, normal);
	}

	/*----------------------------------------------------------------------
	*  @brief     ���܃x�N�g�����Z�o
	*  @param[in] const Vector3f& : input  (���˃x�N�g��)
	*  @param[in] const Vector3f& : normal (���˂��s���@���x�N�g��)
	*  @param[in] const float    : refractionIndex (���ܗ�)
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Refract(const Vector3f& input, const Vector3f& normal, const float refractionIndex) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::RefractVector3(input, normal, refractionIndex);
	}

	__forceinline Vector3f RefractV(const Vector3f& input, const Vector3f& normal, const Vector3f& refractionIndex) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::RefractVector3V(input, normal, refractionIndex);
	}

	/*----------------------------------------------------------------------
	*  @brief : ���ɑ傫���l��
	/*----------------------------------------------------------------------*/
	__forceinline bool IsInfinite(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::IsInfiniteVector3(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f�ɍ�������������̂�Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Sqrt(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Sqrt(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f�̋t�������, ����ɍ�������������̂�Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f ReciprocalSqrt(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ReciprocalSqrt(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��sin, cos, tan��Ԃ��܂� [rad]
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Sin(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Sin(vector);
	}

	__forceinline Vector3f Cos(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Cos(vector);
	}

	__forceinline Vector3f Tan(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Tan(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��arcsin, arccos, arctan��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f ArcSin(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ArcSin(vector);
	}

	__forceinline Vector3f ArcCos(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ArcCos(vector);
	}

	__forceinline Vector3f ArcTan(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ArcTan(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��sinH, cosH, tanH��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f SinH(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::SinH(vector);
	}

	__forceinline Vector3f CosH(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::CosH(vector);
	}

	__forceinline Vector3f TanH(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::TanH(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��2^{vector}��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Exp2(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Exp2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��10^{vector}��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Exp10(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Exp10(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��e^{vector}��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f ExpE(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ExpE(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��log2{vector}��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Log2(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Log2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��log10{vector}��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Log10(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Log10(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��loge{vector}��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f LogE(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::LogE(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��Pow��Ԃ��܂�
	*           base^{power}
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Pow(const Vector3f& base, const Vector3f& power) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Pow(base, power);
	}

	/*----------------------------------------------------------------------
	*  @brief      ���`��Ԃ�Ԃ��܂�
	*  @param[in]  t = 0�̎��̒l
	   @param[in]  t = 1�̎��̒l
	   @param[in]  t : ���`��Ԃ̊���
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Lerp(const Vector3f& start, const Vector3f& end, const float t) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Lerp(start, end, t);
	}

	__forceinline Vector3f LerpV(const Vector3f& start, const Vector3f& end, const Vector3f& t) noexcept
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
	__forceinline Vector3f Hermite(const Vector3f& startPosition, const Vector3f& startTangent, const Vector3f& endPosition, const Vector3f& endTangent, const float t) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Hermite(startPosition, startTangent, endPosition, endTangent, t);
	}

	__forceinline Vector3f HermiteV(const Vector3f& startPosition, const Vector3f& startTangent, const Vector3f& endPosition, const Vector3f& endTangent, const Vector3f& t) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::HermiteV(startPosition, startTangent, endPosition, endTangent, t);
	}

	/*----------------------------------------------------------------------
	*  @brief      CatMull-Rom�X�v���C����Ԃ̌��ʂ�Ԃ��܂��B4�̐���_��S�Ēʂ�X�v���C�����
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f CatMullRom(const Vector3f& position0, const Vector3f& position1, const Vector3f& position2, const Vector3f& position3, const float t) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::CatMullRom(position0, position1, position2, position3, t);
	}

	__forceinline Vector3f CatMullRomV(const Vector3f& position0, const Vector3f& position1, const Vector3f& position2, const Vector3f& position3, const Vector3f& t) noexcept
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
	__forceinline Vector3f BaryCentric(const Vector3f& position0, const Vector3f& position1, const Vector3f& position2, const float f, const float g) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::BaryCentric(position0, position1, position2, f, g);
	}

	__forceinline Vector3f BaryCentricV(const Vector3f& position0, const Vector3f& position1, const Vector3f& position2, const Vector3f& f, const Vector3f& g) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::BaryCentricV(position0, position1, position2, f, g);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f�ɂ��čŏ��l�ƂȂ����I�����܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Min(const Vector3f& left, const Vector3f& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Min(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f�ɂ��čő�l�ƂȂ����I�����܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Max(const Vector3f& left, const Vector3f& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Max(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : �ł��߂������Ɋۂ߂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Round(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Round(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �؂�̂� (�w�肵�����l�̒[����؂�̂Ă������Ɋۂ߂�B���������0�̕����ɂ��鎟�̐���
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Truncate(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Truncate(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : ���̕����Ɋۂ߂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Floor(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Floor(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �؂�グ ��������Ő��̕����ɂ��鎟�̐����Ɋۂ߂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Ceiling(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Ceiling(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �w��͈͓��Ő��l��ݒ肷��
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Clamp(const Vector3f& vector, const Vector3f& min, const Vector3f& max) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Clamp(vector, min, max);
	}

	/*----------------------------------------------------------------------
	*  @brief : 1�ȏ�ɂ͍s���Ȃ��悤�ɂ���
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Saturate(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Saturate(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : ��Βl
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f Abs(const Vector3f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Abs(vector);
	}
}
#endif