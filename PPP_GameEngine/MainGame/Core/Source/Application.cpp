//////////////////////////////////////////////////////////////////////////////////
//              @file   Application.cpp
///             @brief  Main Window
///             @author Toide Yutaro
///             @date   2022_04_13
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Core/Include/Application.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "Platform/Core/Include/CorePlatformApplication.hpp"
#include "Platform/Core/Include/CoreWindow.hpp"
#include "resource.h"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
static constexpr LPCWSTR CLASS_NAME = L"Game Window";
static constexpr LPCWSTR GAME_TITLE = L"PPP Engine";
static constexpr int GAME_WINDOW_WIDTH  = 1920;
static constexpr int GAME_WINDOW_HEIGHT = 1080;

using namespace platform::core;
using namespace engine::setting;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
bool Application::StartUp()
{
	_gameTimer = std::make_shared<GameTimer>();
	
	StartUpParameters engineParameter = {};
	engineParameter.WindowSettings.DesiredScreenWidth  = 1920;
	engineParameter.WindowSettings.DesiredScreenHeight = 1080;
	engineParameter.WindowSettings.DesiredScreenPositionX  = 960;
	engineParameter.WindowSettings.DesiredScreenPositionY  = 540;
	engineParameter.GraphicsSettings.APIversion            = _apiVersion;

	_gameManager.GameStart(engineParameter, _gameTimer);
	return true;
}

void Application::Run()
{
	const auto engine = _gameManager.GetEngine();

	MSG message = { NULL };

	_gameTimer->Reset();
	/*---------------------------------------------------------------
						Main Loop
	-----------------------------------------------------------------*/
	while (!engine->GetPlatformApplication()->IsQuit())
	{
		if (!engine->GetPlatformApplication()->PumpMessage())
		{
			_gameTimer->Tick();
			if (!_isApplicationPaused)
			{
				_gameTimer->AverageFrame(engine->GetWindow()->GetWindowHandle());
				_gameInput.Update();
				_gameManager.GameMain();
			}
		}
	}
}

void Application::ShutDown()
{
	_gameInput.Finalize();
	_gameManager.GameEnd();
}

#pragma region Private Function

#pragma endregion Private Function