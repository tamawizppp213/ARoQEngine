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

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
//struct SkinMeshVertex
//{
//	gm::Float3 Position;
//	gm::Float3 Normal;
//	gm::Float2 UV;
//	INT32      BoneIndices[4] = {0};
//	float      BoneWeights[4] = {0};
//
//	static const D3D12_INPUT_LAYOUT_DESC InputLayout;
//	SkinMeshVertex() = default;
//	SkinMeshVertex(const SkinMeshVertex&)             = default;
//	SkinMeshVertex& operator=(const SkinMeshVertex&) = default;
//	SkinMeshVertex& operator=(SkinMeshVertex&&)       = default;
//	SkinMeshVertex(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& normal, DirectX::XMFLOAT2 const& uv)
//		: Position(position), Normal(normal), UV(uv)
//	{
//	};
//	SkinMeshVertex(DirectX::FXMVECTOR position, DirectX::FXMVECTOR normal, DirectX::FXMVECTOR uv)
//	{
//		DirectX::XMStoreFloat3(&this->Position, position);
//		DirectX::XMStoreFloat3(&this->Normal, normal);
//		DirectX::XMStoreFloat2(&this->UV, uv);
//	}
//
//private:
//	static constexpr unsigned int InputElementCount = 5;
//	static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];
//};

namespace gc::core
{
	class Mesh;
	/****************************************************************************
	*				  			Model
	*************************************************************************//**
	*  @class     Model
	*  @brief     Model
	*****************************************************************************/
	class Model : public gc::core::GameActor
	{
		using MeshPtr = std::shared_ptr<Mesh>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Load(const PrimitiveMeshType type);

		void Load(const std::wstring& filePath);
		
		void Update(const float deltaTime, const bool enableUpdateChild = false) override;
		
		void Draw(const GPUResourceViewPtr& scene) override;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/


		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Model();

		~Model();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::vector<MeshPtr> _meshes = {};
	};
}
#endif