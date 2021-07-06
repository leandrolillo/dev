/*
 * Geometry.cpp
 *
 *  Created on: Jun 30, 2021
 *      Author: leandro
 */


#include <Geometry.h>
#include <IntersectionTester.h>

bool Sphere::intersects(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const {
    return intersectionTester->intersectsVisit(*this, op2);
}

std::vector<GeometryContact>  Sphere::detectCollision(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const {
    return intersectionTester->detectCollisionVisit(*this, op2);
}

bool Plane::intersects(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const {
    return intersectionTester->intersectsVisit(*this, op2);
}

std::vector<GeometryContact>  Plane::detectCollision(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const {
    return intersectionTester->detectCollisionVisit(*this, op2);
}


bool Line::intersects(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const {
    return intersectionTester->intersectsVisit(*this, op2);
}

std::vector<GeometryContact>  Line::detectCollision(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const {
    return intersectionTester->detectCollisionVisit(*this, op2);
}



