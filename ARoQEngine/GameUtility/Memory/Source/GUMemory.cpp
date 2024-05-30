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
*  @brief     1byte�̒l�Ŏw�肵���o�C�g���������������܂�.
*  @param[out] void* ����������擪�A�h���X
*  @param[in] const uint8 1byte�̒l
*  @param[in] const uint64 ����������o�C�g��
*  @return    void*
*************************************************************************/
void* Memory::Set(void* destination, const uint8 oneByteValue, const uint64 byteLength)
{
	return memset(destination, oneByteValue, byteLength);
}

/*!**********************************************************************
*  @brief     �w�肵���o�C�g�������[���ɏ��������܂�.
*  @param[out]void* 0�N���A����擪�A�h���X
*  @param[in] uint64 �[���N���A����o�C�g��
*  @return    void*
*************************************************************************/
void* Memory::Zero(void* destination, const uint64 byteLength)
{
	return memset(destination, 0, byteLength);
}

/*!**********************************************************************
*  @brief     �w�肵�����������������ړ����܂�. memmove�Ɠ�������, �I�[�o�[���b�v���Ă���肠��܂���.
*  @param[out]void* �ړ���̃A�h���X
*  @param[in] const void* �ړ����̃A�h���X
*  @param[in] const uint64 �ړ�����o�C�g��
*  @return    void*
*************************************************************************/
void* Memory::Move(void* destination, const void* source, const uint64 byteLength)
{
	return memmove(destination, source, byteLength);
}

/*!**********************************************************************
*  @brief    ���������w�肵���o�C�g���������R�s�[. memcpy�Ɠ�������, �I�[�o�[���b�v���Ă���Ɩ�肪�������܂�.
*  @param[out]void* �ړ���̃A�h���X
*  @param[in] const void* �ړ����̃A�h���X
*  @param[in] const uint64 �ړ�����o�C�g��
*  @return    void*
*************************************************************************/
void* Memory::Copy(void* destination, const void* source, const uint64 byteLength)
{
	return memcpy(destination, source, byteLength);
}

/*!**********************************************************************
*  @brief     �������r���� (+) left > right, (0) left == right (-) left < right
*  @param[in] const void* ��r���鍶�̃A�h���X
*  @param[in] const void* ��r����E�̃A�h���X
*  @param[in] const uint64 ��r����o�C�g��
*  @return    int32 �o�b�t�@�[�Ԃōŏ��ɈقȂ�o�C�g�̍���Ԃ��܂�. �l�ɈӖ��͂��܂�Ȃ�����, �d�v�Ȃ̂͂��̕����ɂȂ�܂�.
*************************************************************************/
int32 Memory::Compare(const void* left, const void* right, const uint64 byteLength)
{
	return memcmp(left, right, byteLength);
}

/*!**********************************************************************
*  @brief     �w�肵���o�C�g���[���ɐݒ肳��Ă��邩
*  @param[in] const void* ��r����A�h���X
*  @param[in] const uint64 ��r����o�C�g��
*  @return    bool �[���ɐݒ肳��Ă���ꍇ��true, ����ȊO��false
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