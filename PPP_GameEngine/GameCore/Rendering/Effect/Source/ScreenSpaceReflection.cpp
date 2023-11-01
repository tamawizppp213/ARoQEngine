//////////////////////////////////////////////////////////////////////////////////
///             @file   ScreenSpaceReflection.hpp
///             @brief  ScreenSpaceReflection post effect
///             @author Toide Yutaro
///             @date   2023_02_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/ScreenSpaceReflection.hpp"
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


//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
ScreenSpaceReflection::ScreenSpaceReflection()
{

}

ScreenSpaceReflection::~ScreenSpaceReflection()
{
	_vertexBuffers.clear(); _vertexBuffers.shrink_to_fit();
	_indexBuffers.clear(); _indexBuffers.shrink_to_fit();
}

ScreenSpaceReflection::ScreenSpaceReflection(const LowLevelGraphicsEnginePtr& engine, const ResourceViewPtr& normalMap, const ResourceViewPtr& depthMap, const SSRSettings& settings, const std::wstring& addName)
	: _engine(engine), _normalMap(normalMap), _depthMap(depthMap)
{
	assert(("engine is nullptr", _engine));
	assert(("normal map is nullptr", _normalMap));
	assert(("depth map is nullptr", _depthMap));

	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"SSR::";

	/*-------------------------------------------------------------------
	-           Prepare Pipeline
	---------------------------------------------------------------------*/
	PrepareVertexAndIndexBuffer(name);
	PrepareBuffer(settings, name);
	PreparePipelineState(name);
	PrepareResourceView();
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
void ScreenSpaceReflection::OnResize(int newWidth, int newHeight)
{

}

void ScreenSpaceReflection::Draw(const ResourceViewPtr& scene)
{
	if (!scene) { return; }
	if (_isSettingChanged)
	{
		_settingsView->GetBuffer()->Update(&_settings, 1);
		_isSettingChanged = false;
	}

	const auto frameIndex  = _engine->GetCurrentFrameIndex();
	const auto frameBuffer = _engine->GetFrameBuffer(frameIndex);
	const auto baseMap     = frameBuffer->GetRenderTargetSRV();
	const auto device      = _engine->GetDevice();
	const auto graphicsCommandList = _engine->GetCommandList(CommandListType::Graphics);

	/*-------------------------------------------------------------------
	-               Execute commandlist
	---------------------------------------------------------------------*/
	graphicsCommandList->SetResourceLayout(_resourceLayout);
	graphicsCommandList->SetGraphicsPipeline(_pipeline);
	graphicsCommandList->SetVertexBuffer(_vertexBuffers[frameIndex]);
	graphicsCommandList->SetIndexBuffer(_indexBuffers[frameIndex]);
	scene->Bind(graphicsCommandList, 0);
	_settingsView->Bind(graphicsCommandList, 1);
	baseMap->Bind(graphicsCommandList, 2);
	_normalMap->Bind(graphicsCommandList, 3);
	_depthMap ->Bind(graphicsCommandList, 4);
	_engine->GetFrameBuffer(frameIndex)->GetRenderTargetSRV()->Bind(graphicsCommandList, 1);
	graphicsCommandList->DrawIndexedInstanced(
		static_cast<std::uint32_t>(_indexBuffers[frameIndex]->GetElementCount()), 1);
}
#pragma endregion Main Function

#pragma region Set up function

void ScreenSpaceReflection::PrepareBuffer(const SSRSettings& settings, const std::wstring& name)
{
	const auto device = _engine->GetDevice();
	const auto metaData = GPUBufferMetaData::ConstantBuffer(sizeof(SSRSettings), 1);

	const auto buffer = device->CreateBuffer(metaData);
	buffer->SetName(name + L"ScreenSpaceReflectionInfo");

	/*-------------------------------------------------------------------
	-			Set Information
	---------------------------------------------------------------------*/
	buffer->Pack(&_settings, nullptr);
	_settingsView = device->CreateResourceView(ResourceViewType::ConstantBuffer, buffer);
}


void ScreenSpaceReflection::PreparePipelineState(const std::wstring& addName)
{
	const auto device = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();

	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	_resourceLayout = device->CreateResourceLayout
	(
		{
			ResourceLayoutElement(DescriptorHeapType::CBV, 0), // Common resource
			ResourceLayoutElement(DescriptorHeapType::CBV, 1), // ScreenSpaceReflection info
			ResourceLayoutElement(DescriptorHeapType::SRV, 0), // source texture
			ResourceLayoutElement(DescriptorHeapType::SRV, 1), // normal texture
			ResourceLayoutElement(DescriptorHeapType::SRV, 2)  // depth map in the ndc space
		},
		{ 

			SamplerLayoutElement(device->CreateSampler(SamplerInfo::GetDefaultSampler(SamplerLinearWrap)), 0),
			SamplerLayoutElement(device->CreateSampler(SamplerInfo::GetDefaultSampler(SamplerPointClamp)), 1),
			SamplerLayoutElement(device->CreateSampler(SamplerInfo::GetDefaultSampler(SamplerLinearClamp)), 2),
		}
	);

	/*-------------------------------------------------------------------
	-			SetShader
	---------------------------------------------------------------------*/
	const auto vs = factory->CreateShaderState();
	const auto ps = factory->CreateShaderState();
	vs->Compile(ShaderType::Vertex, L"Shader\\Effect\\ShaderSSR.hlsl", L"VSMain", 6.4f, { L"Shader\\Core" });
	ps->Compile(ShaderType::Pixel, L"Shader\\Effect\\ShaderSSR.hlsl", L"ExecuteSSR", 6.4f, { L"Shader\\Core" });

	/*-------------------------------------------------------------------
	-			Build Graphics Pipeline State
	---------------------------------------------------------------------*/
	_pipeline = device->CreateGraphicPipelineState(_engine->GetRenderPass(), _resourceLayout);
	_pipeline->SetBlendState(factory->CreateSingleBlendState(BlendProperty::OverWrite()));
	_pipeline->SetRasterizerState(factory->CreateRasterizerState(RasterizerProperty::Solid()));
	_pipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultVertexElement()));
	_pipeline->SetDepthStencilState(factory->CreateDepthStencilState());
	_pipeline->SetVertexShader(vs);
	_pipeline->SetPixelShader(ps);
	_pipeline->CompleteSetting();
	_pipeline->SetName(addName + L"PSO");
}

/****************************************************************************
*							PrepareVertexAndIndexBuffer
*************************************************************************//**
*  @fn        void IFullScreenEffector::PrepareVertexAndIndexBuffer()
*  @brief     Prepare Rect Vertex and Index Buffer
*  @param[in] const std::wstring& addName
*  @return @@void
*****************************************************************************/
void ScreenSpaceReflection::PrepareVertexAndIndexBuffer(const std::wstring& addName)
{
	const auto device = _engine->GetDevice();
	const auto commandList = _engine->GetCommandList(CommandListType::Copy);
	/*-------------------------------------------------------------------
	-            Create Sphere Mesh
	---------------------------------------------------------------------*/
	core::PrimitiveMesh rectMesh = core::PrimitiveMeshGenerator::Rect(2.0f, 2.0f, 0.0f);
	/*-------------------------------------------------------------------
	-            Create Mesh Buffer
	---------------------------------------------------------------------*/
	const auto frameCount = LowLevelGraphicsEngine::FRAME_BUFFER_COUNT;
	// prepare frame count buffer
	_vertexBuffers.resize(frameCount);
	_indexBuffers.resize(frameCount);
	for (std::uint32_t i = 0; i < frameCount; ++i)
	{
		/*-------------------------------------------------------------------
		-            Set up
		---------------------------------------------------------------------*/
		auto vertexByteSize = sizeof(Vertex);
		auto indexByteSize = sizeof(std::uint32_t);
		auto vertexCount = rectMesh.Vertices.size();
		auto indexCount = rectMesh.Indices.size();

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

void ScreenSpaceReflection::PrepareResourceView()
{

}
#pragma endregion Set up function