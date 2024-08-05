//////////////////////////////////////////////////////////////////////////////////
///  @file   CoreProcess.cpp
///  @brief  OSのプロセスを操作するクラス
///  @author toide
///  @date   2024/06/20 23:46:21
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Core/Include/CoreProcess.hpp"
#include "Platform/Core/Include/CorePlatformMacros.hpp"
#include PLATFORM_COMPILED_HEADER(Platform, Process.hpp)
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     シングルトンインスタンスを取得します. (基本的にメンバ変数を使わないため, 特に悪影響はありません)
*  @param[in] const bool 強制的に初期化を行います. (Default: false)
*  @return    gu::SharedPointer<IProcess> プロセスのインスタンス
* ************************************************************************/
gu::SharedPointer<platform::core::CoreProcess> platform::core::CoreProcess::Get(const bool forceInitialize)
{
	static gu::SharedPointer<platform::core::CoreProcess> instance = nullptr;
	
	if (!instance || forceInitialize)
	{
		instance = gu::StaticPointerCast<platform::core::CoreProcess>
			      (gu::MakeShared<PLATFORM_CLASS(CoreProcess)>());
	}
	
	return instance;

}
#pragma endregion Public Function