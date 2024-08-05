//////////////////////////////////////////////////////////////////////////////////
///  @file   CoreOS.hpp
///  @brief  temp
///  @author toide
///  @date   2024/06/23 23:42:12
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CORE_OS_HPP
#define CORE_OS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "CorePlatformMacros.hpp"
#include PLATFORM_COMPILED_HEADER(Platform, OS.hpp)

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace platform::core
{
	/****************************************************************************
	*				  			   CoreOS
	*************************************************************************//**
	/*  @brief   OS情報をまとめたクラス
	*****************************************************************************/
	#if PLATFORM_OS_WINDOWS
	using OS = platform::windows::WindowsOS;
	#endif
}

#endif