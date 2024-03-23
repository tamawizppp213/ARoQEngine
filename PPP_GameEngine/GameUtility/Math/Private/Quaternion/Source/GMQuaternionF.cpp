//////////////////////////////////////////////////////////////////////////////////
///             @file   GMQuaternionF.cpp
///             @brief  temp
///             @author toide
///             @date   2024/03/23 12:39:28
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/GMQuaternionF.hpp"
#include "../../Vector/Include/GMVector3f.hpp"
#include "../../Vector/Include/GMVector4f.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gm;

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 
QuaternionF::QuaternionF(const Vector3f& axis, const float radian)
{
	_quaternion = SIMD_NAME_SPACE::Quaternion128Utility::RotationAxis(axis.GetSimdVector(), radian);
}

QuaternionF::QuaternionF(const Vector3f& rollPitchYaw)
{
	_quaternion = SIMD_NAME_SPACE::Quaternion128Utility::RotationRollPitchYaw(rollPitchYaw);
}

QuaternionF::QuaternionF(const Vector4f& vector)
{
	_quaternion = vector.GetSimdVector();
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
float QuaternionF::Angle() const
{
	return 2.0f * acosf(Vector4f(_quaternion).GetW());
}

__forceinline QuaternionF gm::RotationRollPitchYaw(const Vector3f& rollPitchYaw) noexcept
{
	return SIMD_NAME_SPACE::Quaternion128Utility::RotationRollPitchYaw(rollPitchYaw);
}

/*----------------------------------------------------------------------
*  @brief : ある法線ベクトル軸を中心に回転を行います
/*----------------------------------------------------------------------*/
__forceinline QuaternionF gm::RotationNormal(const Vector3f& normalAxis, const float angle) noexcept
{
	return SIMD_NAME_SPACE::Quaternion128Utility::RotationNormal(normalAxis, angle);
}

/*----------------------------------------------------------------------
*  @brief : ある軸を中心に回転を行います
/*----------------------------------------------------------------------*/
__forceinline QuaternionF gm::RotationAxis(const Vector3f& axis, const float angle) noexcept
{
	return SIMD_NAME_SPACE::Quaternion128Utility::RotationAxis(axis, angle);
}

#pragma endregion Main Function