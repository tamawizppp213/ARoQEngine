//////////////////////////////////////////////////////////////////////////////////
//              @file   Material.cpp
///             @brief  Material Buffer Module
///             @author Copyright(c) Pocol. All right reserved.
///                     Partially edit by Toide Reference : DirectX12 実践ガイド 
///             @date   2022_06_02
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/Include/Material.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GameCore/Core/Include/ResourceManager.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIDescriptorHeap.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceCache.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::core;
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
std::uint64_t gc::core::Material::InstanceCount = 0;
gc::core::Material::GPUResourceCachePtr gc::core::Material::ResourceCache = nullptr;

#pragma region Constructor and Destructor 
Material::Material(const LowLevelGraphicsEnginePtr& engine, const GPUBufferMetaData& bufferInfo, const std::wstring& addName, 
	const RHIDescriptorHeapPtr& customHeap): _engine(engine), _customHeap(customHeap)
{
#ifdef _DEBUG
	assert(bufferInfo.Count == 1);
	assert(bufferInfo.ResourceUsage == ResourceUsage::ConstantBuffer);
	assert(bufferInfo.BufferType == BufferType::Constant);
#endif

	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"Material::";

	/*-------------------------------------------------------------------
	-            Set constant buffer (material buffer)
	---------------------------------------------------------------------*/
	SetUpBuffer(bufferInfo, name);

	/*-------------------------------------------------------------------
	-            Set Texture
	---------------------------------------------------------------------*/
	if (InstanceCount == 0)
	{
		const auto device      = _engine->GetDevice();
		const auto frameIndex  = _engine->GetCurrentFrameIndex();
		const auto commandList = _engine->GetCommandList(CommandListType::Graphics, frameIndex);
		ResourceCache = std::make_shared<GPUResourceCache>(device, commandList);
	}

	_textures[(int)UsageTexture::Diffuse]  = ResourceCache->Load(L"Resources/Preset/NullAlbedoMap.png");
	_textures[(int)UsageTexture::Normal]   = ResourceCache->Load(L"Resources/Preset/NullNormalMap.DDS");
	_textures[(int)UsageTexture::Specular] = ResourceCache->Load(L"Resources/Preset/NullSpecMap.DDS");

	InstanceCount++;
}

Material::~Material()
{
	for (auto& texture : _textures)
	{
		texture.reset();
	}

	InstanceCount--;
	if (InstanceCount == 0)
	{
		ResourceCache.reset();
	}
}
#pragma endregion Constructor and Destructor 

#pragma region Main Function
/****************************************************************************
*					PackMaterial
*************************************************************************//**
*  @fn        void Material::PackMaterial(const void* data)
*
*  @brief     Pack constant material buffer
*
*  @param[in] const void* data
*
*  @return 　　void
*****************************************************************************/
void Material::PackMaterial(const void* data)
{
	const auto buffer = _materialBufferView->GetBuffer();
	buffer->Update(data, 1);
}

#pragma endregion Main Function

#pragma region Set up function
/****************************************************************************
*					SetUpBuffer
*************************************************************************//**
*  @fn        void Material::SetUpBuffer(const GPUBufferMetaData& bufferInfo, const std::wstring& name)
*
*  @brief     Set up cbv buffer (for material)
*
*  @param[in] const GPUBufferMetaData& bufferInfo, 
*  @param[in] const std::wstring& name
*
*  @return 　　void
*****************************************************************************/
void Material::SetUpBuffer(const GPUBufferMetaData& bufferInfo, const std::wstring& name)
{
	if (bufferInfo.BufferType != BufferType::Constant) 
	{
		OutputDebugStringA("SetUpBuffer: Unavailable buffer type"); 
		return; 
	}

	const auto device = _engine->GetDevice();

	/*-------------------------------------------------------------------
	-            Get material buffer
	---------------------------------------------------------------------*/
	const auto materialBuffer = device->CreateBuffer(bufferInfo);
	materialBuffer->SetName(name + L"CB");

	_materialBufferView = device->CreateResourceView(
		ResourceViewType::ConstantBuffer, materialBuffer, _customHeap);
}
#pragma endregion Set up function