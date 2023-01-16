/////////////////////////////////////////////////////////////////////////////////
//              Title:  GameObject.hpp
//            Content:  GameObject.hpp
//             Author:  Toide Yutaro
//             Create:  2021_07_30
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Core/Include/GameObject.hpp"
#include "GameCore/Core/Include/GameComponent.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::core;
static constexpr int INVALID_VALUE = -1;

//////////////////////////////////////////////////////////////////////////////////
//                   Implement
//////////////////////////////////////////////////////////////////////////////////
namespace gc::core
{
	std::vector<GameObject::GameObjectPtr> GameObject::GameObjects = {};
	std::vector<std::wstring> GameObject::LayerList = {};
}

#pragma region Constructor and Destructor 
GameObject::GameObject()
{
	GameObjects.emplace_back(shared_from_this());
	_tag     = L"";
	_name    = L"";
	_isActive = true;
	_parent = nullptr;

}

GameObject::~GameObject()
{
	_children.clear();
	_children.shrink_to_fit();
}

#pragma endregion Constructor and Destructor

#pragma region Find Function 
/****************************************************************************
*                          Find
*************************************************************************//**
*  @fn        GameObject* GameObject::Find(const std::string& name)
* 
*  @brief     This function returns the gameObject with the same name as the assign name.
* 
*  @param[in] std::string name
* 
*  @return 　　GameObject*
*****************************************************************************/
GameObject::GameObjectPtr GameObject::Find(const std::wstring& name)
{
	for (auto it = GameObjects.begin(); it != GameObjects.end(); ++it)
	{
		if ((*it)->_name == name)
		{
			return (*it);
		}
	}
	return nullptr; // Failed to find
}

/****************************************************************************
*                          GameObjectsWithTag
*************************************************************************//**
*  @fn        std::vector<GameObject*> GameObject::GameObjectsWithTag(const std::string& tag)
* 
*  @brief     This function returns the gameObject list with the same tag as the assign tag.
* 
*  @param[in] std::string tag
* 
*  @return 　　std::vector<GameObject*>
*****************************************************************************/
std::vector<GameObject::GameObjectPtr> GameObject::GameObjectsWithTag(const std::wstring& tag)
{
	std::vector<GameObjectPtr> gameObjects = {};

	for (auto it = GameObjects.begin(); it != GameObjects.end(); ++it)
	{
		if ((*it)->GetTag() == tag)
		{
			gameObjects.emplace_back((*it));
		}
	}

	return gameObjects; // Return Value Optimization (C++17)
}

#pragma endregion Find Function

#pragma region Destroy
/****************************************************************************
*                          Destroy
*************************************************************************//**
*  @fn        GameObject* GameObject::Destroy(GameObject* gameObject)
* 
*  @brief     (Safe delete ) This function destroy the game object with the same assigned object
* 
*  @param[in,out] GameObjectPtr& gameObject -> set nullptr
* 
*  @return 　　bool (true: success destroy including the object to be destroyed after updating, false: failed to destroy)
*****************************************************************************/
bool GameObject::Destroy(GameObjectPtr& gameObject)
{
	if (gameObject == nullptr) { return false; }

	const auto foundCount = std::erase(GameObjects, gameObject);
	if (foundCount == 0) { return false; }
	gameObject.reset();

	return true;
}

/****************************************************************************
*                          DestroyAllTagObject
*************************************************************************//**
*  @fn        void GameObject::DestroyAllTagObject(const std::string& tag)
* 
*  @brief     This function destroys all objects with the tag
* 
*  @param[in] std::string tag
* 
*  @return 　　void
*****************************************************************************/
void GameObject::DestroyAllTagObjects(const std::wstring& tag)
{
	std::erase_if(GameObjects,[&](const GameObjectPtr& gameObject) 
	{
		return gameObject->GetTag() == tag; 
	});
}

/****************************************************************************
*                          DestroyWithChildren
*************************************************************************//**
*  @fn        void GameObject::DestroyWithChildren(GameObjectPtr& gameObject)
* 
*  @brief     This function destroys gameObjects including child objects.
* 
*  @param[in] GameObjectPtr& child
* 
*  @return 　　void
*****************************************************************************/
void GameObject::DestroyWithChildren(GameObjectPtr& gameObject)
{
	if (gameObject == nullptr) { return; }
	for (int i = gameObject->GetChildCount() - 1; i >= 0; --i)
	{
		GameObjectPtr child = gameObject->GetChild(i);
		DestroyWithChildren(child);
	}
	Destroy(gameObject);
}

/****************************************************************************
*                          RemoveChild
*************************************************************************//**
*  @fn        bool GameObject::RemoveChild(GameObject* child)
*
*  @brief     This function removes child gameObject
*
*  @param[in] GameObject* child -> set nullptr
*
*  @return 　　bool
*****************************************************************************/
bool GameObject::RemoveChild(GameObjectPtr& child)
{
	for (auto it = _children.begin(); it != _children.end(); ++it)
	{
		if (*it == child)
		{
			_children.erase(it);
			child = nullptr;
			return true;
		}
	}
	return false;
}

/****************************************************************************
*                          ClearChildren
*************************************************************************//**
*  @fn        bool GameObject::ClearChildren()
*
*  @brief     This function clears children object.
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void GameObject::ClearChildren()
{
	_children.clear();
	_children.shrink_to_fit();
}

/****************************************************************************
*                          ClearAllGameObjects
*************************************************************************//**
*  @fn        bool GameObject::ClearAllGameObjects()
*
*  @brief     Clear all game objects.
*
*  @param[in] void
*
*  @return 　　bool
*****************************************************************************/
void GameObject::ClearAllGameObjects()
{
	GameObjects.clear();
	GameObjects.shrink_to_fit();
}

#pragma endregion Destroy Function

#pragma region Component Function 
///****************************************************************************
//*                          AddComponent
//*************************************************************************//**
//*  @fn        void GameObject::AddComponent(Component* component)
//*  @brief     Add GameComponent
//*  @param[in] Component* component
//*  @return 　　void
//*****************************************************************************/
//void gc::core::GameObject::AddComponent(Component* component)
//{
//	/*-------------------------------------------------------------------
//	-        Find insert position in order to decide update order.
//	---------------------------------------------------------------------*/
//	unsigned int myOrder   = component->GetUpdateOrder();
//	auto iterator = _components.begin();
//	for (; iterator != _components.end(); ++iterator)
//	{
//		if (myOrder < (*iterator)->GetUpdateOrder()) { break; }
//	}
//	_components.insert(iterator, component);
//	/*-------------------------------------------------------------------
//	-              Regist gameobject to component
//	---------------------------------------------------------------------*/
//	if (!component->ExistsOwner()) { component->SetOwner(this); }
//}
///****************************************************************************
//*                          RemoveComponent
//*************************************************************************//**
//*  @fn        void GameObject::RemoveComponent(Component* component)
//*  @brief     Remove GameComponent. (Erase same component )
//*  @param[in] Component* component
//*  @return 　　void
//*****************************************************************************/
//void gc::core::GameObject::RemoveComponent(Component* component)
//{
//	auto iterator = std::find(_components.begin(), _components.end(), component);
//	if (iterator != _components.end()) 
//	{
//		_components.erase(iterator);
//	}
//}
#pragma endregion Component
#pragma region Private Function
int gc::core::GameObject::GetLayerBit(const std::wstring& layer)
{
	if (layer == L"") { return INVALID_VALUE; }
	for (int i = 0; i < 31; ++i) // 4byte
	{
		if (LayerList[i] == layer)
		{
			return i;
		}
	}
	return INVALID_VALUE;
}

#pragma endregion Private Function