//////////////////////////////////////////////////////////////////////////////////
//              @file   GameManager.cpp
///             @brief  game manager class
///             @author Toide Yutaro
///             @date   2022_04_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Core/Include/GameManager.hpp"
#include "GameCore/Core/Include/GameObject.hpp"
#include "GameCore/Core/Include/ResourceManager.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;
using namespace gc::core;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
void GameManager::GameStart(APIVersion apiVersion, GameTimer& gameTimer, HWND hwnd, HINSTANCE hInstance)
{
	_engine = std::make_shared<LowLevelGraphicsEngine>();
	_engine->StartUp(apiVersion, hwnd, hInstance);
	_sceneManager.StartUp(_engine, &gameTimer);
}

void GameManager::GameMain()
{
	_sceneManager.CallSceneUpdate();
	_sceneManager.CallSceneDraw();
}

void GameManager::GameEnd()
{
	_sceneManager.CallSceneTerminate();
	_sceneManager.ShutDown();
	ResourceManager::Instance().ClearAllResources();
	GameObject::ClearAllGameObjects();
	_engine->ShutDown();
}

void GameManager::SetHWND(HWND hwnd)
{
	_mainWindow = hwnd;
}