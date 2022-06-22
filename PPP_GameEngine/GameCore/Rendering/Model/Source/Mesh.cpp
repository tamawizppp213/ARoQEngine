//////////////////////////////////////////////////////////////////////////////////
//              @file   Mesh.cpp
///             @brief  Mesh Drawer Module
///             @author Copyright(c) Pocol. All right reserved.
///                     Partially edit by Toide Reference : DirectX12 実践ガイド 
///             @date   2022_06_02
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/Include/Mesh.hpp"
#include "GraphicsCore/Engine/Include/GraphicsCoreEngine.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12PrimitiveGeometry.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::directX12;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
Mesh::~Mesh()
{

}
bool Mesh::Initialize(const MeshData& meshData, const std::wstring& addName)
{
	/*-------------------------------------------------------------------
	-            Set name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"Mesh::";

	PrepareVertexAndIndexBuffer(meshData, name);
	_materialID = meshData.MaterialID;
	return true;
}
bool Mesh::Initialize(const void* vertexData, UINT vertexByteSize, UINT vertexCount, const void* indexData, UINT indexByteSize, UINT indexCount, UINT materialID, bool hasSkin, const std::wstring& addName)
{
	/*-------------------------------------------------------------------
	-            Set name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"Mesh::";

	PrepareVertexAndIndexBuffer(vertexData, vertexByteSize, vertexCount, indexData, indexByteSize, indexCount,name);
	_materialID = materialID;
	_hasSkin    = hasSkin;
	return true;
}
/****************************************************************************
*					Draw
*************************************************************************//**
*  @fn        void Mesh::Draw(CommandContext* context, int currentFrameIndex)
*  @brief     Mesh Renderer must need. (最低限のDrawCall)
*  @param[in] CommandContext* context
*  @param[in] int currentFrameIndex
*  @return 　　void
*****************************************************************************/
void Mesh::Draw(rhi::directX12::CommandContext* context, int currentFrameIndex)
{
	assert(currentFrameIndex < GraphicsCoreEngine::Instance().GetFrameBufferCount());
	context->SetPrimitiveTopology(rhi::core::PrimitiveTopology::TriangleList);
	context->SetVertexBuffer(_meshBuffer[currentFrameIndex].VertexBufferView());
	context->SetIndexBuffer (_meshBuffer[currentFrameIndex].IndexBufferView());
	context->DrawIndexedInstanced(_indexCount, 1, 0);
}



void Mesh::Terminate()
{
	_meshBuffer.get()->Dispose(); _meshBuffer.reset();
	_materialID = 0;
}
/****************************************************************************
*					PrepareVertexAndIndexBuffer
*************************************************************************//**
*  @fn        void Mesh::PrepareVertexAndIndexBuffer(const MeshData& meshData, const std::wstring& name)
*  @brief     Prepare Rect vertex and index buffer
*  @param[in] const MeshData& meshData
*  @param[in] const std::wstring& name
*  @return 　　void
*****************************************************************************/
void Mesh::PrepareVertexAndIndexBuffer(const MeshData& meshData, const std::wstring& name)
{
	auto& engine         = GraphicsCoreEngine::Instance();
	int   totalFrameSize = engine.GetFrameBufferCount();
	_meshBuffer          = std::make_unique<MeshBuffer[]>(totalFrameSize);

	for (int i = 0; i < totalFrameSize; ++i)
	{
		auto& meshBuffer = _meshBuffer[i];
		/*-------------------------------------------------------------------
		-            Calcurate Buffer Size
		---------------------------------------------------------------------*/
		auto vertexByteSize = sizeof(VertexPositionNormalColorTexture);       
		auto indexByteSize  = sizeof(UINT32);                                // 32 byte index
		auto vertexCount    = meshData.Vertices.size();
		auto indexCount     = meshData.Indices.size();
		/*-------------------------------------------------------------------
		-            Set Vertex Buffer and Index Buffer
		---------------------------------------------------------------------*/
		meshBuffer.VertexBuffer = std::make_unique<UploadBuffer>(engine.GetDevice(), static_cast<UINT>(vertexByteSize), static_cast<UINT>(vertexCount), false, name + L"VertexBuffer");
		meshBuffer.VertexBuffer->CopyStart();
		meshBuffer.VertexBuffer->CopyTotalData(meshData.Vertices.data(), static_cast<int>(vertexCount));
		meshBuffer.VertexBuffer->CopyEnd();

		meshBuffer.IndexBuffer = std::make_unique<UploadBuffer>(engine.GetDevice(), static_cast<UINT>(indexByteSize), static_cast<UINT>(indexCount), false, name + L"IndexBuffer");
		meshBuffer.IndexBuffer->CopyStart();
		meshBuffer.IndexBuffer->CopyTotalData(meshData.Indices.data(), static_cast<int>(indexCount));
		meshBuffer.IndexBuffer->CopyEnd();
	}
}
/****************************************************************************
*					PrepareVertexAndIndexBuffer
*************************************************************************//**
*  @fn        void Mesh::PrepareVertexAndIndexBuffer(const void* vertexData, UINT vertexByteSize, UINT vertexCount, const void* indexData, UINT indexByteSize, UINT indexCount, const std::wstring& name)
*  @brief     Prepare Rect vertex and index buffer
*  @param[in] const void* vertexData
*  @param[in] UINT vertexByteSize
*  @param[in] UINT vertexCount
*  @param[in] const void* indexData
*  @param[in] UINT indexByteSize
*  @param[in] UINT indexCount
*  @return 　　void
*****************************************************************************/
void Mesh::PrepareVertexAndIndexBuffer(const void* vertexData, UINT vertexByteSize, UINT vertexCount, const void* indexData, UINT indexByteSize, UINT indexCount, const std::wstring& name)
{
	auto& engine         = GraphicsCoreEngine::Instance();
	int   totalFrameSize = engine.GetFrameBufferCount();
	_meshBuffer          = std::make_unique<MeshBuffer[]>(totalFrameSize);

	for (int i = 0; i < totalFrameSize; ++i)
	{
		auto& meshBuffer = _meshBuffer[i];
		/*-------------------------------------------------------------------
		-            Calcurate Buffer Size
		---------------------------------------------------------------------*/
		/*-------------------------------------------------------------------
		-            Set Vertex Buffer and Index Buffer
		---------------------------------------------------------------------*/
		meshBuffer.VertexBuffer = std::make_unique<UploadBuffer>(engine.GetDevice(), static_cast<UINT>(vertexByteSize), static_cast<UINT>(vertexCount), false, name + L"VertexBuffer");
		meshBuffer.VertexBuffer->CopyStart();
		meshBuffer.VertexBuffer->CopyTotalData(vertexData, vertexCount);
		meshBuffer.VertexBuffer->CopyEnd();

		meshBuffer.IndexBuffer = std::make_unique<UploadBuffer>(engine.GetDevice(), static_cast<UINT>(indexByteSize), static_cast<UINT>(indexCount), false, name + L"IndexBuffer");
		meshBuffer.IndexBuffer->CopyStart();
		meshBuffer.IndexBuffer->CopyTotalData(indexData, indexCount);
		meshBuffer.IndexBuffer->CopyEnd();
	}
}