/*
 * Sphere.h
 *
 *  Created on: May 17, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_GEOMETRY_SPHERE_H_
#define SRC_PHYSICS_GEOMETRY_SPHERE_H_

#include<Geometry.h>

class Sphere : public AbstractSphere {
    vector origin;

public:
    Sphere(const vector &origin, real radius) : AbstractSphere(radius) {
        this->origin = origin;
    }
    const vector& getOrigin() const {
        return origin;
    }

    void setOrigin(const vector &origin) {
        this->origin = origin;
    }
};




#endif /* SRC_PHYSICS_GEOMETRY_SPHERE_H_ */
