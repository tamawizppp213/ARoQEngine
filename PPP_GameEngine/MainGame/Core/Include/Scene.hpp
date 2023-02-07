//////////////////////////////////////////////////////////////////////////////////
///             @file   TemplateText.hpp
///             @brief  TemplateText
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SCENE_HPP
#define SCENE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GameCore/Input/Include/GameInput.hpp"
#include "GameUtility/Base/Include/GameTimer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Scene class
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			 Scene
*************************************************************************//**
*  @class     Scene
*  @brief     Scene
*****************************************************************************/
class Scene
{
protected:
	using GameTimerPtr = std::shared_ptr<GameTimer>;
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	virtual void Initialize(const std::shared_ptr<LowLevelGraphicsEngine>& engine, const GameTimerPtr& gameTimer);
	virtual void Update   ();
	virtual void Draw     () = 0;
	virtual void Terminate() = 0;
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	Scene();
	virtual ~Scene();
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/
	virtual void LoadMaterials() = 0;
	virtual void OnKeyboardInput() {};
	virtual void OnMouseInput   () {};
	virtual void OnGamePadInput () {};
	virtual void ExecuteSceneTransition(){};
	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	std::shared_ptr<LowLevelGraphicsEngine> _engine;
	GameInput& _gameInput            = GameInput::Instance();
	std::shared_ptr<GameTimer> _gameTimer  = nullptr;
	bool _hasExecutedSceneTransition = false;
	bool _hasExecutedBackScene       = false;

};
#endif