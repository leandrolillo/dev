/*
 * Line.h
 *
 *  Created on: May 16, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_GEOMETRY_PLANE_H_
#define SRC_PHYSICS_GEOMETRY_PLANE_H_

#include<Geometry.h>

class Plane : public AbstractPlane {
    vector origin;

public:
    Plane(const vector &origin, const vector &normal) : AbstractPlane(normal) {
        setOrigin(origin);
    }

    const vector& getOrigin() const {
        return origin;
    }
    void setOrigin(const vector &origin) {
        this->origin = origin;
    }
};



#endif /* SRC_PHYSICS_GEOMETRY_LINE_H_ */
