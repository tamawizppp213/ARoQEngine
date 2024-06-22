//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsCriticalSection.cpp
///  @brief  OSにおいて, データの競合を防ぐためのクリティカルセクションを提供します.@n
///          一つのプロセス内で使用でき, プロセス間で共有することは出来ない.
///  @author toide
///  @date   2024/06/22 0:19:52
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Windows/Include/WindowsCriticalSection.hpp"
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform::windows;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
static_assert(sizeof(CRITICAL_SECTION) == sizeof(details::criticalSection::Info));

#pragma region Constructor and Destructor
/*! @brief デフォルトコンストラクタ*/
CoreCriticalSection::CoreCriticalSection()
{
	InitializeCriticalSection((LPCRITICAL_SECTION)&_criticalSectionInfo);
	SetCriticalSectionSpinCount((LPCRITICAL_SECTION)&_criticalSectionInfo, 4000);
}

/*! @brief デストラクタ*/
CoreCriticalSection::~CoreCriticalSection()
{
	DeleteCriticalSection((LPCRITICAL_SECTION)&_criticalSectionInfo);
}
#pragma endregion Constructor and Destructor

#pragma region Public Function

/*!**********************************************************************
*  @brief     スレッドの排他制御を行うために, 呼び出しスレッドをロックします. @n
*             他のスレッドがロックしている場合は, ロックが解除されるまで待機します.
*  @param[in] void
*  @return    void
*************************************************************************/
void CoreCriticalSection::Lock()
{
	EnterCriticalSection((LPCRITICAL_SECTION)&_criticalSectionInfo);
}

/*!**********************************************************************
*  @brief     スレッドの排他制御を行うために, 呼び出しスレッドをロックします. @n
*             他のスレッドがロックしている場合は, 即座にfalseを返します.
*  @param[in] void
*  @return    bool ロックが成功した場合はtrue
* ************************************************************************/
bool CoreCriticalSection::TryLock()
{
	return !!TryEnterCriticalSection((LPCRITICAL_SECTION)&_criticalSectionInfo);
}

/*!**********************************************************************
*  @brief     Lock関数でロックしたスレッドを解放し, 内部の変数を読み書き可能にします
*  @param[in] void
*  @return    void
*************************************************************************/
void CoreCriticalSection::Unlock()
{
	LeaveCriticalSection((LPCRITICAL_SECTION)&_criticalSectionInfo);
}


#pragma endregion Public Function