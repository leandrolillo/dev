/*
 * PhysicsDemo.h
 *
 *  Created on: Mar 22, 2021
 *      Author: leandro
 */

#ifndef SRC_BATTLEROAD_H_
#define SRC_BATTLEROAD_H_

#include <iostream>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGLRunner.h>
#include <AudioRunner.h>
#include<renderers/DefaultRenderer.h>
#include<renderers/SkyboxRenderer.h>
#include<renderers/GridRenderer.h>
#include<renderers/geometry/GeometryRenderer.h>

#include<Math3d.h>
#include<Geometry.h>

#include<InvalidArgumentException.h>
#include<Camera.h>

class RoadFighterRunner: public PlaygroundRunner {
    Logger *logger = LoggerFactory::getLogger("CollisionDetectionDemoRunner");
    VideoRunner *video = null;
    AudioRunner *audio = null;

    //DefaultRenderer defaultRenderer;

    ParticleManager particleManager;
    const CollisionTester &intersectionTester = *(particleManager.getCollisionDetector().getIntersectionTester());
    Camera camera;

	VertexArrayResource *car = null;
	TextureResource *texture = null;

public:
    RoadFighterRunner() {
        logger->addAppender(LoggerFactory::getAppender("stdout"));
    }

    unsigned char getId() const override {
        return 200;
    }

    virtual unsigned char getInterests() override {
        return KEY_DOWN | KEY_UP | MOUSE_MOVE | MOUSE_WHEEL | MOUSE_BUTTON_DOWN | MOUSE_BUTTON_UP | RESIZE;
    }

    void onResize(unsigned int height, unsigned int width) override {
        camera.setProjectionMatrix(Camera::perspectiveProjection(45.0, (GLfloat) width / (GLfloat) height, 0.1, 300.0));
    }

    void reset() {
        camera.setViewMatrix(matriz_4x4::traslacion(vector(0.0f, -0.5f, -10.0f)));
    }

    bool init() override {
        video = (VideoRunner*) this->getContainer()->getRequiredRunner(VideoRunner::ID);
        audio = (AudioRunner*) this->getContainer()->getRequiredRunner(AudioRunner::ID);

        this->video->resize(800, 600);

        logger->debug("Initializing renderers");
        //defaultRenderer.setVideoRunner(video);

        logger->debug("Setting up video %d", video);
        video->setClearColor(0.0, 0.5, 0.0, 0.0);
        video->enable(DEPTH_TEST, true);
        video->enable(CULL_FACE, CULL_FACE_BACK);
        video->enable(BLEND, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        car = (VertexArrayResource *)this->getContainer()->getResourceManager()->load("roadFighter/corvette.obj", "video/vertexArray");

        reset();

        logger->debug("Completed initialization");
        return true;
    }


    LoopResult doLoop() override {
        //defaultRenderer.clear();
//        defaultRenderer.drawAxes(matriz_4x4::identidad);
//
//        defaultRenderer.drawObject(matrix_4x4::identidad, car);

        return LoopResult::CONTINUE;
    }

    void onMouseMove(int x, int y, int dx, int dy) override {
    }

    void onMouseButtonUp(unsigned char button, int x, int y) override {
    }

	void onMouseWheel(int wheel) override {
		logger->info("Mouse wheel %d", wheel);
	    camera.setPosition(camera.getPosition() - vector(0.0, 0.0, std::min(1.0, 0.1 * wheel)));
	    logger->info("Camera position: %s", camera.getPosition().toString().c_str());
	}


    void onMouseButtonDown(unsigned char button, int x, int y) override {
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

class RoadFighter: public Playground {
public:
	RoadFighter(const String &resourcesBasePath) : Playground(resourcesBasePath) {
    }
    void init() override {
        Playground::init();
        this->addRunner(new OpenGLRunner());
        this->addRunner(new AudioRunner());
        this->addRunner(new RoadFighterRunner());
    }
};

#endif
