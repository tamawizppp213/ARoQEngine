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
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GameUtility/Math/Include/GMTransform.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include <vector>
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class Component;
class LowLevelGraphicsEngine;
//////////////////////////////////////////////////////////////////////////////////
//								Class 
//////////////////////////////////////////////////////////////////////////////////

namespace gc::core
{
	class GameObject;
	enum class ObjectType
	{
		Static,
		Actor,
		Model,
	};
	/****************************************************************************
	*				  			GameObject
	*************************************************************************//**
	*  @class    GameObject
	*  @brief    GameObject (base class in the all object class)
	*****************************************************************************/
	class GameObject : public NonCopyable, public gu::EnableSharedFromThis<GameObject>
	{
	protected:
		using GameObjectPtr = gu::SharedPointer<GameObject>;
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
	public:
		/****************************************************************************
		**                Static Function
		*****************************************************************************/
		/*-------------------------------------------------------------------
		-                       Create
		---------------------------------------------------------------------*/
		template <typename T> requires std::is_base_of_v<GameObject, T>
		static gu::SharedPointer<T> Create(const LowLevelGraphicsEnginePtr& engine)
		{
			const auto gameObject = gu::MakeShared<T>(engine);
			GameObjects.push_back(gameObject);
			return gameObject;
		}

		/*-------------------------------------------------------------------
		-                       Find
		---------------------------------------------------------------------*/
		/* @brief : Obtain a gameObject matching the name*/
		static GameObjectPtr Find(const gu::tstring& name);

		/* @brief : This function returns the gameobject list with the same tag as the assign tag.*/
		static std::vector<GameObjectPtr> GameObjectsWithTag(const gu::tstring& tag);

		/*-------------------------------------------------------------------
		-               Destroy and Clear
		---------------------------------------------------------------------*/
		/* @brief : This function destroy the game object with the same assigned object*/
		static bool Destroy(GameObjectPtr& gameObject);

		/* @brief : Destroy objects including parent and children object*/
		static void DestroyWithChildren(GameObjectPtr& parent);

		/* @brief : destroy all objects have the tag*/
		static void DestroyAllTagObjects(const gu::tstring& tag);

		/* @brief : Clear all game objects*/
		static void ClearAllGameObjects();
		
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		inline void SetActive(bool isActive) { _isActive = isActive; }

		inline bool  IsActive() const { return _isActive; }

		/*-------------------------------------------------------------------
		-               GameObject Transform
		---------------------------------------------------------------------*/
		inline       gm::Transform&  GetTransform()       { return _transform; }

		inline const gm::Transform&  GetTransform() const { return _transform; }

		inline       gm::Vector3&    GetPosition()       { return _transform.LocalPosition; }

		inline const gm::Vector3&    GetPosition() const { return _transform.LocalPosition; }

		inline       gm::Quaternion& GetRotation()       { return _transform.LocalRotation; }

		inline const gm::Quaternion& GetRotation() const { return _transform.LocalRotation; }

		inline       gm::Vector3&    GetScale()          { return _transform.LocalScale; }

		inline const gm::Vector3&    GetScale() const    { return _transform.LocalScale; }

		inline void SetPosition(float x, float y, float z)   { _transform.LocalPosition = gm::Vector3(x, y, z); }

		inline void SetPosition(const gm::Vector3& position) { _transform.LocalPosition = position; }

		inline void SetScale(float x, float y, float z) { _transform.LocalScale = gm::Vector3(x, y, z); }

		inline void SetScale(const gm::Vector3& scale) { _transform.LocalScale = scale; }

		inline void SetRotation(const gm::Quaternion& rotation) { _transform.LocalRotation = rotation; }
		
		/*-------------------------------------------------------------------
		-               GameObject Default Infomation
		---------------------------------------------------------------------*/
		inline gu::tstring GetName() const { return _name; }

		inline gu::tstring GetTag() const { return _tag; }

		inline gu::tstring GetLayerName() const { return LayerList[_layer]; }

		inline ObjectType GetType() const { return _type; }

		inline void SetName(const gu::tstring& name) { _name = name; }

		inline void SetTag(const gu::tstring& name) { _tag = name; }

		inline void SetLayer(const gu::tstring& name) { int bit = GetLayerBit(name); if (bit >= 0) { _layer = (1 << bit); } }


		/*-------------------------------------------------------------------
		-               GameObject Child
		---------------------------------------------------------------------*/
		GameObjectPtr GetChild(int index) { return  (index < _children.size()) ? _children[index] : nullptr; }

		void AddChild(const GameObjectPtr& child) { _children.emplace_back(child); }
		
		bool RemoveChild(GameObjectPtr& child);

		void ClearChildren();

		inline int  GetChildCount() const { return static_cast<int>(_children.size()); }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GameObject(const LowLevelGraphicsEnginePtr& engine);

		virtual ~GameObject();
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
		gu::tstring _name = SP(""); // object name

		gu::tstring _tag  = SP(""); // object tag

		int          _layer = 0;

		ObjectType _type = ObjectType::Static;

		/*-------------------------------------------------------------------
		-           gameObject state
		---------------------------------------------------------------------*/
		bool  _isActive = true;

		/*-------------------------------------------------------------------
		-           gameObject Config
		---------------------------------------------------------------------*/
		gm::Transform _transform;

		/*-------------------------------------------------------------------
		-           parent tree
		---------------------------------------------------------------------*/
		GameObjectPtr _parent = nullptr;

		LowLevelGraphicsEnginePtr _engine = nullptr;

		std::vector<GameObjectPtr> _children = {};
		
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		int GetLayerBit(const gu::tstring& layer);

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		static std::vector<GameObjectPtr> GameObjects;
		static std::vector<gu::tstring>  LayerList;
	};
}
#endif