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
#include "Platform//Core/Include/CorePlatformCommand.hpp"

// renderer
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GameCore/Rendering/Core/Renderer/Include/RenderPipeline.hpp"
#include "GameCore/Core/Include/Camera.hpp"

// input
#include "GameCore/Input/Include/GameInput.hpp"

// timer
#include "GameUtility/Base/Include/GameTimer.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"

#include "MainGame/Core/Include/GameManager.hpp"
#include "MainGame/Core/Include/SceneManager.hpp"
#include "Platform/Windows/Include/WindowsCursor.hpp"

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
PPPEngine::~PPPEngine()
{
	ShutDown();
}

#pragma endregion Constructor and Destructor

#pragma region Main Function
void PPPEngine::StartUp(const StartUpParameters& setting)
{
	_hasShutdown = false;

	StartUpParameter = setting;

	/*---------------------------------------------------------------
					  Timer
	-----------------------------------------------------------------*/
	_mainThreadTimer = gu::MakeShared<GameTimer>();

	/*---------------------------------------------------------------
					  Platform Applicationの作成
	-----------------------------------------------------------------*/
	_platformApplication = PlatformApplication::Create();

	/*---------------------------------------------------------------
					  ウィンドウクラスの作成
	-----------------------------------------------------------------*/
	_mainWindow = _platformApplication->MakeWindow();
	_platformApplication->SetUpWindow(_mainWindow, setting.WindowSettings);

	if (!_mainWindow->Show()) { printf("failed to show window\n"); }
	
	/*---------------------------------------------------------------
					  Platform commandクラスの作成
	-----------------------------------------------------------------*/
	_platformCommand = _platformApplication->MakeCommand();

	/*---------------------------------------------------------------
					  レンダリングエンジンの作成
	-----------------------------------------------------------------*/
	_graphicsEngine = gu::MakeShared<LowLevelGraphicsEngine>();
	_graphicsEngine->StartUp(StartUpParameter.GraphicsSettings.APIversion, _mainWindow->GetWindowHandle(), _platformApplication->GetInstanceHandle());

	/*---------------------------------------------------------------
					  Inputの作成
	-----------------------------------------------------------------*/
	GameInput::Instance().Initialize(_platformApplication->GetInstanceHandle(), _mainWindow->GetWindowHandle());

}

void PPPEngine::Run()
{
	/*---------------------------------------------------------------
					  スレッドの管理
	-----------------------------------------------------------------*/
	_engineThreadManager = gu::MakeShared<EngineThreadManager>();
	_isStoppedAllThreads.store(false); // falseを書き込み

	_engineThreadManager->GetUpdateMainThread()->Submit([&]() { this->ExecuteUpdateThread(); });
	_engineThreadManager->GetRenderMainThread()->Submit([&]() { this->ExecuteRenderThread(); });
}

/****************************************************************************
*                     ExecuteMainThread
*************************************************************************//**
*  @fn        void PPPEngine::ExecuteMainThread()
*
*  @brief     Message loopを入れてます.
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void PPPEngine::ExecuteMainThread()
{
	_mainThreadTimer->Reset();

	/*---------------------------------------------------------------
						Main Loop
	-----------------------------------------------------------------*/
	while (!_platformApplication->IsQuit())
	{
		if (!_platformApplication->PumpMessage())
		{
			_mainThreadTimer->Tick();
			if (!_isApplicationPaused)
			{
				_mainThreadTimer->AverageFrame(_mainWindow->GetWindowHandle());
				GameInput::Instance().Update();
				// 現状シングルスレッドで.
				GameManager::Instance().GameUpdateMain();
			}
		}

	}
	_isStoppedAllThreads.store(true);

	// 全てのスレッドに対する実行完了待ち
	_engineThreadManager->ShutDown();

}

void PPPEngine::ExecuteUpdateThread()
{
	while (!_isStoppedAllThreads)
	{
		
	}

	_engineThreadManager->CallExecuteComplete(ThreadPoolType::UpdateMain);
	printf("update finish\n");
}

/****************************************************************************
*                     ExecuteRenderThread
*************************************************************************//**
*  @fn        void PPPEngine::ExecuteRenderThread()
*
*  @brief     描画ループです
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void PPPEngine::ExecuteRenderThread()
{
	/*---------------------------------------------------------------
					  描画ループ
	-----------------------------------------------------------------*/
	while (!_isStoppedAllThreads)
	{
		// 開始処理 : コマンドバッファをOpenにする
		_graphicsEngine->BeginDrawFrame();

		// 初期レンダーパスの設定
		_graphicsEngine->BeginSwapchainRenderPass();
		
		// シーンの描画
		GameManager::Instance().GameDrawMain();

		// 終了処理, 描画コマンドを実行する
		_graphicsEngine->EndDrawFrame();
	}


	_engineThreadManager->CallExecuteComplete(ThreadPoolType::RenderMain);
	printf("draw finish\n");
}

void PPPEngine::ShutDown()
{
	if (_hasShutdown) { return; }

	// スレッドの破棄
	_engineThreadManager.Reset();

	// 入力デバイスの破棄
	GameInput::Instance().Finalize();
	
	// グラフィックエンジンの破棄
	_graphicsEngine->ShutDown();


	_mainWindow.Reset();
	_platformCommand.Reset();
	_platformApplication.Reset();

}
#pragma endregion Main Function