/*
 * Sphere.h
 *
 *  Created on: May 16, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_GEOMETRY_BOUNDINGSPHERE_H_
#define SRC_PHYSICS_GEOMETRY_BOUNDINGSPHERE_H_

#include<Geometry.h>

class LinkedSphere : public AbstractSphere {
    vector &origin;

public:
    LinkedSphere(vector &origin, real radius) : AbstractSphere(radius), origin(origin) {
    }
    const vector& getOrigin() const {
        return origin;
    }
};




#endif /* SRC_PHYSICS_GEOMETRY_BOUNDINGSPHERE_H_ */
