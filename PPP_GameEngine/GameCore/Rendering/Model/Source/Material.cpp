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
#include "GraphicsCore/Engine/Include/GraphicsCoreEngine.hpp"
#include "GameCore/Core/Include/ResourceManager.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
MaterialBuffer::MaterialBuffer()
{

}
MaterialBuffer::~MaterialBuffer()
{

}

/****************************************************************************
*                       Initialize
*************************************************************************//**
*  @fn        bool MaterialBuffer::Initialize(size_t materialByteSize, size_t materialCount, const std::wstring& addName)
*  @brief     Set Initialize Buffer
*  @param[in] size_t materialByteSize
*  @param[in] size_t materialCount
*  @param[in] const std::wstring& addName
*  @return 　　bool
*****************************************************************************/
bool MaterialBuffer::Initialize(size_t materialByteSize, size_t materialCount, const std::wstring& addName)
{
	Finalize();

	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"Material::";
	/*-------------------------------------------------------------------
	-            Set Constant Buffer
	---------------------------------------------------------------------*/
	_constantBuffer = std::make_unique<UploadBuffer>(GraphicsCoreEngine::Instance().GetDevice(), materialByteSize, materialCount, name);
	/*-------------------------------------------------------------------
	-            Set Null Texture 
	---------------------------------------------------------------------*/
	auto& resourceManager = ResourceManager::Instance();
	_textures.resize(materialCount);
	for (int i = 0; i < materialCount; ++i)
	{
		_textures[i].Textures[(int)UsageTexture::Diffuse]  = std::shared_ptr<const Texture>(&resourceManager.LoadTexture(L"Resources/Preset/NullAlbedoMap.png"));
		_textures[i].Textures[(int)UsageTexture::Normal ]  = std::shared_ptr<const Texture>(&resourceManager.LoadTexture(L"Resources/Preset/NullNormalMap.DDS"));
		_textures[i].Textures[(int)UsageTexture::Specular] = std::shared_ptr<const Texture>(&resourceManager.LoadTexture(L"Resources/Preset/NullSpecMap.DDS"));
	}
	return true;
}
/****************************************************************************
*                       Finalize
*************************************************************************//**
*  @fn        void MaterialBuffer::Finalize()
*  @brief     Clear Buffer
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void MaterialBuffer::Finalize()
{
	_constantBuffer.reset();
	for (auto& texture : _textures)
	{
		texture.Textures[(int)UsageTexture::Diffuse].reset();
		texture.Textures[(int)UsageTexture::Normal  ].reset();
		texture.Textures[(int)UsageTexture::Specular].reset();
	}
	_textures.clear(); _textures.shrink_to_fit();
}
/****************************************************************************
*                       SetTexture
*************************************************************************//**
*  @fn        bool MaterialBuffer::SetTexture(const std::wstring& texturePath, UsageTexture usage)
*  @brief     Set texture
*  @param[in] const std::wstring& texturePath
*  @param[in] UsageTexture usage
*  @return 　　bool
*****************************************************************************/
bool MaterialBuffer::SetTexture(const std::wstring& texturePath, UsageTexture usage)
{
	UINT32 index = static_cast<UINT32>(usage);
	if (index >= GetMaterialCount()) { return false; }
	
	auto& resourceManager                 = ResourceManager::Instance();
	_textures[index].Textures[(int)usage] = std::shared_ptr<const Texture>(&resourceManager.LoadTexture(texturePath));
	return true;
}