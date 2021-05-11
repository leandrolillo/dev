/*
 * Plane.h
 *
 *  Created on: May 9, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_GEOMETRY_PLANE_H_
#define SRC_PHYSICS_GEOMETRY_PLANE_H_

#include<geometry/Geometry.h>

class Plane : public Geometry {
    vector origin;
    vector normal;

    /**
     * "Accept" method in visitor pattern / double dispatch
     */
    bool intersects(const Geometry &geometry) const {
        return geometry.intersects(*this);
    }
};



#endif /* SRC_PHYSICS_GEOMETRY_PLANE_H_ */
