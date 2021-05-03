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

class CollisionDetector {
public:
    virtual std::vector<Contact>detectCollisions(const std::vector<Particle *> &particles) const = 0;
};


#endif /* SRC_PHYSICS_COLLISIONDETECTOR_H_ */
