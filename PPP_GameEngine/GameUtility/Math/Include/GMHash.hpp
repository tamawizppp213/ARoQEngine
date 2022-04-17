//////////////////////////////////////////////////////////////////////////////////
///             @file   GMHash.hpp
///             @brief  Hash function (CRC32 algorithm)
///             @author https://github.com/microsoft/DirectX-Graphics-Samples/blob/master/MiniEngine/Core/Hash.h
///             @date   2022_04_17
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_HASH
#define GM_HASH

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMVectorUtility.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#ifdef _M_X64
#define ENABLE_SSE_CRC32 1
#else
#define ENABLE_SSE_CRC32 0
#endif

#if ENABLE_SSE_CRC32
#pragma intrinsic(_mm_crc32_u32)
#pragma intrinsic(_mm_crc32_u64)
#endif
//////////////////////////////////////////////////////////////////////////////////
//                           Type def
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	inline size_t HashRange(const uint32_t* const begin, const uint32_t* const  end, size_t hash)
	{
#if ENABLE_SSE_CRC32
		const uint64_t*       iter64 = (const uint64_t*)utils::AlignUp(begin, 8);
		const uint64_t* const end64  = (const uint64_t* const)utils::AlignDown(end, 8);

		// If not 64-bit aligned, start with a single u32
		if ((uint32_t*)iter64 > begin) { hash = _mm_crc32_u32((uint32_t)hash, *begin); }

		// iterate over consecutive u64 values
		while (iter64 < end64)
		{
			hash = _mm_crc32_u64((uint64_t)hash, *iter64++);
		}

		// if there is a 32-bit remainder, accumulate that
		if ((uint32_t*)iter64 < end)
		{
			hash = _mm_crc32_u32((uint32_t)hash, *(uint32_t*)iter64);
		}

#else
		for (const uint32_t* iter = begin; iter < end; ++iter)
		{
			hash = 16777619U * hash ^ *iter;
		}
#endif
		return hash;
	}

	template<typename T> inline size_t HashState(const T* stateDesc, size_t count = 1, size_t hash = 2166136261U)
	{
		static_assert((sizeof(T) & 3) == 0 && alignof(T) >= 4, "State object is not word-aligned");
		return HashRange((uint32_t*)stateDesc, (uint32_t*)(stateDesc + count), hash);
	}
}

#endif