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
#include "GameCore/Rendering/Core/Renderer/Include/RenderPipeline.hpp"
#include "GameCore/Core/Include/Camera.hpp"

// input
#include "GameCore/Input/Include/GameInput.hpp"

// timer
#include "GameUtility/Base/Include/GameTimer.hpp"

#include "MainGame/Core/Include/GameManager.hpp"
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
					  Timer
	-----------------------------------------------------------------*/
	_mainThreadTimer = std::make_shared<GameTimer>();

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
				GameManager::Instance().GameDrawMain();
			}
		}

	}
	_isStoppedAllThreads.store(true);
}

void PPPEngine::ExecuteUpdateThread()
{
	while (!_isStoppedAllThreads)
	{
		
	}

	printf("update finish\n");
}

void PPPEngine::ExecuteRenderThread()
{
	while (!_isStoppedAllThreads)
	{
		if (!_renderPipeline) { continue; }
		/*_graphicsEngine->BeginDrawFrame();
		_renderPipeline->Draw();
		_graphicsEngine->EndDrawFrame();*/
	}

	printf("draw finish\n");
}

void PPPEngine::ShutDown()
{
	// スレッドの破棄
	_engineThreadManager.reset();

	// 入力デバイスの破棄
	GameInput::Instance().Finalize();
	
	// グラフィックエンジンの破棄
	_graphicsEngine->ShutDown();

}
#pragma endregion Main Function