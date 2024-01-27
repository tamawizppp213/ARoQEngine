//////////////////////////////////////////////////////////////////////////////////
//              @file   BasePassGBuffer.cpp
///             @brief  GBuffers
///             @author Toide Yutaro
///             @date   2022_05_04
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/BasePassGBuffer.hpp"
#include "../../../Model/Include/GameModel.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIRenderPass.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIResourceLayout.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::basepass;
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
GBuffer::GBuffer(const LowLevelGraphicsEnginePtr& engine, const gc::rendering::GBufferDesc& desc, const gu::wstring& addName)
	: gc::rendering::GBuffer(engine, desc, addName)
{
	/*-------------------------------------------------------------------
	-            Set name
	---------------------------------------------------------------------*/
	gu::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"GBuffer::";

	PrepareFrameBuffers(name);
	PreparePipelineState(name);
}

GBuffer::~GBuffer()
{

}
#pragma endregion Constructor and Destructor

#pragma region Main Function
void GBuffer::OnResize(const std::uint32_t width, const std::uint32_t height)
{
	printf("width: %d, height: %d\n", width, height);
}

void GBuffer::Draw(const GPUResourceViewPtr& scene)
{
	const auto currentFrame = _engine->GetCurrentFrameIndex();
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics);

	/*-------------------------------------------------------------------
	-                 Change render target
	---------------------------------------------------------------------*/
	commandList->BeginRenderPass(_renderPass, _frameBuffers[currentFrame]);

	/*-------------------------------------------------------------------
	-                 Execute command list
	---------------------------------------------------------------------*/
	commandList->SetDescriptorHeap(scene->GetHeap());
	commandList->SetResourceLayout(_resourceLayout);
	commandList->SetGraphicsPipeline(_pipeline);
	scene->Bind(commandList, 0);
	for (const auto& gameModel : _gameModels)
	{
		gameModel->Draw(true);
	}

	commandList->EndRenderPass();
}


#pragma endregion Main Function

#pragma region Set up Function
/****************************************************************************
*                          PreparePipelineState
*************************************************************************//**
*  @fn        void GBuffer::PreparePipelineState(const gu::wstring& name)
*
*  @brief     Prepare pipeline state
*
*  @param[in] gu::wstring& name
*
*  @return 　　void
*****************************************************************************/
void GBuffer::PreparePipelineState(const gu::wstring& name)
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
			ResourceLayoutElement(DescriptorHeapType::CBV, 2), // material constants
			ResourceLayoutElement(DescriptorHeapType::SRV, 0), // Diffuse map
			ResourceLayoutElement(DescriptorHeapType::SRV, 1), // Specular map
			ResourceLayoutElement(DescriptorHeapType::SRV, 2), // Normal map
		},
		{ SamplerLayoutElement(device->CreateSampler(SamplerInfo::GetDefaultSampler(SamplerLinearWrap)),0) }
	);

	/*-------------------------------------------------------------------
	-             Setup shader
	---------------------------------------------------------------------*/
	const auto vs = factory->CreateShaderState();
	const auto ps = factory->CreateShaderState();
	vs->Compile(ShaderType::Vertex, L"Shader\\Lighting\\ShaderGBuffer.hlsl", L"VSMain", 6.4f, { L"Shader\\Core" });
	ps->Compile(ShaderType::Pixel , L"Shader\\Lighting\\ShaderGBuffer.hlsl", L"PSMain", 6.4f, { L"Shader\\Core" });

	/*-------------------------------------------------------------------
	-             Setup blend state (all alpha blend)
	---------------------------------------------------------------------*/
	std::vector<BlendProperty> blends(_desc.BufferCount, BlendProperty::AlphaBlend());

	/*-------------------------------------------------------------------
	-             Set up graphic pipeline state
	---------------------------------------------------------------------*/
	_pipeline = device->CreateGraphicPipelineState(_renderPass, _resourceLayout);
	_pipeline->SetBlendState        (factory->CreateBlendState(blends));
	_pipeline->SetRasterizerState   (factory->CreateRasterizerState(RasterizerProperty::Solid()));
	_pipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultSkinVertexElement()));
	_pipeline->SetDepthStencilState (factory->CreateDepthStencilState());
	_pipeline->SetVertexShader(vs);
	_pipeline->SetPixelShader(ps);
	_pipeline->CompleteSetting();
	_pipeline->SetName(name + L"PSO");
}

/****************************************************************************
*                          PrepareFrameBuffers
*************************************************************************//**
*  @fn        void ZPrepass::PrepareFrameBuffers()
*
*  @brief     Prepare render resources. (renderPass, frameCount's frame buffers)
*
*  @param[in] const gu::wstring& name
*
*  @return 　　void
*****************************************************************************/
void GBuffer::PrepareFrameBuffers(const gu::wstring& name)
{
	const auto frameCount      = LowLevelGraphicsEngine::FRAME_BUFFER_COUNT;
	const auto device          = _engine->GetDevice();
	const auto clearColor      = ClearValue(0.0f, 0.0f, 0.0f, 1.0f);
	const auto depthClearColor = ClearValue(0.0f, 0.0f, 0.0f, 1.0f);

	/*-------------------------------------------------------------------
	-             Setup render pass
	---------------------------------------------------------------------*/
	{
		std::vector<Attachment> colorAttachment(_desc.BufferCount, Attachment::RenderTarget(PixelFormat::R32G32B32A32_FLOAT));
		Attachment depthAttachment = Attachment::DepthStencil(PixelFormat::D32_FLOAT);

		_renderPass = device->CreateRenderPass(colorAttachment, depthAttachment);
		_renderPass->SetClearValue(std::vector<ClearValue>(_desc.BufferCount, clearColor), depthClearColor);
	}

	/*-------------------------------------------------------------------
	-             Setup frame buffer
	---------------------------------------------------------------------*/
	_frameBuffers.resize(frameCount);
	for (std::uint32_t i = 0; i < frameCount; ++i)
	{
		auto renderInfo = GPUTextureMetaData::RenderTarget(_desc.Width, _desc.Height, PixelFormat::R32G32B32A32_FLOAT, clearColor);
		auto depthInfo  = GPUTextureMetaData::DepthStencil(_desc.Width, _desc.Height, PixelFormat::D32_FLOAT, depthClearColor);
		renderInfo.ResourceUsage = (ResourceUsage::UnorderedAccess | ResourceUsage::RenderTarget);
		std::vector<TexturePtr> renderTexture(_desc.BufferCount);
		for (size_t j = 0; j < _desc.BufferCount; ++j)
		{
			renderTexture[j] = device->CreateTexture(renderInfo, name + L"RenderTarget");
		}
		const auto depthTexture = device->CreateTexture(depthInfo, name + L"DepthStencil");

		_frameBuffers[i] = device->CreateFrameBuffer(_renderPass, renderTexture, depthTexture);
	}
}
#pragma endregion Set up Function