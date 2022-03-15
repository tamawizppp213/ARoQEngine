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
#include "GraphicsCore/DirectX12/Core/Include/DirectX12GraphicsDevice.hpp"
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
	
}
void TextureManager::ClearTextureTable()
{
	
	for (auto& table : _textureTable)
	{
		table.second.get()->Resource = nullptr;
	}
	_textureTable.clear();
	_id = 0;
	
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
void TextureManager::LoadTexture(const std::wstring& filePath, Texture& texture, TextureType type)
{
	GraphicsDeviceDirectX12& directX12 = GraphicsDeviceDirectX12::Instance();
	/*-------------------------------------------------------------------
	-               If the file is loaded once, read from it
	---------------------------------------------------------------------*/
	if (_textureTable.find(filePath) != _textureTable.end())
	{
		texture = *_textureTable[filePath].get();
		return;
	}

	/*-------------------------------------------------------------------
	-                Choose Extension and Load Texture Data
	---------------------------------------------------------------------*/
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

	CreateTextureFromImageData(directX12.GetDevice().Get(), image, texture.Resource, isDiscrete, &metaData);

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
		ThrowIfFailed(PrepareUpload(directX12.GetDevice().Get(), image, scratchImage.GetImageCount(), metaData, subResources));

		/*-------------------------------------------------------------------
		-                 Calculate Upload Buffer Size
		---------------------------------------------------------------------*/
		const UINT64 uploadBufferSize = GetRequiredIntermediateSize(
			texture.Resource.Get(), 0, static_cast<UINT>(subResources.size()));

		/*-------------------------------------------------------------------
		-                 Create Upload Buffer
		---------------------------------------------------------------------*/
		D3D12_HEAP_PROPERTIES heapProperty = HEAP_PROPERTY(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC   resourceDesc = RESOURCE_DESC::Buffer(uploadBufferSize);
		
		ResourceComPtr uploadBuffer = nullptr;
		ThrowIfFailed(directX12.GetDevice()->CreateCommittedResource(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(uploadBuffer.GetAddressOf())));

		/*-------------------------------------------------------------------
		-                 Copy Texture Data 
		---------------------------------------------------------------------*/
		UpdateSubresources(directX12.GetCommandList().Get(),
			texture.Resource.Get(), uploadBuffer.Get(),
			0, 0, static_cast<unsigned int>(subResources.size()),
			subResources.data());

		/*-------------------------------------------------------------------
		-                Execute Command List
		---------------------------------------------------------------------*/
		directX12.GetCommandList()->Close();
		ID3D12CommandList* commandLists[] = { directX12.GetCommandList().Get()};
		directX12.GetCommandQueue().Get()->ExecuteCommandLists(_countof(commandLists), commandLists);
		directX12.FlushCommandQueue();
		directX12.ResetCommandList();

		
	}

	/*-------------------------------------------------------------------
	-                    Create SRV Desc
	---------------------------------------------------------------------*/
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	switch (type)
	{
		case TextureType::Texture2D:
			srvDesc.Format                        = texture.Resource.Get()->GetDesc().Format;
			srvDesc.Shader4ComponentMapping       = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.ViewDimension                 = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels           = 1;
			srvDesc.Texture2D.PlaneSlice          = 0;
			srvDesc.Texture2D.MostDetailedMip     = 0;
			srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
			break;
		case TextureType::TextureCube:
			srvDesc.Format                          = texture.Resource.Get()->GetDesc().Format;
			srvDesc.Shader4ComponentMapping         = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.ViewDimension                   = D3D12_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MipLevels           = texture.Resource.Get()->GetDesc().MipLevels;
			srvDesc.TextureCube.MostDetailedMip     = 0;
			srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
			break;
	}
	/*-------------------------------------------------------------------
	-                    Create SRV
	---------------------------------------------------------------------*/
	_id = directX12.IssueViewID(HeapFlag::SRV);
	directX12.GetDevice()->CreateShaderResourceView(texture.Resource.Get(), &srvDesc,
		directX12.GetCPUResourceView(HeapFlag::SRV, _id));

	/*-------------------------------------------------------------------
	-                    Describe texture infomation
	---------------------------------------------------------------------*/
	Texture addTexture;
	addTexture.Resource   = texture.Resource;
	addTexture.Format     = texture.Resource.Get()->GetDesc().Format;
	addTexture.GPUHandler = directX12.GetGPUResourceView(HeapFlag::SRV, _id);
	addTexture.ImageSize  = gm::Float2((float)image->width, (float)image->height);
	addTexture.Resource->SetName(filePath.c_str());
	/*-------------------------------------------------------------------
	-                    Add texture table
	---------------------------------------------------------------------*/
	_textureTable[filePath] = std::make_unique<Texture>(addTexture);
	texture = std::move(addTexture);
}

/****************************************************************************
*					    CreateTextureFromImageData
*************************************************************************//**
*  @fn         void Texture::CreateTextureFromImageData(Device* device, const DirectX::Image* image, ResourceComPtr& textureBuffer, bool isDiscreteGPU)
*  @brief      Load Texture (.tga, .dds, ,png, .jpg, .bmp, .hdr, .sph, .spa)
*  @param[out] Device* device
*  @param[in]  const DirectX::Image* image
*  @param[out] ResourceComPtr& textureBuffer
*  @param[in]  bool isDiscreteGPU
*  @return 　　 void
*****************************************************************************/
void TextureManager::CreateTextureFromImageData(Device* device, const DirectX::Image* image, ResourceComPtr& textureBuffer, bool isDiscreteGPU, const DirectX::TexMetadata* metadata)
{
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
	D3D12_RESOURCE_DESC resourceDesc = RESOURCE_DESC::Texture2D(metadata->format, (UINT64)image->width, (UINT)image->height, (UINT16)metadata->arraySize, (UINT16)metadata->mipLevels);

	if (isDiscreteGPU)
	{
		ThrowIfFailed(device->CreateCommittedResource(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(textureBuffer.ReleaseAndGetAddressOf())))
	}
	else
	{
		ThrowIfFailed(device->CreateCommittedResource(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			nullptr,
			IID_PPV_ARGS(textureBuffer.ReleaseAndGetAddressOf())));
	}
	
}