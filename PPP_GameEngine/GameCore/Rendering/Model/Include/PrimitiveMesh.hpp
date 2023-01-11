//////////////////////////////////////////////////////////////////////////////////
//              Title:  PrimitiveMesh.hpp
//            Content:  2D or 3D Primitive Mesh Loader
//             Author:  Toide Yutaro
//             Create:  2022_11_16
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PRIMITIVE_MESH_HPP
#define PRIMITIVE_MESH_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Math/Include/GMVertex.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <vector>

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{
	/****************************************************************************
	*							 PrimitiveMesh
	*************************************************************************//**
	*  @struct    PrimitiveMesh
	*  @brief     PrimitiveMesh
	*****************************************************************************/
	struct PrimitiveMesh : public Copyable
	{
	public:
		std::vector<gm::Vertex>    Vertices; /// Vertex (Position, Normal, UV)
		std::vector<std::uint32_t> Indices;  /// Index  (note: Index uses UINT16)
		PrimitiveMesh() = default;
		~PrimitiveMesh();
	};

	enum class PrimitiveMeshType
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
	class PrimitiveMeshGenerator : public NonCopyAndMove
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		static PrimitiveMesh Rect(float width, float height, float depth, const gm::Float4& color = gm::Float4(1, 1, 1, 1));
		static PrimitiveMesh Quadrangle(float x, float y, float w, float h, float depth, const gm::Float4& color = gm::Float4(1, 1, 1, 1));
		static PrimitiveMesh Box(float width, float height, float depth, std::uint32_t numSubdivisions, bool isInvertNormal, const gm::Float4& color = gm::Float4(1, 1, 1, 1));
		static PrimitiveMesh Sphere(float radius, std::uint32_t sliceCount, std::uint32_t stackCount, bool isInvertNormal, const gm::Float4& color = gm::Float4(1, 1, 1, 1));
		static PrimitiveMesh GeoSphere(float radius, std::uint32_t numSubdivisions, bool isInvertNormal, const gm::Float4& color = gm::Float4(1, 1, 1, 1));
		static PrimitiveMesh Cylinder(float bottomRadius, float topRadius, float height, std::uint32_t sliceCount, std::uint32_t stackCount, bool isInvertNormal, const gm::Float4& color = gm::Float4(1, 1, 1, 1));
		static PrimitiveMesh Grid(float width, float depth, std::uint32_t rows, std::uint32_t columns, bool isInvertNormal, const gm::Float4& color = gm::Float4(1, 1, 1, 1));

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor
		*****************************************************************************/
		// Prohibit Copy and Move 
		PrimitiveMeshGenerator() = default;

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		static void SubDivide(PrimitiveMesh& meshData);
		static gm::Vertex MidPoint(const gm::Vertex& v0, const gm::Vertex& v1);
		static void BuildCylinderTopCap(float topRadius, float height, std::uint32_t sliceCount, PrimitiveMesh& meshData, const gm::Float4& color);
		static void BuildCylinderBottomCap(float bottomRadius, float height, std::uint32_t sliceCount, PrimitiveMesh& meshData, const gm::Float4& color);
		static void IsInvertNormal(PrimitiveMesh& meshData);

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
	};
}

#endif
