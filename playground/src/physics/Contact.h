/*
 * Contact.h
 *
 *  Created on: Mar 24, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_CONTACT_H_
#define SRC_PHYSICS_CONTACT_H_

class Contact {
private:
    Particle *particleA;
    Particle *particleB;
    vector normal;
    real penetration;
    real restitution;

public:
    Contact(Particle *particleA, Particle *particleB, vector normal, real restitution, real penetration = 0.0) {
        this->particleA = particleA;
        this->particleB = particleB;
        this->normal = normal;
        this->penetration = penetration;
        this->restitution = restitution;
    }

    Particle *getParticleA() const {
        return this->particleA;
    }

    Particle *getParticleB() const {
        return this->particleB;
    }

    const vector &getNormal() const {
        return this->normal;
    }

    real getPenetration() const {
        return this->penetration;
    }

    real getRestitution() const {
        return this->restitution;
    }
};



#endif /* SRC_PHYSICS_CONTACT_H_ */
