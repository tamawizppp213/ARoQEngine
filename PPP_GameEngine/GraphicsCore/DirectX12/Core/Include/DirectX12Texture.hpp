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
#include "GraphicsCore/DirectX12/Core/Include/DirectX12BaseStruct.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
#include <unordered_map>
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
struct Texture
{
	ResourceComPtr    Resource;
	DXGI_FORMAT       Format;
	GPU_DESC_HANDLER  GPUHandler;
	gm::Float2        ImageSize;
	~Texture();
};
namespace DirectX
{
	struct Image;
	struct TexMetadata;
}
enum class TextureType : unsigned int
{
	Unknown          = 0,
	Buffer           = 1,
	Texture1D        = 2,
	Texture1DArray   = 3,
	Texture2D        = 4,
	Texture2DArray   = 5,
	Texture2DMS      = 6,
	Texture2DMSArray = 7,
	Texture3D        = 8,
	TextureCube      = 9,
	TextureCubeArray = 10,
	RayTracing_Acceleration_Structure = 11
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
	static void LoadTexture(const std::wstring& filePath, Texture& texture, TextureType type = TextureType::Texture2D);
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
	static void CreateTextureFromImageData(Device* device, const DirectX::Image* image, ResourceComPtr& textureBuffer, bool isDiscreteGPU, const DirectX::TexMetadata* metadata);
	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	static TextureTable _textureTable;
	static int _id;
};
#endif