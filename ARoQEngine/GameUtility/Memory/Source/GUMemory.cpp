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
/*!**********************************************************************
*  @brief     1byteの値で指定したバイト長だけ初期化します.
*  @param[out] void* 初期化する先頭アドレス
*  @param[in] const uint8 1byteの値
*  @param[in] const uint64 初期化するバイト長
*  @return    void*
*************************************************************************/
void* Memory::Set(void* destination, const uint8 oneByteValue, const uint64 byteLength)
{
	return memset(destination, oneByteValue, byteLength);
}

/*!**********************************************************************
*  @brief     指定したバイト長だけゼロに初期化します.
*  @param[out]void* 0クリアする先頭アドレス
*  @param[in] uint64 ゼロクリアするバイト長
*  @return    void*
*************************************************************************/
void* Memory::Zero(void* destination, const uint64 byteLength)
{
	return memset(destination, 0, byteLength);
}

/*!**********************************************************************
*  @brief     指定した分だけメモリを移動します. memmoveと同じため, オーバーラップしても問題ありません.
*  @param[out]void* 移動先のアドレス
*  @param[in] const void* 移動元のアドレス
*  @param[in] const uint64 移動するバイト長
*  @return    void*
*************************************************************************/
void* Memory::Move(void* destination, const void* source, const uint64 byteLength)
{
	return memmove(destination, source, byteLength);
}

/*!**********************************************************************
*  @brief    メモリを指定したバイト数分だけコピー. memcpyと同じため, オーバーラップしていると問題が発生します.
*  @param[out]void* 移動先のアドレス
*  @param[in] const void* 移動元のアドレス
*  @param[in] const uint64 移動するバイト長
*  @return    void*
*************************************************************************/
void* Memory::Copy(void* destination, const void* source, const uint64 byteLength)
{
	return memcpy(destination, source, byteLength);
}

/*!**********************************************************************
*  @brief     文字を比較する (+) left > right, (0) left == right (-) left < right
*  @param[in] const void* 比較する左のアドレス
*  @param[in] const void* 比較する右のアドレス
*  @param[in] const uint64 比較するバイト長
*  @return    int32 バッファー間で最初に異なるバイトの差を返します. 値に意味はあまりないため, 重要なのはその符号になります.
*************************************************************************/
int32 Memory::Compare(const void* left, const void* right, const uint64 byteLength)
{
	return memcmp(left, right, byteLength);
}

/*!**********************************************************************
*  @brief     指定したバイトがゼロに設定されているか
*  @param[in] const void* 比較するアドレス
*  @param[in] const uint64 比較するバイト長
*  @return    bool ゼロに設定されている場合はtrue, それ以外はfalse
*************************************************************************/
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