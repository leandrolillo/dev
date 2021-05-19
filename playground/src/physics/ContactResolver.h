/*
 * ContactResolver.h
 *
 *  Created on: Mar 24, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_CONTACTRESOLVER_H_
#define SRC_PHYSICS_CONTACTRESOLVER_H_

#include "ParticleContact.h"
#include <vector>

class ContactResolver {
protected:
    real calculateSeparatingVelocity(const ParticleContact &contact) const {
        vector relativeVelocity = contact.getParticleA()->getVelocity();

        if(contact.getParticleB() != null) {
            relativeVelocity -= contact.getParticleB()->getVelocity();
        }

        return relativeVelocity * contact.getNormal();
    }

public:

    void resolveVelocity(const ParticleContact &contact) const {
        Particle *particleA = contact.getParticleA();
        Particle *particleB = contact.getParticleB();

        real separatingVelocity = calculateSeparatingVelocity(contact);
        if(separatingVelocity <= 0.0) {
            real newSepVelocity = -separatingVelocity * contact.getRestitution();
            real deltaVelocity = newSepVelocity - separatingVelocity;

            real totalInverseMass = particleA->getInverseMass();
            if (particleB) {
                totalInverseMass += particleB->getInverseMass();
            }

            if(totalInverseMass > 0) {
                real impulsePerIMass = deltaVelocity / totalInverseMass;

                particleA->setVelocity(particleA->getVelocity() + contact.getNormal() * impulsePerIMass * particleA->getInverseMass());
                if (particleB) {
                    particleB->setVelocity(particleB->getVelocity() - contact.getNormal() * impulsePerIMass * particleB->getInverseMass());
                }
            }
//            particleA->onCollision();
//
//            if(particleB) {
//                particleB->onCollision();
//            }
        }
    }

    void resolveInterpenetration(const ParticleContact &contact) const {
        if(contact.getPenetration() > (real)0) {
            Particle *particleA = contact.getParticleA();
            Particle *particleB = contact.getParticleB();

            real totalInverseMass = particleA->getInverseMass();

            if(particleB != null) {
                totalInverseMass += particleB->getInverseMass();
            }

            if(totalInverseMass > (real)0) {
                particleA->setPosition(particleA->getPosition() + contact.getNormal() * (contact.getPenetration() * particleA->getInverseMass() / totalInverseMass));

                if(particleB != null) {
                    particleB->setPosition(particleB->getPosition() + contact.getNormal() * (contact.getPenetration() * particleB->getInverseMass() / totalInverseMass));
                }
            }
        }

    }

	void resolve(const std::vector<ParticleContact> &contacts) const {
	    for(std::vector<ParticleContact>::const_iterator iterator = contacts.begin(); iterator != contacts.end(); iterator++) {
	        resolveVelocity(*iterator);
	        resolveInterpenetration(*iterator);

	        Particle *particleA = (*iterator).getParticleA();
	        Particle *particleB = (*iterator).getParticleB();

            particleA->onCollisionResolved(*iterator);

            if(particleB) {
                particleB->onCollisionResolved(*iterator);
            }
        }
    }

};




#endif /* SRC_PHYSICS_CONTACTRESOLVER_H_ */
