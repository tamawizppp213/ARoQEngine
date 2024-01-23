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
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
GameActor::GameActor(const LowLevelGraphicsEnginePtr& engine) : GameObject(engine)
{
	_type = ObjectType::Actor;
}
GameActor::~GameActor()
{
	
}
#pragma endregion Constructor and Destructor
#pragma region Main Function
/****************************************************************************
*                       GameActor
*************************************************************************//**
*  @fn        void GameActor::Update(const float deltaTime, const bool enableUpdateChild)
*
*  @brief     Update
*
*  @param[in] float deltaTime
*  @param[in] bool enableUpdateChild
*
*  @return    void
*****************************************************************************/
void GameActor::Update(const float deltaTime, const bool enableUpdateChild)
{
	if (!_isActive) { return; }
	if (enableUpdateChild) { UpdateChild(deltaTime); }
}

#pragma endregion Main Function

/****************************************************************************
*                       UpdateComponents
*************************************************************************//**
*  @fn        void GameActor::UpdateComponents(float deltaTime)
*  @brief     Update game components
*  @param[in] float deltaTime
*  @return @@void
*****************************************************************************/
void GameActor::UpdateComponents([[maybe_unused]]float deltaTime)
{
	/*for (auto component : _components)
	{
		component->Update(deltaTime);
	}*/
}

/****************************************************************************
*                       UpdateChild
*************************************************************************//**
*  @fn        void GameActor::UpdateChild(const float deltaTime)
* 
*  @brief     Update child 
* 
*  @param[in] float deltaTime
* 
*  @return    void
*****************************************************************************/
void GameActor::UpdateChild(const float deltaTime)
{
	for (const auto& child : _children)
	{
		if (child->GetType() == ObjectType::Static) { continue; }

		const auto actorChild = gu::StaticPointerCast<GameActor>(child);
		actorChild->Update(deltaTime, true);
	}
}