/*
 * Particle.h
 *
 *  Created on: Mar 18, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_PARTICLE_H_
#define SRC_PHYSICS_PARTICLE_H_

#include <Math3d.h>

class Particle {
	friend class ParticleIntegrator;

protected:
	vector position;
	vector velocity;
	vector acceleration;

	real inverseMass;

	bool status;

	/**
	 * rough approximation of drag to avoid numerical stability issues - without this objects are likely to accelerate magically.
	 *
	 */
	real damping;

public:

	virtual void afterIntegrate(real dt) {
	}

	virtual ~Particle() {

	}

	bool getStatus() {
		return this->status;
	}

	void setStatus(bool active) {
		this->status = active;
	}

	void setPosition(const vector &position) {
		this->position = position;
	}

	const vector &getPosition() {
		return this->position;
	}

	void setVelocity(const vector &velocity) {
		this->velocity = velocity;
	}

	const vector &getVelocity() {
			return this->velocity;
		}

	void setAcceleration(const vector &acceleration) {
		this->acceleration = acceleration;
	}

	void setMass(real mass) {
		if(mass == 0.0f) {
			this->inverseMass = (real)0.0;
		} else {
			this->inverseMass = (real)1.0 / mass;
		}
	}

	void setDamping(real damping) {
		this->damping = damping;
	}

};

#endif /* SRC_PHYSICS_PARTICLE_H_ */
