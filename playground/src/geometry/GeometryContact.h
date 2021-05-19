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
    const Geometry *particleA;
    const Geometry *particleB;

public:
    GeometryContact(const Geometry *particleA, const Geometry *particleB, vector normal, real restitution, real penetration = 0.0) : BaseContact (normal, restitution, penetration){
        this->particleA = particleA;
        this->particleB = particleB;
    }

    const Geometry *getParticleA() const {
        return this->particleA;
    }

    const Geometry *getParticleB() const {
        return this->particleB;
    }
};

GeometryContact GeometryContact::noContact(null, null, vector(0, 0, 0), 0.0, -1.0);
#endif /* SRC_PHYSICS_COLLISIONDETECTION_GEOMETRY_GEOMETRYCONTACT_H_ */
