//////////////////////////////////////////////////////////////////////////////////
//              @file   BasePassGBuffer.cpp
///             @brief  GBuffers
///             @author Toide Yutaro
///             @date   2022_05_04
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/BasePassLightCulling.hpp"
#include "../Include/BasePassZPrepass.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUTexture.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"
#include <string>
#include <cassert>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;
using namespace gc::basepass;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
LightCulling::LightCulling(const LowLevelGraphicsEnginePtr& engine, const LightCullingDesc& desc)
	: _engine(engine), _zprepass(desc.ZPrepass)
{
#ifdef _DEBUG
	assert(_engine);
	assert(_zprepass);
	for (int i = 0; i < _countof(desc.LightCounts); ++i)
	{
		assert(desc.LightCounts[i] > 0);
	}
#endif


}

LightCulling::~LightCulling()
{
	_lightIDLists.Clear();
	_lightIDLists.ShrinkToFit();
}

#pragma endregion Constructor and Destructor

#pragma region Main Function 
/****************************************************************************
*                        Execute
*************************************************************************//**
*  @fn        void LightCulling::Execute(const ResourceViewPtr& scene, const ResourceViewPtr& light)
*
*  @brief     Execute light culling
*
*  @param[in] const ResourceViewPtr& camera view constant buffer view
*  @param[in] const ResourceViewPtr& scene light constant buffer view
*
*  @return 　　void
*****************************************************************************/
void LightCulling::Execute(const ResourceViewPtr& scene, const ResourceViewPtr& light)
{
#ifdef _DEBUG
	if (scene) { OutputDebugStringA("Please set scene"); return; }
	if (light) { OutputDebugStringA("Please set light"); return; }
#endif

	const auto computeList = _engine->GetCommandList(CommandListType::Compute);

	/*-------------------------------------------------------------------
	-           Execute command list
	---------------------------------------------------------------------*/
	computeList->SetDescriptorHeap(scene->GetHeap());
	computeList->SetResourceLayout(_resourceLayout);
	computeList->SetComputePipeline(_pipeline);

	// bind resource 
	scene->Bind(computeList, 0);
	light->Bind(computeList, 1);
	_zprepass->GetRenderedTextureView()->Bind(computeList, 2);
	for (int i = 0; i < CullingLightType::CountOf; ++i)
	{
		_lightIDLists[i]->Bind(computeList, i + 3); // 3 is resource layout offset
	}

	computeList->Dispatch(static_cast<std::uint32_t>(_width / TILE_WIDTH), static_cast<std::uint32_t>(_height / TILE_HEIGHT), 1);
}

#pragma endregion Main Function

#pragma region Set Up Function
/****************************************************************************
*                        PrepareBuffer
*************************************************************************//**
*  @fn        void LightCulling::PrepareBuffer(const LightCullingDesc& desc)
*
*  @brief     Prepare read and writable buffers (each light id buffer)
*
*  @param[in] const LightCullingDesc& desc
*
*  @return 　　void
*****************************************************************************/
void LightCulling::PrepareBuffer(const LightCullingDesc& desc)
{
	const auto device        = _engine->GetDevice();
	const auto textureWidth  = _zprepass->GetRenderedTexture()->GetWidth();
	const auto textureHeight = _zprepass->GetRenderedTexture()->GetHeight();
	const auto tileCount     = (textureWidth / TILE_WIDTH) * (textureHeight * TILE_HEIGHT);

	/*-------------------------------------------------------------------
	-			Set up 
	---------------------------------------------------------------------*/
	_lightIDLists.Resize(CullingLightType::CountOf);
	for (size_t i = 0; i < _lightIDLists.Size(); ++i)
	{
		auto bufferInfo  = GPUBufferMetaData::UploadBuffer(sizeof(int), static_cast<gu::uint32>(desc.LightCounts[i] * tileCount));
		bufferInfo.Usage = BufferCreateFlags::UnorderedAccess;

		const auto bufferName = L"LightCulling::LightIDLists::" + std::to_wstring(i);
		const auto buffer = device->CreateBuffer(bufferInfo, gu::tstring(bufferName.c_str(), bufferName.size()));
		_lightIDLists[i] = device->CreateResourceView(ResourceViewType::ConstantBuffer, buffer);
	}

	_width  = textureWidth;
	_height = textureHeight;
}

/****************************************************************************
*                        PreparePipelineState
*************************************************************************//**
*  @fn        void LightCulling::PreparePipelineState()
*
*  @brief     Prepare compute pipeline state and resource layout
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void LightCulling::PreparePipelineState()
{
	const auto defaultPath = L"Shader\\Lighting\\ShaderTileBasedLightCulling.hlsl";
	const auto device      = _engine->GetDevice();
	const auto factory     = device->CreatePipelineFactory();

	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	_resourceLayout = device->CreateResourceLayout
	(
		{
			ResourceLayoutElement(DescriptorHeapType::CBV, 0),  // scece constant
			ResourceLayoutElement(DescriptorHeapType::CBV, 3),  // light constant
			ResourceLayoutElement(DescriptorHeapType::SRV, 0),  // zprepass texture
			ResourceLayoutElement(DescriptorHeapType::UAV, 0),  // point light list
			ResourceLayoutElement(DescriptorHeapType::UAV, 1)   // spot light list 
			                                                    // If you needs the other light list, you should add this layout.
		},
		{
			SamplerLayoutElement(device->CreateSampler(SamplerInfo::GetDefaultSampler(SamplerLinearClamp)), 0)
		}
	);

	/*-------------------------------------------------------------------
	-             Set shader
	---------------------------------------------------------------------*/
	const auto cs = factory->CreateShaderState();
	cs->Compile(ShaderType::Compute, defaultPath, L"LightCulling", 6.4f, { L"Shader\\Core" });

	/*-------------------------------------------------------------------
	-             Build compute pipeline state
	---------------------------------------------------------------------*/
	_pipeline = device->CreateComputePipelineState(_resourceLayout);
	_pipeline->SetComputeShader(cs);
	_pipeline->CompleteSetting();
	_pipeline->SetName(L"LightCulling::PSO");
}
#pragma endregion Set Up Function