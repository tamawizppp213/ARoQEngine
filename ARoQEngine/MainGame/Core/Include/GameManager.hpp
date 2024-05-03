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
#include "Engine/Public/Include/PPPEngine.hpp"
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
	void GameStart(const engine::setting::StartUpParameters& parameters);
	void GameUpdateMain();
	void GameDrawMain();
	void GameEnd();

	/****************************************************************************
	**                Public Property
	*****************************************************************************/
	gu::SharedPointer<PPPEngine> GetEngine() const { return _engine; }
	gu::SharedPointer<LowLevelGraphicsEngine> GetGraphicsEngine() { return _engine->GetLowLevelGraphics(); }
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
	**                Private Property
	*****************************************************************************/
	SceneManager& _sceneManager = SceneManager::Instance();
	gu::SharedPointer<PPPEngine> _engine = nullptr;
	HWND _mainWindow = nullptr;
};
#endif