//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdMatrix128SSE.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/20 15:36:48
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_QUATERNION_128_SSE3_HPP
#define GM_SIMD_QUATERNION_128_SSE3_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMSimdMacros.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_CPU_INSTRUCTION_SSE3 && !defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
#include "GMSimdVector128SSE3.hpp"
#include "GMSimdQuaternion128SSE2.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gm::simd::sse3
{
	/****************************************************************************
	*				  			   GMSimdMatrix128SSE
	*************************************************************************//**
	*  @class     GMSimdMatrix128SSE
	*  @brief     temp
	*****************************************************************************/
	class Quaternion128Utility : public gm::simd::sse2::Quaternion128Utility
	{
	public:

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : 内積を算出します. 通常のベクトル同様, 互いのベクトルのなす角度を導出したいときに使用します
		/*----------------------------------------------------------------------*/
		float SIMD_CALL_CONVENTION Dot(ConstQuaternion128 left, ConstQuaternion128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 1次ノルム (絶対値の総和を取った後に根号を取る)を取得します
		/*----------------------------------------------------------------------*/
		float SIMD_CALL_CONVENTION Norm(ConstQuaternion128 quaternion) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : クォータニオンの正規化を行います
		/*----------------------------------------------------------------------*/
		sse::Quaternion128 SIMD_CALL_CONVENTION Normalize(ConstQuaternion128 quaternion) noexcept;
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
	*                       Dot
	*************************************************************************//**
	*  @fn        float SIMD_CALL_CONVENTION Quaternion128Utility::Dot(ConstQuaternion128 left, ConstQuaternion128 right) noexcept
	*
	*  @brief      内積を算出します. 通常のベクトル同様, 互いのベクトルのなす角度を導出したいときに使用します
	*
	*  @param[in] ConstQuaternion128 quaternion
	*
	*  @return 　　float
	*****************************************************************************/
	float SIMD_CALL_CONVENTION Quaternion128Utility::Dot(ConstQuaternion128 left, ConstQuaternion128 right) noexcept
	{
		return Vector128Utility::DotVector4(left, right);
	}

	/****************************************************************************
	*                       Norm
	*************************************************************************//**
	*  @fn        float SIMD_CALL_CONVENTION Quaternion128Utility::Norm(ConstQuaternion128 quaternion) noexcept
	*
	*  @brief     1次ノルム (絶対値の総和を取った後に根号を取る)を取得します
	*
	*  @param[in] ConstQuaternion128 quaternion
	*
	*  @return 　　float
	*****************************************************************************/
	float SIMD_CALL_CONVENTION Quaternion128Utility::Norm(ConstQuaternion128 quaternion) noexcept
	{
		return Vector128Utility::NormVector4(quaternion);
	}

	/****************************************************************************
	*                       Normalize
	*************************************************************************//**
	*  @fn        Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::Normalize(ConstQuaternion128 quaternion) noexcept
	*
	*  @brief      クォータニオンの正規化を行います
	*
	*  @param[in] ConstQuaternion128 quaternion
	*
	*  @return 　　Quaternion128
	*****************************************************************************/
	sse::Quaternion128 SIMD_CALL_CONVENTION Quaternion128Utility::Normalize(ConstQuaternion128 quaternion) noexcept
	{
		return Vector128Utility::NormalizeVector4(quaternion);
	}
	#pragma endregion Math
#pragma endregion Implement
}
#endif
#endif