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
	*  @brief     SIMD���Z�p��Vector4�N���X�ł�. �A���C�������g���s���Ă��邽��, �f�[�^��ێ�����ꍇ��Double3���g�p���Ă�������
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

		// @brief : ��̗v�f�ŏ�����
		Double3(const double ix) : x(ix), y(ix), z(ix) {};

		// @brief : �S�Ă̗v�f�ŏ�����
		Double3(const double ix, const double iy, const double iz) : x(ix), y(iy), z(iz) {};

		// @brief : �z����g���ď�����
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
	*  @brief     SIMD���Z�p��Vector4�N���X�ł�. �A���C�������g���s���Ă��邽��, �f�[�^��ێ�����ꍇ��Double3���g�p���Ă�������
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
		*  @brief : FPU���W�X�^�Ɋi�[���ꂽX(, Y, Z, W)���̗v�f�����o��
		*----------------------------------------------------------------------*/
		__forceinline gu::double64 GetX() const { return SIMD_NAME_SPACE::Vector256Utility::GetX(_vector); }
		__forceinline gu::double64 GetY() const { return SIMD_NAME_SPACE::Vector256Utility::GetY(_vector); }
		__forceinline gu::double64 GetZ() const { return SIMD_NAME_SPACE::Vector256Utility::GetZ(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : Double3�ɕϊ�����
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
		*  @brief : Float���g����Vector��X(, Y, Z, W)�v�f�ɒl��������
		*----------------------------------------------------------------------*/
		__forceinline void SetX(const double x) { _vector = SIMD_NAME_SPACE::Vector256Utility::SetX(_vector, x); }
		__forceinline void SetY(const double y) { _vector = SIMD_NAME_SPACE::Vector256Utility::SetY(_vector, y); }
		__forceinline void SetZ(const double z) { _vector = SIMD_NAME_SPACE::Vector256Utility::SetZ(_vector, z); }
		
#pragma endregion Setter

#pragma region Operator Function
		// @brief : ���Z
		__forceinline Vector3d operator+  (const Vector3d& right) const { return Vector3d(SIMD_NAME_SPACE::Vector256Utility::Add(_vector, right._vector)); }

		// @brief : ���Z
		__forceinline Vector3d operator- (const Vector3d& right) const { return Vector3d(SIMD_NAME_SPACE::Vector256Utility::Subtract(_vector, right._vector)); }

		// @brief : ��Z
		__forceinline Vector3d operator* (const Vector3d& right) const { return Vector3d(SIMD_NAME_SPACE::Vector256Utility::Multiply(_vector, right._vector)); }

		// @brief : ���Z
		__forceinline Vector3d operator/ (const Vector3d& right) const { return Vector3d(SIMD_NAME_SPACE::Vector256Utility::Vector256Utility::Divide(_vector, right._vector)); }

		// @brief : �]��
		__forceinline Vector3d operator% (const Vector3d& right) const { return Vector3d(SIMD_NAME_SPACE::Vector256Utility::Vector256Utility::Mod(_vector, right._vector)); }

		// @brief : ����
		__forceinline double operator| (const Vector3d& right) const { return SIMD_NAME_SPACE::Vector256Utility::DotVector3(_vector, right._vector); }

		// @brief : �X�J���[�{
		__forceinline        Vector3d operator* (const double right) const { return Vector3d(SIMD_NAME_SPACE::Vector256Utility::Scale(_vector, right)); }
		__forceinline friend Vector3d operator* (const double left, const Vector3d right) { return Vector3d(SIMD_NAME_SPACE::Vector256Utility::Scale(right._vector, left)); }

		// @brief : �X�J���[�ŏ��Z
		__forceinline        Vector3d operator/ (const double right) const { return Vector3d(SIMD_NAME_SPACE::Vector256Utility::Scale(_vector, 1.0f / right)); }
		__forceinline friend Vector3d operator/ (const double left, const Vector3d right) { return Vector3d(SIMD_NAME_SPACE::Vector256Utility::Scale(right._vector, 1.0f / left)); }

		// @brief : ���Z (_vector�ł͂Ȃ�, *this���g���đ����̂�simd::Vector256���}�N���ɉ����ĕς�邽��, ���b�v����K�v������)
		__forceinline Vector3d& operator +=(const Vector3d& right) { *this = *this + right;  return *this; }

		// @brief : ���Z (_vector�ł͂Ȃ�, *this���g���Ĉ����̂�simd::Vector256���}�N���ɉ����ĕς�邽��, ���b�v����K�v������)
		__forceinline Vector3d& operator -=(const Vector3d& right) { *this = *this - right; return *this; }

		// @brief : ��Z (_vector�ł͂Ȃ�, *this���g���Ă�����̂�simd::Vector256���}�N���ɉ����ĕς�邽��, ���b�v����K�v������)
		__forceinline Vector3d& operator *=(const Vector3d& right) { *this = *this * right; return *this; }

		// @brief : ���Z (_vector�ł͂Ȃ�, *this���g���Ċ���̂�simd::Vector256���}�N���ɉ����ĕς�邽��, ���b�v����K�v������)
		__forceinline Vector3d& operator /=(const Vector3d& right) { *this = *this / right; return *this; }

		// @brief : �S�Ă̗v�f����������
		__forceinline bool operator == (const Vector3d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::EqualVector4(_vector, right._vector); }

		// @brief : ��ł��v�f���قȂ邩
		__forceinline bool operator != (const Vector3d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::NotEqualVector4(_vector, right._vector); }

		// @brief : �S�Ă̗v�f�ő傫����
		__forceinline bool operator >  (const Vector3d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::GreaterVector4(_vector, right._vector); }

		// @brief : �S�Ă̗v�f�ő傫��or��������
		__forceinline bool operator >=  (const Vector3d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::GreaterOrEqualVector4(_vector, right._vector); }

		// @brief : �S�Ă̗v�f�ŏ�������
		__forceinline bool operator <  (const Vector3d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::LessVector4(_vector, right._vector); }

		// @brief : �S�Ă̗v�f�ő傫����
		__forceinline bool operator <= (const Vector3d& right) const noexcept { return SIMD_NAME_SPACE::Vector256Utility::LessOrEqualVector4(_vector, right._vector); }

		// @brief : ���ڗv�f�ɃA�N�Z�X���܂�
		__forceinline       double& operator[](const gu::uint32 index)       noexcept { return _vector.m256d_f64[index]; }
		__forceinline const double& operator[](const gu::uint32 index) const noexcept { return _vector.m256d_f64[index]; }

		/*----------------------------------------------------------------------
		*  @brief : �w��͈͓��ɂ��邩�ǂ��� -bounds <= vector <= +bounds
		*----------------------------------------------------------------------*/
		__forceinline bool InBounds(const Vector3d& bounds) noexcept { return SIMD_NAME_SPACE::Vector256Utility::InBoundsVector4(_vector, bounds._vector); }

		__forceinline operator VECTOR256() const { return _vector; }

#pragma endregion Operator Function

#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g���̊e�v�f���m�̋t�����Z�o���܂�. (1.0f / vector)
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Reciprocal() const { return SIMD_NAME_SPACE::Vector256Utility::Reciprocal(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �e�v�f�Ƀ}�C�i�X����������̂�Ԃ�.
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Negate() const { return SIMD_NAME_SPACE::Vector256Utility::Negate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 2���m����(�e�v�f��2��a�����[�g��������Ƃ��̃x�N�g���̑傫��)���Z�o
		*----------------------------------------------------------------------*/
		__forceinline double Length() const { return SIMD_NAME_SPACE::Vector256Utility::LengthVector3(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 2���m�����̓����Z�o
		*----------------------------------------------------------------------*/
		__forceinline double LengthSquared() const { return SIMD_NAME_SPACE::Vector256Utility::LengthSquaredVector3(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : ���ς��Z�o
		*----------------------------------------------------------------------*/
		__forceinline double Dot(const Vector3d& right) const { return SIMD_NAME_SPACE::Vector256Utility::DotVector3(_vector, right._vector); }

		/*----------------------------------------------------------------------
		*  @brief : �O�ς��Z�o second�ɐ����ȃx�N�g����Ԃ�
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Cross(const Vector3d& second) const { return SIMD_NAME_SPACE::Vector256Utility::CrossVector3(_vector, second._vector); }

		/*----------------------------------------------------------------------
		*  @brief : ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Normalize() const { return SIMD_NAME_SPACE::Vector256Utility::NormalizeVector3(_vector); }

		/*----------------------------------------------------------------------
		*  @brief     ���˃x�N�g�����Z�o
		*  @param[in] const Vector256&f : input  (���˃x�N�g��)
		*  @param[in] const Vector256&f : normal (���˂��s���@���x�N�g��)
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Reflect(const Vector3d& normal) const { return SIMD_NAME_SPACE::Vector256Utility::ReflectVector3(_vector, normal._vector); }

		/*----------------------------------------------------------------------
		*  @brief     ���܃x�N�g�����Z�o
		*  @param[in] const Vector3d& : input  (���˃x�N�g��)
		*  @param[in] const Vector3d& : normal (���˂��s���@���x�N�g��)
		*  @param[in] const double    : refractionIndex (���ܗ�)
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Refract(const Vector3d& normal, const double     refractionIndex) noexcept { return SIMD_NAME_SPACE::Vector256Utility::RefractVector3(_vector, normal._vector, refractionIndex); }
		__forceinline Vector3d Refract(const Vector3d& normal, const Vector3d& refractionIndex) noexcept { return SIMD_NAME_SPACE::Vector256Utility::RefractVector3V(_vector, normal._vector, refractionIndex._vector); }

		/*----------------------------------------------------------------------
		*  @brief : ���ɑ傫���l��
		*----------------------------------------------------------------------*/
		__forceinline bool IsInfinite() const { return SIMD_NAME_SPACE::Vector256Utility::IsInfiniteVector3(_vector); }

		/*----------------------------------------------------------------------
		*  @brief :  �[�����ǂ���
		*----------------------------------------------------------------------*/
		__forceinline bool IsZero() const { return ZERO == _vector; }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�ɍ�������������̂�Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Sqrt() const { return SIMD_NAME_SPACE::Vector256Utility::Sqrt(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�̋t�������, ����ɍ�������������̂�Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline Vector3d ReciprocalSqrt() const { return SIMD_NAME_SPACE::Vector256Utility::ReciprocalSqrt(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��sin, cos, tan��Ԃ��܂� [rad]
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Sin() const { return SIMD_NAME_SPACE::Vector256Utility::Sin(_vector); }
		__forceinline Vector3d Cos() const { return SIMD_NAME_SPACE::Vector256Utility::Cos(_vector); }
		__forceinline Vector3d Tan() const { return SIMD_NAME_SPACE::Vector256Utility::Tan(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��arcsin, arccos, arctan��Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline Vector3d ArcSin() const { return SIMD_NAME_SPACE::Vector256Utility::ArcSin(_vector); }
		__forceinline Vector3d ArcCos() const { return SIMD_NAME_SPACE::Vector256Utility::ArcCos(_vector); }
		__forceinline Vector3d ArcTan() const { return SIMD_NAME_SPACE::Vector256Utility::ArcTan(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��sinH, cosH, tanH��Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline Vector3d SinH() const { return SIMD_NAME_SPACE::Vector256Utility::SinH(_vector); }
		__forceinline Vector3d CosH() const { return SIMD_NAME_SPACE::Vector256Utility::CosH(_vector); }
		__forceinline Vector3d TanH() const { return SIMD_NAME_SPACE::Vector256Utility::TanH(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�ɑ΂���sin������cos���������o���܂�
		*----------------------------------------------------------------------*/
		__forceinline void SinCos(Vector3d& sin, Vector3d& cos) noexcept
		{
			SIMD_NAME_SPACE::Vector256Utility::SinCos(_vector, &sin._vector, &cos._vector);
		}

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��2^{vector}��Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Exp2() const { return SIMD_NAME_SPACE::Vector256Utility::Exp2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��10^{vector}��Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Exp10() const { return SIMD_NAME_SPACE::Vector256Utility::Exp10(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��e^{vector}��Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline Vector3d ExpE() const { return SIMD_NAME_SPACE::Vector256Utility::ExpE(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��log2{vector}��Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Log2() const { return SIMD_NAME_SPACE::Vector256Utility::Log2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��log10{vector}��Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Log10() const { return SIMD_NAME_SPACE::Vector256Utility::Log10(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��loge{vector}��Ԃ��܂�
		*----------------------------------------------------------------------*/
		__forceinline Vector3d LogE() const { return SIMD_NAME_SPACE::Vector256Utility::LogE(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��Pow��Ԃ��܂�
		*           base^{power}
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Pow(const Vector3d& power) const { return SIMD_NAME_SPACE::Vector256Utility::Pow(_vector, power._vector); }

		/*----------------------------------------------------------------------
		*  @brief : �ł��߂������Ɋۂ߂�
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Round() const { return SIMD_NAME_SPACE::Vector256Utility::Round(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �؂�̂� (�w�肵�����l�̒[����؂�̂Ă������Ɋۂ߂�B���������0�̕����ɂ��鎟�̐���
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Truncate() const { return SIMD_NAME_SPACE::Vector256Utility::Truncate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : ���̕����Ɋۂ߂�
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Floor() const { return SIMD_NAME_SPACE::Vector256Utility::Floor(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �؂�グ ��������Ő��̕����ɂ��鎟�̐����Ɋۂ߂�
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Ceiling() const { return SIMD_NAME_SPACE::Vector256Utility::Ceiling(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �w��͈͓��Ő��l��ݒ肷��
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Clamp(const Vector3d& min, const Vector3d& max) const { return SIMD_NAME_SPACE::Vector256Utility::Clamp(_vector, min._vector, max._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 1�ȏ�ɂ͍s���Ȃ��悤�ɂ���
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Saturate() const { return SIMD_NAME_SPACE::Vector256Utility::Saturate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : ��Βl
		*----------------------------------------------------------------------*/
		__forceinline Vector3d Abs() const { return SIMD_NAME_SPACE::Vector256Utility::Abs(_vector); }

#pragma endregion Math
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor
		Vector3d() { _vector = SIMD_NAME_SPACE::Vector256Utility::Zero(); }

		// @brief : Float�^�Ő�������R���X�g���N�^
		Vector3d(const gu::double64 x, const gu::double64 y, const gu::double64 z)
		{
			_vector = SIMD_NAME_SPACE::Vector256Utility::Set(x, y, z, 0.0);
		}

		// @brief : Vector256�𒼐ڑ������R���X�g���N�^
		Vector3d(const VECTOR256 vector) : _vector(vector) {};

		// @brief : Double3�^�Ő�������R���X�g���N�^
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
	*  @brief : ���ς��Z�o (������, �Ԃ�l��double�ł͂Ȃ�Vector256�^�ŕԂ���܂��̂�, ��XGetX�ȂǂőΉ����Ă�������.)
	*----------------------------------------------------------------------*/
	__forceinline double Dot(const Vector3d& left, const Vector3d& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::DotVector3(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : �O�ς��Z�o first, second, third�ɐ����ȃx�N�g����Ԃ�
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Cross(const Vector3d& first, const Vector3d& second) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::CrossVector3(first, second);
	}

	/*----------------------------------------------------------------------
	*  @brief : ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Normalize(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::NormalizeVector3(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief     ���˃x�N�g�����Z�o
	*  @param[in] const Vector3d& : input  (���˃x�N�g��)
	*  @param[in] const Vector3d& : normal (���˂��s���@���x�N�g��)
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Reflect(const Vector3d& input, const Vector3d& normal) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ReflectVector3(input, normal);
	}

	/*----------------------------------------------------------------------
	*  @brief     ���܃x�N�g�����Z�o
	*  @param[in] const Vector3d& : input  (���˃x�N�g��)
	*  @param[in] const Vector3d& : normal (���˂��s���@���x�N�g��)
	*  @param[in] const double    : refractionIndex (���ܗ�)
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
	*  @brief : ���ɑ傫���l��
	*----------------------------------------------------------------------*/
	__forceinline bool IsInfinite(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::IsInfiniteVector3(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f�ɍ�������������̂�Ԃ��܂�
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Sqrt(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Sqrt(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f�̋t�������, ����ɍ�������������̂�Ԃ��܂�
	*----------------------------------------------------------------------*/
	__forceinline Vector3d ReciprocalSqrt(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ReciprocalSqrt(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��sin, cos, tan��Ԃ��܂� [rad]
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
	*  @brief : �S�Ă̗v�f��arcsin, arccos, arctan��Ԃ��܂�
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
	*  @brief : �S�Ă̗v�f��sinH, cosH, tanH��Ԃ��܂�
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
	*  @brief : �S�Ă̗v�f��2^{vector}��Ԃ��܂�
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Exp2(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Exp2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��10^{vector}��Ԃ��܂�
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Exp10(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Exp10(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��e^{vector}��Ԃ��܂�
	*----------------------------------------------------------------------*/
	__forceinline Vector3d ExpE(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::ExpE(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��log2{vector}��Ԃ��܂�
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Log2(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Log2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��log10{vector}��Ԃ��܂�
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Log10(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Log10(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��loge{vector}��Ԃ��܂�
	*----------------------------------------------------------------------*/
	__forceinline Vector3d LogE(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::LogE(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��Pow��Ԃ��܂�
	*           base^{power}
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Pow(const Vector3d& base, const Vector3d& power) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Pow(base, power);
	}

	/*----------------------------------------------------------------------
	*  @brief      ���`��Ԃ�Ԃ��܂�
	*  @param[in]  t = 0�̎��̒l
	   @param[in]  t = 1�̎��̒l
	   @param[in]  t : ���`��Ԃ̊���
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
	*  @brief      �G���~�[�g��Ԃ�Ԃ��܂�
	*  @param[in]  �n�_�̈ʒu
	   @param[in]  �n�_�ɂ�����X�v���C���̐ڐ���\���x�N�g��
	   @param[in]  �I�_�̈ʒu
	   @param[in]  �I�_�ɂ�����X�v���C���̐ڐ���\���x�N�g��
	   @param[in]  t : �G���~�[�g��Ԃ̊���
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
	*  @brief      CatMull-Rom�X�v���C����Ԃ̌��ʂ�Ԃ��܂��B4�̐���_��S�Ēʂ�X�v���C�����
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
	*  @brief      �O�p�`�̏d�S�d�݈ʒu���Z�o
	*              (f>=0 && g>=0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�͎O�p�`�� Position0>Position1>Position2 �̓����ɂ���܂�>�B
				   (f==0 && g>=0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�� Position0>Position2 �s�ڂɂ���܂�>�B
				   (f>=0 && g==0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�� Position0>Position1> �s�ڂɂ���܂��B
				   (f>=0 && g>=0 && 1-f-g==0) �̏ꍇ�A�|�C���g�� Position1>Position2 �s�ڂɂ���܂�>�B
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
	*  @brief : �S�Ă̗v�f�ɂ��čŏ��l�ƂȂ����I�����܂�
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Min(const Vector3d& left, const Vector3d& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Min(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f�ɂ��čő�l�ƂȂ����I�����܂�
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Max(const Vector3d& left, const Vector3d& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Max(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : �ł��߂������Ɋۂ߂�
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Round(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Round(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �؂�̂� (�w�肵�����l�̒[����؂�̂Ă������Ɋۂ߂�B���������0�̕����ɂ��鎟�̐���
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Truncate(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Truncate(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : ���̕����Ɋۂ߂�
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Floor(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Floor(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �؂�グ ��������Ő��̕����ɂ��鎟�̐����Ɋۂ߂�
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Ceiling(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Ceiling(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �w��͈͓��Ő��l��ݒ肷��
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Clamp(const Vector3d& vector, const Vector3d& min, const Vector3d& max) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Clamp(vector, min, max);
	}

	/*----------------------------------------------------------------------
	*  @brief : 1�ȏ�ɂ͍s���Ȃ��悤�ɂ���
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Saturate(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Saturate(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : ��Βl
	*----------------------------------------------------------------------*/
	__forceinline Vector3d Abs(const Vector3d& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector256Utility::Abs(vector);
	}
}
#endif