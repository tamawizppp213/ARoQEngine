//////////////////////////////////////////////////////////////////////////////////
//              Title:  DirectX12PrimitiveGeometry.hpp
//            Content:  DirectX12 2D or 3D Primitive Geometry
//             Author:  Toide Yutaro
//             Create:  2022_03_14
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_PRIMITIVE_GEOMETRY_HPP
#define DIRECTX12_PRIMITIVE_GEOMETRY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "DirectX12VertexTypes.hpp"
#include <vector>


//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*							 MeshData 
*************************************************************************//**
*  @struct    MeshData
*  @brief     MeshData 
*****************************************************************************/
struct MeshData
{
public:
	std::vector<VertexPositionNormalColorTexture> Vertices; /// Vertex (Position, Normal, UV)
	std::vector<UINT32> Indices;                       /// Index  (note: Index uses UINT16)

	MeshData()                           = default;
	MeshData(const MeshData&)            = default;
	MeshData& operator=(const MeshData&) = default;
	MeshData(MeshData&&)                 = default;
	MeshData& operator=(MeshData&&)      = default;
	~MeshData();
};

enum class MeshType
{
	Rect, 
	Quadrangle,
	Box,
	Sphere,
	GeoSphere,
	Cylinder,
	Grid,
};

/****************************************************************************
*							 Primitive Geometry
*************************************************************************//**
*  @struct    GeometryGenerator
*  @brief     GeometryGenerator struct (box, grid, sphere, cylinder...)
*****************************************************************************/
class GeometryGenerator
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	static MeshData Rect(float width, float height, float depth, const gm::Float4& color = gm::Float4(1, 1, 1, 1));
	static MeshData Quadrangle(float x, float y, float w, float h, float depth, const gm::Float4& color = gm::Float4(1, 1, 1, 1));
	static MeshData Box(float width, float height, float depth, UINT32 numSubdivisions, bool isInvertNormal, const gm::Float4& color = gm::Float4(1, 1, 1, 1));
	static MeshData Sphere(float radius, UINT32 sliceCount, UINT32 stackCount, bool isInvertNormal, const gm::Float4& color = gm::Float4(1, 1, 1, 1));
	static MeshData GeoSphere(float radius, UINT32 numSubdivisions, bool isInvertNormal, const gm::Float4& color = gm::Float4(1, 1, 1, 1));
	static MeshData Cylinder(float bottomRadius, float topRadius, float height, UINT32 sliceCount, UINT32 stackCount, bool isInvertNormal, const gm::Float4& color = gm::Float4(1, 1, 1, 1));
	static MeshData Grid(float width, float depth, UINT32 rows, UINT32 columns, bool isInvertNormal, const gm::Float4& color = gm::Float4(1, 1, 1, 1));

	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/

	/****************************************************************************
	**                Constructor
	*****************************************************************************/
	// Prohibit Copy and Move 
	GeometryGenerator()                                    = default;
	GeometryGenerator(const GeometryGenerator&)            = delete;
	GeometryGenerator& operator=(const GeometryGenerator&) = delete;
	GeometryGenerator(GeometryGenerator&&)                 = delete;
	GeometryGenerator& operator=(GeometryGenerator&&)      = delete;

private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	static void SubDivide(MeshData& meshData);
	static VertexPositionNormalColorTexture MidPoint(const VertexPositionNormalColorTexture& v0, const VertexPositionNormalColorTexture& v1);
	static void BuildCylinderTopCap(float topRadius, float height, UINT32 sliceCount, MeshData& meshData, const gm::Float4& color);
	static void BuildCylinderBottomCap(float bottomRadius, float height, UINT32 sliceCount,  MeshData& meshData, const gm::Float4& color);
	static void IsInvertNormal(MeshData& meshData);

	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
};
#endif
