//////////////////////////////////////////////////////////////////////////////////
///             @file   SimpleCollisionDetector.hpp
///             @brief  Collision detection (not broadphase and narrow phase)
///             @author Toide Yutaro
///             @date   2023_04_21
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/SimpleCollisionDetector.hpp"
#include "../../../Core/Public/Include/PhysicsActor.hpp"
#include "../../../Geometry/Private/Include/IGeometry.hpp"
#include "../../../Geometry/Public/Include/GeometrySphere.hpp"
#include "../../../Geometry/Public/Include/GeometryBox.hpp"
#include "../../../Geometry/Public/Include/GeometryCapsule.hpp"
#include "../../../Geometry/Public/Include/GeometryPlane.hpp"
#include <DirectXCollision.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace physics::core;
using namespace gm;

namespace
{
	bool Sphere_vs_Sphere_Intersects(const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion = false);
	bool Sphere_vs_Box_Intersects   (const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion = false);
	bool Box_vs_Sphere   (const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion = false);
	bool Box_vs_Box      (const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion = false);
}

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
using namespace physics::core;
using namespace physics::geometry;

#pragma region Main Function
bool SimpleCollisionDetector::Intersects(const PhysicsActorPtr& first, const PhysicsActorPtr& second) const
{
	static bool(*functionList[2][2])
		(const PhysicsActorPtr&, const PhysicsActorPtr&, const bool) =
	{
		{::Sphere_vs_Sphere, ::Sphere_vs_Box},
		{::Box_vs_Sphere   , ::Box_vs_Box}
	};

	return true;
}
#pragma endregion Main Function

#pragma region Each Collision Detection Intersects
bool ::Sphere_vs_Sphere(const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion)
{
	assert(first ->GetGeometry()->GetGeometryType() == GeometryType::Sphere);
	assert(second->GetGeometry()->GetGeometryType() == GeometryType::Sphere);

	const auto geometryA = std::static_pointer_cast<GeometrySphere>(first ->GetGeometry());
	const auto geometryB = std::static_pointer_cast<GeometrySphere>(second->GetGeometry());
	
	// sphere world position
	const auto centerA   = first ->GetPosition();
	const auto centerB   = second->GetPosition();

	// sphere radius
	const auto radiusA   = geometryA->Radius;
	const auto radiusB   = geometryB->Radius;

	// Distance squared between centers
	const auto delta           = centerB - centerA;
	const auto distanceSquared = NormSquared(delta);

	const auto radiusSum = radiusA + radiusB;
	const auto radiusSumSquared = radiusSum * radiusSum;

	// d * d > (r1 + r2) ^2 -> false
	if (distanceSquared > radiusSumSquared) { return false; }

	// extrude the first object from the second object
	if (!useExtrusion) { return false; }


	return true;
}

bool ::Sphere_vs_Box(const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion)
{
	assert(first ->GetGeometry()->GetGeometryType() == GeometryType::Sphere);
	assert(second->GetGeometry()->GetGeometryType() == GeometryType::Box);
	return true;
}

bool ::Box_vs_Sphere(const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion)
{
	assert(first ->GetGeometry()->GetGeometryType() == GeometryType::Box);
	assert(second->GetGeometry()->GetGeometryType() == GeometryType::Sphere);
	return true;
}

bool ::Box_vs_Box(const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion)
{
	assert(first ->GetGeometry()->GetGeometryType() == GeometryType::Box);
	assert(second->GetGeometry()->GetGeometryType() == GeometryType::Box);
	return true;
}
#pragma endregion Each Collision Detection