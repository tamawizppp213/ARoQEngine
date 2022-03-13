//////////////////////////////////////////////////////////////////////////////////
///             @file   GMMatrix.hpp
///             @brief  Matrix
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
#ifndef GM_SIMD_FUNCTIONS_INL
#define GM_SIMD_FUNCTIONS_INL

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMVectorUtility.hpp"
#include <DirectXMath.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
#define CREATE_SIMD_FUNCTIONS( TYPE ) \
	INLINE TYPE Sqrt( const TYPE& s )                                { return TYPE(DirectX::XMVectorSqrt(s)); } \
	INLINE TYPE Recip( const TYPE& s )                               { return TYPE(DirectX::XMVectorReciprocal(s)); } \
	INLINE TYPE RecipSqrt( const TYPE& s )                           { return TYPE(DirectX::XMVectorReciprocalSqrt(s)); } \
	INLINE TYPE Floor( const TYPE& s )                               { return TYPE(DirectX::XMVectorFloor(s)); } \
	INLINE TYPE Ceiling( const TYPE& s )                             { return TYPE(DirectX::XMVectorCeiling(s)); } \
	INLINE TYPE Round( const TYPE& s )                               { return TYPE(DirectX::XMVectorRound(s)); } \
	INLINE TYPE Abs( const TYPE& s )                                 { return TYPE(DirectX::XMVectorAbs(s)); } \
	INLINE TYPE Exp( const TYPE& s )                                 { return TYPE(DirectX::XMVectorExp(s)); } \
	INLINE TYPE Pow( const TYPE& b, const TYPE& e )                  { return TYPE(DirectX::XMVectorPow(b, e)); } \
	INLINE TYPE Log( const TYPE& s )                                 { return TYPE(DirectX::XMVectorLog(s)); } \
	INLINE TYPE Sin( const TYPE& s )                                 { return TYPE(DirectX::XMVectorSin(s)); } \
	INLINE TYPE Cos( const TYPE& s )                                 { return TYPE(DirectX::XMVectorCos(s)); } \
	INLINE TYPE Tan( const TYPE& s )                                 { return TYPE(DirectX::XMVectorTan(s)); } \
	INLINE TYPE ASin( const TYPE& s )                                { return TYPE(DirectX::XMVectorASin(s)); } \
	INLINE TYPE ACos( const TYPE& s )                                { return TYPE(DirectX::XMVectorACos(s)); } \
	INLINE TYPE ATan( const TYPE& s )                                { return TYPE(DirectX::XMVectorATan(s)); } \
	INLINE TYPE ATan2( const TYPE& y, const TYPE& x )                { return TYPE(DirectX::XMVectorATan2(y, x)); } \
	INLINE TYPE Lerp( const TYPE& a, const TYPE& b, const TYPE& t )  { return TYPE(DirectX::XMVectorLerpV(a, b, t)); } \
    INLINE TYPE Lerp( const TYPE& a, const TYPE& b, float t )        { return TYPE(DirectX::XMVectorLerp(a, b, t)); } \
    INLINE TYPE SmoothStep (const TYPE& a, const TYPE& b, float t)   { t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t); t = t * t * (3.f - 2.f * t); \
	                                                                   return TYPE(DirectX::XMVectorLerp(a, b, t)); }  \
	INLINE TYPE Max( const TYPE& a, const TYPE& b )                  { return TYPE(DirectX::XMVectorMax(a, b)); } \
	INLINE TYPE Min( const TYPE& a, const TYPE& b )                  { return TYPE(DirectX::XMVectorMin(a, b)); } \
	INLINE TYPE Clamp( const TYPE& v, const TYPE& a, const TYPE& b ) { return Min(Max(v, a), b); } \
	INLINE BoolVector operator<  ( const TYPE& lhs, const TYPE& rhs ){ return DirectX::XMVectorLess(lhs, rhs); } \
	INLINE BoolVector operator<= ( const TYPE& lhs, const TYPE& rhs ){ return DirectX::XMVectorLessOrEqual(lhs, rhs); } \
	INLINE BoolVector operator>  ( const TYPE& lhs, const TYPE& rhs ){ return DirectX::XMVectorGreater(lhs, rhs); } \
	INLINE BoolVector operator>= ( const TYPE& lhs, const TYPE& rhs ){ return DirectX::XMVectorGreaterOrEqual(lhs, rhs); } \
	INLINE TYPE Select( const TYPE& lhs, const TYPE& rhs, BoolVector mask ) { return TYPE(DirectX::XMVectorSelect(lhs, rhs, mask)); }\
	INLINE void Swap(TYPE& a, TYPE& b){TYPE temp = a; a=b;b = temp;}
}

//INLINE BoolVector operator== ( const TYPE& lhs, const TYPE& rhs ){ return DirectX::XMVectorEqual(lhs, rhs); } 
#endif
