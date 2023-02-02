//////////////////////////////////////////////////////////////////////////////////
///             @file   MMDModelConverter.hpp
///             @brief  Pmx amd pmd model <-> Game engine model
///             @author Toide Yutaro
///             @date   2023_02_01
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/MMDModelConverter.hpp"
#include "../Include/PMXParser.hpp"
#include "../Include/PMDParser.hpp"
#include "../../../Include/Model.hpp"
#include "../../../Include/Mesh.hpp"
#include "../../../Include/Material.hpp"
#include "../../../Include/MaterialType.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Math/Include/GMVertex.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::core;
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Main Function
bool PMXConverter::Load(const std::wstring& filePath, ModelPtr model)
{
	if (model == nullptr) { OutputDebugStringA("model is nullptr.");  return false; }

	/*-------------------------------------------------------------------
	-            PMXFile Load
	---------------------------------------------------------------------*/
	pmx::PMXFile file;
	if(!file.Load(filePath)) {return false;}

	/*-------------------------------------------------------------------
	-            PMXFile Load
	---------------------------------------------------------------------*/
	model->_hasSkin = !file.Bones.empty();

	/*-------------------------------------------------------------------
	-            Copy PMXvertex -> skin vertex
	---------------------------------------------------------------------*/
	std::unique_ptr<gm::SkinMeshVertex[]> vertices = std::make_unique<gm::SkinMeshVertex[]>(file.Vertices.size());
	for (size_t i = 0; i < file.Vertices.size(); ++i)
	{
		const auto& pmxVertex = file.Vertices[i];

		vertices[i].Position = pmxVertex.Position;
		vertices[i].Normal   = pmxVertex.Normal;
		vertices[i].UV       = pmxVertex.UV;
		std::memcpy(vertices[i].BoneIndices, pmxVertex.BoneIndices, sizeof(pmxVertex.BoneIndices));
		std::memcpy(vertices[i].BoneWeights, pmxVertex.BoneWeights, sizeof(pmxVertex.BoneWeights));
	}

	///*-------------------------------------------------------------------
	//-            Total mesh
	//---------------------------------------------------------------------*/
	//const auto vbData = GPUBufferMetaData::VertexBuffer(sizeof(gm::SkinMeshVertex), file.Vertices.size(), MemoryHeap::Default, ResourceState::Common, vertices.get());
	//const auto ibData = GPUBufferMetaData::IndexBuffer (sizeof(UINT32)            , file.Indices .size(), MemoryHeap::Default, ResourceState::Common, file.Indices.data());
	//model->_totalMesh = std::make_shared<Mesh>(model->_engine, vbData, ibData);	

	///*-------------------------------------------------------------------
	//-            Mesh of each material
	//---------------------------------------------------------------------*/
	//model->_materialCount = file.Materials.size();
	//model->_meshes   .resize(file.Materials.size());
	//model->_materials.resize(file.Materials.size());

	//size_t indexOffset = 0;
	//for (size_t i = 0; i < model->_materialCount; ++i)
	//{
	//	auto& mesh     = model->_meshes[i];
	//	auto& material = model->_materials[i];

	//	/*-------------------------------------------------------------------
	//	-            Create material
	//	---------------------------------------------------------------------*/
	//	// Set up physical material
	//	PBRMaterial pbrMaterial;
	//	{
	//		pbrMaterial.Diffuse           = file.Materials[i].Diffuse;
	//		pbrMaterial.Ambient           = file.Materials[i].Ambient;
	//		pbrMaterial.Specular          = file.Materials[i].Specular;
	//		pbrMaterial.SpecularIntensity = file.Materials[i].SpecularPower;
	//	};

	//	// GPU constant buffer information
	//	GPUBufferMetaData bufferInfo = GPUBufferMetaData::ConstantBuffer(sizeof(PBRMaterial), 1, MemoryHeap::Upload, ResourceState::Common, &pbrMaterial);
	//	
	//	// material buffer
	//	material = std::make_shared<Material>(model->_engine, bufferInfo, unicode::ToWString(file.Materials[i].MaterialName));

	//	/*-------------------------------------------------------------------
	//	-            Create mesh
	//	---------------------------------------------------------------------*/
	//	mesh = std::make_shared<Mesh>(model->_engine, 
	//		model->_totalMesh->GetVertexBuffers(), 
	//		model->_totalMesh->GetIndexBuffer(), 
	//		file.Materials[i].FaceIndicesCount,
	//		indexOffset);
	//}
	return true;
}


bool PMDConverter::Load(const std::wstring& filePath, ModelPtr model)
{
	return true;
}


#pragma endregion Main Function