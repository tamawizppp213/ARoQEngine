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
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUTexture.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"
#include "GameCore/Rendering/Model/Include/PrimitiveMesh.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gm;
using namespace gc;
using namespace rhi;
using namespace rhi::core;

namespace
{
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
#pragma region Constructor and Destructor
ColorChange::ColorChange()
{

}
ColorChange::~ColorChange()
{
	_pipeline.reset();
	_resourceLayout.reset();
	_resourceViews.clear(); _resourceViews.shrink_to_fit();
	_indexBuffers.clear(); _indexBuffers.shrink_to_fit();
	_vertexBuffers.clear(); _vertexBuffers.shrink_to_fit();
}
ColorChange::ColorChange(const ColorChangeType type, const LowLevelGraphicsEnginePtr& engine, const std::wstring& addName) : _engine(engine)
{
	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"ColorChange::";
	/*-------------------------------------------------------------------
	-           Prepare Pipeline
	---------------------------------------------------------------------*/
	PrepareVertexAndIndexBuffer(name);
	PreparePipelineState(type, name);
	PrepareResourceView();
}

#pragma endregion Constructor and Destructor
#pragma region Main Function
/****************************************************************************
*							OnResize
*************************************************************************//**
*  @fn        void ColorChange::OnResize(int newWidth, int newHeight)
*  @brief     OnResize
*  @param[in] int newWidth
*  @param[in] int newHeight
*  @return 　　void
*****************************************************************************/
void ColorChange::OnResize(int newWidth, int newHeight)
{
	
}
/****************************************************************************
*							Draw
*************************************************************************//**
*  @fn        void ColorChange::Draw
*  @brief     Render to Back Buffer
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void ColorChange::Draw()
{
	const auto frameIndex          = _engine->GetCurrentFrameIndex();
	const auto device              = _engine->GetDevice();
	const auto graphicsCommandList = _engine->GetCommandList(CommandListType::Graphics, frameIndex);

	/*-------------------------------------------------------------------
	-               Execute commandlist
	---------------------------------------------------------------------*/
	graphicsCommandList->SetResourceLayout(_resourceLayout);
	graphicsCommandList->SetGraphicsPipeline(_pipeline);
	graphicsCommandList->SetVertexBuffer(_vertexBuffers[frameIndex]);
	graphicsCommandList->SetIndexBuffer (_indexBuffers[frameIndex]);
	_resourceViews[frameIndex]->Bind(graphicsCommandList, 0);
	graphicsCommandList->DrawIndexedInstanced(_indexBuffers[frameIndex]->GetElementCount(), 1);
}
#pragma endregion Main Function

#pragma region Protected Function
/****************************************************************************
*							PrepareVertexAndIndexBuffer
*************************************************************************//**
*  @fn        void ColorChange::PrepareVertexAndIndexBuffer()
*  @brief     Prepare Rect Vertex and Index Buffer
*  @param[in] const std::wstring& addName
*  @return 　　void
*****************************************************************************/
void ColorChange::PrepareVertexAndIndexBuffer(const std::wstring& addName)
{
	const auto frameIndex = _engine->GetCurrentFrameIndex();
	const auto device     = _engine->GetDevice();
	const auto commandList = _engine->GetCommandList(CommandListType::Copy, frameIndex);
	/*-------------------------------------------------------------------
	-            Create Sphere Mesh
	---------------------------------------------------------------------*/
	MeshData rectMesh = PrimitiveMeshGenerator::Rect(2.0f, 2.0f, 0.0f);
	/*-------------------------------------------------------------------
	-            Create Mesh Buffer
	---------------------------------------------------------------------*/
	const auto frameCount = device->GetFrameCount();
	// prepare frame count buffer
	_vertexBuffers.resize(frameCount);
	_indexBuffers .resize(frameCount);
	for (std::uint32_t i = 0; i < frameCount; ++i)
	{
		/*-------------------------------------------------------------------
		-            Set up
		---------------------------------------------------------------------*/
		auto vertexByteSize = sizeof(Vertex);
		auto indexByteSize  = sizeof(std::uint32_t);
		auto vertexCount    = rectMesh.Vertices.size();
		auto indexCount     = rectMesh.Indices.size();

		/*-------------------------------------------------------------------
		-            Set Vertex Buffer 
		---------------------------------------------------------------------*/
		const auto vbMetaData = GPUBufferMetaData::VertexBuffer(vertexByteSize, vertexCount, MemoryHeap::Upload);
		_vertexBuffers[i] = device->CreateBuffer(vbMetaData);
		_vertexBuffers[i]->SetName(addName + L"VB");
		_vertexBuffers[i]->Pack(rectMesh.Vertices.data()); // Map

		/*-------------------------------------------------------------------
		-            Set Index Buffer
		---------------------------------------------------------------------*/
		const auto ibMetaData = GPUBufferMetaData::IndexBuffer(indexByteSize, indexCount, MemoryHeap::Default, ResourceState::Common);
		_indexBuffers[i] = device->CreateBuffer(ibMetaData);
		_indexBuffers[i]->SetName(addName + L"IB");
		_indexBuffers[i]->Pack(rectMesh.Indices.data(), commandList);

	}
}
/****************************************************************************
*							PreparePipelineState
*************************************************************************//**
*  @fn        void ColorChange::PreparePipelineState(ColorChangeType type)
*  @brief     Prepare PipelineState
*  @param[in] ColorChangeType type
*  @param[in] const std::wstring& addName
*  @return 　　void
*****************************************************************************/
void ColorChange::PreparePipelineState(ColorChangeType type, const std::wstring& addName)
{
	const auto device  = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();

	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	_resourceLayout = device->CreateResourceLayout
	(
		{ ResourceLayoutElement(DescriptorHeapType::SRV, 0)},
		{ SamplerLayoutElement(device->CreateSampler(SamplerInfo::GetDefaultSampler(SamplerLinearClamp)), 0) }
	);

	/*-------------------------------------------------------------------
	-			SetShader
	---------------------------------------------------------------------*/
	const auto vs = factory->CreateShaderState();
	const auto ps = factory->CreateShaderState();
	vs->Compile(ShaderType::Vertex, L"Shader\\Effect\\ShaderColorChange.hlsl", L"VSMain", 6.4f, { L"Shader\\Core" });
	ps->Compile(ShaderType::Pixel , L"Shader\\Effect\\ShaderColorChange.hlsl", s_ShaderFunctionName[(int)type], 6.4f, { L"Shader\\Core" });

	/*-------------------------------------------------------------------
	-			Build Graphics Pipeline State
	---------------------------------------------------------------------*/
	_pipeline = device->CreateGraphicPipelineState(_engine->GetRenderPass(), _resourceLayout);
	_pipeline->SetBlendState        (factory->CreateSingleBlendState(BlendProperty::OverWrite()));
	_pipeline->SetRasterizerState   (factory->CreateRasterizerState());
	_pipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultVertexElement()));
	_pipeline->SetDepthStencilState (factory->CreateDepthStencilState());
	_pipeline->SetVertexShader(vs);
	_pipeline->SetPixelShader (ps);
	_pipeline->CompleteSetting();
	_pipeline->SetName(addName + L"PSO");
}
/****************************************************************************
*							PrepareResourceView
*************************************************************************//**
*  @fn        void ColorChange::PrepareResourceView()
*  @brief     Prepare resource view and render texture (back buffer)
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void ColorChange::PrepareResourceView()
{
	const auto device     = _engine->GetDevice();
	const auto frameCount = device->GetFrameCount();
	const auto metaData = GPUTextureMetaData::Texture2D(Screen::GetScreenWidth(), Screen::GetScreenHeight(), _engine->GetBackBufferFormat());
	
	_resourceViews.resize(frameCount);
	for (size_t i = 0; i < _resourceViews.size(); ++i)
	{
		_resourceViews[i] = device->CreateResourceView(ResourceViewType::Texture, _engine->GetFrameBuffer(i)->GetRenderTarget());
	}
}
#pragma endregion Protected Function