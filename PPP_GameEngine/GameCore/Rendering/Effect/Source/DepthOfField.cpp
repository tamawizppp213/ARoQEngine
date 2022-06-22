//////////////////////////////////////////////////////////////////////////////////
//              @file   DepthOfField.cpp
///             @brief  DepthOfField effect
///             @author Toide Yutaro
///             @date   2022_05_01
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Effect/Include/DepthOfField.hpp"
#include "GraphicsCore/RHI/DirectX12/SimpleInclude/IncludeGraphicsPSO.hpp"
#include "GraphicsCore/Engine/Include/GraphicsCoreEngine.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12PrimitiveGeometry.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Texture.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::directX12;
namespace
{
	RootSignature        s_RootSignature;
	ComputePipelineState s_VerticalPipeline;
	ComputePipelineState s_RhomboidPipeline;
	ComputePipelineState s_FinalRenderPipeline;

}
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
Dof::Dof()
{
	
}
Dof::~Dof()
{

}

#pragma region Public Function 
/****************************************************************************
*							StartOn
*************************************************************************//**
*  @fn        void Dof::StartOn(float width, float height, float radius, float nearClip, float farClip, const std::wstring& addName)
*  @brief     Start On
*  @param[in] float width 
*  @param[in] float height
*  @param[in] float radius
*  @param[in] float nearClip
*  @param[in] float farClip
*  @param[in] const std::wstring& addName
*  @return 　　void
*****************************************************************************/
void Dof::StartOn(float width, float height, float radius, float nearClip, float farClip, const std::wstring& addName)
{
	if (addName != L"") { _addName = addName; _addName += L"::"; }
	_addName += L"Dof::";
	for (auto& buffer : _colorBuffer)
	{
		buffer.Create(*GraphicsCoreEngine::Instance().GetGraphicsDevice(), static_cast<UINT>(width), static_cast<UINT>(height), 1, addName,
			GraphicsCoreEngine::Instance().GetGraphicsDevice()->GetBackBufferRenderFormat());
	}

	PrepareBlurParameterBuffer(width, height, radius);
	PrepareClipSizeBuffer(nearClip, farClip);
	PreparePipelineState();
}
void Dof::OnResize(float newWidth, float newHeight)
{
	for (auto& buffer : _colorBuffer)
	{
		buffer.OnResize(static_cast<UINT>(newWidth), static_cast<UINT>(newHeight), 1);
	}
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
void Dof::Draw(GPUResource* renderTarget, GPUResource* zPrepass)
{
	auto& engine       = GraphicsCoreEngine::Instance();
	auto  context      = engine.GetCommandContext();
	//int   currentFrame = engine.GetGraphicsDevice()->GetCurrentFrameIndex();

	_colorBuffer[0].CopyFrom(context, renderTarget);
	_colorBuffer[1].CopyFrom(context, zPrepass);
	/*-------------------------------------------------------------------
	-               Execute commandlist
	---------------------------------------------------------------------*/
	context->SetComputeRootSignature(s_RootSignature.GetSignature());
	context->SetComputeConstantBufferView(0, _blurParameterBuffer->GetGPUVirtualAddress());
	context->SetComputeConstantBufferView(1, _clipSizeBuffer->GetGPUVirtualAddress());
	/*-------------------------------------------------------------------
	-               Vertical Blur
	---------------------------------------------------------------------*/
	context->SetPipelineState(s_VerticalPipeline.GetPipelineState());
	context->SetComputeRootDescriptorTable(2, _colorBuffer[0].GetGPUSRV());
	context->SetComputeRootDescriptorTable(5, _colorBuffer[2].GetGPUUAV()); // dest common
	context->SetComputeRootDescriptorTable(6, _colorBuffer[3].GetGPUUAV());
	context->Dispatch((UINT)_colorBuffer[0].GetColorBuffer().get()->PixelSize.x / 16, (UINT)_colorBuffer[0].GetColorBuffer().get()->PixelSize.y / 16, 1);
	/*-------------------------------------------------------------------
	-               Rhomboid Blur
	---------------------------------------------------------------------*/
	context->SetPipelineState(s_RhomboidPipeline.GetPipelineState());
	context->SetComputeRootDescriptorTable(2, _colorBuffer[2].GetGPUSRV());
	context->SetComputeRootDescriptorTable(3, _colorBuffer[3].GetGPUSRV());
	context->SetComputeRootDescriptorTable(5, _colorBuffer[4].GetGPUUAV());
	context->Dispatch((UINT)_colorBuffer[0].GetColorBuffer().get()->PixelSize.x / 16, (UINT)_colorBuffer[0].GetColorBuffer().get()->PixelSize.y / 16, 1);
	/*-------------------------------------------------------------------
	-               FinalRender
	---------------------------------------------------------------------*/
	context->SetPipelineState(s_FinalRenderPipeline.GetPipelineState());
	context->SetComputeRootDescriptorTable(3, _colorBuffer[1].GetGPUSRV());
	context->SetComputeRootDescriptorTable(4, _colorBuffer[0].GetGPUSRV());
	context->SetComputeRootDescriptorTable(5, _colorBuffer[5].GetGPUUAV());
	context->Dispatch((UINT)_colorBuffer[0].GetColorBuffer().get()->PixelSize.x / 16, (UINT)_colorBuffer[0].GetColorBuffer().get()->PixelSize.y / 16, 1);
	context->CopyBuffer(renderTarget, &_colorBuffer[5].GetColorBuffer()->Resource);
}
/****************************************************************************
*							ShutDown
*************************************************************************//**
*  @fn        void Dof::ShutDown()
*  @brief     Shut down
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void Dof::ShutDown()
{
	_blurParameterBuffer.reset();
	_clipSizeBuffer.reset();
}
/****************************************************************************
*							SetUpBlurParameter
*************************************************************************//**
*  @fn        void Dof::SetUpBlurParameter(float width, float height, float radius)
*  @brief     Set up blur parameter
*  @param[in] float screenWidth
*  @param[in] float screenHeight
*  @param[in] float radius
*  @return 　　void
*****************************************************************************/
void Dof::SetUpBlurParameter(float width, float height, float radius)
{
	_blurParameter.TextureSize[0] = width;
	_blurParameter.TextureSize[1] = height;
	_blurParameter.Radius         = radius;
	_blurParameterBuffer->CopyStart();
	_blurParameterBuffer->CopyData(0, &_blurParameter);
	_blurParameterBuffer->CopyEnd();
}
/****************************************************************************
*							SetUpClipSize
*************************************************************************//**
*  @fn        void Dof::SetUpClipSize(float nearClip, float farClip)
*  @brief     Set up clip size
*  @param[in] float nearClip
*  @param[in] float farClip
*  @return 　　void
*****************************************************************************/
void Dof::SetUpClipSize(float nearClip, float farClip)
{
	_clipSize.Near = nearClip;
	_clipSize.Far  = farClip;
	_clipSizeBuffer->CopyStart();
	_clipSizeBuffer->CopyData(0, &_clipSize);
	_clipSizeBuffer->CopyEnd();
}
#pragma endregion Public Function
#pragma region Protected Function
/****************************************************************************
*							PrepareBlurParameterBuffer
*************************************************************************//**
*  @fn        void Dof::PrepareBlurParameterBuffer(float width, float height, float radius)
*  @brief     Prepare Blur Parameter Buffer
*  @param[in] float screenWidth
*  @param[in] float screenHeight
*  @param[in] float radius
*  @return 　　void
*****************************************************************************/
void Dof::PrepareBlurParameterBuffer(float width, float height, float radius)
{
	auto& engine = GraphicsCoreEngine::Instance();
	_blurParameterBuffer = std::make_unique<UploadBuffer>(engine.GetDevice(), static_cast<UINT>(sizeof(BlurParameter)), 1, true, _addName + L"BlurParameter");
	SetUpBlurParameter(width, height, radius);
}
/****************************************************************************
*							PrepareClipSizeBuffer
*************************************************************************//**
*  @fn        void Dof::PrepareClipSizeBuffer(float nearClip, float farClip)
*  @brief     Prepare Clip Size
*  @param[in] float nearClip
*  @param[in] float farClip
*  @return 　　void
*****************************************************************************/
void Dof::PrepareClipSizeBuffer(float nearClip, float farClip)
{
	auto& engine    = GraphicsCoreEngine::Instance();
	_clipSizeBuffer = std::make_unique<UploadBuffer>(engine.GetDevice(), static_cast<UINT>(sizeof(ClipSize)), 1, true, _addName + L"ClipSize");
	SetUpClipSize(nearClip, farClip);
}
/****************************************************************************
*							PreparePipelineState
*************************************************************************//**
*  @fn        void Dof::PreparePipelineState()
*  @brief     Prepare vertical, rhomboid, finalRender PSO
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void Dof::PreparePipelineState()
{
	std::wstring defaultPath = L"Shader\\Effect\\ShaderDepthOfField.hlsl";
	auto device = GraphicsCoreEngine::Instance().GetDevice();
	/*-------------------------------------------------------------------
	-			Set Root signature
	---------------------------------------------------------------------*/
	s_RootSignature.Reset(7, 1);
	s_RootSignature[0].InitAsCBV(0); // BlurEffect
	s_RootSignature[1].InitAsCBV(1); // ClipSize
	s_RootSignature[2].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1);
	s_RootSignature[3].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);
	s_RootSignature[4].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 1);
	s_RootSignature[5].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 0, 1);
	s_RootSignature[6].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 1);
	s_RootSignature.SetStaticSampler(GetStaticSampler(SamplerType::SamplerLinearWrap, 0));
	s_RootSignature.CompleteSetting(device, _addName + L"RootSignature");
	/*-------------------------------------------------------------------
	-			Load Blob Data
	---------------------------------------------------------------------*/
	BlobComPtr verticalCS, rhomboidCS, finalRenderCS;
	verticalCS    = CompileShader(defaultPath, L"VerticalBlur", L"cs_6_4");
	rhomboidCS    = CompileShader(defaultPath, L"RhomboidBlur", L"cs_6_4");
	finalRenderCS = CompileShader(defaultPath, L"FinalRender", L"cs_6_4");
	/*-------------------------------------------------------------------
	-			Load Blob Data
	---------------------------------------------------------------------*/
	s_VerticalPipeline   .SetRootSignature(s_RootSignature);
	s_RhomboidPipeline   .SetRootSignature(s_RootSignature);
	s_FinalRenderPipeline.SetRootSignature(s_RootSignature);
	s_VerticalPipeline   .SetComputeShader(verticalCS);
	s_RhomboidPipeline   .SetComputeShader(rhomboidCS);
	s_FinalRenderPipeline.SetComputeShader(finalRenderCS);
	s_VerticalPipeline   .CompleteSetting(device, _addName + L"Vertical Blur PipelineState");
	s_RhomboidPipeline   .CompleteSetting(device, _addName + L"Rhomboid Blur PipelineState");
	s_FinalRenderPipeline.CompleteSetting(device, _addName + L"Final Render PipelineState");
}
#pragma endregion Protected Function