//////////////////////////////////////////////////////////////////////////////////
//              @file   DepthOfField.cpp
///             @brief  DepthOfField effect
///             @author Toide Yutaro
///             @date   2022_05_01
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DepthOfField.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUTexture.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIResourceLayout.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include <stdio.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;
using namespace engine;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
Dof::Dof(const LowLevelGraphicsEnginePtr& engine,
	const float width, const float height, const float radius,
	const float nearClip, const float farClip, const gu::tstring& addName)
	: _engine(engine)
{
	gu::tstring name = SP("");
	if (addName != SP("")) { name = addName; name += SP("::"); }
	name += SP("Dof::");

	PrepareRenderBuffer((size_t)width, (size_t)height);
	PrepareBlurParameterBuffer(width, height, radius, name);
	PrepareClipSizeBuffer(nearClip, farClip, name);
	PreparePipelineState(name);
}

Dof::~Dof()
{
	_shaderResourceViews.Clear();
	_unorderedAccessViews.Clear();
	_shaderResourceViews.ShrinkToFit();
	_unorderedAccessViews.ShrinkToFit();
}

#pragma endregion Constructor and Destructor
#pragma region Public Function 

void Dof::OnResize(float newWidth, float newHeight)
{
	printf("width: %f, height: %f\n", newWidth, newHeight);
}
/****************************************************************************
*							Draw
****************************************************************************/
/* @fn        void Dof::Draw(GPUResource* renderTarget, GPUResource* zPrepass) 
*  @brief     Draw 
*  @param[in] GPUResource* renderTarget
*  @param[in] GPUResource* zPrepass
*  @return 　　void
*****************************************************************************/
void Dof::Draw(const ResourceViewPtr& zPrepass)
{
	const auto frameIndex    = _engine->GetCurrentFrameIndex();
	const auto computeList   = _engine->GetCommandList(CommandListType::Compute);
	const auto graphicsList  = _engine->GetCommandList(CommandListType::Graphics);
	const auto frameBuffer   = _engine->GetFrameBuffer(frameIndex);
	const auto pixelWidth    = _blurParameter.TextureSize[0];
	const auto pixelHeight   = _blurParameter.TextureSize[1];
	const auto roundUpWidth  = (std::uint32_t)((pixelWidth + THREAD - 1) / THREAD) * THREAD;
	const auto roundUpHeight = (std::uint32_t)((pixelHeight + THREAD - 1) / THREAD) * THREAD;

	/*-------------------------------------------------------------------
	-               Pause current render pass
	---------------------------------------------------------------------*/
	auto waitValue = _engine->FlushGPUCommands(CommandListType::Graphics, false);
	_engine->WaitExecutionGPUCommands(CommandListType::Compute, waitValue, false);

	/*-------------------------------------------------------------------
	-               Execute commandlist
	---------------------------------------------------------------------*/
	computeList->SetDescriptorHeap(_blurParameterView->GetHeap());
	computeList->SetComputeResourceLayout(_resourceLayout);
	_blurParameterView->Bind(computeList, 0);
	_clipSizeView     ->Bind(computeList, 1);
	
	/*-------------------------------------------------------------------
	-               Vertical Blur
	---------------------------------------------------------------------*/
	computeList->SetComputePipeline(_verticalPipeline);
	
	frameBuffer->GetRenderTargetSRV()->Bind(computeList, 2);
	_unorderedAccessViews[0]->Bind(computeList, 5);  // vertical blur buffer 
	_unorderedAccessViews[1]->Bind(computeList, 6);  // diagonal blur buffer

	computeList->Dispatch(roundUpWidth / THREAD, roundUpHeight / THREAD, 1);

	//*-------------------------------------------------------------------
	//-               Rhomboid Blur
	//---------------------------------------------------------------------*/
	computeList->SetComputePipeline(_rhomboidPipeline);
	_shaderResourceViews [0]->Bind(computeList, 2); // vertical blur texture
	_shaderResourceViews [1]->Bind(computeList, 3); // diagonal blur texture
	_unorderedAccessViews[2]->Bind(computeList, 5); // rhomboid blur buffer

	computeList->Dispatch(roundUpWidth / THREAD, roundUpHeight / THREAD, 1);

	//*-------------------------------------------------------------------
	//-               FinalRender
	//---------------------------------------------------------------------*/
	computeList->SetComputePipeline(_finalRenderPipeline);
	frameBuffer->GetRenderTargetSRV()->Bind(computeList, 2); // original texture
	zPrepass   ->Bind(computeList, 4);                       // depth texture
	_shaderResourceViews[2]->Bind(computeList, 3);           // rhomboid blur texture
	_unorderedAccessViews[3]->Bind(computeList, 5);          // Final buffer 

	computeList->Dispatch(roundUpWidth / THREAD, roundUpHeight / THREAD, 1);

	/*-------------------------------------------------------------------
	-               Restart current render pass
	---------------------------------------------------------------------*/
	waitValue = _engine->FlushGPUCommands(CommandListType::Compute, true);
	_engine->WaitExecutionGPUCommands(CommandListType::Graphics, waitValue, false);

	graphicsList->BeginRecording(true);
	graphicsList->CopyResource(frameBuffer->GetRenderTarget(), _unorderedAccessViews[3]->GetTexture());
	graphicsList->BeginRenderPass(_engine->GetDrawContinueRenderPass(), frameBuffer);
}

/****************************************************************************
*							SetUpBlurParameter
****************************************************************************/
/* @fn        void Dof::SetUpBlurParameter(const float width, const float height, const float radius)
* 
*  @brief     Set up blur parameter
* 
*  @param[in] float screenWidth
* 
*  @param[in] float screenHeight
* 
*  @param[in] float radius
* 
*  @return 　　void
*****************************************************************************/
void Dof::SetUpBlurParameter(const float width, const float height, const float radius)
{
	_blurParameter.TextureSize[0] = width;
	_blurParameter.TextureSize[1] = height;
	_blurParameter.Radius         = radius;
	_blurParameterView->GetBuffer()->UploadByte(&_blurParameter, sizeof(_blurParameter));
}
/****************************************************************************
*							SetUpClipSize
****************************************************************************/
/* @fn        void Dof::SetUpClipSize(const float nearClip, const float farClip)
* 
*  @brief     Set up clip size
* 
*  @param[in] const float nearClip
* 
*  @param[in] const float farClip
* 
*  @return 　　void
*****************************************************************************/
void Dof::SetUpClipSize(float nearClip, float farClip)
{
	_clipSize.Near = nearClip;
	_clipSize.Far  = farClip;
	_clipSizeView->GetBuffer()->UploadByte(&_clipSize, sizeof(_clipSize));
}
#pragma endregion Public Function
#pragma region Protected Function
void Dof::PrepareRenderBuffer(const gu::uint32 width, const gu::uint32 height)
{
	const auto device = _engine->GetDevice();
	const auto format = _engine->GetBackBufferFormat();

	_shaderResourceViews.Resize(3);
	_unorderedAccessViews.Resize(4);
	for (size_t i = 0; i < _shaderResourceViews.Size(); ++i)
	{
		const auto textureInfo   = GPUTextureMetaData::Texture2D(width, height, format, 1, TextureCreateFlags::UnorderedAccess);
		const auto texture       = device->CreateTexture(textureInfo, SP("Dof::RWTexture"));
		_unorderedAccessViews[i] = device->CreateResourceView(ResourceViewType::Texture, texture);
		_shaderResourceViews[i]  = device->CreateResourceView(ResourceViewType::Texture, texture);
	}
	{
		const auto textureInfo = GPUTextureMetaData::Texture2D(width, height, format, 1, TextureCreateFlags::UnorderedAccess);
		const auto texture     = device->CreateTexture(textureInfo, SP("Dof::FinalBuffer"));
		_unorderedAccessViews[3] = device->CreateResourceView(ResourceViewType::Texture, texture);
	}
}

/****************************************************************************
*							PrepareBlurParameterBuffer
****************************************************************************/
/* @fn        void Dof::PrepareBlurParameterBuffer(const float width, const float height, const float radius, const gu::tstring& name)
* 
*  @brief     Prepare Blur Parameter Buffer
* 
*  @param[in] const float screenWidth
* 
*  @param[in] const float screenHeight
* 
*  @param[in] const float radius
*  
*  @param[in] const gu::tstring name
* 
*  @return 　　void
*****************************************************************************/
void Dof::PrepareBlurParameterBuffer(const float width, const float height, const float radius, const gu::tstring& name)
{
	const auto device     = _engine->GetDevice();
	const auto bufferInfo = GPUBufferMetaData::ConstantBuffer(sizeof(BlurParameter), 1);
	const auto buffer     = device->CreateBuffer(bufferInfo);
	buffer->SetName(name + SP("BlurParameter"));
	_blurParameterView = device->CreateResourceView(ResourceViewType::ConstantBuffer, buffer);
	SetUpBlurParameter(width, height, radius);
}

/****************************************************************************
*							PrepareClipSizeBuffer
****************************************************************************/
/* @fn        void Dof::PrepareClipSizeBuffer(float nearClip, float farClip, const gu::tstring& name)
* 
*  @brief     Prepare Clip Size
* 
*  @param[in] const float nearClip
* 
*  @param[in] const float farClip
* 
*  @param[in] const std:wstring& name
* 
*  @return 　　void
*****************************************************************************/
void Dof::PrepareClipSizeBuffer(float nearClip, float farClip, const gu::tstring& name)
{
	const auto device     = _engine->GetDevice();
	const auto bufferInfo = GPUBufferMetaData::ConstantBuffer(sizeof(ClipSize), 1);
	const auto buffer     = device->CreateBuffer(bufferInfo);
	buffer->SetName(name + SP("ClipSize"));
	_clipSizeView = device->CreateResourceView(ResourceViewType::ConstantBuffer, buffer);
	SetUpClipSize(nearClip, farClip);
}
/****************************************************************************
*							PreparePipelineState
****************************************************************************/
/* @fn        void Dof::PreparePipelineState(const gu::tstring& name)
* 
*  @brief     Prepare vertical, rhomboid, finalRender PSO
* 
*  @param[in] const gu::tstring& name
* 
*  @return 　　void
*****************************************************************************/
void Dof::PreparePipelineState(const gu::tstring& name)
{
	gu::tstring defaultPath = SP("Shader\\Effect\\ShaderDepthOfField.hlsl");
	const auto device = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();
	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	_resourceLayout = device->CreateResourceLayout
	(
		{
			ResourceLayoutElement(DescriptorHeapType::CBV, 0), // blur parameter
			ResourceLayoutElement(DescriptorHeapType::CBV, 1), // clip size
			ResourceLayoutElement(DescriptorHeapType::SRV, 0), // original texture, verticalBlurTexture
			ResourceLayoutElement(DescriptorHeapType::SRV, 1),
			ResourceLayoutElement(DescriptorHeapType::SRV, 2),
			ResourceLayoutElement(DescriptorHeapType::UAV, 0),
			ResourceLayoutElement(DescriptorHeapType::UAV, 1)
		},
		{
			SamplerLayoutElement(device->CreateSampler(SamplerInfo::GetDefaultSampler(LinearWrap)),0)
		}
	);

	/*-------------------------------------------------------------------
	-			Load Blob Data
	---------------------------------------------------------------------*/
	const auto verticalCS    = factory->CreateShaderState();
	const auto rhomboidCS    = factory->CreateShaderState();
	const auto finalRenderCS = factory->CreateShaderState();
	verticalCS  ->Compile({ ShaderType::Compute, defaultPath, SP("VerticalBlur"), { SP("Shader\\Core") } });
	rhomboidCS   ->Compile({ ShaderType::Compute, defaultPath, SP("RhomboidBlur"), { SP("Shader\\Core") } });
	finalRenderCS->Compile({ ShaderType::Compute, defaultPath, SP("FinalRender") , { SP("Shader\\Core") } });
	
	/*-------------------------------------------------------------------
	-			Load pipeline state
	---------------------------------------------------------------------*/
	_verticalPipeline    = device->CreateComputePipelineState(_resourceLayout);
	_rhomboidPipeline    = device->CreateComputePipelineState(_resourceLayout);
	_finalRenderPipeline = device->CreateComputePipelineState(_resourceLayout);
	
	_verticalPipeline   ->SetComputeShader(verticalCS);
	_rhomboidPipeline   ->SetComputeShader(rhomboidCS);
	_finalRenderPipeline->SetComputeShader(finalRenderCS);
	
	_verticalPipeline   ->CompleteSetting();
	_rhomboidPipeline   ->CompleteSetting();
	_finalRenderPipeline->CompleteSetting();

	_verticalPipeline   ->SetName(name + SP("Vertical Blur PipelineState"));
	_rhomboidPipeline   ->SetName(name + SP("Rhomboid Blur PipelineState"));
	_finalRenderPipeline->SetName(name + SP("Final Render PipelineState"));
}
#pragma endregion Protected Function