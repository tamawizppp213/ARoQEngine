//////////////////////////////////////////////////////////////////////////////////
//              @file   Blur.cpp
///             @brief  Blur effect
///             @author Toide Yutaro
///             @date   2022_04_30
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Effect/Include/Blur.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIRenderPass.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUTexture.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"
#include "GameCore/Rendering/Model/Include/PrimitiveMesh.hpp"
#include "GameUtility/Math/Include/GMVertex.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc;
using namespace rhi;
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{
	float GaussianDistribution(float r, float sigma)
	{
		return expf(-0.5f * (float)(r * r) / (sigma * sigma));
	}

}

GaussianBlur::GaussianBlur()
{

}
GaussianBlur::~GaussianBlur()
{
	_vertexBuffers.clear(); _vertexBuffers.shrink_to_fit();
	_indexBuffers.clear(); _indexBuffers.shrink_to_fit();
	_computePipeline.reset();
	_xBlur.Pipeline.reset();
	_yBlur.Pipeline.reset();
	for (auto& u : _unorderedResourceViews) { u.reset(); }
	for (auto& s : _shaderResourceViews   ) { s.reset(); }
	_textureSizeView.reset();
	_blurParameterView.reset();
}
GaussianBlur::GaussianBlur(const LowLevelGraphicsEnginePtr& engine, const std::uint32_t width, const std::uint32_t height, const bool useCS, const std::wstring& addName)
	: _engine(engine), _useCS(useCS)
{
	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"GaussianBlur::";
	_addName = name;
	/*-------------------------------------------------------------------
	-            Prepare resource
	---------------------------------------------------------------------*/
	PrepareBlurParameters(name);
	PrepareVertexAndIndexBuffer(name);
	PrepareTextureSizeBuffer(width, height, name);
	PreparePipelineState(name);
	PrepareResourceView();
	SetUpWeightTable(DEFAULT_BLUR_SIGMA);
}
#pragma region Public Function

/****************************************************************************
*							OnResize
*************************************************************************//**
*  @fn        void GaussianBlur::OnResize(int newWidth, int newHeight)
*  @brief     OnResize
*  @param[in] std::uint32_t newWidth
*  @param[in] std::uint32_t newHeight
*  @return 　　void
*****************************************************************************/
void GaussianBlur::OnResize(const std::uint32_t newWidth, const std::uint32_t newHeight)
{
	PrepareTextureSizeBuffer(newWidth, newHeight, _addName);
}
/****************************************************************************
*							Draw
*************************************************************************//**
*  @fn        void GaussianBlur::Draw(GPUResource* renderTarget)
*  @brief     Draw Gaussian blur
*  @param[in,out] GPUResource* renderTarget
*  @return 　　void
*****************************************************************************/
void GaussianBlur::DrawCS(const ResourceViewPtr& sourceSRV, const ResourceViewPtr& destUAV)
{
	const auto device      = _engine->GetDevice();
	const auto commandList = _engine->GetCommandList(CommandListType::Compute);
	const auto graphicsCommandList = _engine->GetCommandList(CommandListType::Graphics);

#ifdef _DEBUG
	assert(_useCS);
#endif

	/*-------------------------------------------------------------------
	-               Pause current render pass
	---------------------------------------------------------------------*/
	auto waitValue = _engine->FlushGPUCommands(CommandListType::Graphics, true);
	_engine->WaitExecutionGPUCommands(CommandListType::Compute, waitValue, false);

	/*-------------------------------------------------------------------
	-               Execute commandlist
	---------------------------------------------------------------------*/
	commandList->SetDescriptorHeap(_blurParameterView->GetHeap());
	commandList->SetComputeResourceLayout(_resourceLayout);
	
	/*-------------------------------------------------------------------
	-               Bind
	---------------------------------------------------------------------*/
	_blurParameterView->Bind(commandList, 0);
	_textureSizeView  ->Bind(commandList, 1);
	sourceSRV->Bind(commandList, 2);
	destUAV  ->Bind(commandList,  3);
	_unorderedResourceViews[0]->Bind(commandList, 4);
	_unorderedResourceViews[1]->Bind(commandList, 5);

	commandList->SetComputePipeline(_computePipeline);
	commandList->Dispatch((_textureSize.OriginalTexture[0] + THREAD - 1) / THREAD, (_textureSize.OriginalTexture[1] + THREAD - 1) / THREAD, 1);

	/*-------------------------------------------------------------------
	-               Restart current render pass
	---------------------------------------------------------------------*/
	waitValue = _engine->FlushGPUCommands(CommandListType::Compute, true);
	_engine->WaitExecutionGPUCommands(CommandListType::Graphics, waitValue, false);
}

void GaussianBlur::DrawPS(const FrameBufferPtr& frameBuffer, const std::uint32_t renderTargetIndex)
{
	assert(!_useCS);

	const auto device       = _engine->GetDevice();
	const auto commandList  = _engine->GetCommandList(CommandListType::Graphics);
	const auto currentFrame = _engine->GetCurrentFrameIndex();
	const auto inputImage   = frameBuffer->GetRenderTargetSRV(renderTargetIndex);

	/*-------------------------------------------------------------------
	-               Change render resource
	---------------------------------------------------------------------*/
	commandList->EndRenderPass();

	/*-------------------------------------------------------------------
	-               Set graphics pipeline
	---------------------------------------------------------------------*/
	commandList->SetDescriptorHeap(_blurParameterView->GetHeap());
	commandList->SetGraphicsPipeline(_xBlur.Pipeline);
	commandList->SetResourceLayout(_resourceLayout);
		
	/*-------------------------------------------------------------------
	-               Bind gpu resources
	---------------------------------------------------------------------*/
	_blurParameterView->Bind(commandList, 0);
	_textureSizeView  ->Bind(commandList, 1);

	/*-------------------------------------------------------------------
	-               XBlur
	---------------------------------------------------------------------*/
	commandList->BeginRenderPass(_xBlur.RenderPass, _xBlur.FrameBuffer);
	inputImage->Bind(commandList, 2);
	commandList->SetVertexBuffer(_xBlur.VB[currentFrame]);
	commandList->SetIndexBuffer(_xBlur.IB[currentFrame]);
	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
	commandList->EndRenderPass();
		

	/*-------------------------------------------------------------------
	-               YBlur
	---------------------------------------------------------------------*/
	commandList->BeginRenderPass(_yBlur.RenderPass, _yBlur.FrameBuffer);
	commandList->SetGraphicsPipeline(_yBlur.Pipeline);
	_shaderResourceViews[0]->Bind(commandList, 2);
	commandList->SetVertexBuffer(_yBlur.VB[currentFrame]);
	commandList->SetIndexBuffer(_yBlur.IB[currentFrame]);
	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
	commandList->EndRenderPass();

	/*-------------------------------------------------------------------
	-               Default Path
	---------------------------------------------------------------------*/
	commandList->BeginRenderPass(_engine->GetDrawContinueRenderPass(), frameBuffer);
	commandList->SetGraphicsPipeline(_graphicsPipeline);
	_shaderResourceViews[1]->Bind(commandList, 2);
	commandList->SetVertexBuffer(_vertexBuffers[currentFrame]);
	commandList->SetIndexBuffer(_indexBuffers[currentFrame]);
	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
	commandList->EndRenderPass();
}

void GaussianBlur::Draw(const FrameBufferPtr& frameBuffer, const std::uint32_t renderTargetIndex)
{
	// compute shader draw function
	if (_useCS)
	{
		const auto inputImage = frameBuffer->GetRenderTargetSRV(renderTargetIndex);
		const auto outputImage = frameBuffer->GetRenderTargetUAV(renderTargetIndex);
		DrawCS(inputImage, outputImage);
		return;
	}
	// the following pixel shader 
	else
	{
		DrawPS(frameBuffer, renderTargetIndex);
	}
}

/****************************************************************************
*							SetUpWeightTable
*************************************************************************//**
*  @fn        void GaussianBlur::SetUpWeightTable(float sigma)
*  @brief     WeightTable
*  @param[in] float sigma
*  @return 　　void
*****************************************************************************/
void GaussianBlur::SetUpWeightTable(float sigma)
{
	float weights[WEIGHT_TABLE_SIZE]={0.0f};
	float total = 0.0f;
	for (std::uint32_t i = 0; i < WEIGHT_TABLE_SIZE; ++i)
	{
		weights[i] = ::GaussianDistribution((float)i, sigma);
		if (i == 0) { total +=        weights[i]; }
		else        { total += 2.0f * weights[i]; }
	}

	// normalize
	for (std::uint32_t i = 0; i < WEIGHT_TABLE_SIZE; ++i)
	{
		weights[i] /= total;
	}

	// Copy weights table
	BlurParameter parameter;
	parameter.Weights[0] = gm::Float4(weights[0], weights[1], weights[2], weights[3]);
	parameter.Weights[1] = gm::Float4(weights[4], weights[5], weights[6], weights[7]);
	
	_blurParameterView->GetBuffer()->Update(&parameter, 1);
}
#pragma endregion Public Function
#pragma region Protected Function
/****************************************************************************
*							PrepareBlurParameters
*************************************************************************//**
*  @fn        void GaussianBlur::PrepareBlurParameters()
*  @brief     Prepare Blur Parameter
*  @param[in] const std::wstring& name
*  @return 　　void
*****************************************************************************/
void GaussianBlur::PrepareBlurParameters(const std::wstring& name)
{
	const auto device = _engine->GetDevice();

	BlurParameter parameter;
	parameter.Weights[0] = gm::Float4(0, 0, 0, 0);
	parameter.Weights[1] = gm::Float4(0, 0, 0, 0);

	const auto metaData = GPUBufferMetaData::ConstantBuffer(sizeof(BlurParameter), 1, MemoryHeap::Upload, ResourceState::Common);
	
	const auto blurParameter = _engine->GetDevice()->CreateBuffer(metaData);
	// Blur parameter gpu resource name 
	blurParameter->SetName(name + L"BlurParameter");
	// Pack blur paramter init data 
	blurParameter->Pack(&parameter, nullptr);

	_blurParameterView = device->CreateResourceView(ResourceViewType::ConstantBuffer, blurParameter, nullptr);
}
/****************************************************************************
*							PrepareTextureSizeBuffer
*************************************************************************//**
*  @fn        void GaussianBlur::PrepareTextureSizeBuffer(int width, int height)
*  @brief     Prepare Texture Size Buffer
*  @param[in] std::uint32_t width
*  @param[in] std::uint32_t height
*  @return 　　void
*****************************************************************************/
void GaussianBlur::PrepareTextureSizeBuffer(const std::uint32_t width, const std::uint32_t height, const std::wstring& name)
{
	const auto device   = _engine->GetDevice();
	const auto metaData = GPUBufferMetaData::ConstantBuffer(sizeof(TextureSizeParameter), 1, MemoryHeap::Upload, ResourceState::Common);
	
	const auto textureSizeBuffer = device->CreateBuffer(metaData);
	textureSizeBuffer->SetName(name + L"TextureSize");
	/*-------------------------------------------------------------------
	-			Set Texture Size Parameter
	---------------------------------------------------------------------*/
	_textureSize.OriginalTexture[0] = width;
	_textureSize.OriginalTexture[1] = height;
	_textureSize.XBlurTexture[0]    = width / 2;
	_textureSize.XBlurTexture[1]    = height;
	_textureSize.YBlurTexture[0]    = width / 2;
	_textureSize.YBlurTexture[1]    = height / 2;

	textureSizeBuffer->Pack(&_textureSize, nullptr);
	_textureSizeView = device->CreateResourceView(ResourceViewType::ConstantBuffer, textureSizeBuffer, nullptr);
}

/****************************************************************************
*							PreparePipelineState
*************************************************************************//**
*  @fn        void GaussianBlur::PreparePipelineState()
* 
*  @brief     Prepare xblur, yblur, and finalblur PSO
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void GaussianBlur::PreparePipelineState(const std::wstring& name)
{
	std::wstring defaultPath = L"Shader\\Effect\\ShaderGaussianBlur.hlsl";
	const auto device = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();
	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	_resourceLayout = device->CreateResourceLayout
	(
		{
			ResourceLayoutElement(DescriptorHeapType::CBV, 1), // weight table 
			ResourceLayoutElement(DescriptorHeapType::CBV, 2), // texture size
			ResourceLayoutElement(DescriptorHeapType::SRV, 0), // source texture
			ResourceLayoutElement(DescriptorHeapType::UAV, 0), // dest texture
			ResourceLayoutElement(DescriptorHeapType::UAV, 1), // xblur texture
			ResourceLayoutElement(DescriptorHeapType::UAV, 2), // xy blur texture
		},
		{ SamplerLayoutElement(device->CreateSampler(SamplerInfo::GetDefaultSampler(SamplerLinearWrap)), 0) }
	);

	/*-------------------------------------------------------------------
	-			Load Blob data
	---------------------------------------------------------------------*/
	if (_useCS)
	{
		const auto blurCS = factory->CreateShaderState();
		blurCS->Compile(ShaderType::Compute, defaultPath, L"ExecuteBlur", 6.4f, { L"Shader\\Core" });

		_computePipeline = device->CreateComputePipelineState(_resourceLayout);
		_computePipeline->SetComputeShader(blurCS);
		_computePipeline->CompleteSetting();
		_computePipeline->SetName(name + L"BlurPSO");
	}
	else
	{
		// render pass
		const auto colorAttachment = Attachment::RenderTarget(_engine->GetBackBufferFormat(), ResourceState::RenderTarget, ResourceState::Present, AttachmentLoad::Load);
		_xBlur.RenderPass = device->CreateRenderPass(colorAttachment, std::nullopt);
		_yBlur.RenderPass = device->CreateRenderPass(colorAttachment, std::nullopt);

		// load blob data
		const auto blurVS_X = factory->CreateShaderState();
		const auto blurVS_Y = factory->CreateShaderState();
		const auto blurPS   = factory->CreateShaderState();
		const auto mainPS   = factory->CreateShaderState();
		const auto mainVS   = factory->CreateShaderState();
		blurVS_X->Compile(ShaderType::Vertex, defaultPath, L"VS_XBlur", 6.4f, { L"Shader\\Core" });
		blurVS_Y->Compile(ShaderType::Vertex, defaultPath, L"VS_YBlur", 6.4f, { L"Shader\\Core" });
		blurPS  ->Compile(ShaderType::Pixel, defaultPath,  L"PSBlur"  , 6.4f, { L"Shader\\Core" });
		mainVS  ->Compile(ShaderType::Vertex, defaultPath, L"VSFinal", 6.4f, { L"Shader\\Core" });
		mainPS  ->Compile(ShaderType::Pixel, defaultPath, L"PSFinal", 6.4f, {L"Shader\\Core"});

		// XBlur
		_xBlur.Pipeline = device->CreateGraphicPipelineState(_xBlur.RenderPass, _resourceLayout);
		_xBlur.Pipeline->SetVertexShader(blurVS_X);
		_xBlur.Pipeline->SetPixelShader(blurPS);
		_xBlur.Pipeline->SetBlendState(factory->CreateSingleBlendState(BlendProperty::OverWrite()));
		_xBlur.Pipeline->SetRasterizerState(factory->CreateRasterizerState(RasterizerProperty::Solid()));
		_xBlur.Pipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultVertexElement()));
		_xBlur.Pipeline->CompleteSetting();
		_xBlur.Pipeline->SetName(name + L"XBlurPipeline");

		// YBlur
		_yBlur.Pipeline = device->CreateGraphicPipelineState(_yBlur.RenderPass, _resourceLayout);
		_yBlur.Pipeline->SetBlendState(factory->CreateSingleBlendState(BlendProperty::OverWrite()));
		_yBlur.Pipeline->SetRasterizerState(factory->CreateRasterizerState(RasterizerProperty::Solid()));
		_yBlur.Pipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultVertexElement()));
		_yBlur.Pipeline->SetVertexShader(blurVS_Y);
		_yBlur.Pipeline->SetPixelShader(blurPS);
		_yBlur.Pipeline->CompleteSetting();
		_yBlur.Pipeline->SetName(name + L"YBlurPipeline");

		// Final Blur
		_graphicsPipeline = device->CreateGraphicPipelineState(_engine->GetDrawContinueRenderPass(), _resourceLayout);
		_graphicsPipeline->SetVertexShader(mainVS);
		_graphicsPipeline->SetPixelShader(mainPS);
		_graphicsPipeline->SetBlendState        (factory->CreateSingleBlendState(BlendProperty::OverWrite()));
		_graphicsPipeline->SetRasterizerState   (factory->CreateRasterizerState(RasterizerProperty::Solid()));
		_graphicsPipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultVertexElement()));
		_graphicsPipeline->CompleteSetting();
		_graphicsPipeline->SetName(name + L"MainPS");
	}
	
}

/****************************************************************************
*							PrepareResourceView
*************************************************************************//**
*  @fn        void GaussianBlur::PrepareResourceView()
* 
*  @brief     Prepare xblur, yblur, and finalblur resource views
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void GaussianBlur::PrepareResourceView()
{
	const auto device = _engine->GetDevice();
	const auto format = _engine->GetBackBufferFormat();

	//  xblur
	{
		const auto dstData     = GPUTextureMetaData::Texture2D(Screen::GetScreenWidth() / 2, Screen::GetScreenHeight(), format, 1, ResourceUsage::UnorderedAccess | ResourceUsage::RenderTarget);
		const auto destTexture = device->CreateTexture(dstData);
		
		_unorderedResourceViews[0] = device->CreateResourceView(ResourceViewType::RWTexture, destTexture, nullptr); // x half texture uav
		_shaderResourceViews   [0] = device->CreateResourceView(ResourceViewType::Texture  , destTexture, nullptr); // x half texture srv
		if (!_useCS)
		{
			_renderTargetResourceViews[0] = device->CreateResourceView(ResourceViewType::RenderTarget, destTexture, nullptr);
			_xBlur.FrameBuffer            = device->CreateFrameBuffer(_xBlur.RenderPass, destTexture, nullptr);
		}
	}
	// yblur
	{
		const auto dstData         = GPUTextureMetaData::Texture2D(Screen::GetScreenWidth() / 2, Screen::GetScreenHeight() / 2, format, 1, ResourceUsage::UnorderedAccess | ResourceUsage::RenderTarget);
		const auto destTexture     = device->CreateTexture(dstData);
		_unorderedResourceViews[1] = device->CreateResourceView(ResourceViewType::RWTexture, destTexture, nullptr);
		_shaderResourceViews[1]    = device->CreateResourceView(ResourceViewType::Texture  , destTexture, nullptr);
		if (!_useCS)
		{
			_renderTargetResourceViews[1] = device->CreateResourceView(ResourceViewType::RenderTarget, destTexture);
			_yBlur.FrameBuffer            = device->CreateFrameBuffer(_yBlur.RenderPass, destTexture, nullptr);
		}
	}
	// finalblur
	{
		const auto dstData     = GPUTextureMetaData::Texture2D(Screen::GetScreenWidth()    , Screen::GetScreenHeight()    , format, 1, ResourceUsage::UnorderedAccess | ResourceUsage::RenderTarget);
		const auto destTexture = device->CreateTexture(dstData);
		_unorderedResourceViews[2] = device->CreateResourceView(ResourceViewType::RWTexture, destTexture, nullptr);

	}
}

void GaussianBlur::PrepareVertexAndIndexBuffer(const std::wstring& addName)
{
	if (_useCS) { return; }

	const auto device     = _engine->GetDevice();
	const auto commandList = _engine->GetCommandList(CommandListType::Copy);
	/*-------------------------------------------------------------------
	-            Create Sphere Mesh
	---------------------------------------------------------------------*/
	gc::core::PrimitiveMesh rectMesh = gc::core::PrimitiveMeshGenerator::Rect(2.0f, 2.0f, 0.0f);
	/*-------------------------------------------------------------------
	-            Create Mesh Buffer
	---------------------------------------------------------------------*/
	const auto frameCount = LowLevelGraphicsEngine::FRAME_BUFFER_COUNT;
	// prepare frame count buffer
	_vertexBuffers.resize(frameCount);
	_indexBuffers .resize(frameCount);
	_xBlur.VB.resize(frameCount);
	_xBlur.IB.resize(frameCount);
	_yBlur.VB.resize(frameCount);
	_yBlur.IB.resize(frameCount);
	for (std::uint32_t i = 0; i < frameCount; ++i)
	{
		/*-------------------------------------------------------------------
		-            Set up
		---------------------------------------------------------------------*/
		auto vertexByteSize = sizeof(gm::Vertex);
		auto indexByteSize  = sizeof(std::uint32_t);
		auto vertexCount    = rectMesh.Vertices.size();
		auto indexCount     = rectMesh.Indices.size();

		/*-------------------------------------------------------------------
		-            Set Vertex Buffer 
		---------------------------------------------------------------------*/
		const auto vbMetaData = GPUBufferMetaData::VertexBuffer(vertexByteSize, vertexCount, MemoryHeap::Upload);
		_vertexBuffers[i] = device->CreateBuffer(vbMetaData);
		_vertexBuffers[i]->SetName(addName + L"FinalVB");
		_vertexBuffers[i]->Pack(rectMesh.Vertices.data()); // Map
		_xBlur.VB[i] = device->CreateBuffer(vbMetaData);
		_yBlur.VB[i] = device->CreateBuffer(vbMetaData);
		_xBlur.VB[i]->Pack(rectMesh.Vertices.data()); // Map
		_yBlur.VB[i]->Pack(rectMesh.Vertices.data()); // Map

		/*-------------------------------------------------------------------
		-            Set Index Buffer
		---------------------------------------------------------------------*/
		const auto ibMetaData = GPUBufferMetaData::IndexBuffer(indexByteSize, indexCount, MemoryHeap::Default, ResourceState::Common);
		_indexBuffers[i] = device->CreateBuffer(ibMetaData);
		_indexBuffers[i]->SetName(addName + L"FinalIB");
		_indexBuffers[i]->Pack(rectMesh.Indices.data(), commandList);
		_xBlur.IB[i] = device->CreateBuffer(vbMetaData);
		_yBlur.IB[i] = device->CreateBuffer(vbMetaData);
		_xBlur.IB[i]->Pack(rectMesh.Indices.data(), commandList);
		_yBlur.IB[i]->Pack(rectMesh.Indices.data(), commandList);
		_xBlur.VB[i]->SetName(addName + L"XVB");
		_yBlur.VB[i]->SetName(addName + L"YVB");
		_xBlur.IB[i]->SetName(addName + L"XIB");
		_yBlur.IB[i]->SetName(addName + L"YVB");
	}
}
#pragma endregion Protected Function