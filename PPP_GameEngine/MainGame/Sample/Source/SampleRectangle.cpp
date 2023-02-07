//////////////////////////////////////////////////////////////////////////////////
//              @file   SampleSky.cpp
///             @brief  SampleSky
///             @author Toide Yutaro
///             @date   2022_04_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Sample/Include/SampleRectangle.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include "GameCore/Rendering/Model/Include/PrimitiveMesh.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIRenderPass.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace sample;
using namespace rhi;
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
SampleRectangle::SampleRectangle()
{

}
SampleRectangle::~SampleRectangle()
{

}
#pragma region Public Function
/****************************************************************************
*                       Initialize
*************************************************************************//**
*  @fn        void SampleRectangle::Initialize( const GameTimerPtr& gameTimer)
*  @brief     Initialize scene
*  @param[in]  const GameTimerPtr& gameTimer
*  @return 　　void
*****************************************************************************/
void SampleRectangle::Initialize(const std::shared_ptr<LowLevelGraphicsEngine>& engine, const GameTimerPtr& gameTimer)
{
	Scene::Initialize(engine, gameTimer);
}
/****************************************************************************
*                       Update
*************************************************************************//**
*  @fn        void SampleRectangle::Update()
*  @brief     Update Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleRectangle::Update()
{
	Scene::Update();
}
/****************************************************************************
*                       Draw
*************************************************************************//**
*  @fn        void SampleRectangle::Draw()
*  @brief     Draw Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleRectangle::Draw()
{
	/*-------------------------------------------------------------------
	-             Start frame
	---------------------------------------------------------------------*/
	_engine->BeginDrawFrame();
	_engine->BeginSwapchainRenderPass();
	/*-------------------------------------------------------------------
	-             Regist graphics pipeline command
	---------------------------------------------------------------------*/
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics, _engine->GetCurrentFrameIndex());
	commandList->SetResourceLayout(_resourceLayout);
	commandList->SetGraphicsPipeline(_pipelineState);
	commandList->SetViewportAndScissor(
		core::Viewport(0, 0, (float)Screen::GetScreenWidth(), (float)Screen::GetScreenHeight()), 
		core::ScissorRect(0, 0, (long)Screen::GetScreenWidth(), (long)Screen::GetScreenHeight()));
	commandList->SetVertexBuffer(_vertexBuffer);
	commandList->SetIndexBuffer (_indexBuffer);
	commandList->DrawIndexedInstanced(static_cast<std::uint32_t>(_indexBuffer->GetElementCount()), 1);
	/*-------------------------------------------------------------------
	-             End frame
	---------------------------------------------------------------------*/
	_engine->EndDrawFrame();
}
/****************************************************************************
*                       Terminate
*************************************************************************//**
*  @fn        void SampleRectangle::Terminate()
*  @brief     Terminate Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleRectangle::Terminate()
{
	if (_resourceLayout) { _resourceLayout.reset(); }
	if (_pipelineState) { _pipelineState.reset(); }
	if (_indexBuffer ) { _indexBuffer.reset(); }
	if (_vertexBuffer) { _vertexBuffer.reset(); }
}
#pragma endregion Public Function

#pragma region Protected Function
/****************************************************************************
*                       LoadMaterials
*************************************************************************//**
*  @fn        void SampleRectangle::LoadMaterials(GameTimer* gameTimer)
*  @brief     Load Materials
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleRectangle::LoadMaterials()
{
	/*-------------------------------------------------------------------
	-             Open Copy CommandList
	---------------------------------------------------------------------*/
	const auto commandList = _engine->GetCommandList(CommandListType::Copy, _engine->GetCurrentFrameIndex());
	commandList->BeginRecording();
	/*-------------------------------------------------------------------
	-             SetUp Resources
	---------------------------------------------------------------------*/
	BuildBuffer();
	BuildPipelineState();

	/*-------------------------------------------------------------------
	-             Close Copy CommandList and Flush CommandQueue
	---------------------------------------------------------------------*/
	commandList->EndRecording();
	_engine->FlushCommandQueue(CommandListType::Copy);
}
/****************************************************************************
*                       OnKeyboardInput
*************************************************************************//**
*  @fn        void SampleRectangle::OnKeyboardInput()
*  @brief     KeyboardInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleRectangle::OnKeyboardInput()
{

}
/****************************************************************************
*                       OnMouseInput
*************************************************************************//**
*  @fn        void SampleRectangle::OnMouseInput()
*  @brief     MouseInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleRectangle::OnMouseInput()
{

}
/****************************************************************************
*                       OnGamePadInput
*************************************************************************//**
*  @fn        void SampleRectangle::OnGamePadInput()
*  @brief     GamePadInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleRectangle::OnGamePadInput()
{

}
/****************************************************************************
*                     ExecuteSceneTransition
*************************************************************************//**
*  @fn        void SampleRectangle::ExecuteSceneTranstion()
*  @brief     Scene Transition
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleRectangle::ExecuteSceneTransition()
{

}
#pragma endregion Protected Function
#pragma region SetUp Function
/****************************************************************************
*                     BuildBuffer
*************************************************************************//**
*  @fn        void SampleRectangle::BuildBuffer()
*  @brief     Build Vertex and Index Buffers
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleRectangle::BuildBuffer()
{
	// 頂点情報変えるならFrameCount分用意した方が良い.
	gc::core::PrimitiveMesh rectangle = gc::core::PrimitiveMeshGenerator::Rect
	(1.0f, 1.0f, 1.0f, gm::Float4(1,1,0,1));
	/*-------------------------------------------------------------------
	-             Vertex Buffer
	---------------------------------------------------------------------*/
	{
		GPUBufferMetaData metaData = GPUBufferMetaData::VertexBuffer(
			sizeof(gm::Vertex),
			rectangle.Vertices.size(),
			rhi::core::MemoryHeap::Upload);
		_vertexBuffer = _engine->GetDevice()->CreateBuffer(metaData);
		_vertexBuffer->SetName(L"VertexBuffer");
		_vertexBuffer->Pack(rectangle.Vertices.data());
	}
	/*-------------------------------------------------------------------
	-             Index Buffer
	---------------------------------------------------------------------*/
	{
		GPUBufferMetaData metaData = GPUBufferMetaData::IndexBuffer(
			sizeof(std::uint32_t),
			rectangle.Indices.size(),
			rhi::core::MemoryHeap::Default,
			rhi::core::ResourceState::Common);
		_indexBuffer = _engine->GetDevice()->CreateBuffer(metaData);
		_indexBuffer->SetName(L"IndexBuffer");
		_indexBuffer->Pack(rectangle.Indices.data(), _engine->GetCommandList(CommandListType::Copy, _engine->GetCurrentFrameIndex()));
	
	}
}

void SampleRectangle::BuildPipelineState()
{
	const auto rhiDevice = _engine->GetDevice();
	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	std::vector<SamplerLayoutElement> samplerElements(1);
	samplerElements[0] = SamplerLayoutElement(rhiDevice->CreateSampler(SamplerInfo::GetDefaultSampler(SamplerLinearWrap)),0,0);

	/*-------------------------------------------------------------------
	-             Set up graphics resource layout
	---------------------------------------------------------------------*/
	_resourceLayout = rhiDevice->CreateResourceLayout({}, samplerElements);

	/*-------------------------------------------------------------------
	-             Set up shaders
	---------------------------------------------------------------------*/
	const auto factory      = rhiDevice->CreatePipelineFactory();
	const auto vertexShader = factory->CreateShaderState();
	const auto pixelShader  = factory->CreateShaderState();
	// Compile 
	vertexShader->Compile(ShaderType::Vertex, L"Shader\\Sprite\\ShaderTest.hlsl", L"VSMain", 6.4f, {L"Shader\\Core"});
	pixelShader ->Compile(ShaderType::Pixel , L"Shader\\Sprite\\ShaderTest.hlsl", L"PSTest", 6.4f, { L"Shader\\Core" });

	/*-------------------------------------------------------------------
	-             Set up graphic pipeline state
	---------------------------------------------------------------------*/
	_pipelineState = rhiDevice->CreateGraphicPipelineState(_engine->GetRenderPass(), _resourceLayout);
	_pipelineState->SetBlendState(factory->CreateBlendState());
	_pipelineState->SetRasterizerState(factory->CreateRasterizerState());
	_pipelineState->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultVertexElement()));
	_pipelineState->SetDepthStencilState(factory->CreateDepthStencilState());
	_pipelineState->SetVertexShader(vertexShader);
	_pipelineState->SetPixelShader(pixelShader);
	_pipelineState->CompleteSetting();
}
#pragma endregion SetUp Function