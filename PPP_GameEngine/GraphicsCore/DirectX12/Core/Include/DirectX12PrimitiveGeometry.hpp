//////////////////////////////////////////////////////////////////////////////////
//              Title:  DirectX12PrimitiveGeometry.hpp
//            Content:  DirectX12 2D or 3D Primitive Geometry
//             Author:  Toide Yutaro
//             Create:  2020_11_23
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
	std::vector<VertexPositionNormalTexture> Vertices; /// Vertex (Position, Normal, UV)
	std::vector<UINT16> Indices;                       /// Index  (note: Index uses UINT16)

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
	static MeshData Rect      (float width, float height, float depth);
	static MeshData Quadrangle(float x, float y, float w, float h, float depth);
	static MeshData Box       (float width, float height, float depth, UINT32 numSubdivisions, bool isInvertNormal);
	static MeshData Sphere    (float radius, UINT32 sliceCount, UINT32 stackCount, bool isInvertNormal);
	static MeshData GeoSphere (float radius, UINT32 numSubdivisions, bool isInvertNormal);
	static MeshData Cylinder  (float bottomRadius, float topRadius, float height, UINT32 sliceCount, UINT32 stackCount, bool isInvertNormal);
	static MeshData Grid      (float width, float depth, UINT32 rows, UINT32 columns, bool isInvertNormal);

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
	static VertexPositionNormalTexture MidPoint(const VertexPositionNormalTexture& v0, const VertexPositionNormalTexture& v1);
	static void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, UINT32 sliceCount, UINT32 stackCount, MeshData& meshData);
	static void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, UINT32 sliceCount, UINT32 stackCount, MeshData& meshData);
	static void IsInvertNormal(MeshData& meshData);

	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
};
#endif
