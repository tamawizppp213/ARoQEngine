//////////////////////////////////////////////////////////////////////////////////
///             @file   GMVertex.hpp
///             @brief  Vertex
///             @author Toide Yutaro
///             @date   2022_11_02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_VERTEX_HPP
#define GM_VERTEX_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMVector.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                              Struct
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	/****************************************************************************
	*							 Vertex
	*************************************************************************//**
	*  @struct    Vertex
	*  @brief     Vertex struct holding position, normal, uv, and color, 
	*****************************************************************************/
	struct Vertex
	{
	public:
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		gm::Float3 Position; /// Position
		gm::Float3 Normal;   /// Normal
		gm::Float4 Color;    /// Color
		gm::Float2 UV;       /// UV

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Vertex()                         = default;
		Vertex(const Vertex&)            = default;
		Vertex& operator=(const Vertex&) = default;
		Vertex(Vertex&&)                 = default;
		Vertex& operator=(Vertex&&)      = default;
		Vertex(gm::Float3 const& position, gm::Float3 const& normal, gm::Float4 const& color, gm::Float2 const& uv)
			: Position(position), Normal(normal), Color(color), UV(uv)
		{
		};
	
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Property
		*****************************************************************************/
	};

	struct SkinMeshVertex
	{
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		gm::Float3   Position;
		gm::Float3   Normal;
		gm::Float2   UV;
		int          BoneIndices[4] = {0};
		float        BoneWeights[4] = {0};

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SkinMeshVertex()                                 = default;
		SkinMeshVertex(const SkinMeshVertex&)            = default;
		SkinMeshVertex& operator=(const SkinMeshVertex&) = default;
		SkinMeshVertex& operator=(SkinMeshVertex&&)      = default;
		SkinMeshVertex(gm::Float3 const& position, gm::Float3 const& normal, gm::Float2 const& uv)
			: Position(position), Normal(normal), UV(uv)
		{
		};
	};

}

#endif