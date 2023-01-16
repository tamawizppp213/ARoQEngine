//////////////////////////////////////////////////////////////////////////////////
///             @file   TemplateText.hpp
///             @brief  TemplateText
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GAME_ACTOR_HPP
#define GAME_ACTOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameObject.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  			GameActor
*************************************************************************//**
*  @class     GameActor
*  @brief     Updatable object
*****************************************************************************/
class GameActor : public gc::core::GameObject
{
protected:
	using SceneGPUAddress = uint64_t;
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	virtual void Update(float deltaTime) = 0;

	virtual void Draw(SceneGPUAddress address) = 0;
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	GameActor();
	GameActor(const GameActor&)            = default;
	GameActor& operator=(const GameActor&) = default;
	GameActor(GameActor&&)                 = default;
	GameActor& operator=(GameActor&&)      = default;
	~GameActor();
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/
	void UpdateComponents(float deltaTime);

	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
};
#endif