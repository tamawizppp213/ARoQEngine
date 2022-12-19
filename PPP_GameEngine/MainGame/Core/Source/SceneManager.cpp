//////////////////////////////////////////////////////////////////////////////////
//              @file   Scene.cpp
///             @brief  Scene
///             @author Toide Yutaro
///             @date   2022_04_14
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Core/Include/SceneManager.hpp"
#include "GameUtility/Base/Include/GameTimer.hpp"
#include "MainGame/Core/Include/Scene.hpp"
#include "MainGame/Sample/Include/SampleColorChange.hpp"
#include "MainGame/Sample/Include/SampleTexture.hpp"
#include "MainGame/Sample/Include/SampleText.hpp"
#include "MainGame/Sample/Include/SampleRayTracingRectangle.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
void SceneManager::StartUp(LowLevelGraphicsEnginePtr engine, GameTimer* gameTimer)
{
	_engine = engine;
	PushScene(new sample::SampleText());
	CallSceneInitialize(gameTimer);
}
/****************************************************************************
*                       TransitScene
*************************************************************************//**
*  @fn        void SceneManager::TransitScene(const ScenePtr scene, GameTimer* gameTimer)
*  @brief     Transit scene
*  @param[in] ScenePtr scene
*  @param[in] GameTimer& gameTimer
*  @return 　　void
*****************************************************************************/
void SceneManager::TransitScene(ScenePtr scene, GameTimer* gameTimer)
{
	CallSceneTerminate();
	_currentScene.pop();
	_currentScene.emplace(std::move(scene));
	CallSceneInitialize(gameTimer);
}

void SceneManager::CallSceneInitialize( GameTimer* gameTimer)
{
	if (_currentScene.empty()) { return; }
	_currentScene.top()->Initialize(_engine, gameTimer);
	//GraphicsCoreEngine::Instance().OnInitializeRenderScene();
}
void SceneManager::CallSceneUpdate()
{
	_currentScene.top()->Update();
}
void SceneManager::CallSceneDraw()
{
	_currentScene.top()->Draw();
}
void SceneManager::CallSceneTerminate()
{
	if (_currentScene.empty()) { return; }
	_currentScene.top()->Terminate();
}
void SceneManager::PushScene(ScenePtr scene)
{
	_currentScene.emplace(scene);
}
void SceneManager::PopScene()
{
	_currentScene.pop();
}
/****************************************************************************
*                       ShutDown
*************************************************************************//**
*  @fn        void SceneManger::ShutDown()
*  @brief     Shut Down
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SceneManager::ShutDown()
{
	while (!_currentScene.empty())
	{
		_currentScene.top().reset();
		_currentScene.pop();
	}
}