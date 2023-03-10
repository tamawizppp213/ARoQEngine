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

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 
URP::URP(const LowLevelGraphicsEnginePtr& engine) : IRenderPipeline(engine)
{
	_zPrepass = std::make_shared<ZPrepass>(_engine, Screen::GetScreenWidth(), Screen::GetScreenHeight(), L"URP");

	_gBuffer = std::make_shared<GBuffer>(engine, gc::rendering::GBufferDesc((std::uint64_t)GBuffer::BufferType::CountOf), L"URP");

	_uiRenderer = std::make_shared<ui::UIRenderer>(engine, L"URP", MAX_UI_COUNT);

	PrepareModelPipeline();
}

URP::~URP()
{
	_forwardModels.clear();
	_forwardModels.shrink_to_fit();
}

#pragma endregion Constructor and Destructor

#pragma region Main Function
bool URP::Draw(const ResourceViewPtr& scene)
{
	/*-------------------------------------------------------------------
	-         Is there a scene object
	---------------------------------------------------------------------*/
	if (!scene) { return false; }

	const auto commandList = _engine->GetCommandList(CommandListType::Graphics);

	/*-------------------------------------------------------------------
	-         Preprocess
	---------------------------------------------------------------------*/
	_zPrepass->Draw(scene);
	_gBuffer ->Draw(scene);

	/*-------------------------------------------------------------------
	-         Rendering
	---------------------------------------------------------------------*/
	_engine->BeginSwapchainRenderPass();
	commandList->SetResourceLayout(_resourceLayout);
	commandList->SetGraphicsPipeline(_pipeline);
	for (const auto& model : _forwardModels)
	{
		// model active check
		if (!model->IsActive()) { continue; }

		// forward rendering with each materials
		model->Draw(true);
	}
	return true;
}

void URP::Add(const URPDrawType type, const GameModelPtr& gameModel)
{
#ifdef _DEBUG
	assert(gameModel);
#endif

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
	ps->Compile(ShaderType::Pixel, L"Shader\\Model\\ShaderURPForwardRendering.hlsl",  L"PSMain", 6.4f, { L"Shader\\Core" });

	/*-------------------------------------------------------------------
	-             Set up graphic pipeline state
	---------------------------------------------------------------------*/
	_pipeline = device->CreateGraphicPipelineState(_engine->GetRenderPass(), _resourceLayout);
	_pipeline->SetBlendState(factory->CreateBlendState());
	_pipeline->SetRasterizerState(factory->CreateRasterizerState(RasterizerProperty::Solid()));
	_pipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultSkinVertexElement()));
	_pipeline->SetDepthStencilState(factory->CreateDepthStencilState());
	_pipeline->SetVertexShader(vs);
	_pipeline->SetPixelShader(ps);
	_pipeline->CompleteSetting();
	_pipeline->SetName(L"URP::PSO");
}
#pragma endregion SetUp