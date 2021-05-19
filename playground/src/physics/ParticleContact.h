/*
 * Contact.h
 *
 *  Created on: Mar 24, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_PARTICLE_CONTACT_H_
#define SRC_PHYSICS_PARTICLE_CONTACT_H_

#include<BaseContact.h>
#include<Particle.h>

class ParticleContact : public BaseContact {
public:
    static ParticleContact noContact;
private:
    Particle *particleA;
    Particle *particleB;

public:
    ParticleContact(Particle *particleA, Particle *particleB, vector normal, real restitution, real penetration = 0.0) : BaseContact(normal, restitution, penetration){
        this->particleA = particleA;
        this->particleB = particleB;
    }

    Particle *getParticleA() const {
        return this->particleA;
    }

    Particle *getParticleB() const {
        return this->particleB;
    }
};

#endif /* SRC_PHYSICS_CONTACT_H_ */
