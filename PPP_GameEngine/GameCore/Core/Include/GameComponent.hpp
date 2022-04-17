//////////////////////////////////////////////////////////////////////////////////
///             @file   GameComponent.hpp
///             @brief  GameObject Component
///             @author Toide Yutaro
///             @date   2022_04_15
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GAME_COMPONENT_HPP
#define GAME_COMPONENT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class GameObject;
//////////////////////////////////////////////////////////////////////////////////
//                          Component
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			TemplateClass
*************************************************************************//**
*  @class     TemplateClass
*  @brief     temp
*****************************************************************************/
class Component
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	virtual void Update(float deltaTime) = 0;
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	inline unsigned int GetUpdateOrder() const { return _updateOrder; }
	inline bool ExistsOwner() { return _owner != nullptr; }
	inline void SetOwner(GameObject* gameObject) { _owner = gameObject; }
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	Component();
	Component(GameObject* gameObject, unsigned int updateOrder = DEFAULT_UPDATE_ORDER);
	virtual ~Component() {};
	Component(const Component&) = default;
	Component& operator=(const Component&) = default;
	Component(Component&&)                 = default;
	Component& operator=(Component&&)      = default;
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	enum {DEFAULT_UPDATE_ORDER = 100};
	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	GameObject* _owner          = nullptr;
	unsigned int _updateOrder   = DEFAULT_UPDATE_ORDER; // The smaller its value, the faster the update order.
};
#endif