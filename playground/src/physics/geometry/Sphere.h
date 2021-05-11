/*
 * Sphere.h
 *
 *  Created on: May 9, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_GEOMETRY_SPHERE_H_
#define SRC_PHYSICS_GEOMETRY_SPHERE_H_

#include<geometry/Geometry.h>

class Sphere : public Geometry {
    real radius;
    vector origin;
public:
    Sphere(const vector &origin, real radius) {
        this->origin = origin;
        this->radius = radius;
    }

    const vector &getOrigin() const {
        return this->origin;
    }

    void setOrigin(const vector &origin) {
        this->origin = origin;
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

    bool intersects(const Line &geometry) const {
        return false;
    }

    bool intersects(const Sphere &geometry) const {
        return false;
    }

    String toString() const {
        return "Sphere(origin: " + this->origin.toString("%.2f") + ", radius: " + std::to_string(this->radius) + ")";
    }
};



#endif /* SRC_PHYSICS_GEOMETRY_SPHERE_H_ */
