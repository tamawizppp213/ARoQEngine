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
Bloom::Bloom(const LowLevelGraphicsEnginePtr& engine, const std::uint32_t width, const std::uint32_t height, const float power, const std::wstring& addName)
	:_engine(engine), _explosion(power)
{
	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"GaussianBlur::";
	
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
	const auto computeCommandList  = _engine->GetCommandList(CommandListType::Compute, frameIndex);
	const auto graphicsCommandList = _engine->GetCommandList(CommandListType::Graphics, frameIndex);

	/*-------------------------------------------------------------------
	-               Pause current render pass
	---------------------------------------------------------------------*/
	graphicsCommandList->EndRenderPass();
	graphicsCommandList->CopyResource(_shaderResourceViews[0]->GetTexture(), _engine->GetFrameBuffer(frameIndex)->GetRenderTarget(0));

	/*-------------------------------------------------------------------
	-               Execute Luminance commandlist
	---------------------------------------------------------------------*/
	computeCommandList->SetResourceLayout(_resourceLayout);
	computeCommandList->SetComputePipeline(_luminancePipeline);
	_shaderResourceViews   [0]->Bind(computeCommandList, 0);
	_unorderedResourceViews[0]->Bind(computeCommandList, 5);
	computeCommandList->Dispatch(Screen::GetScreenWidth() / THREAD, Screen::GetScreenHeight() / THREAD, 1);

	/*-------------------------------------------------------------------
	-               Execute Blur
	---------------------------------------------------------------------*/
	
	/*-------------------------------------------------------------------
	-               Execute Luminance commandlist
	---------------------------------------------------------------------*/
	computeCommandList->SetComputePipeline(_finalBloomPipeline);
	// set descriptor table
	computeCommandList->Dispatch(Screen::GetScreenWidth() / THREAD, Screen::GetScreenHeight() / THREAD, 1);

	// copy
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
	for (std::uint32_t i = 0; i < _countof(_gaussianBlur); ++i)
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


}
#pragma endregion Set up Function