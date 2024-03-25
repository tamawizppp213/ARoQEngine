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
#define PLATFORM_OS_WINDOWS 1
#else
#define PLATFORM_OS_WINDOWS 0
#endif // defined(_WIN32) || defined(_WIN64)

/*-------------------------------------------------------------------
-             Android
---------------------------------------------------------------------*/
#if defined(__ANDROID__)
#define PLATFORM_OS_ANDROID 1
#else
#define PLATFORM_OS_ANDROID 0
#endif // defined(__ANDROID__)

/*-------------------------------------------------------------------
-             Linux
---------------------------------------------------------------------*/
#if !defined(__ANDROID__) && defined(__linux__)
#define PLATFORM_OS_LINUX 1
#else
#define PLATFORM_OS_LINUX 0
#endif // !defined(__ANDROID__) && defined(__linux__)

/*-------------------------------------------------------------------
-             IOS
---------------------------------------------------------------------*/
#if defined(IPHONE)
#define PLATFORM_OS_IOS 1
#else
#define PLATFORM_OS_IOS 0
#endif // defined(IPHONE)

/*-------------------------------------------------------------------
-             Mac
---------------------------------------------------------------------*/
#if !defined(IPHONE) && defined(__MACH__)
#define PLATFORM_OS_MAC 1
#else
#define PLATFORM_OS_MAC 0
#endif // !defined(IPHONE) && defined(__MACH__)

/*-------------------------------------------------------------------
-             Unix
---------------------------------------------------------------------*/
#if defined(__unix__)
#define PLATFORM_OS_UNIX 1
#else
#define PLATFORM_OS_UNIX 0
#endif // defined(__unix__)

/*-------------------------------------------------------------------
-             FreeBSD
---------------------------------------------------------------------*/
#if defined(__FreeBSD__)
#define PLATFORM_OS_FREEBSD 1
#else
#define PLATFORM_OS_FREEBSD 0
#endif // defined(__FreeBSD__)

#pragma endregion Platform type

#pragma region Compiler
/*-------------------------------------------------------------------
-             LLVM and Clang
---------------------------------------------------------------------*/
#if defined(__llvm__)
#define PLATFORM_COMPILER_GCC 0
#define PLATFORM_COMPILER_LLVM  1
#define PLATFORM_COMPILER_CLANG 1
#define PLATFORM_COMPILER_GCC_COMPATIBLE 1
#elif defined(__clang__)
#define PLATFORM_COMPILER_GCC 0
#define PLATFORM_COMPILER_LLVM 0
#define PLATFORM_COMPILER_CLANG 1
#define PLATFORM_COMPILER_GCC_COMPATIBLE 1
#elif defined(__GNUC__)
#define PLATFORM_COMPILER_GCC 1
#define PLATFORM_COMPILER_LLVM 0
#define PLATFORM_COMPILER_CLANG 0
#define PLATFORM_COMPILER_GCC_COMPATIBLE 1
#else
#define PLATFORM_COMPILER_GCC 0
#define PLATFORM_COMPILER_LLVM 0
#define PLATFORM_COMPILER_CLANG 0
#define PLATFORM_COMPILER_GCC_COMPATIBLE 0
#endif

/*-------------------------------------------------------------------
-            Visual studio (MSVC)
---------------------------------------------------------------------*/
#if defined(_MSC_VER)
#define PLATFORM_COMPILER_MSVC 1
#else
#define PLATFORM_COMPILER_MSVC 0
#endif
#pragma endregion Compiler

#pragma region CPU architecture
#ifndef PLATFORM_CPU_ARCHITECTURE
#define PLATFORM_CPU_ARCHITECTURE
/*-------------------------------------------------------------------
-             X86
---------------------------------------------------------------------*/
#if (defined(_M_IX86) || defined(__i386__) || defined(_M_X64) || defined(__amd64__) || defined(__x86_64__)) && !defined(_M_ARM64EC)
#define PLATFORM_CPU_X86_FAMILY 1
#else 
#define PLATFORM_CPU_X86_FAMILY 0
#endif

/*-------------------------------------------------------------------
-             ARM
---------------------------------------------------------------------*/
#if (defined(__arm__) || defined(_M_ARM) || defined(__aarch64__) || defined(_M_ARM64) || defined(_M_ARM64EC))
#define PLATFORM_CPU_ARM_FAMILY	1
#else
#define PLATFORM_CPU_ARM_FAMILY	0
#endif

#endif // PLATFORM_CPU_ARCHITECTURE
#pragma endregion CPU architecture

// GM_SIMD_Macrosも変更をお願いいたします。
#pragma region SIMD
#ifndef PLATFORM_SIMD
#define PLATFORM_SIMD
#if PLATFORM_COMPILER_GCC_COMPATIBLE
	#if defined(__MMX__)
		#define PLATFORM_CPU_INSTRUCTION_MMX 1
	#else
		#define PLATFORM_CPU_INSTRUCTION_MMX 0
	#endif

	#if defined(__SSE__)
		#define PLATFORM_CPU_INSTRUCTION_SSE 1
	#else
		#define PLATFORM_CPU_INSTRUCTION_SSE 0
	#endif

	#if defined(__SSE2__)
		#define PLATFORM_CPU_INSTRUCTION_SSE2 1
	#else
		#define PLATFORM_CPU_INSTRUCTION_SSE2 0
	#endif

	#if defined(__SSE3__)
		#define PLATFORM_CPU_INSTRUCTION_SSE3 1
	#else
		#define PLATFORM_CPU_INSTRUCTION_SSE3 0
	#endif

	#if defined(__SSSE3__)
		#define PLATFORM_CPU_INSTRUCTION_SSSE3 1
	#else
		#define PLATFORM_CPU_INSTRUCTION_SSSE3 0
	#endif

	#if defined(__SSE4_1__)
		#define PLATFORM_CPU_INSTRUCTION_SSE4_1 1
	#else
		#define PLATFORM_CPU_INSTRUCTION_SSE4_1 0
	#endif

	#if defined(__SSE4_2__)
		#define PLATFORM_CPU_INSTRUCTION_SSE4_2 1
	#else
		#define PLATFORM_CPU_INSTRUCTION_SSE4_2 0
	#endif

	#if defined(__PCLMUL__)
		#define PLATFORM_CPU_INSTRUCTION_PCLMUL 1
	#else
		#define PLATFORM_CPU_INSTRUCTION_PCLMUL 0
	#endif

	#if defined(__AVX__)
		#define PLATFORM_CPU_INSTRUCTION_AVX 1
	#else
		#define PLATFORM_CPU_INSTRUCTION_AVX 0
	#endif
	
	#if defined(__AVX2__)
		#define PLATFORM_CPU_INSTRUCTION_AVX2 1
	#else
		#define PLATFORM_CPU_INSTRUCTION_AVX2 0
	#endif

	#if (_MSC_VER >= 1920)
		#define PLATFORM_CPU_INSTRUCTION_SVML 1
	#else
		#define PLATFORM_CPU_INSTRUCTION_SVML 0
	#endif

	#if defined(__ARM_NEON__) || defined(__ARM_NEON)
		#define PLATFORM_CPU_INSTRUCTION_NEON 1
	#else
		#define PLATFORM_CPU_INSTRUCTION_NEON 0
	#endif
#elif defined(_MSC_VER) && PLATFORM_CPU_X86_FAMILY
    // MMX, SSE, SSE2はVS2003で導入された.
	#if (_MSC_VER >= 1310)
		#define PLATFORM_CPU_INSTRUCTION_MMX  1
		#define PLATFORM_CPU_INSTRUCTION_SSE  1
		#define PLATFORM_CPU_INSTRUCTION_SSE2 1
	#else
		#define PLATFORM_CPU_INSTRUCTION_MMX  0
		#define PLATFORM_CPU_INSTRUCTION_SSE  0
		#define PLATFORM_CPU_INSTRUCTION_SSE2 0
	#endif
	// SSE3はVS2005で導入された.
	#if (_MSC_VER >= 1400)
		#define PLATFORM_CPU_INSTRUCTION_SSE3 1
	#else
		#define PLATFORM_CPU_INSTRUCTION_SSE3 0
	#endif
	// SSSE3, SSE4.1, SSE4.2, PCLMULはVS2008で導入された
	#if (_MSC_VER >= 1500)
		#define PLATFORM_CPU_INSTRUCTION_SSSE3 1
		#define PLATFORM_CPU_INSTRUCTION_SSE4_1 1
		#define PLATFORM_CPU_INSTRUCTION_SSE4_2 1
		#define PLATFORM_CPU_INSTRUCTION_PCLMUL 1
	#else
		#define PLATFORM_CPU_INSTRUCTION_SSSE3 0
		#define PLATFORM_CPU_INSTRUCTION_SSE4_1 0
		#define PLATFORM_CPU_INSTRUCTION_SSE4_2 0
		#define PLATFORM_CPU_INSTRUCTION_PCLMUL 0
	#endif
	// AVX and AVX2 introduced in VS2012
	#if (_MSC_VER >= 1700)
		#define PLATFORM_CPU_INSTRUCTION_AVX 1
		#define PLATFORM_CPU_INSTRUCTION_AVX2 1
	#else
		#define PLATFORM_CPU_INSTRUCTION_AVX 0
		#define PLATFORM_CPU_INSTRUCTION_AVX2 0
	#endif
	#if (_MSC_VER >= 1920)
		#define PLATFORM_CPU_INSTRUCTION_SVML 1
	#else
		#define PLATFORM_CPU_INSTRUCTION_SVML 0
	#endif
#else
	#define PLATFORM_CPU_INSTRUCTION_MMX  0
	#define PLATFORM_CPU_INSTRUCTION_SSE  0
	#define PLATFORM_CPU_INSTRUCTION_SSE2 0
	#define PLATFORM_CPU_INSTRUCTION_SSE3 0
	#define PLATFORM_CPU_INSTRUCTION_SSSE3 0
	#define PLATFORM_CPU_INSTRUCTION_SSE4_1 0
	#define PLATFORM_CPU_INSTRUCTION_SSE4_2 0
	#define PLATFORM_CPU_INSTRUCTION_PCLMUL 0
	#define PLATFORM_CPU_INSTRUCTION_AVX 0
	#define PLATFORM_CPU_INSTRUCTION_AVX2 0
#endif
#endif // PLATFORM_SIMD
#pragma endregion SIMD

#pragma region Platform Class
#if PLATFORM_OS_WINDOWS 
	#define PLATFORM_CLASS(className) platform::windows::##className
	#define PLATFORM_NAME Windows
#elif PLATFORM_OS_ANDROID
	#define PLATFORM_CLASS(className) platform::android::##className
	#define PLATFORM_NAME Android
#elif PLATFORM_OS_LINUX
	#define PLATFORM_CLASS(className) platform::Linux::##className
	#define PLATFORM_NAME Linux
#elif PLATFORM_OS_IOS
	#define PLATFORM_CLASS(className) platform::ios::##className
	#define PLATFORM_NAME IOS
#elif PLATFORM_OS_MAC
	#define PLATFORM_CLASS(className) platform::mac::##className
	#define PLATFORM_NAME MAC
#elif PLATFORM_OS_UNIX
	#define PLATFORM_CLASS(className) platform::unix::##className
	#define PLATFORM_NAME Unix
#elif PLATFORM_OS_FREEBSD
	#define PLATFORM_CLASS(className) platform::freebsd::##className
	#define PLATFORM_NAME FreeBSD
#else
	#define PLATFORM_CLASS(className) platform::core::##className
	#define PLATFORM_NAME Core
#endif

#ifndef PREPROCESSOR_TO_STRING_INNER
	#define PREPROCESSOR_TO_STRING_INNER(str) #str
#endif  PREPROCESSOR_TO_STRING_INNER

#ifndef PREPROCESSOR_TO_STRING
	#define PREPROCESSOR_TO_STRING(str) PREPROCESSOR_TO_STRING_INNER(str)
#endif PREPROCESSOR_TO_STRING

#ifndef PREPROCESSOR_JOIN_INNER
	#define PREPROCESSOR_JOIN_INNER(left, right) left##right
#endif PREPROCESSOR_JOIN_INNER

#ifndef PREPROCESSOR_JOIN
	#define PREPROCESSOR_JOIN(left, right) PREPROCESSOR_JOIN_INNER(left, right)
#endif PREPROCESSOR_JOIN

// PreDirectory/PLATFORM_NAME/Include/PLATFORM_NAME + commonHeaderName
#define PLATFORM_COMPILED_HEADER(preDirectory, commonHeaderName) PREPROCESSOR_TO_STRING(preDirectory/PLATFORM_NAME/Include/PREPROCESSOR_JOIN(PLATFORM_NAME,commonHeaderName)) 
#pragma endregion Platform NameSpace
#pragma region Platform Desktop
#if PLATFORM_OS_WINDOWS 
#define PLATFORM_DESKTOP (1)
#elif PLATFORM_OS_ANDROID
#define PLATFORM_DESKTOP (0)
#elif PLATFORM_OS_LINUX
#define PLATFORM_DESKTOP (1)
#elif PLATFORM_OS_IOS
#define PLATFORM_DESKTOP (0)
#elif PLATFORM_OS_MAC
#define PLATFORM_DESKTOP (1)
#elif PLATFORM_OS_UNIX
#define PLATFORM_DESKTOP (1)
#else
#define PLATFORM_DESKTOP (0)
#endif
#pragma endregion Platform Desktop
#endif