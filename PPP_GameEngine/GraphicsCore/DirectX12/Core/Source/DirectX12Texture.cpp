//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12TextureTexture.cpp
///             @brief  .tga, .dds, ,png, .jpg, .bmp, .hdr, .sph, .spa
///             @author Toide Yutaro
///             @date   2020_12_06
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                              Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/DirectX12/Core/Include/DirectX12Texture.hpp"
#include "GraphicsCore/Engine/Include/GraphicsCoreEngine.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12GraphicsDevice.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12BaseStruct.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GameUtility/File/Include/FileSystem.hpp"
#include <DirectXTex/DirectXTex.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma comment(lib, "Pluguins/DirectXTex.lib")
#else
#pragma comment(lib, "Pluguins/DirectXTexd.lib")
#endif
//////////////////////////////////////////////////////////////////////////////////
//                              Texture 
//////////////////////////////////////////////////////////////////////////////////
using namespace DirectX;
TextureManager::TextureTable TextureManager::_textureTable;
int TextureManager::_id = 0;

Texture::~Texture()
{
	Resource.Destroy();
}

void TextureManager::ClearTextureTable()
{
	
	for (auto& table : _textureTable)
	{
		table.second.get()->Resource.Destroy();
	}
	_textureTable.clear();
	_id = 0;
	
}
/****************************************************************************
*							  CreateTexture2D
*************************************************************************//**
*  @fn         void TextureLoader::LoadTexture(const std::wstring& filePath, ResourceComPtr& texture)
*  @brief      Load Texture (.tga, .dds, ,png, .jpg, .bmp, .hdr, .sph, .spa)
*  @param[out] DirectX12& directX12,
*  @param[in]  const std::wstring& filePath
*  @param[out] Texture& texture
*  @return 　　 int
*****************************************************************************/
void TextureManager::CreateTexture1D(const std::wstring& name, Texture& texture, TextureRGBA* data)
{
	/*-------------------------------------------------------------------
	-               If the file is loaded once, read from it
	---------------------------------------------------------------------*/
	if (_textureTable.find(name) != _textureTable.end())
	{
		texture = *_textureTable[name].get(); return;
	}
	/*-------------------------------------------------------------------
	-                 Create texture buffer
	---------------------------------------------------------------------*/
	D3D12_RESOURCE_DESC resourceDesc = RESOURCE_DESC::Texture1D(texture.Format, (UINT64)texture.PixelSize.x);
	CreateTextureBuffer(resourceDesc, &texture.Resource);
	/*-------------------------------------------------------------------
	-                 Create texture data
	---------------------------------------------------------------------*/
	ThrowIfFailed(texture.Resource->WriteToSubresource(0, nullptr, (const void*)data,
		sizeof(TextureRGBA) * (UINT)texture.PixelSize.x,
		sizeof(TextureRGBA) * (UINT)texture.PixelSize.x));
	/*-------------------------------------------------------------------
	-                    Create SRV Desc
	---------------------------------------------------------------------*/
	_id = RegistSRV(TextureType::Texture1D, texture);
	/*-------------------------------------------------------------------
	-                    Add texture table
	---------------------------------------------------------------------*/
	_textureTable[name] = std::make_unique<Texture>(std::move(texture));
	texture = *_textureTable[name].get();
}
void TextureManager::CreateTexture2D(const std::wstring& name, Texture& texture, TextureRGBA* data)
{
	/*-------------------------------------------------------------------
	-               If the file is loaded once, read from it
	---------------------------------------------------------------------*/
	if (_textureTable.find(name) != _textureTable.end())
	{
		texture = *_textureTable[name].get(); return;
	}
	/*-------------------------------------------------------------------
	-                 Create texture buffer
	---------------------------------------------------------------------*/
	D3D12_RESOURCE_DESC resourceDesc = RESOURCE_DESC::Texture2D(texture.Format, (UINT64)texture.PixelSize.x, (UINT)texture.PixelSize.y, 1, 1);
	CreateTextureBuffer(resourceDesc, &texture.Resource);
	/*-------------------------------------------------------------------
	-                 Create texture data
	---------------------------------------------------------------------*/
	ThrowIfFailed(texture.Resource->WriteToSubresource(0, nullptr, (const void*)data,
		sizeof(TextureRGBA) * (UINT)texture.PixelSize.x,
		sizeof(TextureRGBA) * (UINT)texture.PixelSize.x * (UINT)texture.PixelSize.y));
	/*-------------------------------------------------------------------
	-                    Create SRV Desc
	---------------------------------------------------------------------*/
	_id = RegistSRV(TextureType::Texture2D, texture);
	/*-------------------------------------------------------------------
	-                    Add texture table
	---------------------------------------------------------------------*/
	_textureTable[name] = std::make_unique<Texture>(std::move(texture));
	texture = *_textureTable[name].get();
}

void TextureManager::CreateTexture3D(const std::wstring& name, Texture& texture, TextureRGBA* data)
{
	/*-------------------------------------------------------------------
	-               If the file is loaded once, read from it
	---------------------------------------------------------------------*/
	if (_textureTable.find(name) != _textureTable.end())
	{
		texture = *_textureTable[name].get(); return;
	}
	/*-------------------------------------------------------------------
	-                 Create texture buffer
	---------------------------------------------------------------------*/
	D3D12_RESOURCE_DESC resourceDesc = RESOURCE_DESC::Texture3D(texture.Format, (UINT64)texture.PixelSize.x, (UINT64)texture.PixelSize.y, (UINT64)texture.PixelSize.z);
	CreateTextureBuffer(resourceDesc, &texture.Resource);
	/*-------------------------------------------------------------------
	-                 Create texture data
	---------------------------------------------------------------------*/
	ThrowIfFailed(texture.Resource->WriteToSubresource(0, nullptr, (const void*)data,
		sizeof(TextureRGBA) * (UINT)texture.PixelSize.x,
		sizeof(TextureRGBA) * (UINT)texture.PixelSize.x * (UINT)texture.PixelSize.y));
	/*-------------------------------------------------------------------
	-                    Create SRV Desc
	---------------------------------------------------------------------*/
	_id = RegistSRV(TextureType::Texture3D, texture);
	/*-------------------------------------------------------------------
	-                    Add texture table
	---------------------------------------------------------------------*/
	_textureTable[name] = std::make_unique<Texture>(std::move(texture));
	texture = *_textureTable[name].get();
	
}

/****************************************************************************
*							  LoadTexture
*************************************************************************//**
*  @fn         void TextureLoader::LoadTexture(const std::wstring& filePath, ResourceComPtr& texture)
*  @brief      Load Texture (.tga, .dds, ,png, .jpg, .bmp, .hdr, .sph, .spa)
*  @param[out] DirectX12& directX12,
*  @param[in]  const std::wstring& filePath
*  @param[out] Texture& texture
*  @return 　　 int
*****************************************************************************/
const Texture& TextureManager::LoadTexture(const std::wstring& filePath, TextureType type)
{
	auto& engine = GraphicsCoreEngine::Instance();
	/*-------------------------------------------------------------------
	-               If the file is loaded once, read from it
	---------------------------------------------------------------------*/
	if (_textureTable.find(filePath) != _textureTable.end())
	{
		return *_textureTable[filePath].get();
	}

	/*-------------------------------------------------------------------
	-                Choose Extension and Load Texture Data
	---------------------------------------------------------------------*/
	Texture texture;
	TexMetadata  metaData     = {};
	ScratchImage scratchImage = {};
	bool isDXT                = false;
	std::wstring extension    = file::FileSystem::GetExtension(filePath);
	
	/*-------------------------------------------------------------------
	-    Select the appropriate texture loading function for each extension
	---------------------------------------------------------------------*/
	if (extension == L"tga")
	{
		ThrowIfFailed(LoadFromTGAFile(filePath.c_str(), TGA_FLAGS_NONE, &metaData, scratchImage));
	}
	else if (extension == L"dds")
	{
		ThrowIfFailed(LoadFromDDSFile(filePath.c_str(),DDS_FLAGS_NONE, &metaData, scratchImage));
		isDXT = true;
	}
	else if (extension == L"hdr")
	{
		ThrowIfFailed(LoadFromHDRFile(filePath.c_str(), &metaData, scratchImage));
	}
	else
	{
		ThrowIfFailed(LoadFromWICFile(filePath.c_str(), WIC_FLAGS_NONE, &metaData, scratchImage));
	}
	auto image      = scratchImage.GetImage(0, 0, 0);
	bool isDiscrete = true;

	/*-------------------------------------------------------------------
	-                 Create texture buffer
	---------------------------------------------------------------------*/
	D3D12_RESOURCE_DESC resourceDesc = RESOURCE_DESC::Texture2D(metaData.format, (UINT64)image->width, (UINT)image->height, (UINT16)metaData.arraySize, (UINT16)metaData.mipLevels);
	CreateTextureBuffer(resourceDesc, &texture.Resource);

	/*-------------------------------------------------------------------
	-                 Transmit texture buffer to GPU
	---------------------------------------------------------------------*/
	if (!isDiscrete)
	{
		ThrowIfFailed(texture.Resource->WriteToSubresource(
			0,
			nullptr,
			image->pixels,
			static_cast<UINT>(image->rowPitch),
			static_cast<UINT>(image->slicePitch)));
	}
	else
	{
		/*-------------------------------------------------------------------
		-                 Prepare Upload Buffer Setting
		---------------------------------------------------------------------*/
		std::vector<D3D12_SUBRESOURCE_DATA> subResources;
		ThrowIfFailed(PrepareUpload(engine.GetGraphicsDevice()->GetDevice(), image, scratchImage.GetImageCount(), metaData, subResources));

		/*-------------------------------------------------------------------
		-                 Calculate Upload Buffer Size
		---------------------------------------------------------------------*/
		const UINT64 uploadBufferSize = GetRequiredIntermediateSize(
			texture.Resource.GetResource(), 0, static_cast<UINT>(subResources.size()));

		/*-------------------------------------------------------------------
		-                 Create Upload Buffer
		---------------------------------------------------------------------*/
		D3D12_HEAP_PROPERTIES heapProperty = HEAP_PROPERTY(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC   uploadDesc   = RESOURCE_DESC::Buffer(uploadBufferSize);
		
		ResourceComPtr uploadBuffer = nullptr;
		engine.CreateCommittedResource(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&uploadDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(uploadBuffer.GetAddressOf()));

		/*-------------------------------------------------------------------
		-                 Copy Texture Data 
		---------------------------------------------------------------------*/
		UpdateSubresources(engine.GetCommandContext()->GetCommandList(),
			texture.Resource.GetResource(), uploadBuffer.Get(),
			0, 0, static_cast<unsigned int>(subResources.size()),
			subResources.data());

		/*-------------------------------------------------------------------
		-                Execute Command List
		---------------------------------------------------------------------*/
		engine.ExecuteCommandContext();
		engine.WaitNextFrame();	
	}

	/*-------------------------------------------------------------------
	-                    Create SRV Desc
	---------------------------------------------------------------------*/
	_id = RegistSRV(type, texture);
	/*-------------------------------------------------------------------
	-                    Describe texture infomation
	---------------------------------------------------------------------*/
	texture.TextureID  = _id;
	texture.Format     = texture.Resource->GetDesc().Format;
	texture.PixelSize  = gm::Float3((float)image->width, (float)image->height,0.0f);
	texture.Resource->SetName(filePath.c_str());
	/*-------------------------------------------------------------------
	-                    Add texture table
	---------------------------------------------------------------------*/
	_textureTable[filePath] = std::make_unique<Texture>(texture);
	return *_textureTable[filePath].get();
}
/****************************************************************************
*							  CreateTexture
*************************************************************************//**
*  @fn         void TextureLoader::LoadTexture(const std::wstring& filePath, ResourceComPtr& texture)
*  @brief      Load Texture (.tga, .dds, ,png, .jpg, .bmp, .hdr, .sph, .spa)
*  @param[out] DirectX12& directX12,
*  @param[in]  const std::wstring& filePath
*  @param[out] Texture& texture
*  @return 　　 int
*****************************************************************************/
int TextureManager::RegistSRV(TextureType type, Texture& texture)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	switch (type)
	{
		case TextureType::Texture2D:
			srvDesc.Format                        = texture.Resource.GetResource()->GetDesc().Format;
			srvDesc.Shader4ComponentMapping       = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.ViewDimension                 = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels           = texture.Resource->GetDesc().MipLevels;
			srvDesc.Texture2D.PlaneSlice          = 0;
			srvDesc.Texture2D.MostDetailedMip     = 0;
			srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
			break;
		case TextureType::TextureCube:
			srvDesc.Format                          = texture.Resource->GetDesc().Format;
			srvDesc.Shader4ComponentMapping         = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.ViewDimension                   = D3D12_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MipLevels           = texture.Resource->GetDesc().MipLevels;
			srvDesc.TextureCube.MostDetailedMip     = 0;
			srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
			break;
	}
	/*-------------------------------------------------------------------
	-                    Create SRV
	---------------------------------------------------------------------*/
	return GraphicsCoreEngine::Instance().CreateShaderResourceView(&texture.Resource, &srvDesc);

}
void TextureManager::CreateTextureBuffer(const D3D12_RESOURCE_DESC& resourceDesc, GPUResource* textureBuffer, bool isDiscreteGPU)
{
	/*-------------------------------------------------------------------
	-             Setting heap property
	---------------------------------------------------------------------*/
	D3D12_HEAP_PROPERTIES heapProperty = {};
	
	if (isDiscreteGPU)
	{
		heapProperty = HEAP_PROPERTY(D3D12_HEAP_TYPE_DEFAULT);
	}
	else
	{
		heapProperty.Type                 = D3D12_HEAP_TYPE_CUSTOM;
		heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
		heapProperty.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		heapProperty.CreationNodeMask     = 0;
		heapProperty.VisibleNodeMask      = 0;
	}
	/*-------------------------------------------------------------------
	-             Setting the final write destination resource
	---------------------------------------------------------------------*/
	auto& engine = GraphicsCoreEngine::Instance();
	if (isDiscreteGPU)
	{
		engine.CreateCommittedResource(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(textureBuffer->GetAddressOf()));
	}
	else
	{
		engine.CreateCommittedResource(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			nullptr,
			IID_PPV_ARGS(textureBuffer->GetAddressOf()));
	}
}