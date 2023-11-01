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
*************************************************************************//**
*  @fn        void PlatformCommand::PutDebugBreak() const
*
*  @brief     デバッグ用のブレークポイントを設置します
*
* 　@param[in] void
*
*  @return 　　void
*****************************************************************************/
void PlatformCommand::PutDebugBreak() const
{
	DebugBreak();
}

#pragma endregion Debugging Command

gu::UUID PlatformCommand::IssueUUID()
{
	gu::UUID uuid(0, 0, 0, 0);
	Verifyf(CoCreateGuid((GUID*)&uuid) == S_OK, "failed to create uuid");
	return uuid;
}
#pragma endregion Main Function