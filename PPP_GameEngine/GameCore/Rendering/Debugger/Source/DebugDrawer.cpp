//////////////////////////////////////////////////////////////////////////////////
//              @file   DebugDrawer.cpp
///             @brief  Wire frame drawing.
///             @author Toide Yutaro
///             @date   2023_02_07
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DebugDrawer.hpp"
#include "../../Model/Include/GameModel.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIResourceLayout.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;
using namespace gc::core;
using namespace gc::rendering;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
DebugDrawer::DebugDrawer(const LowLevelGraphicsEnginePtr& engine, const std::wstring& addName)
	: _engine(engine)
{
	/*-------------------------------------------------------------------
	-            Set name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"DebugDrawer::";

	PreparePipelineState(name);
}

DebugDrawer::~DebugDrawer()
{
	_gameModels.clear();
	_gameModels.shrink_to_fit();
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*                          Draw
*************************************************************************//**
*  @fn        void DebugDrawer::Draw(const GPUResourceViewPtr& scene)
*
*  @brief     Draw wireframe object
*
*  @param[in] const GPUResourceViewPtr& sceneConstantBuffer
*
*  @return 　　void
*****************************************************************************/
void DebugDrawer::Draw(const GPUResourceViewPtr& scene)
{
#ifdef _DEBUG
	const auto& currentFrame = _engine->GetCurrentFrameIndex();
	const auto commandList  = _engine->GetCommandList(CommandListType::Graphics);

	/*-------------------------------------------------------------------
	-                 Execute command list
	---------------------------------------------------------------------*/
	commandList->SetDescriptorHeap(scene->GetHeap());
	commandList->SetResourceLayout(_resourceLayout);
	commandList->SetGraphicsPipeline(_pipeline);
	scene->Bind(commandList, 0); // sceneConstants
	for (const auto gameModel : _gameModels)
	{
		gameModel->Draw(false);
	}
#endif
}

/****************************************************************************
*                          Add
*************************************************************************//**
*  @fn        void DebugDrawer::Add(const GameModelPtr& model)
*
*  @brief     Add model
*
*  @param[in] const GameModelPtr& model
*
*  @return 　　void
*****************************************************************************/
void DebugDrawer::Add(const GameModelPtr& model)
{
	_gameModels.emplace_back(model);
}

/****************************************************************************
*                          Clear
*************************************************************************//**
*  @fn        void DebugDrawer::Clear()
*
*  @brief     Clear registered model buffer
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void DebugDrawer::Clear()
{
	_gameModels.clear();
	_gameModels.shrink_to_fit();
}

void DebugDrawer::Clear(const GameModelPtr& model)
{
	std::erase(_gameModels, model);
}


#pragma endregion Main Function

#pragma region Set up
void DebugDrawer::PreparePipelineState(const std::wstring& name)
{
	const auto device = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();

	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	_resourceLayout = device->CreateResourceLayout
	(
		{
			ResourceLayoutElement(DescriptorHeapType::CBV, 0), // scene constant
			ResourceLayoutElement(DescriptorHeapType::CBV, 1), // object world position information
		}
	);

	/*-------------------------------------------------------------------
	-             Set up graphic pipeline state
	---------------------------------------------------------------------*/
	const auto vs = factory->CreateShaderState();
	const auto ps = factory->CreateShaderState();
	vs->Compile(ShaderType::Vertex, L"Shader\\Core\\ShaderDebug.hlsl", L"VSMain", 6.4f, { L"Shader\\Core" });
	ps->Compile(ShaderType::Pixel , L"Shader\\Core\\ShaderDebug.hlsl", L"PSMain", 6.4f, { L"Shader\\Core" });

	/*-------------------------------------------------------------------
	-             Setup blend state (all alpha blend)
	---------------------------------------------------------------------*/
	_pipeline = device->CreateGraphicPipelineState(_engine->GetRenderPass(), _resourceLayout);
	_pipeline->SetBlendState(factory->CreateSingleBlendState(BlendProperty::AlphaBlend()));
	_pipeline->SetRasterizerState(factory->CreateRasterizerState(RasterizerProperty::WireFrame()));
	_pipeline->SetDepthStencilState(factory->CreateDepthStencilState());
	_pipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultSkinVertexElement()));
	_pipeline->SetVertexShader(vs);
	_pipeline->SetPixelShader(ps);
	_pipeline->CompleteSetting();
	_pipeline->SetName(name + L"pso");

}

#pragma endregion Set up