//////////////////////////////////////////////////////////////////////////////////
///             @file   GUMemory.cpp
///             @brief  temp
///             @author toide
///             @date   2023/12/03 0:28:32
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/GUMemory.hpp"
#include <string.h>
#include <malloc.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 

#pragma endregion Constructor and Destructor

#pragma region Main Function
/*---------------------------------------------------------------
	@brief :  指定した分だけメモリの初期化を行います.
-----------------------------------------------------------------*/
void* Memory::Set(void* destination, const uint8 oneByteValue, const uint64 byteLength)
{
	return memset(destination, oneByteValue, byteLength);
}

/*---------------------------------------------------------------
	@brief :  指定したバイト長だけゼロに初期化します.
-----------------------------------------------------------------*/
void* Memory::Zero(void* destination, const uint64 byteLength)
{
	return memset(destination, 0, byteLength);
}

/*---------------------------------------------------------------
	@brief :  指定した分だけメモリを移動します.
-----------------------------------------------------------------*/
void* Memory::Move(void* destination, const void* source, const uint64 byteLength)
{
	return memmove(destination, source, byteLength);
}

/*---------------------------------------------------------------
	@brief :  メモリを指定したバイト数分だけコピー
-----------------------------------------------------------------*/
void* Memory::Copy(void* destination, const void* source, const uint64 byteLength)
{
	return memcpy(destination, source, byteLength);
}

/*---------------------------------------------------------------
	@brief :   文字を比較する (+) left > right, (0) left == right (-) left < right
-----------------------------------------------------------------*/
int32 Memory::Compare(const void* left, const void* right, const uint64 byteLength)
{
	return memcmp(left, right, byteLength);
}
/*---------------------------------------------------------------
	@brief :  指定したバイトがゼロに設定されているか
-----------------------------------------------------------------*/
bool Memory::IsZero(const void* pointer, const uint64 byteLength)
{
	uint8* start = (uint8*)pointer;
	uint8* end   = start + byteLength;
	while (start < end)
	{
		if((*start++) != 0){return false;}
	}
	return true;
}

void* Memory::Allocate(const uint64 byteLength)
{
	return ::malloc(byteLength);
}

void* Memory::Reallocate(void* pointer, const uint64 byteLength)
{
	return realloc(pointer, byteLength);
}

void* Memory::AllocateAligned(const uint64 byteLength, const uint64 alignment)
{
	return ::_aligned_malloc(byteLength, alignment);
}

void Memory::Free(void* pointer)
{
	::free(pointer);
}

void Memory::FreeAligned(void* pointer)
{
	::_aligned_free(pointer);
}
#pragma endregion Main Function