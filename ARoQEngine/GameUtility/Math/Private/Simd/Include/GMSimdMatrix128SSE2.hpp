//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdMatrix128SSE.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/20 15:36:48
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_MATRIX_128_SSE2_HPP
#define GM_SIMD_MATRIX_128_SSE2_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMSimdMacros.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_CPU_INSTRUCTION_SSE2 && !defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
#include "GMSimdQuaternion128SSE2.hpp"
#include "GMSimdMatrix128SSE.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gm::simd::sse2
{
	using Matrix128Utility = gm::simd::sse::Matrix128Utility;
}
#endif
#endif