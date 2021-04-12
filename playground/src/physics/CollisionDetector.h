/*
 * CollisionDetector.h
 *
 *  Created on: Apr 5, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_COLLISIONDETECTOR_H_
#define SRC_PHYSICS_COLLISIONDETECTOR_H_

#define sphere_radius 0.1

class CollisionDetector {

public:
    std::vector<Contact>detectCollisions(const std::vector<Particle *> &particles) const{
        std::vector<Contact> contacts;
        for(std::vector<Particle*>::const_iterator iterator = particles.begin(); iterator != particles.end(); iterator++) {
            Particle *particle = *iterator;
            if(particle->getPosition().y <= sphere_radius) {
                contacts.push_back(Contact(particle, null, vector(0, 1, 0), 1.0f, sphere_radius - particle->getPosition().y));
            }
        }

        return contacts;
    }
};



#endif /* SRC_PHYSICS_COLLISIONDETECTOR_H_ */
