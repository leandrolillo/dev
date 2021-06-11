/*
 * CollisionDetector.h
 *
 *  Created on: Apr 5, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_COLLISIONDETECTOR_H_
#define SRC_PHYSICS_COLLISIONDETECTOR_H_

#define sphere_radius 0.1

#include<vector>
#include<ParticleContact.h>
#include<Particle.h>

#include <Geometry.h>


class CollisionDetector {
    std::vector <const Geometry *>scenery;
public:
    virtual ~CollisionDetector() {
    }

    void addScenery(const Geometry *scenery) {
        this->scenery.push_back(scenery);
    }

    virtual std::vector<ParticleContact>detectCollisions(const std::vector<Particle *> &particles) const {
            std::vector<ParticleContact> contacts;

            for(std::vector<Particle *>::const_iterator iteratorA = particles.begin(); iteratorA != particles.end(); iteratorA++) {
                Particle *particleA = *iteratorA;
                if(particleA->getStatus() && particleA->getGeometry() != null) {
                    for(std::vector<const Geometry *>::const_iterator sceneryIterator = scenery.begin();
                            sceneryIterator != scenery.end();
                            sceneryIterator ++)  {

                        GeometryContact contact = particleA->getGeometry()->detectCollision(**sceneryIterator);
                        if(contact.isIntersecting()) {
                            contacts.push_back(
                                    ParticleContact(particleA,
                                            null,
                                            contact.getIntersection(),
                                            contact.getNormal(),
                                            contact.getRestitution(),
                                            contact.getPenetration())
                            );

                            particleA->onCollision(contacts.back());
                        }
                    }

                    for(std::vector<Particle *>::const_iterator iteratorB = iteratorA+1; iteratorB != particles.end(); iteratorB++) {
                        Particle *particleB = *iteratorB;
                        if(particleB->getStatus() && particleB->getGeometry() != null) {
                            GeometryContact contact = particleA->getGeometry()->detectCollision(*particleB->getGeometry());
                            if(contact.isIntersecting()) {
                                contacts.push_back(
                                        ParticleContact(particleA,
                                                particleB,
                                                contact.getIntersection(),
                                                contact.getNormal(),
                                                contact.getRestitution(),
                                                contact.getPenetration())
                                );

                                particleA->onCollision(contacts.back());
                                particleB->onCollision(contacts.back());
                            }
                        }
                    }
                }
            }

            return contacts;

        }
};


#endif /* SRC_PHYSICS_COLLISIONDETECTOR_H_ */
