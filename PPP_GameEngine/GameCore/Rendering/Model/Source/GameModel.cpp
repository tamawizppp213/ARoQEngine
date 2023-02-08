//////////////////////////////////////////////////////////////////////////////////
//              @file   Model.cpp
///             @brief  Mesh Drawer Module
///             @author Toide Yutaro
///             @date   2022_06_02
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/GameModel.hpp"
#include "../Include/PrimitiveMesh.hpp"
#include "../Include/Mesh.hpp"
#include "../Include/Material.hpp"
#include "../../../Core/Include/GameWorldInfo.hpp"
#include "../External/MMD/Include/MMDModelConverter.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GameUtility/File/Include/FileSystem.hpp"
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::core;
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Model

#pragma region Constructor and Destructor
GameModel::GameModel(const LowLevelGraphicsEnginePtr& engine, const GameWorldInfoPtr& customGameWorldInfo) : GameActor(engine), _gameWorld(customGameWorldInfo)
{
    if (customGameWorldInfo) { _hasCustomGameWorld = true; }
    PrepareGameWorldBuffer();
}

GameModel::GameModel(const LowLevelGraphicsEnginePtr& engine, const MeshPtr& mesh, const GameWorldInfoPtr& customGameWorldInfo)
    : GameActor(engine), _gameWorld(customGameWorldInfo)
{
    if (customGameWorldInfo) { _hasCustomGameWorld = true; }
    _meshes.push_back(mesh);
    PrepareGameWorldBuffer();
}


GameModel::~GameModel()
{
    _materials.clear();
    _materials.shrink_to_fit();
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
void GameModel::Load(const PrimitiveMeshType type, const MaterialPtr& material)
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
void GameModel::Load(const std::wstring& filePath)
{
    std::unique_ptr<IGameModelConverter> loader = nullptr;

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

void GameModel::Update(const float deltaTime, const bool enableUpdateChild)
{
    if (!_isActive) { return; }

    if (!_hasCustomGameWorld)
    {
        GameWorldConstant world = 
        { 
            .World = _transform.GetFloat4x4(),
#ifdef _DEBUG
            .DebugColor = _debugColor
#endif
        };

        _gameWorld->GetBuffer()->Update(&world, 1);
    }
    GameActor::Update(deltaTime, enableUpdateChild);
}

void GameModel::Draw(bool isDrawingEachMaterial, const std::uint32_t materialOffsetID)
{
    if (isDrawingEachMaterial)
    {
        DrawWithMaterials(materialOffsetID);
    }
    else
    {
        DrawWithoutMaterial();
    }
}
#pragma endregion Main Function


#pragma endregion Model

#pragma region Set up
/****************************************************************************
*					PrepareGameWorldBuffer
*************************************************************************//**
*  @fn        void GameModel::PrepareGameWorldBuffer()
*
*  @brief     Prepare Game world constant buffer
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void GameModel::PrepareGameWorldBuffer()
{
    if (_gameWorld) { return; }

    _gameWorld = std::make_shared<GameWorldInfo>(_engine, 1);

}
#pragma endregion Set up
#pragma region Draw
void GameModel::DrawWithMaterials(const std::uint32_t materialOffsetID)
{
    const auto frameIndex = _engine->GetCurrentFrameIndex();
    const auto commandList = _engine->GetCommandList(CommandListType::Graphics, frameIndex);

    /*-------------------------------------------------------------------
    -              Get texture ids Shift by 1 from the next to the materialID
    ---------------------------------------------------------------------*/
    std::vector<std::uint32_t> textureIDs((std::uint32_t)UsageTexture::CountOf);
    for (std::uint32_t i = 0; i < (std::uint32_t)UsageTexture::CountOf; ++i)
    {
        textureIDs[i] = materialOffsetID + i + 1;
    }

    /*-------------------------------------------------------------------
    -              Total Mesh Set up
    ---------------------------------------------------------------------*/
    commandList->SetPrimitiveTopology(PrimitiveTopology::TriangleList);
    commandList->SetVertexBuffer(_totalMesh->GetVertexBuffers()[frameIndex]);
    commandList->SetIndexBuffer(_totalMesh->GetIndexBuffer());

    /*-------------------------------------------------------------------
    -              Draw each material mesh
    ---------------------------------------------------------------------*/
    _gameWorld->Bind(commandList, 1);
    for (size_t i = 0; i < _materialCount; ++i)
    {
        _materials[i]->Bind(commandList, frameIndex, materialOffsetID, textureIDs);
        _meshes[i]->Draw(commandList, frameIndex);
    }
}

void GameModel::DrawWithoutMaterial()
{
    const auto frameIndex = _engine->GetCurrentFrameIndex();
    const auto commandList = _engine->GetCommandList(CommandListType::Graphics, frameIndex);
    _gameWorld->Bind(commandList, 1);
    _totalMesh->Draw(commandList, frameIndex);
}
#pragma endregion Draw