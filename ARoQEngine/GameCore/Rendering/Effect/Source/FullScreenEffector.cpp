//////////////////////////////////////////////////////////////////////////////////
//              @file   ColorChange.cpp
///             @brief  Color change post effect
///             @author Toide Yutaro
///             @date   2022_04_30
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Effect/Include/FullScreenEffector.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GameCore/Rendering/Model/Include/PrimitiveMesh.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gm;
using namespace gc;
using namespace rhi;
using namespace rhi::core;
using namespace gc::core;

//////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
IFullScreenEffector::IFullScreenEffector()
{

}

IFullScreenEffector::~IFullScreenEffector()
{
	_pipeline.Reset();
	_resourceLayout.Reset();
	_resourceViews.Clear(); _resourceViews.ShrinkToFit();
	_indexBuffers.Clear(); _indexBuffers.ShrinkToFit();
	_vertexBuffers.Clear(); _vertexBuffers.ShrinkToFit();
}

IFullScreenEffector::IFullScreenEffector(const LowLevelGraphicsEnginePtr& engine) : _engine(engine)
{
	Check(engine);

	_width  = Screen::GetScreenWidth();
	_height = Screen::GetScreenHeight();
}

#pragma endregion Constructor and Destructor

#pragma region Protected Function
/****************************************************************************
*							PrepareVertexAndIndexBuffer
****************************************************************************/
/* @fn        void IFullScreenEffector::PrepareVertexAndIndexBuffer()
*  @brief     Prepare Rect Vertex and Index Buffer
*  @param[in] const gu::tstring& addName
*  @return @@void
*****************************************************************************/
void IFullScreenEffector::PrepareVertexAndIndexBuffer(const gu::tstring& addName)
{
	const auto device     = _engine->GetDevice();
	const auto commandList = _engine->GetCommandList(CommandListType::Copy);
	/*-------------------------------------------------------------------
	-            Create Sphere Mesh
	---------------------------------------------------------------------*/
	PrimitiveMesh rectMesh = PrimitiveMeshGenerator::Rect(2.0f, 2.0f, 0.0f);
	/*-------------------------------------------------------------------
	-            Create Mesh Buffer
	---------------------------------------------------------------------*/
	const auto frameCount = LowLevelGraphicsEngine::FRAME_BUFFER_COUNT;
	// prepare frame count buffer
	_vertexBuffers.Resize(frameCount);
	_indexBuffers .Resize(frameCount);
	for (std::uint32_t i = 0; i < frameCount; ++i)
	{
		/*-------------------------------------------------------------------
		-            Set up
		---------------------------------------------------------------------*/
		auto vertexByteSize = sizeof(Vertex);
		auto indexByteSize  = sizeof(std::uint32_t);
		auto vertexCount    = rectMesh.Vertices.size();
		auto indexCount     = rectMesh.Indices.size();

		/*-------------------------------------------------------------------
		-            Set Vertex Buffer 
		---------------------------------------------------------------------*/
		const auto vbMetaData = GPUBufferMetaData::VertexBuffer(static_cast<gu::uint32>(vertexByteSize), static_cast<gu::uint32>(vertexCount), MemoryHeap::Upload);
		_vertexBuffers[i] = device->CreateBuffer(vbMetaData);
		_vertexBuffers[i]->SetName(addName + SP("VB"));
		_vertexBuffers[i]->UploadByte(rectMesh.Vertices.data(), vbMetaData.GetTotalByte()); // Map

		/*-------------------------------------------------------------------
		-            Set Index Buffer
		---------------------------------------------------------------------*/
		const auto ibMetaData = GPUBufferMetaData::IndexBuffer(static_cast<gu::uint32>(indexByteSize), static_cast<gu::uint32>(indexCount), MemoryHeap::Default, ResourceState::Common);
		_indexBuffers[i] = device->CreateBuffer(ibMetaData);
		_indexBuffers[i]->SetName(addName + SP("IB"));
		_indexBuffers[i]->UploadByte(rectMesh.Indices.data(), ibMetaData.GetTotalByte(), 0,  commandList);

	}
}


#pragma endregion Protected Function