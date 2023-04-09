//////////////////////////////////////////////////////////////////////////////////
//              @file   Bloom.cpp
///             @brief  Bloom effect
///             @author Toide Yutaro
///             @date   2022_11_20
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Effect/Include/Bloom.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
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
using namespace rhi;
using namespace rhi::core;
using namespace gc;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
Bloom::Bloom(const LowLevelGraphicsEnginePtr& engine, const std::uint32_t width, const std::uint32_t height, const float power, 
	const ResourceViewPtr& customLuminanceSRV,
	const ResourceViewPtr& customLuminanceUAV,
	const std::wstring& addName)
	:_engine(engine), _explosion(power)
{
	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"GaussianBlur::";
	
	if (customLuminanceSRV && customLuminanceUAV) 
	{ 
#ifdef _DEBUG
		assert(customLuminanceSRV->GetResourceViewType() == ResourceViewType::Texture);
		assert(customLuminanceUAV->GetResourceViewType() == ResourceViewType::RWTexture);
#endif
		_luminanceSRV = customLuminanceSRV;
		_luminanceUAV = customLuminanceUAV;
	}
	PrepareGaussianBlurs(width, height, name);
	PreparePipelineState(name);
}

Bloom::~Bloom()
{

}
#pragma endregion Constructor and Destructor

#pragma region Main Function
void Bloom::Draw()
{
	const auto device      = _engine->GetDevice();
	const auto frameIndex  = _engine->GetCurrentFrameIndex();
	const auto computeCommandList  = _engine->GetCommandList(CommandListType::Compute);
	const auto graphicsCommandList = _engine->GetCommandList(CommandListType::Graphics);
	const auto frameBuffer         = _engine->GetFrameBuffer(frameIndex);

	/*-------------------------------------------------------------------
	-               Pause current render pass
	---------------------------------------------------------------------*/
	auto waitValue = _engine->FlushGPUCommands(CommandListType::Graphics, true);
	_engine->WaitExecutionGPUCommands(CommandListType::Compute, waitValue, false);

	/*-------------------------------------------------------------------
	-               Execute Luminance commandlist
	---------------------------------------------------------------------*/
	computeCommandList->SetDescriptorHeap(frameBuffer->GetRenderTargetSRV()->GetHeap());
	computeCommandList->SetResourceLayout(_resourceLayout);
	computeCommandList->SetComputePipeline(_luminancePipeline);
	frameBuffer->GetRenderTargetSRV()->Bind(computeCommandList, 0);
	_luminanceUAV->Bind(computeCommandList, 5);
	computeCommandList->Dispatch(Screen::GetScreenWidth() / THREAD, Screen::GetScreenHeight() / THREAD, 1);

	/*-------------------------------------------------------------------
	-               Execute Blur
	---------------------------------------------------------------------*/
	_gaussianBlur[0]->DrawCS(_luminanceSRV, _luminanceUAV);
	_gaussianBlur[1]->DrawCS(_shaderResourceViews[0], _unorderedResourceViews[0]);

	for (int i = 1; i <= ViewCount; ++i)
	{
		_gaussianBlur[i]->DrawCS(_shaderResourceViews[i - 1], _unorderedResourceViews[i - 1]); // luminance 1/4
	}
	
	/*-------------------------------------------------------------------
	-               Execute Luminance commandlist
	---------------------------------------------------------------------*/
	computeCommandList->SetComputePipeline(_finalBloomPipeline);
	_luminanceSRV->Bind(computeCommandList, 1);
	for (int i = 0; i < ViewCount; ++i)
	{
		_shaderResourceViews[i]->Bind(computeCommandList, i + 2);
	}
	frameBuffer->GetRenderTargetUAV()->Bind(computeCommandList, 5);
	computeCommandList->Dispatch(Screen::GetScreenWidth() / THREAD, Screen::GetScreenHeight() / THREAD, 1);

	/*-------------------------------------------------------------------
	-               Restart current render pass
	---------------------------------------------------------------------*/
	waitValue = _engine->FlushGPUCommands(CommandListType::Compute, true);
	_engine->WaitExecutionGPUCommands(CommandListType::Graphics, waitValue, false);
}
void Bloom::OnResize(const std::uint32_t newWidth, const std::uint32_t newHeight)
{

}
/****************************************************************************
*                          UpdateBloomPower
*************************************************************************//**
*  @fn        void Bloom::UpdateBloomPower(float power)
*  @brief     Update bloom power (Default value is 10)
*  @param[in] void
*  @return Å@Å@bool
*****************************************************************************/
void Bloom::UpdateBloomPower(const float power)
{
	for (std::uint32_t i = 0; i < _countof(_gaussianBlur); ++i)
	{
		_gaussianBlur[i]->SetUpWeightTable(power);
	}
}
#pragma endregion Main Function

#pragma region Set up Function
void Bloom::PrepareGaussianBlurs(const std::uint32_t width, const std::uint32_t height, const std::wstring& name)
{
	_gaussianBlur[0] = std::make_shared<GaussianBlur>(_engine, width, height, name + L"GaussianBlur");
	for (std::uint32_t i = 1; i < _countof(_gaussianBlur); ++i)
	{
		_gaussianBlur[i] = std::make_shared<GaussianBlur>(_engine, 
			width / pow(2, i),
			height / pow(2, i),
			name + L"GaussianBlur");
	}
}

void Bloom::PreparePipelineState(const std::wstring& name)
{
	const std::wstring defaultPath = L"Shader\\Effect\\ShaderBloom.hlsl";
	const auto device = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();

	_resourceLayout = device->CreateResourceLayout
	(
		{
			ResourceLayoutElement(DescriptorHeapType::SRV, 0), // main texture
			ResourceLayoutElement(DescriptorHeapType::SRV, 1), // blur texture0
			ResourceLayoutElement(DescriptorHeapType::SRV, 2), // blur texture1
			ResourceLayoutElement(DescriptorHeapType::SRV, 3), // blur texture2
			ResourceLayoutElement(DescriptorHeapType::SRV, 4), // blur texture3
			ResourceLayoutElement(DescriptorHeapType::UAV, 0)  // dest texture
		}, {}
	);

	const auto luminanceCS  = factory->CreateShaderState();
	const auto finalBloomCS = factory->CreateShaderState();
	luminanceCS->Compile(ShaderType::Compute, defaultPath, L"SamplingLuminance", 6.4f, {});
	finalBloomCS->Compile(ShaderType::Compute, defaultPath, L"FinalBloom", 6.4f, {});

	_luminancePipeline  = device->CreateComputePipelineState(_resourceLayout);
	_finalBloomPipeline = device->CreateComputePipelineState(_resourceLayout);

	_luminancePipeline->SetComputeShader(luminanceCS);
	_finalBloomPipeline->SetComputeShader(finalBloomCS);

	_luminancePipeline->CompleteSetting();
	_finalBloomPipeline->CompleteSetting();

	_luminancePipeline ->SetName(name + L"SamplingLuminancePSO");
	_finalBloomPipeline->SetName(name + L"FinalBloomPSO");
}

void Bloom::PrepareResourceView(const std::wstring& name)
{
	const auto device = _engine->GetDevice();
	const auto format = _engine->GetBackBufferFormat();

	for (int i = 0; i < _countof(_shaderResourceViews); ++i)
	{
		_shaderResourceViews[i]    = _gaussianBlur[i]->GetHalfDownSampledSRV();
		_unorderedResourceViews[i] = _gaussianBlur[i]->GetHalfDownSampledUAV();
	}

	if (!_luminanceSRV || !_luminanceUAV)
	{
		const auto metaData = GPUTextureMetaData::Texture2D(Screen::GetScreenWidth(), Screen::GetScreenHeight(), format, 1, ResourceUsage::UnorderedAccess);
		const auto texture = device->CreateTexture(metaData);
		_luminanceSRV = device->CreateResourceView(ResourceViewType::Texture  , texture, nullptr);
		_luminanceUAV = device->CreateResourceView(ResourceViewType::RWTexture, texture, nullptr);
	}
}
#pragma endregion Set up Function