//////////////////////////////////////////////////////////////////////////////////
///             @file    DirectX12VertexType.cpp
///             @brief   DirectX12 Input Layout
///             @author  Toide Yutaro (Reference: DirectXTK http://go.microsoft.com/fwlink/?LinkID=615561)
///             @date    2020_11_20
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12VertexTypes.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*							 VertexPosition
*****************************************************************************/
const D3D12_INPUT_ELEMENT_DESC VertexPosition::InputElements[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    
    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(VertexPosition) == 12, "Vertex struct/layout mismatch");

const D3D12_INPUT_LAYOUT_DESC VertexPosition::InputLayout =
{
    VertexPosition::InputElements,
    VertexPosition::InputElementCount
};

/****************************************************************************
*							 VertexPositionColor
*****************************************************************************/
const D3D12_INPUT_ELEMENT_DESC VertexPositionColor::InputElements[] =
{
    { "POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(VertexPositionColor) == 28, "Vertex struct/layout mismatch");

const D3D12_INPUT_LAYOUT_DESC VertexPositionColor::InputLayout =
{
    VertexPositionColor::InputElements,
    VertexPositionColor::InputElementCount
};

/****************************************************************************
*							 VertexPositionTexture
*****************************************************************************/
const D3D12_INPUT_ELEMENT_DESC VertexPositionTexture::InputElements[] =
{
    { "POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(VertexPositionTexture) == 20, "Vertex struct/layout mismatch");

const D3D12_INPUT_LAYOUT_DESC VertexPositionTexture::InputLayout =
{
    VertexPositionTexture::InputElements,
    VertexPositionTexture::InputElementCount
};

/****************************************************************************
*							 VertexPositionDualTexture
*****************************************************************************/
const D3D12_INPUT_ELEMENT_DESC VertexPositionDualTexture::InputElements[] =
{
    { "POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "TEXCOORD",    1, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(VertexPositionDualTexture) == 28, "Vertex struct/layout mismatch");

const D3D12_INPUT_LAYOUT_DESC VertexPositionDualTexture::InputLayout =
{
    VertexPositionDualTexture::InputElements,
    VertexPositionDualTexture::InputElementCount
};

/****************************************************************************
*							 VertexPositionNormal
*****************************************************************************/
const D3D12_INPUT_ELEMENT_DESC VertexPositionNormal::InputElements[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(VertexPositionNormal) == 24, "Vertex struct/layout mismatch");

const D3D12_INPUT_LAYOUT_DESC VertexPositionNormal::InputLayout =
{
    VertexPositionNormal::InputElements,
    VertexPositionNormal::InputElementCount
};

/****************************************************************************
*							 VertexPositionColorTexture
*****************************************************************************/
const D3D12_INPUT_ELEMENT_DESC VertexPositionColorTexture::InputElements[] =
{
    { "POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(VertexPositionColorTexture) == 36, "Vertex struct/layout mismatch");

const D3D12_INPUT_LAYOUT_DESC VertexPositionColorTexture::InputLayout =
{
    VertexPositionColorTexture::InputElements,
    VertexPositionColorTexture::InputElementCount
};

/****************************************************************************
*							 VertexPositionColorTexture
*****************************************************************************/
const D3D12_INPUT_ELEMENT_DESC VertexPositionNormalColor::InputElements[] =
{
    { "POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(VertexPositionNormalColor) == 40, "Vertex struct/layout mismatch");

const D3D12_INPUT_LAYOUT_DESC VertexPositionNormalColor::InputLayout =
{
    VertexPositionNormalColor::InputElements,
    VertexPositionNormalColor::InputElementCount
};

/****************************************************************************
*							 VertexPositionNormalTexture
*****************************************************************************/
const D3D12_INPUT_ELEMENT_DESC VertexPositionNormalTexture::InputElements[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(VertexPositionNormalTexture) == 32, "Vertex struct/layout mismatch");

const D3D12_INPUT_LAYOUT_DESC VertexPositionNormalTexture::InputLayout =
{
    VertexPositionNormalTexture::InputElements,
    VertexPositionNormalTexture::InputElementCount
};

/****************************************************************************
*							 VertexPositionNormalColorTexture
*****************************************************************************/
const D3D12_INPUT_ELEMENT_DESC VertexPositionNormalColorTexture::InputElements[] =
{
    { "POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};

static_assert(sizeof(VertexPositionNormalColorTexture) == 48, "Vertex struct/layout mismatch");

const D3D12_INPUT_LAYOUT_DESC VertexPositionNormalColorTexture::InputLayout =
{
    VertexPositionNormalColorTexture::InputElements,
    VertexPositionNormalColorTexture::InputElementCount
};

/****************************************************************************
*							 VertexPositionNormalTextureNormal
*****************************************************************************/
const D3D12_INPUT_ELEMENT_DESC VertexPositionNormalTextureTangent::InputElements[] =
{
    { "POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "TANGENT",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    { "BINORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};