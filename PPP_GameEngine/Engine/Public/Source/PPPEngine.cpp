//////////////////////////////////////////////////////////////////////////////////
//              @file   PPPEngine.cpp
///             @brief  各分野のエンジンをまとめ, スレッドを管理するクラス
///             @author Toide Yutaro
///             @date   2023_09_24
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/PPPEngine.hpp"

// platform
#include "Platform/Core/Include/CorePlatformApplication.hpp"
#include "Platform/Core/Include/CoreWindow.hpp"

// renderer
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"

// input
#include "GameCore/Input/Include/GameInput.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace engine::setting;
using namespace engine::core;
using namespace platform::core;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 

#pragma endregion Constructor and Destructor

#pragma region Main Function
void PPPEngine::StartUp(const StartUpParameters& setting)
{
	/*---------------------------------------------------------------
					  Platform Applicationの作成
	-----------------------------------------------------------------*/
	_platformApplication = PlatformApplication::Create(PlatformType::Windows);

	/*---------------------------------------------------------------
					  ウィンドウクラスの作成
	-----------------------------------------------------------------*/
	_mainWindow = _platformApplication->MakeWindow();
	_platformApplication->SetUpWindow(_mainWindow, setting.WindowSettings);

	if (!_mainWindow->Show()) { printf("failed to show window\n"); }

	/*---------------------------------------------------------------
					  レンダリングエンジンの作成
	-----------------------------------------------------------------*/
	_graphicsEngine = std::make_shared<LowLevelGraphicsEngine>();
	_graphicsEngine->StartUp(setting.GraphicsSettings.APIversion, _mainWindow->GetWindowHandle(), _platformApplication->GetInstanceHandle());

	/*---------------------------------------------------------------
					  Inputの作成
	-----------------------------------------------------------------*/
	GameInput::Instance().Initialize(_platformApplication->GetInstanceHandle(), _mainWindow->GetWindowHandle());

	/*---------------------------------------------------------------
					  スレッドの管理
	-----------------------------------------------------------------*/
	_engineThreadManager = std::make_unique<EngineThreadManager>();
}


void PPPEngine::ShutDown()
{
	// 入力デバイスの破棄
	GameInput::Instance().Finalize();
	
	// グラフィックエンジンの破棄
	_graphicsEngine->ShutDown();

	// スレッドの破棄
	_engineThreadManager.reset();

}
#pragma endregion Main Function