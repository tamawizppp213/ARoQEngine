//////////////////////////////////////////////////////////////////////////////////
///             @file   GameObject.hpp
///             @brief  GameObjet Class
///             @author Toide Yutaro
///             @date   2022_03_15
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Math/Include/GMTransform.hpp"
#include <string>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//								Class 
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  			GameObject
*************************************************************************//**
*  @class    GameObject
*  @brief    GameObject (base class in the all object class)
*****************************************************************************/
class GameObject
{
public:
	/****************************************************************************
	**                Static Function
	*****************************************************************************/
	/*-------------------------------------------------------------------
	-                       Find
	---------------------------------------------------------------------*/
	static GameObject* Find(const std::wstring& name);
	static std::vector<GameObject*> GameObjectsWithTag(const std::wstring& tag);
	static size_t CountGameObjects(){return _gameObjects.size(); }
	/*-------------------------------------------------------------------
	-               Destroy and Clear
	---------------------------------------------------------------------*/
	static bool Destroy(GameObject* gameObject);
	static bool DestroyImmediate(GameObject* gameObject);
	static void DestroyWithChildren(GameObject* parent);
	static void DestroyAllTagObjects(const std::wstring& tag);
	static bool ClearAllGameObjects();
	static void ClearDestroyObjects();
	/*-------------------------------------------------------------------
	-               GameObject List
	---------------------------------------------------------------------*/
	static GameObject* GetGameObjectList(int index) { return _gameObjects[index]; }
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	template <typename T>T* Create();

	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	/*-------------------------------------------------------------------
	-               GameObject Transform
	---------------------------------------------------------------------*/
	      gm::Transform & GetTransform()       { return _transform; }
	const gm::Transform & GetTransform() const { return _transform; }
	      gm::Vector3   & GetPosition ()       { return _transform.LocalPosition; }
	const gm::Vector3   & GetPosition () const { return _transform.LocalPosition; }
	      gm::Quaternion& GetRotation ()       { return _transform.LocalRotation; }
	const gm::Quaternion& GetRotation () const { return _transform.LocalRotation; }
	      gm::Vector3   & GetScale    ()       { return _transform.LocalScale; }
	const gm::Vector3   & GetScale    () const { return _transform.LocalScale; }

	void SetPosition(float x, float y, float z)      { _transform.LocalPosition = gm::Vector3(x, y, z); }
	void SetPosition(const gm::Vector3& position)    { _transform.LocalPosition = position; }
	void SetScale   (float x, float y, float z)      { _transform.LocalScale    = gm::Vector3(x, y, z); }
	void SetScale   (const gm::Vector3& scale)       { _transform.LocalScale    = scale; }
	void SetRotation(const gm::Quaternion& rotation) { _transform.LocalRotation = rotation; }
	/*-------------------------------------------------------------------
	-               GameObject Default Infomation
	---------------------------------------------------------------------*/
	std::wstring GetName     () const { return _name; }
	std::wstring GetTag      () const { return _tag; }
	std::wstring GetLayerName() const { return _layerList[_layer]; }
	bool         IsActive    () const { return _isActive; }

	void SetName(const std::wstring& name) { _name = name; }
	void SetTag (const std::wstring& name) { _tag = name; }
	void SetLayer(const std::wstring& name) { int bit = GetLayerBit(name); if (bit >= 0) { _layer = (1 << bit); } }
	void SetActive(bool isActive) { _isActive = isActive; }
	/*-------------------------------------------------------------------
	-               GameObject Child
	---------------------------------------------------------------------*/
	GameObject* GetChild   (int index) { return  (index < _children.size()) ? _children[index] : nullptr; }
	bool        RemoveChild(GameObject* child);
	void        ClearChildren();
	int  GetChildCount() const { return static_cast<int>(_children.size()); }
	
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	GameObject();
	GameObject(const GameObject&)            = default;
	GameObject& operator=(const GameObject&) = default;
	GameObject(GameObject&&)                 = default;
	GameObject& operator=(GameObject&&)      = default;
	~GameObject();
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/

	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	/*-------------------------------------------------------------------
	-           gameObject default info
	---------------------------------------------------------------------*/
	std::wstring _name;
	std::wstring _tag;
	int          _layer;
	/*-------------------------------------------------------------------
	-           gameObject state
	---------------------------------------------------------------------*/
	bool         _isActive;
	/*-------------------------------------------------------------------
	-           gameObject Config
	---------------------------------------------------------------------*/
	gm::Transform _transform;
	/*-------------------------------------------------------------------
	-           parent tree
	---------------------------------------------------------------------*/
	GameObject* _parent;
	std::vector<GameObject*> _children;
	static bool _isUpdating;
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	int GetLayerBit(const std::wstring& layer);

	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	static std::vector<GameObject*> _gameObjects;
	static std::vector<GameObject*> _destroyGameObjects;
	static std::vector<std::wstring> _layerList;
};

template<typename T> T* GameObject::Create()
{
	return new GameObject();
}
#endif