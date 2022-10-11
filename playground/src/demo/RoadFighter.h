/*
 * PhysicsDemo.h
 *
 *  Created on: Mar 22, 2021
 *      Author: leandro
 */

#ifndef SRC_BATTLEROAD_H_
#define SRC_BATTLEROAD_H_

#include "base/BaseDemo.h"

class RoadFighterRunner: public BaseDemoRunner {
//    ParticleManager particleManager;
//    const CollisionTester &intersectionTester = *(particleManager.getCollisionDetector().getIntersectionTester());

	VertexArrayResource *car = null;
	VertexArrayResource *axes = null;
//	TextureResource *texture = null;

public:
    bool init() override {
    	logger->info("-----------------");
    	logger->info("| Road fighter");
    	logger->info("-----------------");

    	if(!BaseDemoRunner::init()) {
    		return false;
    	}

    	//video->enable(CULL_FACE, GL_NONE);

        if((axes = (VertexArrayResource *)this->getResourceManager()->load("roadFighter/axes.obj", "video/vertexArray")) == null) {
        	logger->error("Could not load axes model");
        	return false;
        }

        if((car = (VertexArrayResource *)this->getResourceManager()->load("roadFighter/corvette.obj", "video/vertexArray")) == null) {
        	logger->error("Could not load car model");
        	return false;
        }

        return true;
    }


    LoopResult doLoop() override {
        defaultRenderer.clear();
        defaultRenderer.drawAxes(matriz_4x4::identidad);

        defaultRenderer.drawObject(matrix_4x4::identidad, car);
        defaultRenderer.drawObject(matrix_4x4::identidad, axes);

        defaultRenderer.render(camera);

        return LoopResult::CONTINUE;
    }

//    void onMouseMove(int x, int y, int dx, int dy) override {
//    }
//
//    void onMouseButtonUp(unsigned char button, int x, int y) override {
//    }
//
//	void onMouseWheel(int wheel) override {
//		logger->info("Mouse wheel %d", wheel);
//	    camera.setPosition(camera.getPosition() - vector(0.0, 0.0, std::min(1.0, 0.1 * wheel)));
//	    logger->info("Camera position: %s", camera.getPosition().toString().c_str());
//	}
//
//
//    void onMouseButtonDown(unsigned char button, int x, int y) override {
//    }
//
//    void onKeyDown(unsigned int key, unsigned int keyModifier) override {
//        switch (key) {
//			case '+':
//				camera.setPosition(camera.getPosition() - vector(0.0, 0.0, 0.1));
//			break;
//			case '-':
//				camera.setPosition(camera.getPosition() + vector(0.0, 0.0, 0.1));
//			break;
//			case SDLK_LEFT:
//				camera.setPosition(camera.getPosition() - vector(0.1, 0.0, 0.0));
//			break;
//			case SDLK_RIGHT:
//				camera.setPosition(camera.getPosition() + vector(0.1, 0.0, 0.0));
//			break;
//        	case SDLK_UP:
//        		camera.setPosition(camera.getPosition() + vector(0.0, 0.1, 0.0));
//        	break;
//        	case SDLK_DOWN:
//        		camera.setPosition(camera.getPosition() - vector(0.0, 0.1, 0.0));
//        		break;
//            case SDLK_BACKSPACE:
//                reset();
//                break;
//            case SDLK_SPACE:
//                break;
//        }
//    }

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
