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
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUTexture.hpp"
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
float GaussianDistribution(float r, float sigma)
{
	return expf(-0.5f * (float)(r * r) / (sigma * sigma));
}
GaussianBlur::GaussianBlur()
{

}
GaussianBlur::~GaussianBlur()
{
	_finalBlurPipeline.reset();
	_yBlurPipeline.reset();
	_xBlurPipeline.reset();
	for (auto& u : _unorderedResourceViews) { u.reset(); }
	for (auto& s : _shaderResourceViews   ) { s.reset(); }
	_textureSizeView.reset();
	_blurParameterView.reset();
}
GaussianBlur::GaussianBlur(const LowLevelGraphicsEnginePtr& engine, const std::uint32_t width, const std::uint32_t height, const std::wstring& addName)
	: _engine(engine)
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
void GaussianBlur::Draw()
{
	const auto device      = _engine->GetDevice();
	const auto frameIndex  = _engine->GetCurrentFrameIndex();
	const auto commandList = _engine->GetCommandList(CommandListType::Compute);
	const auto graphicsCommandList = _engine->GetCommandList(CommandListType::Graphics);
	const auto frameBuffer         = _engine->GetFrameBuffer(frameIndex);

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
	_blurParameterView->Bind(commandList, 0);
	_textureSizeView  ->Bind(commandList, 1);
	
	/*-------------------------------------------------------------------
	-               Execute XBlur Command
	---------------------------------------------------------------------*/
	frameBuffer->GetRenderTargetSRV()->Bind(commandList, 2);
	//_shaderResourceViews[0]->Bind(commandList, 2);
	_unorderedResourceViews[0]->Bind(commandList, 3);
	commandList->SetComputePipeline(_xBlurPipeline);
	commandList->Dispatch( _textureSize.XBlurTexture[0] / THREAD + 1, _textureSize.XBlurTexture[1] / THREAD + 1, 1);
	
	/*-------------------------------------------------------------------
	-               Execute YBlur Command
	---------------------------------------------------------------------*/
	_shaderResourceViews   [0]->Bind(commandList, 2);
	_unorderedResourceViews[1]->Bind(commandList, 3);
	commandList->SetComputePipeline(_yBlurPipeline);
	commandList->Dispatch( _textureSize.YBlurTexture[0] / THREAD + 1, _textureSize.YBlurTexture[1] / THREAD + 1, 1);
	
	/*-------------------------------------------------------------------
	-               Execute FinalBlur Command
	---------------------------------------------------------------------*/
	_shaderResourceViews[1]->Bind(commandList, 2);
	frameBuffer->GetRenderTargetUAV()->Bind(commandList, 3);
	commandList->SetComputePipeline(_finalBlurPipeline);
	commandList->Dispatch(_textureSize.OriginalTexture[0] / THREAD + 1, _textureSize.OriginalTexture[1] / THREAD + 1, 1);

	/*-------------------------------------------------------------------
	-               Restart current render pass
	---------------------------------------------------------------------*/
	waitValue = _engine->FlushGPUCommands(CommandListType::Compute, true);
	_engine->WaitExecutionGPUCommands(CommandListType::Graphics, waitValue, false);
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
		weights[i] = GaussianDistribution((float)i, sigma);
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
	
	const auto textureSizeBuffer = _engine->GetDevice()->CreateBuffer(metaData);
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
*  @brief     Prepare xblur, yblur, and finalblur PSO
*  @param[in] void
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
			ResourceLayoutElement(DescriptorHeapType::CBV, 0), // weight table 
			ResourceLayoutElement(DescriptorHeapType::CBV, 1), // texture size
			ResourceLayoutElement(DescriptorHeapType::SRV, 0), // source texture
			ResourceLayoutElement(DescriptorHeapType::UAV, 0), // dest texture
		},
		{ }
	);

	/*-------------------------------------------------------------------
	-			Load Blob data
	---------------------------------------------------------------------*/
	const auto xBlurCS = factory->CreateShaderState(); 
	const auto yBlurCS = factory->CreateShaderState();
	const auto finalBlurCS = factory->CreateShaderState();;
	xBlurCS    ->Compile(ShaderType::Compute, defaultPath, L"XBlur", 6.4f, { L"Shader\\Core" });
	yBlurCS    ->Compile(ShaderType::Compute, defaultPath, L"YBlur", 6.4f, { L"Shader\\Core" });
	finalBlurCS->Compile(ShaderType::Compute, defaultPath, L"FinalBlur", 6.4f, { L"Shader\\Core" });
	/*-------------------------------------------------------------------
	-			Set pipeline state
	---------------------------------------------------------------------*/
	_xBlurPipeline     = device->CreateComputePipelineState(_resourceLayout);
	_yBlurPipeline     = device->CreateComputePipelineState(_resourceLayout);
	_finalBlurPipeline = device->CreateComputePipelineState(_resourceLayout);

	_xBlurPipeline    ->SetComputeShader(xBlurCS);
	_yBlurPipeline    ->SetComputeShader(yBlurCS);
	_finalBlurPipeline->SetComputeShader(finalBlurCS);
	
	_xBlurPipeline    ->CompleteSetting();
	_yBlurPipeline    ->CompleteSetting();
	_finalBlurPipeline->CompleteSetting();

	_xBlurPipeline    ->SetName(name + L"XBlurPSO");
	_yBlurPipeline    ->SetName(name + L"YBlurPSO");
	_finalBlurPipeline->SetName(name + L"FinalBlurPSO");
}
/****************************************************************************
*							PrepareResourceView
*************************************************************************//**
*  @fn        void GaussianBlur::PrepareResourceView()
*  @brief     Prepare xblur, yblur, and finalblur resource views
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GaussianBlur::PrepareResourceView()
{
	const auto device = _engine->GetDevice();
	const auto format = _engine->GetBackBufferFormat();

	//  xblur
	{
		const auto srcData     = GPUTextureMetaData::Texture2D(Screen::GetScreenWidth()    , Screen::GetScreenHeight(), format);
		const auto dstData     = GPUTextureMetaData::Texture2D(Screen::GetScreenWidth() / 2, Screen::GetScreenHeight(), format, 1, ResourceUsage::UnorderedAccess);
		const auto srcTexture  = device->CreateTexture(srcData);
		const auto destTexture = device->CreateTexture(dstData);
		_unorderedResourceViews[0] = device->CreateResourceView(ResourceViewType::RWTexture, destTexture, nullptr); // x half texture uav
		_shaderResourceViews   [0] = device->CreateResourceView(ResourceViewType::Texture  , destTexture, nullptr); // x half texture srv
	}
	// yblur
	{
		const auto dstData         = GPUTextureMetaData::Texture2D(Screen::GetScreenWidth() / 2, Screen::GetScreenHeight() / 2, format, 1, ResourceUsage::UnorderedAccess);
		const auto destTexture     = device->CreateTexture(dstData);
		_unorderedResourceViews[1] = device->CreateResourceView(ResourceViewType::RWTexture, destTexture, nullptr);
		_shaderResourceViews[1]    = device->CreateResourceView(ResourceViewType::Texture  , destTexture, nullptr);
	}
	// finalblur
	{
		const auto dstData     = GPUTextureMetaData::Texture2D(Screen::GetScreenWidth()    , Screen::GetScreenHeight()    , format, 1, ResourceUsage::UnorderedAccess);
		const auto destTexture = device->CreateTexture(dstData);
		_unorderedResourceViews[2] = device->CreateResourceView(ResourceViewType::RWTexture, destTexture, nullptr);
	}
}
#pragma endregion Protected Function