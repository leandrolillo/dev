/*
 * GroundPlaneCollisionDetector.h
 *
 *  Created on: Apr 29, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_COLLISIONDETECTION_GROUNDPLANECOLLISIONDETECTOR_H_
#define SRC_PHYSICS_COLLISIONDETECTION_GROUNDPLANECOLLISIONDETECTOR_H_

#include<CollisionDetector.h>

class GroundPlaneCollisionDetector : public CollisionDetector {

    virtual std::vector<Contact>detectCollisions(const std::vector<Particle *> &particles) const {
            std::vector<Contact> contacts;
            for(std::vector<Particle*>::const_iterator iterator = particles.begin(); iterator != particles.end(); iterator++) {
                Particle *particle = *iterator;
                if(particle->getStatus()) {
                    if(particle->getPosition().y <= sphere_radius) {
                        contacts.push_back(Contact(particle, null, vector(0, 1, 0), 0.7f, sphere_radius - particle->getPosition().y));
                    }
                }
            }

            return contacts;
        }

};


#endif /* SRC_PHYSICS_COLLISIONDETECTION_GROUNDPLANECOLLISIONDETECTOR_H_ */
