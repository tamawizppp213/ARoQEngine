//////////////////////////////////////////////////////////////////////////////////
//              @file   PhysicsScene.cpp
///             @brief  Management physics object
///             @author Toide Yutaro
///             @date   2023_01_09
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/PhysicsRigidBody.hpp"
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace physics::core;
using namespace gm;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Main Function
void RigidBody::Simulate(const float dt)
{
	const auto linearAcceleration  = _accumulateForce  * _inverseMass;
	const auto angularAcceleration = _accumulateTorque * _inverseInertiaTensor;

	_linearVelocity  += linearAcceleration  * dt;
	_angularVelocity += angularAcceleration * dt;

	_position    += _linearVelocity * dt;
	_orientation *= _angularVelocity * dt;

	ClearForceAndTorque();
}
/****************************************************************************
*					AddForce
*************************************************************************//**
*  @fn        void RigidBody::AddForce(const gm::Vector3& force)
*
*  @brief     Add force (N = kg * m/s^2)
*
*  @param[in] const gm::Vector3 force
*
*  @return 　　void
*****************************************************************************/
void RigidBody::AddForce(const gm::Vector3& force, const ForceMode mode)
{
	if (IsInfinite(force)) { OutputDebugStringA("RigidBody::AddForce: force is invalid"); }
	AddSpatialForce(&force, nullptr, mode);
}

/****************************************************************************
*					AddTorque
*************************************************************************//**
*  @fn        void RigidBody::AddTorque(const gm::Vector3& torque)
*
*  @brief     Add torque
*
*  @param[in] const gm::Vector3 torque
*
*  @return 　　void
*****************************************************************************/
void RigidBody::AddTorque(const gm::Vector3& torque, const ForceMode mode)
{
	if (IsInfinite(torque)) { OutputDebugStringA("RigidBody::AddTorque: torque is invalid"); }
	AddSpatialForce(nullptr, &torque, mode);
}

/****************************************************************************
*					AddForceAndTorque
*************************************************************************//**
*  @fn        void RigidBody::AddForceAndTorque(const gm::Vector3& force, const gm::Vector3& torque)
*
*  @brief     Add force and torque
*
*  @param[in] const gm::Vector3 force
*  @param[in] const gm::Vector3 torque
*
*  @return 　　void
*****************************************************************************/
void RigidBody::AddForceAndTorque(const gm::Vector3& force, const gm::Vector3& torque, const ForceMode mode)
{
	if (IsInfinite(force))  { OutputDebugStringA("RigidBody::AddForce: force is invalid"); }
	if (IsInfinite(torque)) { OutputDebugStringA("RigidBody::AddTorque: torque is invalid"); }
	AddSpatialForce(&force, &torque, mode);
}

void RigidBody::AddSpatialForce(const gm::Vector3* force, const gm::Vector3* torque, const ForceMode mode)
{
	switch (mode)
	{
		case ForceMode::Force: 
		{
			if (force ) { _accumulateForce  += *force; }
			if (torque) { _accumulateTorque += *torque; }
			break;
		}
		case ForceMode::Impulse:
		{
			if (force)  { _linearVelocity  += (*force) * _inverseMass; }
			if (torque) { _angularVelocity += (*torque) * _inverseInertiaTensor;}
			break;
		}
	}
}
#pragma endregion Main Function
#pragma region Property
/****************************************************************************
*                       SetMass
*************************************************************************//**
*  @fn        void RigidBody::SetMass(const float mass)
* 
*  @brief     Set mass (unit : kg)
* 
*  @param[in] const float mass
* 
*  @return 　　void
*****************************************************************************/
void RigidBody::SetMass(const float mass)
{
	if (mass != isfinite(mass)) { OutputDebugStringA("The mass is invalid float")   ; return; }
	if (mass < 0.0f)            { OutputDebugStringA("The mass must be nonnegative"); return; }

	_mass        = mass;
	_inverseMass = mass > 0.0f ? 1.0f / mass : 0.0f;
}

/****************************************************************************
*                       SetLinearVelocity
*************************************************************************//**
*  @fn        void RigidBody::SetLinearVelocity(const Vector3& angularVelocity)
*
*  @brief     Set linear velocity
*
*  @param[in] const Vector3& linearVelocity
*
*  @return 　　void
*****************************************************************************/
void RigidBody::SetLinearVelocity(const Vector3& linearVelocity)
{
	if (IsInfinite(linearVelocity)) { OutputDebugStringA("The linear velocity is invalid vector3"); return; }
	_linearVelocity = linearVelocity;
}

/****************************************************************************
*                       SetAngularVelocity
*************************************************************************//**
*  @fn        void RigidBody::SetAngularVelocity(const Vector3& angularVelocity)
*
*  @brief     Set angular velocity
*
*  @param[in] const Vector3& angularVelocity
*
*  @return 　　void
*****************************************************************************/
void RigidBody::SetAngularVelocity(const Vector3& angularVelocity)
{
	if (IsInfinite(angularVelocity)) { OutputDebugStringA("The angular velocity is invalid vector3 "); return; }
	_angularVelocity = angularVelocity;
}

/****************************************************************************
*                       SetLinearDamping
*************************************************************************//**
*  @fn        void RigidBody::SetLinearDamping(const float linearDamping)
*
*  @brief     Set the attenuation rate in translational direction
*
*  @param[in] const float linear damping (the attenuation rate in translational direction)
*
*  @return 　　void
*****************************************************************************/
void RigidBody::SetLinearDamping(const float linearDamping)
{
	if (linearDamping != isfinite(linearDamping)) { OutputDebugStringA("The linear damping is invalid float");  return; }
	if (linearDamping < 0.0f)                     { OutputDebugStringA("The linear damping must be nonnegative"); return; }

	_linearDamping = linearDamping;
}

/****************************************************************************
*                       SetAngularDamping
*************************************************************************//**
*  @fn        void RigidBody::SetLinearDamping(const float linearDamping)
*
*  @brief     Set the attenuation rate in angular direction
*
*  @param[in] const float angular damping (the attenuation rate in angular direction)
*
*  @return 　　void
*****************************************************************************/
void RigidBody::SetAngularDamping(const float angularDamping)
{
	if (angularDamping != isfinite(angularDamping)) { OutputDebugStringA("The angular damping is invalid float");  return; }
	if (angularDamping < 0.0f)                      { OutputDebugStringA("The angular damping must be nonnegative"); return; }

	_angularDamping = angularDamping;
}
#pragma endregion Property