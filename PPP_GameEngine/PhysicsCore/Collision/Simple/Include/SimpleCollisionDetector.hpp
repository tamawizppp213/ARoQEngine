//////////////////////////////////////////////////////////////////////////////////
///             @file   SimpleCollisionDetector.hpp
///             @brief  Collision detection (not broadphase and narrow phase)
///             @author Toide Yutaro
///             @date   2023_04_21
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SIMPLE_COLLISION_DETECTOR_HPP
#define SIMPLE_COLLISION_DETECTOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GameUtility/Base/Include/GUSharedPointer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace physics::core
{
	class PhysicsActor;
	/****************************************************************************
	*				        SimpleCollisionDetector
	*************************************************************************//**
	*  @class     SimpleCollisionDetector
	*  @brief     Collision detection using the each geometry
	*****************************************************************************/
	class SimpleCollisionDetector : public NonCopyable
	{
	protected:
		using PhysicsActorPtr = gu::SharedPointer<PhysicsActor>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool Intersects(const PhysicsActorPtr& first, const PhysicsActorPtr& second, const bool useExtrusion = false) const;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SimpleCollisionDetector() = default;

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
	};
}
#endif