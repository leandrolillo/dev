/*
 * IntersectionTester.cpp
 *
 *  Created on: Jun 30, 2021
 *      Author: leandro
 */

#include<IntersectionTester.h>
#include<IntersectionHelper.h>


/**
 * Sphere
 */

bool SphereIntersectionTester::intersectsVisit(const HierarchicalGeometry &hierarchy, const Geometry &sphere) const {
    return IntersectionHelper::sphereHierarchy((const Sphere&) sphere, hierarchy);
}

bool SphereIntersectionTester::intersectsVisit(const AABB &aabb, const Geometry &sphere) const {
    return IntersectionHelper::sphereAabb((const Sphere&) sphere, aabb);
}

bool SphereIntersectionTester::intersectsVisit(const Plane &plane, const Geometry &sphere) const {
    return IntersectionHelper::planeSphere(plane, (const Sphere&) sphere);
}

bool SphereIntersectionTester::intersectsVisit(const Line &line, const Geometry &sphere) const {
    return IntersectionHelper::lineSphere(line, (const Sphere&) sphere);
}

bool SphereIntersectionTester::intersectsVisit(const Sphere &anotherSphere, const Geometry &sphere) const {
    return IntersectionHelper::sphereSphere(anotherSphere, (Sphere&) sphere);
}


std::vector<GeometryContact> SphereIntersectionTester::detectCollisionVisit(const HierarchicalGeometry &hierarchy, const Geometry &sphere) const {
    return IntersectionHelper::sphereHierarchyContact((const Sphere&) sphere, hierarchy);
}

std::vector<GeometryContact>  SphereIntersectionTester::detectCollisionVisit(const AABB &aabb, const Geometry &sphere) const {
    return IntersectionHelper::sphereAabbContact((const Sphere &)sphere, aabb);
}
std::vector<GeometryContact>  SphereIntersectionTester::detectCollisionVisit(const Sphere &anotherSphere, const Geometry &sphere) const {
    return IntersectionHelper::sphereSphereContact(anotherSphere, (Sphere &)sphere);
}
std::vector<GeometryContact>  SphereIntersectionTester::detectCollisionVisit(const Plane &plane, const Geometry &sphere) const {
    return IntersectionHelper::planeSphereContact(plane, (Sphere &)sphere);
}
std::vector<GeometryContact>  SphereIntersectionTester::detectCollisionVisit(const Line &line, const Geometry &sphere) const {
    return IntersectionHelper::lineSphereContact(line, (Sphere &)sphere);
}


/**
 * Plane
 */

bool PlaneIntersectionTester::intersectsVisit(const HierarchicalGeometry &hierarchy, const Geometry &plane) const {
    return IntersectionHelper::planeHierarchy((const Plane&) plane, hierarchy);
}

bool PlaneIntersectionTester::intersectsVisit(const AABB &aabb, const Geometry &plane) const {
    return IntersectionHelper::planeAabb((const Plane&) plane, aabb);
}

bool PlaneIntersectionTester::intersectsVisit(const Plane &anotherPlane, const Geometry &plane) const {
    return IntersectionHelper::planePlane(anotherPlane, (const Plane&) plane);
}

bool PlaneIntersectionTester::intersectsVisit(const Line &line, const Geometry &plane) const {
    return IntersectionHelper::linePlane(line, (const Plane&) plane);
}

bool PlaneIntersectionTester::intersectsVisit(const Sphere &sphere, const Geometry &plane) const {
    return IntersectionHelper::planeSphere((const Plane&) plane, sphere);
}

std::vector<GeometryContact> PlaneIntersectionTester::detectCollisionVisit(const HierarchicalGeometry &hierarchy, const Geometry &plane) const {
    return IntersectionHelper::planeHierarchyContact((const Plane&) plane, hierarchy);
}

std::vector<GeometryContact>  PlaneIntersectionTester::detectCollisionVisit(const AABB &aabb, const Geometry &plane) const {
    return IntersectionHelper::planeAabbContact((const Plane&) plane, aabb);
}

std::vector<GeometryContact>  PlaneIntersectionTester::detectCollisionVisit(const Sphere &sphere, const Geometry &plane) const {
    return IntersectionHelper::planeSphereContact((const Plane&) plane, sphere);
}
std::vector<GeometryContact>  PlaneIntersectionTester::detectCollisionVisit(const Plane &anotherPlane, const Geometry &plane) const {
    return IntersectionHelper::planePlaneContact(anotherPlane, (const Plane&) plane);
}
std::vector<GeometryContact>  PlaneIntersectionTester::detectCollisionVisit(const Line &line, const Geometry &plane) const {
    return IntersectionHelper::linePlaneContact(line, (const Plane&) plane);
}

/**
 * Line
 */
bool LineIntersectionTester::intersectsVisit(const HierarchicalGeometry &hierarchy, const Geometry &line) const {
    return IntersectionHelper::lineHierarchy((const Line&) line, hierarchy);
}

bool LineIntersectionTester::intersectsVisit(const AABB &aabb, const Geometry &line) const {
    return IntersectionHelper::lineAabb((const Line&) line, aabb);
}

bool LineIntersectionTester::intersectsVisit(const Plane &plane, const Geometry &line) const {
    return IntersectionHelper::linePlane((const Line&) line, plane);
}

bool LineIntersectionTester::intersectsVisit(const Line &anotherLine, const Geometry &line) const {
    return IntersectionHelper::lineLine(anotherLine, (const Line&) line);
}

bool LineIntersectionTester::intersectsVisit(const Sphere &sphere, const Geometry &line) const {
    return IntersectionHelper::lineSphere((const Line&) line, sphere);
}

std::vector<GeometryContact> LineIntersectionTester::detectCollisionVisit(const HierarchicalGeometry &hierarchy, const Geometry &line) const {
    return IntersectionHelper::lineHierarchyContact((const Line&) line, hierarchy);
}

std::vector<GeometryContact>  LineIntersectionTester::detectCollisionVisit(const AABB &aabb, const Geometry &line) const {
    return IntersectionHelper::lineAabbContact((const Line&) line, aabb);
}

std::vector<GeometryContact>  LineIntersectionTester::detectCollisionVisit(const Sphere &sphere, const Geometry &line) const {
    return IntersectionHelper::lineSphereContact((const Line&) line, sphere);
}
std::vector<GeometryContact>  LineIntersectionTester::detectCollisionVisit(const Plane &plane, const Geometry &line) const {
    return IntersectionHelper::linePlaneContact((const Line&) line, plane);
}
std::vector<GeometryContact>  LineIntersectionTester::detectCollisionVisit(const Line &anotherLine, const Geometry &line) const {
    return IntersectionHelper::lineLineContact((const Line&) line, anotherLine);
}

/**
 * AABB
 */

bool AABBIntersectionTester::intersectsVisit(const HierarchicalGeometry &hierarchy, const Geometry &aabb) const {
    return IntersectionHelper::aabbHierarchy((const AABB&) aabb, hierarchy);
}

bool AABBIntersectionTester::intersectsVisit(const AABB &anotherAabb, const Geometry &aabb) const {
    return IntersectionHelper::aabbAabb(anotherAabb, (const AABB&) aabb);
}

bool AABBIntersectionTester::intersectsVisit(const Plane &plane, const Geometry &aabb) const {
    return IntersectionHelper::planeAabb(plane, (const AABB&) aabb);
}

bool AABBIntersectionTester::intersectsVisit(const Line &line, const Geometry &aabb) const {
    return IntersectionHelper::lineAabb(line, (const AABB&) aabb);
}

bool AABBIntersectionTester::intersectsVisit(const Sphere &sphere, const Geometry &aabb) const {
    return IntersectionHelper::sphereAabb(sphere, (const AABB&) aabb);
}

std::vector<GeometryContact> AABBIntersectionTester::detectCollisionVisit(const HierarchicalGeometry &hierarchy, const Geometry &aabb) const {
    return IntersectionHelper::aabbHierarchyContact((const AABB&) aabb, hierarchy);
}

std::vector<GeometryContact>  AABBIntersectionTester::detectCollisionVisit(const AABB &anotherAabb, const Geometry &aabb) const {
    return IntersectionHelper::aabbAabbContact(anotherAabb, (const AABB&) aabb);
}

std::vector<GeometryContact>  AABBIntersectionTester::detectCollisionVisit(const Sphere &sphere, const Geometry &aabb) const {
    return IntersectionHelper::sphereAabbContact(sphere, (const AABB&) aabb);
}
std::vector<GeometryContact>  AABBIntersectionTester::detectCollisionVisit(const Plane &plane, const Geometry &aabb) const {
    return IntersectionHelper::planeAabbContact(plane, (const AABB&) aabb);
}
std::vector<GeometryContact>  AABBIntersectionTester::detectCollisionVisit(const Line &line, const Geometry &aabb) const {
    return IntersectionHelper::lineAabbContact(line, (const AABB&) aabb);
}

/**
 * Hierarchical
 */

bool HierarchyIntersectionTester::intersectsVisit(const HierarchicalGeometry &anotherHierarchy, const Geometry &hierarchy) const {
    return IntersectionHelper::hierarchyHierarchy(anotherHierarchy, (const HierarchicalGeometry&) hierarchy);
}

bool HierarchyIntersectionTester::intersectsVisit(const AABB &aabb, const Geometry &hierarchy) const {
    return IntersectionHelper::aabbHierarchy(aabb, (const HierarchicalGeometry&) hierarchy);
}

bool HierarchyIntersectionTester::intersectsVisit(const Plane &plane, const Geometry &hierarchy) const {
    return IntersectionHelper::planeHierarchy(plane, (const HierarchicalGeometry &) hierarchy);
}

bool HierarchyIntersectionTester::intersectsVisit(const Line &line, const Geometry &hierarchy) const {
    return IntersectionHelper::lineHierarchy(line, (const HierarchicalGeometry&) hierarchy);
}

bool HierarchyIntersectionTester::intersectsVisit(const Sphere &sphere, const Geometry &hierarchy) const {
    return IntersectionHelper::sphereHierarchy(sphere, (const HierarchicalGeometry&) hierarchy);
}

std::vector<GeometryContact> HierarchyIntersectionTester::detectCollisionVisit(const HierarchicalGeometry &anotherHierarchy, const Geometry &hierarchy) const {
    return IntersectionHelper::hierarchyHierarchyContact(anotherHierarchy, (const HierarchicalGeometry &) hierarchy);
}

std::vector<GeometryContact>  HierarchyIntersectionTester::detectCollisionVisit(const AABB &aabb, const Geometry &hierarchy) const {
    return IntersectionHelper::aabbHierarchyContact(aabb, (const HierarchicalGeometry&) hierarchy);
}

std::vector<GeometryContact>  HierarchyIntersectionTester::detectCollisionVisit(const Sphere &sphere, const Geometry &hierarchy) const {
    return IntersectionHelper::sphereHierarchyContact(sphere, (const HierarchicalGeometry&) hierarchy);
}
std::vector<GeometryContact>  HierarchyIntersectionTester::detectCollisionVisit(const Plane &plane, const Geometry &hierarchy) const {
    return IntersectionHelper::planeHierarchyContact(plane, (const HierarchicalGeometry&) hierarchy);
}
std::vector<GeometryContact>  HierarchyIntersectionTester::detectCollisionVisit(const Line &line, const Geometry &hierarchy) const {
    return IntersectionHelper::lineHierarchyContact(line, (const HierarchicalGeometry&) hierarchy);
}
