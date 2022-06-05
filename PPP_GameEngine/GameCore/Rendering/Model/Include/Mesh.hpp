//////////////////////////////////////////////////////////////////////////////////
///             @file   Mesh.hpp
///             @brief  Mesh 
///             @author Copyright(c) Pocol. All right reserved.
///                     Partially edit by Toide Reference : DirectX12 é¿ëHÉKÉCÉh 
///             @date   2022_06_02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef MESH_HPP
#define MESH_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/DirectX12/Core/Include/DirectX12Buffer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
struct MeshData;
//////////////////////////////////////////////////////////////////////////////////
//                         Mesh Class
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  			Mesh
*************************************************************************//**
*  @class     Mesh
*  @brief     Mesh
*****************************************************************************/
class Mesh
{
	using MeshBufferPtr   = std::unique_ptr<MeshBuffer[]>;
	using SceneGPUAddress = UINT64;
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	bool Initialize(const MeshData& meshData, const std::wstring& addName = L"");
	void Draw(CommandContext* context, int currentFrameIndex);
	void Terminate();

	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	inline UINT32 GetMaterialID() const { return _materialID; }
	inline UINT32 GetIndexCount() const { return _indexCount; }
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	Mesh() = default;
	virtual ~Mesh();
	Mesh(Mesh&&)            = default;
	Mesh& operator=(Mesh&&) = default;
protected:
	/****************************************************************************
	**                Protected Function                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	*****************************************************************************/
	Mesh(const Mesh&)            = delete; 
	Mesh operator= (const Mesh&) = delete;

	void PrepareVertexAndIndexBuffer(const MeshData& meshData, const std::wstring& name);
	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	MeshBufferPtr   _meshBuffer;  // vertex and index buffer
	UINT32          _materialID;  // material ID in CBV Allocator
	UINT32          _indexCount;  // index Count
};
#endif