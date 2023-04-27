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
Sobel::Sobel(const LowLevelGraphicsEnginePtr& engine, const std::uint32_t width, const std::uint32_t height, const gm::Float4& color, const std::wstring& addName)
	: _engine(engine), _width(width), _height(height), _outlineInfo({ color })
{
#ifdef _DEBUG
	assert(_engine);
#endif
	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"Sobel::";

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
*************************************************************************//**
*  @fn        void Sobel::OnResize(int newWidth, int newHeight)
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
	std::cout << "Sobel::OnResize未実装" << std::endl;
}

/****************************************************************************
*							Draw
*************************************************************************//**
*  @fn        void Draw(const ResourceViewPtr& sourceSRV)
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
	const auto frameIndex      = _engine->GetCurrentFrameIndex();

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
	_outlineCBV->GetBuffer()->Update(&_outlineInfo, 1);
}

#pragma endregion Main Function

#pragma region SetUp Function
/****************************************************************************
*							PrepareOutlineInfo
*************************************************************************//**
*  @fn        void Sobel::PrepareOutlineInfo(const std::wstring& name)
*
*  @brief     Prepare outline info structure and GPU buffer
*
*  @param[in] const std::wstring& name
*
*  @return 　　void
*****************************************************************************/
void Sobel::PrepareOutlineInfo(const std::wstring& name)
{
	const auto device   = _engine->GetDevice();
	const auto metaData = GPUBufferMetaData::ConstantBuffer(sizeof(OutlineInfo), 1, MemoryHeap::Upload, ResourceState::Common);
	const auto buffer   = device->CreateBuffer(metaData);
	buffer->SetName(name + L"OutlineInfo");

	/*-------------------------------------------------------------------
	-			Set OutlineInfo Parameter
	---------------------------------------------------------------------*/
	// pack
	_outlineInfo.Color = _outlineInfo.Color;
	buffer->Pack(&_outlineInfo, nullptr);
	
	// create resource view
	_outlineCBV = device->CreateResourceView(ResourceViewType::ConstantBuffer, buffer, nullptr);
}

/****************************************************************************
*							PrepareResourceView
*************************************************************************//**
*  @fn        void Sobel::PrepareResourceView()
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
		const auto metaData = GPUTextureMetaData::Texture2D(Screen::GetScreenWidth(), Screen::GetScreenHeight(), format, 1, ResourceUsage::UnorderedAccess);
		const auto texture = device->CreateTexture(metaData);
		_uav = device->CreateResourceView(ResourceViewType::RWTexture, texture, nullptr);
		_srv = device->CreateResourceView(ResourceViewType::Texture  , texture, nullptr);
	}
}

/****************************************************************************
*							PreparePipelineState
*************************************************************************//**
*  @fn        void Sobel::PreparePipelineState(const std::wstring& name)
*
*  @brief     Prepare Compute pipeline state
*
*  @param[in] const std::wstring& name
*
*  @return 　　void
*****************************************************************************/
void Sobel::PreparePipelineState(const std::wstring& name)
{
	const std::wstring defaultPath = L"Shader\\Effect\\ShaderSobel.hlsl";
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
	sobelCS->Compile(ShaderType::Compute, defaultPath, L"ExecuteSobel", 6.4f, {});

	/*-------------------------------------------------------------------
	-			Set pipeline state
	---------------------------------------------------------------------*/
	_pipeline = device->CreateComputePipelineState(_resourceLayout);
	_pipeline->SetComputeShader(sobelCS);
	_pipeline->CompleteSetting();
	_pipeline->SetName(name + L"PSO");
}
#pragma endregion SetUp Function