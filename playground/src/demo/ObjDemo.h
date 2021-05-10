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

#include<renderers/DefaultRenderer.h>
#include<renderers/GridRenderer.h>
#include<Math3d.h>


class ObjDemoRunner: public PlaygroundRunner {
	Logger *logger = Logger::getLogger("ObjDemoRunner");
	VideoRunner *video = null;
	AudioRunner *audio = null;

	Camera camera;
	LightResource light;
	MaterialResource material;

	GridRenderer gridRenderer;
	DefaultRenderer defaultRenderer;

	VertexArrayResource *obj;
	TextureResource *texture;

	real rotacion = 0.0f;

	vector posicion;

public:
	ObjDemoRunner() : material(vector(0.2f, 0.2f, 0.2f), vector(0.5f, 0.5f, 0.5f), vector(0.5f, 0.5f, 0.5f), 32.0f),
            light(vector(0.0f, 0.0f, 1.0f), vector(0.3f, 0.3f, 0.3f), vector(0.5f, 0.5f, 0.5f), vector(1.0f, 1.0f, 1.0f), 1.0f){
	    reset();
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
	    posicion = vector(0, 0, -5);
	    light.setPosition(vector(0, 0, -5));
        camera.setViewMatrix(matriz_4x4::matrizTraslacion(vector(0.0f, 0.0f, -5.0f)));
	}

	bool init() {
		video = (VideoRunner*) this->getContainer()->getRequiredRunner(VideoRunner::ID);
		audio = (AudioRunner*) this->getContainer()->getRequiredRunner(AudioRunner::ID);

		gridRenderer.setVideoRunner(video);
	    defaultRenderer.setVideoRunner(video);
	    defaultRenderer.setLight(&light);
	    defaultRenderer.setMaterial(&material);

//	    texture = (TextureResource *)this->getContainer()->getResourceManager()->load("images/fern.png", "video/texture");
//        obj = (VertexArrayResource *)this->getContainer()->getResourceManager()->load("geometry/fern.obj", "video/vertexArray");
        texture = (TextureResource *)this->getContainer()->getResourceManager()->load("images/lowPolyTree.png", "video/texture");
        obj = (VertexArrayResource *)this->getContainer()->getResourceManager()->load("geometry/lowPolyTree.obj", "video/vertexArray");

//	    texture = (TextureResource *)this->getContainer()->getResourceManager()->load("images/lowPolyTree.png", "video/texture");
        //obj = (VertexArrayResource *)this->getContainer()->getResourceManager()->load("geometry/bunny.obj", "video/vertexArray");
//        obj = (VertexArrayResource *)this->getContainer()->getResourceManager()->load("geometry/untitled.obj", "video/vertexArray");

        defaultRenderer.setTexture(texture);


	    video->setClearColor(0.0, 0.5, 0.0, 0.0);
		video->setAttribute(DEPTH_TEST, true);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glEnable( GL_BLEND );


		reset();

		return true;
	}

	LoopResult doLoop() {
	    gridRenderer.render(camera);

		defaultRenderer.clearObjects();
		defaultRenderer.drawAxis(matriz_4x4::identidad, 1.0f);
		defaultRenderer.drawSphere(matriz_4x4::matrizTraslacion(posicion), 0.1f);
		defaultRenderer.drawObject(matriz_4x4::matrizRotacion(0.0, radian(rotacion), 0.0), obj);
		defaultRenderer.render(camera);

		rotacion+=0.1;

		return CONTINUE;
	}

	void mouseWheel(int wheel) {
        camera.setViewMatrix(matriz_4x4::matrizTraslacion(camera.getViewPosition() + vector(0.0f, 0.0f, wheel)));
        posicion = posicion + vector(0.0f, 0.0f, wheel);

        light.setPosition(posicion);
        logger->info("viewMatrix:\n%s\nlight:%s\nposition:%s\n",
                camera.getViewMatrix().toString("%.2f").c_str(),
                light.getPosition().toString("%.2f").c_str(),
                posicion.toString("%.2f").c_str());
	}

	virtual void mouseMove(int dx, int dy) {
        camera.setViewMatrix(matriz_4x4::matrizTraslacion(camera.getViewPosition() + vector(0.1f * dx, 0.1f * dy, 0)));
        light.setPosition(camera.getViewPosition());
        posicion = posicion + vector(0.1f * dx, 0.1f * dy, 0);
        logger->info("viewMatrix:\n%s\nlight:%s\nposition:%s\n",
                camera.getViewMatrix().toString("%.2f").c_str(),
                light.getPosition().toString("%.2f").c_str(),
                posicion.toString("%.2f").c_str());
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
			this->video->setFullscreen(!this->video->getFullscreen());
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
		this->addRunner(new ObjDemoRunner());
	}
};



#endif /* SRC_OBJDEMO_H_ */
