//////////////////////////////////////////////////////////////////////////////////
//              @file   GameComponent.cpp
///             @brief  Object component
///             @author Toide Yutaro
///             @date   2022_04_15
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Core/Include/GameComponent.hpp"
#include "GameCore/Core/Include/GameObject.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
Component::Component()
{
	_owner       = nullptr;
	_updateOrder = DEFAULT_UPDATE_ORDER;
}

Component::Component(GameObject* gameObject, unsigned int updateOrder)
{
	_owner       = gameObject;
	_updateOrder = updateOrder;
}

