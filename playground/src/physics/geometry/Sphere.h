/*
 * Sphere.h
 *
 *  Created on: May 9, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_GEOMETRY_SPHERE_H_
#define SRC_PHYSICS_GEOMETRY_SPHERE_H_

#include<geometry/Geometry.h>
#include<collisionDetection/IntersectionTester.h>

class Sphere : public Geometry {
    real radius;
    vector origin;
public:
    Sphere(const vector &origin, real radius) {
        this->origin = origin;
        this->radius = radius;
    }

    void setOrigin(const vector &origin) {
        this->origin = origin;
    }

    const vector &getOrigin() const {
        return this->origin;
    }

    real getRadius() const {
        return this->radius;
    }

    /**
     * "Accept" method in visitor pattern / double dispatch
     */

    bool intersects(const Geometry &geometry) const {
        return geometry.intersects(*this);
    }

    /**
     * "Visit" methods in visitor pattern / double dispatch
     */

    bool intersects(const Line &line) const {
        //return IntersectionTester::areIntersecting(line.getOrigin(), line.getDirection(), this->getOrigin(), this->getRadius());
        return false;
    }

    bool intersects(const Sphere &sphere) const {
        return IntersectionTester::areIntersecting(sphere.getOrigin(), sphere.getRadius(), this->getOrigin(), this->getRadius());
    }

    String toString() const {
        return "Sphere(origin: " + this->origin.toString("%.2f") + ", radius: " + std::to_string(this->radius) + ")";
    }
};
#endif /* SRC_PHYSICS_GEOMETRY_SPHERE_H_ */
