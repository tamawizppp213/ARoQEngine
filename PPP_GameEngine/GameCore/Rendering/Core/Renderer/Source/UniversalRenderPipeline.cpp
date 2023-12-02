//////////////////////////////////////////////////////////////////////////////////
//              @file   UniversalRenderPipeline.cpp
///             @brief  Default rendering pipeline
///             @author Toide Yutaro
///             @date   2023_03_03
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/UniversalRenderPipeline.hpp"
#include "../../BasePass/Include/BasePassZPrepass.hpp"
#include "../../BasePass/Include/BasePassGBuffer.hpp"
#include "../../BasePass/Include/BasePassLightCulling.hpp"
#include "GameCore/Rendering/Effect/Include/SSAO.hpp"
#include "GameCore/Rendering/Light/Include/CascadeShadow.hpp"
#include "GameCore/Rendering/Model/Include/GameModel.hpp"
#include "GameCore/Rendering/UI/Public/Include/UIRenderer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc;
using namespace gc::basepass;
using namespace rhi::core;

namespace
{
	const size_t MAX_DIRECTIONAL_LIGHT = 4;
}
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 
URP::URP(const LowLevelGraphicsEnginePtr& engine, const gu::SharedPointer<GameTimer>& gameTimer) : IRenderPipeline(engine),
_gameTimer(gameTimer)
{
	_zPrepass = gu::MakeShared<ZPrepass>(_engine, Screen::GetScreenWidth(), Screen::GetScreenHeight(), L"URP");

	_gBuffer = gu::MakeShared<GBuffer>(engine, gc::rendering::GBufferDesc((std::uint64_t)GBuffer::BufferType::CountOf), L"URP");

	_ssao = gu::MakeShared<SSAO>(engine, _gBuffer->GetRenderedTextureView(1), _zPrepass->GetRenderedTextureView());
	
	const auto shadowDesc = gc::rendering::CascadeShadowDesc();
	_cascadeShadowMap = gu::MakeShared<rendering::CascadeShadow>(_engine, shadowDesc, L"URP");

	_uiRenderer = gu::MakeShared<ui::UIRenderer>(engine, L"URP", MAX_UI_COUNT);

	_directionalLights = gu::MakeShared<gc::rendering::SceneLightBuffer<gc::rendering::DirectionalLightData>>(_engine, MAX_DIRECTIONAL_LIGHT, false);

	PrepareModelPipeline();
}

URP::~URP()
{
	_forwardModels.clear();
	_forwardModels.shrink_to_fit();
}

#pragma endregion Constructor and Destructor

#pragma region Main Function
bool URP::Draw()
{
	/*-------------------------------------------------------------------
	-         Is there a scene object
	---------------------------------------------------------------------*/
	if (!_scene) { return false; }

	const auto commandList = _engine->GetCommandList(CommandListType::Graphics);
	if (!commandList->IsOpen()) { return false; }
	/*-------------------------------------------------------------------
	-         Preprocess
	---------------------------------------------------------------------*/
	_zPrepass->Draw(_scene);
	_gBuffer ->Draw(_scene);
	_ssao->Draw(_scene);
	//_cascadeShadowMap->Draw(_gameTimer, _directionalLights->GetLight(0).Direction);

	/*-------------------------------------------------------------------
	-         Rendering
	---------------------------------------------------------------------*/
	_engine->BeginSwapchainRenderPass();
	commandList->SetResourceLayout(_resourceLayout);
	commandList->SetGraphicsPipeline(_pipeline);
	_scene->Bind(commandList, 0);
	_directionalLights->BindLightData(commandList, 2);
	_cascadeShadowMap->GetShadowInfoView()->Bind(commandList, 3);
	for (const auto& model : _forwardModels)
	{
		// model active check
		if (!model->IsActive()) { continue; }

		// forward rendering with each materials
		model->Draw(true, 4);
	}
	
	return true;
}

void URP::Add(const URPDrawType type, const GameModelPtr& gameModel)
{
#ifdef _DEBUG
	assert(gameModel);
#endif

	_cascadeShadowMap->Add(gameModel);
	_zPrepass->Add(gameModel);
	_gBuffer ->Add(gameModel);

	if (type & URPDrawType::Differed)
	{
		OutputDebugStringA("NonImplement\n");
	}
	if (type & URPDrawType::Forward)
	{
		_forwardModels.push_back(gameModel);
	}
}
#pragma endregion Main Function

#pragma region SetUp
void URP::PrepareModelPipeline()
{
	const auto device  = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();

	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	_resourceLayout = device->CreateResourceLayout
	(
		{
			ResourceLayoutElement(DescriptorHeapType::CBV, 0), // Scene constant 
			ResourceLayoutElement(DescriptorHeapType::CBV, 1), // object world position information
			ResourceLayoutElement(DescriptorHeapType::CBV, 5), // directional light buffer
			ResourceLayoutElement(DescriptorHeapType::CBV, 10), // ShadowInfo
			ResourceLayoutElement(DescriptorHeapType::CBV, 2), // material constants
			ResourceLayoutElement(DescriptorHeapType::SRV, 0), // Diffuse map
			ResourceLayoutElement(DescriptorHeapType::SRV, 1), // Specular map
			ResourceLayoutElement(DescriptorHeapType::SRV, 2), // Normal map
		},
		{ SamplerLayoutElement(device->CreateSampler(SamplerInfo::GetDefaultSampler(SamplerLinearWrap)),0) }
	);

	/*-------------------------------------------------------------------
	-             Setup shader
	---------------------------------------------------------------------*/
	const auto vs = factory->CreateShaderState();
	const auto ps = factory->CreateShaderState();
	vs->Compile(ShaderType::Vertex, L"Shader\\Model\\ShaderURPForwardRendering.hlsl", L"VSMain", 6.4f, { L"Shader\\Core" });
	ps->Compile(ShaderType::Pixel, L"Shader\\Model\\ShaderURPForwardRendering.hlsl",  L"PSMain", 6.4f, { L"Shader\\Core" }, {L"USE_SPECULAR_F_NONE"});

	/*-------------------------------------------------------------------
	-             Set up graphic pipeline state
	---------------------------------------------------------------------*/
	auto blend = BlendProperty::AlphaBlend();

	_pipeline = device->CreateGraphicPipelineState(_engine->GetRenderPass(), _resourceLayout);
	_pipeline->SetBlendState(factory->CreateSingleBlendState(blend));
	_pipeline->SetRasterizerState(factory->CreateRasterizerState(RasterizerProperty::Solid(false, FrontFace::Clockwise, CullingMode::Back)));
	_pipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultSkinVertexElement()));
	DepthStencilProperty depthProp = {};
	depthProp.DepthOperator = CompareOperator::Less;
	_pipeline->SetDepthStencilState(factory->CreateDepthStencilState(depthProp));
	_pipeline->SetVertexShader(vs);
	_pipeline->SetPixelShader(ps);
	_pipeline->CompleteSetting();
	_pipeline->SetName(L"URP::PSO");
}
#pragma endregion SetUp