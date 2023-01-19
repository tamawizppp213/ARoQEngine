//////////////////////////////////////////////////////////////////////////////////
//              @file   Model.cpp
///             @brief  Mesh Drawer Module
///             @author Toide Yutaro
///             @date   2022_06_02
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/Include/Model.hpp"
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
Model::Model()
{

}

Model::~Model()
{
    _meshes.clear();
    _meshes.shrink_to_fit();
}

#pragma endregion Constructor and Destructor 
#pragma region Main Function
void Model::Load(const PrimitiveMeshType type)
{

}

void Model::Load(const std::wstring& filePath)
{

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
#pragma endregion Model