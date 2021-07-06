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
bool SphereIntersectionTester::intersectsVisit(const Plane &plane, const Geometry &sphere) const {
    return IntersectionHelper::planeSphere(plane, (const Sphere&) sphere);
}

bool SphereIntersectionTester::intersectsVisit(const Line &line, const Geometry &sphere) const {
    return IntersectionHelper::lineSphere(line, (const Sphere&) sphere);
}

bool SphereIntersectionTester::intersectsVisit(const Sphere &sphere, const Geometry &anotherSphere) const {
    return IntersectionHelper::sphereSphere(sphere, (Sphere&) anotherSphere);
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

bool PlaneIntersectionTester::intersectsVisit(const Plane &plane, const Geometry &anotherPlane) const {
    return IntersectionHelper::planePlane(plane, (const Plane&) anotherPlane);
}

bool PlaneIntersectionTester::intersectsVisit(const Line &line, const Geometry &plane) const {
    return IntersectionHelper::linePlane(line, (const Plane&) plane);
}

bool PlaneIntersectionTester::intersectsVisit(const Sphere &sphere, const Geometry &plane) const {
    return IntersectionHelper::planeSphere((const Plane&) plane, sphere);
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

bool LineIntersectionTester::intersectsVisit(const Plane &plane, const Geometry &line) const {
    return IntersectionHelper::linePlane((const Line&) line, plane);
}

bool LineIntersectionTester::intersectsVisit(const Line &anotherLine, const Geometry &line) const {
    return IntersectionHelper::lineLine(anotherLine, (const Line&) line);
}

bool LineIntersectionTester::intersectsVisit(const Sphere &sphere, const Geometry &line) const {
    return IntersectionHelper::lineSphere((const Line&) line, sphere);
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

