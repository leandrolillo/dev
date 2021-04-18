/*
 * PhysicsDemo.h
 *
 *  Created on: Mar 22, 2021
 *      Author: leandro
 */

#ifndef SRC_OBJDEMO_H_
#define SRC_OBJDEMO_H_

#include <iostream>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGLRunner.h>
#include <AudioRunner.h>
#include<renderers/GridRenderer.h>
#include<Math3d.h>


class ObjDemoRunner: public PlaygroundRunner {
	Logger *logger = Logger::getLogger("PhysicsDemoRunner");
	OpenGLRunner *openGl = null;
	AudioRunner *audio = null;

	Camera camera;
	GridRenderer gridRenderer;
	DefaultRenderer defaultRenderer;

	VertexArrayResource *obj;

public:
	ObjDemoRunner() {
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
        camera.setViewMatrix(matriz_4x4::matrizTraslacion(vector(0.0f, 0.0f, -5.0f)));
	}

	bool init() {
		openGl = (OpenGLRunner*) this->getContainer()->getRunner(1);
		audio = (AudioRunner*) this->getContainer()->getRunner(3);

		gridRenderer.setVideoRunner(openGl);
	    defaultRenderer.setVideoRunner(openGl);

	    openGl->setClearColor(0.0, 0.5, 0.0, 0.0);
		openGl->setAttribute(DEPTH_TEST, true);

		obj = (VertexArrayResource *)this->getContainer()->getResourceManager()->load("geometry/fern.obj");

		reset();

		return true;
	}

	LoopResult doLoop() {
		gridRenderer.render(camera);

		defaultRenderer.clearObjects();
//		defaultRenderer.drawObject(matriz_4x4::identidad, obj);
		defaultRenderer.render(camera);

		return CONTINUE;
	}

	void mouseWheel(int wheel) {
        camera.setViewMatrix(matriz_4x4::matrizTraslacion(camera.getViewPosition() + vector(0.0f, 0.0f, wheel)));

	}

	virtual void mouseMove(int dx, int dy) {
        camera.setViewMatrix(matriz_4x4::matrizTraslacion(camera.getViewPosition() + vector(0.1f * dx, 0.1f * dy, 0)));

	}

	void mouseButtonDown(unsigned char button, int x, int y) {
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

class ObjDemoPlayground: public Playground {
public:
    ObjDemoPlayground(const String &resourcesBasePath) :
			Playground(resourcesBasePath) {
	}
	void init() {
		Playground::init();
		this->addRunner(new OpenGLRunner());
		this->addRunner(new AudioRunner());
		this->addRunner(new PhysicsRunner());
		this->addRunner(new ObjDemoRunner());
	}
};



#endif /* SRC_OBJDEMO_H_ */
