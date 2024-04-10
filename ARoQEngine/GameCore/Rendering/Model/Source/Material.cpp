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
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandList.hpp"
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
Material::Material(const LowLevelGraphicsEnginePtr& engine, const GPUBufferMetaData& bufferInfo, const gu::tstring& addName, 
	const RHIDescriptorHeapPtr& customHeap): _engine(engine), _customHeap(customHeap)
{
#ifdef _DEBUG
	Check(bufferInfo.Count == 1);
	Check(bufferInfo.ResourceUsage == ResourceUsage::ConstantBuffer);

#endif

	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	gu::tstring name = SP(""); if (addName != SP("")) { name += addName; name += SP("::"); }
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
		const auto commandList = _engine->GetCommandList(CommandListType::Graphics);
		ResourceCache = gu::MakeShared<GPUResourceCache>(device, commandList);
	}

	_textures[(int)UsageTexture::Diffuse]  = ResourceCache->Load(SP("Resources/Preset/NullAlbedoMap.png"));
	_textures[(int)UsageTexture::Normal]   = ResourceCache->Load(SP("Resources/Preset/NullNormalMap.DDS"));
	_textures[(int)UsageTexture::Specular] = ResourceCache->Load(SP("Resources/Preset/NullSpecMap.DDS"));

	InstanceCount++;
}

Material::~Material()
{
	for (auto& texture : _textures)
	{
		texture.Reset();
	}

	InstanceCount--;
	if (InstanceCount == 0)
	{
		ResourceCache.Reset();
	}
}
#pragma endregion Constructor and Destructor 

#pragma region Main Function
/****************************************************************************
*					Bind
*************************************************************************//**
*  @fn      void Material::Bind(const gu::SharedPointer<rhi::core::RHICommandList>& commandList,
			const std::uint32_t frameIndex,
			const std::uint32_t bindID,
			const gu::DynamicArray<std::uint32_t>& bindTextureIDs)
*
*  @brief     Draw material
*
*  @param[in] const std::shard_ptr<rhi::core::RHICommandList>& graphicsCommandList
*  @param[in] const std::uint32_t currentFrameIndex
*  @param[in] const std::uint32_t bind id : material constant buffer 
*  @param[in] const gu::DynamicArray<std::uint32_t>& texture srv ids 
*
*  @return 　　void
*****************************************************************************/
void Material::Bind(const gu::SharedPointer<rhi::core::RHICommandList>& commandList,
	const std::uint32_t frameIndex,
	const std::uint32_t bindID,
	const gu::DynamicArray<std::uint32_t>& bindTextureIDs)
{
#ifdef _DEBUG
	Check(commandList->GetType() == CommandListType::Graphics);
	Check(frameIndex < LowLevelGraphicsEngine::FRAME_BUFFER_COUNT);
	Check(bindTextureIDs.Size() == (size_t)UsageTexture::CountOf);
#endif

	_materialBufferView->Bind(commandList, bindID);

	for (size_t i = 0; i < (size_t)UsageTexture::CountOf; ++i)
	{
		_textures[i]->Bind(commandList, bindTextureIDs[i]);
	}
}
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

/****************************************************************************
*					LoadTexture
*************************************************************************//**
*  @fn        Material::GPUResourceViewPtr Material::LoadTexture(const gu::tstring& filePath, const UsageTexture textureType)
*
*  @brief     Load texture according to the usage texture.
*
*  @param[in] const gu::tstring& filePath
*  @param[in] const UsageTexture texture Type
*
*  @return 　　GPUResourceViewPtr (std::shard_ptr<GPUResourceView>)
*****************************************************************************/
Material::GPUResourceViewPtr Material::LoadTexture(const gu::tstring& filePath, const UsageTexture textureType)
{
	const auto resourceView = ResourceCache->Load(filePath);

	_textures[(int)textureType] = resourceView;

	return resourceView;
}
#pragma endregion Main Function

#pragma region Set up function
/****************************************************************************
*					SetUpBuffer
*************************************************************************//**
*  @fn        void Material::SetUpBuffer(const GPUBufferMetaData& bufferInfo, const gu::tstring& name)
*
*  @brief     Set up cbv buffer (for material)
*
*  @param[in] const GPUBufferMetaData& bufferInfo, 
*  @param[in] const gu::tstring& name
*
*  @return 　　void
*****************************************************************************/
void Material::SetUpBuffer(const GPUBufferMetaData& bufferInfo, const gu::tstring& name)
{
	if (gu::HasAnyFlags(bufferInfo.ResourceUsage, ResourceUsage::ConstantBuffer))
	{
		OutputDebugStringA("SetUpBuffer: Unavailable buffer type");
		return;
	}

	const auto device = _engine->GetDevice();

	/*-------------------------------------------------------------------
	-            Get material buffer
	---------------------------------------------------------------------*/
	const auto materialBuffer = device->CreateBuffer(bufferInfo);
	materialBuffer->SetName(name + SP("CB"));
	if (bufferInfo.InitData)
	{
		materialBuffer->Update(bufferInfo.InitData, 1);
	}

	_materialBufferView = device->CreateResourceView(
		ResourceViewType::ConstantBuffer, materialBuffer, _customHeap);
}
#pragma endregion Set up function