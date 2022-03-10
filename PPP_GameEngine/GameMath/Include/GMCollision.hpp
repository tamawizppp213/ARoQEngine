//////////////////////////////////////////////////////////////////////////////////
///             @file   GMColor.hpp
///             @brief  Game Math ()
///             @author Toide Yutaro
///             @date   2021_05_16
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_COLLISION_HPP
#define GM_COLLISION_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMMatrix.hpp"
#include <DirectXCollision.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	enum ContainmentType
	{
		Disjoint   = 0,
		Intersects = 1,
		Contains   = 2
	};

	enum PlaneIntersectionType
	{
		Front        = 0,
		Intersecting = 1,
		Back         = 2
	};

	struct BoundingSphere;
	struct BoundingBox;
	struct BoundingOrientedBox;
	struct BoundingFrustum;

#pragma warning(push)
#pragma warning(disable:4324 4820)
// C4324: alignment padding warnings
// C4820: Off by default noise

	/****************************************************************************
	*				  			BoundingSphere
	*************************************************************************//**
	*  @class     Bounding Sphere
	*  @brief     BoundingSphere class
	*****************************************************************************/
	struct BoundingSphere
	{
	public:		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		Float3 Center;
		float  Radius;

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void __vectorcall Transform(BoundingSphere& Out, gm::Matrix4 M) const {};

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		BoundingSphere() { Center = Float3(0.0f, 0.0f, 0.0f); Radius = 1.0f; }
		BoundingSphere(const BoundingSphere&)            = default;
		BoundingSphere& operator=(const BoundingSphere&) = default;
		BoundingSphere(const BoundingSphere&&)           = default;

		constexpr BoundingSphere(const Float3& center, float radius) :Center(center),Radius(radius){}
	protected:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
	};

	/****************************************************************************
	*				  			Ray
	*************************************************************************//**
	*  @class     Ray
	*  @brief     Ray class
	*****************************************************************************/
	struct Ray 
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool Intersects();

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		gm::Float3 Position;
		gm::Float3 Direction;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Ray() noexcept : Position(0, 0, 0), Direction(0, 0, 0) {}
		Ray(const gm::Float3& position, const gm::Float3& direction) noexcept : Position(position), Direction(direction){}

		Ray(const Ray&)            = default;
		Ray& operator=(const Ray&) = default;
		Ray(Ray&&)                 = default;
		Ray& operator=(Ray&&)      = default;

		bool operator == (const Ray& ray) const noexcept;
		bool operator != (const Ray& ray) const noexcept;
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