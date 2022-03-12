//////////////////////////////////////////////////////////////////////////////////
///             @file   GMScalar.hpp
///             @brief  Scalar Vector
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
#ifndef GM_SCALAR_HPP
#define GM_SCALAR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMVectorUtility.hpp"
#include "GMSimdFunctions.inl"
#pragma warning(disable: 26812  26495 4514)

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	struct Scalar
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		INLINE Scalar() { _vector = vector::SplatOne(); }
        INLINE Scalar( const Scalar& scalar )               { _vector = scalar; }
        INLINE Scalar( float f )                            { _vector = DirectX::XMVectorReplicate(f); }
        INLINE explicit Scalar( DirectX::FXMVECTOR vector ) { _vector = vector; }
        INLINE explicit Scalar( EZeroTag )                  { _vector = vector::SplatZero(); }
        INLINE explicit Scalar( EIdentityTag )              { _vector = vector::SplatOne(); }

        INLINE operator DirectX::XMVECTOR() const { return _vector; }
        INLINE operator float() const { return DirectX::XMVectorGetX(_vector); }


	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		DirectX::XMVECTOR _vector;
	};

	INLINE Scalar operator- (Scalar s) { return Scalar(DirectX::XMVectorNegate(s)); }
	INLINE Scalar operator+ (Scalar s1, Scalar s2) { return Scalar(DirectX::XMVectorAdd(s1, s2)); }
	INLINE Scalar operator- (Scalar s1, Scalar s2) { return Scalar(DirectX::XMVectorSubtract(s1, s2)); }
	INLINE Scalar operator* (Scalar s1, Scalar s2) { return Scalar(DirectX::XMVectorMultiply(s1, s2)); }
	INLINE Scalar operator/ (Scalar s1, Scalar s2) { return Scalar(DirectX::XMVectorDivide(s1, s2)); }
	INLINE Scalar operator+ (Scalar s1, float s2) { return s1 + Scalar(s2); }
	INLINE Scalar operator- (Scalar s1, float s2) { return s1 - Scalar(s2); }
	INLINE Scalar operator* (Scalar s1, float s2) { return s1 * Scalar(s2); }
	INLINE Scalar operator/ (Scalar s1, float s2) { return s1 / Scalar(s2); }
	INLINE Scalar operator+ (float s1, Scalar s2) { return Scalar(s1) + s2; }
	INLINE Scalar operator- (float s1, Scalar s2) { return Scalar(s1) - s2; }
	INLINE Scalar operator* (float s1, Scalar s2) { return Scalar(s1) * s2; }
	INLINE Scalar operator/ (float s1, Scalar s2) { return Scalar(s1) / s2; }

	INLINE bool operator<  (Scalar lhs, float rhs) { return (float)lhs < rhs; }
	INLINE bool operator<= (Scalar lhs, float rhs) { return (float)lhs <= rhs; }
	INLINE bool operator>  (Scalar lhs, float rhs) { return (float)lhs > rhs; }
	INLINE bool operator>= (Scalar lhs, float rhs) { return (float)lhs >= rhs; }
	INLINE bool operator== (Scalar lhs, float rhs) { return (float)lhs == rhs; }
	INLINE bool operator<  (float lhs, Scalar rhs) { return lhs < (float)rhs; }
	INLINE bool operator<= (float lhs, Scalar rhs) { return lhs <= (float)rhs; }
	INLINE bool operator>  (float lhs, Scalar rhs) { return lhs > (float)rhs; }
	INLINE bool operator>= (float lhs, Scalar rhs) { return lhs >= (float)rhs; }
	INLINE bool operator== (float lhs, Scalar rhs) { return lhs == (float)rhs; }
	
	CREATE_SIMD_FUNCTIONS(Scalar);
	INLINE Scalar NormalizeAngle(float angleInRadians)
	{
		angleInRadians = fmodf(angleInRadians, GM_2PI);
		if (angleInRadians < -GM_PI)
		{
			return angleInRadians + GM_2PI;
		}
		else if (angleInRadians > GM_PI)
		{
			return angleInRadians - GM_2PI;
		}
		else
		{
			return angleInRadians;
		}
	}
	INLINE bool FuzzyZero(const Scalar& v) { return Abs(v) < FLT_EPSILON; }
}
#endif