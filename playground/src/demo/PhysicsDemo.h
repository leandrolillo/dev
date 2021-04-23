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
#include<renderers/SkyboxRenderer.h>
#include<renderers/GridRenderer.h>

#include<Math3d.h>
#include<forces/Gravity.h>

#define numberOfParticles 5

class PhysicsDemoRunner;

class BulletParticle : public Particle
{
private:
    PhysicsDemoRunner *runner = null;

public:
    void setRunner(PhysicsDemoRunner *runner);
	void afterIntegrate(real dt);
	void onCollision();
};

class PhysicsDemoRunner: public PlaygroundRunner {
	Logger *logger = Logger::getLogger("PhysicsDemoRunner");
	OpenGLRunner *openGl = null;
	AudioRunner *audio = null;
	PhysicsRunner *physics = null;

	Source *gunshotSource;
	Source *bounceSource;

	BulletParticle particles[numberOfParticles];
	Gravity gravity;

	unsigned long to = 0;
	real invPerformanceFreq = 1.0f;

	/**
	 * stats
	 */
	real elapsedTime = 0.0f;
	unsigned long frames = 0;

	Camera camera;
	DefaultRenderer defaultRenderer;
	SkyboxRenderer skyboxRenderer;
	GridRenderer gridRenderer;
public:
	PhysicsDemoRunner() : gravity(vector(0.0, -9.8, 0.0)) {
		reset();
	}

	unsigned char getId() {
		return 200;
	}

	virtual unsigned char getInterests() {
			return KEY_DOWN | KEY_UP | MOUSE_MOVE | MOUSE_WHEEL | MOUSE_BUTTON_DOWN | RESIZE;
	}

	void resize(unsigned int height, unsigned int width) {
	    camera.setProjectionMatrix(camera.perspectiveProjection(45.0, (GLfloat) width / (GLfloat) height, 0.1, 300.0));
	}

	void reset() {
		for(Particle *particle = particles; particle < (particles + numberOfParticles); particle++) {
			particle->setStatus(false);
		}

        camera.setViewMatrix(matriz_4x4::matrizTraslacion(vector(0.0f, 0.0f, -5.0f)));
	}

	bool init() {
		openGl = (OpenGLRunner*) this->getContainer()->getRunner(1);
		audio = (AudioRunner*) this->getContainer()->getRunner(3);
		physics = (PhysicsRunner *)this->getContainer()->getRunner(4);

		gunshotSource = audio->createSource("audio/handgunfire.wav", vector(0, 0, 0), vector(0, 0, 0), false);
		bounceSource = audio->createSource("audio/twang3.wav", vector(0, 0, 0), vector(0, 0, 0), false);


	    defaultRenderer.setVideoRunner(openGl);
	    gridRenderer.setVideoRunner(openGl);
	    skyboxRenderer.setVideoRunner(openGl);
	    skyboxRenderer.setSize(200);

		openGl->setClearColor(0.0, 0.5, 0.0, 0.0);
		openGl->setAttribute(DEPTH_TEST, true);

		reset();

		for(BulletParticle *particle = particles; particle < (particles + numberOfParticles); particle++) {
		    particle->setRunner(this);
			physics->getParticleManager()->addParticle(particle);
		}

		physics->getParticleManager()->addForce(&this->gravity);

		return true;
	}

	LoopResult doLoop() {
	    defaultRenderer.clearObjects();
	    defaultRenderer.drawAxis(matriz_4x4::identidad);


//		logger->debug("Drawing");
		for(BulletParticle *particle = particles; particle < (particles + numberOfParticles); particle++) {
			if(particle->getStatus() == true) {
                defaultRenderer.drawSphere(matriz_4x4::matrizTraslacion(particle->getPosition()), 0.1);

//				logger->info("Particle is enabled: position: %s, velocity: %s",
//						particle->getPosition().toString().c_str(),
//						particle->getVelocity().toString().c_str());

			} else {
//				logger->debug("Particle is disabled");
			}
		}

		defaultRenderer.render(camera);
		skyboxRenderer.render(camera);
		gridRenderer.render(camera);

		return CONTINUE;
	}

	void onCollision(BulletParticle *bulletParticle) {
	    bounceSource->setPosition(bulletParticle->getPosition());
	    audio->updateSource(bounceSource);
        audio->playSource(bounceSource);
	}

	void afterIntegrate(BulletParticle *bulletParticle) {
        if(bulletParticle->getPosition().modulo() > 100) {
            bulletParticle->setStatus(false);
        }
	}

	void fire(const vector &position) {
		Particle *bullet = null;

		logger->debug("Iterating particles");
		for(BulletParticle *particle = particles; particle < (particles + numberOfParticles); particle++) {
			if(!particle->getStatus()) {
				bullet = particle;
				break;
			}
		}

		if(bullet) {
			bullet->setPosition(position);
			bullet->setVelocity(((vector)camera.getViewMatrix().columna(2)).normalizado() * -35);
			bullet->setAcceleration(vector(0, 0, 0));
			bullet->setMass(0.1);
			bullet->setDamping(0.99f);
			bullet->setStatus(true);

			gunshotSource->setPosition(position);
			audio->updateSource(gunshotSource);
			audio->playSource(gunshotSource);

			logger->info("bullet at position: %s",
			        bullet->getPosition().toString("%.2f").c_str());

		} else {
			logger->debug("no particle found");
		}
	}

	void mouseWheel(int wheel) {
        camera.setViewMatrix(matriz_4x4::matrizTraslacion(camera.getViewPosition() + vector(0.0f, 0.0f, wheel)));
        audio->updateListener(camera.getViewPosition() * -1);
        logger->info("camera: %s", camera.getViewPosition().toString("%.2f").c_str());
	}

	virtual void mouseMove(int dx, int dy) {
        camera.setViewMatrix(matriz_4x4::matrizTraslacion(camera.getViewPosition() + vector(0.1f * dx, 0.1f * dy, 0)));
        audio->updateListener(camera.getViewPosition() * -1);
        logger->info("camera: %s", camera.getViewPosition().toString("%.2f").c_str());
	}

	void mouseButtonDown(unsigned char button, int x, int y) {
		fire(camera.getViewPosition() * -1);
		fire(vector(1.0, 2.0, 0.0));
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

void BulletParticle::setRunner(PhysicsDemoRunner *runner) {
    this->runner = runner;
}

void BulletParticle::afterIntegrate(real dt) {
    if(runner != null) {
        runner->afterIntegrate(this);
    }
}

void BulletParticle::onCollision() {
    if(runner != null) {
        runner->onCollision(this);
    }

}




#endif /* SRC_PHYSICSDEMO_H_ */
