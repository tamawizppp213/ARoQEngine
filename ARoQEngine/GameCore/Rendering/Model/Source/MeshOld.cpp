//////////////////////////////////////////////////////////////////////////////////
//              @file   Mesh.cpp
///             @brief  Mesh Drawer Module
///             @author Copyright(c) Pocol. All right reserved.
///                     Partially edit by Toide Reference : DirectX12 ���H�K�C�h 
///             @date   2022_06_02
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/MeshOld.hpp"
#include "../Include/PrimitiveMesh.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace engine;
using namespace rhi::core;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
Mesh::~Mesh()
{
	_vertexBuffers.Clear(); _vertexBuffers.ShrinkToFit();
	_indexBuffer.Reset();
}

Mesh::Mesh(const LowLevelGraphicsEnginePtr& engine, const PrimitiveMesh& mesh, const MaterialPtr& material, const gu::tstring& addName)
	: _engine(engine), _material(material)
{
	/*-------------------------------------------------------------------
	-            Set name
	---------------------------------------------------------------------*/
	gu::tstring name = SP(""); if (addName != SP("")) { name += addName; name += SP("::"); }
	name += SP("Mesh::");

	/*-------------------------------------------------------------------
	-            Prepare each buffer
	---------------------------------------------------------------------*/
	Prepare(mesh, name);
}

Mesh::Mesh(const LowLevelGraphicsEnginePtr& engine,
	const rhi::core::GPUBufferMetaData& vertexInfo,
	const rhi::core::GPUBufferMetaData& indexInfo,
	const MaterialPtr& material,
	const gu::tstring& addName)
	: _engine(engine), _material(material)
{
	/*-------------------------------------------------------------------
	-            Set name
	---------------------------------------------------------------------*/
	gu::tstring name = SP(""); if (addName != SP("")) { name += addName; name += SP("::"); }
	name += SP("Mesh::");

	/*-------------------------------------------------------------------
	-            Prepare each buffer
	---------------------------------------------------------------------*/
	Prepare(vertexInfo, indexInfo, name);
}

Mesh::Mesh(const LowLevelGraphicsEnginePtr& engine, const gu::DynamicArray<GPUBufferPtr>& vertexBuffers,
	const GPUBufferPtr& indexBuffer,
	const uint64 indexCount, 
	const uint32 indexOffset,
	const MaterialPtr& material):
	_engine(engine), _vertexBuffers(vertexBuffers), _indexBuffer(indexBuffer), _indexCount(indexCount), _material(material), _indexOffset(indexOffset)
{
	if (LowLevelGraphicsEngine::FRAME_BUFFER_COUNT != vertexBuffers.Size())
	{
		OutputDebugStringA("FrameBufferCount is not same with vertexBuffer size");
		return;
	}

	_hasCreatedNewBuffer = false;
}
#pragma endregion Constructor and Destructor

/****************************************************************************
*					Draw
****************************************************************************/
/* @fn        void Mesh::Draw()
* 
*  @brief     Mesh Renderer must need. (�Œ����DrawCall)
* 
*  @param[in] gu::SharedPointer<RHICommandList>& graphicsCommandList
*  @param[in] std::uint32_t currentFrameIndex
* 
*  @return �@�@void
*****************************************************************************/
void Mesh::Draw(const RHICommandListPtr& commandList, const uint32 frameIndex)
{
	Check(frameIndex < LowLevelGraphicsEngine::FRAME_BUFFER_COUNT);
	Check(commandList->GetType() == CommandListType::Graphics);

	if (_hasCreatedNewBuffer)
	{
		commandList->SetPrimitiveTopology(PrimitiveTopology::TriangleList);
		commandList->SetVertexBuffer(_vertexBuffers[frameIndex]);
		commandList->SetIndexBuffer(_indexBuffer);
	}

	commandList->DrawIndexedInstanced(static_cast<uint32>(_indexCount), 1, _indexOffset);
}

/****************************************************************************
*					Prepare
****************************************************************************/
/* @fn        void Mesh::Prepare(const GPUBufferMetaData& vertexInfo, const GPUBufferMetaData& indexInfo, const gu::tstring& name)
*
*  @brief     Prepare vertex and index buffer
*
*  @param[in] const PrimitiveMesh& mesh
*  @param[in] const gu::tstring& name
*
*  @return �@�@void
*****************************************************************************/
void Mesh::Prepare(const PrimitiveMesh& mesh, const gu::tstring& name)
{
	const auto totalFrameSize  = LowLevelGraphicsEngine::FRAME_BUFFER_COUNT;
	const auto device          = _engine->GetDevice();
	const auto copyCommandList = _engine->GetCommandList(CommandListType::Copy);

	/*-------------------------------------------------------------------
	-             Vertex Buffer
	---------------------------------------------------------------------*/
	_vertexBuffers.Resize(totalFrameSize);
	for (size_t i = 0; i < totalFrameSize; ++i)
	{
		const auto metaData = GPUBufferMetaData::VertexBuffer(sizeof(gm::Vertex), (gu::uint32)mesh.Vertices.size(), rhi::core::MemoryHeap::Upload);
		_vertexBuffers[i] = device->CreateBuffer(metaData);
		_vertexBuffers[i]->SetName(name + SP("VB"));
		_vertexBuffers[i]->UploadByte(mesh.Vertices.data(), metaData.GetTotalByte(), 0, nullptr);
	}

	/*-------------------------------------------------------------------
	-              Index Buffer
	---------------------------------------------------------------------*/
	{
		const auto metaData = GPUBufferMetaData::IndexBuffer(sizeof(std::uint32_t), (gu::uint32)mesh.Indices.size(), MemoryHeap::Default, ResourceState::Common);
		_indexCount = mesh.Indices.size();

		_indexBuffer = device->CreateBuffer(metaData);
		_indexBuffer->SetName(name + SP("IB"));
		_indexBuffer->UploadByte(mesh.Indices.data(), metaData.GetTotalByte(), 0, copyCommandList);
	}

	_hasCreatedNewBuffer = true;
}

/****************************************************************************
*					Prepare
****************************************************************************/
/* @fn        void Mesh::Prepare(const GPUBufferMetaData& vertexInfo, const GPUBufferMetaData& indexInfo, const gu::tstring& name)
* 
*  @brief     Prepare vertex and index buffer
* 
*  @param[in] const GPUBufferMetaData vertexInfo
*  @param[in] const GPUBufferMetaData indexInfo
*  @param[in] const gu::tstring& name
* 
*  @return �@�@void
*****************************************************************************/
void Mesh::Prepare(const GPUBufferMetaData& vertexInfo, const GPUBufferMetaData& indexInfo, const gu::tstring& name)
{
	if (gu::HasAnyFlags(vertexInfo.Usage, BufferCreateFlags::VertexBuffer)) { OutputDebugStringA("Please set vertex buffer\n"); return; }
	if (gu::HasAnyFlags(indexInfo .Usage, BufferCreateFlags::IndexBuffer))  { OutputDebugStringA("Please set index buffer\n "); return ;}

	const auto totalFrameSize  = LowLevelGraphicsEngine::FRAME_BUFFER_COUNT;
	const auto device          = _engine->GetDevice();
	const auto copyCommandList = _engine->GetCommandList(CommandListType::Copy);

	/*-------------------------------------------------------------------
	-             Vertex Buffer
	---------------------------------------------------------------------*/
	_vertexBuffers.Resize(totalFrameSize);
	for (size_t i = 0; i < totalFrameSize; ++i)
	{
		_vertexBuffers[i] = device->CreateBuffer(vertexInfo);

		_vertexBuffers[i]->SetName(name + SP("VB"));

		if (vertexInfo.InitData) { _vertexBuffers[i]->UploadByte(vertexInfo.InitData, vertexInfo.GetTotalByte()); }
	}

	/*-------------------------------------------------------------------
	-              Index Buffer
	---------------------------------------------------------------------*/
	{
		_indexBuffer = device->CreateBuffer(indexInfo);

		_indexBuffer->SetName(name + SP("IB"));

		_indexCount = indexInfo.Count;

		if (indexInfo.InitData) { _indexBuffer->UploadByte(indexInfo.InitData, indexInfo.GetTotalByte(), 0, copyCommandList); }
	}

	_hasCreatedNewBuffer = true;
}