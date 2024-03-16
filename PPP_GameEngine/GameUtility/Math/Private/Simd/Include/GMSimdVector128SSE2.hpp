//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdVector128SSE2.hpp
///             @brief  SSEによるSimd演算です.
///                     Vector128型の実装 
///             @author toide
///             @date   2024/03/16 13:06:35
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_VECTOR_128_SSE2_HPP
#define GM_SIMD_VECTOR_128_SSE2_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMSimdMacros.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_CPU_INSTRUCTION_SSE2 && !defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
#include "GMSimdVector128SSE.hpp"
#include <emmintrin.h> // SSE2
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::simd::sse2
{

	/****************************************************************************
	*				  			   Vector128Utility
	*************************************************************************//**
	*  @class     Vector128Utility
	*  @brief     SSE2のバージョンによるSimd演算
	*****************************************************************************/
	class Vector128Utility : public gu::simd::sse::Vector128Utility
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

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
	};
}
#endif
#endif