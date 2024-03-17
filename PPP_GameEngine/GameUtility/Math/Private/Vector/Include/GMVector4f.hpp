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
	*  @brief     SIMD���Z�p��Vector4�N���X�ł�. �A���C�������g���s���Ă��邽��, �f�[�^��ێ�����ꍇ��Float4���g�p���Ă�������
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

		// @brief : ��̗v�f�ŏ�����
		Float4(const float ix) : x(ix), y(ix), z(ix), w(ix) {};

		// @brief : �S�Ă̗v�f�ŏ�����
		Float4(const float ix, const float iy, const float iz, const float iw) : x(ix), y(iy), z(iz), w(iw) {};

		// @brief : �z����g���ď�����
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
	*  @brief     SIMD���Z�p��Vector4�N���X�ł�. �A���C�������g���s���Ă��邽��, �f�[�^��ێ�����ꍇ��Float4���g�p���Ă�������
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
		*  @brief : FPU���W�X�^�Ɋi�[���ꂽX(, Y, Z, W)���̗v�f�����o��
		/*----------------------------------------------------------------------*/
		__forceinline gu::float32 GetX() const { return SIMD_NAME_SPACE::Vector128Utility::GetX(_vector); }
		__forceinline gu::float32 GetY() const { return SIMD_NAME_SPACE::Vector128Utility::GetY(_vector); }
		__forceinline gu::float32 GetZ() const { return SIMD_NAME_SPACE::Vector128Utility::GetZ(_vector); }
		__forceinline gu::float32 GetW() const { return SIMD_NAME_SPACE::Vector128Utility::GetW(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : Float4�ɕϊ�����
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
		*  @brief : Float���g����Vector��X(, Y, Z, W)�v�f�ɒl��������
		/*----------------------------------------------------------------------*/
		__forceinline void SetX(const float x) { _vector = SIMD_NAME_SPACE::Vector128Utility::SetX(_vector, x); }
		__forceinline void SetY(const float y) { _vector = SIMD_NAME_SPACE::Vector128Utility::SetY(_vector, y); }
		__forceinline void SetZ(const float z) { _vector = SIMD_NAME_SPACE::Vector128Utility::SetZ(_vector, z); }
		__forceinline void SetW(const float w) { _vector = SIMD_NAME_SPACE::Vector128Utility::SetW(_vector, w); }
		#pragma endregion Setter

		#pragma region Operator Function
		// @brief : ���Z
		__forceinline Vector4f operator+  (const Vector4f& right) const { return Vector4f(SIMD_NAME_SPACE::Vector128Utility::Add(_vector, right._vector)); }
		
		// @brief : ���Z
		__forceinline Vector4f operator- (const Vector4f& right) const { return Vector4f(SIMD_NAME_SPACE::Vector128Utility::Subtract(_vector, right._vector)); }

		// @brief : ��Z
		__forceinline Vector4f operator* (const Vector4f& right) const { return Vector4f(SIMD_NAME_SPACE::Vector128Utility::Multiply(_vector, right._vector)); }

		// @brief : ���Z
		__forceinline Vector4f operator/ (const Vector4f& right) const { return Vector4f(SIMD_NAME_SPACE::Vector128Utility::Vector128Utility::Divide(_vector, right._vector)); }

		// @brief : �]��
		__forceinline Vector4f operator% (const Vector4f& right) const { return Vector4f(SIMD_NAME_SPACE::Vector128Utility::Vector128Utility::Mod(_vector, right._vector)); }

		// @brief : �X�J���[�{
		__forceinline        Vector4f operator* (const float right) const { return Vector4f(SIMD_NAME_SPACE::Vector128Utility::Scale(_vector, right)); }
		__forceinline friend Vector4f operator* (const float left, const Vector4f right) { return Vector4f(SIMD_NAME_SPACE::Vector128Utility::Scale(right._vector, left)); }

		// @brief : �X�J���[�ŏ��Z
		__forceinline        Vector4f operator/ (const float right) const                { return Vector4f(SIMD_NAME_SPACE::Vector128Utility::Scale(_vector, 1.0f / right)); }
		__forceinline friend Vector4f operator/ (const float left, const Vector4f right) { return Vector4f(SIMD_NAME_SPACE::Vector128Utility::Scale(right._vector, 1.0f / left)); }

		// @brief : ���Z (_vector�ł͂Ȃ�, *this���g���đ����̂�simd::Vector128���}�N���ɉ����ĕς�邽��, ���b�v����K�v������)
		__forceinline Vector4f& operator +=(const Vector4f& right) { *this = *this + right;  return *this; }

		// @brief : ���Z (_vector�ł͂Ȃ�, *this���g���Ĉ����̂�simd::Vector128���}�N���ɉ����ĕς�邽��, ���b�v����K�v������)
		__forceinline Vector4f& operator -=(const Vector4f& right) { *this = *this - right; return *this; }

		// @brief : ��Z (_vector�ł͂Ȃ�, *this���g���Ă�����̂�simd::Vector128���}�N���ɉ����ĕς�邽��, ���b�v����K�v������)
		__forceinline Vector4f& operator *=(const Vector4f& right) { *this = *this * right; return *this; }

		// @brief : ���Z (_vector�ł͂Ȃ�, *this���g���Ċ���̂�simd::Vector128���}�N���ɉ����ĕς�邽��, ���b�v����K�v������)
		__forceinline Vector4f& operator /=(const Vector4f& right) { *this = *this / right; return *this; }

		// @brief : �S�Ă̗v�f����������
		__forceinline bool operator == (const Vector4f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::EqualVector4(_vector, right._vector); }

		// @brief : ��ł��v�f���قȂ邩
		__forceinline bool operator != (const Vector4f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::NotEqualVector4(_vector, right._vector); }
		
		// @brief : �S�Ă̗v�f�ő傫����
		__forceinline bool operator >  (const Vector4f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::GreaterVector4(_vector, right._vector); }

		// @brief : �S�Ă̗v�f�ő傫��or��������
		__forceinline bool operator >=  (const Vector4f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::GreaterOrEqualVector4(_vector, right._vector); }
		
		// @brief : �S�Ă̗v�f�ŏ�������
		__forceinline bool operator <  (const Vector4f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::LessVector4(_vector, right._vector); }

		// @brief : �S�Ă̗v�f�ő傫����
		__forceinline bool operator <= (const Vector4f& right) const noexcept { return SIMD_NAME_SPACE::Vector128Utility::LessOrEqualVector4(_vector, right._vector); }

		/*----------------------------------------------------------------------
		*  @brief : �w��͈͓��ɂ��邩�ǂ��� -bounds <= vector <= +bounds
		/*----------------------------------------------------------------------*/
		__forceinline bool InBounds(const Vector4f& bounds) noexcept { return SIMD_NAME_SPACE::Vector128Utility::InBoundsVector4(_vector, bounds._vector); }
		
		__forceinline operator VECTOR128() const { return _vector; }

		#pragma endregion Operator Function

		#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : 2�̃x�N�g���̊e�v�f���m�̋t�����Z�o���܂�. (1.0f / vector)
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Reciprocal() const { return SIMD_NAME_SPACE::Vector128Utility::Reciprocal(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �e�v�f�Ƀ}�C�i�X����������̂�Ԃ�.
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Negate() const { return SIMD_NAME_SPACE::Vector128Utility::Negate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 1���m����(�e�v�f�̐�Βl�̘a�ɂ�����x�N�g���̑傫��)���Z�o
		/*----------------------------------------------------------------------*/
		__forceinline float Norm() const { return SIMD_NAME_SPACE::Vector128Utility::NormVector4(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : 2���m����(�e�v�f��2��a�����[�g��������Ƃ��̃x�N�g���̑傫��)���Z�o
		/*----------------------------------------------------------------------*/
		__forceinline float NormSquared() const { return SIMD_NAME_SPACE::Vector128Utility::NormSquaredVector4(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : ���ς��Z�o 
		/*----------------------------------------------------------------------*/
		__forceinline float Dot(const Vector4f& right) const { return SIMD_NAME_SPACE::Vector128Utility::DotVector4(_vector, right._vector); }

		/*----------------------------------------------------------------------
		*  @brief : �O�ς��Z�o second, third�ɐ����ȃx�N�g����Ԃ�
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Cross(const Vector4f& second, const Vector4f& third) const { return SIMD_NAME_SPACE::Vector128Utility::CrossVector4(_vector, second._vector, third._vector); }

		/*----------------------------------------------------------------------
		*  @brief : ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Normalize() const { return SIMD_NAME_SPACE::Vector128Utility::NormalizeVector4(_vector); }

		/*----------------------------------------------------------------------
		*  @brief     ���˃x�N�g�����Z�o
		*  @param[in] const Vector128&f : input  (���˃x�N�g��)
		*  @param[in] const Vector128&f : normal (���˂��s���@���x�N�g��)
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Reflect(const Vector4f& normal) const { return SIMD_NAME_SPACE::Vector128Utility::ReflectVector4(_vector, normal._vector); }

		/*----------------------------------------------------------------------
		*  @brief     ���܃x�N�g�����Z�o
		*  @param[in] const Vector4f& : input  (���˃x�N�g��)
		*  @param[in] const Vector4f& : normal (���˂��s���@���x�N�g��)
		*  @param[in] const float    : refractionIndex (���ܗ�)
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Refract(const Vector4f& normal, const float     refractionIndex) noexcept { return SIMD_NAME_SPACE::Vector128Utility::RefractVector4(_vector, normal._vector, refractionIndex); }
		__forceinline Vector4f Refract(const Vector4f& normal, const Vector4f& refractionIndex) noexcept { return SIMD_NAME_SPACE::Vector128Utility::RefractVector4V(_vector, normal._vector, refractionIndex._vector); }

		/*----------------------------------------------------------------------
		*  @brief : ���ɑ傫���l��
		/*----------------------------------------------------------------------*/
		__forceinline bool IsInfinite() const { return SIMD_NAME_SPACE::Vector128Utility::IsInfiniteVector4(_vector); }

		/*----------------------------------------------------------------------
		*  @brief :  �[�����ǂ���
		/*----------------------------------------------------------------------*/
		__forceinline bool IsZero() const { return ZERO == _vector; }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�ɍ�������������̂�Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Sqrt() const { return SIMD_NAME_SPACE::Vector128Utility::Sqrt(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�̋t�������, ����ɍ�������������̂�Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f ReciprocalSqrt() const { return SIMD_NAME_SPACE::Vector128Utility::ReciprocalSqrt(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��sin, cos, tan��Ԃ��܂� [rad]
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Sin() const { return SIMD_NAME_SPACE::Vector128Utility::Sin(_vector); }
		__forceinline Vector4f Cos() const { return SIMD_NAME_SPACE::Vector128Utility::Cos(_vector); }
		__forceinline Vector4f Tan() const { return SIMD_NAME_SPACE::Vector128Utility::Tan(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��arcsin, arccos, arctan��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f ArcSin() const { return SIMD_NAME_SPACE::Vector128Utility::ArcSin(_vector); }
		__forceinline Vector4f ArcCos() const { return SIMD_NAME_SPACE::Vector128Utility::ArcCos(_vector); }
		__forceinline Vector4f ArcTan() const { return SIMD_NAME_SPACE::Vector128Utility::ArcTan(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��sinH, cosH, tanH��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f SinH() const { return SIMD_NAME_SPACE::Vector128Utility::SinH(_vector); }
		__forceinline Vector4f CosH() const { return SIMD_NAME_SPACE::Vector128Utility::CosH(_vector); }
		__forceinline Vector4f TanH() const { return SIMD_NAME_SPACE::Vector128Utility::TanH(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f�ɑ΂���sin������cos���������o���܂�
		/*----------------------------------------------------------------------*/
		__forceinline void SinCos(Vector4f& sin, Vector4f& cos) noexcept
		{
			SIMD_NAME_SPACE::Vector128Utility::SinCos(_vector, &sin._vector, &cos._vector);
		}

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��2^{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Exp2() const { return SIMD_NAME_SPACE::Vector128Utility::Exp2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��10^{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Exp10() const { return SIMD_NAME_SPACE::Vector128Utility::Exp10(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��e^{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f ExpE() const { return SIMD_NAME_SPACE::Vector128Utility::ExpE(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��log2{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Log2() const { return SIMD_NAME_SPACE::Vector128Utility::Log2(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��log10{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Log10() const { return SIMD_NAME_SPACE::Vector128Utility::Log10(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��loge{vector}��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f LogE() const { return SIMD_NAME_SPACE::Vector128Utility::LogE(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��Pow��Ԃ��܂�
		*           base^{power}
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Pow(const Vector4f& power) const { return SIMD_NAME_SPACE::Vector128Utility::Pow(_vector, power._vector); }

		/*----------------------------------------------------------------------
		*  @brief : �ł��߂������Ɋۂ߂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Round() const { return SIMD_NAME_SPACE::Vector128Utility::Round(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �؂�̂� (�w�肵�����l�̒[����؂�̂Ă������Ɋۂ߂�B���������0�̕����ɂ��鎟�̐���
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Truncate() const { return SIMD_NAME_SPACE::Vector128Utility::Truncate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : ���̕����Ɋۂ߂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Floor() const { return SIMD_NAME_SPACE::Vector128Utility::Floor(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �؂�グ ��������Ő��̕����ɂ��鎟�̐����Ɋۂ߂�
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Ceiling() const { return SIMD_NAME_SPACE::Vector128Utility::Ceiling(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : �w��͈͓��Ő��l��ݒ肷��
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Clamp(const Vector4f& min, const Vector4f& max) const { return SIMD_NAME_SPACE::Vector128Utility::Clamp(_vector, min._vector, max._vector); }

		/*----------------------------------------------------------------------
		*  @brief : 1�ȏ�ɂ͍s���Ȃ��悤�ɂ���
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Saturate() const { return SIMD_NAME_SPACE::Vector128Utility::Saturate(_vector); }

		/*----------------------------------------------------------------------
		*  @brief : ��Βl
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f Abs() noexcept { return SIMD_NAME_SPACE::Vector128Utility::Abs(_vector); }

		#pragma endregion Math
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor
		Vector4f() { _vector = SIMD_NAME_SPACE::Vector128Utility::Zero(); }

		// @brief : Float�^�Ő�������R���X�g���N�^
		Vector4f(const gu::float32 x, const gu::float32 y, const gu::float32 z, const gu::float32 w) 
		{
			_vector = SIMD_NAME_SPACE::Vector128Utility::Set(x, y, z, w);
		}

		// @brief : Vector128�𒼐ڑ������R���X�g���N�^
		Vector4f(const VECTOR128 vector) : _vector(vector) {};

		// @brief : Float4�^�Ő�������R���X�g���N�^
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
	*  @brief : ���ς��Z�o (������, �Ԃ�l��float�ł͂Ȃ�Vector128�^�ŕԂ���܂��̂�, ��XGetX�ȂǂőΉ����Ă�������.)
	/*----------------------------------------------------------------------*/
	__forceinline float Dot(const Vector4f& left, const Vector4f& right) noexcept 
	{ 
		return SIMD_NAME_SPACE::Vector128Utility::DotVector4(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : �O�ς��Z�o first, second, third�ɐ����ȃx�N�g����Ԃ�
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Cross(const Vector4f& first, const Vector4f& second, const Vector4f& third) noexcept 
	{ 
		return SIMD_NAME_SPACE::Vector128Utility::CrossVector4(first, second, third);
	}

	/*----------------------------------------------------------------------
	*  @brief : ���̓x�N�g���̒P�ʃx�N�g����Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Normalize(const Vector4f& vector) noexcept 
	{ 
		return SIMD_NAME_SPACE::Vector128Utility::NormalizeVector4(vector); 
	}

	/*----------------------------------------------------------------------
	*  @brief     ���˃x�N�g�����Z�o
	*  @param[in] const Vector4f& : input  (���˃x�N�g��)
	*  @param[in] const Vector4f& : normal (���˂��s���@���x�N�g��)
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Reflect(const Vector4f& input, const Vector4f& normal) noexcept 
	{ 
		return SIMD_NAME_SPACE::Vector128Utility::ReflectVector4(input, normal); 
	}

	/*----------------------------------------------------------------------
	*  @brief     ���܃x�N�g�����Z�o
	*  @param[in] const Vector4f& : input  (���˃x�N�g��)
	*  @param[in] const Vector4f& : normal (���˂��s���@���x�N�g��)
	*  @param[in] const float    : refractionIndex (���ܗ�)
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
	*  @brief : ���ɑ傫���l��
	/*----------------------------------------------------------------------*/
	__forceinline bool IsInfinite(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::IsInfiniteVector4(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f�ɍ�������������̂�Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Sqrt(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Sqrt(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f�̋t�������, ����ɍ�������������̂�Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f ReciprocalSqrt(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ReciprocalSqrt(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��sin, cos, tan��Ԃ��܂� [rad]
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
	*  @brief : �S�Ă̗v�f��arcsin, arccos, arctan��Ԃ��܂�
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
	*  @brief : �S�Ă̗v�f��sinH, cosH, tanH��Ԃ��܂�
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
	*  @brief : �S�Ă̗v�f��2^{vector}��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Exp2(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Exp2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��10^{vector}��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Exp10(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Exp10(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��e^{vector}��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f ExpE(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::ExpE(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��log2{vector}��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Log2(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Log2(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��log10{vector}��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Log10(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Log10(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��loge{vector}��Ԃ��܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f LogE(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::LogE(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f��Pow��Ԃ��܂�
	*           base^{power}
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Pow(const Vector4f& base, const Vector4f& power) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Pow(base, power);
	}

	/*----------------------------------------------------------------------
	*  @brief      ���`��Ԃ�Ԃ��܂�
	*  @param[in]  t = 0�̎��̒l
	   @param[in]  t = 1�̎��̒l
	   @param[in]  t : ���`��Ԃ̊���
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
	*  @brief      �G���~�[�g��Ԃ�Ԃ��܂�
	*  @param[in]  �n�_�̈ʒu
	   @param[in]  �n�_�ɂ�����X�v���C���̐ڐ���\���x�N�g��
	   @param[in]  �I�_�̈ʒu
	   @param[in]  �I�_�ɂ�����X�v���C���̐ڐ���\���x�N�g��
	   @param[in]  t : �G���~�[�g��Ԃ̊���
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
	*  @brief      CatMull-Rom�X�v���C����Ԃ̌��ʂ�Ԃ��܂��B4�̐���_��S�Ēʂ�X�v���C�����
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
	*  @brief      �O�p�`�̏d�S�d�݈ʒu���Z�o
	*              (f>=0 && g>=0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�͎O�p�`�� Position0>Position1>Position2 �̓����ɂ���܂�>�B
				   (f==0 && g>=0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�� Position0>Position2 �s�ڂɂ���܂�>�B
				   (f>=0 && g==0 && 1-f-g>=0) �̏ꍇ�A�|�C���g�� Position0>Position1> �s�ڂɂ���܂��B
				   (f>=0 && g>=0 && 1-f-g==0) �̏ꍇ�A�|�C���g�� Position1>Position2 �s�ڂɂ���܂�>�B
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
	*  @brief : �S�Ă̗v�f�ɂ��čŏ��l�ƂȂ����I�����܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Min(const Vector4f& left, const Vector4f& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Min(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : �S�Ă̗v�f�ɂ��čő�l�ƂȂ����I�����܂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Max(const Vector4f& left, const Vector4f& right) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Max(left, right);
	}

	/*----------------------------------------------------------------------
	*  @brief : �ł��߂������Ɋۂ߂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Round(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Round(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �؂�̂� (�w�肵�����l�̒[����؂�̂Ă������Ɋۂ߂�B���������0�̕����ɂ��鎟�̐���
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Truncate(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Truncate(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : ���̕����Ɋۂ߂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Floor(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Floor(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �؂�グ ��������Ő��̕����ɂ��鎟�̐����Ɋۂ߂�
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Ceiling(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Ceiling(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : �w��͈͓��Ő��l��ݒ肷��
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Clamp(const Vector4f& vector, const Vector4f& min, const Vector4f& max) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Clamp(vector, min, max);
	}

	/*----------------------------------------------------------------------
	*  @brief : 1�ȏ�ɂ͍s���Ȃ��悤�ɂ���
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Saturate(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Saturate(vector);
	}

	/*----------------------------------------------------------------------
	*  @brief : ��Βl
	/*----------------------------------------------------------------------*/
	__forceinline Vector4f Abs(const Vector4f& vector) noexcept
	{
		return SIMD_NAME_SPACE::Vector128Utility::Abs(vector);
	}
}
#endif