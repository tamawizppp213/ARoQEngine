//////////////////////////////////////////////////////////////////////////////////
///             @file   IGeometry.hpp
///             @brief  Geometry interface. 
///             @author Toide Yutaro
///             @date   2023_01_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PHYSICS_RIGIDBODY_HPP
#define PHYSICS_RIGIDBODY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Math/Include/GMTransform.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace physics::core
{
	enum class ForceMode
	{
		Force,          //!< parameter has unit of mass * length / time^2, i.e., a force    (—Í)
		Impulse,        //!< parameter has unit of mass * length / time, i.e., force * time (—ÍÏ)
	};
	/****************************************************************************
	*				  			  IGeometry
	*************************************************************************//**
	*  @class     IGeometry
	*  @brief     Geometry interface
	*****************************************************************************/
	class RigidBody
	{
		struct Limit
		{
			gm::Vector3 MaxLinearVelocity  = {FLT_MAX, FLT_MAX, FLT_MAX};
			gm::Vector3 MaxAngularVelocity = {FLT_MAX, FLT_MAX, FLT_MAX};
		};
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*-------------------------------------------------------------------
		-                    Update 
		---------------------------------------------------------------------*/
		void Simulate(const float dt);

		/*-------------------------------------------------------------------
		-                    Add spatial force
		---------------------------------------------------------------------*/
		void AddForce(const gm::Vector3& force, const ForceMode mode);

		void AddTorque(const gm::Vector3& torque, const ForceMode mode);

		void AddForceAndTorque(const gm::Vector3& force, const gm::Vector3& torque, const ForceMode mode);

		/*-------------------------------------------------------------------
		-                    Clear spatial force and Torque
		---------------------------------------------------------------------*/
		void ClearForceAndTorque() { _accumulateForce = gm::Vector3(); _accumulateTorque = gm::Vector3(); };

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/*-------------------------------------------------------------------
		-                    Mass
		---------------------------------------------------------------------*/
		float GetMass       () const { return _mass; };

		float GetInverseMass() const { return _inverseMass; }

		void  SetMass(const float mass);

		/*-------------------------------------------------------------------
		-                   Velocity
		---------------------------------------------------------------------*/
		gm::Vector3 GetLinearVelocity () const { return _linearVelocity; }

		gm::Vector3 GetAngularVelocity() const { return _angularVelocity; }

		float GetLinearDamping() const { return _linearDamping; }

		float GetAngularDamping() const { return _angularDamping; }

		void SetLinearVelocity (const gm::Vector3& linearVelocity);

		void SetAngularVelocity(const gm::Vector3& angularVelocity);

		void SetLinearDamping(const float linearDamping);

		void SetAngularDamping(const float angularDamping);

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RigidBody() = default;

	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void AddSpatialForce(const gm::Vector3* force, const gm::Vector3* torque, const ForceMode mode);

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gm::Vector3 _accumulateForce = {};
		gm::Vector3 _accumulateTorque = {};

		gm::Matrix3 _inverseInertiaTensor = gm::Matrix3();

		// Holds the linear position of the rigid body in world space.
		gm::Vector3    _position = {};

		// HOlds the angular orientation of the rigid body in world space.
		gm::Quaternion _orientation = {};

		// Holds the linear velocity the rigid body in world space. (unit: m/s)
		gm::Vector3    _linearVelocity = {};

		// Holds the angular velocity of the rigid body in world space. (unit: rad/s)
		gm::Vector3    _angularVelocity = {};

		// Holds the attenuation rate in translational direction
		float _linearDamping = 0.0f;

		// Holds the attenuation rate in rotational direction
		float _angularDamping = 0.0f;

		// Holds the mass. (unit: kg)
		float _mass = 0.0f;

		// Holds the inverse mass (= 1.0f / mass)
		float _inverseMass = 0.0f;

		Limit _limit = {};
	};
}
#endif