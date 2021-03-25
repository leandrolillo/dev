/*
 * PhysicsDemo.h
 *
 *  Created on: Mar 22, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICSDEMO_H_
#define SRC_PHYSICSDEMO_H_

#include <iostream>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGLRunner.h>
#include <AudioRunner.h>
#include<PhysicsRunner.h>

#include<Math3d.h>
#include<forces/Gravity.h>

#define numberOfParticles 5

class BulletParticle : public Particle
{
	void afterIntegrate(real dt) {
		if(this->position.modulo() > 100) {
			this->status = false;
		}
	}

};

class PhysicsDemoRunner: public PlaygroundRunner {
	Logger *logger = Logger::getLogger("Main.cpp");
	OpenGLRunner *openGl = null;
	AudioRunner *audio = null;
	PhysicsRunner *physics = null;

	Source *gunshotSource;

	matriz_4x4 view;
	vector viewPosition;

	BulletParticle particles[numberOfParticles];
	Gravity gravity;

	unsigned long to = 0;
	real invPerformanceFreq = 1.0f;

	/**
	 * stats
	 */
	real elapsedTime = 0.0f;
	unsigned long frames = 0;
public:
	PhysicsDemoRunner() : gravity(vector(0.0, -9.8, 0.0)) {
		reset();
	}

	unsigned char getId() {
		return 200;
	}

	virtual unsigned char getInterests() {
			return KEY_DOWN | KEY_UP | MOUSE_MOVE | MOUSE_WHEEL | MOUSE_BUTTON_DOWN;
	}

	void reset() {
		for(Particle *particle = particles; particle < (particles + numberOfParticles); particle++) {
			particle->setStatus(false);
		}

		viewPosition = vector(0.0f, 0.0f, -5.0f);

	}

	bool init() {
		openGl = (OpenGLRunner*) this->getContainer()->getRunner(1);
		audio = (AudioRunner*) this->getContainer()->getRunner(3);
		physics = (PhysicsRunner *)this->getContainer()->getRunner(4);

		gunshotSource = audio->createSource("audio/handgunfire.wav", vector(0, 0, 0), vector(0, 0, 0), false);

		glClearColor(0.0, 0.5, 0.0, 0.0);
		glEnable(GL_DEPTH_TEST);

		reset();

		for(BulletParticle *particle = particles; particle < (particles + numberOfParticles); particle++) {
			physics->getParticleManager()->addParticle(particle);
		}

		physics->getParticleManager()->addForce(&this->gravity);

		return true;
	}

	LoopResult doLoop() {
		openGl->setViewMatrix(matriz_4x4::matrizTraslacion(this->viewPosition));
		openGl->setModelMatrix(matriz_4x4::Identidad);
		openGl->sendMatrices();
		openGl->glAxis(1.0);

//		logger->debug("Drawing");
		int count = 0;
		for(BulletParticle *particle = particles; particle < (particles + numberOfParticles); particle++) {
			if(particle->getStatus() == true) {
//				logger->debug("Particle is enabled: position: %s, velocity: %s",
//						particle->getPosition().toString().c_str(),
//						particle->getVelocity().toString().c_str());

				count++;
				openGl->setModelMatrix(matriz_4x4::matrizTraslacion(particle->getPosition()));

				openGl->sendMatrices();
				openGl->glSphere(0.1);
			} else {
//				logger->debug("Particle is disabled");
			}
		}

		return CONTINUE;
	}

	void fire() {
		Particle *bullet = null;

		logger->debug("Iterating particles");
		for(BulletParticle *particle = particles; particle < (particles + numberOfParticles); particle++) {
			if(!particle->getStatus()) {
				bullet = particle;
				break;
			}
		}

		if(bullet) {
			bullet->setPosition((vector)(view.columna(3)));
			bullet->setVelocity(((vector)view.columna(2)).normalizado() * -35);
			bullet->setAcceleration(vector(0, 0, 0));
			bullet->setMass(0.1);
			bullet->setDamping(0.99f);
			bullet->setStatus(true);

			audio->playSource(gunshotSource);
		} else {
			logger->debug("no particle found");
		}
	}

	void mouseWheel(int wheel) {
		viewPosition += vector(0.0f, 0.0f, wheel);
	}

	virtual void mouseMove(int dx, int dy) {
		viewPosition += vector(0.1f * dx, 0.1f * dy, 0);
	}

	void mouseButtonDown(unsigned char button ) {
		fire();
	}

	virtual void keyDown(unsigned int key) {
		switch (key) {
		case SDLK_ESCAPE:
			this->getContainer()->stop();
			break;
		case SDLK_SPACE:
			reset();
			//fire();
			break;
		case 'f':
		case 'F':
			this->openGl->setFullscreen(!this->openGl->getFullscreen());
			break;
		}
	}

};

class PhysicsPlayground: public Playground {
public:
	PhysicsPlayground(const String &resourcesBasePath) :
			Playground(resourcesBasePath) {
	}
	void init() {
		Playground::init();
		this->addRunner(new OpenGLRunner());
		this->addRunner(new AudioRunner());
		this->addRunner(new PhysicsRunner());
		this->addRunner(new PhysicsDemoRunner());
	}
};



#endif /* SRC_PHYSICSDEMO_H_ */
