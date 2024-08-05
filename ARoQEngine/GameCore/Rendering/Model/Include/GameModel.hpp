//////////////////////////////////////////////////////////////////////////////////
///             @file   Model.hpp
///             @brief  Model
///             @author Toide Yutaro
///             @date   2022_06_06
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GAME_MODEL_HPP
#define GAME_MODEL_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Core/Include/GameActor.hpp"
#include "PrimitiveMesh.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine
{
	class Mesh;
	class Material;
	class GameWorldInfo;
	/****************************************************************************
	*				  			Model
	****************************************************************************/
	/* @class     Model
	*  @brief     This class doesn't have the skin mesh. If you use skin mesh, you should use SkinModel class.
	*****************************************************************************/
	class GameModel : public GameActor
	{
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
		using MeshPtr          = gu::SharedPointer<Mesh>; // single mesh pointer
		using MeshArrayPtr     = gu::DynamicArray<MeshPtr>; // material count array 
		using MaterialPtr      = gu::SharedPointer<Material>;
		using GameWorldInfoPtr = gu::SharedPointer<GameWorldInfo>;
	
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Load primitive mesh.*/
		void Load(const PrimitiveMeshType type, const MaterialPtr& material = nullptr);

		/* @brief : Load model according to the extension.*/
		void Load(const gu::tstring& filePath);
		
		/* @brief : Update motion*/
		virtual void Update(const float deltaTime, const bool enableUpdateChild = false) override;
		
		virtual void Draw(const bool isDrawingEachMaterial = true, const std::uint32_t materialOffsetID = 2);

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		MeshPtr GetTotalMesh() const noexcept { return _totalMesh; }

		MeshArrayPtr GetMeshes() const noexcept { return _meshes; }

		size_t GetMaterialCount() const { return _materialCount; }

		gm::Float4 GetDebugColor() const { return _debugColor; }

		bool HasSkin() const { return _hasSkin; }

		void SetDebugColor(const gm::Float4& color) { _debugColor = color; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GameModel(const LowLevelGraphicsEnginePtr& engine, const GameWorldInfoPtr& customGameWorldInfo = nullptr);

		GameModel(const LowLevelGraphicsEnginePtr& engine, const MeshPtr& mesh, const GameWorldInfoPtr& customGameWorldInfo = nullptr);


		~GameModel();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		virtual void DrawWithMaterials(const std::uint32_t materialOffsetID);

		virtual void DrawWithoutMaterial();

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		/* @brief : world position matrix. When you use the instancing drawing,  you should set the custom game world information.  */
		GameWorldInfoPtr _gameWorld = nullptr;
		bool _hasCustomGameWorld = false;

		/*-------------------------------------------------------------------
		-            Mesh
		---------------------------------------------------------------------*/
		/* @brief : each material mesh. (length : material count)*/
		MeshArrayPtr _meshes = {};

		/* @brief : total mesh (ignore material)*/
		MeshPtr _totalMesh = nullptr;

		/*-------------------------------------------------------------------
		-            Material
		---------------------------------------------------------------------*/
		/* @brief : material */
		gu::DynamicArray<MaterialPtr> _materials = {};

		/* @brief : Material count*/
		size_t  _materialCount = 0;

		gm::Float4 _debugColor = gm::Float4(1.0f, 1.0f, 1.0f, 1.0f);
		/*-------------------------------------------------------------------
		-            Bone
		---------------------------------------------------------------------*/
		/* @brief : Uses skin mesh model (true: Has skin mesh , false : Doesn't have skin mesh)*/
		bool _hasSkin = false;

	private:
		void PrepareGameWorldBuffer(); 

	public:
		friend class PMXConverter;
		friend class PMDConverter;
		friend class GLTFConverter;
	};
}
#endif