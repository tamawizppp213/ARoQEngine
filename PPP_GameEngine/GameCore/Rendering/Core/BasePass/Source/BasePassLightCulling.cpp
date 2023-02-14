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
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUTexture.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"
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
	_lightIDLists.clear();
	_lightIDLists.shrink_to_fit();
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

	/*-------------------------------------------------------------------
	-           Set heap property
	---------------------------------------------------------------------*/
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
	_lightIDLists.resize(CullingLightType::CountOf);
	for (size_t i = 0; i < _lightIDLists.size(); ++i)
	{
		auto bufferInfo          = GPUBufferMetaData::UploadBuffer(sizeof(int), desc.LightCounts[i] * tileCount);
		bufferInfo.ResourceUsage = ResourceUsage::UnorderedAccess;

		_lightIDLists[i] = device->CreateBuffer(bufferInfo, L"LightCulling::LightIDLists::" + std::to_wstring(i));
	}
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
	const auto defaultPath = L"Shader\\Lighting\\ShaderLightCulling.hlsl";
	const auto device      = _engine->GetDevice();
	const auto factory     = device->CreatePipelineFactory();

	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	_resourceLayout = device->CreateResourceLayout
	(
		{
			ResourceLayoutElement(DescriptorHeapType::CBV, 0),
			ResourceLayoutElement(DescriptorHeapType::CBV, 1),
			ResourceLayoutElement(DescriptorHeapType::CBV, 3),
			ResourceLayoutElement(DescriptorHeapType::SRV, 0),
			ResourceLayoutElement(DescriptorHeapType::UAV, 0),
			ResourceLayoutElement(DescriptorHeapType::UAV, 1)
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