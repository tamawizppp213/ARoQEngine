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
#include "GameUtility/Math/Include/GMVertex.hpp"
#include <vector>


//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{
	/****************************************************************************
	*							 MeshData
	*************************************************************************//**
	*  @struct    MeshData
	*  @brief     MeshData
	*****************************************************************************/
	struct MeshData
	{
	public:
		std::vector<gm::Vertex>    Vertices; /// Vertex (Position, Normal, UV)
		std::vector<std::uint32_t> Indices;  /// Index  (note: Index uses UINT16)
		std::uint32_t              MaterialID = 0;
		MeshData() = default;
		MeshData(const MeshData&) = default;
		MeshData& operator=(const MeshData&) = default;
		MeshData(MeshData&&) = default;
		MeshData& operator=(MeshData&&) = default;
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
		static MeshData Box(float width, float height, float depth, std::uint32_t numSubdivisions, bool isInvertNormal, const gm::Float4& color = gm::Float4(1, 1, 1, 1));
		static MeshData Sphere(float radius, std::uint32_t sliceCount, std::uint32_t stackCount, bool isInvertNormal, const gm::Float4& color = gm::Float4(1, 1, 1, 1));
		static MeshData GeoSphere(float radius, std::uint32_t numSubdivisions, bool isInvertNormal, const gm::Float4& color = gm::Float4(1, 1, 1, 1));
		static MeshData Cylinder(float bottomRadius, float topRadius, float height, std::uint32_t sliceCount, std::uint32_t stackCount, bool isInvertNormal, const gm::Float4& color = gm::Float4(1, 1, 1, 1));
		static MeshData Grid(float width, float depth, std::uint32_t rows, std::uint32_t columns, bool isInvertNormal, const gm::Float4& color = gm::Float4(1, 1, 1, 1));

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor
		*****************************************************************************/
		// Prohibit Copy and Move 
		GeometryGenerator() = default;
		GeometryGenerator(const GeometryGenerator&)            = delete;
		GeometryGenerator& operator=(const GeometryGenerator&) = delete;
		GeometryGenerator(GeometryGenerator&&)                 = delete;
		GeometryGenerator& operator=(GeometryGenerator&&)      = delete;

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		static void SubDivide(MeshData& meshData);
		static gm::Vertex MidPoint(const gm::Vertex& v0, const gm::Vertex& v1);
		static void BuildCylinderTopCap(float topRadius, float height, std::uint32_t sliceCount, MeshData& meshData, const gm::Float4& color);
		static void BuildCylinderBottomCap(float bottomRadius, float height, std::uint32_t sliceCount, MeshData& meshData, const gm::Float4& color);
		static void IsInvertNormal(MeshData& meshData);

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
	};
}

#endif
