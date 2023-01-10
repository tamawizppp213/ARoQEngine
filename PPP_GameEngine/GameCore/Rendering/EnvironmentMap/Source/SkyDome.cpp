//////////////////////////////////////////////////////////////////////////////////
//              @file   Skybox.cpp
///             @brief  Skybox
///             @author Toide Yutaro
///             @date   2022/04/16
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/EnvironmentMap/Include/SkyDome.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GameUtility/Math/Include/GMMatrix.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUTexture.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"
#include "GameCore/Rendering/Model/Include/PrimitiveMesh.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gm;
using namespace gc;
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
SkyDome::SkyDome() {};
SkyDome::~SkyDome()
{
	_skyObject.reset();
	_vertexBuffers.clear(); _vertexBuffers.shrink_to_fit();
	_indexBuffers.clear(); _indexBuffers.shrink_to_fit();
	_resourceViews.clear(); _resourceViews.shrink_to_fit();
	_resourceLayout.reset();
	_pipeline.reset();
}
#pragma region Constructor and Destructor 
SkyDome::SkyDome(const LowLevelGraphicsEnginePtr& engine, const std::wstring& cubeMapPath, const std::wstring& addName)
	:_engine(engine)
{
	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"Skybox::";

	/*-------------------------------------------------------------------
	-           Load Texture
	---------------------------------------------------------------------*/
	const auto frameIndex  = _engine->GetCurrentFrameIndex();
	const auto device      = _engine->GetDevice();
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics, frameIndex);
	const auto texture     = device->CreateTextureEmpty();
	texture->Load(cubeMapPath, commandList);
	texture->SetName(cubeMapPath);
	/*-------------------------------------------------------------------
	-           Prepare Pipeline
	---------------------------------------------------------------------*/
	PrepareVertexAndIndexBuffer(name);
	PrepareSkyObject(name);
	PreparePipelineState(name);
	PrepareResourceView(texture);
}
#pragma endregion Constructor and Destructor

void SkyDome::Draw(const GPUResourceViewPtr& cameraResourceView) 
{
	const auto currentFrame = _engine->GetCurrentFrameIndex();
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics, currentFrame);
	
	/*-------------------------------------------------------------------
	-               Execute commandlist
	---------------------------------------------------------------------*/
	commandList->SetResourceLayout(_resourceLayout);
	commandList->SetGraphicsPipeline(_pipeline);
	commandList->SetVertexBuffer(_vertexBuffers[currentFrame]);
	commandList->SetIndexBuffer (_indexBuffers[currentFrame]);
	for (size_t i = 0; i < _resourceViews.size(); ++i) 
	{
		commandList->SetDescriptorHeap(_resourceViews[i].second->GetHeap());
		_resourceViews[i].second->Bind(commandList, _resourceViews[i].first);
	}
	cameraResourceView->Bind(commandList, 0);
	commandList->DrawIndexedInstanced(static_cast<std::uint32_t>(_indexBuffers[currentFrame]->GetElementCount()), 1);
}


#pragma region Private Function
/****************************************************************************
*							PrepareVertexAndIndexBuffer
*************************************************************************//**
*  @fn        void Skybox::PrepareVertexAndIndexBuffer(const std::wstring& addName)
*  @brief     Prepare Sphere Vertex Buffer
*  @param[in] const std::wstring& addName
*  @return 　　void
*****************************************************************************/
void SkyDome::PrepareVertexAndIndexBuffer(const std::wstring& addName)
{
	const auto frameIndex  = _engine->GetCurrentFrameIndex();
	const auto device      = _engine->GetDevice();
	const auto commandList = _engine->GetCommandList(CommandListType::Copy, frameIndex); 
	/*-------------------------------------------------------------------
	-            Create Sphere Mesh
	---------------------------------------------------------------------*/
	PrimitiveMesh sphereMesh = PrimitiveMeshGenerator::Sphere(0.5f, 20, 20, false);    // Sphere mesh
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
		const auto vertexByteSize = sizeof(Vertex);       
		const auto indexByteSize  = sizeof(std::uint32_t);                                // 4 byte index
		const auto vertexCount    = sphereMesh.Vertices.size();
		const auto indexCount     = sphereMesh.Indices.size();

		/*-------------------------------------------------------------------
		-            Set Vertex Buffer 
		---------------------------------------------------------------------*/
		const auto vbMetaData = GPUBufferMetaData::VertexBuffer(vertexByteSize, vertexCount, MemoryHeap::Upload);
		_vertexBuffers[i] = device->CreateBuffer(vbMetaData);
		_vertexBuffers[i]->SetName(addName + L"VB");
		_vertexBuffers[i]->Pack(sphereMesh.Vertices.data()); // Map

		/*-------------------------------------------------------------------
		-            Set Index Buffer
		---------------------------------------------------------------------*/
		const auto ibMetaData = GPUBufferMetaData::IndexBuffer(indexByteSize, indexCount, MemoryHeap::Default, ResourceState::Common);
		_indexBuffers[i] = device->CreateBuffer(ibMetaData);
		_indexBuffers[i]->SetName(addName + L"IB");
		_indexBuffers[i]->Pack(sphereMesh.Indices.data(), commandList);

	}
}
/****************************************************************************
*							PrepareSkyObject
*************************************************************************//**
*  @fn        void Skybox::PrepareSkyObject(const std::wstring& addName)
*  @brief     Build World Matrix Infomation
*  @param[in] std::wstring& addName
*  @return 　　void
*****************************************************************************/
void SkyDome::PrepareSkyObject(const std::wstring& addName)
{
	const auto frameIndex  = _engine->GetCurrentFrameIndex();
	const auto device      = _engine->GetDevice();
	const auto commandList = _engine->GetCommandList(CommandListType::Copy, frameIndex);

	/*-------------------------------------------------------------------
	-			Set Skydata
	---------------------------------------------------------------------*/
	Matrix4 skyData; // sphere
	skyData = gm::Scaling(SKY_SCALE, SKY_SCALE, SKY_SCALE).ToFloat4x4();

	/*-------------------------------------------------------------------
	-			Copy Sky object data
	---------------------------------------------------------------------*/
	const auto cbMetaData = GPUBufferMetaData::ConstantBuffer(sizeof(Matrix4), 1, MemoryHeap::Upload, ResourceState::Common);
	_skyObject = device->CreateBuffer(cbMetaData);
	_skyObject->SetName(addName + L"CB");
	_skyObject->Pack(&skyData, commandList);
}

/****************************************************************************
*							PreparePipelineState
*************************************************************************//**
*  @fn        void Skybox::PreparePipelineState(const std::wstring& addName)
*  @brief     Prepare pipelineState
*  @param[in] std::wstring& addName
*  @return 　　void
*****************************************************************************/
void SkyDome::PreparePipelineState(const std::wstring& addName)
{
	const auto device = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();

	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	_resourceLayout = device->CreateResourceLayout
	(
		{
			ResourceLayoutElement(DescriptorHeapType::CBV, 0), // scene constants
			ResourceLayoutElement(DescriptorHeapType::CBV, 1), // sky object
			ResourceLayoutElement(DescriptorHeapType::SRV, 0)  // cubemap
		},
		{ SamplerLayoutElement(device->CreateSampler(SamplerInfo::GetDefaultSampler(SamplerLinearWrap)), 0) }
	);

	/*-------------------------------------------------------------------
	-             Set up graphic pipeline state
	---------------------------------------------------------------------*/
	const auto vs = factory->CreateShaderState();
	const auto ps = factory->CreateShaderState();
	vs->Compile(ShaderType::Vertex, L"Shader\\EnvironmentMap\\ShaderSkybox.hlsl", L"VSMain", 6.4f, { L"Shader\\Core" });
	ps->Compile(ShaderType::Pixel , L"Shader\\EnvironmentMap\\ShaderSkybox.hlsl", L"PSMain", 6.4f, { L"Shader\\Core" });

	_pipeline = device->CreateGraphicPipelineState(_engine->GetRenderPass(), _resourceLayout);
	_pipeline->SetBlendState        (factory->CreateSingleBlendState(BlendProperty::AlphaBlend()));
	_pipeline->SetRasterizerState   (factory->CreateRasterizerState());
	_pipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultVertexElement()));
	_pipeline->SetDepthStencilState (factory->CreateDepthStencilState());
	_pipeline->SetVertexShader(vs);
	_pipeline->SetPixelShader(ps);
	_pipeline->CompleteSetting();
	_pipeline->SetName(addName + L"PSO");
}

void SkyDome::PrepareResourceView(const std::shared_ptr<GPUTexture>& texture)
{
	const auto frameIndex  = _engine->GetCurrentFrameIndex();
	const auto device      = _engine->GetDevice();
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics, frameIndex);
	
	/*-------------------------------------------------------------------
	-           Prepare Resource View
	---------------------------------------------------------------------*/
	_resourceViews.resize(2);
	// sky object
	_resourceViews[0].first  = 1; // resource view array index (please see resource layout)
	_resourceViews[0].second = device->CreateResourceView(ResourceViewType::ConstantBuffer, _skyObject, nullptr);

	// cubemap
	_resourceViews[1].first  = 2;
	_resourceViews[1].second = device->CreateResourceView(ResourceViewType::Texture, texture, nullptr);
}
#pragma endregion Private Function