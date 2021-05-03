/*
 * SpheresCollisionDetector.h
 *
 *  Created on: Apr 29, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_COLLISIONDETECTION_SPHERESCOLLISIONDETECTOR_H_
#define SRC_PHYSICS_COLLISIONDETECTION_SPHERESCOLLISIONDETECTOR_H_


#include<CollisionDetector.h>

class SpheresCollisionDetector : public CollisionDetector {

    virtual std::vector<Contact>detectCollisions(const std::vector<Particle *> &particles) const {
        std::vector<Contact> contacts;

        for(std::vector<Particle *>::const_iterator particleA = particles.begin(); particleA != particles.end(); particleA++) {
            for(std::vector<Particle *>::const_iterator particleB = particleA+1; particleB != particles.end(); particleB++) {
                if((*particleA)->getStatus() && (*particleB)->getStatus()) {
                    vector delta = (*particleB)->getPosition() - (*particleA)->getPosition();

                    if(delta * delta < 4 * sphere_radius * sphere_radius) {
                        real modulo = delta.modulo();
                        vector normal = delta * (1.0 / modulo);
                        contacts.push_back(Contact(*particleA, *particleB, delta, 1.0f,  2* sphere_radius - modulo));
                    }
                }
            }
        }

        return contacts;

    }
};


#endif /* SRC_PHYSICS_COLLISIONDETECTION_SPHERESCOLLISIONDETECTOR_H_ */
