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
		template<typename T>INLINE static T        CubicInterpolate(T const& startPoint, T const& controlPoint1, T const& controlPoint2, T const& endPoint, float t);
		template<typename T>INLINE static T        CubicTangent    (T const& startPoint, T const& controlPoint1, T const& controlPoint2, T const& endPoint, float t);
		template<         > INLINE static Vector2  CubicInterpolate(Vector2 const& startPoint, Vector2 const& controlPoint1, Vector2 const& controlPoint2, Vector2 const& endPoint, float t);
		template<         > INLINE static Vector2  CubicTangent    (Vector2 const& startPoint, Vector2 const& controlPoint1, Vector2 const& controlPoint2, Vector2 const& endPoint, float t);
		template<         > INLINE static Vector3  CubicInterpolate(Vector3 const& startPoint, Vector3 const& controlPoint1, Vector3 const& controlPoint2, Vector3 const& endPoint, float t);
		template<         > INLINE static Vector3  CubicTangent    (Vector3 const& startPoint, Vector3 const& controlPoint1, Vector3 const& controlPoint2, Vector3 const& endPoint, float t);
		template<         > INLINE static Vector4  CubicInterpolate(Vector4 const& startPoint, Vector4 const& controlPoint1, Vector4 const& controlPoint2, Vector4 const& endPoint, float t);
		template<         > INLINE static Vector4  CubicTangent    (Vector4 const& startPoint, Vector4 const& controlPoint1, Vector4 const& controlPoint2, Vector4 const& endPoint, float t);
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

	template<>Vector2 Bezier::CubicInterpolate(Vector2 const& startPoint, Vector2 const& controlPoint1, Vector2 const& controlPoint2, Vector2 const& endPoint, float t)
	{
		Vector2 T0 = ReplicateVector2((1 - t) * (1 - t) * (1 - t));
		Vector2 T1 = ReplicateVector2(3 * t * (1 - t) * (1 - t));
		Vector2 T2 = ReplicateVector2(3 * t * t * (1 - t));
		Vector2 T3 = ReplicateVector2(t * t * t);

		Vector2 Result = startPoint * T0;
		Result = controlPoint1 * T1 + Result;
		Result = controlPoint2 * T2 + Result;
		Result = endPoint * T3 + Result;

		return Result;
	}

	template<>Vector3 Bezier::CubicInterpolate(Vector3 const& startPoint, Vector3 const& controlPoint1, Vector3 const& controlPoint2, Vector3 const& endPoint, float t)
	{
		Vector3 T0 = ReplicateVector3((1 - t) * (1 - t) * (1 - t));
		Vector3 T1 = ReplicateVector3(3 * t * (1 - t) * (1 - t));
		Vector3 T2 = ReplicateVector3(3 * t * t * (1 - t));
		Vector3 T3 = ReplicateVector3(t * t * t);

		Vector3 Result = startPoint * T0;
		Result = controlPoint1 * T1 + Result;
		Result = controlPoint2 * T2 + Result;
		Result = endPoint * T3 + Result;

		return Result;
	}

	template<>Vector4 Bezier::CubicInterpolate(Vector4 const& startPoint, Vector4 const& controlPoint1, Vector4 const& controlPoint2, Vector4 const& endPoint, float t)
	{
		Vector4 T0 = ReplicateVector4((1 - t) * (1 - t) * (1 - t));
		Vector4 T1 = ReplicateVector4(3 * t * (1 - t) * (1 - t));
		Vector4 T2 = ReplicateVector4(3 * t * t * (1 - t));
		Vector4 T3 = ReplicateVector4(t * t * t);

		Vector4 Result = startPoint * T0;
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

	template<> Vector2 Bezier::CubicTangent(Vector2 const& startPoint, Vector2 const& controlPoint1, Vector2 const& controlPoint2, Vector2 const& endPoint, float t) noexcept
	{
		Vector2 T0 = ReplicateVector2(-1 + 2 * t - t * t);
		Vector2 T1 = ReplicateVector2(1 - 4 * t + 3 * t * t);
		Vector2 T2 = ReplicateVector2(2 * t - 3 * t * t);
		Vector2 T3 = ReplicateVector2(t * t);

		Vector2 Result = startPoint * T0;
		Result = controlPoint1 * T1 + Result;
		Result = controlPoint2 * T2 + Result;
		Result = endPoint * T3 + Result;

		return Result;
	}

	template<> Vector3 Bezier::CubicTangent(Vector3 const& startPoint, Vector3 const& controlPoint1, Vector3 const& controlPoint2, Vector3 const& endPoint, float t) noexcept
	{
		Vector3 T0 = ReplicateVector3(-1 + 2 * t - t * t);
		Vector3 T1 = ReplicateVector3(1 - 4 * t + 3 * t * t);
		Vector3 T2 = ReplicateVector3(2 * t - 3 * t * t);
		Vector3 T3 = ReplicateVector3(t * t);

		Vector3 Result = startPoint * T0;
		Result = controlPoint1 * T1 + Result;
		Result = controlPoint2 * T2 + Result;
		Result = endPoint * T3 + Result;

		return Result;
	}
	template<> Vector4 Bezier::CubicTangent(Vector4 const& startPoint, Vector4 const& controlPoint1, Vector4 const& controlPoint2, Vector4 const& endPoint, float t) noexcept
	{
		Vector4 T0 = ReplicateVector4(-1 + 2 * t - t * t);
		Vector4 T1 = ReplicateVector4(1 - 4 * t + 3 * t * t);
		Vector4 T2 = ReplicateVector4(2 * t - 3 * t * t);
		Vector4 T3 = ReplicateVector4(t * t);

		Vector4 Result = startPoint * T0;
		Result = controlPoint1 * T1 + Result;
		Result = controlPoint2 * T2 + Result;
		Result = endPoint * T3 + Result;

		return Result;
	}
}
#endif