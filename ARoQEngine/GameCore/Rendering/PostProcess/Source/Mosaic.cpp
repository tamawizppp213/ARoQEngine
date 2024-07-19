//////////////////////////////////////////////////////////////////////////////////
///             @file   Mosaic.hpp
///             @brief  Mosaic post effect
///             @author Toide Yutaro
///             @date   2023_02_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/Mosaic.hpp"
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
Mosaic::Mosaic()
{

}

Mosaic::~Mosaic()
{

}

Mosaic::Mosaic(const LowLevelGraphicsEnginePtr& engine, const float blockSize, const gu::tstring& addName)
	: IFullScreenEffector(engine)
{
#ifdef _DEBUG
	Check(blockSize >= 0.0f);
#endif

	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	const auto name = CombineDebugName(addName, SP("Mosaic"));

	/*-------------------------------------------------------------------
	-           Prepare Pipeline
	---------------------------------------------------------------------*/
	PrepareVertexAndIndexBuffer(name);
	PrepareBuffer(blockSize, name);
	PreparePipelineState(name);
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
void Mosaic::OnResize(const gu::uint32 newWidth, const gu::uint32 newHeight)
{
	printf("width: %d, height: %d\n", newWidth, newHeight);
}

void Mosaic::Draw()
{
	const auto frameIndex          = _engine->GetCurrentFrameIndex();
	const auto device              = _engine->GetDevice();
	const auto graphicsCommandList = _engine->GetCommandList(CommandListType::Graphics);

	/*-------------------------------------------------------------------
	-               Execute commandlist
	---------------------------------------------------------------------*/
	graphicsCommandList->SetResourceLayout(_resourceLayout);
	graphicsCommandList->SetGraphicsPipeline(_pipeline);
	graphicsCommandList->SetVertexBuffer(_vertexBuffers[frameIndex]);
	graphicsCommandList->SetIndexBuffer (_indexBuffers[frameIndex], core::PixelFormat::R16_UINT);
	_resourceViews[0]->Bind(graphicsCommandList, 0);
	_engine->GetFrameBuffer(frameIndex)->GetRenderTargetSRV()->Bind(graphicsCommandList, 1);
	graphicsCommandList->DrawIndexedInstanced(
		static_cast<std::uint32_t>(_indexBuffers[frameIndex]->GetElementCount()), 1);
}
#pragma endregion Main Function

#pragma region Set up function

void Mosaic::PrepareBuffer(const float blockSize, const gu::tstring& name)
{
	const auto device = _engine->GetDevice();
	const auto metaData = GPUBufferMetaData::ConstantBuffer(sizeof(MosaicInfo), 1);

	const auto buffer = device->CreateBuffer(metaData);
	buffer->SetName(name + SP("MosaicInfo"));

	/*-------------------------------------------------------------------
	-			Set Information
	---------------------------------------------------------------------*/
	_mosaicInfo.WindowSize = {(float)_width, (float)_height };
	_mosaicInfo.BlockSize  = blockSize;
	_mosaicInfo.Padding    = 0.0f;

	buffer->UploadByte(&_mosaicInfo, metaData.GetTotalByte(), 0, nullptr);
	_resourceViews.Push(device->CreateResourceView(ResourceViewType::ConstantBuffer, buffer));
}


void Mosaic::PreparePipelineState(const gu::tstring& addName)
{
	const auto device  = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();

	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	_resourceLayout = device->CreateResourceLayout
	(
		{
			ResourceLayoutElement(DescriptorHeapType::CBV, 0), // Mosaic info
			ResourceLayoutElement(DescriptorHeapType::SRV, 0), // source texture
		},
		{ SamplerLayoutElement(device->CreateSampler(SamplerInfo::GetDefaultSampler(LinearClamp)), 0) }
	);

	/*-------------------------------------------------------------------
	-			SetShader
	---------------------------------------------------------------------*/
	const auto vs = factory->CreateShaderState();
	const auto ps = factory->CreateShaderState();
	vs->Compile({ ShaderType::Vertex, SP("Shader\\PostProcess\\ShaderScreenPass.hlsl"), SP("MainVS"), { SP("Shader\\Core")} });
	ps->Compile({ ShaderType::Pixel , SP("Shader\\PostProcess\\ShaderMosaic.hlsl"    ),SP("PSMain"),  {SP("Shader\\Core")} });

	/*-------------------------------------------------------------------
	-			Build Graphics Pipeline State
	---------------------------------------------------------------------*/
	_pipeline = device->CreateGraphicPipelineState(_engine->GetRenderPass(), _resourceLayout);
	_pipeline->SetBlendState(factory->CreateSingleBlendState(BlendProperty::OverWrite(true)));
	_pipeline->SetRasterizerState(factory->CreateRasterizerState(RasterizerProperty::Solid()));
	_pipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultVertexElement()));
	_pipeline->SetDepthStencilState(factory->CreateDepthStencilState());
	_pipeline->SetVertexShader(vs);
	_pipeline->SetPixelShader(ps);
	_pipeline->CompleteSetting();
	_pipeline->SetName(addName + SP("PSO"));
}

#pragma endregion Set up function