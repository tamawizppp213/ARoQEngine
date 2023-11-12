//////////////////////////////////////////////////////////////////////////////////
///             @file   SSAO.hpp
///             @brief  Screen space ambient occlusion
///             @author Toide Yutaro
///             @date   2023_04_02
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/SSAO.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUTexture.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"
#include "GameCore/Rendering/Model/Include/PrimitiveMesh.hpp"
#include <iostream>
#include "GameUtility/Math/Include/GMDistribution.hpp"
#include "GameUtility/Math/Include/GMColor.hpp"
#include "GameCore/Rendering/Effect/Include/Blur.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;
using namespace gc;
using namespace gm;

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{
	float GaussianDistribution(float r, float sigma)
	{
		return expf(-0.5f * (float)(r * r) / (sigma * sigma));
	}
}

#pragma region Constructor and Destructor
SSAO::SSAO(const LowLevelGraphicsEnginePtr& engine, const ResourceViewPtr& normalMap, const ResourceViewPtr& depthMap, const std::wstring& addName)
	: _engine(engine), _normalMap(normalMap), _depthMap(depthMap)
{
#ifdef _DEBUG
	assert(_engine);
	assert(_normalMap);
	assert(_depthMap);
#endif
	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"SSAO::";

	/*-------------------------------------------------------------------
	-            Prepare resources
	---------------------------------------------------------------------*/
	PrepareSSAOSettings(name);
	PrepareBlurMode(name);
	PrepareRandomTexture(name);
	PrepareVertexAndIndexBuffer(name);
	PreparePipelineState(name);

	_gaussianBlur = std::make_shared<gc::GaussianBlur>(_engine, Screen::GetScreenWidth(), Screen::GetScreenHeight(), false);
}

SSAO::~SSAO()
{
	
}

#pragma endregion Constructor and Destructor 

#pragma region Main Function
void SSAO::OnResize(const std::uint32_t newWidth, const std::uint32_t newHeight)
{
	printf("width: %d, height: %d\n", newWidth, newHeight);
}

void SSAO::Draw(const ResourceViewPtr& scene)
{
	/*-------------------------------------------------------------------
	-            Update ssao settings
	---------------------------------------------------------------------*/
	if (_isUpdated)
	{
		_settingView->GetBuffer()->Update(&_setting, 1);
		_isUpdated = false;
	}

	/*-------------------------------------------------------------------
	-            Set up
	---------------------------------------------------------------------*/
	const auto device      = _engine->GetDevice();
	const auto frameIndex  = _engine->GetCurrentFrameIndex();
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics);
	const auto frameBuffer = _engine->GetFrameBuffer(frameIndex);

	/*-------------------------------------------------------------------
	-            Execute SSAO
	---------------------------------------------------------------------*/
	commandList->BeginRenderPass(_engine->GetRenderPass(), frameBuffer);
	commandList->SetDescriptorHeap(_settingView->GetHeap());
	commandList->SetResourceLayout(_ssaoResourceLayout);
	commandList->SetGraphicsPipeline(_ssaoPipeline);
	commandList->SetVertexBuffer(_vertexBuffers[frameIndex]);
	commandList->SetIndexBuffer(_indexBuffers[frameIndex]);
	scene                ->Bind(commandList, 0);
	_settingView         ->Bind(commandList, 1);
	_blurVerticalModeView->Bind(commandList, 2);
	_normalMap->Bind(commandList, 3);
	_depthMap ->Bind(commandList, 4);
	_randomMap->Bind(commandList, 5);
	
	// draw fullscreen quad
	commandList->DrawIndexedInstanced(static_cast<std::uint32_t>(_indexBuffers[frameIndex]->GetElementCount()), 1);
	commandList->EndRenderPass();
	
	// blur
	_gaussianBlur->DrawPS(frameBuffer);
}

#pragma endregion Main Function

#pragma region Property Function
void SSAO::SetOcclusionRadius(const float radius)
{
	_setting.Radius = radius;
	_isUpdated      = true;
}

void SSAO::SetSharpness(const float sharpness)
{
	_setting.Sharpness = sharpness;
	_isUpdated         = true;
}

void SSAO::SetFadeStart(const float startDepthInViewSpace)
{
	if (_setting.FadeEnd < startDepthInViewSpace) { OutputDebugStringA("FadeEnd value is smaller than startDepthInViewSpace"); return; }
	_setting.FadeStart = startDepthInViewSpace;
	_isUpdated = true;
}

void SSAO::SetFadeEnd(const float endDepthInViewSpace)
{
	if (_setting.FadeStart > endDepthInViewSpace) { OutputDebugStringA("FadeStart value is bigger than endDepthInViewSpace"); }
	_setting.FadeEnd = endDepthInViewSpace;
	_isUpdated = true;
}

void SSAO::SetSurfaceEpsilon(const float epsilonDepth)
{
	_setting.SurfaceEpsilon = epsilonDepth;
	_isUpdated = true;
}
#pragma region Property Function

#pragma region SetUpFunction 
/****************************************************************************
*							PrepareSSAOSettings
*************************************************************************//**
*  @fn        void SSAO::PrepareSSAOSettings(const std::wstring& name)
* 
*  @brief     Prepare SSAO setting buffer and constant buffer view.
* 
*  @param[in] const std::wstring& name
* 
*  @return 　　void
*****************************************************************************/
void SSAO::PrepareSSAOSettings(const std::wstring& name)
{
	const auto device = _engine->GetDevice();

	// Start with 14 uniformly distributed vectors. 
	// We choose the 8 corners of the cube and the 6 center points along each cube face. 
	// We always alternate the points on opposites sides of the cubes. 
	// This way we still get the vectors spread out aeven if we choose to use less than 14 samples.
	_setting.Offsets[0]  = Float4(+1.0f, +1.0f, +1.0f, 0.0f);
	_setting.Offsets[1]  = Float4(-1.0f, -1.0f, -1.0f, 0.0f);
	_setting.Offsets[2]  = Float4(-1.0f, +1.0f, +1.0f, 0.0f);
	_setting.Offsets[3]  = Float4(+1.0f, -1.0f, -1.0f, 0.0f);
	_setting.Offsets[4]  = Float4(+1.0f, +1.0f, -1.0f, 0.0f);
	_setting.Offsets[5]  = Float4(-1.0f, -1.0f, +1.0f, 0.0f);
	_setting.Offsets[6]  = Float4(-1.0f, +1.0f, -1.0f, 0.0f);
	_setting.Offsets[7]  = Float4(+1.0f, -1.0f, +1.0f, 0.0f);
	_setting.Offsets[8]  = Float4(-1.0f, +0.0f, +0.0f, 0.0f);
	_setting.Offsets[9]  = Float4(+1.0f, +0.0f, +0.0f, 0.0f);
	_setting.Offsets[10] = Float4(+0.0f, -1.0f, +0.0f, 0.0f);
	_setting.Offsets[11] = Float4(+0.0f, +1.0f, +0.0f, 0.0f);
	_setting.Offsets[12] = Float4(+0.0f, +0.0f, -1.0f, 0.0f);
	_setting.Offsets[13] = Float4(+0.0f, +0.0f, +1.0f, 0.0f);

	// Create random length in [0.25f, 1.0f]
	Random<float> random;
	random.SetRange(0.25f, 1.0f);

	// set diff
	for (int i = 0; i < _countof(_setting.Offsets); ++i)
	{
		const auto randomValue = random.GetRandomValue();
		Vector4    offset      = randomValue * Normalize(_setting.Offsets[i]);
		_setting.Offsets[i]    = offset.ToFloat4();
	}

	// create constant buffer for ssao setting.
	const auto metaData = GPUBufferMetaData::ConstantBuffer(sizeof(SSAOSetting), 1, MemoryHeap::Upload, ResourceState::Common);
	const auto buffer   = device->CreateBuffer(metaData);
	buffer->SetName(name + L"SSAOSetting");
	buffer->Pack(&_setting, nullptr);

	// create constant buffer view.
	_settingView = device->CreateResourceView(ResourceViewType::ConstantBuffer, buffer, nullptr);
}

/****************************************************************************
*							PrepareBlurMode
*************************************************************************//**
*  @fn        void SSAO::PrepareBlurMode(const std::wstring& name)
*
*  @brief     Prepare blur mode buffer and constant buffer view.
*
*  @param[in] const std::wstring& name
*
*  @return 　　void
*****************************************************************************/
void SSAO::PrepareBlurMode(const std::wstring& name)
{
	const auto device = _engine->GetDevice();

	// vertical blur view
	{
		// set default blur mode.
		BlurMode blurMode = {};

		// create constant buffer for blur mode setting
		const auto metaData = GPUBufferMetaData::ConstantBuffer(sizeof(BlurMode), 1, MemoryHeap::Upload, ResourceState::Common);
		const auto buffer = device->CreateBuffer(metaData);
		buffer->SetName(name + L"BlurMode");
		buffer->Pack(&blurMode, nullptr);

		// create constant buffer view
		_blurVerticalModeView = device->CreateResourceView(ResourceViewType::ConstantBuffer, buffer, nullptr);
	}

	//horizontal blur view
	{
		// set default blur mode.
		BlurMode blurMode = {};

		// create constant buffer for blur mode setting
		const auto metaData = GPUBufferMetaData::ConstantBuffer(sizeof(BlurMode), 1, MemoryHeap::Upload, ResourceState::Common);
		const auto buffer = device->CreateBuffer(metaData);
		buffer->SetName(name + L"BlurMode");
		buffer->Pack(&blurMode, nullptr);

		// create constant buffer view
		_blurHorizontalModeView = device->CreateResourceView(ResourceViewType::ConstantBuffer, buffer, nullptr);
	}
}

void SSAO::PrepareRandomTexture(const std::wstring& name)
{
	const auto device   = _engine->GetDevice();
	const auto metaData = GPUTextureMetaData::Texture2D(256, 256, PixelFormat::R8G8B8A8_UNORM);
	const auto texture  = device->CreateTexture(metaData, name + L"Random");
	const auto pixel    = new RGBA[256 * 256];
	Random<float> random;
	random.SetRange(0.0f, 1.0f);
	for (int i = 0; i < 256 * 256; ++i)
	{
		pixel[i] = RGBA(random.GetRandomValue(), random.GetRandomValue(), random.GetRandomValue(), 0.0f);
	}

	texture->Write(_engine->GetCommandList(CommandListType::Graphics), pixel);

	_randomMap = device->CreateResourceView(ResourceViewType::Texture, texture, nullptr);

	delete[] pixel;

}
void SSAO::PreparePipelineState(const std::wstring& name)
{
	std::wstring defaultPath = L"Shader\\Lighting\\ShaderSSAO.hlsl";
	const auto device  = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();

	/*-------------------------------------------------------------------
	-             Setup default sampler
	---------------------------------------------------------------------*/
	auto depthSampler   = SamplerInfo::GetDefaultSampler(SamplerLinearBorder);
	depthSampler.Border = BorderColor::OpaqueWhite;

	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	_ssaoResourceLayout = device->CreateResourceLayout
	(
		{
			ResourceLayoutElement(DescriptorHeapType::CBV, 0), // scene
			ResourceLayoutElement(DescriptorHeapType::CBV, 3), // ssao setting
			ResourceLayoutElement(DescriptorHeapType::CBV, 4), // blur mode
			ResourceLayoutElement(DescriptorHeapType::SRV, 0), // normal texture
			ResourceLayoutElement(DescriptorHeapType::SRV, 1), // depth  texture
			ResourceLayoutElement(DescriptorHeapType::SRV, 2), // random texture
			ResourceLayoutElement(DescriptorHeapType::SRV, 3), // input  texture
		},
		{
			SamplerLayoutElement(device->CreateSampler(depthSampler), 2),
			SamplerLayoutElement(device->CreateSampler(SamplerInfo::GetDefaultSampler(SamplerPointClamp)), 1),
			SamplerLayoutElement(device->CreateSampler(SamplerInfo::GetDefaultSampler(SamplerPointClamp)), 0)
		}
	);

	/*-------------------------------------------------------------------
	-			Load Blob data
	---------------------------------------------------------------------*/
	const auto vs = factory->CreateShaderState();
	const auto mainPS = factory->CreateShaderState();
	const auto blurPS = factory->CreateShaderState();

	vs->Compile(ShaderType::Vertex   , defaultPath, L"VSMain"     , 6.4f, { L"Shader\\Core" });
	mainPS->Compile(ShaderType::Pixel, defaultPath, L"ExecuteSSAO", 6.4f, { L"Shader\\Core" });
	blurPS->Compile(ShaderType::Pixel, defaultPath, L"ExecuteSSAOBlur", 6.4f, { L"Shader\\Core" });

	/*-------------------------------------------------------------------
	-			Build Graphics pipeline state
	---------------------------------------------------------------------*/
	_ssaoPipeline = device->CreateGraphicPipelineState(_engine->GetRenderPass(), _ssaoResourceLayout);
	_ssaoPipeline->SetBlendState     (factory->CreateSingleBlendState(BlendProperty::OverWrite()));
	_ssaoPipeline->SetRasterizerState(factory->CreateRasterizerState(RasterizerProperty::Solid(false, FrontFace::Clockwise, CullingMode::Back)));
	_ssaoPipeline->SetInputAssemblyState(factory->CreateInputAssemblyState({ GPUInputAssemblyState::GetDefaultVertexElement() }));
	_ssaoPipeline->SetVertexShader(vs);
	_ssaoPipeline->SetPixelShader (mainPS);
	_ssaoPipeline->CompleteSetting();
	_ssaoPipeline->SetName(name + L"mainPSO");

	_blurPipeline = device->CreateGraphicPipelineState(_engine->GetRenderPass(), _ssaoResourceLayout);
	_blurPipeline->SetBlendState(factory->CreateSingleBlendState(BlendProperty::OverWrite()));
	_blurPipeline->SetRasterizerState(factory->CreateRasterizerState(RasterizerProperty::Solid(false, FrontFace::Clockwise, CullingMode::Back)));
	_blurPipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultVertexElement()));
	_blurPipeline->SetVertexShader(vs);
	_blurPipeline->SetPixelShader(blurPS);
	_blurPipeline->CompleteSetting();
	_blurPipeline->SetName(name + L"blurPSO");
}

/****************************************************************************
*							PrepareVertexAndIndexBuffer
*************************************************************************//**
*  @fn        void IFullScreenEffector::PrepareVertexAndIndexBuffer()
*  @brief     Prepare Rect Vertex and Index Buffer
*  @param[in] const std::wstring& addName
*  @return 　　void
*****************************************************************************/
void SSAO::PrepareVertexAndIndexBuffer(const std::wstring& addName)
{
	const auto device      = _engine->GetDevice();
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
#pragma endregion SetUp Function