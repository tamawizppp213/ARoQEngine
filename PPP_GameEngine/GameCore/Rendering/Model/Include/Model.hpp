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
	class Model : public gc::core::GameActor
	{
		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
		using MeshPtr     = std::shared_ptr<Mesh>;
		using MaterialPtr = std::shared_ptr<Material>;
	
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Load(const PrimitiveMeshType type, const MaterialPtr& material = nullptr);

		void Load(const std::wstring& filePath);
		
		void Update(const float deltaTime, const bool enableUpdateChild = false) override;
		
		void Draw(const GPUResourceViewPtr& scene) override;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const std::vector<MeshPtr>& GetMeshes() const { return _meshes; }

		bool HasSkin() const { return _hasSkin; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Model(const LowLevelGraphicsEnginePtr& engine);

		Model(const LowLevelGraphicsEnginePtr& engine, const MeshPtr& mesh);

		Model(const LowLevelGraphicsEnginePtr& engine, const std::vector<MeshPtr>& mesh);

		~Model();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::vector<MeshPtr> _meshes = {}; // each material mesh

		MeshPtr _totalMesh = nullptr;      // total mesh (ignore material)

		bool _hasSkin = false;

		LowLevelGraphicsEnginePtr _engine = nullptr;

	public:
		friend class PMXConverter;
		friend class PMDConverter;
	};
}
#endif