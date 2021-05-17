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
#include<Contact.h>
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

    virtual std::vector<Contact>detectCollisions(const std::vector<Particle *> &particles) const {
            std::vector<Contact> contacts;

            for(std::vector<Particle *>::const_iterator iteratorA = particles.begin(); iteratorA != particles.end(); iteratorA++) {
                Particle *particleA = *iteratorA;
                if(particleA->getStatus() && particleA->getGeometry() != null) {
                    for(std::vector<const Geometry *>::const_iterator sceneryIterator = scenery.begin();
                            sceneryIterator != scenery.end();
                            sceneryIterator ++)  {
                        if(particleA->getGeometry()->intersects(**sceneryIterator)) {
                            Contact contact = Contact(particleA, null, vector(0, 1, 0), 0.7f, sphere_radius - particleA->getPosition().y);
                            contacts.push_back(contact);

                            particleA->onCollision(contact);
                        }
                    }

                    for(std::vector<Particle *>::const_iterator iteratorB = iteratorA+1; iteratorB != particles.end(); iteratorB++) {
                        Particle *particleB = *iteratorB;
                        if(particleB->getStatus() && particleB->getGeometry() != null) {
                            if(particleA->getGeometry()->intersects(*particleB->getGeometry())) {
                                vector delta = particleB->getPosition() - particleA->getPosition();
                                real modulo = delta.modulo();
                                vector normal = delta * (1.0 / modulo);
                                Contact contact = Contact(particleA, particleB, normal, 1.0f,  2* sphere_radius - modulo);
                                contacts.push_back(contact);

                                particleA->onCollision(contact);
                                particleB->onCollision(contact);
                            }
                        }
                    }
                }
            }

            return contacts;

        }
};


#endif /* SRC_PHYSICS_COLLISIONDETECTOR_H_ */
