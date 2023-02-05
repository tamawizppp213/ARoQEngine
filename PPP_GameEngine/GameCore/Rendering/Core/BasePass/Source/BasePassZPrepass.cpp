//////////////////////////////////////////////////////////////////////////////////
//              @file   BasePassGBuffer.cpp
///             @brief  GBuffers
///             @author Toide Yutaro
///             @date   2022_05_04
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/BasePassZPrepass.hpp"
#include "../../../Model/Include/GameModel.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIResourceLayout.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;
using namespace gc;
using namespace gc::core;
using namespace gc::basepass;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 
ZPrepass::ZPrepass(const LowLevelGraphicsEnginePtr& engine, const std::uint32_t width, const std::uint32_t height, const std::wstring& addName):
	_engine(engine), _width(width), _height(height)
{
	/*-------------------------------------------------------------------
	-            Set name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"ZPrepass::";

	PrepareFrameBuffers();
	PreparePipelineState(name);
}

ZPrepass::~ZPrepass()
{
	_gameModels.clear(); _gameModels.shrink_to_fit();
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
void ZPrepass::OnResize(const std::uint32_t width, const std::uint32_t height)
{

}

void ZPrepass::Draw(const GPUResourceViewPtr& scene)
{
	const auto currentFrame = _engine->GetCurrentFrameIndex();
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics, currentFrame);

	/*-------------------------------------------------------------------
	-                 Change render target
	---------------------------------------------------------------------*/
	commandList->EndRenderPass();
	commandList->BeginRenderPass(_renderPass, _frameBuffers[currentFrame]);

	/*-------------------------------------------------------------------
	-                 Execute command list
	---------------------------------------------------------------------*/
	commandList->SetDescriptorHeap(scene->GetHeap());
	commandList->SetResourceLayout(_resourceLayout);
	commandList->SetGraphicsPipeline(_pipeline);
	scene->Bind(commandList, 0); // scene constants
	for (const auto gameModel : _gameModels)
	{
		gameModel->Draw(false);
	}

	/*-------------------------------------------------------------------
	-                 Return current render target
	---------------------------------------------------------------------*/
	commandList->EndRenderPass();
	commandList->BeginRenderPass(_engine->GetRenderPass(), _engine->GetFrameBuffer(currentFrame));
}

void ZPrepass::Add(const GameModelPtr& actor)
{
	_gameModels.emplace_back(actor);
}

void ZPrepass::Clear()
{
	_gameModels.clear();
	_gameModels.shrink_to_fit();
}

void ZPrepass::Clear(const GameModelPtr& actor)
{
	std::erase(_gameModels, actor);
}

ZPrepass::TexturePtr ZPrepass::GetRenderedTexture() const noexcept
{
	return _frameBuffers[_engine->GetCurrentFrameIndex()]->GetRenderTarget();
}
#pragma endregion Main Function

#pragma region Setup Function
void ZPrepass::PreparePipelineState(const std::wstring& name)
{
	const auto device  = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();

	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	_resourceLayout = device->CreateResourceLayout
	(
		{
			ResourceLayoutElement(DescriptorHeapType::CBV, 0), // Scene constant 
			ResourceLayoutElement(DescriptorHeapType::CBV, 1), // object world position information
		}
	);

	/*-------------------------------------------------------------------
	-             Set up graphic pipeline state
	---------------------------------------------------------------------*/
	const auto vs = factory->CreateShaderState();
	const auto ps = factory->CreateShaderState();
	vs->Compile(ShaderType::Vertex, L"Shader\\Lighting\\ShaderZPrepass.hlsl", L"VSMain", 6.4f, { L"Shader\\Core"});
	ps->Compile(ShaderType::Pixel , L"Shader\\Lighting\\ShaderZPrepass.hlsl", L"PSMain", 6.4f, { L"Shader\\Core" });

	/*-------------------------------------------------------------------
	-             Set up graphic pipeline state
	---------------------------------------------------------------------*/
	_pipeline = device->CreateGraphicPipelineState(_renderPass, _resourceLayout);
	_pipeline->SetBlendState(factory->CreateSingleBlendState(BlendProperty::AlphaBlend()));
	_pipeline->SetRasterizerState   (factory->CreateRasterizerState());
	_pipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultSkinVertexElement()));
	_pipeline->SetDepthStencilState(factory->CreateDepthStencilState());
	_pipeline->SetVertexShader(vs);
	_pipeline->SetPixelShader(ps);
	_pipeline->CompleteSetting();
	_pipeline->SetName(name + L"PSO");
}

void ZPrepass::PrepareFrameBuffers()
{
	const auto frameCount = LowLevelGraphicsEngine::FRAME_BUFFER_COUNT;
	const auto device     = _engine->GetDevice();
	const auto clearColor      = ClearValue(1.0f, 0.0f, 0.0f, 1.0f);
	const auto clearDepthColor = ClearValue(0.0f, 0.0f, 0.0f, 1.0f);

	/*-------------------------------------------------------------------
	-             Setup render pass
	---------------------------------------------------------------------*/
	{
		Attachment colorAttachment = Attachment::RenderTarget(PixelFormat::R32_FLOAT);
		Attachment depthAttachment = Attachment::DepthStencil(PixelFormat::D32_FLOAT);

		_renderPass = device->CreateRenderPass(colorAttachment, depthAttachment);
		_renderPass->SetClearValue(clearColor, clearDepthColor);
	}

	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	_frameBuffers.resize(frameCount);
	for (std::uint32_t i = 0; i < frameCount; ++i)
	{
		const auto renderInfo    = GPUTextureMetaData::RenderTarget(_width, _height, PixelFormat::R32_FLOAT, clearColor);
		const auto depthInfo     = GPUTextureMetaData::DepthStencil(_width, _height, PixelFormat::D32_FLOAT, clearDepthColor);
		const auto renderTexture = device->CreateTexture(renderInfo);
		const auto depthTexture  = device->CreateTexture(depthInfo);
		
		_frameBuffers[i] = device->CreateFrameBuffer(_renderPass, renderTexture, depthTexture);
	}
}
#pragma endregion Setup Function
