//////////////////////////////////////////////////////////////////////////////////
//              @file   a.cpp
///             @brief  a
///             @author Toide Yutaro
///             @date   -
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/ShadowMap.hpp"
#include "../../Model/Include/GameModel.hpp"
#include "../../Effect/Include/Blur.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"
#include "GameUtility/Math/Include/GMVertex.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::core;
using namespace gc::rendering;
using namespace rhi::core;
using namespace gm;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 
ShadowMap::ShadowMap(const LowLevelGraphicsEnginePtr& engine, const std::uint32_t width, const std::uint32_t height, const std::wstring& addName)
	: _engine(engine)
{
	/*-------------------------------------------------------------------
	-            Error log
	---------------------------------------------------------------------*/
	assert(("engine is nullptr", _engine));
	assert(("width is non zero" , width > 0));
	assert(("height is non zero", height > 0));

	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (name != L"") { name += addName; name += L"::"; }
	name += L"ShadowMap::";

	/*-------------------------------------------------------------------
	-            Prepare render resource
	---------------------------------------------------------------------*/
	PrepareVertexAndIndexBuffer(name);
	PrepareRenderResource(width, height, name);
	PreparePipelineState(name);

	_gaussianBlur = std::make_shared<gc::GaussianBlur>(_engine, width, height, false);
}

ShadowMap::~ShadowMap()
{
	_gameModels.clear();
	_gameModels.shrink_to_fit();
}

#pragma endregion Constructor and Destructor 

#pragma region Main Function
/****************************************************************************
*                     Draw
*************************************************************************//**
*  @fn        void ShadowMap::Draw(const ResourceViewPtr& scene)
*
*  @brief     Draw the shadow map to the frame buffer. 
*             In addition, we apply the gaussian blur for the VSM method.
*
*  @param[in] const ResourceViewPtr& scene resource view of the light camera.
*
*  @return �@�@void
*****************************************************************************/
void ShadowMap::Draw(const ResourceViewPtr& scene)
{
	/*-------------------------------------------------------------------
	-               Set variables
	---------------------------------------------------------------------*/
	const auto device      = _engine->GetDevice();
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics);

	/*-------------------------------------------------------------------
	-               Change render resource
	---------------------------------------------------------------------*/
	// if not close the render pass, we close the current render pass. 
	commandList->EndRenderPass();

	// clear previous frame rendered frame buffer.
	commandList->BeginRenderPass(_renderPass, _frameBuffer);

	/*-------------------------------------------------------------------
	-               Draw render 
	---------------------------------------------------------------------*/
	commandList->SetGraphicsPipeline(_pipeline);
	commandList->SetResourceLayout  (_resourceLayout);
	scene->Bind(commandList, 0);
	for (size_t i = 0; i < _gameModels.size(); ++i)
	{
		_gameModels[i]->Draw(false);
	}

	/*-------------------------------------------------------------------
	-         Apply the gaussian blur for the VSM method.
	---------------------------------------------------------------------*/
	_gaussianBlur->DrawPS(_frameBuffer, 0);
}

/****************************************************************************
*							Add
*************************************************************************//**
*  @fn        void ShadowMap::Add(const GameModelPtr& gameModel)
*
*  @brief     Add game models for the rendering shadow
*
*  @param[in] const GameModelPtr& gameModel shared pointer.
*
*  @return �@�@void
*****************************************************************************/
void ShadowMap::Add(const GameModelPtr& gameModel)
{
	if (!gameModel) { return; }

	_gameModels.push_back(gameModel);
}
#pragma endregion Main Function

#pragma region Set Up Function
/****************************************************************************
*                     PrepareVertexAndIndexBuffer
*************************************************************************//**
*  @fn        void ShadowMap::PrepareVertexAndIndexBuffer(const std::wstring& name)
*
*  @brief     Prepare the vertex and index buffers. These buffers were created for the frame count.
*
*  @param[in] const std::wstring& debug name
*
*  @return �@�@void
*****************************************************************************/
void ShadowMap::PrepareVertexAndIndexBuffer(const std::wstring& name)
{
	const auto device      = _engine->GetDevice();
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
	_vertexBuffers.resize(frameCount);
	_indexBuffers .resize(frameCount);
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
		const auto vbMetaData = GPUBufferMetaData::VertexBuffer(vertexByteSize, vertexCount, MemoryHeap::Upload);
		_vertexBuffers[i] = device->CreateBuffer(vbMetaData);
		_vertexBuffers[i]->SetName(name + L"VB");
		_vertexBuffers[i]->Pack(rectMesh.Vertices.data()); // Map

		/*-------------------------------------------------------------------
		-            Set Index Buffer
		---------------------------------------------------------------------*/
		const auto ibMetaData = GPUBufferMetaData::IndexBuffer(indexByteSize, indexCount, MemoryHeap::Default, ResourceState::Common);
		_indexBuffers[i] = device->CreateBuffer(ibMetaData);
		_indexBuffers[i]->SetName(name + L"IB");
		_indexBuffers[i]->Pack(rectMesh.Indices.data(), commandList);

	}
}

/****************************************************************************
*                     PrepareRenderResource
*************************************************************************//**
*  @fn        void ShadowMap::PrepareRenderResource(const std::uint32_t width, const std::uint32_t height, const std::wstring& name)
*
*  @brief     Prepare the renderPass and frame buffer
*
*  @param[in] const std::uint32_t textureWidth
*  @param[in] const std::uint32_t textureHeight
*  @param[in] const std::wstring& debug name
*
*  @return �@�@void
*****************************************************************************/
void ShadowMap::PrepareRenderResource(const std::uint32_t width, const std::uint32_t height, const std::wstring& name)
{
	const auto device = _engine->GetDevice();

	/*-------------------------------------------------------------------
	-             Prepare render pass
	---------------------------------------------------------------------*/
	const auto renderFormat    = PixelFormat::R8G8B8A8_UNORM;
	const auto depthFormat     = PixelFormat::D32_FLOAT;
	const auto clearColor      = ClearValue(1.0f, 1.0f, 1.0f, 1.0f); // white
	const auto clearDepth      = ClearValue(1.0f, 0); // black
	const auto colorAttachment = Attachment::RenderTarget(renderFormat);
	const auto depthAttachment = Attachment::DepthStencil(depthFormat);

	_renderPass = device->CreateRenderPass(colorAttachment, depthAttachment);
	_renderPass->SetClearValue(clearColor, clearDepth);

	/*-------------------------------------------------------------------
	-             Prepare frame buffer
	---------------------------------------------------------------------*/
	const auto renderInfo    = GPUTextureMetaData::RenderTarget(width, height, renderFormat, clearColor);
	const auto depthInfo     = GPUTextureMetaData::DepthStencil(width, height, depthFormat , clearDepth);
	const auto renderTexture = device->CreateTexture(renderInfo, name + L"FrameBuffer");
	const auto depthTexture  = device->CreateTexture(depthInfo , name + L"FrameBufferDepth");
	_frameBuffer = device->CreateFrameBuffer(_renderPass, renderTexture, depthTexture);
}

/****************************************************************************
*                     PreparePipelineState
*************************************************************************//**
*  @fn        void ShadowMap::PreparePipelineState(const std::wstring& name)
* 
*  @brief     Prepare the vertex and index buffers. These buffers we
*
*  @param[in] const std::wstring& debugName
*
*  @return �@�@void
*****************************************************************************/
void ShadowMap::PreparePipelineState(const std::wstring& name)
{
	const auto device  = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();


	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	_resourceLayout = device->CreateResourceLayout
	(
		{ ResourceLayoutElement(DescriptorHeapType::CBV, 0),
		  ResourceLayoutElement(DescriptorHeapType::CBV, 1),
		}, // light camera,
		{}
	);

	/*-------------------------------------------------------------------
	-			Load Blob data
	---------------------------------------------------------------------*/
	std::wstring defaultPath = L"Shader\\Lighting\\ShaderShadowMap.hlsl";
	
	const auto vs = factory->CreateShaderState();
	const auto ps = factory->CreateShaderState();
	vs->Compile(ShaderType::Vertex, defaultPath, L"VSMain", 6.4f , {L"Shader\\Core"});
	ps->Compile(ShaderType::Pixel , defaultPath, L"PSMain", 6.4f,  {L"Shader\\Core"});

	/*-------------------------------------------------------------------
	-			Create graphics pipeline
	---------------------------------------------------------------------*/
	_pipeline = device->CreateGraphicPipelineState(_engine->GetRenderPass(), _resourceLayout);
	_pipeline->SetBlendState        (factory->CreateSingleBlendState(BlendProperty::OverWrite())); // not use alpha blend
	_pipeline->SetRasterizerState   (factory->CreateRasterizerState(RasterizerProperty::Solid()));
	_pipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultVertexElement()));
	_pipeline->SetDepthStencilState (factory->CreateDepthStencilState());
	_pipeline->SetVertexShader(vs);
	_pipeline->SetPixelShader (ps);
	_pipeline->CompleteSetting();
	_pipeline->SetName(name + L"PSO");
}
#pragma endregion Set Up Function