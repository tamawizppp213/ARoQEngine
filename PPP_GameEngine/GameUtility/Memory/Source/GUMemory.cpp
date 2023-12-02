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
	@brief :  �w�肵���������������̏��������s���܂�.
-----------------------------------------------------------------*/
void* Memory::Set(void* destination, const uint8 oneByteValue, const size_t byteLength)
{
	return memset(destination, oneByteValue, byteLength);
}

/*---------------------------------------------------------------
	@brief :  �w�肵���o�C�g�������[���ɏ��������܂�.
-----------------------------------------------------------------*/
void* Memory::Zero(void* destination, const size_t byteLength)
{
	return memset(destination, 0, byteLength);
}

/*---------------------------------------------------------------
	@brief :  �w�肵�����������������ړ����܂�.
-----------------------------------------------------------------*/
void* Memory::Move(void* destination, const void* source, const size_t byteLength)
{
	return memmove(destination, source, byteLength);
}

/*---------------------------------------------------------------
	@brief :  ���������w�肵���o�C�g���������R�s�[
-----------------------------------------------------------------*/
void* Memory::Copy(void* destination, const void* source, const size_t byteLength)
{
	return memcpy(destination, source, byteLength);
}

/*---------------------------------------------------------------
	@brief :   �������r���� (+) left > right, (0) left == right (-) left < right
-----------------------------------------------------------------*/
int32 Memory::Compare(const void* left, const void* right, const size_t byteLength)
{
	return memcmp(left, right, byteLength);
}
/*---------------------------------------------------------------
	@brief :  �w�肵���o�C�g���[���ɐݒ肳��Ă��邩
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