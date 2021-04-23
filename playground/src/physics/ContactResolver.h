/*
 * ContactResolver.h
 *
 *  Created on: Mar 24, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_CONTACTRESOLVER_H_
#define SRC_PHYSICS_CONTACTRESOLVER_H_

#include "Contact.h"
#include <vector>

class ContactResolver {
protected:
    real calculateSeparatingVelocity(const Contact &contact) const {
        vector relativeVelocity = contact.getParticleA()->getVelocity();

        if(contact.getParticleB() != null) {
            relativeVelocity -= contact.getParticleB()->getVelocity();
        }

        return relativeVelocity * contact.getNormal();
    }

public:

	void resolve(const std::vector<Contact> &contacts) const {
	    for(std::vector<Contact>::const_iterator iterator = contacts.begin(); iterator != contacts.end(); iterator++) {
	        Particle *particleA = (*iterator).getParticleA();
	        Particle *particleB = (*iterator).getParticleB();

	        real separatingVelocity = calculateSeparatingVelocity((*iterator));
	        if(separatingVelocity <= 0.0) {
	            real newSepVelocity = -separatingVelocity * (*iterator).getRestitution();
	            real deltaVelocity = newSepVelocity - separatingVelocity;

	            real totalInverseMass = particleA->getInverseMass();
	            if (particleB) totalInverseMass += particleA->getInverseMass();

	            if(totalInverseMass > 0) {
	                real impulse = deltaVelocity / totalInverseMass;

	                vector impulsePerIMass = (*iterator).getNormal() * impulse;

	                particleA->setVelocity(particleA->getVelocity() + impulsePerIMass * particleA->getInverseMass());
	                if (particleB) {
	                    particleB->setVelocity(particleB->getVelocity() + impulsePerIMass * particleB->getInverseMass());
	                }
	            }
	            particleA->onCollision();

	            if(particleB) {
	                particleB->onCollision();
	            }
	        }
	    }
	}

};




#endif /* SRC_PHYSICS_CONTACTRESOLVER_H_ */
