//////////////////////////////////////////////////////////////////////////////////
///             @file   MMDModelConverter.hpp
///             @brief  Pmx amd pmd model <-> Game engine model
///             @author Toide Yutaro
///             @date   2023_02_01
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/GLTFModelConverter.hpp"
#include "../Include/GLTFParser.hpp"
#include "../../../../Include/GameModel.hpp"
#include "../../../../Include/Mesh.hpp"
#include "../../../../Include/Material.hpp"
#include "../../../../Include/MaterialType.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Math/Include/GMVertex.hpp"
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::core;
using namespace rhi::core;
namespace
{
	constexpr std::int32_t INVALID_ID = -1;
}

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Main Function
bool GLTFConverter::Load(const std::wstring& filePath, GameModelPtr model)
{
	if (model == nullptr) { OutputDebugStringA("model is nullptr.");  return false; }
	if (model->_engine == nullptr) { OutputDebugStringA("engine is nullptr"); return false; }

	/*-------------------------------------------------------------------
	-            PMXFile Load
	---------------------------------------------------------------------*/
	gltf::GLTFFile file;
	file.Load(unicode::ToUtf8String(filePath));

	/*-------------------------------------------------------------------
	-            Set up resource
	---------------------------------------------------------------------*/
	PrepareTotalMesh(model, file);
	PrepareEachMaterialMesh(model, file);

	/*-------------------------------------------------------------------
	-            Check skin mesh model
	---------------------------------------------------------------------*/
	model->_hasSkin = file.Document.Skins.Size() != 0;

	return true;
}

#pragma endregion Main Function

#pragma region PMX 
/****************************************************************************
*					PrepareTotalMesh
*************************************************************************//**
*  @fn        void PMXConverter::PrepareTotalMesh(const GameModelPtr model, pmx::PMXFile& file)
*
*  @brief     Prepare total mesh buffer (all material index buffer and vertex buffer)(ignore material)
*
*  @param[in] const GameModelPtr
*  @param[in] pmx::PMXFile& file
*
*  @return 　　void
*****************************************************************************/
void GLTFConverter::PrepareTotalMesh(const GameModelPtr model, gltf::GLTFFile& file)
{
	///*-------------------------------------------------------------------
	//-            Copy PMXvertex -> skin vertex
	//---------------------------------------------------------------------*/
	//std::unique_ptr<gm::SkinMeshVertex[]> vertices = std::make_unique<gm::SkinMeshVertex[]>(file.Document.Meshes.Vertices.size());
	//for (size_t i = 0; i < file.Vertices.size(); ++i)
	//{
	//	const auto& pmxVertex = file.Vertices[i];

	//	vertices[i].Position = pmxVertex.Position;
	//	vertices[i].Normal = pmxVertex.Normal;
	//	vertices[i].UV = pmxVertex.UV;
	//	std::memcpy(vertices[i].BoneIndices, pmxVertex.BoneIndices, sizeof(pmxVertex.BoneIndices));
	//	std::memcpy(vertices[i].BoneWeights, pmxVertex.BoneWeights, sizeof(pmxVertex.BoneWeights));
	//}

	///*-------------------------------------------------------------------
	//-            Total mesh
	//---------------------------------------------------------------------*/
	//const auto vbData = GPUBufferMetaData::VertexBuffer(sizeof(gm::SkinMeshVertex), file.Vertices.size(), MemoryHeap::Upload, ResourceState::Common, vertices.get());
	//const auto ibData = GPUBufferMetaData::IndexBuffer(sizeof(UINT32), file.Indices.size(), MemoryHeap::Default, ResourceState::Common, file.Indices.data());
	//model->_totalMesh = std::make_shared<Mesh>(model->_engine, vbData, ibData);
}

/****************************************************************************
*					PrepareEachMaterialMesh
*************************************************************************//**
*  @fn        vvoid PMXConverter::PrepareEachMaterialMesh(const GameModelPtr model, pmx::PMXFile& file)
*
*  @brief     Prepare each material mesh
*
*  @param[in] const GameModelPtr
*  @param[in] pmx::PMXFile& file
*
*  @return 　　void
*****************************************************************************/
void GLTFConverter::PrepareEachMaterialMesh(const GameModelPtr model, gltf::GLTFFile& file)
{
	//model->_materialCount = file.Materials.size();
	//model->_meshes.resize(file.Materials.size());
	//model->_materials.resize(file.Materials.size());

	//size_t indexOffset = 0;
	//for (size_t i = 0; i < model->_materialCount; ++i)
	//{
	//	auto& mesh = model->_meshes[i];
	//	auto& material = model->_materials[i];

	//	/*-------------------------------------------------------------------
	//	-            Create material
	//	---------------------------------------------------------------------*/
	//	// Set up physical material
	//	PBRMaterial pbrMaterial;
	//	{
	//		pbrMaterial.Diffuse = file.Materials[i].Diffuse;
	//		pbrMaterial.Ambient = file.Materials[i].Ambient;
	//		pbrMaterial.Specular = file.Materials[i].Specular;
	//		pbrMaterial.SpecularIntensity = file.Materials[i].SpecularPower;
	//	};

	//	// GPU constant buffer information
	//	GPUBufferMetaData bufferInfo = GPUBufferMetaData::ConstantBuffer(sizeof(PBRMaterial), 1, MemoryHeap::Upload, ResourceState::Common, &pbrMaterial);

	//	// material buffer
	//	material = std::make_shared<Material>(model->_engine, bufferInfo, unicode::ToWString(file.Materials[i].MaterialName));

	//	/*-------------------------------------------------------------------
	//	-            Set up texture
	//	---------------------------------------------------------------------*/
	//	if (file.Materials[i].TextureIndex != INVALID_ID)
	//	{
	//		material->LoadTexture(unicode::ToWString(file.TexturePathList[file.Materials[i].TextureIndex]), UsageTexture::Diffuse);
	//	}
	//	if (file.Materials[i].SphereMapTextureIndex != INVALID_ID)
	//	{
	//		material->LoadTexture(unicode::ToWString(file.TexturePathList[file.Materials[i].SphereMapTextureIndex]), UsageTexture::Specular);
	//	}

	//	/*-------------------------------------------------------------------
	//	-            Create mesh
	//	---------------------------------------------------------------------*/
	//	mesh = std::make_shared<Mesh>(model->_engine,
	//		model->_totalMesh->GetVertexBuffers(),
	//		model->_totalMesh->GetIndexBuffer(),
	//		file.Materials[i].FaceIndicesCount,
	//		indexOffset);

	//	indexOffset += file.Materials[i].FaceIndicesCount;
	//}
}
#pragma endregion PMX