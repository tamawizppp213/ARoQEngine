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
void* Memory::Set(void* destination, const uint8 oneByteValue, const size_t byteLength)
{
	return memset(destination, oneByteValue, byteLength);
}

/*---------------------------------------------------------------
	@brief :  指定したバイト長だけゼロに初期化します.
-----------------------------------------------------------------*/
void* Memory::Zero(void* destination, const size_t byteLength)
{
	return memset(destination, 0, byteLength);
}

/*---------------------------------------------------------------
	@brief :  指定した分だけメモリを移動します.
-----------------------------------------------------------------*/
void* Memory::Move(void* destination, const void* source, const size_t byteLength)
{
	return memmove(destination, source, byteLength);
}

/*---------------------------------------------------------------
	@brief :  メモリを指定したバイト数分だけコピー
-----------------------------------------------------------------*/
void* Memory::Copy(void* destination, const void* source, const size_t byteLength)
{
	return memcpy(destination, source, byteLength);
}

/*---------------------------------------------------------------
	@brief :   文字を比較する (+) left > right, (0) left == right (-) left < right
-----------------------------------------------------------------*/
int32 Memory::Compare(const void* left, const void* right, const size_t byteLength)
{
	return memcmp(left, right, byteLength);
}
/*---------------------------------------------------------------
	@brief :  指定したバイトがゼロに設定されているか
-----------------------------------------------------------------*/
bool Memory::IsZero(const void* pointer, const size_t byteLength)
{
	uint8* start = (uint8*)pointer;
	uint8* end   = start + byteLength;
	while (start < end)
	{
		if((*start++) != 0){return false;}
	}
	return true;
}
#pragma endregion Main Function