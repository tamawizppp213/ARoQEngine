//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdMatrix128SSE.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/20 15:36:48
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_MATRIX_128_SSE4_HPP
#define GM_SIMD_MATRIX_128_SSE4_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMSimdMacros.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_CPU_INSTRUCTION_SSE4_1 && !defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
#include "GMSimdQuaternion128SSE4.hpp"
#include "GMSimdMatrix128SSE3.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gm::simd::sse4
{
	/****************************************************************************
	*				  			   GMSimdMatrix128SSE
	*************************************************************************//**
	*  @class     GMSimdMatrix128SSE
	*  @brief     temp
	*****************************************************************************/
	class Matrix128Utility : public gm::simd::sse3::Matrix128Utility
	{
	private:
		using VectorFunction = Vector128Utility;
	public:

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : FocusPositionに視点を向けるための変換行列を作成します(左手系, 右手系)
		/*----------------------------------------------------------------------*/
		__forceinline static sse::Matrix128 SIMD_CALL_CONVENTION LookAtLH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 focusPosition, Vector128Utility::ConstVector128 worldUp) noexcept;
		__forceinline static sse::Matrix128 SIMD_CALL_CONVENTION LookAtRH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 focusPosition, Vector128Utility::ConstVector128 worldUp) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 視点を対象方向に向けるための変換行列を作成します(左手系, 右手系)
		/*----------------------------------------------------------------------*/
		__forceinline static sse::Matrix128 SIMD_CALL_CONVENTION LookToLH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 eyeDirection, Vector128Utility::ConstVector128 worldUp) noexcept;
		__forceinline static sse::Matrix128 SIMD_CALL_CONVENTION LookToRH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 eyeDirection, Vector128Utility::ConstVector128 worldUp) noexcept;

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
	};

#pragma region Implement
	#pragma region Math
	/****************************************************************************
	*                       LookAtLH
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::LookAtLH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 focusPosition, Vector128Utility::ConstVector128 worldUp) noexcept
	*
	*  @brief     FocusPositionに視点を向けるための変換行列を作成します(左手系)
	*
	*  @param[in] Vector128Utility::ConstVector128 eyePosition (カメラの位置)
	*  @param[in] Vector128Utility::ConstVector128 focusPosition (見つめたい対象物体の位置) 
	*  @param[in] Vector128Utility::ConstVector128 worldUp (ワールドの上方向：外積のために使用)
	*
	*  @return 　　Matrix128
	*****************************************************************************/
	inline sse::Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::LookAtLH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 focusPosition, Vector128Utility::ConstVector128 worldUp) noexcept
	{
		return LookToLH(eyePosition, VectorFunction::Subtract(focusPosition, eyePosition), worldUp);
	}

	/****************************************************************************
	*                       LookAtRH
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::LookAtRH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 focusPosition, Vector128Utility::ConstVector128 worldUp) noexcept
	*
	*  @brief     FocusPositionに視点を向けるための変換行列を作成します(右手系)
	*
	*  @param[in] Vector128Utility::ConstVector128 eyePosition (カメラの位置)
	*  @param[in] Vector128Utility::ConstVector128 focusPosition (見つめたい対象物体の位置)
	*  @param[in] Vector128Utility::ConstVector128 worldUp (ワールドの上方向：外積のために使用)
	*
	*  @return 　　Matrix128
	*****************************************************************************/
	inline sse::Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::LookAtRH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 focusPosition, Vector128Utility::ConstVector128 worldUp) noexcept
	{
		return LookToRH(eyePosition, VectorFunction::Subtract(eyePosition, focusPosition), worldUp);
	}

	/****************************************************************************
	*                       LookToLH
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::LookToLH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 eyeDirection, Vector128Utility::ConstVector128 worldUp) noexcept
	*
	*  @brief     視点を対象方向に向けるための変換行列を作成します(左手系)
	*
	*  @param[in] Vector128Utility::ConstVector128 eyePosition (カメラの位置)
	*  @param[in] Vector128Utility::ConstVector128 eyeDirection (カメラを向ける方向)
	*  @param[in] Vector128Utility::ConstVector128 worldUp (ワールドの上方向：外積のために使用)
	*
	*  @return 　　Matrix128
	*****************************************************************************/
	inline sse::Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::LookToLH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 eyeDirection, Vector128Utility::ConstVector128 worldUp) noexcept
	{
		Check(!VectorFunction::EqualVector3(eyeDirection, sse::VECTOR_128F_ZERO));
		Check(!VectorFunction::IsInfiniteVector3(eyeDirection));
		Check(!VectorFunction::EqualVector3(worldUp, sse::VECTOR_128F_ZERO));
		Check(!VectorFunction::IsInfiniteVector3(worldUp));

		sse::Vector128 normalizedForwardDirection = VectorFunction::NormalizeVector3(eyeDirection);
		sse::Vector128 normalizedRightDirection   = VectorFunction::NormalizeVector3(VectorFunction::CrossVector3(worldUp, normalizedForwardDirection));
		sse::Vector128 normalizedUpDirection      = VectorFunction::CrossVector3(normalizedForwardDirection, normalizedRightDirection);

		sse::Vector128 negativeEyePosition = VectorFunction::Negate(eyePosition);

		// 視点の位置に対する各方向のオフセットを計算する
		sse::Vector128 dotRight   = VectorFunction::Set(VectorFunction::DotVector3(normalizedRightDirection, negativeEyePosition));
		sse::Vector128 dotUp      = VectorFunction::Set(VectorFunction::DotVector3(normalizedUpDirection, negativeEyePosition));
		sse::Vector128 dotForward = VectorFunction::Set(VectorFunction::DotVector3(normalizedForwardDirection, negativeEyePosition));

		sse::Matrix128 matrix
		(
			VectorFunction::Select(dotRight, normalizedRightDirection, sse::VECTOR_128F_SELECT_1110.V),
			VectorFunction::Select(dotRight, normalizedRightDirection, sse::VECTOR_128F_SELECT_1110.V),
			VectorFunction::Select(dotRight, normalizedRightDirection, sse::VECTOR_128F_SELECT_1110.V),
			VectorFunction::Select(dotRight, normalizedRightDirection, sse::VECTOR_128F_SELECT_1110.V)
		);

		return Transpose(matrix);
	}

	/****************************************************************************
	*                       LookToRH
	*************************************************************************//**
	*  @fn        inline Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::LookToRH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 eyeDirection, Vector128Utility::ConstVector128 worldUp) noexcept
	*
	*  @brief     視点を対象方向に向けるための変換行列を作成します(右手系)
	*
	*  @param[in] Vector128Utility::ConstVector128 eyePosition (カメラの位置)
	*  @param[in] Vector128Utility::ConstVector128 eyeDirection (カメラを向ける方向)
	*  @param[in] Vector128Utility::ConstVector128 worldUp (ワールドの上方向：外積のために使用)
	*
	*  @return 　　Matrix128
	*****************************************************************************/
	inline sse::Matrix128 SIMD_CALL_CONVENTION Matrix128Utility::LookToRH(Vector128Utility::ConstVector128 eyePosition, Vector128Utility::ConstVector128 eyeDirection, Vector128Utility::ConstVector128 worldUp) noexcept
	{
		return LookToLH(eyePosition, Vector128Utility::Negate(eyeDirection), worldUp);
	}
	#pragma endregion Math
#pragma endregion Implement
}
#endif
#endif