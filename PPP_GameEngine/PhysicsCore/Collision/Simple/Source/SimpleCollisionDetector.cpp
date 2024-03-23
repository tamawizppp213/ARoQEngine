////////////////////////////////////////////////////////////////////////////////////
/////             @file   SimpleCollisionDetector.hpp
/////             @brief  Collision detection (not broadphase and narrow phase)
/////             @author Toide Yutaro
/////             @date   2023_04_21
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
////                             Include
////////////////////////////////////////////////////////////////////////////////////
//#include "../Include/SimpleCollisionDetector.hpp"
//#include "../../../Core/Public/Include/PhysicsActor.hpp"
//#include "../../../Geometry/Private/Include/IGeometry.hpp"
//#include "../../../Geometry/Public/Include/GeometrySphere.hpp"
//#include "../../../Geometry/Public/Include/GeometryBox.hpp"
//#include "../../../Geometry/Public/Include/GeometryCapsule.hpp"
//#include "../../../Geometry/Public/Include/GeometryPlane.hpp"
//#include <DirectXCollision.h>
////////////////////////////////////////////////////////////////////////////////////
////                              Define
////////////////////////////////////////////////////////////////////////////////////
//using namespace physics::core;
//using namespace gm;
//
//namespace
//{
//	bool Sphere_vs_Sphere_Intersects (const gu::SharedPointer<PhysicsActor>& first, const gu::SharedPointer<PhysicsActor>& second, const bool useExtrusion = false);
//	bool Sphere_vs_AABB_Intersects   (const gu::SharedPointer<PhysicsActor>& first, const gu::SharedPointer<PhysicsActor>& second, const bool useExtrusion = false);
//	bool Sphere_vs_OBB_Intersects    (const gu::SharedPointer<PhysicsActor>& first, const gu::SharedPointer<PhysicsActor>& second, const bool useExtrusion = false);
//	bool AABB_vs_Sphere_Intersects   (const gu::SharedPointer<PhysicsActor>& first, const gu::SharedPointer<PhysicsActor>& second, const bool useExtrusion = false);
//	bool AABB_vs_AABB_Intersects     (const gu::SharedPointer<PhysicsActor>& first, const gu::SharedPointer<PhysicsActor>& second, const bool useExtrusion = false);
//	bool AABB_vs_OBB_Intersects      (const gu::SharedPointer<PhysicsActor>& first, const gu::SharedPointer<PhysicsActor>& second, const bool useExtrusion = false);
//	bool OBB_vs_Sphere_Intersects    (const gu::SharedPointer<PhysicsActor>& first, const gu::SharedPointer<PhysicsActor>& second, const bool useExtrusion = false);
//	bool OBB_vs_AABB_Intersects      (const gu::SharedPointer<PhysicsActor>& first, const gu::SharedPointer<PhysicsActor>& second, const bool useExtrusion = false);
//	bool OBB_vs_OBB_Intersects       (const gu::SharedPointer<PhysicsActor>& first, const gu::SharedPointer<PhysicsActor>& second, const bool useExtrusion = false);
//}
//
////////////////////////////////////////////////////////////////////////////////////
////                          Implement
////////////////////////////////////////////////////////////////////////////////////
//using namespace physics::core;
//using namespace physics::geometry;
//
//#pragma region Main Function
//bool SimpleCollisionDetector::Intersects(const PhysicsActorPtr& first, const PhysicsActorPtr& second, const bool useExtrusion) const
//{
//	assert(("first is nullptr.", first));
//	assert(("second is nullptr.", second));
//
//	/*-------------------------------------------------------------------
//	-              Record the collision function list.
//	---------------------------------------------------------------------*/
//	static bool(*functionList[3][3])
//		(const PhysicsActorPtr&, const PhysicsActorPtr&, const bool) =
//	{
//		{::Sphere_vs_Sphere_Intersects, ::Sphere_vs_AABB_Intersects, ::Sphere_vs_OBB_Intersects},
//		{::AABB_vs_Sphere_Intersects  , ::AABB_vs_AABB_Intersects  , :: AABB_vs_OBB_Intersects},
//		{::OBB_vs_Sphere_Intersects   , ::OBB_vs_AABB_Intersects   , ::OBB_vs_OBB_Intersects}
//	};
//
//	const auto geometryTypeA = first ->GetGeometry()->GetGeometryType();
//	const auto geometryTypeB = second->GetGeometry()->GetGeometryType();
//
//	return functionList[(int)geometryTypeA][(int)geometryTypeB](first, second, useExtrusion);
//}
//#pragma endregion Main Function
//
//#pragma region Each Collision Detection Intersects
//namespace
//{
//
//	bool Sphere_vs_Sphere_Intersects(const gu::SharedPointer<PhysicsActor>& first, const gu::SharedPointer<PhysicsActor>& second, const bool useExtrusion)
//	{
//		assert(first->GetGeometry()->GetGeometryType() == GeometryType::Sphere);
//		assert(second->GetGeometry()->GetGeometryType() == GeometryType::Sphere);
//
//		const auto geometryA = gu::StaticPointerCast<GeometrySphere>(first->GetGeometry());
//		const auto geometryB = gu::StaticPointerCast<GeometrySphere>(second->GetGeometry());
//
//		// sphere world position
//		const auto centerA = first->GetPosition();
//		const auto centerB = second->GetPosition();
//
//		// sphere radius
//		const auto radiusA = geometryA->Radius;
//		const auto radiusB = geometryB->Radius;
//
//		// Distance squared between centers
//		const auto delta = centerB - centerA;
//		const auto distanceSquared = (float)NormSquared(delta);
//
//		const auto radiusSum = radiusA + radiusB;
//		const auto radiusSumSquared = radiusSum * radiusSum;
//
//		// d * d > (r1 + r2) ^2 -> false
//		if (distanceSquared > radiusSumSquared) { return false; }
//
//		// extrude the first object from the second object
//		if (!useExtrusion) { return true; }
//
//		// ˆÈ‰º”½”­
//		return true;
//	}
//
//	bool Sphere_vs_AABB_Intersects(const gu::SharedPointer<PhysicsActor>& first, const gu::SharedPointer<PhysicsActor>& second, const bool useExtrusion)
//	{
//		return AABB_vs_Sphere_Intersects(second, first, useExtrusion);
//	}
//
//	bool Sphere_vs_OBB_Intersects(const gu::SharedPointer<PhysicsActor>& first, const gu::SharedPointer<PhysicsActor>& second, const bool useExtrusion)
//	{
//		return OBB_vs_Sphere_Intersects(second, first, useExtrusion);
//	}
//
//	bool AABB_vs_Sphere_Intersects(const gu::SharedPointer<PhysicsActor>& first, const gu::SharedPointer<PhysicsActor>& second, const bool useExtrusion)
//	{
//		assert(first ->GetGeometry()->GetGeometryType() == GeometryType::AABB);
//		assert(second->GetGeometry()->GetGeometryType() == GeometryType::Sphere);
//
//		const auto box    = gu::StaticPointerCast<GeometryAABB>(first->GetGeometry());
//		const auto sphere = gu::StaticPointerCast<GeometrySphere>(second->GetGeometry());
//
//		// box config
//		const auto  boxCenter  = first->GetPosition();
//		const auto& boxExtents = box->HalfExtents;
//		const auto  boxMin     = boxCenter - boxExtents;
//		const auto  boxMax     = boxCenter + boxExtents;
//
//		// sphere config
//		const auto sphereCenter  = second->GetPosition();
//		const auto sphereRadius  = sphere->Radius;
//
//		/*-------------------------------------------------------------------
//		-           Find the nearest point on the box
//		-      if (SphereCenter(i) < BoxMin(i)) d2 += (SphereCenter(i) - BoxMin(i)) ^ 2
//        - else if (SphereCenter(i) > BoxMax(i)) d2 += (SphereCenter(i) - BoxMax(i)) ^ 2
//		---------------------------------------------------------------------*/
//		const auto minDelta = sphereCenter - boxMin;
//		const auto maxDelta = sphereCenter - boxMax;
//
//		const auto lessThanMin    = Less(sphereCenter, boxMin);
//		const auto greaterThanMax = Greater(sphereCenter, boxMax);
//
//		Vector3 distance = Vector3(0, 0, 0);
//		distance = Select(distance, minDelta, BoolVector(lessThanMin));
//		distance = Select(distance, maxDelta, BoolVector(greaterThanMax));
//
//		const auto distance2 = Dot(distance, distance);
//		const auto radius2   = sphereRadius * sphereRadius;
//		
//		if (distance2 > radius2) { return false; }
//
//		if (!useExtrusion) { return true; }
//
//		return true;
//	}
//
//	bool AABB_vs_AABB_Intersects(const gu::SharedPointer<PhysicsActor>& first, const gu::SharedPointer<PhysicsActor>& second, const bool useExtrusion)
//	{
//		assert(first ->GetGeometry()->GetGeometryType() == GeometryType::AABB);
//		assert(second->GetGeometry()->GetGeometryType() == GeometryType::AABB);
//
//		// AABB geometry
//		const auto geometryA = gu::StaticPointerCast<GeometryAABB>(first->GetGeometry());
//		const auto geometryB = gu::StaticPointerCast<GeometryAABB>(second->GetGeometry());
//
//		// centroid of the AABB
//		const auto centerA = first ->GetPosition();
//		const auto centerB = second->GetPosition();
//
//		// half extents
//		const auto& halfExtentsA = geometryA->HalfExtents;
//		const auto& halfExtentsB = geometryB->HalfExtents;
//
//		// calculate min and max
//		const auto minA = centerA - halfExtentsA;
//		const auto minB = centerB - halfExtentsB;
//		const auto maxA = centerA + halfExtentsA;
//		const auto maxB = centerB + halfExtentsB;
//
//		// for each i in (x, y,z) if a_min(i) > b_min(i) or b_min(i) > a_max(i) then return false
//		if (AnyTrue(GreaterR(minA, maxB))) { return false;}
//		if (AnyTrue(GreaterR(minB, maxA))) { return false; }
//
//		// extrude the first object from the second object
//		if (!useExtrusion) { return true; }
//		// ˆÈ‰º”½”­
//		return true;
//	}
//
//	bool AABB_vs_OBB_Intersects(const gu::SharedPointer<PhysicsActor>& first, const gu::SharedPointer<PhysicsActor>& second, const bool useExtrusion)
//	{
//		return OBB_vs_AABB_Intersects(second, first, useExtrusion);
//	}
//
//	bool OBB_vs_Sphere_Intersects(const gu::SharedPointer<PhysicsActor>& actorOBB, const gu::SharedPointer<PhysicsActor>& actorSphere, const bool useExtrusion)
//	{
//		assert(actorOBB   ->GetGeometry()->GetGeometryType() == GeometryType::OBB);
//		assert(actorSphere->GetGeometry()->GetGeometryType() == GeometryType::Sphere);
//
//		// geometry
//		const auto geometryOBB    = gu::StaticPointerCast<GeometryOBB>(actorOBB->GetGeometry());
//		const auto geometrySphere = gu::StaticPointerCast<GeometrySphere>(actorSphere->GetGeometry());
//
//		// sphere config
//		auto sphereCenter = actorSphere->GetPosition();
//		const auto sphereRadius = geometrySphere->Radius;
//
//		// obb config
//		const auto  obbCenter      = actorOBB->GetPosition();
//		const auto& obbExtents     = geometryOBB->HalfExtents;
//		const auto& obbOrientation = actorOBB->GetRotation();
//
//		// Transform the center of the sphere to be local to the box
//		// boxMin = -BoxExtents
//		// boxMax = +BoxExtents
//
//		sphereCenter = Vector3(DirectX::XMVector3InverseRotate(sphereCenter - obbCenter, obbOrientation));
//
//		// Compute distance for each dimension
//		const auto lessThanMin    = Less(sphereCenter, -Vector3(obbExtents));
//		const auto greaterThanMax = Greater(sphereCenter, Vector3(obbExtents));
//		const auto minDelta       = sphereCenter + obbExtents;
//		const auto maxDelta       = sphereCenter - obbExtents;
//
//		// Choose value for each dimension based on the comparison
//		auto distance = Vector3(0, 0, 0);
//		distance = Select(distance, minDelta, BoolVector(lessThanMin));
//		distance = Select(distance, maxDelta, BoolVector(greaterThanMax));
//
//		const auto distance2 = Dot(distance, distance);
//
//		return distance2 <= sphereRadius * sphereRadius;
//	}
//
//	bool OBB_vs_AABB_Intersects(const gu::SharedPointer<PhysicsActor>& first, const gu::SharedPointer<PhysicsActor>& second, const bool useExtrusion)
//	{
//		OBB_vs_OBB_Intersects(first, second);
//		return true;
//	}
//
//	bool OBB_vs_OBB_Intersects(const gu::SharedPointer<PhysicsActor>& first, const gu::SharedPointer<PhysicsActor>& second, const bool useExtrusion)
//	{
//		using namespace DirectX;
//		assert(first ->GetGeometry()->GetGeometryType() == GeometryType::OBB);
//		assert(second->GetGeometry()->GetGeometryType() == GeometryType::OBB ||
//			   second->GetGeometry()->GetGeometryType() == GeometryType::AABB);
//
//		// geometry
//		const auto geometryA = gu::StaticPointerCast<GeometryOBB>(first ->GetGeometry());
//		const auto geometryB = gu::StaticPointerCast<GeometryOBB>(second->GetGeometry());
//
//		// get orientation
//		const auto orientationA = first ->GetRotation();
//		const auto orientationB = second->GetRotation();
//		const auto quaternion   = orientationA * Conjugate(orientationB);
//		const auto rotation     =  RotationQuaternion(quaternion);
//
//		// compute the translation of B relative to A.
//		const auto centerA = first ->GetPosition();
//		const auto centerB = second->GetPosition();
//		const auto t = Vector3(DirectX::XMVector3InverseRotate(centerB - centerA, orientationA));
//
//		//
//		// h(A) = extents of A.
//		// h(B) = extents of B.
//		//
//		// a(u) = axes of A = (1,0,0), (0,1,0), (0,0,1)
//		// b(u) = axes of B relative to A = (r00,r10,r20), (r01,r11,r21), (r02,r12,r22)
//		//
//		// For each possible separating axis l:
//		//   d(A) = sum (for i = u,v,w) h(A)(i) * abs( a(i) dot l )
//		//   d(B) = sum (for i = u,v,w) h(B)(i) * abs( b(i) dot l )
//		//   if abs( t dot l ) > d(A) + d(B) then disjoint
//		//
//
//		// Load extents of A and B.
//		const auto& halfExtentsA = geometryA->HalfExtents;
//		const auto& halfExtentsB = geometryB->HalfExtents;
//
//		const auto R0X = rotation.GetX();
//		const auto R1X = rotation.GetY();
//		const auto R2X = rotation.GetZ();
//
//		const auto tRotation = Transpose(rotation);
//
//		const auto RX0 = tRotation.GetX();
//		const auto RX1 = tRotation.GetY();
//		const auto RX2 = tRotation.GetZ();
//
//		// absolute value of rows
//		const auto AR0X = Abs(R0X);
//		const auto AR1X = Abs(R1X);
//		const auto AR2X = Abs(R2X);
//
//		const auto ARX0 = Abs(RX0);
//		const auto ARX1 = Abs(RX1);
//		const auto ARX2 = Abs(RX2);
//
//		Vector3 d, d_A, d_B;
//		// l = a(u) = (1, 0, 0)
//		// t dot l = t.x
//		// d(A) = h(A).x
//		// d(B) = h(B) dot abs(r00, r01, r02)
//		const auto a = DirectX::XMVectorSplatX(t);
//		d   = Vector3(DirectX::XMVectorSplatX(t));
//		d_A = Vector3(DirectX::XMVectorSplatX(halfExtentsA));
//		d_B = Dot(Vector3(halfExtentsB), AR0X);
//		auto NoIntersection = Greater(Abs(d), d_A + d_B);
//
//		// l = a(v) = (0, 1, 0)
//		// t dot l = t.y
//		// d(A) = h(A).y
//		// d(B) = h(B) dot abs(r10, r11, r12)
//		d   = Vector3(DirectX::XMVectorSplatY(t));
//		d_A = Vector3(DirectX::XMVectorSplatY(halfExtentsA));
//		d_B = Dot(Vector3(halfExtentsB), AR1X);
//		NoIntersection = Vector3(DirectX::XMVectorOrInt(NoIntersection, Greater(Abs(d), d_A + d_B)));
//
//		// l = a(w) = (0, 0, 1)
//		// t dot l = t.z
//		// d(A) = h(A).z
//		// d(B) = h(B) dot abs(r20, r21, r22)
//		d   = Vector3(DirectX::XMVectorSplatZ(t));
//		d_A = Vector3(DirectX::XMVectorSplatZ(halfExtentsA));
//		d_B = Dot(Vector3(halfExtentsB), AR2X);
//		NoIntersection = Vector3(DirectX::XMVectorOrInt(NoIntersection, Greater(Abs(d), d_A + d_B)));
//
//		// l = b(u) = (r00, r10, r20)
//		// d(A) = h(A) dot abs(r00, r10, r20)
//		// d(B) = h(B).x
//		d   = Dot(t, RX0);
//		d_A = Dot(Vector3(halfExtentsA), ARX0);
//		d_B = Vector3(DirectX::XMVectorSplatX(halfExtentsB));
//		NoIntersection = Vector3(DirectX::XMVectorOrInt(NoIntersection, Greater(Abs(d), d_A + d_B)));
//
//		// l = b(v) = (r01, r11, r21)
//		// d(A) = h(A) dot abs(r01, r11, r21)
//		// d(B) = h(B).y
//		d   = Dot(t, RX1);
//		d_A = Dot(Vector3(halfExtentsA), ARX1);
//		d_B = Vector3(DirectX::XMVectorSplatY(halfExtentsB));
//		NoIntersection = Vector3(DirectX::XMVectorOrInt(NoIntersection, Greater(Abs(d), d_A + d_B)));
//
//		// l = b(w) = (r02, r12, r22)
//		// d(A) = h(A) dot abs(r02, r12, r22)
//		// d(B) = h(B).z
//		d   = Dot(t, RX2);
//		d_A = Dot(Vector3(halfExtentsA), ARX2);
//		d_B = Vector3(DirectX::XMVectorSplatZ(halfExtentsB));
//		NoIntersection = Vector3(DirectX::XMVectorOrInt(NoIntersection, Greater(Abs(d), d_A + d_B)));
//
//		// l = a(u) x b(u) = (0, -r20, r10)
//		// d(A) = h(A) dot abs(0, r20, r10)
//		// d(B) = h(B) dot abs(0, r02, r01)
//		d   = Dot(t, Vector3(XMVectorPermute<XM_PERMUTE_0W, XM_PERMUTE_1Z, XM_PERMUTE_0Y, XM_PERMUTE_0X>(RX0, XMVectorNegate(RX0))));
//		d_A = Dot(Vector3(halfExtentsA), Vector3(DirectX::XMVectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_X>(ARX0)));
//		d_B = Dot(Vector3(halfExtentsB), Vector3(DirectX::XMVectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_X>(AR0X)));
//		NoIntersection = Vector3(XMVectorOrInt(NoIntersection, Greater(Abs(d), d_A + d_B)));
//
//		// l = a(u) x b(v) = (0, -r21, r11)
//		// d(A) = h(A) dot abs(0, r21, r11)
//		// d(B) = h(B) dot abs(r02, 0, r00)
//		d   = Dot(t, Vector3(XMVectorPermute<XM_PERMUTE_0W, XM_PERMUTE_1Z, XM_PERMUTE_0Y, XM_PERMUTE_0X>(RX1, XMVectorNegate(RX1))));
//		d_A = Dot(Vector3(halfExtentsA), Vector3(DirectX::XMVectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_X>(ARX1)));
//		d_B = Dot(Vector3(halfExtentsB), Vector3(DirectX::XMVectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_X, XM_SWIZZLE_Y>(AR0X)));
//		NoIntersection = Vector3(XMVectorOrInt(NoIntersection, Greater(Abs(d), d_A + d_B)));
//
//		// l = a(u) x b(w) = (0, -r22, r12)
//		// d(A) = h(A) dot abs(0, r22, r12)
//		// d(B) = h(B) dot abs(r01, r00, 0)
//		d   = Dot(t, Vector3(XMVectorPermute<XM_PERMUTE_0W, XM_PERMUTE_1Z, XM_PERMUTE_0Y, XM_PERMUTE_0X>(RX2, XMVectorNegate(RX2))));
//		d_A = Dot(halfExtentsA, Vector3(DirectX::XMVectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_X>(ARX2)));
//		d_B = Dot(halfExtentsB, Vector3(DirectX::XMVectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_W, XM_SWIZZLE_Z>(AR0X)));
//		NoIntersection = Vector3(XMVectorOrInt(NoIntersection, Greater(Abs(d), d_A + d_B)));
//
//		// l = a(v) x b(u) = (r20, 0, -r00)
//		// d(A) = h(A) dot abs(r20, 0, r00)
//		// d(B) = h(B) dot abs(0, r12, r11)
//		d   = Dot(t, Vector3(XMVectorPermute<XM_PERMUTE_0Z, XM_PERMUTE_0W, XM_PERMUTE_1X, XM_PERMUTE_0Y>(RX0, XMVectorNegate(RX0))));
//		d_A = Dot(halfExtentsA, Vector3(DirectX::XMVectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_X, XM_SWIZZLE_Y>(ARX0)));
//		d_B = Dot(halfExtentsB, Vector3(DirectX::XMVectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_X>(AR1X)));
//		NoIntersection = Vector3(XMVectorOrInt(NoIntersection, Greater(Abs(d), d_A + d_B)));
//
//		// l = a(v) x b(v) = (r21, 0, -r01)
//		// d(A) = h(A) dot abs(r21, 0, r01)
//		// d(B) = h(B) dot abs(r12, 0, r10)
//		d = Dot(t, Vector3(XMVectorPermute<XM_PERMUTE_0Z, XM_PERMUTE_0W, XM_PERMUTE_1X, XM_PERMUTE_0Y>(RX1, XMVectorNegate(RX1))));
//		d_A = Dot(halfExtentsA, Vector3(DirectX::XMVectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_X, XM_SWIZZLE_Y>(ARX1)));
//		d_B = Dot(halfExtentsB, Vector3(DirectX::XMVectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_X, XM_SWIZZLE_Y>(AR1X)));
//		NoIntersection = Vector3(XMVectorOrInt(NoIntersection, Greater(Abs(d), d_A + d_B)));
//
//		// l = a(v) x b(w) = (r22, 0, -r02)
//		// d(A) = h(A) dot abs(r22, 0, r02)
//		// d(B) = h(B) dot abs(r11, r10, 0)
//		d = Dot(t, Vector3(XMVectorPermute<XM_PERMUTE_0Z, XM_PERMUTE_0W, XM_PERMUTE_1X, XM_PERMUTE_0Y>(RX2, XMVectorNegate(RX2))));
//		d_A = Dot(halfExtentsA, Vector3(DirectX::XMVectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_X, XM_SWIZZLE_Y>(ARX2)));
//		d_B = Dot(halfExtentsB, Vector3(DirectX::XMVectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_W, XM_SWIZZLE_Z>(AR1X)));
//		NoIntersection = Vector3(XMVectorOrInt(NoIntersection, Greater(Abs(d), d_A + d_B)));
//
//		// l = a(w) x b(u) = (-r10, r00, 0)
//		// d(A) = h(A) dot abs(r10, r00, 0)
//		// d(B) = h(B) dot abs(0, r22, r21)
//		d = Dot(t, Vector3(XMVectorPermute<XM_PERMUTE_1Y, XM_PERMUTE_0X, XM_PERMUTE_0W, XM_PERMUTE_0Z>(RX0, XMVectorNegate(RX0))));
//		d_A = Dot(halfExtentsA, Vector3(DirectX::XMVectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_W, XM_SWIZZLE_Z>(ARX0)));
//		d_B = Dot(halfExtentsB, Vector3(DirectX::XMVectorSwizzle<XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_X>(AR2X)));
//		NoIntersection = Vector3(XMVectorOrInt(NoIntersection, Greater(Abs(d), d_A + d_B)));
//
//		// l = a(w) x b(v) = (-r11, r01, 0)
//		// d(A) = h(A) dot abs(r11, r01, 0)
//		// d(B) = h(B) dot abs(r22, 0, r20)
//		d   = Dot(t, Vector3(XMVectorPermute<XM_PERMUTE_1Y, XM_PERMUTE_0X, XM_PERMUTE_0W, XM_PERMUTE_0Z>(RX1, XMVectorNegate(RX1))));
//		d_A = Dot(halfExtentsA, Vector3(DirectX::XMVectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_W, XM_SWIZZLE_Z>(ARX1)));
//		d_B = Dot(halfExtentsB, Vector3(DirectX::XMVectorSwizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_X, XM_SWIZZLE_Y>(AR2X)));
//		NoIntersection = Vector3(XMVectorOrInt(NoIntersection, Greater(Abs(d), d_A + d_B)));
//
//		// l = a(w) x b(w) = (-r12, r02, 0)
//		// d(A) = h(A) dot abs(r12, r02, 0)
//		// d(B) = h(B) dot abs(r21, r20, 0)
//		d = Dot(t, Vector3(XMVectorPermute<XM_PERMUTE_1Y, XM_PERMUTE_0X, XM_PERMUTE_0W, XM_PERMUTE_0Z>(RX2, XMVectorNegate(RX2))));
//		d_A = Dot(halfExtentsA, Vector3(DirectX::XMVectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_W, XM_SWIZZLE_Z>(ARX2)));
//		d_B = Dot(halfExtentsB, Vector3(DirectX::XMVectorSwizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_W, XM_SWIZZLE_Z>(AR2X)));
//		NoIntersection = Vector3(XMVectorOrInt(NoIntersection, Greater(Abs(d), d_A + d_B)));
//
//		// No seperating axis found, boxes must intersect.
//		if (!XMVector3NotEqualInt(NoIntersection, DirectX::XMVectorTrueInt())) {return false;}
//
//		if (!useExtrusion) { return true; }
//
//		return true;
//	}
//}
//#pragma endregion Each Collision Detection