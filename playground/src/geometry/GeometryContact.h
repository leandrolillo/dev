/*
 * GeometryContact.h
 *
 *  Created on: May 18, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_GEOMETRY_GEOMETRYCONTACT_H_
#define SRC_PHYSICS_GEOMETRY_GEOMETRYCONTACT_H_

#include<BaseContact.h>
#include<Geometry.h>

class Geometry;

class GeometryContact : public BaseContact {
public:
    static GeometryContact noContact;
private:
    const Geometry *geometryA;
    const Geometry *geometryB;

public:
    GeometryContact(const Geometry *geometryA, const Geometry *geometryB, const vector &intersection, const vector &normal, real restitution, real penetration = 0.0) : BaseContact (intersection, normal, restitution, penetration){
        this->geometryA = geometryA;
        this->geometryB = geometryB;
    }

    const Geometry *getGeometryA() const {
        return this->geometryA;
    }

    const Geometry *getGeometryB() const {
        return this->geometryB;
    }
};

GeometryContact GeometryContact::noContact(null, null, vector(0, 0, 0), vector(0, 0, 0), 0.0, -1.0);
#endif /* SRC_PHYSICS_COLLISIONDETECTION_GEOMETRY_GEOMETRYCONTACT_H_ */
