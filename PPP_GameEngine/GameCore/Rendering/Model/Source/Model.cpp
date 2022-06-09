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
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
const D3D12_INPUT_ELEMENT_DESC SkinMeshVertex::InputElements[] =
{
    { "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "BONE_NO",    0, DXGI_FORMAT_R32G32B32A32_SINT , 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "WEIGHT"   ,  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(SkinMeshVertex) == 64, "Vertex struct/layout mismatch");
const D3D12_INPUT_LAYOUT_DESC SkinMeshVertex::InputLayout =
{
    SkinMeshVertex::InputElements,
    SkinMeshVertex::InputElementCount
};
#pragma region Model
void Model::Initialize(const std::wstring& filePath, const std::wstring& addName)
{

}
void Model::Update(float deltaTime)
{

}
void Model::Draw(SceneGPUAddress address)
{

}
#pragma endregion Model