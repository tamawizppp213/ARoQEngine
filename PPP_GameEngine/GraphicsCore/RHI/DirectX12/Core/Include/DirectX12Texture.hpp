//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12Texture.hpp
///             @brief  .tga, .dds, ,png, .jpg, .bmp, .hdr
///             @author Toide Yutaro
///             @date   2020_12_06
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_TEXTURE_HPP
#define DIRECTX12_TEXTURE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "DirectX12GPUResource.hpp"
#include "GraphicsCore/Engine/Include/GraphicsCoreResourceType.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
#include <unordered_map>
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace DirectX
{
	struct Image;
	struct TexMetadata;
}
struct Texture
{
	GPUResource Resource;
	int         TextureID;
	DXGI_FORMAT Format;
	gm::Float3  PixelSize;
	~Texture();
};

struct TextureRGBA
{
	unsigned char R, G, B, A;
};

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			    Texture
*************************************************************************//**
*  @class     Texture
*  @brief     Texture Manager .tga, .dds, ,png, .jpg, .bmp, .hdr
*****************************************************************************/
class TextureManager
{
	using TextureTable = std::unordered_map<std::wstring, std::unique_ptr<Texture>>;
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	static const Texture& LoadTexture(const std::wstring& filePath, TextureType type = TextureType::Texture2D);
	static void CreateTexture1D(const std::wstring& name, Texture& texture, TextureRGBA* data);
	static void CreateTexture2D(const std::wstring& name, Texture& texture, TextureRGBA* data);
	static void CreateTexture3D(const std::wstring& name, Texture& texture, TextureRGBA* data);
	static void ClearTextureTable();
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	static int  RegistSRV(TextureType type, Texture& texture);
	static void CreateTextureBuffer(const D3D12_RESOURCE_DESC& resourceDesc, GPUResource* textureBuffer, bool isDiscreteGPU = true);
	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	static TextureTable _textureTable;
	static int _id;
};
#endif