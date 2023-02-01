//////////////////////////////////////////////////////////////////////////////////
//              @file   BasePassGBuffer.cpp
///             @brief  GBuffers
///             @author Toide Yutaro
///             @date   2022_05_04
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/BasePassZPrepass.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIResourceLayout.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;
using namespace gc;
using namespace gc::basepass;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 
ZPrepass::ZPrepass(const LowLevelGraphicsEnginePtr& engine, const std::uint32_t width, const std::uint32_t height, const std::wstring& addName):
	_engine(engine), _width(width), _height(height)
{
	/*-------------------------------------------------------------------
	-            Set name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"ZPrepass::";

	PreparePipelineState(name);
}

ZPrepass::~ZPrepass()
{
	_gameActors.clear(); _gameActors.shrink_to_fit();
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
void ZPrepass::OnResize(const std::uint32_t width, const std::uint32_t height)
{

}

void ZPrepass::Draw(const GPUResourceViewPtr& scene)
{
	const auto currentFrame = _engine->GetCurrentFrameIndex();
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics, currentFrame);

	/*-------------------------------------------------------------------
	-                 Change render target
	---------------------------------------------------------------------*/

	/*-------------------------------------------------------------------
	-                 Execute command list
	---------------------------------------------------------------------*/
	commandList->SetResourceLayout(_resourceLayout);
	commandList->SetGraphicsPipeline(_pipeline);
	scene->Bind(commandList, 0); // scene constants
	for (const auto gameActor : _gameActors)
	{

	}

	/*-------------------------------------------------------------------
	-                 Return current render target
	---------------------------------------------------------------------*/
}

void ZPrepass::Add(const std::shared_ptr<GameActor>& actor)
{
	_gameActors.emplace_back(actor);
}

void ZPrepass::Clear()
{
	_gameActors.clear();
	_gameActors.shrink_to_fit();
}

void ZPrepass::Clear(const std::shared_ptr<GameActor>& actor)
{
	std::erase(_gameActors, actor);
}

#pragma endregion Main Function

#pragma region Setup Function
void ZPrepass::PreparePipelineState(const std::wstring& name)
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
		}
	);

	/*-------------------------------------------------------------------
	-             Set up graphic pipeline state
	---------------------------------------------------------------------*/
	const auto vs = factory->CreateShaderState();
	const auto ps = factory->CreateShaderState();
	vs->Compile(ShaderType::Vertex, L"Shader\\Lighting\\ShaderZPrepass.hlsl", L"VSMain", 6.4f, { L"Shader\\Core"});
	ps->Compile(ShaderType::Pixel , L"Shader\\Lighting\\ShaderZPrepass.hlsl", L"PSMain", 6.4f, { L"Shader\\Core" });

	/*-------------------------------------------------------------------
	-             Set up graphic pipeline state
	---------------------------------------------------------------------*/
	_pipeline = device->CreateGraphicPipelineState(_engine->GetRenderPass(), _resourceLayout);
	_pipeline->SetRasterizerState   (factory->CreateRasterizerState());
	_pipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultVertexElement()));
	_pipeline->SetVertexShader(vs);
	_pipeline->SetPixelShader(ps);
	_pipeline->CompleteSetting();
	_pipeline->SetName(name + L"PSO");
}
#pragma endregion Setup Function
