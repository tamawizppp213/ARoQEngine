//////////////////////////////////////////////////////////////////////////////////
///  @file   CoreAtomics.hpp
///  @brief  temp
///  @author toide
///  @date   2024/06/22 12:35:20
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CORE_ATOMICS_HPP
#define CORE_ATOMICS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "CorePlatformMacros.hpp"
#include PLATFORM_COMPILED_HEADER(Platform, AtomicOperator.hpp)

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::core
{
	/*! @brief 極力高速化のため, static関数呼び出しが出来るようにしました*/
	using AtomicOperator = PLATFORM_CLASS(CoreAtomicOperator);
}
#endif