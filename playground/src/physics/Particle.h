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
	real mass;

	bool status;

	/**
	 * rough approximation of drag to avoid numerical stability issues - without this objects are likely to accelerate magically.
	 *
	 */
	real damping;

	vector forceAccumulator;

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

		this->mass = mass;
	}

	void setInverseMass(real inverseMass) {
			if(inverseMass == 0.0f) {
				this->mass = (real)0.0;
			} else {
				this->mass = (real)1.0 / inverseMass;
			}

			this->inverseMass = inverseMass;
		}

	real getMass() {
		return this->mass;
	}

	real getInverseMass() {
		return this->inverseMass;
	}

	void setDamping(real damping) {
		this->damping = damping;
	}

	void clearForceAccumulator() {
		this->forceAccumulator = vector(0, 0, 0);
	}

	void applyForce(vector force) {
		this->forceAccumulator += force;
	}

};

#endif /* SRC_PHYSICS_PARTICLE_H_ */
