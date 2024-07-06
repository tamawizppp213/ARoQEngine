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
	_vertexBuffers.Clear(); _vertexBuffers.ShrinkToFit();
	_indexBuffers.Clear(); _indexBuffers.ShrinkToFit();
}

ScreenSpaceReflection::ScreenSpaceReflection(const LowLevelGraphicsEnginePtr& engine, const ResourceViewPtr& normalMap, const ResourceViewPtr& depthMap, const SSRSettings& settings, const gu::tstring& addName)
	: _engine(engine), _normalMap(normalMap), _depthMap(depthMap)
{
	Checkf(_engine   , "engine     is nullptr");
	Checkf(_normalMap, "normal map is nullptr");
	Checkf(_depthMap , "depth map is nullptr");

	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	gu::tstring name = SP(""); if (addName != SP("")) { name += addName; name += SP("::"); }
	name += SP("SSR::");

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
	printf("width: %d, height: %d\n", newWidth, newHeight);
}

void ScreenSpaceReflection::Draw(const ResourceViewPtr& scene)
{
	if (!scene) { return; }
	if (_isSettingChanged)
	{
		_settingsView->GetBuffer()->UploadByte(&_settings, sizeof(_settings));
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

void ScreenSpaceReflection::PrepareBuffer(const SSRSettings& settings, const gu::tstring& name)
{
	const auto device = _engine->GetDevice();
	const auto metaData = GPUBufferMetaData::ConstantBuffer(sizeof(SSRSettings), 1);

	const auto buffer     = device->CreateBuffer(metaData);
	buffer->SetName(name + SP("ScreenSpaceReflectionInfo"));

	/*-------------------------------------------------------------------
	-			Set Information
	---------------------------------------------------------------------*/
	_settings = settings;
	buffer->UploadByte(&_settings, metaData.GetTotalByte(), 0, nullptr);
	_settingsView = device->CreateResourceView(ResourceViewType::ConstantBuffer, buffer);
}


void ScreenSpaceReflection::PreparePipelineState(const gu::tstring& addName)
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
	vs->Compile(ShaderType::Vertex, SP("Shader\\Effect\\ShaderSSR.hlsl"), SP("VSMain"), 6.4f, { SP("Shader\\Core") });
	ps->Compile(ShaderType::Pixel, SP("Shader\\Effect\\ShaderSSR.hlsl"), SP("ExecuteSSR"), 6.4f, { SP("Shader\\Core") });

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
	_pipeline->SetName(addName + SP("PSO"));
}

/****************************************************************************
*							PrepareVertexAndIndexBuffer
****************************************************************************/
/* @fn        void IFullScreenEffector::PrepareVertexAndIndexBuffer()
*  @brief     Prepare Rect Vertex and Index Buffer
*  @param[in] const gu::tstring& addName
*  @return �@�@void
*****************************************************************************/
void ScreenSpaceReflection::PrepareVertexAndIndexBuffer(const gu::tstring& addName)
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
	_vertexBuffers.Resize(frameCount);
	_indexBuffers.Resize(frameCount);
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
		{
			const auto vbMetaData = GPUBufferMetaData::VertexBuffer(vertexByteSize, vertexCount, MemoryHeap::Upload);
			const auto bufferName = addName + SP("VB");
			_vertexBuffers[i] = device->CreateBuffer(vbMetaData);
			_vertexBuffers[i]->SetName(gu::tstring(bufferName));
			_vertexBuffers[i]->UploadByte(rectMesh.Vertices.data(), vbMetaData.GetTotalByte()); // Map

		}
		
		/*-------------------------------------------------------------------
		-            Set Index Buffer
		---------------------------------------------------------------------*/
		{
			const auto ibMetaData = GPUBufferMetaData::IndexBuffer(indexByteSize, indexCount, MemoryHeap::Default, ResourceState::Common);
			const auto bufferName = addName + SP("IB");
			_indexBuffers[i] = device->CreateBuffer(ibMetaData);
			_indexBuffers[i]->SetName(bufferName);
			_indexBuffers[i]->UploadByte(rectMesh.Indices.data(), ibMetaData.GetTotalByte(), 0, commandList);
		}

	}
}

void ScreenSpaceReflection::PrepareResourceView()
{

}
#pragma endregion Set up function