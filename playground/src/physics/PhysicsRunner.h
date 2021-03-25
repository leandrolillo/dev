/*
 * PhysicsPlaygroundRunner.h
 *
 *  Created on: Mar 22, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_PHYSICSRUNNER_H_
#define SRC_PHYSICS_PHYSICSRUNNER_H_

#include<OpenGLRunner.h>
#include<playground.h>
#include<ParticleManager.h>

class PhysicsRunner: public PlaygroundRunner {
	Logger *logger = Logger::getLogger("physics/PhysicsRunner");

	/**
	 * TODO: Get rid of this dependency (it exists for timing functions).
	 */
	OpenGLRunner *openGl = null;

	ParticleManager particleManager;

	unsigned long to = 0;
	real invPerformanceFreq = 1.0f;

	/**
	 * stats
	 */
	real elapsedTime = 0.0f;
	unsigned long frames = 0;
public:
	static const unsigned char ID = 4;
public:
	PhysicsRunner() {
	}

	unsigned char getId() {
		return ID;
	}

	ParticleManager *getParticleManager() {
		return &this->particleManager;
	}

	bool init() {
		openGl = (OpenGLRunner*) this->getContainer()->getRunner(1);

		to = openGl->getPerformanceCounter();
		invPerformanceFreq = (real)1 / (real)openGl->getPerformanceFreq();

		return true;
	}

	void beforeLoop() {
		this->particleManager.clearAccumulators();
	}

	LoopResult doLoop() {
		unsigned long tf = openGl->getPerformanceCounter();
		real dt = (real)(tf - to) * invPerformanceFreq;
		to = tf;

		this->particleManager.step(dt);

		/**
		 * stats
		 */
		elapsedTime += dt;
		frames++;
		real fps = (real)frames / elapsedTime;
//		printf("Elapsed time: %.2f, frames: %u, fps:%.2f. View position: %s - count = %d\r",
//				elapsedTime,
//				frames,
//				fps,
//				viewPosition.toString().c_str(),
//				count);

		return CONTINUE;
	}
};



#endif /* SRC_PHYSICS_PHYSICSRUNNER_H_ */
