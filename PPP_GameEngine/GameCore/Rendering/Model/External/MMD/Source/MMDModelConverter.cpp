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
#include "../../../Include/GameModel.hpp"
#include "../../../Include/Mesh.hpp"
#include "../../../Include/Material.hpp"
#include "../../../Include/MaterialType.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Math/Include/GMVertex.hpp"
#include <string>
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
bool PMXConverter::Load(const gu::tstring& filePath, GameModelPtr model)
{
	if (model == nullptr)          { OutputDebugStringA("model is nullptr.");  return false; }
	if (!model->_engine) { OutputDebugStringA("engine is nullptr"); return false; }

	/*-------------------------------------------------------------------
	-            PMXFile Load
	---------------------------------------------------------------------*/
	pmx::PMXFile file;
	if(!file.Load(filePath)) {return false;}

	/*-------------------------------------------------------------------
	-            Set up resource
	---------------------------------------------------------------------*/
	PrepareTotalMesh(model, file);
	PrepareEachMaterialMesh(model, file);
	
	/*-------------------------------------------------------------------
	-            Check skin mesh model
	---------------------------------------------------------------------*/
	model->_hasSkin = !file.Bones.IsEmpty();

	return true;
}


bool PMDConverter::Load(const gu::tstring& filePath, GameModelPtr model)
{
	if (model == nullptr)          { OutputDebugStringA("model is nullptr") ; return false; }
	if (!model->_engine) { OutputDebugStringA("engine is nullptr"); return false; }
	
	/*-------------------------------------------------------------------
	-            PMXFile Load
	---------------------------------------------------------------------*/
	pmd::PMDFile file;
	if (!file.Load(filePath)) { return false; }


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
void PMXConverter::PrepareTotalMesh(const GameModelPtr model, pmx::PMXFile& file)
{
	/*-------------------------------------------------------------------
	-            Copy PMXvertex -> skin vertex
	---------------------------------------------------------------------*/
	std::unique_ptr<gm::SkinMeshVertex[]> vertices = std::make_unique<gm::SkinMeshVertex[]>(file.Vertices.Size());
	for (size_t i = 0; i < file.Vertices.Size(); ++i)
	{
		const auto& pmxVertex = file.Vertices[i];

		vertices[i].Position = pmxVertex.Position;
		vertices[i].Normal   = pmxVertex.Normal;
		vertices[i].UV       = pmxVertex.UV;
		std::memcpy(vertices[i].BoneIndices, pmxVertex.BoneIndices, sizeof(pmxVertex.BoneIndices));
		std::memcpy(vertices[i].BoneWeights, pmxVertex.BoneWeights, sizeof(pmxVertex.BoneWeights));
	}

	/*-------------------------------------------------------------------
	-            Total mesh
	---------------------------------------------------------------------*/
	const auto vbData = GPUBufferMetaData::VertexBuffer(sizeof(gm::SkinMeshVertex), file.Vertices.Size(), MemoryHeap::Upload , ResourceState::Common, vertices.get());
	const auto ibData = GPUBufferMetaData::IndexBuffer (sizeof(UINT32)            , file.Indices .Size(), MemoryHeap::Default, ResourceState::Common, file.Indices.Data());
	model->_totalMesh = gu::MakeShared<Mesh>(model->_engine, vbData, ibData);	
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
void PMXConverter::PrepareEachMaterialMesh(const GameModelPtr model, pmx::PMXFile& file)
{
	model->_materialCount = file.Materials.Size();
	model->_meshes.Resize(file.Materials.Size());
	model->_materials.Resize(file.Materials.Size());

	size_t indexOffset = 0;
	for (size_t i = 0; i < model->_materialCount; ++i)
	{
		auto& mesh = model->_meshes[i];
		auto& material = model->_materials[i];

		/*-------------------------------------------------------------------
		-            Create material
		---------------------------------------------------------------------*/
		// Set up physical material
		PBRMaterial pbrMaterial;
		{
			pbrMaterial.Diffuse = file.Materials[i].Diffuse;
			pbrMaterial.Ambient = file.Materials[i].Ambient;
			pbrMaterial.Specular = file.Materials[i].Specular;
			pbrMaterial.SpecularIntensity = file.Materials[i].SpecularPower;
		};

		// GPU constant buffer information
		GPUBufferMetaData bufferInfo = GPUBufferMetaData::ConstantBuffer(sizeof(PBRMaterial), 1, MemoryHeap::Upload, ResourceState::Common, &pbrMaterial);

		// material buffer
		material = gu::MakeShared<Material>(model->_engine, bufferInfo, gu::tstring(unicode::ToWString(std::string(file.Materials[i].MaterialName.CString())).c_str()));

		/*-------------------------------------------------------------------
		-            Set up texture
		---------------------------------------------------------------------*/
		if (file.Materials[i].TextureIndex != INVALID_ID)
		{
			const auto stdPathName = unicode::ToWString(std::string(file.TexturePathList[file.Materials[i].TextureIndex].CString()));
			material->LoadTexture(gu::tstring(stdPathName.c_str()), UsageTexture::Diffuse);
		}
		if (file.Materials[i].SphereMapTextureIndex != INVALID_ID)
		{
			const auto stdPathName = unicode::ToWString(std::string(file.TexturePathList[file.Materials[i].TextureIndex].CString()));
			material->LoadTexture(gu::tstring(stdPathName.c_str()), UsageTexture::Specular);
		}

		/*-------------------------------------------------------------------
		-            Create mesh
		---------------------------------------------------------------------*/
		mesh = gu::MakeShared<Mesh>(model->_engine,
			model->_totalMesh->GetVertexBuffers(),
			model->_totalMesh->GetIndexBuffer(),
			(std::uint64_t)file.Materials[i].FaceIndicesCount,
			(std::uint32_t)indexOffset);

		indexOffset += file.Materials[i].FaceIndicesCount;
	}
}
#pragma endregion PMX