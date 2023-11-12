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
using namespace gc::core;

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
ColorChange::ColorChange(const ColorChangeType type, const LowLevelGraphicsEnginePtr& engine, const std::wstring& addName) 
	: IFullScreenEffector(engine), _colorType(type)
{
	assert(_colorType != ColorChangeType::None);

	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	const auto name = DefineDebugName(addName);

	/*-------------------------------------------------------------------
	-           Prepare Pipeline
	---------------------------------------------------------------------*/
	PrepareVertexAndIndexBuffer(name);
	PreparePipelineState(name);
	PrepareResourceView();
}

#pragma endregion Constructor and Destructor
#pragma region Main Function
/****************************************************************************
*							OnResize
*************************************************************************//**
*  @fn        void ColorChange::OnResize(int newWidth, int newHeight)
* 
*  @brief     OnResize
* 
*  @param[in] int newWidth
* 
*  @param[in] int newHeight
* 
*  @return    void
*****************************************************************************/
void ColorChange::OnResize(int newWidth, int newHeight)
{
	printf("width: %d, height: %d\n", newWidth, newHeight);
}
/****************************************************************************
*							Draw
*************************************************************************//**
*  @fn        void ColorChange::Draw
* 
*  @brief     Render to Back Buffer
* 
*  @param[in] void
* 
*  @return    void
*****************************************************************************/
void ColorChange::Draw()
{
	const auto frameIndex          = _engine->GetCurrentFrameIndex();
	const auto device              = _engine->GetDevice();
	const auto graphicsCommandList = _engine->GetCommandList(CommandListType::Graphics);

	/*-------------------------------------------------------------------
	-               Execute commandlist
	---------------------------------------------------------------------*/
	graphicsCommandList->SetResourceLayout(_resourceLayout);
	graphicsCommandList->SetGraphicsPipeline(_pipeline);
	graphicsCommandList->SetVertexBuffer(_vertexBuffers[frameIndex]);
	graphicsCommandList->SetIndexBuffer (_indexBuffers[frameIndex]);
	_engine->GetFrameBuffer(frameIndex)->GetRenderTargetSRV()->Bind(graphicsCommandList, 0);
	graphicsCommandList->DrawIndexedInstanced(
		static_cast<std::uint32_t>(_indexBuffers[frameIndex]->GetElementCount()), 1);
}
#pragma endregion Main Function

#pragma region Protected Function

/****************************************************************************
*							PreparePipelineState
*************************************************************************//**
*  @fn        void ColorChange::PreparePipelineState(ColorChangeType type)
* 
*  @brief     Prepare PipelineState
* 
*  @param[in] ColorChangeType type
* 
*  @param[in] const std::wstring& addName
* 
*  @return 　　void
*****************************************************************************/
void ColorChange::PreparePipelineState(const std::wstring& addName)
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
	ps->Compile(ShaderType::Pixel , L"Shader\\Effect\\ShaderColorChange.hlsl", s_ShaderFunctionName[(int)_colorType], 6.4f, { L"Shader\\Core" });

	/*-------------------------------------------------------------------
	-			Build Graphics Pipeline State
	---------------------------------------------------------------------*/
	_pipeline = device->CreateGraphicPipelineState(_engine->GetRenderPass(), _resourceLayout);
	_pipeline->SetBlendState        (factory->CreateSingleBlendState(BlendProperty::OverWrite()));
	_pipeline->SetRasterizerState   (factory->CreateRasterizerState(RasterizerProperty::Solid()));
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
	
}
#pragma endregion Protected Function