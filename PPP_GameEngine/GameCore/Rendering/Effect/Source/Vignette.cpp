//////////////////////////////////////////////////////////////////////////////////
///             @file   Mosaic.hpp
///             @brief  Mosaic post effect
///             @author Toide Yutaro
///             @date   2023_02_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/Vignette.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUTexture.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"
#include "GameUtility/Base/Include/Screen.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gm;
using namespace gc;
using namespace rhi;
using namespace rhi::core;


//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
Vignette::Vignette()
{

}

Vignette::~Vignette()
{

}

Vignette::Vignette(const LowLevelGraphicsEnginePtr& engine, const VignetteSettings& settings, const gu::wstring& addName)
	: IFullScreenEffector(engine), _settings(settings)
{
	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	const auto name = DefineDebugName(addName);

	/*-------------------------------------------------------------------
	-           Prepare Pipeline
	---------------------------------------------------------------------*/
	PrepareVertexAndIndexBuffer(name);
	PrepareBuffer(name);
	PreparePipelineState(name);
	PrepareResourceView();
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
void Vignette::OnResize(int newWidth, int newHeight)
{
	printf("width: %d, height: %d\n", newWidth, newHeight);
}

void Vignette::Draw()
{
	if (_isSettingChanged)
	{
		_resourceViews[0]->GetBuffer()->Update(&_settings, 1);
		_isSettingChanged = false;
	}

	const auto frameIndex = _engine->GetCurrentFrameIndex();
	const auto device = _engine->GetDevice();
	const auto graphicsCommandList = _engine->GetCommandList(CommandListType::Graphics);

	/*-------------------------------------------------------------------
	-               Execute commandlist
	---------------------------------------------------------------------*/
	graphicsCommandList->SetResourceLayout(_resourceLayout);
	graphicsCommandList->SetGraphicsPipeline(_pipeline);
	graphicsCommandList->SetVertexBuffer(_vertexBuffers[frameIndex]);
	graphicsCommandList->SetIndexBuffer(_indexBuffers[frameIndex]);
	_resourceViews[0]->Bind(graphicsCommandList, 0);
	_engine->GetFrameBuffer(frameIndex)->GetRenderTargetSRV()->Bind(graphicsCommandList, 1);
	graphicsCommandList->DrawIndexedInstanced(
		static_cast<std::uint32_t>(_indexBuffers[frameIndex]->GetElementCount()), 1);
}
#pragma endregion Main Function

#pragma region Set up function

void Vignette::PrepareBuffer(const gu::wstring& name)
{
	const auto device = _engine->GetDevice();
	const auto metaData = GPUBufferMetaData::ConstantBuffer(sizeof(VignetteSettings), 1);

	const auto buffer = device->CreateBuffer(metaData);
	const auto bufferName = name + L"VignetteSettings";
	buffer->SetName(bufferName);

	/*-------------------------------------------------------------------
	-			Set Information
	---------------------------------------------------------------------*/
	buffer->Pack(&_settings, nullptr);
	_resourceViews.push_back(device->CreateResourceView(ResourceViewType::ConstantBuffer, buffer));
}


void Vignette::PreparePipelineState(const gu::wstring& addName)
{
	const auto device = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();

	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	_resourceLayout = device->CreateResourceLayout
	(
		{
			ResourceLayoutElement(DescriptorHeapType::CBV, 0), // Vignette setting
			ResourceLayoutElement(DescriptorHeapType::SRV, 0), // source texture
		},
		{ SamplerLayoutElement(device->CreateSampler(SamplerInfo::GetDefaultSampler(SamplerLinearClamp)), 0) }
	);

	/*-------------------------------------------------------------------
	-			SetShader
	---------------------------------------------------------------------*/
	const auto vs = factory->CreateShaderState();
	const auto ps = factory->CreateShaderState();
	vs->Compile(ShaderType::Vertex, L"Shader\\Effect\\ShaderVignette.hlsl", L"VSMain", 6.4f, { L"Shader\\Core" });
	ps->Compile(ShaderType::Pixel, L"Shader\\Effect\\ShaderVignette.hlsl", L"PSMain", 6.4f, { L"Shader\\Core" });

	/*-------------------------------------------------------------------
	-			Build Graphics Pipeline State
	---------------------------------------------------------------------*/
	_pipeline = device->CreateGraphicPipelineState(_engine->GetRenderPass(), _resourceLayout);
	_pipeline->SetBlendState(factory->CreateSingleBlendState(BlendProperty::OverWrite()));
	_pipeline->SetRasterizerState(factory->CreateRasterizerState(RasterizerProperty::Solid()));
	_pipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultVertexElement()));
	_pipeline->SetDepthStencilState(factory->CreateDepthStencilState());
	_pipeline->SetVertexShader(vs);
	_pipeline->SetPixelShader(ps);
	_pipeline->CompleteSetting();
	_pipeline->SetName(addName + L"PSO");
}

void Vignette::PrepareResourceView()
{

}
#pragma endregion Set up function