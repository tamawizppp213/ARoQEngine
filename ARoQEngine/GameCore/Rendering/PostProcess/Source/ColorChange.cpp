//////////////////////////////////////////////////////////////////////////////////
//              @file   ColorChange.cpp
///             @brief  Color change post effect
///             @author Toide Yutaro
///             @date   2022_04_30
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/PostProcess/Include/ColorChange.hpp"
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
using namespace engine;
using namespace rhi;
using namespace rhi::core;

namespace
{
	gu::tstring s_ShaderFunctionName[ColorChangeType::CountOfType] = 
	{ 
		SP("None"),
		SP("PSMonochrome"),
		SP("PSSepia"),
		SP("PSGrayScale"),
		SP("PSBinary"),
		SP("PSInvert"),
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
	_pipeline.Reset();
	_resourceLayout.Reset();
	_resourceViews.Clear(); _resourceViews.ShrinkToFit();
	_vertexBuffers.Clear(); _vertexBuffers.ShrinkToFit();
}
ColorChange::ColorChange(const ColorChangeType type, const LowLevelGraphicsEnginePtr& engine, const gu::tstring& addName) 
	: IFullScreenEffector(engine), _colorType(type)
{
	Check(_colorType != ColorChangeType::None);

	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	const auto name = CombineDebugName(addName, SP("ColorChange"));

	/*-------------------------------------------------------------------
	-           Prepare Pipeline
	---------------------------------------------------------------------*/
	PrepareVertexAndIndexBuffer(name);
	PreparePipelineState(name);
}

#pragma endregion Constructor and Destructor
#pragma region Main Function
/****************************************************************************
*							OnResize
****************************************************************************/
/* @fn        void ColorChange::OnResize(int newWidth, int newHeight)
* 
*  @brief     OnResize
* 
*  @param[in] int newWidth
* 
*  @param[in] int newHeight
* 
*  @return    void
*****************************************************************************/
void ColorChange::OnResize(const gu::uint32 newWidth, const gu::uint32 newHeight)
{
	printf("width: %d, height: %d\n", newWidth, newHeight);
}
/****************************************************************************
*							Draw
****************************************************************************/
/* @fn        void ColorChange::Draw
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
	graphicsCommandList->SetIndexBuffer (_indexBuffer, core::PixelFormat::R16_UINT);
	_engine->GetFrameBuffer(frameIndex)->GetRenderTargetSRV()->Bind(graphicsCommandList, 0);
	graphicsCommandList->DrawIndexedInstanced(
		static_cast<gu::uint32>(_indexBuffer->GetElementCount()), 1);
}
#pragma endregion Main Function

#pragma region Protected Function

/****************************************************************************
*							PreparePipelineState
****************************************************************************/
/* @fn        void ColorChange::PreparePipelineState(ColorChangeType type)
* 
*  @brief     Prepare PipelineState
* 
*  @param[in] ColorChangeType type
* 
*  @param[in] const gu::tstring& addName
* 
*  @return @@void
*****************************************************************************/
void ColorChange::PreparePipelineState(const gu::tstring& addName)
{
	const auto device  = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();

	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	_resourceLayout = device->CreateResourceLayout
	(
		{ ResourceLayoutElement(DescriptorHeapType::SRV, 0)},
		{ SamplerLayoutElement(device->CreateSampler(SamplerInfo::GetDefaultSampler(LinearClamp)), 0) }
	);

	/*-------------------------------------------------------------------
	-			SetShader
	---------------------------------------------------------------------*/
	const auto vs = factory->CreateShaderState();
	const auto ps = factory->CreateShaderState();
	vs->Compile({ ShaderType::Vertex, SP("Shader\\PostProcess\\ShaderScreenPass.hlsl"), SP("MainVS"), { SP("Shader\\Core") } });
	ps->Compile({ ShaderType::Pixel , SP("Shader\\PostProcess\\ShaderColorChange.hlsl"), s_ShaderFunctionName[(int)_colorType], { SP("Shader\\Core") } });

	/*-------------------------------------------------------------------
	-			Build Graphics Pipeline State
	---------------------------------------------------------------------*/
	_pipeline = CreateDefaultFullScreenGraphicsPipelineState(_engine->GetDrawClearRenderPass(), _resourceLayout, vs, ps);
	_pipeline->CompleteSetting();
	_pipeline->SetName(addName + SP("PSO"));
}

#pragma endregion Protected Function