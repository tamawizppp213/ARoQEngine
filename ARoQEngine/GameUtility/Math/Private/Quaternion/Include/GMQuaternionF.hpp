//////////////////////////////////////////////////////////////////////////////////
///             @file   GMQuaternionf.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/23 11:46:55
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_QUATERNIONF_HPP
#define GM_QUATERNIONF_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Simd/Include/GMSimdMacros.hpp"
#include SIMD_COMPILED_HEADER(GameUtility/Math/Private/Simd/Include, GMSimdQuaternion128)

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
	#define QUATERNION128 gm::simd::non::Quaternion128
#elif PLATFORM_CPU_INSTRUCTION_NEON
	#define QUATERNION128 gm::simd::neon::Quaternion128
#else
	#define QUATERNION128 gm::simd::sse::Quaternion128
#endif
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gm
{
	struct Vector3f;
	struct Vector4f;

	/****************************************************************************
	*				  			   GMQuaternionf
	****************************************************************************/
	/* @class     GMQuaternionf
	*  @brief     temp
	*****************************************************************************/
	struct QuaternionF
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : �P�ʃN�H�[�^�j�I����
		*----------------------------------------------------------------------*/
		__forceinline bool IsIdentity() const 
		{ 
			return SIMD_NAME_SPACE::Quaternion128Utility::IsIdentity(_quaternion);
		}

		/*----------------------------------------------------------------------
		*  @brief : ��]�p���擾���܂�
		*----------------------------------------------------------------------*/
		float Angle() const;

		/*----------------------------------------------------------------------
		*  @brief : ���ς��Z�o���܂�. �ʏ�̃x�N�g�����l, �݂��̃x�N�g���̂Ȃ��p�x�𓱏o�������Ƃ��Ɏg�p���܂�
		*----------------------------------------------------------------------*/
		__forceinline float Dot(const QuaternionF& right) const
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::Dot(_quaternion, right._quaternion);
		}

		/*----------------------------------------------------------------------
		*  @brief : 2���m���� (�S�Ă̗v�f��2��a)���擾���܂�
		*----------------------------------------------------------------------*/
		__forceinline float Length() const
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::Length(_quaternion);
		}

		/*----------------------------------------------------------------------
		*  @brief : 2���m�����ɓ����擾���܂�
		*----------------------------------------------------------------------*/
		__forceinline float LengthSquare() const
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::LengthSquare(_quaternion);
		}

		/*----------------------------------------------------------------------
		*  @brief : �N�H�[�^�j�I���̐��K�����s���܂�
		*----------------------------------------------------------------------*/
		__forceinline QuaternionF Normalize() const
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::Normalize(_quaternion);
		}

		/*----------------------------------------------------------------------
		*  @brief : �����ȃN�H�[�^�j�I����Ԃ��܂�. �����ȃN�H�[�^�j�I���́A�t�����̉�]��\�����܂�.
		*----------------------------------------------------------------------*/
		__forceinline QuaternionF Conjugate() const
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::Conjugate(_quaternion);
		}

		/*----------------------------------------------------------------------
		*  @brief : �N�H�[�^�j�I���̋t�֐���n���܂�
		*----------------------------------------------------------------------*/
		__forceinline QuaternionF Inverse() const
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::Inverse(_quaternion);
		}

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��Log��K�p���܂�
		*----------------------------------------------------------------------*/
		__forceinline QuaternionF SIMD_CALL_CONVENTION Log() const
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::Log(_quaternion);
		}

		/*----------------------------------------------------------------------
		*  @brief : �S�Ă̗v�f��Exp��K�p���܂�
		*----------------------------------------------------------------------*/
		__forceinline QuaternionF SIMD_CALL_CONVENTION Exp() const
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::Exp(_quaternion);
		}

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		#pragma region Operator
		/*----------------------------------------------------------------------
		*  @brief : �|���Z left * right (directXMath�̎g�����Ƃ͈����̏��Ԃ��t�Ȃ̂Œ��� (q1, q2 -> q2 * q1))
		*           �Œ�̉�]���ŉ�]���������ꍇ�� left�ɉ�]���Ɖ�]�x�����̃N�H�[�^�j�I��, �I�u�W�F�N�g�̎p����right�ɐݒ肷��
		*           ex ) worldY�����S��30�x��]�݂����ȏ󋵂ł���� left : worldYQuaternion30 right : objectRotation
		*----------------------------------------------------------------------*/
		__forceinline QuaternionF operator* (const QuaternionF& right)
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::Multiply(_quaternion, right._quaternion);
		}

		// @brief : �|���Z (this = rotateQuaternion * this)
		__forceinline QuaternionF& operator*=(const QuaternionF& rotateQuaternion)
		{
			this->_quaternion = SIMD_NAME_SPACE::Quaternion128Utility::Multiply(rotateQuaternion, _quaternion);
			return *this;
		}

		// @brief : ���҂̃N�H�[�^�j�I������������
		__forceinline bool operator ==(const QuaternionF& right) const noexcept 
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::Equal(_quaternion, right._quaternion);
		}

		// @brief : ���҂̃N�H�[�^�j�I�����������Ȃ���
		__forceinline bool operator !=(const QuaternionF& right) const noexcept
		{
			return SIMD_NAME_SPACE::Quaternion128Utility::NotEqual(_quaternion, right._quaternion);
		}

		__forceinline float& operator[](const gu::uint32 index) noexcept
		{
			return _quaternion.m128_f32[index];
		}

		__forceinline const float& operator[](const gu::uint32 index) const noexcept 
		{
			return _quaternion.m128_f32[index];
		}

		#pragma endregion Operator

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor
		QuaternionF() 
		{
			_quaternion = SIMD_NAME_SPACE::Quaternion128Utility::Identity(); 
		};

		// @brief : ��]���Ɖ�]�ʂŐ�������R���X�g���N�^
		QuaternionF(const Vector3f& axis, const float radian);

		// @brief : Quaternion128�𒼐ڑ������R���X�g���N�^
		QuaternionF(const QUATERNION128 quaternion) : _quaternion(quaternion) {};

		// @brief : roll pitch yaw�����Ƃɉ�]�ʂ𐧌䂷��R���X�g���N�^
		QuaternionF(const float roll, const float pitch, const float yaw) 
		{
			_quaternion = SIMD_NAME_SPACE::Quaternion128Utility::RotationRollPitchYaw(roll, pitch, yaw); 
		}

		// @brief : roll Pitch Yaw, 0�Ƃ����x�N�g���^�Ŏw�肷��R���X�g���N�^
		QuaternionF(const Vector3f& rollPitchYaw);

		// @brief : Vector4�^�Œ��ڃN�H�[�^�j�I���𐶐�����R���X�g���N�^
		QuaternionF(const Vector4f& vector);

		// @brief : Copy constructor
		QuaternionF(const QuaternionF&) = default;
		QuaternionF& operator=(const QuaternionF& right)
		{
			_quaternion = right._quaternion; return *this;
		}

		// @brief : Move constructor
		QuaternionF(QuaternionF&&) = default;
		QuaternionF& operator=(QuaternionF&&) = default;

		operator QUATERNION128() const noexcept
		{
			return SIMD_NAME_SPACE::Vector128Utility::LoadFloat4((float*)this);
		}
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		QUATERNION128 _quaternion;
	};

	/*----------------------------------------------------------------------
	*  @brief : Roll pitch yaw���g���ĉ�]���s���܂�
	*----------------------------------------------------------------------*/
	__forceinline QuaternionF RotationRollPitchYawQuaternion(const float roll, const float pitch, const float yaw) noexcept
	{
		return SIMD_NAME_SPACE::Quaternion128Utility::RotationRollPitchYaw(roll, pitch, yaw);
	}

	__forceinline QuaternionF RotationRollPitchYawQuaternion(const Vector3f& rollPitchYaw) noexcept;

	/*----------------------------------------------------------------------
	*  @brief : ����@���x�N�g�����𒆐S�ɉ�]���s���܂�
	*----------------------------------------------------------------------*/
	__forceinline QuaternionF RotationNormalQuaternion(const Vector3f& normalAxis, const float angle) noexcept;

	/*----------------------------------------------------------------------
	*  @brief : ���鎲�𒆐S�ɉ�]���s���܂�
	*----------------------------------------------------------------------*/
	__forceinline QuaternionF RotationAxisQuaternion(const Vector3f& axis, const float angle) noexcept;

	/*----------------------------------------------------------------------
	*  @brief : �ʏ��4�����x�N�g���̂悤�ɐ��`��Ԃ��|���܂�. Slerp���͐��x���e���ł���, �ȃR�X�g�ł�
	*----------------------------------------------------------------------*/
	__forceinline QuaternionF Lerp(const QuaternionF& start, const QuaternionF& end, const float t) noexcept
	{
		return SIMD_NAME_SPACE::Quaternion128Utility::Lerp(start, end, t);
	}

	/*----------------------------------------------------------------------
	*  @brief : ���ʕ�Ԃł�
	*----------------------------------------------------------------------*/
	__forceinline QuaternionF Slerp(const QuaternionF& start, const QuaternionF& end, const float t) noexcept
	{
		return SIMD_NAME_SPACE::Quaternion128Utility::Slerp(start, end, t);
	}

}

#endif