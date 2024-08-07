//////////////////////////////////////////////////////////////////////////////////
///             @file   IGeometry.hpp
///             @brief  Geometry interface. 
///             @author Toide Yutaro
///             @date   2023_01_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PHYSICS_SPHERE_GEOMETRY_HPP
#define PHYSICS_SPHERE_GEOMETRY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Private/Include/IGeometry.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace physics::geometry
{
	/****************************************************************************
	*				  			  IGeometry
	****************************************************************************/
	/* @class     IGeometry
	*  @brief     Geometry interface
	*****************************************************************************/
	class GeometrySphere : public IGeometry
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool IsValid() const override;

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		float Radius = 0.0f; // @brief: the radius of the capsule
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GeometrySphere(const float radius = 1.0f) :
			IGeometry(GeometryType::Sphere), Radius(radius)
		{
			Check(IsValid());
		}

	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
	};
}
#endif