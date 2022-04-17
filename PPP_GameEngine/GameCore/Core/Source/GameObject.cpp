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
static constexpr int INVALID_VALUE = -1;

//////////////////////////////////////////////////////////////////////////////////
//                   Implement
//////////////////////////////////////////////////////////////////////////////////
std::vector<GameObject*> GameObject::_gameObjects;
std::vector<GameObject*> GameObject::_destroyGameObjects;
std::vector<std::wstring> GameObject::_layerList;
bool GameObject::_isUpdating = false;


GameObject::GameObject()
{
	_gameObjects.emplace_back(this);
	_tag     = L"";
	_name    = L"";
	_isActive = true;
	_parent = nullptr;

}

GameObject::~GameObject()
{
	_components.clear(); _components.shrink_to_fit();
}

/****************************************************************************
*                          Find
*************************************************************************//**
*  @fn        GameObject* GameObject::Find(const std::string& name)
*  @brief     This function returns the gameObject with the same name as the assign name.
*  @param[in] std::string name
*  @return 　　GameObject*
*****************************************************************************/
GameObject* GameObject::Find(const std::wstring& name)
{
	for (auto it = _gameObjects.begin(); it != _gameObjects.end(); ++it)
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
*  @brief     This function returns the gameObject list with the same tag as the assign tag.
*  @param[in] std::string tag
*  @return 　　std::vector<GameObject*>
*****************************************************************************/
std::vector<GameObject*> GameObject::GameObjectsWithTag(const std::wstring& tag)
{
	std::vector<GameObject*> gameObjects;
	for (auto it = _gameObjects.begin(); it != _gameObjects.end(); ++it)
	{
		if ((*it)->GetTag() == tag)
		{
			gameObjects.emplace_back((*it));
		}
	}
	return gameObjects; // Return Value Optimization (C++17)
}

#pragma region Destroy
/****************************************************************************
*                          Destroy
*************************************************************************//**
*  @fn        GameObject* GameObject::Destroy(GameObject* gameObject)
*  @brief     This function return the gameObject with the same name as the assign name.
*  @param[in] std::string name
*  @return 　　GameObject*
*****************************************************************************/
bool GameObject::Destroy(GameObject* gameObject)
{
	if (gameObject == nullptr) { return false; }

	if (_isUpdating)
	{
		_destroyGameObjects.emplace_back(gameObject);
		gameObject->SetActive(false);
	}
	else
	{
		for (auto it = _gameObjects.begin(); it != _gameObjects.end(); ++it)
		{
			if ((*it) == gameObject)
			{
				std::swap(*it, _gameObjects.back());
				_gameObjects.pop_back();
				_gameObjects.shrink_to_fit();         // memory free
				delete gameObject;                    // delete object
				return true;
			}
		}
	}

	return false;
}

/****************************************************************************
*                          DestroyImmediate
*************************************************************************//**
*  @fn        GameObject* GameObject::DestroyImmediate(GameObject* gameObject)
*  @brief     This function return the gameObject with the same name as the assign name.
*  @param[in] std::string name
*  @return 　　GameObject*
*****************************************************************************/
bool GameObject::DestroyImmediate(GameObject* gameObject)
{
	if (gameObject == nullptr) { return false; }

	for (auto it = _gameObjects.begin(); it != _gameObjects.end(); ++it)
	{
		if (*it == gameObject)
		{
			std::swap(*it, _gameObjects.back());
			_gameObjects.pop_back();
			_gameObjects.shrink_to_fit();         // memory free
			delete gameObject;                    // delete object
			return true;
		}
	}

	return false;
}

/****************************************************************************
*                          DestroyAllTagObject
*************************************************************************//**
*  @fn        GameObject* GameObject::DestroyAllTagObject(const std::string& tag)
*  @brief     This function destroys all objects with the tag
*  @param[in] std::string tag
*  @return 　　void
*****************************************************************************/
void GameObject::DestroyAllTagObjects(const std::wstring& tag)
{

	for (auto it = _gameObjects.begin(); it != _gameObjects.end(); ++it)
	{
		if ((*it)->GetTag() == tag)
		{
			if (_isUpdating)
			{
				_destroyGameObjects.emplace_back(std::move(*it));
				(*it)->SetActive(false);
			}
			else
			{
				_gameObjects.erase(it);
				delete* it;
			}
		}
	}
	_gameObjects.shrink_to_fit();
}

/****************************************************************************
*                          DestroyWithChildren
*************************************************************************//**
*  @fn        void GameObject::DestroyWithChildren(GameObject* gameObject)
*  @brief     This function destroys gameObjects including child objects.
*  @param[in] GameObject* child
*  @return 　　GameObject*
*****************************************************************************/
void GameObject::DestroyWithChildren(GameObject* gameObject)
{
	if (gameObject == nullptr) { return; }
	for (int i = gameObject->GetChildCount() - 1; i >= 0; --i)
	{
		GameObject* child = gameObject->GetChild(i);
		DestroyWithChildren(child);
	}
	Destroy(gameObject);
}
/****************************************************************************
*                          ClearDestroyObjects
*************************************************************************//**
*  @fn        void GameObject::ClearDestroyObjects()
*  @brief     Destroy all game objects.
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GameObject::ClearDestroyObjects()
{
	for (auto it = _destroyGameObjects.begin(); it != _destroyGameObjects.end(); ++it)
	{
		Destroy(*it);
	}
	_destroyGameObjects.clear();
	_destroyGameObjects.shrink_to_fit();
}
#pragma endregion Destroy

/****************************************************************************
*                          RemoveChild
*************************************************************************//**
*  @fn        bool GameObject::RemoveChild(GameObject* child)
*  @brief     This function removes child gameObject
*  @param[in] GameObject* child
*  @return 　　GameObject*
*****************************************************************************/
bool GameObject::RemoveChild(GameObject* child)
{
	for (auto it = _children.begin(); it != _children.end(); ++it)
	{
		if (*it == child)
		{
			_children.erase(it);
			return true;
		}
	}
	return false;
}
/****************************************************************************
*                          ClearChildren
*************************************************************************//**
*  @fn        bool GameObject::ClearChildren()
*  @brief     This function clears children object.
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GameObject::ClearChildren()
{
	_children.clear();
	std::vector<GameObject*>().swap(_children); // memory free

}
/****************************************************************************
*                          ClearAllGameObjects
*************************************************************************//**
*  @fn        bool GameObject::ClearAllGameObjects()
*  @brief     Clear all game objects.
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool GameObject::ClearAllGameObjects()
{
	_gameObjects.clear();
	_gameObjects.shrink_to_fit();
	return true;
}
/****************************************************************************
*                          AddComponent
*************************************************************************//**
*  @fn        void GameObject::AddComponent(Component* component)
*  @brief     Add GameComponent
*  @param[in] Component* component
*  @return 　　void
*****************************************************************************/
void GameObject::AddComponent(Component* component)
{
	/*-------------------------------------------------------------------
	-        Find insert position in order to decide update order.
	---------------------------------------------------------------------*/
	int myOrder   = component->GetUpdateOrder();
	auto iterator = _components.begin();
	for (; iterator != _components.end(); ++iterator)
	{
		if (myOrder < (*iterator)->GetUpdateOrder()) { break; }
	}
	_components.insert(iterator, component);
	/*-------------------------------------------------------------------
	-              Regist gameobject to component
	---------------------------------------------------------------------*/
	if (!component->ExistsOwner()) { component->SetOwner(this); }
}
/****************************************************************************
*                          RemoveComponent
*************************************************************************//**
*  @fn        void GameObject::RemoveComponent(Component* component)
*  @brief     Remove GameComponent. (Erase same component )
*  @param[in] Component* component
*  @return 　　void
*****************************************************************************/
void GameObject::RemoveComponent(Component* component)
{
	auto iterator = std::find(_components.begin(), _components.end(), component);
	if (iterator != _components.end()) 
	{
		_components.erase(iterator);
	}
}
#pragma region Private Function
int GameObject::GetLayerBit(const std::wstring& layer)
{
	if (layer == L"") { return INVALID_VALUE; }
	for (int i = 0; i < 31; ++i) // 4byte
	{
		if (_layerList[i] == layer)
		{
			return i;
		}
	}
	return INVALID_VALUE;
}

#pragma endregion Private Function