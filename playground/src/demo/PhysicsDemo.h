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

#include<renderers/DefaultRenderer.h>
#include<renderers/SkyboxRenderer.h>
#include<renderers/GridRenderer.h>

#include<Math3d.h>
#include<forces/Gravity.h>
#include<collisionDetection/GroundPlaneCollisionDetector.h>
#include<collisionDetection/SpheresCollisionDetector.h>

#define numberOfParticles 10

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
	Logger *logger = LoggerFactory::getLogger("PhysicsDemoRunner");
	VideoRunner *video = null;
	AudioRunner *audio = null;
	PhysicsRunner *physics = null;

	GroundPlaneCollisionDetector groundPlaneCollisionDetector;
	SpheresCollisionDetector spheresCollisionDetector;

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

	MaterialResource materials[3] = {MaterialResource(vector(1, 0.5, 0.5), vector(1, 0.5, 0.5), vector(1, 1, 1), 32),
	        MaterialResource(vector(0.5, 1, 0.5), vector(0.5, 1, 0.5), vector(1, 1, 1), 32),
	        MaterialResource(vector(0.5, 0.5, 1), vector(0.5, 0.5, 1), vector(1, 1, 1), 32)};
public:
	PhysicsDemoRunner() : gravity(vector(0.0, -9.8, 0.0)) {
	}

	unsigned char getId() {
		return 200;
	}

	virtual unsigned char getInterests() {
			return KEY_DOWN | KEY_UP | MOUSE_MOVE | MOUSE_WHEEL | MOUSE_BUTTON_DOWN | RESIZE;
	}

	void resize(unsigned int height, unsigned int width) {
	    camera.setProjectionMatrix(Camera::perspectiveProjection(45.0, (GLfloat) width / (GLfloat) height, 0.1, 300.0));
	}

	void reset() {
		for(Particle *particle = particles; particle < (particles + numberOfParticles); particle++) {
			particle->setStatus(false);
		}

		video->setMousePosition(video->getScreenWidth() >> 1, video->getScreenHeight() >> 1);

        camera.setViewMatrix(matriz_4x4::matrizTraslacion(vector(0.0f, 0.0f, -5.0f)));
	}

	bool init() {
		video = (VideoRunner*) this->getContainer()->getRequiredRunner(VideoRunner::ID);
		audio = (AudioRunner*) this->getContainer()->getRequiredRunner(AudioRunner::ID);
		physics = (PhysicsRunner *)this->getContainer()->getRequiredRunner(PhysicsRunner::ID);
		physics->setPlaybackSpeed(0.3);

		gunshotSource = audio->createSource("audio/handgunfire.wav", vector(0, 0, 0), vector(0, 0, 0), false);
		bounceSource = audio->createSource("audio/twang3.wav", vector(0, 0, 0), vector(0, 0, 0), false);


	    defaultRenderer.setVideoRunner(video);
	    gridRenderer.setVideoRunner(video);
	    skyboxRenderer.setVideoRunner(video);
	    skyboxRenderer.setSize(200);

		video->setClearColor(0.0, 0.5, 0.0, 0.0);
		video->setAttribute(DEPTH_TEST, true);

		reset();

		physics->getParticleManager()->addCollisionDetector(&groundPlaneCollisionDetector);
		physics->getParticleManager()->addCollisionDetector(&spheresCollisionDetector);

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
		for(unsigned int index = 0; index < numberOfParticles; index++) {
		    BulletParticle *particle = &particles[index];

			if(particle->getStatus() == true) {
			    defaultRenderer.sendMaterial(&materials[index % 3]);
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

		return LoopResult::CONTINUE;
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

	void fire(const vector &position, bool isStatic = false) {
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
			if(isStatic) {
			    bullet->setVelocity(vector(0, 0, 0));
			    bullet->setDamping(0.99f);
			} else {
			    bullet->setVelocity(((vector)camera.getViewMatrix().columna(2)).normalizado() * -35);
			    bullet->setDamping(0.99f);
			}
			bullet->setAcceleration(vector(0, 0, 0));
			bullet->setMass(0.1);

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
        logger->debug("camera: %s", camera.getViewPosition().toString("%.2f").c_str());
	}

	void mouseMove(int x, int y, int dx, int dy) {
        camera.setViewMatrix(matriz_4x4::matrizTraslacion(camera.getViewPosition() + vector(0.1f * dx, 0.1f * dy, 0)));
        audio->updateListener(camera.getViewPosition() * -1);
        logger->debug("camera: %s", camera.getViewPosition().toString("%.2f").c_str());
	}

	void mouseButtonDown(unsigned char button, int x, int y) {
		fire(camera.getViewPosition() * -1);
		fire(vector(1.0, 2.0, 0.0), true);
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
			this->video->setFullscreen(!this->video->getFullscreen());
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
