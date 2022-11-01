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
		**                Public Function
		*****************************************************************************/
		gm::Float3 Position; /// Position
		gm::Float3 Normal;   /// Normal
		gm::Float4 Color;    /// Color
		gm::Float2 UV;       /// UV
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		Vertex()                         = default;
		Vertex(const Vertex&)            = default;
		Vertex& operator=(const Vertex&) = default;
		Vertex(Vertex&&)                 = default;
		Vertex& operator=(Vertex&&)      = default;
		Vertex(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& normal, DirectX::XMFLOAT4 const& color, DirectX::XMFLOAT2 const& uv)
			: Position(position), Normal(normal), Color(color), UV(uv)
		{
		};
		Vertex(DirectX::FXMVECTOR position, DirectX::FXMVECTOR normal, DirectX::FXMVECTOR color, DirectX::FXMVECTOR uv)
		{
			DirectX::XMStoreFloat3(&this->Position, position);
			DirectX::XMStoreFloat3(&this->Normal, normal);
			DirectX::XMStoreFloat4(&this->Color, color);
			DirectX::XMStoreFloat2(&this->UV, uv);
		}
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
	};
}

#endif