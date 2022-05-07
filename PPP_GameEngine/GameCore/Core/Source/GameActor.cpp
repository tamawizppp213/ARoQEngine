//////////////////////////////////////////////////////////////////////////////////
//              @file   GameActor.cpp
///             @brief  game actor
///             @author Toide Yutaro
///             @date   2022_04_16
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Core/Include/GameActor.hpp"
#include "GameCore/Core/Include/GameComponent.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
GameActor::GameActor() : GameObject()
{

}
GameActor::~GameActor()
{
	
}
#pragma endregion Constructor and Destructor
/****************************************************************************
*                       UpdateComponents
*************************************************************************//**
*  @fn        void GameActor::UpdateComponents(float deltaTime)
*  @brief     Update game components
*  @param[in] float deltaTime
*  @return @@void
*****************************************************************************/
void GameActor::UpdateComponents(float deltaTime)
{
	for (auto component : _components)
	{
		component->Update(deltaTime);
	}
}