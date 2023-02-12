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
#include "GraphicsCore/Engine/Include/GraphicsCoreEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUTexture.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIResourceLayout.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"


//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;
using namespace gc;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
Dof::Dof(const LowLevelGraphicsEnginePtr& engine,
	const float width, const float height, const float radius,
	const float nearClip, const float farClip, const std::wstring& addName)
	: _engine(engine)
{
	std::wstring name = L"";
	if (addName != L"") { name = addName; name += L"::"; }
	name += L"Dof::";

	PrepareRenderBuffer();
	PrepareBlurParameterBuffer(width, height, radius, name);
	PrepareClipSizeBuffer(nearClip, farClip, name);
	PreparePipelineState(name);
}

Dof::~Dof()
{
	_shaderResourceViews.clear();
	_unorderedAccessViews.clear();
	_shaderResourceViews.shrink_to_fit();
	_unorderedAccessViews.shrink_to_fit();
}

#pragma endregion Constructor and Destructor
#pragma region Public Function 

void Dof::OnResize(float newWidth, float newHeight)
{
	
}
/****************************************************************************
*							Draw
*************************************************************************//**
*  @fn        void Dof::Draw(GPUResource* renderTarget, GPUResource* zPrepass) 
*  @brief     Draw 
*  @param[in] GPUResource* renderTarget
*  @param[in] GPUResource* zPrepass
*  @return 　　void
*****************************************************************************/
void Dof::Draw(const GPUTexturePtr& renderTarget, const GPUTexturePtr& zPrepass)
{
	const auto frameIndex   = _engine->GetCurrentFrameIndex();
	const auto computeList  = _engine->GetCommandList(CommandListType::Compute, frameIndex);
	const auto graphicsList = _engine->GetCommandList(CommandListType::Graphics, frameIndex);
	const auto frameBuffer  = _engine->GetFrameBuffer(frameIndex);

	/*-------------------------------------------------------------------
	-               Execute commandlist
	---------------------------------------------------------------------*/
	computeList->SetDescriptorHeap(_blurParameterView->GetHeap());
	computeList->SetComputeResourceLayout(_resourceLayout);
	_blurParameterView->Bind(computeList, 0);
	_clipSizeView     ->Bind(computeList, 1);

	/*-------------------------------------------------------------------
	-               Execute XBlur Command
	---------------------------------------------------------------------*/

	//_colorBuffer[0].CopyFrom(context, renderTarget);
	//_colorBuffer[1].CopyFrom(context, zPrepass);
	
	/*-------------------------------------------------------------------
	-               Vertical Blur
	---------------------------------------------------------------------*/
	computeList->SetComputePipeline(_verticalPipeline);
	//context->SetComputeRootDescriptorTable(2, _colorBuffer[0].GetGPUSRV());
	//context->SetComputeRootDescriptorTable(5, _colorBuffer[2].GetGPUUAV()); // dest common
	//context->SetComputeRootDescriptorTable(6, _colorBuffer[3].GetGPUUAV());
	//context->Dispatch((UINT)_colorBuffer[0].GetColorBuffer().get()->PixelSize.x / 16, (UINT)_colorBuffer[0].GetColorBuffer().get()->PixelSize.y / 16, 1);
	///*-------------------------------------------------------------------
	//-               Rhomboid Blur
	//---------------------------------------------------------------------*/
	computeList->SetComputePipeline(_rhomboidPipeline);
	//context->SetComputeRootDescriptorTable(2, _colorBuffer[2].GetGPUSRV());
	//context->SetComputeRootDescriptorTable(3, _colorBuffer[3].GetGPUSRV());
	//context->SetComputeRootDescriptorTable(5, _colorBuffer[4].GetGPUUAV());
	//context->Dispatch((UINT)_colorBuffer[0].GetColorBuffer().get()->PixelSize.x / 16, (UINT)_colorBuffer[0].GetColorBuffer().get()->PixelSize.y / 16, 1);
	///*-------------------------------------------------------------------
	//-               FinalRender
	//---------------------------------------------------------------------*/
	computeList->SetComputePipeline(_finalRenderPipeline);
	//context->SetComputeRootDescriptorTable(3, _colorBuffer[1].GetGPUSRV());
	//context->SetComputeRootDescriptorTable(4, _colorBuffer[0].GetGPUSRV());
	//context->SetComputeRootDescriptorTable(5, _colorBuffer[5].GetGPUUAV());
	//context->Dispatch((UINT)_colorBuffer[0].GetColorBuffer().get()->PixelSize.x / 16, (UINT)_colorBuffer[0].GetColorBuffer().get()->PixelSize.y / 16, 1);
	//context->CopyBuffer(renderTarget, &_colorBuffer[5].GetColorBuffer()->Resource);

	/*-------------------------------------------------------------------
	-               Restart current render pass
	---------------------------------------------------------------------*/
	graphicsList->CopyResource(frameBuffer->GetRenderTarget(), nullptr);
	graphicsList->BeginRenderPass(_engine->GetDrawContinueRenderPass(), frameBuffer);
}

/****************************************************************************
*							SetUpBlurParameter
*************************************************************************//**
*  @fn        void Dof::SetUpBlurParameter(const float width, const float height, const float radius)
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
	_blurParameterView->GetBuffer()->Update(&_blurParameter, 1);
}
/****************************************************************************
*							SetUpClipSize
*************************************************************************//**
*  @fn        void Dof::SetUpClipSize(const float nearClip, const float farClip)
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
	_clipSizeView->GetBuffer()->Update(&_clipSize, 1);
}
#pragma endregion Public Function
#pragma region Protected Function
void Dof::PrepareRenderBuffer()
{

}

/****************************************************************************
*							PrepareBlurParameterBuffer
*************************************************************************//**
*  @fn        void Dof::PrepareBlurParameterBuffer(const float width, const float height, const float radius, const std::wstring& name)
* 
*  @brief     Prepare Blur Parameter Buffer
* 
*  @param[in] const float screenWidth
* 
*  @param[in] const float screenHeight
* 
*  @param[in] const float radius
*  
*  @param[in] const std::wstring name
* 
*  @return 　　void
*****************************************************************************/
void Dof::PrepareBlurParameterBuffer(const float width, const float height, const float radius, const std::wstring& name)
{
	const auto device     = _engine->GetDevice();
	const auto bufferInfo = GPUBufferMetaData::ConstantBuffer(sizeof(BlurParameter), 1);
	const auto buffer     = device->CreateBuffer(bufferInfo);
	buffer->SetName(name + L"BlurParameter");
	SetUpBlurParameter(width, height, radius);
	device->CreateResourceView(ResourceViewType::ConstantBuffer, buffer);
}

/****************************************************************************
*							PrepareClipSizeBuffer
*************************************************************************//**
*  @fn        void Dof::PrepareClipSizeBuffer(float nearClip, float farClip, const std::wstring& name)
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
void Dof::PrepareClipSizeBuffer(float nearClip, float farClip, const std::wstring& name)
{
	const auto device     = _engine->GetDevice();
	const auto bufferInfo = GPUBufferMetaData::ConstantBuffer(sizeof(ClipSize), 1);
	const auto buffer     = device->CreateBuffer(bufferInfo);
	buffer->SetName(name + L"ClipSize");
	SetUpClipSize(nearClip, farClip);
	device->CreateResourceView(ResourceViewType::ConstantBuffer, buffer);
}
/****************************************************************************
*							PreparePipelineState
*************************************************************************//**
*  @fn        void Dof::PreparePipelineState(const std::wstring& name)
* 
*  @brief     Prepare vertical, rhomboid, finalRender PSO
* 
*  @param[in] const std::wstring& name
* 
*  @return 　　void
*****************************************************************************/
void Dof::PreparePipelineState(const std::wstring& name)
{
	std::wstring defaultPath = L"Shader\\Effect\\ShaderDepthOfField.hlsl";
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
			ResourceLayoutElement(DescriptorHeapType::SRV, 0),
			ResourceLayoutElement(DescriptorHeapType::SRV, 1),
			ResourceLayoutElement(DescriptorHeapType::SRV, 2),
			ResourceLayoutElement(DescriptorHeapType::UAV, 0),
			ResourceLayoutElement(DescriptorHeapType::UAV, 1)
		},
		{
			SamplerLayoutElement(device->CreateSampler(SamplerInfo::GetDefaultSampler(SamplerLinearWrap)),0)
		}
	);

	/*-------------------------------------------------------------------
	-			Load Blob Data
	---------------------------------------------------------------------*/
	const auto verticalCS    = factory->CreateShaderState();
	const auto rhomboidCS    = factory->CreateShaderState();
	const auto finalRenderCS = factory->CreateShaderState();
	verticalCS   ->Compile(ShaderType::Compute, defaultPath, L"VerticalBlur", 6.4f, { L"Shader\\Core" });
	rhomboidCS   ->Compile(ShaderType::Compute, defaultPath, L"RhomboidBlur", 6.4f, { L"Shader\\Core" });
	finalRenderCS->Compile(ShaderType::Compute, defaultPath, L"FinalRender", 6.4f, { L"Shader\\Core" });
	
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

	_verticalPipeline   ->SetName(name + L"Vertical Blur PipelineState");
	_rhomboidPipeline   ->SetName(name + L"Rhomboid Blur PipelineState");
	_finalRenderPipeline->SetName(name + L"Final Render PipelineState");
}
#pragma endregion Protected Function