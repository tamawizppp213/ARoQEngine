//////////////////////////////////////////////////////////////////////////////////
//              @file   Model.cpp
///             @brief  Mesh Drawer Module
///             @author Toide Yutaro
///             @date   2022_06_02
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/Model.hpp"
#include "../Include/PrimitiveMesh.hpp"
#include "../Include/Mesh.hpp"
#include "../Include/Material.hpp"
#include "../External/MMD/Include/MMDModelConverter.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GameUtility/File/Include/FileSystem.hpp"
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
//const D3D12_INPUT_ELEMENT_DESC SkinMeshVertex::InputElements[] =
//{
//    { "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
//    { "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
//    { "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
//    { "BONE_NO",    0, DXGI_FORMAT_R32G32B32A32_SINT , 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
//    { "WEIGHT"   ,  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
//};
//
//static_assert(sizeof(SkinMeshVertex) == 64, "Vertex struct/layout mismatch");
//const D3D12_INPUT_LAYOUT_DESC SkinMeshVertex::InputLayout =
//{
//    SkinMeshVertex::InputElements,
//    SkinMeshVertex::InputElementCount
//};
#pragma region Model

#pragma region Constructor and Destructor
Model::Model(const LowLevelGraphicsEnginePtr& engine) : _engine(engine)
{

}

Model::Model(const LowLevelGraphicsEnginePtr& engine, const MeshPtr& mesh)
    : _engine(engine)
{
    _meshes.push_back(mesh);
}

Model::Model(const LowLevelGraphicsEnginePtr& engine, const std::vector<MeshPtr>& meshes)
    :_engine(engine), _meshes(meshes)
{

}

Model::~Model()
{
    _meshes.clear();
    _meshes.shrink_to_fit();
}

#pragma endregion Constructor and Destructor 
#pragma region Main Function
/****************************************************************************
*					Load
*************************************************************************//**
*  @fn        void Model::Load(const PrimitiveMeshType type, const std::int32_t materialID)
*
*  @brief     Load primitive mesh (The mesh size is set all 1.0f (width, height and depth...)).
*             If you want to change size, you should change the scale parameter.
*
*  @param[in] const PrimitiveMeshType type
*  @param[in] const std::int32_t materialID (default set NEEDLESS = -1)
*
*  @return 　　void
*****************************************************************************/
void Model::Load(const PrimitiveMeshType type, const MaterialPtr& material)
{
#if _DEBUG
    assert(_engine);
#endif

    /*-------------------------------------------------------------------
    -              Select primitive mesh type
    ---------------------------------------------------------------------*/
    PrimitiveMesh primitiveMesh;
    switch (type)
    {
        using enum PrimitiveMeshType;
        case Box:       {primitiveMesh = PrimitiveMeshGenerator::Box      (1.0f, 1.0f, 1.0f, 0, false); break;  }
        case Sphere:    {primitiveMesh = PrimitiveMeshGenerator::Sphere   (1.0f, 20  , 20,      false); break; }
        case GeoSphere: {primitiveMesh = PrimitiveMeshGenerator::GeoSphere(1.0f, 20  ,          false); break; }
        case Grid:      {primitiveMesh = PrimitiveMeshGenerator::Grid     (1.0f, 1.0f, 1, 1,    false); break; }
        case Rect:      {primitiveMesh = PrimitiveMeshGenerator::Rect(1.0f, 1.0f, 1.0f); break; }
        default:
        {
            throw std::runtime_error("not support primitive mesh type");
        }
    }

    const auto mesh = std::make_shared<Mesh>(_engine, primitiveMesh, material);
    _meshes.push_back(mesh);
    _totalMesh = mesh;
}

/****************************************************************************
*					Load
*************************************************************************//**
*  @fn        void Model::Load(const std::wstring& filePath)
*
*  @brief     Load model mesh
*
*  @param[in] const std::wstring& filePath
*
*  @return 　　void
*****************************************************************************/
void Model::Load(const std::wstring& filePath)
{
    std::unique_ptr<IModelConverter> loader = nullptr;

    /*-------------------------------------------------------------------
    -              Load
    ---------------------------------------------------------------------*/
    const auto extension = file::FileSystem::GetExtension(filePath);

    if (extension == L"pmx") { loader = std::make_unique<PMXConverter>(); }
    else
    {
        throw std::runtime_error("not support extension type");
    }

    if (!loader->Load(filePath, this)) { throw std::runtime_error("failed to load model"); }
}

void Model::Update(const float deltaTime, const bool enableUpdateChild)
{
    if (!_isActive) { return; }

    GameActor::Update(deltaTime, enableUpdateChild);
}

void Model::Draw(const GPUResourceViewPtr& address)
{
    
}
#pragma endregion Main Function

#pragma region Load

#pragma endregion Load
#pragma endregion Model