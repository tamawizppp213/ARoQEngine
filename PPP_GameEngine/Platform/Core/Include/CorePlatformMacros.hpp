//////////////////////////////////////////////////////////////////////////////////
///             @file   CorePlatformMacros.hpp
///             @brief  プラットフォームを決定するマクロ群です. 
///             @author Toide Yutaro
///             @date   2023_10_25
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CORE_PLATFORM_MACROS_HPP
#define CORE_PLATFORM_MACROS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
**                Platform type
*****************************************************************************/
#pragma region Platform type
/*-------------------------------------------------------------------
-             Windows
---------------------------------------------------------------------*/
#if defined(_WIN32) || defined(_WIN64)
#define PLATFORM_WINDOWS 1
#else
#define PLATFORM_WINDOWS 0
#endif // defined(_WIN32) || defined(_WIN64)

/*-------------------------------------------------------------------
-             Android
---------------------------------------------------------------------*/
#if defined(__ANDROID__)
#define PLATFORM_ANDROID 1
#else
#define PLATFORM_ANDROID 0
#endif // defined(__ANDROID__)

/*-------------------------------------------------------------------
-             Linux
---------------------------------------------------------------------*/
#if !defined(__ANDROID__) && defined(__linux__)
#define PLATFORM_LINUX 1
#else
#define PLATFORM_LINUX 0
#endif // !defined(__ANDROID__) && defined(__linux__)

/*-------------------------------------------------------------------
-             IOS
---------------------------------------------------------------------*/
#if defined(IPHONE)
#define PLATFORM_IOS 1
#else
#define PLATFORM_IOS 0
#endif // defined(IPHONE)

/*-------------------------------------------------------------------
-             Mac
---------------------------------------------------------------------*/
#if !defined(IPHONE) && defined(__MACH__)
#define PLATFORM_MAC 1
#else
#define PLATFORM_MAC 0
#endif // !defined(IPHONE) && defined(__MACH__)

/*-------------------------------------------------------------------
-             Unix
---------------------------------------------------------------------*/
#if defined(__unix__)
#define PLATFORM_UNIX 1
#else
#define PLATFORM_UNIX 0
#endif // defined(__unix__)

/*-------------------------------------------------------------------
-             FreeBSD
---------------------------------------------------------------------*/
#if defined(__FreeBSD__)
#define PLATFORM_FREEBSD 1
#else
#define PLATFORM_FREEBSD 0
#endif // defined(__FreeBSD__)

#pragma endregion Platform type

#endif