//////////////////////////////////////////////////////////////////////////////////
///  @file   CoreFileSystem.cpp
///  @brief  ファイルの読み書きや, ファイルが存在するかの確認, ディレクトリの作成や削除などの基本的なファイル操作の機能を提供します.
///  @author toide
///  @date   2024/05/11 19:14:45
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Core/Include/CoreFileSystem.hpp"
#include "Platform/Core/Include/CorePlatformMacros.hpp"
#include PLATFORM_COMPILED_HEADER(Platform, FileSystem.hpp)
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
*  @return    gu::SharedPointer<IFileSystem> ファイルシステムのインスタンス
*************************************************************************/
gu::SharedPointer<platform::core::IFileSystem> platform::core::IFileSystem::Get(const bool forceInitialize)
{
	static gu::SharedPointer<platform::core::IFileSystem> instance = nullptr;
	
	if (!instance || forceInitialize)
	{
		instance = gu::StaticPointerCast<platform::core::IFileSystem>
			      (gu::MakeShared<PLATFORM_CLASS(IFileSystem)>());
	}
	
	return instance;

}
#pragma endregion Public Function