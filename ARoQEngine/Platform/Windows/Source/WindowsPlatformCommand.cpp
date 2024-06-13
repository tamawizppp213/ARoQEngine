//////////////////////////////////////////////////////////////////////////////////
///             @file   WindowsPlatformCommand.cpp
///             @brief  temp
///             @author toide
///             @date   2023/10/30 3:09:18
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/WindowsPlatformCommand.hpp"
#if PLATFORM_OS_WINDOWS
#include "GameUtility/Base/Include/GUAssert.hpp"
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform::windows;
using namespace platform;
//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 

#pragma endregion Constructor and Destructor

#pragma region Main Function
#pragma region Debugging Command
bool PlatformCommand::UseDebugger() const
{
	return !!::IsDebuggerPresent();
}

/****************************************************************************
*                       PutDebugBreak
****************************************************************************/
/* @fn       void PlatformCommand::PutDebugBreak() const
*
*  @brief     �f�o�b�O�p�̃u���[�N�|�C���g��ݒu���܂�
*
* �@@param[in] void
*
*  @return �@�@void
*****************************************************************************/
void PlatformCommand::PutDebugBreak() const
{
	DebugBreak();
}

#pragma endregion Debugging Command
/****************************************************************************
*                       IssueUUID
****************************************************************************/
/* @fn       gu::UUID PlatformCommand::IssueUUID()
*
*  @brief     �I�u�W�F�N�g����肷��UUID�𔭍s���܂�. 
*
* �@@param[in] void
*
*  @return �@�@gu::UUID
*****************************************************************************/
gu::UUID PlatformCommand::IssueUUID()
{
	gu::UUID uuid(0, 0, 0, 0);
	Confirmf(CoCreateGuid((GUID*)&uuid) == S_OK, "failed to create uuid");
	return uuid;
}

#pragma region Initialize and Finalize Windows.h
/****************************************************************************
*                       CoInitialize
****************************************************************************/
/* @fn       bool PlatformCommand::CoInitialize(const bool useMultiThread = false)
*
*  @brief     COM���C�u���������������܂�
*
* �@@param[in] bool useMultiThread
*
*  @return �@�@bool
*****************************************************************************/
bool PlatformCommand::CoInitialize(const bool useMultiThread)
{
	HRESULT hResult = ::CoInitializeEx(nullptr, useMultiThread ? COINIT_MULTITHREADED : COINIT_APARTMENTTHREADED);
	
	// S_FALSE�ɂ��Ă͊��ɏ��������s���Ă��邽�߂ɐ�������Ƃ݂Ȃ��܂�. 
	return hResult == S_OK || hResult == S_FALSE;
}

/****************************************************************************
*                       CoUnInitialize
****************************************************************************/
/* @fn       bool PlatformCommand::CoUnInitialize()
*
*  @brief     COM���C�u���������������܂�
*
* �@@param[in] void
*
*  @return �@�@void
*****************************************************************************/
void PlatformCommand::CoUnInitialize()
{
	::CoUninitialize();
}

#pragma endregion Initialize and Finalize Windows.h
#pragma endregion Main Function
#endif