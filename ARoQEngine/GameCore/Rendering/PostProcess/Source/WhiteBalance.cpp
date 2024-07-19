//////////////////////////////////////////////////////////////////////////////////
///             @file   Mosaic.hpp
///             @brief  Mosaic post effect
///             @author Toide Yutaro
///             @date   2023_02_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/WhiteBalance.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUTexture.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include <stdio.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gm;
using namespace engine;
using namespace rhi;
using namespace rhi::core;


//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
WhiteBalance::WhiteBalance()
{

}

WhiteBalance::~WhiteBalance()
{

}

WhiteBalance::WhiteBalance(const LowLevelGraphicsEnginePtr& engine, const float temperature, const float tint, const gu::tstring& addName)
	: IFullScreenEffector(engine)
{
	_settings.Temperature = temperature;
	_settings.Tint = tint;

	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	const auto name = CombineDebugName(addName, SP("WhiteBalance"));

	/*-------------------------------------------------------------------
	-           Prepare Pipeline
	---------------------------------------------------------------------*/
	PrepareVertexAndIndexBuffer(name);
	PrepareBuffer(_settings, name);
	PreparePipelineState(name);
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
void WhiteBalance::OnResize(const gu::uint32 newWidth, const gu::uint32 newHeight)
{
	printf("width: %d, height: %d\n", newWidth, newHeight);
}

void WhiteBalance::Draw()
{
	if (_isSettingChanged)
	{
		_resourceViews[0]->GetBuffer()->UploadByte(&_settings, sizeof(_settings));
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
	graphicsCommandList->SetIndexBuffer(_indexBuffer, core::PixelFormat::R16_UINT);
	_resourceViews[0]->Bind(graphicsCommandList, 0);
	_engine->GetFrameBuffer(frameIndex)->GetRenderTargetSRV()->Bind(graphicsCommandList, 1);
	graphicsCommandList->DrawIndexedInstanced(
		static_cast<gu::uint32>(_indexBuffer->GetElementCount()), 1);
}
#pragma endregion Main Function

#pragma region Set up function

void WhiteBalance::PrepareBuffer(const WhiteBalanceSettings& settings, const gu::tstring& name)
{
	const auto device = _engine->GetDevice();
	const auto metaData = GPUBufferMetaData::ConstantBuffer(sizeof(WhiteBalanceSettings), 1);

	const auto buffer = device->CreateBuffer(metaData);
	buffer->SetName(name + L"VignetteSettings");

	/*-------------------------------------------------------------------
	-			Set Information
	---------------------------------------------------------------------*/
	_settings = settings;
	buffer->UploadByte(&_settings, metaData.GetTotalByte(), 0, nullptr);
	_resourceViews.Push(device->CreateResourceView(ResourceViewType::ConstantBuffer, buffer));
}


void WhiteBalance::PreparePipelineState(const gu::tstring& addName)
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
		{ SamplerLayoutElement(device->CreateSampler(SamplerInfo::GetDefaultSampler(LinearClamp)), 0) }
	);

	/*-------------------------------------------------------------------
	-			SetShader
	---------------------------------------------------------------------*/
	const auto vs = factory->CreateShaderState();
	const auto ps = factory->CreateShaderState();
	vs->Compile({ ShaderType::Vertex, L"Shader\\PostProcess\\ShaderScreenPass.hlsl", L"MainVS",{L"Shader\\Core" } });
	ps->Compile({ ShaderType::Pixel, L"Shader\\PostProcess\\ShaderWhiteBalance.hlsl", L"PSMain", { L"Shader\\Core" } });

	/*-------------------------------------------------------------------
	-			Build Graphics Pipeline State
	---------------------------------------------------------------------*/
	_pipeline = device->CreateGraphicPipelineState(_engine->GetRenderPass(), _resourceLayout);
	_pipeline->SetBlendState(factory->CreateSingleBlendState(BlendProperty::OverWrite()));
	_pipeline->SetRasterizerState(factory->CreateRasterizerState(RasterizerProperty::Solid()));
	_pipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultScreenElement()));
	_pipeline->SetDepthStencilState(factory->CreateDepthStencilState());
	_pipeline->SetVertexShader(vs);
	_pipeline->SetPixelShader(ps);
	_pipeline->CompleteSetting();
	_pipeline->SetName(addName + L"PSO");
}

#pragma endregion Set up function