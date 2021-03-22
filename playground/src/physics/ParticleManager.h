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

#include<vector>
#include <algorithm>

class ParticleIntegrator {
public:
	void integrate(real dt, Particle &particle) {
		particle.position = particle.position + particle.velocity * dt;
		particle.velocity = particle.velocity * powr(particle.damping, dt) + particle.acceleration * dt;
	}
};

class ParticleManager {
	std::vector<Particle *> particles;
	ParticleIntegrator particleIntegrator;

public:

	void addParticle(Particle *particle) {
		this->particles.push_back(particle);
	}

	void removeParticle(const Particle *particle) {
		//this->particles.erase(__position)
		particles.erase(std::remove(particles.begin(), particles.end(), particle),  particles.end());
	}

	void step(real dt) {
		for(std::vector<Particle *>::iterator iterator = particles.begin(); iterator < particles.end(); iterator++) {
			particleIntegrator.integrate(dt,  **iterator);

			(*iterator)->afterIntegrate(dt);
		}
	}
};




#endif /* SRC_PHYSICS_PARTICLEMANAGER_H_ */
