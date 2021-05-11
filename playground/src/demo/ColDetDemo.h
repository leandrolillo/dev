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
#include<collisionDetection/GroundPlaneCollisionDetector.h>
#include<collisionDetection/SpheresCollisionDetector.h>

#include <geometry/Sphere.h>
#include <geometry/Line.h>

#include<InvalidArgumentException.h>


class CollisionDetectionDemoRunner: public PlaygroundRunner {
	Logger *logger = Logger::getLogger("CollisionDetectionDemoRunner");
	VideoRunner *video = null;
	AudioRunner *audio = null;

	GroundPlaneCollisionDetector groundPlaneCollisionDetector;
	SpheresCollisionDetector spheresCollisionDetector;
	Sphere sphere[2] = { Sphere(vector(-1, 0, -1), 1), Sphere(vector(1, 0, -1), 1)};

	char selectedGeometry = -1;
	vector2 startPosition;
	vector2 endPoisition;

	Camera camera;
	DefaultRenderer defaultRenderer;
	SkyboxRenderer skyboxRenderer;
	GridRenderer gridRenderer;

	MaterialResource materials[3] = {MaterialResource(vector(1, 0.5, 0.5), vector(1, 0.5, 0.5), vector(1, 1, 1), 32),
	        MaterialResource(vector(0.5, 1, 0.5), vector(0.5, 1, 0.5), vector(1, 1, 1), 32),
	        MaterialResource(vector(0.5, 0.5, 1), vector(0.5, 0.5, 1), vector(1, 1, 1), 32)};
public:
	CollisionDetectionDemoRunner() {
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
        camera.setViewMatrix(matriz_4x4::matrizTraslacion(vector(0.0f, 0.0f, -5.0f)));
	}

	bool init() {
		video = (VideoRunner*) this->getContainer()->getRequiredRunner(VideoRunner::ID);
		audio = (AudioRunner*) this->getContainer()->getRequiredRunner(AudioRunner::ID);

		logger->debug("Initializing renderers");
	    defaultRenderer.setVideoRunner(video);
	    gridRenderer.setVideoRunner(video);
	    skyboxRenderer.setVideoRunner(video);
	    skyboxRenderer.setSize(200);

	    logger->debug("Setting up video %d", video);
		video->setClearColor(0.0, 0.5, 0.0, 0.0);
		video->setAttribute(DEPTH_TEST, true);

		reset();

		logger->debug("Completed initialization");
		return true;
	}

	LoopResult doLoop() {
	    defaultRenderer.clearObjects();
	    defaultRenderer.drawAxis(matriz_4x4::identidad);
	    defaultRenderer.drawSphere(matriz_4x4::matrizTraslacion(sphere[0].getOrigin()), sphere[0].getRadius());
	    defaultRenderer.drawSphere(matriz_4x4::matrizTraslacion(sphere[1].getOrigin()), sphere[1].getRadius());

		defaultRenderer.render(camera);
		skyboxRenderer.render(camera);
		gridRenderer.render(camera);

		return CONTINUE;
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
	            real t = (this->sphere[selectedGeometry].getOrigin().z - line.getOrigin().z) / line.getDirection().z;
	            this->sphere[selectedGeometry].setOrigin(line.getOrigin() + t * line.getDirection());
	        }

	        if(sphere[0].intersects(sphere[1])) {
	            printf("Intersecting!\n");
	        } else {
	            printf("Not intersecting!\n");
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
            logger->info("Line: %s", line.toString().c_str());

            selectedGeometry = -1;
            for(unsigned char index = 0; index < 2; index++) {
                Geometry &current = sphere[index];

                if(current.intersects((Geometry &)line)) {
                    selectedGeometry = index;
                }
            }

            printf("Selected sphere [%d]\n", selectedGeometry);
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
