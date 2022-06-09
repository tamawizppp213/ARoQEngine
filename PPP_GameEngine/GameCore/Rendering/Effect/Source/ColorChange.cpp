//////////////////////////////////////////////////////////////////////////////////
//              @file   ColorChange.cpp
///             @brief  Color change post effect
///             @author Toide Yutaro
///             @date   2022_04_30
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Effect/Include/ColorChange.hpp"
#include "GraphicsCore/RHI/DirectX12/SimpleInclude/IncludeGraphicsPSO.hpp"
#include "GraphicsCore/Engine/Include/GraphicsCoreEngine.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12PrimitiveGeometry.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gm;
namespace
{
	RootSignature s_RootSignature;
	std::wstring s_ShaderFunctionName[ColorChangeType::CountOfType] = 
	{ 
		L"None",
		L"PSMonochrome",
		L"PSSepia",
		L"PSGrayScale",
		L"PSBinary",
		L"PSInvert",

	};
}

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
ColorChange::ColorChange()
{

}
ColorChange::~ColorChange()
{

}
/****************************************************************************
*							StartOn
*************************************************************************//**
*  @fn        void ColorChange::StartOn(ColorChangeType type, const std::wstring& addName)
*  @brief     Start ColorChange Effect
*  @param[in] ColorChangeType type
*  @param[in] const std::wstring& addName
*  @return 　　void
*****************************************************************************/
void ColorChange::StartOn(ColorChangeType type, const std::wstring& addName)
{
	if (addName != L"") { _addName = addName; _addName += L"::"; }
	_addName += L"ColorChange::";
	_colorBuffer.Create(*GraphicsCoreEngine::Instance().GetGraphicsDevice(), Screen::GetScreenWidth(), Screen::GetScreenHeight(), 1, addName,
		GraphicsCoreEngine::Instance().GetGraphicsDevice()->GetBackBufferRenderFormat());
	
	PrepareVertexAndIndexBuffer();
	PreparePipelineState(type);
}
/****************************************************************************
*							OnResize
*************************************************************************//**
*  @fn        void ColorChange::OnResize(int newWidth, int newHeight)
*  @brief     OnResize
*  @param[in] ColorChangeType type
*  @param[in] const std::wstring& addName
*  @return 　　void
*****************************************************************************/
void ColorChange::OnResize(int newWidth, int newHeight)
{
	_colorBuffer.OnResize(newWidth, newHeight, 1);
}
/****************************************************************************
*							OnResize
*************************************************************************//**
*  @fn        void ColorChange::OnResize(int newWidth, int newHeight)
*  @brief     OnResize
*  @param[in] ColorChangeType type
*  @param[in] const std::wstring& addName
*  @return 　　void
*****************************************************************************/
void ColorChange::Draw(GPUResource* renderTarget)
{
	auto& engine       = GraphicsCoreEngine::Instance();
	auto  context      = engine.GetCommandContext();
	int   currentFrame = engine.GetGraphicsDevice()->GetCurrentFrameIndex();
	/*-------------------------------------------------------------------
	-               Execute commandlist
	---------------------------------------------------------------------*/
	_colorBuffer.CopyFrom(context, renderTarget);
	context->SetRootSignature(s_RootSignature.GetSignature());
	context->SetPipelineState(_pipelineState->GetPipelineState());
	context->SetVertexBuffer(_meshBuffer[currentFrame].VertexBufferView());
	context->SetIndexBuffer (_meshBuffer[currentFrame].IndexBufferView());
	context->SetGraphicsRootDescriptorTable(0, _colorBuffer.GetGPUSRV());
	context->DrawIndexedInstanced(_meshBuffer[currentFrame].IndexBuffer->GetElementCount(),1);
}
/****************************************************************************
*							ShutDown
*************************************************************************//**
*  @fn        void ColorChange::ShutDown()
*  @brief     ShutDownColorChangeEffect
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void ColorChange::ShutDown()
{
	_meshBuffer.get()->Dispose();
	_pipelineState.reset();
}

#pragma region Protected Function
/****************************************************************************
*							PrepareVertexAndIndexBuffer
*************************************************************************//**
*  @fn        void ColorChange::PrepareVertexAndIndexBuffer()
*  @brief     Prepare Rect Vertex and Index Buffer
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void ColorChange::PrepareVertexAndIndexBuffer()
{
	/*-------------------------------------------------------------------
	-            Create Sphere Mesh
	---------------------------------------------------------------------*/
	MeshData rectMesh = GeometryGenerator::Rect(2.0f, 2.0f, 0.0f);
	/*-------------------------------------------------------------------
	-            Create Mesh Buffer
	---------------------------------------------------------------------*/
	auto& engine       = GraphicsCoreEngine::Instance();
	int totalFrameSize = engine.GetFrameBufferCount();
	_meshBuffer = std::make_unique<MeshBuffer[]>(totalFrameSize);
	for (int i = 0; i < totalFrameSize; ++i)
	{
		auto& meshBuffer = _meshBuffer[i];
		/*-------------------------------------------------------------------
		-            Calcurate Buffer Size
		---------------------------------------------------------------------*/
		auto vertexByteSize = sizeof(VertexPositionNormalTexture);
		auto indexByteSize  = sizeof(UINT16);
		auto vertexCount    = rectMesh.Vertices.size();
		auto indexCount     = rectMesh.Indices.size();
		/*-------------------------------------------------------------------
		-            Set Vertex Buffer and Index Buffer
		---------------------------------------------------------------------*/
		meshBuffer.VertexBuffer = std::make_unique<UploadBuffer>(engine.GetDevice(), static_cast<UINT>(vertexByteSize), static_cast<UINT>(vertexCount), false, _addName + L"VertexBuffer");
		meshBuffer.VertexBuffer->CopyStart();
		meshBuffer.VertexBuffer->CopyTotalData(rectMesh.Vertices.data(), static_cast<int>(vertexCount));
		meshBuffer.VertexBuffer->CopyEnd();

		meshBuffer.IndexBuffer  = std::make_unique<UploadBuffer>(engine.GetDevice(), static_cast<UINT>(indexByteSize), static_cast<UINT>(indexCount), false, _addName + L"IndexBuffer");
		meshBuffer.IndexBuffer->CopyStart();
		meshBuffer.IndexBuffer->CopyTotalData(rectMesh.Indices.data(), static_cast<int>(indexCount));
		meshBuffer.IndexBuffer->CopyEnd();

	}
}
/****************************************************************************
*							PreparePipelineState
*************************************************************************//**
*  @fn        void ColorChange::PreparePipelineState(ColorChangeType type)
*  @brief     Prepare PipelineState
*  @param[in] ColorChangeType type
*  @return 　　void
*****************************************************************************/
void ColorChange::PreparePipelineState(ColorChangeType type)
{
	auto device = GraphicsCoreEngine::Instance().GetGraphicsDevice();
	/*-------------------------------------------------------------------
	-			SetShader
	---------------------------------------------------------------------*/
	BlobComPtr vs = CompileShader(L"Shader\\Effect\\ShaderColorChange.hlsl", L"VSMain", L"vs_6_4");
	BlobComPtr ps = CompileShader(L"Shader\\Effect\\ShaderColorChange.hlsl", s_ShaderFunctionName[(int)type], L"ps_6_4");
	/*-------------------------------------------------------------------
	-			Build root signature
	---------------------------------------------------------------------*/
	s_RootSignature.Reset(1, 1);
	s_RootSignature.SetStaticSampler(SamplerType::SamplerLinearClamp);
	s_RootSignature[0].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1);
	s_RootSignature.CompleteSetting(device->GetDevice(), _addName + L"RootSignature");
	/*-------------------------------------------------------------------
	-			Build Graphics Pipeline State
	---------------------------------------------------------------------*/
	auto pipelineState = std::make_unique<GraphicsPipelineState>();
	pipelineState->SetGraphicsPipelineStateDescriptor(device->GetDefaultPSOConfig());
	pipelineState->SetRootSignature(s_RootSignature);
	pipelineState->SetInputLayouts(VertexPositionNormalTexture::InputLayout);
	pipelineState->SetBlendState(GetBlendState(BlendStateType::OverWrite));
	pipelineState->SetVertexShader(vs);
	pipelineState->SetPixelShader(ps);
	pipelineState->CompleteSetting(device->GetDevice(), _addName + L"PSO");
	_pipelineState = std::move(pipelineState);
}
#pragma endregion Protected Function