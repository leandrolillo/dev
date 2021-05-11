/*
 * Line.h
 *
 *  Created on: May 9, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_GEOMETRY_LINE_H_
#define SRC_PHYSICS_GEOMETRY_LINE_H_

#include<geometry/Geometry.h>
#include<collisionDetection/IntersectionTester.h>

class Line : public Geometry {
    vector origin;
    vector direction;
public:
    Line(const vector &origin, const vector &direction) {
        this->origin = origin;
        this->direction = direction;
    }

    const vector &getOrigin() const {
        return this->origin;
    }

    const vector &getDirection() const {
        return this->direction;
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
    bool intersects(const Line &line) const;

    bool intersects(const Sphere &sphere) const;

    String toString() const {
        return "Line(origin: " + this->origin.toString("%.2f") + ", dir: " + this->direction.toString("%.2f") + ")";
    }
};

bool Line::intersects(const Line &line) const {
        return false;
    }

    bool Line::intersects(const Sphere &sphere) const {
        return IntersectionTester::areIntersecting(this->getOrigin(), this->getDirection(), sphere.getOrigin(), sphere.getRadius());
    }



#endif /* SRC_PHYSICS_GEOMETRY_LINE_H_ */
