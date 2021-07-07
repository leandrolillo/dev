/*
 * Geometry.cpp
 *
 *  Created on: Jun 30, 2021
 *      Author: leandro
 */


#include <Geometry.h>
#include <IntersectionTester.h>

/**
 * Sphere
 */
bool Sphere::intersects(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const {
    return intersectionTester->intersectsVisit(*this, op2);
}

std::vector<GeometryContact>  Sphere::detectCollision(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const {
    return intersectionTester->detectCollisionVisit(*this, op2);
}

/**
 * Plane
 */

bool Plane::intersects(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const {
    return intersectionTester->intersectsVisit(*this, op2);
}

std::vector<GeometryContact>  Plane::detectCollision(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const {
    return intersectionTester->detectCollisionVisit(*this, op2);
}


/**
 * Line
 */

bool Line::intersects(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const {
    return intersectionTester->intersectsVisit(*this, op2);
}

std::vector<GeometryContact>  Line::detectCollision(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const {
    return intersectionTester->detectCollisionVisit(*this, op2);
}

/**
 * AABB
 */

bool AABB::intersects(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const {
    return intersectionTester->intersectsVisit(*this, op2);
}

std::vector<GeometryContact>  AABB::detectCollision(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const {
    return intersectionTester->detectCollisionVisit(*this, op2);
}


/**
 * HierarchicalGeometry
 */

//bool HierarchicalGeometry::intersects(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const {
//    return false;
//}
//
//std::vector<GeometryContact>  HierarchicalGeometry::detectCollision(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const {
//    return std::vector<GeometryContact>();
//}


