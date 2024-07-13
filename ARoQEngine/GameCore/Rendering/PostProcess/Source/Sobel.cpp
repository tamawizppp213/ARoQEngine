//////////////////////////////////////////////////////////////////////////////////
//              @file   Blur.cpp
///             @brief  Blur effect
///             @author Toide Yutaro
///             @date   2022_04_30
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/Sobel.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"
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
Sobel::Sobel(const LowLevelGraphicsEnginePtr& engine, const std::uint32_t width, const std::uint32_t height, const gm::Float4& color, const gu::tstring& addName)
	: _engine(engine), _width(width), _height(height), _outlineInfo({ color })
{
#ifdef _DEBUG
	Check(_engine);
#endif
	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	gu::tstring name = SP(""); if (addName != SP("")) { name += addName; name += SP("::"); }
	name += SP("Sobel::");

	/*-------------------------------------------------------------------
	-            Prepare resource
	---------------------------------------------------------------------*/
	PrepareOutlineInfo(name);
	PreparePipelineState(name);
	PrepareResourceView();

}

Sobel::~Sobel()
{

}

#pragma region Main Function
/****************************************************************************
*							OnResize
****************************************************************************/
/* @fn        void Sobel::OnResize(int newWidth, int newHeight)
* 
*  @brief     OnResize
* 
*  @param[in] std::uint32_t newWidth
* 
*  @param[in] std::uint32_t newHeight
* 
*  @return 　　void
*****************************************************************************/
void Sobel::OnResize(const std::uint32_t newWidth, const std::uint32_t newHeight)
{
	printf("width: %d, height: %d\n", newWidth, newHeight);
}

/****************************************************************************
*							Draw
****************************************************************************/
/* @fn        void Draw(const ResourceViewPtr& sourceSRV)
*
*  @brief     Draw sobel filter. 
*
*  @param[in] const ResourceViewPtr& source render target SRV
*
*  @return 　　void
*****************************************************************************/
void Sobel::Draw(const ResourceViewPtr& sourceSRV)
{
	if (!sourceSRV) { OutputDebugStringA("source SRV is nullptr\n"); return; }

	const auto device          = _engine->GetDevice();
	const auto computeCommand  = _engine->GetCommandList(CommandListType::Compute);
	const auto graphicsCommand = _engine->GetCommandList(CommandListType::Graphics);

	/*-------------------------------------------------------------------
	-               Pause current render pass
	---------------------------------------------------------------------*/
	auto waitValue = _engine->FlushGPUCommands(CommandListType::Graphics, true);
	_engine->WaitExecutionGPUCommands(CommandListType::Compute, waitValue, false);

	/*-------------------------------------------------------------------
	-               Execute commandlist
	---------------------------------------------------------------------*/
	computeCommand->SetDescriptorHeap(_outlineCBV->GetHeap());
	computeCommand->SetComputeResourceLayout(_resourceLayout);
	_outlineCBV->Bind(computeCommand, 0);  // outline constant buffer
	sourceSRV  ->Bind(computeCommand, 1);  // render frame srv
	_uav       ->Bind(computeCommand, 2);  // output frame buffer
	
	computeCommand->SetComputePipeline(_pipeline);
	computeCommand->Dispatch((_width + THREAD - 1) / THREAD, (_height + THREAD - 1) / THREAD, 1);

	/*-------------------------------------------------------------------
	-               Restart current render pass
	---------------------------------------------------------------------*/
	waitValue = _engine->FlushGPUCommands(CommandListType::Compute, true);
	_engine->WaitExecutionGPUCommands(CommandListType::Graphics, waitValue, false);
}

void Sobel::SetColor(const gm::Float4& color)
{
	_outlineInfo.Color = color;
	_outlineCBV->GetBuffer()->UploadByte(&_outlineInfo, sizeof(_outlineInfo));
}

#pragma endregion Main Function

#pragma region SetUp Function
/****************************************************************************
*							PrepareOutlineInfo
****************************************************************************/
/* @fn        void Sobel::PrepareOutlineInfo(const gu::tstring& name)
*
*  @brief     Prepare outline info structure and GPU buffer
*
*  @param[in] const gu::tstring& name
*
*  @return 　　void
*****************************************************************************/
void Sobel::PrepareOutlineInfo(const gu::tstring& name)
{
	const auto device   = _engine->GetDevice();
	const auto metaData = GPUBufferMetaData::ConstantBuffer(sizeof(OutlineInfo), 1, MemoryHeap::Upload, ResourceState::Common);
	const auto buffer   = device->CreateBuffer(metaData);
	buffer->SetName(name + SP("OutlineInfo"));

	/*-------------------------------------------------------------------
	-			Set OutlineInfo Parameter
	---------------------------------------------------------------------*/
	// pack
	_outlineInfo.Color = _outlineInfo.Color;
	buffer->UploadByte(&_outlineInfo, metaData.GetTotalByte(), 0, nullptr);
	
	// create resource view
	_outlineCBV = device->CreateResourceView(ResourceViewType::ConstantBuffer, buffer, 0,0,nullptr);
}

/****************************************************************************
*							PrepareResourceView
****************************************************************************/
/* @fn        void Sobel::PrepareResourceView()
*
*  @brief     Prepare SRV and UAV
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void Sobel::PrepareResourceView()
{
	const auto device = _engine->GetDevice();
	const auto format = _engine->GetBackBufferFormat();

	// sobel texture
	{
		const auto metaData = GPUTextureMetaData::Texture2D(Screen::GetScreenWidth(), Screen::GetScreenHeight(), format, 1, TextureCreateFlags::UnorderedAccess);
		const auto texture = device->CreateTexture(metaData);
		_uav = device->CreateResourceView(ResourceViewType::RWTexture, texture,0,0, nullptr);
		_srv = device->CreateResourceView(ResourceViewType::Texture  , texture,0,0, nullptr);
	}
}

/****************************************************************************
*							PreparePipelineState
****************************************************************************/
/* @fn        void Sobel::PreparePipelineState(const gu::tstring& name)
*
*  @brief     Prepare Compute pipeline state
*
*  @param[in] const gu::tstring& name
*
*  @return 　　void
*****************************************************************************/
void Sobel::PreparePipelineState(const gu::tstring& name)
{
	const gu::tstring defaultPath = SP("Shader\\Effect\\ShaderSobel.hlsl");
	const auto device  = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();

	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	_resourceLayout = device->CreateResourceLayout
	(
		{
			ResourceLayoutElement(DescriptorHeapType::CBV, 0), // color
			ResourceLayoutElement(DescriptorHeapType::SRV, 0), // source texture
			ResourceLayoutElement(DescriptorHeapType::UAV, 0)  // dest texture
		},
		{}
	);

	/*-------------------------------------------------------------------
	-			Load Blob data
	---------------------------------------------------------------------*/
	const auto sobelCS = factory->CreateShaderState();
	sobelCS->Compile({ShaderType::Compute, defaultPath, SP("ExecuteSobel"),{} });

	/*-------------------------------------------------------------------
	-			Set pipeline state
	---------------------------------------------------------------------*/
	_pipeline = device->CreateComputePipelineState(_resourceLayout);
	_pipeline->SetComputeShader(sobelCS);
	_pipeline->CompleteSetting();
	_pipeline->SetName(name + SP("PSO"));
}
#pragma endregion SetUp Function