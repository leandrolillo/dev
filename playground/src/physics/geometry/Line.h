/*
 * Line.h
 *
 *  Created on: May 9, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_GEOMETRY_LINE_H_
#define SRC_PHYSICS_GEOMETRY_LINE_H_

#include<geometry/Geometry.h>

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
    bool intersects(const Line &sphere) const {
        return false;
    }

    bool intersects(const Sphere &sphere) const {
        printf("-------------------------\n");
        printf("Testing %s vs %s\n", this->toString().c_str(), sphere.toString().c_str());
        vector translation = sphere.getOrigin() - this->origin;
        real projection = translation * this->direction;
        vector projectedSphereCenter = this->origin + this->direction * projection;

        vector lineToSphere = sphere.getOrigin() - projectedSphereCenter;

        printf("Shortest vector from line to sphere center: %s\n", lineToSphere.toString().c_str());
        printf("sphere distance to line 2: %.2f, radius2: %.2f\n", lineToSphere * lineToSphere, sphere.getRadius() * sphere.getRadius());


        return (lineToSphere * lineToSphere <= sphere.getRadius() * sphere.getRadius());
    }

    String toString() const {
        return "Line(origin: " + this->origin.toString("%.2f") + ", dir: " + this->direction.toString("%.2f") + ")";
    }
};



#endif /* SRC_PHYSICS_GEOMETRY_LINE_H_ */
