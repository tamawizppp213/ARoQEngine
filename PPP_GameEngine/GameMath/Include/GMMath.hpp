//////////////////////////////////////////////////////////////////////////////////
///             @file   GMMath.hpp
///             @brief  Game Math ()
///             @author Toide Yutaro
///             @date   2021_05_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_MATH_HPP
#define GM_MATH_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMScalar.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{

	INLINE float Sqrt(float s)                    { return Sqrt     (Scalar(s)); }
	INLINE float Recip(float s)                   { return Recip    (Scalar(s)); }
	INLINE float RecipSqrt(float s)               { return RecipSqrt(Scalar(s)); }
	INLINE float Floor(float s)                   { return Floor    (Scalar(s)); }
	INLINE float Ceiling(float s)                 { return Ceiling  (Scalar(s)); }
	INLINE float Round(float s)                   { return Round    (Scalar(s)); }
	INLINE float Abs(float s)                     { return s < 0.0f ? -s : s; }
	INLINE float Exp(float s)                     { return Exp      (Scalar(s)); }
	INLINE float Pow(float b, float e)            { return Pow      (Scalar(b), Scalar(e)); }
	INLINE float Log(float s)                     { return Log      (Scalar(s)); }
	INLINE float Sin(float s)                     { return Sin      (Scalar(s)); }
	INLINE float Cos(float s)                     { return Cos      (Scalar(s)); }
	INLINE float Tan(float s)                     { return Tan      (Scalar(s)); }
	INLINE float ASin(float s)                    { return ASin     (Scalar(s)); }
	INLINE float ACos(float s)                    { return ACos     (Scalar(s)); }
	INLINE float ATan(float s)                    { return ATan     (Scalar(s)); }
	INLINE float ATan2(float y, float x)          { return ATan2    (Scalar(y), Scalar(x)); }
	INLINE float Lerp(float a, float b, float t)  { return a + (b - a) * t; }
	INLINE float Max(float a, float b)            { return a > b ? a : b; }
	INLINE float Min(float a, float b)            { return a < b ? a : b; }
	INLINE float Clamp(float v, float a, float b) { return Min(Max(v, a), b); }
}


#endif