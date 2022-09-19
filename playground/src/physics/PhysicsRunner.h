/*
 * PhysicsPlaygroundRunner.h
 *
 *  Created on: Mar 22, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_PHYSICSRUNNER_H_
#define SRC_PHYSICS_PHYSICSRUNNER_H_

#include<OpenGLRunner.h>
#include<Playground.h>
#include<ParticleManager.h>

class PhysicsRunner: public PlaygroundRunner {
	Logger *logger = LoggerFactory::getLogger("physics/PhysicsRunner");

	/**
	 * TODO: Get rid of this dependency (it exists for timing functions).
	 */
	VideoRunner *videoRunner = null;

	ParticleManager particleManager;

	unsigned long to = 0;
	real invPerformanceFreq = 1.0f;

	real playbackSpeed = 1.0f;

	/**
	 * stats
	 */
	real elapsedTime = 0.0f;
	unsigned long frames = 0;
public:
	static const unsigned char ID;
public:
	PhysicsRunner() {
	}

	unsigned char getId() const override {
		return ID;
	}

	ParticleManager *getParticleManager() {
		return &this->particleManager;
	}

	void setPlaybackSpeed(real playbackspeed) {
	    this->playbackSpeed = playbackspeed;
	}

	bool init() {
		videoRunner = (VideoRunner*) this->getContainer()->getRequiredRunner(VideoRunner::ID);

		invPerformanceFreq = (real)1 / (real)videoRunner->getPerformanceFreq();
		this->start();

		return true;
	}

	void beforeLoop() {
		this->particleManager.clearAccumulators();
	}

	void start() {
		logger->info("starting physics runner");
		if(!this->getEnabled()) {
			to = videoRunner->getPerformanceCounter();
		}
		this->setEnabled(true);

	}

	void stop() {
		logger->info("Starting physics runner");
		this->setEnabled(false);
	}

	void step(real dt) {
		if(!this->getEnabled()) {
			this->beforeLoop();
			this->particleManager.step(dt);
		}
	}

	LoopResult doLoop() {
		unsigned long tf = videoRunner->getPerformanceCounter();
		real dt = (real)(tf - to) * invPerformanceFreq;
		to = tf;

		this->particleManager.step(dt * playbackSpeed);

		/**
		 * stats
		 */
		elapsedTime += dt;
		frames++;
//      real fps = (real)frames / elapsedTime;
//		printf("Elapsed time: %.2f, frames: %u, fps:%.2f. View position: %s - count = %d\r",
//				elapsedTime,
//				frames,
//				fps,
//				viewPosition.toString().c_str(),
//				count);

		return LoopResult::CONTINUE;
	}
};

const unsigned char PhysicsRunner::ID = 4;


#endif /* SRC_PHYSICS_PHYSICSRUNNER_H_ */
