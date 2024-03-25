//////////////////////////////////////////////////////////////////////////////////
///             @file   IGeometry.hpp
///             @brief  Geometry interface. 
///             @author Toide Yutaro
///             @date   2023_01_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PHYSICS_GEOMETRY_TYPE_HPP
#define PHYSICS_GEOMETRY_TYPE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace physics::geometry
{
	enum class GeometryType
	{
		// 3D Geometry
		Sphere  = 0,
		AABB    = 1,
		OBB     = 2,
		Plane   = 3,
		Capsule = 4,
		CountOf = 5,

		Invalid = -1
	};
}
#endif