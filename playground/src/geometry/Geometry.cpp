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

bool HierarchicalGeometry::intersects(const IntersectionTesterBase *intersectionTesterBase, const Geometry &op2) const {
    IntersectionTester intersectionTester;
    if(intersectionTester.intersects(*this->boundingGeometry, op2)) {
        for(auto const &childGeometry : children) {
            if(intersectionTester.intersects(*childGeometry.get(), op2)) {
                return true;
            }
        }
    }

    return false;
}

std::vector<GeometryContact>  HierarchicalGeometry::detectCollision(const IntersectionTesterBase *intersectionTesterBase, const Geometry &op2) const {
    std::vector<GeometryContact> result;

    IntersectionTester intersectionTester;
    if(intersectionTester.intersects(*this->boundingGeometry, op2)) {
        for(auto const &childGeometry : children) {
            std::vector<GeometryContact> childrenContacts = intersectionTester.detectCollision(*childGeometry, op2);
            result.insert(std::end(result), std::begin(childrenContacts), std::end(childrenContacts));
        }
    }

    return result;
}


