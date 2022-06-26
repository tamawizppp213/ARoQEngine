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
#include "Mesh.hpp"
#include "Material.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
struct SkinMeshVertex
{
	gm::Float3 Position;
	gm::Float3 Normal;
	gm::Float2 UV;
	INT32      BoneIndices[4] = {0};
	float      BoneWeights[4] = {0};

	static const D3D12_INPUT_LAYOUT_DESC InputLayout;
	SkinMeshVertex() = default;
	SkinMeshVertex(const SkinMeshVertex&)             = default;
	SkinMeshVertex& operator=(const SkinMeshVertex&) = default;
	SkinMeshVertex& operator=(SkinMeshVertex&&)       = default;
	SkinMeshVertex(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& normal, DirectX::XMFLOAT2 const& uv)
		: Position(position), Normal(normal), UV(uv)
	{
	};
	SkinMeshVertex(DirectX::FXMVECTOR position, DirectX::FXMVECTOR normal, DirectX::FXMVECTOR uv)
	{
		DirectX::XMStoreFloat3(&this->Position, position);
		DirectX::XMStoreFloat3(&this->Normal, normal);
		DirectX::XMStoreFloat2(&this->UV, uv);
	}

private:
	static constexpr unsigned int InputElementCount = 5;
	static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};

/****************************************************************************
*				  			Model
*************************************************************************//**
*  @class     Model
*  @brief     Model
*****************************************************************************/
class Model : public GameActor
{
	using MeshArray     = std::vector<Mesh>;
	using BoneBufferPtr = std::unique_ptr<UploadBuffer>;
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void Initialize(const std::wstring& filePath, const std::wstring& addName = L"");
	void Update(float deltaTime)       override;
	void Draw(SceneGPUAddress address) override;
	void Finalize();
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	// mesh
	      MeshArray& GetMesh()       { return _meshes; }
	const MeshArray& GetMesh() const { return _meshes; }

	// material buffer
	      MaterialBuffer& GetMaterialBuffer()       { return _materialBuffer; }
	const MaterialBuffer& GetMaterialBuffer() const { return _materialBuffer; }


	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/

protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/

	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	MeshArray      _meshes;
	MaterialBuffer _materialBuffer;
	BoneBufferPtr  _boneBuffer;
};

#endif