//////////////////////////////////////////////////////////////////////////////////
///             @file   Material.hpp
///             @brief  Material
///             @author Copyright(c) Pocol. All right reserved.
///                     Partially edit by Toide Reference : DirectX12 é¿ëHÉKÉCÉh 
///             @date   2022_06_04
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef MATERIAL_HPP
#define MATERIAL_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MaterialType.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12Buffer.hpp"
#include <vector>
#include <memory>
#include <map>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class Texture;
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  			Material
*************************************************************************//**
*  @class     Material
*  @brief     Material Buffer
*****************************************************************************/
class MaterialBuffer
{
	using TexturePtr        = std::shared_ptr<const Texture>;
	using ConstantBufferPtr = std::shared_ptr<UploadBuffer>;
public:
	enum class UsageTexture
	{
		Diffuse,
		Specular,
		Normal,
		CountOfUsageTexture
	};
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	bool Initialize(size_t materialByteSize, size_t materialCount, const std::wstring& addName = L""); // 256 alignment 
	void Finalize();
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	std::shared_ptr<UploadBuffer> GetBufferPtr() { return std::shared_ptr<UploadBuffer>(_constantBuffer.get()); }
	bool SetTexture(const std::wstring& texturePath, UsageTexture usage);
	
	size_t GetMaterialCount   () const { return _constantBuffer->GetElementCount(); }
	size_t GetMaterialByteSize() const { return _constantBuffer->GetElementByteSize(); }
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	MaterialBuffer();
	~MaterialBuffer();
	MaterialBuffer(const MaterialBuffer&)            = delete;
	MaterialBuffer& operator=(const MaterialBuffer&) = delete;
	MaterialBuffer(MaterialBuffer&&)                 = delete;
	MaterialBuffer& operator=(MaterialBuffer&&)      = delete;
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	struct TextureList
	{
		TexturePtr Textures[(int)UsageTexture::CountOfUsageTexture];
	};

	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	ConstantBufferPtr _constantBuffer = nullptr;
	std::vector<TextureList> _textures ;
};
#endif