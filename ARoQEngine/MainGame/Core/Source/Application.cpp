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
using namespace engine;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
bool Application::StartUp()
{
	StartUpParameters engineParameter = {};
	engineParameter.WindowSettings.DesiredScreenWidth  = 1920;
	engineParameter.WindowSettings.DesiredScreenHeight = 1080;
	engineParameter.WindowSettings.DesiredScreenPositionX  = 960;
	engineParameter.WindowSettings.DesiredScreenPositionY  = 540;
	engineParameter.GraphicsSettings.APIversion            = _apiVersion;

	_gameManager.GameStart(engineParameter);
	return true;
}

void Application::Run()
{
	_gameManager.GetEngine()->ExecuteMainThread();
}

void Application::ShutDown()
{
	_gameManager.GameEnd();
}

#pragma region Private Function

#pragma endregion Private Function