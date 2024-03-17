//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdMacros.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/03 4:10:36
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_MACROS_HPP
#define GM_SIMD_MACROS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
//#define PLATFORM_CPU_INSTRUCTION_NOT_USE
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm::simd
{
	#if defined(_MSC_VER) && defined(_GAMING_XBOX) && defined(_M_X64) && !defined(USE_F16C_SIMD_INSTRUCTION) && !defined(PLATFORM_NOT_USE_SIMD_INSTRUCTION)
		#define USE_F16C_SIMD_INSTRUCTION
	#endif

	/*----------------------------------------------------------------------
	*             �����̈������܂Ƃ߂Ē��ڒl�n���ɂ���VectorCall�̎g�p���s����
	*----------------------------------------------------------------------*/
	#if defined(_MSC_VER) && !defined(_M_ARM) && !defined(_M_ARM64) && !defined(_M_HYBRID_X86_ARM64) && !defined(_M_ARM64EC) && (!_MANAGED) && (!_M_CEE) && (!defined(_M_IX86_FP) || (_M_IX86_FP > 1)) && !defined(PLATFORM_NOT_USE_SIMD_INSTRUCTION) && !defined(USE_VECTOR_CALL)
		#define USE_VECTOR_CALL 1
	#endif

	/*----------------------------------------------------------------------
	*             �Ăяo���K��̍���(��{��vectorcall���̗p���܂�)
	*----------------------------------------------------------------------*/
	#if USE_VECTOR_CALL
		#define SIMD_CALL_CONVENTION __vectorcall
	#elif defined(__GNUC__)
		#define SIMD_CALL_CONVENTION
	#else
		#define SIMD_CALL_CONVENTION __fastcall
	#endif

	/*----------------------------------------------------------------------
	*             CPU architecture�̐ݒ�
	*----------------------------------------------------------------------*/
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

	/*----------------------------------------------------------------------
	*             SIMD�̐ݒ�
	*----------------------------------------------------------------------*/
	// GM_SIMD_Macros���ύX�����肢�������܂��B
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
		// MMX, SSE, SSE2��VS2003�œ������ꂽ.
		#if (_MSC_VER >= 1310)
			#define PLATFORM_CPU_INSTRUCTION_MMX  1
			#define PLATFORM_CPU_INSTRUCTION_SSE  1
			#define PLATFORM_CPU_INSTRUCTION_SSE2 1
		#else
			#define PLATFORM_CPU_INSTRUCTION_MMX  0
			#define PLATFORM_CPU_INSTRUCTION_SSE  0
			#define PLATFORM_CPU_INSTRUCTION_SSE2 0
		#endif
		// SSE3��VS2005�œ������ꂽ.
		#if (_MSC_VER >= 1400)
			#define PLATFORM_CPU_INSTRUCTION_SSE3 1
		#else
			#define PLATFORM_CPU_INSTRUCTION_SSE3 0
		#endif
		// SSSE3, SSE4.1, SSE4.2, PCLMUL��VS2008�œ������ꂽ
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
		#define PLATFORM_CPU_INSTRUCTION_NEON 0
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
		#define PLATFORM_CPU_INSTRUCTION_NEON 0
	#endif
#endif // PLATFORM_SIMD

/*----------------------------------------------------------------------
*             Simd�֐��̂����ǂ���Ăяo����
*----------------------------------------------------------------------*/
#ifndef PLATFORM_CPU_INSTRUCTION_NOT_USE
	#if PLATFORM_CPU_INSTRUCTION_NEON
		#define SIMD_NAME_SPACE gm::simd::neon
		#define SIMD_NAME Neon
	#elif PLATFORM_CPU_INSTRUCTION_AVX || PLATFORM_CPU_INSTRUCTION_AVX2
		#define SIMD_NAME_SPACE gm::simd::avx
		#define SIMD_STATIC_FUNCTION_NAME(className, functionName) gm::simd::avx::##className::##functionName
		#define SIMD_NAME AVX
	#elif PLATFORM_CPU_INSTRUCTION_SSE4_2 || PLATFORM_CPU_INSTRUCTION_SSE4_1
		#define SIMD_NAME_SPACE gm::simd::sse4
		#define SIMD_NAME SSE4
	#elif PLATFORM_CPU_INSTRUCTION_SSE3
		#define SIMD_NAME_SPACE gm::simd::sse3
		#define SIMD_NAME SSE3
	#elif PLATFORM_CPU_INSTRUCTION_SSE2
		#define SIMD_NAME_SPACE gm::simd::sse2
		#define SIMD_NAME SSE2
	#elif PLATFORM_CPU_INSTRUCTION_SSE
		#define SIMD_NAME_SPACE gm::simd::sse
		#define SIMD_NAME SSE
	#endif
#else
	#define SIMD_NAME_SPACE gm::simd::non
	#define SIMD_NAME Non
#endif  PLATFORM_CPU_INSTRUCTION_NOT_USE

/*----------------------------------------------------------------------
*             �������A��
*----------------------------------------------------------------------*/
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

// PreDirectory/commonHeaderName + SIMD_NAME
#define SIMD_COMPILED_HEADER(preDirectory, commonHeaderName) PREPROCESSOR_TO_STRING(preDirectory/PREPROCESSOR_JOIN(commonHeaderName, SIMD_NAME).hpp)
#pragma endregion SIMD


#if __cplusplus >= 201703L
	#define ALIGNED_DATA(x) alignas(x)
	#define ALIGNED_STRUCT(x) struct alignas(x)
#elif defined(__GNUC__)
	#define ALIGNED_DATA(x) __attribute__ ((aligned(x)))
	#define ALIGNED_STRUCT(x) struct __attribute__ ((aligned(x)))
#else
	#define ALIGNED_DATA(x) __declspec(align(x))
	#define ALIGNED_STRUCT(x) __declspec(align(x)) struct
#endif

#ifndef GLOBAL_CONST
	#if defined(__GNUC__) && !defined(__MINGW32__)
		#define GLOBAL_CONST extern const __attribute__((weak))
	#else
		#define GLOBAL_CONST extern const __declspec(selectany)
	#endif
#endif GLOBAL_CONST
}

#endif