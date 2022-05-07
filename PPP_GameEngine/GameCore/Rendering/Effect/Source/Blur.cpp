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
#include "GraphicsCore/DirectX12/SimpleInclude/IncludeGraphicsPSO.hpp"
#include "GraphicsCore/Engine/Include/GraphicsCoreEngine.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12PrimitiveGeometry.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12Texture.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace
{
	RootSignature s_RootSignature;
	ComputePipelineState s_xBlurPipeline;
	ComputePipelineState s_yBlurPipeline;
	ComputePipelineState s_finalBlurPipeline;
	constexpr int WEIGHT_TABLE_SIZE = 8;
	constexpr float DEFAULT_BLUR_SIGMA = 8.0f;
}

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

}

#pragma region Public Function
/****************************************************************************
*							StartOn
*************************************************************************//**
*  @fn        void GaussianBlur::OnResize(int newWidth, int newHeight)
*  @brief     Start Blur
*  @param[in] int width
*  @param[in] int height
*  @param[in] std::wstring& addName
*  @return 　　void
*****************************************************************************/
void GaussianBlur::StartOn(int width, int height, const std::wstring& addName)
{
	if (addName != L"") { _addName = addName; _addName += L"::"; }
	_addName += L"GaussianBlur::";

	for(auto& buffer : _colorBuffer)
	{
		buffer.Create(*GraphicsCoreEngine::Instance().GetGraphicsDevice(), width, height, 1, addName,
			GraphicsCoreEngine::Instance().GetGraphicsDevice()->GetBackBufferRenderFormat());
	}

	PrepareBlurParameters();
	PrepareTextureSizeBuffer(width, height);
	PreparePipelineState();
	SetUpWeightTable(DEFAULT_BLUR_SIGMA);
}
/****************************************************************************
*							OnResize
*************************************************************************//**
*  @fn        void GaussianBlur::OnResize(int newWidth, int newHeight)
*  @brief     OnResize
*  @param[in] int newWidth
*  @param[in] int newHeight
*  @return 　　void
*****************************************************************************/
void GaussianBlur::OnResize(int newWidth, int newHeight)
{
	_colorBuffer[0].OnResize(newWidth, newHeight, 1);
	_colorBuffer[1].OnResize(newWidth / 2, newHeight, 1);
	_colorBuffer[2].OnResize(newWidth / 2, newHeight / 2, 1);
	_colorBuffer[3].OnResize(newWidth, newHeight, 1);
	PrepareTextureSizeBuffer(newWidth, newHeight);
}
/****************************************************************************
*							Draw
*************************************************************************//**
*  @fn        void GaussianBlur::Draw(GPUResource* renderTarget)
*  @brief     Draw Gaussian blur
*  @param[in,out] GPUResource* renderTarget
*  @return 　　void
*****************************************************************************/
void GaussianBlur::Draw(GPUResource* renderTarget)
{
	auto& engine       = GraphicsCoreEngine::Instance();
	auto  context      = engine.GetCommandContext();

	_colorBuffer[0].CopyFrom(context, renderTarget);
	/*-------------------------------------------------------------------
	-               Execute commandlist
	---------------------------------------------------------------------*/
	context->SetComputeRootSignature(s_RootSignature.GetSignature());
	context->SetComputeConstantBufferView(0, _blurParameter->GetGPUVirtualAddress());
	context->SetComputeConstantBufferView(1, _textureSizeBuffer->GetGPUVirtualAddress());
	/*-------------------------------------------------------------------
	-               Execute XBlur Command
	---------------------------------------------------------------------*/
	context->SetPipelineState(s_xBlurPipeline.GetPipelineState());
	context->SetComputeRootDescriptorTable(2, _colorBuffer[0].GetGPUSRV());
	context->SetComputeRootDescriptorTable(3, _colorBuffer[1].GetGPUUAV());
	context->Dispatch( _textureSize.XBlurTexture[0] / 16, _textureSize.XBlurTexture[1] / 16, 1);
	/*-------------------------------------------------------------------
	-               Execute YBlur Command
	---------------------------------------------------------------------*/
	context->SetPipelineState(s_yBlurPipeline.GetPipelineState());
	context->SetComputeRootDescriptorTable(2, _colorBuffer[1].GetGPUSRV());
	context->SetComputeRootDescriptorTable(3, _colorBuffer[2].GetGPUUAV());
	context->Dispatch( _textureSize.YBlurTexture[0] / 16, _textureSize.YBlurTexture[1] / 16, 1);
	/*-------------------------------------------------------------------
	-               Execute FinalBlur Command
	---------------------------------------------------------------------*/
	context->SetPipelineState(s_finalBlurPipeline.GetPipelineState());
	context->SetComputeRootDescriptorTable(2, _colorBuffer[2].GetGPUSRV());
	context->SetComputeRootDescriptorTable(3, _colorBuffer[3].GetGPUUAV());
	context->Dispatch(_textureSize.OriginalTexture[0] / 16, _textureSize.OriginalTexture[1] / 16, 1);
	context->CopyBuffer(renderTarget, &_colorBuffer[3].GetColorBuffer()->Resource);
}
/****************************************************************************
*							ShutDown
*************************************************************************//**
*  @fn        void GaussianBlur::ShutDown()
*  @brief     Shut down
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GaussianBlur::ShutDown()
{
	_blurParameter.reset();
	_textureSizeBuffer.reset();
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
	for (int i = 0; i < WEIGHT_TABLE_SIZE; ++i)
	{
		weights[i] = expf(-0.5f * (float)(i * i) / (sigma * sigma));
		if (i == 0) { total += weights[i]; }
		else { total += 2.0f * weights[i]; }
	}

	// normalize
	for (int i = 0; i < 8; ++i)
	{
		weights[i] /= total;
	}

	// Copy weights table
	BlurParameter parameter;
	parameter.Weights[0] = gm::Float4(weights[0], weights[1], weights[2], weights[3]);
	parameter.Weights[1] = gm::Float4(weights[4], weights[5], weights[6], weights[7]);
	
	_blurParameter->CopyStart();
	_blurParameter->CopyTotalData(&parameter, 1);
	_blurParameter->CopyEnd();
}
#pragma endregion Public Function
#pragma region Protected Function
/****************************************************************************
*							PrepareBlurParameters
*************************************************************************//**
*  @fn        void GaussianBlur::PrepareBlurParameters()
*  @brief     Prepare Blur Parameter
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GaussianBlur::PrepareBlurParameters()
{
	auto& engine = GraphicsCoreEngine::Instance();

	BlurParameter parameter;
	parameter.Weights[0] = gm::Float4(0, 0, 0, 0);
	parameter.Weights[1] = gm::Float4(0, 0, 0, 0);

	_blurParameter = std::make_unique<UploadBuffer>(engine.GetDevice(), static_cast<UINT>(sizeof(BlurParameter)), 1, true, _addName + L"BlurParameter");
	_blurParameter->CopyStart();
	_blurParameter->CopyData(0, &parameter);
	_blurParameter->CopyEnd();
}
/****************************************************************************
*							PrepareTextureSizeBuffer
*************************************************************************//**
*  @fn        void GaussianBlur::PrepareTextureSizeBuffer(int width, int height)
*  @brief     Prepare Texture Size Buffer
*  @param[in] int width
*  @param[in] int height
*  @return 　　void
*****************************************************************************/
void GaussianBlur::PrepareTextureSizeBuffer(int width, int height)
{
	auto& engine         = GraphicsCoreEngine::Instance();
	auto parameterBuffer = std::make_unique<UploadBuffer>(engine.GetDevice(), static_cast<UINT>(sizeof(TextureSizeParameter)), 1, true, _addName + L"TextureSizeParameter");
	/*-------------------------------------------------------------------
	-			Set Texture Size Parameter
	---------------------------------------------------------------------*/
	_textureSize.OriginalTexture[0] = width;
	_textureSize.OriginalTexture[1] = height;
	_textureSize.XBlurTexture[0]    = width / 2;
	_textureSize.XBlurTexture[1]    = height;
	_textureSize.YBlurTexture[0]    = width / 2;
	_textureSize.YBlurTexture[1]    = height / 2;

	parameterBuffer->CopyStart();
	parameterBuffer->CopyData(0, &_textureSize);
	parameterBuffer->CopyEnd();

	_textureSizeBuffer = std::move(parameterBuffer);
}
/****************************************************************************
*							PreparePipelineState
*************************************************************************//**
*  @fn        void GaussianBlur::PreparePipelineState()
*  @brief     Prepare xblur, yblur, and finalblur PSO
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GaussianBlur::PreparePipelineState()
{
	std::wstring defaultPath = L"Shader\\Effect\\ShaderGaussianBlur.hlsl";
	auto device = GraphicsCoreEngine::Instance().GetDevice();
	/*-------------------------------------------------------------------
	-			Set Root signature
	---------------------------------------------------------------------*/
	s_RootSignature.Reset(4, 0);
	s_RootSignature[0].InitAsCBV(0); // weight table
	s_RootSignature[1].InitAsCBV(1); // texture size
	s_RootSignature[2].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1);
	s_RootSignature[3].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 0, 1);
	s_RootSignature.CompleteSetting(device,_addName + L"RootSignature");
	/*-------------------------------------------------------------------
	-			Load Blob data
	---------------------------------------------------------------------*/
	BlobComPtr xBlurCS, yBlurCS, finalBlurCS;
	xBlurCS     = CompileShader(defaultPath, L"XBlur", L"cs_6_4");
	yBlurCS     = CompileShader(defaultPath, L"YBlur", L"cs_6_4");
	finalBlurCS = CompileShader(defaultPath, L"FinalBlur", L"cs_6_4");
	/*-------------------------------------------------------------------
	-			Set pipeline state
	---------------------------------------------------------------------*/
	s_xBlurPipeline    .SetRootSignature(s_RootSignature);
	s_yBlurPipeline    .SetRootSignature(s_RootSignature);
	s_finalBlurPipeline.SetRootSignature(s_RootSignature);
	s_xBlurPipeline    .SetComputeShader(xBlurCS);
	s_yBlurPipeline    .SetComputeShader(yBlurCS);
	s_finalBlurPipeline.SetComputeShader(finalBlurCS);
	s_xBlurPipeline    .CompleteSetting(device, _addName + L"XBlurPSO");
	s_yBlurPipeline    .CompleteSetting(device, _addName + L"YBlurPSO");
	s_finalBlurPipeline.CompleteSetting(device, _addName + L"FinalBlurPSO");
}
#pragma endregion Protected Function