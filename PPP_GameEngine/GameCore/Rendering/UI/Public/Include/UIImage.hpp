//////////////////////////////////////////////////////////////////////////////////
///             @file   Image.hpp
///             @brief  Image
///             @author Toide Yutaro
///             @date   2022_11_14
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef IMAGE_HPP
#define IMAGE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GameUtility/Math/Include/GMVertex.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::ui
{
	enum class CoordinateType
	{
		Screen,
		NDC,
	};
	/****************************************************************************
	*				  			Image
	*************************************************************************//**
	*  @class     Image
	*  @brief     UI Image
	*****************************************************************************/
	class Image : public Copyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Create image ui in Screen Space*/
		virtual void CreateInScreenSpace(
			const gm::Float3& position,
			const gm::Float2& rectSize,
			const gm::Float2& u = gm::Float2(0, 1),
			const gm::Float2& v = gm::Float2(0, 1),
			const gm::Float4& color = gm::Float4(1, 1, 1, 1),
			float radian = 0);

		/* @brief : Create image ui in NDC space (x, y, z: -1Å`1)*/
		virtual void CreateInNDCSpace(
			const gm::Float3& position = gm::Float3(0, 0, 0),
			const gm::Float2& rectSize = gm::Float2(2, 2),
			const gm::Float2& u = gm::Float2(0, 1),
			const gm::Float2& v = gm::Float2(0, 1),
			const gm::Float4& color = gm::Float4(1, 1, 1, 1),
			float radian = 0);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const gm::Vertex* GetVertices() const { return _vertices; }
		
		gm::Float2 GetRectSize() const { return _size; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Image() = default;
		
		virtual ~Image();
		
		Image(
			const CoordinateType coordinateType,
			const gm::Float3& position,
			const gm::Float2& rectSize,
			const gm::Float2& u = gm::Float2(0, 1),
			const gm::Float2& v = gm::Float2(0, 1),
			const gm::Float4& color = gm::Float4(1, 1, 1, 1),
			float radian = 0);

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		/* Create rect */
		void CreateRect(const gm::Float3& position, const gm::Float2& rectSize, const gm::Float4& color, const gm::Float2& u, const gm::Float2& v, float radian);
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gm::Vertex _vertices[4] = {};
		gm::Float3 _positionNDC = gm::Float3(0, 0, 0);
		gm::Float2 _size;
	};
}
#endif