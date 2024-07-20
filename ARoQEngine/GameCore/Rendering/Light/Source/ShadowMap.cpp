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
#include "../../PostProcess/Include/Blur.hpp"
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
using namespace engine;
using namespace rhi::core;
using namespace gm;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 
ShadowMap::ShadowMap(const LowLevelGraphicsEnginePtr& engine, const std::uint32_t width, const std::uint32_t height, const gu::tstring& addName)
	: _engine(engine)
{
	/*-------------------------------------------------------------------
	-            Error log
	---------------------------------------------------------------------*/
	Checkf(_engine, "engine is nullptr");
	Checkf(width > 0 , "width is non zero");
	Checkf(height > 0, "height is non zero");

	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	gu::tstring name = SP(""); if (name != SP("")) { name += addName; name += SP("::"); }
	name += SP("ShadowMap::");

	/*-------------------------------------------------------------------
	-            Prepare render resource
	---------------------------------------------------------------------*/
	PrepareVertexAndIndexBuffer(name);
	PrepareRenderResource(width, height, name);
	PreparePipelineState(name);

	_gaussianBlur = gu::MakeShared<GaussianBlur>(_engine, width, height, false);
}

ShadowMap::~ShadowMap()
{
	_gameModels.Clear();
	_gameModels.ShrinkToFit();
}

#pragma endregion Constructor and Destructor 

#pragma region Main Function
/****************************************************************************
*                     Draw
****************************************************************************/
/* @fn        void ShadowMap::Draw(const ResourceViewPtr& scene)
*
*  @brief     Draw the shadow map to the frame buffer. 
*             In addition, we apply the gaussian blur for the VSM method.
*
*  @param[in] const ResourceViewPtr& scene resource view of the light camera.
*
*  @return 　　void
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
	for (size_t i = 0; i < _gameModels.Size(); ++i)
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
****************************************************************************/
/* @fn        void ShadowMap::Add(const GameModelPtr& gameModel)
*
*  @brief     Add game models for the rendering shadow
*
*  @param[in] const GameModelPtr& gameModel shared pointer.
*
*  @return 　　void
*****************************************************************************/
void ShadowMap::Add(const GameModelPtr& gameModel)
{
	if (!gameModel) { return; }

	_gameModels.Push(gameModel);
}
#pragma endregion Main Function

#pragma region Set Up Function
/****************************************************************************
*                     PrepareVertexAndIndexBuffer
****************************************************************************/
/* @fn        void ShadowMap::PrepareVertexAndIndexBuffer(const gu::tstring& name)
*
*  @brief     Prepare the vertex and index buffers. These buffers were created for the frame count.
*
*  @param[in] const gu::tstring& debug name
*
*  @return 　　void
*****************************************************************************/
void ShadowMap::PrepareVertexAndIndexBuffer(const gu::tstring& name)
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
		const auto vbMetaData = GPUBufferMetaData::VertexBuffer((gu::uint32)vertexByteSize, (gu::uint32)vertexCount, MemoryHeap::Upload);
		_vertexBuffers[i] = device->CreateBuffer(vbMetaData);
		_vertexBuffers[i]->SetName(name + SP("VB"));
		_vertexBuffers[i]->UploadByte(rectMesh.Vertices.data(), vbMetaData.GetTotalByte()); // Map

		/*-------------------------------------------------------------------
		-            Set Index Buffer
		---------------------------------------------------------------------*/
		const auto ibMetaData = GPUBufferMetaData::IndexBuffer((gu::uint32)indexByteSize, (gu::uint32)indexCount, MemoryHeap::Default, ResourceState::Common);
		_indexBuffers[i] = device->CreateBuffer(ibMetaData);
		_indexBuffers[i]->SetName(name + SP("IB"));
		_indexBuffers[i]->UploadByte(rectMesh.Indices.data(), ibMetaData.GetTotalByte(), 0, commandList);

	}
}

/****************************************************************************
*                     PrepareRenderResource
****************************************************************************/
/* @fn        void ShadowMap::PrepareRenderResource(const std::uint32_t width, const std::uint32_t height, const gu::tstring& name)
*
*  @brief     Prepare the renderPass and frame buffer
*
*  @param[in] const std::uint32_t textureWidth
*  @param[in] const std::uint32_t textureHeight
*  @param[in] const gu::tstring& debug name
*
*  @return 　　void
*****************************************************************************/
void ShadowMap::PrepareRenderResource(const std::uint32_t width, const std::uint32_t height, const gu::tstring& name)
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
	const auto renderTexture = device->CreateTexture(renderInfo, name + SP("FrameBuffer"));
	const auto depthTexture  = device->CreateTexture(depthInfo , name + SP("FrameBufferDepth"));
	_frameBuffer = device->CreateFrameBuffer(_renderPass, renderTexture, depthTexture);
}

/****************************************************************************
*                     PreparePipelineState
****************************************************************************/
/* @fn        void ShadowMap::PreparePipelineState(const gu::tstring& name)
* 
*  @brief     Prepare the vertex and index buffers. These buffers we
*
*  @param[in] const gu::tstring& debugName
*
*  @return 　　void
*****************************************************************************/
void ShadowMap::PreparePipelineState(const gu::tstring& name)
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
	gu::tstring defaultPath = SP("Shader\\Lighting\\ShaderShadowMap.hlsl");
	
	const auto vs = factory->CreateShaderState();
	const auto ps = factory->CreateShaderState();
	vs->Compile({ ShaderType::Vertex, defaultPath, SP("VSMain"), {SP("Shader\\Core")} });
	ps->Compile({ ShaderType::Pixel , defaultPath, SP("PSMain"), {SP("Shader\\Core")} });

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
	_pipeline->SetName(name + SP("PSO"));
}
#pragma endregion Set Up Function