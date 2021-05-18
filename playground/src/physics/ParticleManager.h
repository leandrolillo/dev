/*
 * ParticleManager.h
 *
 *  Created on: Mar 20, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_PARTICLEMANAGER_H_
#define SRC_PHYSICS_PARTICLEMANAGER_H_

#include<Math3d.h>
#include<Particle.h>
#include<Force.h>
#include<ParticleIntegrator.h>
#include<ContactResolver.h>
#include<CollisionDetector.h>

#include<vector>
#include <algorithm>


class ParticleManager {
    Logger *logger = LoggerFactory::getLogger("ParticleManager");
	std::vector<Particle *> particles;
	std::vector<Force *> forces;
	ParticleIntegrator particleIntegrator;
	ContactResolver contactResolver;
	CollisionDetector collisionDetector;

public:
	void addParticle(Particle *particle) {
		this->particles.push_back(particle);
	}

	void addForce(Force *force) {
		this->forces.push_back(force);
	}

	void removeParticle(const Particle *particle) {
		//this->particles.erase(__position)
		particles.erase(std::remove(particles.begin(), particles.end(), particle),  particles.end());
	}

	CollisionDetector &getCollisionDetector() {
	    return this->collisionDetector;
	}

	void clearAccumulators() const {
		for(std::vector<Particle *>::const_iterator iterator = particles.begin(); iterator != particles.end(); iterator++) {
			(*iterator)->clearForceAccumulator();
		}
	}

	void step(real dt) const {
		applyForces(dt);

		integrate(dt);

		/**
		 * generate contacts (collision and contact generators)
		 */
		std::vector<Contact> contacts = collisionDetector.detectCollisions(this->particles);
		contactResolver.resolve(contacts);

	}

protected:

	void integrate(real dt) const {
	    logger->info("Begin integrating");
		for(std::vector<Particle *>::const_iterator iterator = particles.begin(); iterator != particles.end(); iterator++) {
			Particle *particle = *iterator;
			if(particle != null && particle->getStatus()) {
				particleIntegrator.integrate(dt, *particle);
				particle->afterIntegrate(dt);
			}
		}
		logger->info("Done integrating");
	}

	void applyForces(real dt) const {
		for(std::vector<Force *>::const_iterator iterator = forces.begin(); iterator != forces.end(); iterator++) {
			(*iterator)->apply(dt, particles);
		}
	}
};




#endif /* SRC_PHYSICS_PARTICLEMANAGER_H_ */
