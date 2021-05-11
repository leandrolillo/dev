/*
 * Geometry.h
 *
 *  Created on: May 9, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_GEOMETRY_GEOMETRY_H_
#define SRC_PHYSICS_GEOMETRY_GEOMETRY_H_

#include<Math3d.h>

class Line;
class Sphere;

class Geometry {
public:

    /**
     * "Accept" method in visitor pattern / double dispatch
     */
    virtual bool intersects(const Geometry &geometry) const = 0;

    /**
     * "Visit" method in visitor pattern
     */

    virtual bool intersects(const Line &line) const = 0;
    virtual bool intersects(const Sphere &sphere) const = 0;

    virtual String toString() const {
        return "Geometry";
    }
};



#endif /* SRC_PHYSICS_GEOMETRY_GEOMETRY_H_ */
