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
		Sphere,
		Plane,
		Capsule,
		Box,

		CountOf,
		Invalid = -1
	};
}
#endif