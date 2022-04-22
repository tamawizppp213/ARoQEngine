//////////////////////////////////////////////////////////////////////////////////
///             @file   TemplateText.hpp
///             @brief  TemplateText
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SKYBOX_HPP
#define SKYBOX_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/DirectX12/Core/Include/DirectX12Texture.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12Buffer.hpp"
#include <string>
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
struct MeshBuffer;
namespace gm
{
	struct Matrix4;
}
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			TemplateClass
*************************************************************************//**
*  @class     TemplateClass
*  @brief     temp
*****************************************************************************/
class Skybox
{
	using SceneGPUAddress = UINT64;
	using MeshBufferPtr     = std::unique_ptr<MeshBuffer[]>;
	using ObjectConstantPtr = std::unique_ptr<UploadBuffer>;
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void Initialze(const std::wstring& texturePath, const std::wstring& addName = L"");
	void Draw(SceneGPUAddress scene);
	void Finalize();
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	Skybox() = default;
	~Skybox() = default;
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/
	void PrepareRootSignature(const std::wstring& addName);
	void PreparePipelineState(const std::wstring& addName);
	void PrepareVertexAndIndexBuffer(const std::wstring& addName);
	void PrepareSkyObject(const std::wstring& addName);
	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	Texture           _texture;
	MeshBufferPtr     _meshBuffer = nullptr;
	ObjectConstantPtr _skyObject  = nullptr;
};
#endif