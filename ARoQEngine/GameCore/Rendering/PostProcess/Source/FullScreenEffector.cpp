//////////////////////////////////////////////////////////////////////////////////
//              @file   ColorChange.cpp
///             @brief  Color change post effect
///             @author Toide Yutaro
///             @date   2022_04_30
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/PostProcess/Include/FullScreenEffector.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"
#include "GameCore/Rendering/Model/Include/PrimitiveMesh.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gm;
using namespace engine;
using namespace gu;
using namespace rhi;
using namespace rhi::core;

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
/*!**********************************************************************
*  @brief     頂点バッファとインデックスバッファを準備します.
*  @param[in] const gu::tstring& デバッグ表示名
*  @return    void
*************************************************************************/
void IFullScreenEffector::PrepareVertexAndIndexBuffer(const gu::tstring& addName)
{
	const auto device     = _engine->GetDevice();
	const auto commandList = _engine->GetCommandList(CommandListType::Copy);
	/*-------------------------------------------------------------------
	-            Create Sphere Mesh
	---------------------------------------------------------------------*/
	struct ScreenVertex
	{
		Float2 Position;
		Float2 TexCoord;
	};

	DynamicArray<ScreenVertex> vertices = 
	{
		{Float2(-1.0f,  -1.0f), Float2(0.0f, 1.0f)},
		{Float2(-1.0f,   1.0f), Float2(0.0f, 0.0f)},
		{Float2( 1.0f,   1.0f), Float2(1.0f, 0.0f)},
		{Float2( 1.0f,  -1.0f) , Float2(1.0f, 1.0f)}
	};

	DynamicArray<gu::uint16> indices = { 0, 1, 2, 0, 2, 3 };

	/*-------------------------------------------------------------------
	-            Create Mesh Buffer
	---------------------------------------------------------------------*/
	const auto frameCount = LowLevelGraphicsEngine::FRAME_BUFFER_COUNT;

	// prepare frame count buffer
	_vertexBuffers.Resize(frameCount);
	for (uint16 i = 0; i < frameCount; ++i)
	{
		/*-------------------------------------------------------------------
		-            Set up
		---------------------------------------------------------------------*/
		auto vertexByteSize = sizeof(ScreenVertex);
		auto vertexCount    = vertices.Size();

		/*-------------------------------------------------------------------
		-            Set Vertex Buffer 
		---------------------------------------------------------------------*/
		const auto vbMetaData = GPUBufferMetaData::VertexBuffer(static_cast<uint32>(vertexByteSize), static_cast<uint32>(vertexCount), MemoryHeap::Upload);
		_vertexBuffers[i] = device->CreateBuffer(vbMetaData);
		_vertexBuffers[i]->SetName(addName + SP("VB"));
		_vertexBuffers[i]->UploadByte(vertices.Data(), vbMetaData.GetTotalByte()); // Map

	}

	/*-------------------------------------------------------------------
	-            Set Index Buffer
	---------------------------------------------------------------------*/
	const auto ibMetaData = GPUBufferMetaData::IndexBuffer(static_cast<uint32>(sizeof(gu::uint16)), 6, MemoryHeap::Default, ResourceState::Common);
	_indexBuffer = device->CreateBuffer(ibMetaData);
	_indexBuffer->SetName(addName + SP("IB"));
	_indexBuffer->UploadByte(indices.Data(), ibMetaData.GetTotalByte(), 0, commandList);
}

/*!**********************************************************************
*  @brief     デバッグ表示名を準備します. ただし, デフォルト値でFSEffector::が追加されます.
*  @param[in] const gu::tstring 加えたい文字列
*  @param[in] const gu::tstring 現在の文字列
*  @return    gu::tstring
*************************************************************************/
gu::tstring IFullScreenEffector::CombineDebugName(const gu::tstring& addName, const gu::tstring& original) const
{
	if (addName.IsEmpty()) { return original; }

	return addName + SP("::") + original;
}

/*!**********************************************************************
*  @brief     FullScreenStateの基本のグラフィクスパイプラインの設定を行います.
*  @param[in] const gu::SharedPointer<rhi::core::RHIRenderPass>& レンダーパス
*  @param[in] const gu::SharedPointer<rhi::core::RHIResourceLayout>& リソースレイアウト
*  @return    void
*************************************************************************/
IFullScreenEffector::PipelineStatePtr IFullScreenEffector::CreateDefaultFullScreenGraphicsPipelineState (
	const gu::SharedPointer<rhi::core::RHIRenderPass>& renderPass,
	const gu::SharedPointer<rhi::core::RHIResourceLayout>& resourceLayout, 
	const gu::SharedPointer<rhi::core::GPUShaderState>& vs, 
	const gu::SharedPointer<rhi::core::GPUShaderState>& ps) const
{
	const auto device = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();

	// DepthStencilPropertyの設定
	DepthStencilProperty depthStencilProperty = {};
	depthStencilProperty.DepthWriteEnable = false;
	depthStencilProperty.DepthOperator    = core::CompareOperator::Always;

	const auto pipeline = device->CreateGraphicPipelineState(renderPass, resourceLayout);
	pipeline->SetVertexShader(vs);
	pipeline->SetPixelShader(ps);
	pipeline->SetBlendState        (factory->CreateSingleBlendState(BlendProperty::OverWrite(true)));
	pipeline->SetRasterizerState   (factory->CreateRasterizerState(RasterizerProperty::Solid()));
	pipeline->SetDepthStencilState (factory->CreateDepthStencilState(depthStencilProperty));
	pipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultScreenElement()));
	return pipeline;
}

/*!**********************************************************************
*  @brief     FullScreenStateの基本のComputeパイプラインの設定を行います.
*  @param[in] void
*  @return    void
*************************************************************************/
IFullScreenEffector::ComputePipelineStatePtr IFullScreenEffector::CreateDefaultFullScreenComputePipelineState(
	const gu::SharedPointer<rhi::core::RHIResourceLayout>& resourceLayout,
	const gu::SharedPointer<rhi::core::GPUShaderState>& cs
)
{
	const auto device   = _engine->GetDevice();
	const auto pipeline = device->CreateComputePipelineState(resourceLayout);
	pipeline->SetComputeShader(cs);
	return pipeline;
}


#pragma endregion Protected Function