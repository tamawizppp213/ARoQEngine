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
#include "GraphicsCore/Engine/Include/GraphicsCoreEngine.hpp"
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
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	virtual void Initialize(GameTimer* gameTimer);
	virtual void Update   ();
	virtual void Draw     () = 0;
	virtual void Terminate() = 0;
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	Scene() = default;
	virtual ~Scene() = default;
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/
	virtual void LoadMaterials(GameTimer* gameTimer) = 0;
	virtual void OnKeyboardInput() {};
	virtual void OnMouseInput   () {};
	virtual void OnGamePadInput () {};
	virtual void ExecuteSceneTransition(){};
	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	GraphicsCoreEngine& _engine      = GraphicsCoreEngine::Instance();
	GameInput& _gameInput            = GameInput::Instance();
	GameTimer* _gameTimer            = nullptr;
	bool _hasExecutedSceneTransition = false;
	bool _hasExecutedBackScene       = false;

};
#endif