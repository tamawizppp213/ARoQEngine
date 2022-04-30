//////////////////////////////////////////////////////////////////////////////////
///             @file   SceneManager.hpp
///             @brief  Scene Transition Manager
///             @author Toide Yutaro
///             @date   2022_04_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Scene.hpp"
#include <memory>
#include <stack>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class GameTimer;

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  			SceneManager
*************************************************************************//**
*  @class     SceneManager
*  @brief     Scene Transition Manager Class
*****************************************************************************/
class SceneManager
{
	using SceneUniquePtr = std::unique_ptr<Scene>;
	using ScenePtr       = Scene*;
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void StartUp(GameTimer* gameTimer);
	void TransitScene(ScenePtr scene, GameTimer* gameTimer);
	void CallSceneInitialize(GameTimer* gameTimer);
	void CallSceneUpdate();
	void CallSceneDraw();
	void CallSceneTerminate();
	void PushScene(ScenePtr scene);
	void PopScene();
	void ShutDown();
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	/*---------------------------------------------------------------------------
						Singleton
	-----------------------------------------------------------------------------*/
	static SceneManager& Instance()
	{
		static SceneManager sceneManager;
		return sceneManager;
	}

	// Prohibit move and copy
	SceneManager(const SceneManager&)            = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager(SceneManager&&)                 = delete;
	SceneManager& operator=(SceneManager&&)      = delete;
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	SceneManager () = default;
	~SceneManager() = default;
	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	std::stack<SceneUniquePtr> _currentScene;
	GameTimer* _gameTimer = nullptr;
};

#endif