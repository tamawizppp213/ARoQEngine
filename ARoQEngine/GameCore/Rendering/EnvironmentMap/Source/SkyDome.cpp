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
using namespace engine;
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
SkyDome::SkyDome() {};
SkyDome::~SkyDome()
{
	_skyObject.Reset();
	_vertexBuffers.Clear(); _vertexBuffers.ShrinkToFit();
	_indexBuffers.Clear(); _indexBuffers.ShrinkToFit();
	_resourceViews.Clear(); _resourceViews.ShrinkToFit();
	_resourceLayout.Reset();
	_pipeline.Reset();
}
#pragma region Constructor and Destructor 
SkyDome::SkyDome(const LowLevelGraphicsEnginePtr& engine, const gu::tstring& cubeMapPath, const gu::tstring& addName)
	:_engine(engine)
{
	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	gu::tstring name = SP(""); if (addName != SP("")) { name += addName; name += SP("::"); }
	name += SP("Skybox::");

	/*-------------------------------------------------------------------
	-           Load Texture
	---------------------------------------------------------------------*/
	const auto device      = _engine->GetDevice();
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics);
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
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics);
	
	/*-------------------------------------------------------------------
	-               Execute commandlist
	---------------------------------------------------------------------*/
	commandList->SetResourceLayout(_resourceLayout);
	commandList->SetGraphicsPipeline(_pipeline);
	commandList->SetVertexBuffer(_vertexBuffers[currentFrame]);
	commandList->SetIndexBuffer (_indexBuffers[currentFrame]);
	for (size_t i = 0; i < _resourceViews.Size(); ++i) 
	{
		commandList->SetDescriptorHeap(_resourceViews[i].Value->GetHeap());
		_resourceViews[i].Value->Bind(commandList, _resourceViews[i].Key);
	}
	cameraResourceView->Bind(commandList, 0);
	commandList->DrawIndexedInstanced(static_cast<std::uint32_t>(_indexBuffers[currentFrame]->GetElementCount()), 1);
}


#pragma region Private Function
/****************************************************************************
*							PrepareVertexAndIndexBuffer
****************************************************************************/
/* @fn        void Skybox::PrepareVertexAndIndexBuffer(const gu::tstring& addName)
*  @brief     Prepare Sphere Vertex Buffer
*  @param[in] const gu::tstring& addName
*  @return 　　void
*****************************************************************************/
void SkyDome::PrepareVertexAndIndexBuffer(const gu::tstring& addName)
{
	const auto device      = _engine->GetDevice();
	const auto commandList = _engine->GetCommandList(CommandListType::Copy); 
	/*-------------------------------------------------------------------
	-            Create Sphere Mesh
	---------------------------------------------------------------------*/
	PrimitiveMesh sphereMesh = PrimitiveMeshGenerator::Sphere(0.5f, 20, 20, false);    // Sphere mesh
	/*-------------------------------------------------------------------
	-            Create Mesh Buffer
	---------------------------------------------------------------------*/
	const auto frameCount = LowLevelGraphicsEngine::FRAME_BUFFER_COUNT;
	// prepare frame count buffer
	_vertexBuffers.Resize(frameCount);
	_indexBuffers .Resize(frameCount);
	
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
		const auto vbMetaData = GPUBufferMetaData::VertexBuffer((gu::uint32)vertexByteSize, (gu::uint32)vertexCount, MemoryHeap::Upload);
		_vertexBuffers[i] = device->CreateBuffer(vbMetaData);
		_vertexBuffers[i]->SetName(addName + SP("VB"));
		_vertexBuffers[i]->UploadByte(sphereMesh.Vertices.data(), vbMetaData.GetTotalByte()); // Map

		/*-------------------------------------------------------------------
		-            Set Index Buffer
		---------------------------------------------------------------------*/
		const auto ibMetaData = GPUBufferMetaData::IndexBuffer((gu::uint32)indexByteSize, (gu::uint32)indexCount, MemoryHeap::Default, ResourceState::Common);
		_indexBuffers[i] = device->CreateBuffer(ibMetaData);
		_indexBuffers[i]->SetName(addName + SP("IB"));
		_indexBuffers[i]->UploadByte(sphereMesh.Indices.data(), ibMetaData.GetTotalByte(), 0, commandList);

	}
}
/****************************************************************************
*							PrepareSkyObject
****************************************************************************/
/* @fn        void Skybox::PrepareSkyObject(const gu::tstring& addName)
*  @brief     Build World Matrix Infomation
*  @param[in] gu::tstring& addName
*  @return 　　void
*****************************************************************************/
void SkyDome::PrepareSkyObject(const gu::tstring& addName)
{
	const auto device      = _engine->GetDevice();
	const auto commandList = _engine->GetCommandList(CommandListType::Copy);

	/*-------------------------------------------------------------------
	-			Set Skydata
	---------------------------------------------------------------------*/
	Matrix4f skyData; // sphere
	skyData = gm::Scaling(SKY_SCALE, SKY_SCALE, SKY_SCALE);

	/*-------------------------------------------------------------------
	-			Copy Sky object data
	---------------------------------------------------------------------*/
	const auto cbMetaData = GPUBufferMetaData::ConstantBuffer(sizeof(Matrix4f), 1, MemoryHeap::Upload, ResourceState::Common);
	_skyObject = device->CreateBuffer(cbMetaData);
	_skyObject->SetName(addName + SP("CB"));
	_skyObject->UploadByte(&skyData, cbMetaData.GetTotalByte(), 0, commandList);
}

/****************************************************************************
*							PreparePipelineState
****************************************************************************/
/* @fn        void Skybox::PreparePipelineState(const gu::tstring& addName)
*  @brief     Prepare pipelineState
*  @param[in] gu::tstring& addName
*  @return 　　void
*****************************************************************************/
void SkyDome::PreparePipelineState(const gu::tstring& addName)
{
	const auto device = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();

	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	const auto sampler = device->CreateSampler(SamplerInfo::GetDefaultSampler(LinearWrap));
	_resourceLayout = device->CreateResourceLayout
	(
		{
			ResourceLayoutElement(DescriptorHeapType::CBV, 0), // scene constants
			ResourceLayoutElement(DescriptorHeapType::CBV, 1), // sky object
			ResourceLayoutElement(DescriptorHeapType::SRV, 0)  // cubemap
		},
		{ 
			SamplerLayoutElement(sampler, 0) 
		}
	);

	/*-------------------------------------------------------------------
	-             Set up graphic pipeline state
	---------------------------------------------------------------------*/
	const auto vs = factory->CreateShaderState();
	const auto ps = factory->CreateShaderState();
	vs->Compile({ ShaderType::Vertex, SP("Shader\\EnvironmentMap\\ShaderSkybox.hlsl"), SP("VSMain"),  { SP("Shader\\Core") } });
	ps->Compile({ ShaderType::Pixel , SP("Shader\\EnvironmentMap\\ShaderSkybox.hlsl"), SP("PSMain"),  { SP("Shader\\Core") } });

	_pipeline = device->CreateGraphicPipelineState(_engine->GetRenderPass(), _resourceLayout);
	_pipeline->SetBlendState        (factory->CreateSingleBlendState(BlendProperty::AlphaBlend()));
	_pipeline->SetRasterizerState   (factory->CreateRasterizerState(RasterizerProperty::Solid()));
	_pipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultVertexElement()));
	_pipeline->SetDepthStencilState (factory->CreateDepthStencilState());
	_pipeline->SetVertexShader(vs);
	_pipeline->SetPixelShader(ps);
	_pipeline->CompleteSetting();
	_pipeline->SetName(addName + SP("PSO"));
}

void SkyDome::PrepareResourceView(const gu::SharedPointer<GPUTexture>& texture)
{
	const auto device      = _engine->GetDevice();
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics);
	
	/*-------------------------------------------------------------------
	-           Prepare Resource View
	---------------------------------------------------------------------*/
	_resourceViews.Resize(2);
	// sky object
	_resourceViews[0].Key = 1; // resource view array index (please see resource layout)
	_resourceViews[0].Value = device->CreateResourceView(ResourceViewType::ConstantBuffer, _skyObject,0,0, nullptr);

	// cubemap
	_resourceViews[1].Key  = 2;
	_resourceViews[1].Value = device->CreateResourceView(ResourceViewType::Texture, texture,0,0, nullptr);
}
#pragma endregion Private Function