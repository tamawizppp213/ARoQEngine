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
	bool Sphere_vs_Sphere_Intersects (const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion = false);
	bool Sphere_vs_AABB_Intersects   (const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion = false);
	bool Sphere_vs_OBB_Intersects    (const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion = false);
	bool AABB_vs_Sphere_Intersects   (const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion = false);
	bool AABB_vs_AABB_Intersects     (const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion = false);
	bool AABB_vs_OBB_Intersects      (const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion = false);
	bool OBB_vs_Sphere_Intersects    (const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion = false);
	bool OBB_vs_AABB_Intersects      (const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion = false);
	bool OBB_vs_OBB_Intersects       (const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion = false);
}

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
using namespace physics::core;
using namespace physics::geometry;

#pragma region Main Function
bool SimpleCollisionDetector::Intersects(const PhysicsActorPtr& first, const PhysicsActorPtr& second, const bool useExtrusion) const
{
	assert(("first is nullptr.", first));
	assert(("second is nullptr.", second));

	/*-------------------------------------------------------------------
	-              Record the collision function list.
	---------------------------------------------------------------------*/
	static bool(*functionList[3][3])
		(const PhysicsActorPtr&, const PhysicsActorPtr&, const bool) =
	{
		{::Sphere_vs_Sphere_Intersects, ::Sphere_vs_AABB_Intersects, ::Sphere_vs_OBB_Intersects},
		{::AABB_vs_Sphere_Intersects  , ::AABB_vs_AABB_Intersects  , :: AABB_vs_OBB_Intersects},
		{::OBB_vs_Sphere_Intersects   , ::OBB_vs_AABB_Intersects   , ::OBB_vs_OBB_Intersects}
	};

	const auto geometryTypeA = first ->GetGeometry()->GetGeometryType();
	const auto geometryTypeB = second->GetGeometry()->GetGeometryType();

	return functionList[(int)geometryTypeA][(int)geometryTypeB](first, second, useExtrusion);
}
#pragma endregion Main Function

#pragma region Each Collision Detection Intersects
namespace
{

	bool Sphere_vs_Sphere_Intersects(const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion)
	{
		assert(first->GetGeometry()->GetGeometryType() == GeometryType::Sphere);
		assert(second->GetGeometry()->GetGeometryType() == GeometryType::Sphere);

		const auto geometryA = std::static_pointer_cast<GeometrySphere>(first->GetGeometry());
		const auto geometryB = std::static_pointer_cast<GeometrySphere>(second->GetGeometry());

		// sphere world position
		const auto centerA = first->GetPosition();
		const auto centerB = second->GetPosition();

		// sphere radius
		const auto radiusA = geometryA->Radius;
		const auto radiusB = geometryB->Radius;

		// Distance squared between centers
		const auto delta = centerB - centerA;
		const auto distanceSquared = (float)NormSquared(delta);

		const auto radiusSum = radiusA + radiusB;
		const auto radiusSumSquared = radiusSum * radiusSum;

		// d * d > (r1 + r2) ^2 -> false
		if (distanceSquared > radiusSumSquared) { return false; }

		// extrude the first object from the second object
		if (!useExtrusion) { return true; }

		// ˆÈ‰º”½”­
		return true;
	}

	bool Sphere_vs_AABB_Intersects(const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion)
	{
		return AABB_vs_Sphere_Intersects(second, first, useExtrusion);
	}

	bool Sphere_vs_OBB_Intersects(const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion)
	{
		assert(first ->GetGeometry()->GetGeometryType() == GeometryType::Sphere);
		assert(second->GetGeometry()->GetGeometryType() == GeometryType::OBB);


		return true;
	}

	bool AABB_vs_Sphere_Intersects(const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion)
	{
		assert(first ->GetGeometry()->GetGeometryType() == GeometryType::AABB);
		assert(second->GetGeometry()->GetGeometryType() == GeometryType::Sphere);

		const auto box    = std::static_pointer_cast<GeometryAABB>(first->GetGeometry());
		const auto sphere = std::static_pointer_cast<GeometrySphere>(second->GetGeometry());

		// box config
		const auto  boxCenter  = first->GetPosition();
		const auto& boxExtents = box->HalfExtents;
		const auto  boxMin     = boxCenter - boxExtents;
		const auto  boxMax     = boxCenter + boxExtents;

		// sphere config
		const auto sphereCenter  = second->GetPosition();
		const auto sphereRadius  = sphere->Radius;

		/*-------------------------------------------------------------------
		-           Find the nearest point on the box
		-      if (SphereCenter(i) < BoxMin(i)) d2 += (SphereCenter(i) - BoxMin(i)) ^ 2
        - else if (SphereCenter(i) > BoxMax(i)) d2 += (SphereCenter(i) - BoxMax(i)) ^ 2
		---------------------------------------------------------------------*/
		const auto minDelta = sphereCenter - boxMin;
		const auto maxDelta = sphereCenter - boxMax;

		const auto lessThanMin    = Less(sphereCenter, boxMin);
		const auto greaterThanMax = Greater(sphereCenter, boxMax);

		Vector3 distance = Vector3(0, 0, 0);
		distance = Select(distance, minDelta, BoolVector(lessThanMin));
		distance = Select(distance, maxDelta, BoolVector(greaterThanMax));

		const auto distance2 = Dot(distance, distance);
		const auto radius2   = sphereRadius * sphereRadius;
		
		if (distance2 > radius2) { return false; }

		if (!useExtrusion) { return true; }

		return true;
	}

	bool AABB_vs_AABB_Intersects(const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion)
	{
		assert(first ->GetGeometry()->GetGeometryType() == GeometryType::AABB);
		assert(second->GetGeometry()->GetGeometryType() == GeometryType::AABB);

		// AABB geometry
		const auto geometryA = std::static_pointer_cast<GeometryAABB>(first->GetGeometry());
		const auto geometryB = std::static_pointer_cast<GeometryAABB>(second->GetGeometry());

		// centroid of the AABB
		const auto centerA = first ->GetPosition();
		const auto centerB = second->GetPosition();

		// half extents
		const auto& halfExtentsA = geometryA->HalfExtents;
		const auto& halfExtentsB = geometryB->HalfExtents;

		// calculate min and max
		const auto minA = centerA - halfExtentsA;
		const auto minB = centerB - halfExtentsB;
		const auto maxA = centerA + halfExtentsA;
		const auto maxB = centerB + halfExtentsB;

		// for each i in (x, y,z) if a_min(i) > b_min(i) or b_min(i) > a_max(i) then return false
		if (AnyTrue(GreaterR(minA, maxB))) { return false;}
		if (AnyTrue(GreaterR(minB, maxA))) { return false; }

		// extrude the first object from the second object
		if (!useExtrusion) { return true; }
		// ˆÈ‰º”½”­
		return true;
	}

	bool AABB_vs_OBB_Intersects(const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion)
	{

	}

	bool OBB_vs_Sphere_Intersects(const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion)
	{

	}

	bool OBB_vs_AABB_Intersects(const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion)
	{

	}

	bool OBB_vs_OBB_Intersects(const std::shared_ptr<PhysicsActor>& first, const std::shared_ptr<PhysicsActor>& second, const bool useExtrusion)
	{

	}
}
#pragma endregion Each Collision Detection