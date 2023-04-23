//////////////////////////////////////////////////////////////////////////////////
///             @file   GMQuaternion.hpp
///             @brief  Quaternion
///             @author James Stanard  // Partially modified by Toide.
///             @date   2021_05_09 (modified)
//////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
// Developed by Minigraph
//
// Author:  James Stanard 
//
#pragma once
#ifndef GM_QUATERNION_HPP
#define GM_QUATERNION_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMVector.hpp"

#pragma warning (disable: 26812 26495)

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	/****************************************************************************
	*				  			Quaternion
	*************************************************************************//**
	*  @class     Quaternion
	*  @brief     Quaternion class 
	*****************************************************************************/
	class Quaternion
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		INLINE Quaternion Normalize() { DirectX::XMVECTOR q = _vector; return Quaternion(DirectX::XMQuaternionNormalize(q)); }
		INLINE Scalar     GetAngle()  { return Scalar(2.0f)* ACos(DirectX::XMVectorGetW(_vector)); }
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		INLINE Quaternion operator~ (void) const { return Quaternion(DirectX::XMQuaternionConjugate(_vector)); }
		INLINE Quaternion operator- (void) const { return Quaternion(DirectX::XMVectorNegate(_vector)); }

		INLINE Quaternion operator* (Quaternion rhs) const { return Quaternion(DirectX::XMQuaternionMultiply(rhs, _vector)); } // from right to left 
		INLINE Vector3 operator*    (Vector3 rhs)    const { return Vector3(DirectX::XMVector3Rotate(rhs, _vector)); }

		INLINE Quaternion& operator=  (const Quaternion& rhs) { _vector = rhs; return *this; }
		INLINE Quaternion& operator*= (Quaternion rhs) { *this = *this * rhs; return *this; }
		INLINE Quaternion& operator=  (Quaternion&&) = default;

		INLINE bool operator == (const Quaternion& V) const noexcept { DirectX::XMQuaternionEqual(*this, V); };
		INLINE bool operator == (const Float4& V) const noexcept { DirectX::XMQuaternionEqual(*this, V); };
		INLINE bool operator != (const Float4& V) const noexcept { DirectX::XMQuaternionNotEqual(*this, V); };

		INLINE Float4 ToFloat4()
		{ 
			Float4 value;
			DirectX::XMStoreFloat4(&value, _vector);
			return value;
		}
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		INLINE Quaternion()                                                 { _vector = DirectX::XMQuaternionIdentity(); }
		INLINE Quaternion(const Vector3& axis, const Scalar& radian)         { _vector = DirectX::XMQuaternionRotationAxis(axis, radian); }
		INLINE Quaternion(float pitch, float yaw, float roll)               { _vector = DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll); }
		INLINE Quaternion(const Vector3& PitchYawRoll)                      { _vector = DirectX::XMQuaternionRotationRollPitchYawFromVector(PitchYawRoll); }
		INLINE Quaternion(const Vector4& vector)                            { _vector = vector; }
		INLINE explicit Quaternion(const DirectX::XMMATRIX& rotationMatrix) { _vector = DirectX::XMQuaternionRotationMatrix(rotationMatrix); }
		INLINE explicit Quaternion(DirectX::FXMVECTOR vec)                  { _vector = vec; }
		INLINE explicit Quaternion(EIdentityTag)                            { _vector = DirectX::XMQuaternionIdentity(); }
		INLINE operator DirectX::XMVECTOR() const { return _vector; }
		INLINE Quaternion(const Quaternion&) = default;
		INLINE Quaternion(Quaternion&&)      = default;

	protected:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		DirectX::XMVECTOR _vector;
	};
	
	INLINE void AxisAngle(const Quaternion& q, float* rotate, Vector3& axis) { DirectX::XMVECTOR v = axis; DirectX::XMQuaternionToAxisAngle(&v, rotate, q); axis = Vector3(v); }
	INLINE void AxisAngle(const Quaternion& q, float* rotate, DirectX::XMVECTOR* axis) { DirectX::XMQuaternionToAxisAngle(axis,rotate,q); }
	INLINE Quaternion Normalize(const Quaternion& q)                           { return Quaternion(DirectX::XMQuaternionNormalize(q)); }
	INLINE Quaternion Squad(const Quaternion& a, const Quaternion& b, const Quaternion& c, const Quaternion& d, float t) { return Quaternion(DirectX::XMQuaternionSquad(a, b, c, d, t)); }
	INLINE Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t) { return Quaternion(DirectX::XMQuaternionSlerp(a, b, t)); }
	INLINE Quaternion Lerp (const Quaternion& a, const Quaternion& b, float t) { return Quaternion(DirectX::XMVectorLerp(a, b, t)); }
	INLINE Quaternion Norm       (const Quaternion& q)                         { return Quaternion(DirectX::XMQuaternionLength(q)); }
	INLINE Quaternion NormSquared(const Quaternion& q)                         { return Quaternion(DirectX::XMQuaternionLengthSq(q)); }
	INLINE Quaternion Conjugate  (const Quaternion& q)                         { return Quaternion(DirectX::XMQuaternionConjugate(q)); }
	INLINE Quaternion Inverse    (const Quaternion& q)                         { return Quaternion(DirectX::XMQuaternionInverse(q)); }
	INLINE Quaternion BaryCentric(const Quaternion& v1, const Quaternion& v2, const Quaternion& v3, float wf, float wg) { return Quaternion(DirectX::XMQuaternionBaryCentric(v1, v2, v3, wf, wg)); }
	INLINE Scalar Dot(const Quaternion& q1, const Quaternion& q2) { return Scalar(DirectX::XMQuaternionDot(q1, q2)); }
	INLINE Vector3    QuaternionRotate(const Quaternion& rotation, const Vector3& v)
	{
		Quaternion q = rotation * v;
		q *= Inverse(rotation);
		Float4 result = q.ToFloat4();
		return Vector3(result.x, result.y, result.z);
	}
	INLINE Quaternion ShortestArcQuaternion(const Vector3& v0, const Vector3& v1)
	{
		Vector3 c = Cross(v0, v1);
		float   d = Dot(v0, v1);

		if (d < -1.0f + FLT_EPSILON)
		{
			Vector3 n, unused;
			PlaneSpace(v0, n, unused);
			return Quaternion(Vector4(n.GetX(), n.GetY(), n.GetZ(), 0.0f));
		}

		float s = Sqrt((1.0f + d) * 2.0f);
		float rs = 1.0f / s;

		return Quaternion(Vector4(c.GetX() * rs, c.GetY() * rs, c.GetZ() * rs, s * 0.5f));
	}
	//INLINE Quaternion Inverse(const Quaternion& q) { return Quaternion(-q); }
}
#endif
