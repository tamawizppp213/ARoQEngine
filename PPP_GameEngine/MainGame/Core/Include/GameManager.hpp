//////////////////////////////////////////////////////////////////////////////////
///             @file   GameManager.hpp
///             @brief  GameManager class
///             @author Toide Yutaro
///             @date   2022_0423
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "SceneManager.hpp"
#include "GraphicsCore/Engine/Include/GraphicsCoreEngine.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  			TemplateClass
*************************************************************************//**
*  @class     TemplateClass
*  @brief     temp
*****************************************************************************/
class GameManager final
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void GameStart(GameTimer& gameTimer, HWND hwnd);
	void GameMain();
	void GameEnd();

	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	void SetHWND(HWND hwnd);
	GraphicsCoreEngine& GetGraphicsEngine() { return _engine; }
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	/*---------------------------------------------------------------------------
	                 Singleton 
	-----------------------------------------------------------------------------*/
	static GameManager& Instance()
	{
		static GameManager gameManager;
		return gameManager;
	}

	// Prohibit move and copy.
	// Singleton answer (public or private): https://stackoverflow.com/questions/18931133/must-a-deleted-constructor-be-private
	GameManager(const GameManager&)            = delete;
	GameManager& operator=(const GameManager&) = delete;
	GameManager(GameManager&&)                 = delete;
	GameManager& operator=(GameManager&&)      = delete;
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	GameManager() = default;
	~GameManager() = default;

	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	SceneManager& _sceneManager = SceneManager::Instance();
	GraphicsCoreEngine& _engine = GraphicsCoreEngine::Instance();
	HWND _mainWindow = nullptr;
};
#endif