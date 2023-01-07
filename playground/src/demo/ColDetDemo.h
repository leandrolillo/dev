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
#include<renderers/SkyboxRenderer.h>
#include<renderers/GridRenderer.h>
#include<renderers/geometry/GeometryRenderer.h>

#include<Math3d.h>
#include<forces/Gravity.h>

#include<Geometry.h>
#include"../geometry/intersection/CollisionTester.h"

#include "base/BaseDemo.h"


class CollisionDetectionDemoRunner;

class CollidingParticle: public Particle {
private:
    CollisionDetectionDemoRunner *runner = null;
    bool _isSelected = false;

public:
    CollidingParticle(Geometry * geometry) : Particle(geometry) {
    }

    bool isSelected() const {
        return this->_isSelected;
    }

    void setSelected(bool selected) {
        this->_isSelected = selected;
    }

    void setRunner(CollisionDetectionDemoRunner *runner);
    void afterIntegrate(real dt);
    void onCollision(const ParticleContact &contact);
};

class CollisionDetectionDemoRunner: public BaseDemoRunner {
    ParticleManager particleManager;
    const CollisionTester &intersectionTester = *(particleManager.getCollisionDetector().getIntersectionTester());

    std::vector<std::unique_ptr<CollidingParticle>> collidingParticles;
    Particle ground;

    vector2 startPosition;
    vector2 endPosition;

    SkyboxRenderer skyboxRenderer;
    GridRenderer gridRenderer;
    GeometryRenderer geometryRenderer;

    MaterialResource red = MaterialResource(vector(1, 0, 0), vector(1, 0, 0), vector(1, 0, 0), 1.0, 0.2);
    MaterialResource green = MaterialResource(vector(0, 1, 0), vector(0, 1, 0), vector(0, 1, 0), 1.0, 0.2);
    MaterialResource blue = MaterialResource(vector(0, 0, 1), vector(0, 0, 1), vector(0, 0, 1), 1.0, 0.2);

public:
    CollisionDetectionDemoRunner() : ground(new Plane(vector(0, 0, 0), vector(0, 1, 0))), geometryRenderer(defaultRenderer) {
    }

    void reset() {
//		video->setMousePosition(video->getScreenWidth() >> 1, video->getScreenHeight() >> 1);
        camera.setViewMatrix(matriz_4x4::traslacion(vector(0.0f, -0.5f, -10.0f)));

        real radius = (real) 0.5;
        if(collidingParticles.size() > 0) {
            collidingParticles[0]->setPosition(vector(-1, 0, 0));
            ((Sphere*) collidingParticles[0]->getBoundingVolume())->setRadius(radius);
            collidingParticles[0]->setMass(M_PI * radius * radius);
            collidingParticles[0]->setVelocity(vector(1, 1, 0));
        }

        if(collidingParticles.size() > 1) {
            collidingParticles[1]->setPosition(vector(1, 0, 0));
            ((Sphere*) collidingParticles[1]->getBoundingVolume())->setRadius(radius);
            collidingParticles[1]->setMass(M_PI * radius * radius);
            collidingParticles[1]->setVelocity(vector(-1, -1, 0));
        }

        if(collidingParticles.size() > 2) {
            collidingParticles[2]->setPosition(vector(1, 0, 0));
            ((AABB*) collidingParticles[2]->getBoundingVolume())->setHalfSizes(vector(0.5, 0.5, 0.5));
            collidingParticles[2]->setMass(1);
            collidingParticles[2]->setVelocity(vector(-1, -1, 0));
        }

        if(collidingParticles.size() > 3) {
            collidingParticles[3]->setPosition(vector(1, 2, 0));
            ((AABB*) collidingParticles[3]->getBoundingVolume())->setHalfSizes(vector(1, 1, 1));
            collidingParticles[3]->setMass(1);
            collidingParticles[3]->setVelocity(vector(0, 0, 0));
        }

        if(collidingParticles.size() > 4) {
            collidingParticles[4]->setPosition(vector(-2, 1, 0));
            collidingParticles[4]->setMass(1);
            collidingParticles[4]->setVelocity(vector(0, 0, 0));
        }

        ground.setInverseMass((real)0);
    }

    bool init() override {
    	BaseDemoRunner::init();

        this->video->resize(800, 600);

        logger->debug("Initializing renderers");
        gridRenderer.setVideoRunner(video);
//	    skyboxRenderer.setVideoRunner(video);
//	    skyboxRenderer.setSize(200);

        logger->debug("Setting up video %d", video);
        video->enable(BLEND, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        collidingParticles.push_back(std::unique_ptr<CollidingParticle>(new CollidingParticle(new Sphere(vector(0, 0, 0), (real) 0.5))));
        collidingParticles.back()->setRunner(this);
        particleManager.addParticle(collidingParticles.back().get());

        collidingParticles.push_back(std::unique_ptr<CollidingParticle>(new CollidingParticle(new Sphere(vector(0, 0, 0), (real) 0.5))));
        collidingParticles.back()->setRunner(this);
        particleManager.addParticle(collidingParticles.back().get());

        collidingParticles.push_back(std::unique_ptr<CollidingParticle>(new CollidingParticle(new AABB(vector(0, 0, 0), vector(0.5, 0.5, 0.5)))));
        collidingParticles.back()->setRunner(this);
        particleManager.addParticle(collidingParticles.back().get());

        collidingParticles.push_back(std::unique_ptr<CollidingParticle>(new CollidingParticle(new AABB(vector(0, 0, 0), vector(1, 1, 1)))));
        collidingParticles.back()->setRunner(this);
        particleManager.addParticle(collidingParticles.back().get());


        HierarchicalGeometry *hierarchicalGeometry = new HierarchicalGeometry(new AABB(vector(0, 0, 0), vector(1, 0.5, 0.5)));
        hierarchicalGeometry->addChildren(new Sphere(vector(-0.5, 0, 0), 0.5));
        hierarchicalGeometry->addChildren(new Sphere(vector(0.5, 0, 0), 0.5));

        collidingParticles.push_back(std::unique_ptr<CollidingParticle>(new CollidingParticle(hierarchicalGeometry)));
        collidingParticles.back()->setRunner(this);
        particleManager.addParticle(collidingParticles.back().get());

        particleManager.addParticle(&ground);

        geometryRenderer.setCollidingParticleBMaterial(&blue);


        reset();

        logger->debug("Completed initialization");
        return true;
    }


    LoopResult doLoop() override {
        defaultRenderer.clear();
        defaultRenderer.drawAxes(matriz_4x4::identidad);
        defaultRenderer.drawLine(matriz_4x4::identidad, vector(-1, 0, 0), vector(1, 0, 0));
        defaultRenderer.drawLine(matriz_4x4::identidad, vector(0, -1, 0), vector(0, 1, 0));
        defaultRenderer.drawLine(matriz_4x4::identidad, vector(0, 0, -1), vector(0, 0, 1));

        particleManager.detectCollisions();
        std::vector<ParticleContact> contacts = particleManager.getContacts();

        geometryRenderer.render(&particleManager);

        defaultRenderer.render(camera);
        skyboxRenderer.render(camera);
        gridRenderer.render(camera);

        return LoopResult::CONTINUE;
    }

    void onCollision(CollidingParticle *sphereParticle) {
        //sphereParticle->setIsColliding(true);
    }

    void afterIntegrate(CollidingParticle *particle) {
        if (particle->getPosition().modulo() > 100) {
            particle->setStatus(false);
        }
    }

    void onMouseMove(int x, int y, int dx, int dy, unsigned int buttons) override {
            Line line(camera.getPosition(),
                    camera.getRayDirection((unsigned int) x, (unsigned int) y, video->getScreenWidth(), video->getScreenHeight()));

            if (!equalsZero(line.getDirection().z)) {
                for(auto &particle : collidingParticles) {

                if (particle->isSelected()) {
                    vector origin = particle->getBoundingVolume()->getOrigin();

                    real t = (origin.z - line.getOrigin().z) / line.getDirection().z;
                    particle->setPosition(line.getOrigin() + t * line.getDirection());
                }
            }
        }
    }

    void onMouseButtonUp(unsigned char button, int x, int y) override {
        if (button == SDL_BUTTON_LEFT) {
            for (auto &particle : collidingParticles) {
                particle->setSelected(false);
            }
        }
    }

	void onMouseWheel(int wheel) override {
		logger->info("Mouse wheel %d", wheel);
	    camera.setPosition(camera.getPosition() - vector(0.0, 0.0, std::min(1.0, 0.1 * wheel)));
	    logger->info("Camera position: %s", camera.getPosition().toString().c_str());
	}


    void onMouseButtonDown(unsigned char button, int x, int y) override {
        if (button == SDL_BUTTON_LEFT) {
            this->startPosition = vector2(x, y);

            Line line(camera.getPosition(),
                    camera.getRayDirection((unsigned int) x, (unsigned int) y, video->getScreenWidth(), video->getScreenHeight()));

            for(auto &particle : collidingParticles) {
                if (intersectionTester.intersects(*particle->getBoundingVolume(), (Geometry &)line)) {
                    particle->setSelected(true);
                }
            }
        }
    }

    void onKeyDown(unsigned int key, unsigned int keyModifier) override {
        switch (key) {
			case '+':
				camera.setPosition(camera.getPosition() - vector(0.0, 0.0, 0.1));
			break;
			case '-':
				camera.setPosition(camera.getPosition() + vector(0.0, 0.0, 0.1));
			break;
			case SDLK_LEFT:
				camera.setPosition(camera.getPosition() - vector(0.1, 0.0, 0.0));
			break;
			case SDLK_RIGHT:
				camera.setPosition(camera.getPosition() + vector(0.1, 0.0, 0.0));
			break;
        	case SDLK_UP:
        		camera.setPosition(camera.getPosition() + vector(0.0, 0.1, 0.0));
        	break;
        	case SDLK_DOWN:
        		camera.setPosition(camera.getPosition() - vector(0.0, 0.1, 0.0));
        		break;
            case SDLK_BACKSPACE:
                reset();
                break;
            case SDLK_SPACE:
            	particleManager.resolveContacts(0.1);
                break;
        }
    }

};

void CollidingParticle::setRunner(CollisionDetectionDemoRunner *runner) {
    this->runner = runner;
}

void CollidingParticle::afterIntegrate(real dt) {
    if (runner != null) {
        runner->afterIntegrate(this);
    }
}

void CollidingParticle::onCollision(const ParticleContact &contact) {
    if (runner != null) {
        runner->onCollision(this);
    }

}

class CollisionDetectionPlayground: public Playground {
public:
    CollisionDetectionPlayground(const String &resourcesBasePath) : Playground(resourcesBasePath) {
    }
    void init() override {
        Playground::init();
        this->addRunner(new OpenGLRunner());
        this->addRunner(new AudioRunner());
        this->addRunner(new CollisionDetectionDemoRunner());
    }
};

#endif
