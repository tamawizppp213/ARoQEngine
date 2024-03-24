//////////////////////////////////////////////////////////////////////////////////
///             @file   GMInterpolation.hpp
///             @brief  Game Math Interpolation (Bezier)
///             @author Toide Yutaro
///             @date   2021_05_30
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_INTERPOLATION_HPP
#define GM_INTERPOLATION_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMVector.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         NameSpace
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	class Bezier
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		template<typename T>__forceinline static T        CubicInterpolate(T const& startPoint, T const& controlPoint1, T const& controlPoint2, T const& endPoint, float t);
		template<typename T>__forceinline static T        CubicTangent    (T const& startPoint, T const& controlPoint1, T const& controlPoint2, T const& endPoint, float t);
		template<         > __forceinline static Vector2f  CubicInterpolate(Vector2f const& startPoint, Vector2f const& controlPoint1, Vector2f const& controlPoint2, Vector2f const& endPoint, float t);
		template<         > __forceinline static Vector2f  CubicTangent    (Vector2f const& startPoint, Vector2f const& controlPoint1, Vector2f const& controlPoint2, Vector2f const& endPoint, float t);
		template<         > __forceinline static Vector3f  CubicInterpolate(Vector3f const& startPoint, Vector3f const& controlPoint1, Vector3f const& controlPoint2, Vector3f const& endPoint, float t);
		template<         > __forceinline static Vector3f  CubicTangent    (Vector3f const& startPoint, Vector3f const& controlPoint1, Vector3f const& controlPoint2, Vector3f const& endPoint, float t);
		template<         > __forceinline static Vector4f  CubicInterpolate(Vector4f const& startPoint, Vector4f const& controlPoint1, Vector4f const& controlPoint2, Vector4f const& endPoint, float t);
		template<         > __forceinline static Vector4f  CubicTangent    (Vector4f const& startPoint, Vector4f const& controlPoint1, Vector4f const& controlPoint2, Vector4f const& endPoint, float t);
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
	};
	
	template<typename T> T Bezier::CubicInterpolate(T const& startPoint, T const& controlPoint1, T const& controlPoint2, T const& endPoint, float t)
	{
		return startPoint * (1 - t) * (1 - t) * (1 - t) +
				controlPoint1 * 3 * t * (1 - t) * (1 - t) +
				controlPoint2 * 3 * t * t * (1 - t) +
				endPoint * t * t * t;
	}

	template<>Vector2f Bezier::CubicInterpolate(Vector2f const& startPoint, Vector2f const& controlPoint1, Vector2f const& controlPoint2, Vector2f const& endPoint, float t)
	{
		Vector2f T0 = Vector2f((1 - t) * (1 - t) * (1 - t));
		Vector2f T1 = Vector2f(3 * t * (1 - t) * (1 - t));
		Vector2f T2 = Vector2f(3 * t * t * (1 - t));
		Vector2f T3 = Vector2f(t * t * t);

		Vector2f Result = startPoint * T0;
		Result = controlPoint1 * T1 + Result;
		Result = controlPoint2 * T2 + Result;
		Result = endPoint * T3 + Result;

		return Result;
	}

	template<>Vector3f Bezier::CubicInterpolate(Vector3f const& startPoint, Vector3f const& controlPoint1, Vector3f const& controlPoint2, Vector3f const& endPoint, float t)
	{
		Vector3f T0 = Vector3f((1 - t) * (1 - t) * (1 - t));
		Vector3f T1 = Vector3f(3 * t * (1 - t) * (1 - t));
		Vector3f T2 = Vector3f(3 * t * t * (1 - t));
		Vector3f T3 = Vector3f(t * t * t);

		Vector3f Result = startPoint * T0;
		Result = controlPoint1 * T1 + Result;
		Result = controlPoint2 * T2 + Result;
		Result = endPoint * T3 + Result;

		return Result;
	}

	template<>Vector4f Bezier::CubicInterpolate(Vector4f const& startPoint, Vector4f const& controlPoint1, Vector4f const& controlPoint2, Vector4f const& endPoint, float t)
	{
		Vector4f T0 = Vector4f((1 - t) * (1 - t) * (1 - t));
		Vector4f T1 = Vector4f(3 * t * (1 - t) * (1 - t));
		Vector4f T2 = Vector4f(3 * t * t * (1 - t));
		Vector4f T3 = Vector4f(t * t * t);

		Vector4f Result = startPoint * T0;
		Result = controlPoint1 * T1 + Result;
		Result = controlPoint2 * T2 + Result;
		Result = endPoint * T3 + Result;

		return Result;
	}

	template<typename T> T Bezier::CubicTangent(T const& startPoint, T const& controlPoint1, T const& controlPoint2, T const& endPoint, float t) noexcept
	{
		return startPoint * (-1 + 2 * t - t * t) +
			controlPoint1 * (1 - 4 * t + 3 * t * t) +
			controlPoint2 * (2 * t - 3 * t * t) +
			endPoint      * (t * t);
	}

	template<> Vector2f Bezier::CubicTangent(Vector2f const& startPoint, Vector2f const& controlPoint1, Vector2f const& controlPoint2, Vector2f const& endPoint, float t) noexcept
	{
		Vector2f T0 = Vector2f(-1 + 2 * t - t * t);
		Vector2f T1 = Vector2f(1 - 4 * t + 3 * t * t);
		Vector2f T2 = Vector2f(2 * t - 3 * t * t);
		Vector2f T3 = Vector2f(t * t);

		Vector2f Result = startPoint * T0;
		Result = controlPoint1 * T1 + Result;
		Result = controlPoint2 * T2 + Result;
		Result = endPoint * T3 + Result;

		return Result;
	}

	template<> Vector3f Bezier::CubicTangent(Vector3f const& startPoint, Vector3f const& controlPoint1, Vector3f const& controlPoint2, Vector3f const& endPoint, float t) noexcept
	{
		Vector3f T0 = Vector3f(-1 + 2 * t - t * t);
		Vector3f T1 = Vector3f(1 - 4 * t + 3 * t * t);
		Vector3f T2 = Vector3f(2 * t - 3 * t * t);
		Vector3f T3 = Vector3f(t * t);

		Vector3f Result = startPoint * T0;
		Result = controlPoint1 * T1 + Result;
		Result = controlPoint2 * T2 + Result;
		Result = endPoint * T3 + Result;

		return Result;
	}
	template<> Vector4f Bezier::CubicTangent(Vector4f const& startPoint, Vector4f const& controlPoint1, Vector4f const& controlPoint2, Vector4f const& endPoint, float t) noexcept
	{
		Vector4f T0 = Vector4f(-1 + 2 * t - t * t);
		Vector4f T1 = Vector4f(1 - 4 * t + 3 * t * t);
		Vector4f T2 = Vector4f(2 * t - 3 * t * t);
		Vector4f T3 = Vector4f(t * t);

		Vector4f Result = startPoint * T0;
		Result = controlPoint1 * T1 + Result;
		Result = controlPoint2 * T2 + Result;
		Result = endPoint * T3 + Result;

		return Result;
	}
}
#endif