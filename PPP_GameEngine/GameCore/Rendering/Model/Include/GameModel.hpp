//////////////////////////////////////////////////////////////////////////////////
///             @file   Model.hpp
///             @brief  Model
///             @author Toide Yutaro
///             @date   2022_06_06
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef MODEL_HPP
#define MODEL_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Core/Include/GameActor.hpp"
#include "PrimitiveMesh.hpp"
#include <vector>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::core
{
	class Mesh;
	class Material;
	/****************************************************************************
	*				  			Model
	*************************************************************************//**
	*  @class     Model
	*  @brief     This class doesn't have the skin mesh. If you use skin mesh, you should use SkinModel class.
	*****************************************************************************/
	class GameModel : public gc::core::GameActor
	{
		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
		using MeshPtr      = std::shared_ptr<Mesh>; // single mesh pointer
		using MeshArrayPtr = std::vector<MeshPtr>; // material count array 
		using MaterialPtr  = std::shared_ptr<Material>;
	
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Load primitive mesh.*/
		void Load(const PrimitiveMeshType type, const MaterialPtr& material = nullptr);

		/* @brief : Load model according to the extension.*/
		void Load(const std::wstring& filePath);
		
		/* @brief : Update motion*/
		void Update(const float deltaTime, const bool enableUpdateChild = false) override;
		
		void Draw(const GPUResourceViewPtr& scene) override;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		MeshPtr GetTotalMesh() const noexcept { return _totalMesh; }

		MeshArrayPtr GetMeshes() const noexcept { return _meshes; }

		size_t GetMaterialCount() const { return _materialCount; }

		bool HasSkin() const { return _hasSkin; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GameModel(const LowLevelGraphicsEnginePtr& engine);

		GameModel(const LowLevelGraphicsEnginePtr& engine, const MeshPtr& mesh);


		~GameModel();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
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
		std::vector<MaterialPtr> _materials = {};

		/* @brief : Material count*/
		size_t  _materialCount = 0;

		/*-------------------------------------------------------------------
		-            Bone
		---------------------------------------------------------------------*/
		/* @brief : Uses skin mesh model (true: Has skin mesh , false : Doesn't have skin mesh)*/
		bool _hasSkin = false;

	public:
		friend class PMXConverter;
		friend class PMDConverter;
	};
}
#endif