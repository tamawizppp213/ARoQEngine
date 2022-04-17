//////////////////////////////////////////////////////////////////////////////////
///             @file   Sprite.hpp
///             @brief  Sprite
///             @author Toide Yutaro
///             @date   2022_04_16
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SPRITE_HPP
#define SPRITE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/DirectX12/Core/Include/DirectX12VertexTypes.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace ui
{

	/****************************************************************************
	*				  			Sprite
	*************************************************************************//**
	*  @class     Sprite
	*  @brief     2D Sprite
	*****************************************************************************/
	class Image
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void CreateInScreenSpace(
			const gm::Float2& position,
			const gm::Float2& rectSize,
			const gm::Float2& u = gm::Float2(0, 1),
			const gm::Float2& v = gm::Float2(0, 1),
			const gm::Float4 color = gm::Float4(1, 1, 1, 1),
			float radian = 0);
		void CreateInNDCSpace(
			const gm::Float2& position = gm::Float2(0, 0),
			const gm::Float2& rectSize = gm::Float2(2, 2),
			const gm::Float2& u = gm::Float2(0, 1),
			const gm::Float2& v = gm::Float2(0, 1),
			const gm::Float4 color = gm::Float4(1, 1, 1, 1));
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Image() = default;
		Image(const Image&)            = default;
		Image& operator=(const Image&) = default;
		Image(Image&&)                 = default;
		Image& operator=(Image&&)      = default;
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void CreateRect(const gm::Float3& position, const gm::Float2& rectSize, const gm::Float4& color, float radian);
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VertexPositionNormalColorTexture _vertices[4] = {};
		gm::Float2 _size;
	};
}
#endif