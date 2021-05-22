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
    Logger *logger = LoggerFactory::getLogger("ContactResolver");

protected:
    real calculateSeparatingVelocity(const ParticleContact &contact) const {
        vector relativeVelocity = contact.getParticleA()->getVelocity();

        if(contact.getParticleB() != null) {
            relativeVelocity -= contact.getParticleB()->getVelocity();
        }

        return -relativeVelocity * contact.getNormal();
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
                real impulseAmountPerIMass = deltaVelocity / totalInverseMass;

//                vector impulsePerIMass = contact.getNormal() * (deltaVelocity / totalInverseMass);

                particleA->setVelocity(particleA->getVelocity() - impulseAmountPerIMass * particleA->getInverseMass() * contact.getNormal());
                if (particleB) {
                    particleB->setVelocity(particleB->getVelocity() + impulseAmountPerIMass * particleB->getInverseMass() * contact.getNormal());
                }
            }
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
                vector movePerIMass = contact.getNormal() * (-contact.getPenetration() / totalInverseMass);

                vector deltaA = movePerIMass * particleA->getInverseMass();
                particleA->setPosition(particleA->getPosition() + deltaA);

                logger->info("normal: %s, penetration: %.2f", contact.getNormal().toString().c_str(), contact.getPenetration());
                logger->info("normal * penetration: %s", (contact.getNormal() * contact.getPenetration()).toString().c_str());
                logger->info("Particle A displaced: %s", deltaA.toString().c_str());

                if(particleB != null) {
                    vector deltaB = movePerIMass * particleB->getInverseMass();
                    particleB->setPosition(particleB->getPosition() - deltaB);

                    logger->info("Particle B displaced: %s", deltaB.toString().c_str());
                    logger->info("particle A + particle B displacement: %s", (deltaA + deltaB).toString().c_str());
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
