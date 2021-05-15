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

#include <geometry/Geometry.h>


class CollisionDetector {

public:

    virtual std::vector<Contact>detectCollisions(const std::vector<Particle *> &particles) const {
            std::vector<Contact> contacts;

            for(std::vector<Particle *>::const_iterator particleA = particles.begin(); particleA != particles.end(); particleA++) {
                for(std::vector<Particle *>::const_iterator particleB = particleA+1; particleB != particles.end(); particleB++) {
                    if((*particleA)->getStatus() && (*particleB)->getStatus()) {
                        if(IntersectionTester::sphereSphere((*particleA)->getPosition(), sphere_radius, (*particleB)->getPosition(), sphere_radius)) {
                            vector delta = (*particleB)->getPosition() - (*particleA)->getPosition();
                            real modulo = delta.modulo();
                            vector normal = delta * (1.0 / modulo);
                            contacts.push_back(Contact(*particleA, *particleB, normal, 1.0f,  2* sphere_radius - modulo));
                        }
                    }
                }
            }

            return contacts;

        }
};


#endif /* SRC_PHYSICS_COLLISIONDETECTOR_H_ */
