/*
 * PhysicsDemo.h
 *
 *  Created on: Mar 22, 2021
 *      Author: leandro
 */

#ifndef SRC_COLDETDEMO_H_
#define SRC_COLDETDEMO_H_

#include <iostream>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGLRunner.h>
#include <AudioRunner.h>
#include<renderers/DefaultRenderer.h>
#include<renderers/SkyboxRenderer.h>
#include<renderers/GridRenderer.h>

#include<Math3d.h>
#include<forces/Gravity.h>

#include <Geometry.h>

#include<InvalidArgumentException.h>

#include<Sphere.h>
#include<Plane.h>
#include<Line.h>

class CollisionDetectionDemoRunner;

class SphereParticle : public Particle
{
private:
    CollisionDetectionDemoRunner *runner = null;
    LinkedSphere boundingSphere;
    bool _isColliding = false;

public:
    SphereParticle(const vector &position, real radius) : boundingSphere(this->position, radius) {
        this->position = position;
    }
    virtual const Geometry *getGeometry() const {
        return &boundingSphere;
    }

    void setIsColliding(bool isColliding) {
        this->_isColliding = isColliding;
    }

    bool isColliding() const {
        return this->_isColliding;
    }

    void setRunner(CollisionDetectionDemoRunner *runner);
    void afterIntegrate(real dt);
    void onCollision(const ParticleContact &contact);
};

#define number_of_sphere_particles 2

class CollisionDetectionDemoRunner: public PlaygroundRunner {
	Logger *logger = LoggerFactory::getLogger("CollisionDetectionDemoRunner");
	VideoRunner *video = null;
	AudioRunner *audio = null;

	CollisionDetector collisionDetector;
	std::vector<Particle *>particles;
	SphereParticle sphereParticles[number_of_sphere_particles] = { SphereParticle(vector(-1, 0, 0), 1), SphereParticle(vector(1, 0, 0), 0.5)};
	Plane plane = Plane(vector(0, 0, 0), vector(0, 1, 0));

	char selectedGeometry = -1;
	vector2 startPosition;
	vector2 endPoisition;

	Camera camera;
	DefaultRenderer defaultRenderer;
	SkyboxRenderer skyboxRenderer;
	GridRenderer gridRenderer;

	MaterialResource red = MaterialResource(vector4(1, 0, 0, 0.5), vector4(1, 0, 0, 0.5), vector4(1, 0, 0, 0.5), 1.0);
	MaterialResource green = MaterialResource(vector(0, 1, 0), vector(0, 1, 0), vector(0, 1, 0), 1.0);
	MaterialResource blue = MaterialResource(vector(0, 0, 1), vector(0, 0, 1), vector(0, 0, 1), 1.0);

public:
	CollisionDetectionDemoRunner() {
	    logger->addAppender(LoggerFactory::getAppender("stdout"));
	}

	unsigned char getId() {
		return 200;
	}

	virtual unsigned char getInterests() {
			return KEY_DOWN | KEY_UP | MOUSE_MOVE | MOUSE_WHEEL | MOUSE_BUTTON_DOWN | MOUSE_BUTTON_UP| RESIZE;
	}

	void resize(unsigned int height, unsigned int width) {
	    camera.setProjectionMatrix(Camera::perspectiveProjection(45.0, (GLfloat) width / (GLfloat) height, 0.1, 300.0));
	}

	void reset() {
		video->setMousePosition(video->getScreenWidth() >> 1, video->getScreenHeight() >> 1);
        camera.setViewMatrix(matriz_4x4::matrizTraslacion(vector(0.0f, -0.5f, -5.0f)));
	}

	bool init() {
		video = (VideoRunner*) this->getContainer()->getRequiredRunner(VideoRunner::ID);
		audio = (AudioRunner*) this->getContainer()->getRequiredRunner(AudioRunner::ID);

		logger->debug("Initializing renderers");
	    defaultRenderer.setVideoRunner(video);
	    gridRenderer.setVideoRunner(video);
//	    skyboxRenderer.setVideoRunner(video);
//	    skyboxRenderer.setSize(200);

	    logger->debug("Setting up video %d", video);
		video->setClearColor(0.0, 0.5, 0.0, 0.0);
		video->enable(DEPTH_TEST, true);
		video->enable(BLEND, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		collisionDetector.addScenery(&plane);

		for(unsigned char index = 0; index < number_of_sphere_particles; index ++) {
	        sphereParticles[index].setRunner(this);
	        sphereParticles[index].setStatus(true);
	        particles.push_back(&sphereParticles[index]);
		}

		reset();

		logger->debug("Completed initialization");
		return true;
	}

	void drawContact(ParticleContact &contact) {
        vector offset = vector(0, 0, 0); // vector(0, 0, ((AbstractSphere *)contact.getParticleA()->getGeometry())->getRadius());
        vector start = contact.getIntersection(); //contact.getParticleA()->getPosition();
        vector end = start - contact.getNormal() * contact.getPenetration();

        defaultRenderer.drawLine(matriz_4x4::identidad, start + offset, end + offset);

        if(contact.getParticleB() != null) {
            offset = vector(0, 0, 0); // vector(0, 0, ((AbstractSphere *)contact.getParticleB()->getGeometry())->getRadius());
            start = contact.getIntersection();
            end = start - contact.getNormal() * contact.getPenetration();

            defaultRenderer.drawLine(matriz_4x4::identidad, start + offset, end + offset);
        }
	}

	LoopResult doLoop() {
	    defaultRenderer.clear();
	    defaultRenderer.drawAxes(matriz_4x4::identidad);
        defaultRenderer.drawLine(matriz_4x4::identidad, vector(-1, 0, 0), vector(1, 0, 0));
        defaultRenderer.drawLine(matriz_4x4::identidad, vector(0, -1, 0), vector(0, 1, 0));
        defaultRenderer.drawLine(matriz_4x4::identidad, vector(0, 0, -1), vector(0, 0, 1));


	    std::vector<ParticleContact> contacts = collisionDetector.detectCollisions(particles);

	    //defaultRenderer.setMaterial(&blue);
	    for(std::vector<ParticleContact>::iterator contactIterator = contacts.begin(); contactIterator != contacts.end(); contactIterator++) {
	        drawContact(*contactIterator);
	        //Draw contacts
	    }

	    //logger->info("Found %d contacts.", contacts.size());

	    for(std::vector<Particle *>::iterator particleIterator = particles.begin() ; particleIterator != particles.end(); particleIterator++) {
	        SphereParticle *sphereParticle = (SphereParticle *)*particleIterator;
	        if(sphereParticle->isColliding()) {
	            defaultRenderer.setMaterial(&red);
	        } else {
	            defaultRenderer.setMaterial(null);
	        }

	        Sphere *sphere = (Sphere *)sphereParticle->getGeometry();
	        defaultRenderer.drawSphere(matriz_4x4::matrizTraslacion(sphere->getOrigin()), sphere->getRadius());
//	        defaultRenderer.setMaterial(&blue);
//          defaultRenderer.drawLine(matriz_4x4::identidad, vector(0, 0, 0), sphereParticle->getPosition());
//	        defaultRenderer.drawLine(matriz_4x4::identidad, vector(0, 0, 0), sphereParticle->getPosition() + vector(0, 0, sphere->getRadius()));

	        sphereParticle->setIsColliding(false);
	    }

		defaultRenderer.render(camera);
		//skyboxRenderer.render(camera);
		gridRenderer.render(camera);

		return LoopResult::CONTINUE;
	}

    void onCollision(SphereParticle *sphereParticle) {
        sphereParticle->setIsColliding(true);
    }

    void afterIntegrate(SphereParticle *sphereParticle) {
        if(sphereParticle->getPosition().modulo() > 100) {
            sphereParticle->setStatus(false);
        }

        sphereParticle->setIsColliding(false);
    }


	void mouseWheel(int wheel) {
//        camera.setViewMatrix(matriz_4x4::matrizTraslacion(camera.getViewPosition() + vector(0.0f, 0.0f, wheel)));
//        audio->updateListener(camera.getViewPosition() * -1);
        //logger->info("camera: %s", camera.getViewPosition().toString("%.2f").c_str());
	}

	virtual void mouseMove(int x, int y, int dx, int dy) {
	    if(this->selectedGeometry >= 0 && this->selectedGeometry <= 2) {
	        Line line(camera.getViewPosition() * -1, camera.getRayDirection((unsigned int)x, (unsigned int)y, video->getScreenWidth(), video->getScreenHeight()));

	        if(!equalsZero(line.getDirection().z)) {
	            Sphere *selectedSphere = (Sphere *)this->particles[selectedGeometry]->getGeometry();

	            real t = (selectedSphere->getOrigin().z - line.getOrigin().z) / line.getDirection().z;
	            this->particles[selectedGeometry]->setPosition(line.getOrigin() + t * line.getDirection());
	        }
	    }
	}

	void mouseButtonUp(unsigned char button, int x, int y)
    {
	    if(button == SDL_BUTTON_LEFT) {
            selectedGeometry = -1;
	    }
    }

	void mouseButtonDown(unsigned char button, int x, int y) {
	    if(button == SDL_BUTTON_LEFT) {
	        this->startPosition = vector2(x, y);

            Line line(camera.getViewPosition() * -1, camera.getRayDirection((unsigned int)x, (unsigned int)y, video->getScreenWidth(), video->getScreenHeight()));
            //logger->info("Line: %s", line.toString().c_str());

            selectedGeometry = -1;
            for(unsigned char index = 0; index < 2; index++) {
                const Geometry *current = particles[index]->getGeometry();

                if(current->intersects((Geometry &)line)) {
                    selectedGeometry = index;
                }
            }
	    }
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

void SphereParticle::setRunner(CollisionDetectionDemoRunner *runner) {
    this->runner = runner;
}

void SphereParticle::afterIntegrate(real dt) {
    if(runner != null) {
        runner->afterIntegrate(this);
    }
}

void SphereParticle::onCollision(const ParticleContact &contact) {
    if(runner != null) {
        runner->onCollision(this);
    }

}



class CollisionDetectionPlayground: public Playground {
public:
    CollisionDetectionPlayground(const String &resourcesBasePath) :
			Playground(resourcesBasePath) {
	}
	void init() {
		Playground::init();
		this->addRunner(new OpenGLRunner());
		this->addRunner(new AudioRunner());
		this->addRunner(new CollisionDetectionDemoRunner());
	}
};

#endif
