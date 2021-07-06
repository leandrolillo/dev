/*
 * IntersectionHelper.h
 *
 *  Created on: Jun 30, 2021
 *      Author: leandro
 */

#ifndef SRC_GEOMETRY_INTERSECTIONHELPER_H_
#define SRC_GEOMETRY_INTERSECTIONHELPER_H_

#include <vector>
#include <Geometry.h>
#include <GeometryContact.h>


class IntersectionHelper {
public:
    static bool lineSphere(const Line &line, const Sphere &sphere) {
       real projection = (sphere.getOrigin() - line.getOrigin()) * line.getDirection();
       vector projectedSphereCenter = line.getOrigin() + line.getDirection() * projection;
       vector lineToSphere = sphere.getOrigin() - projectedSphereCenter;

       return (lineToSphere * lineToSphere <= sphere.getRadius() * sphere.getRadius());
    }

    static bool linePlane(const Line &line, const Plane &plane) {
       return false;
    }

    static bool lineLine(const Line &line, const Line &anotherLine) {
       return false;
    }

    static bool sphereSphere(const Sphere &sphere, const Sphere &anotherSphere) {
        vector delta = sphere.getOrigin() - anotherSphere.getOrigin();
        real radiuses = sphere.getRadius() + anotherSphere.getRadius();

        return (delta * delta <= radiuses * radiuses);
    }

    static bool planeSphere(const Plane &plane, const Sphere &sphere) {
        vector delta = ((sphere.getOrigin() - plane.getOrigin()) * plane.getNormal()) * plane.getNormal();
        return (delta * delta <= sphere.getRadius() * sphere.getRadius());
    }

    static bool planePlane(const Plane &plane, const Plane &anotherPlane) {
        return false;
    }

    static std::vector<GeometryContact> lineSphereContact(const Line &line, const Sphere &sphere) {
       return std::vector<GeometryContact>();
    }

    static std::vector<GeometryContact> linePlaneContact(const Line &line, const Plane &plane) {
        return std::vector<GeometryContact>();
    }

    static std::vector<GeometryContact> lineLineContact(const Line &line, const Line &anotherLine) {
        return std::vector<GeometryContact>();
    }

    static std::vector<GeometryContact> sphereSphereContact(const Sphere &sphereA, const Sphere &sphereB) {
        vector delta = sphereB.getOrigin() - sphereA.getOrigin();
        real radiuses = sphereA.getRadius() + sphereB.getRadius();

        if(delta * delta <= radiuses * radiuses) {
            real distance = delta.modulo();
            vector normal = delta * (1.0 / distance);
            real penetration = radiuses - distance;
            vector intersection = sphereA.getOrigin() + (normal * sphereA.getRadius());
            return std::vector<GeometryContact> {GeometryContact(&sphereA, &sphereB, intersection, normal, 0.8f,  penetration) };
        }

        return std::vector<GeometryContact>();
    }

    static std::vector<GeometryContact> planeSphereContact(const Plane &plane, const Sphere &sphere) {
        vector normal = plane.getNormal();

        real distance = (sphere.getOrigin() - plane.getOrigin()) * normal;

        if(distance <= sphere.getRadius()) {
            vector delta = distance * normal;
            real penetration = sphere.getRadius() - distance;
            vector intersection = sphere.getOrigin() - (normal * sphere.getRadius());

            return std::vector<GeometryContact> {GeometryContact(&plane, &sphere, intersection, normal, 0.8f, penetration) };
        }

        return std::vector<GeometryContact>();
    }

    static std::vector<GeometryContact> planePlaneContact(const Plane &plane, const Plane &anotherPlane) {
        return std::vector<GeometryContact>();
    }
};



#endif /* SRC_GEOMETRY_INTERSECTIONHELPER_H_ */
