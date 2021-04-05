/*
 * TerrainDemo.h
 *
 *  Created on: Mar 26, 2021
 *      Author: leandro
 */

#ifndef SRC_DEMO_TERRAINDEMO_H_
#define SRC_DEMO_TERRAINDEMO_H_

#include<Playground.h>
#include<OpenGLRunner.h>
#include<AudioRunner.h>

#include<ArcBall.h>

#include<vector>
#include<renderers/TerrainRenderer.h>
#include<renderers/SkyboxRenderer.h>



class TerrainDemoRunner: public PlaygroundRunner {
private:
	Logger *logger = Logger::getLogger("TerrainDemoRunner");
	OpenGLRunner *openGl = null;
	AudioRunner *audio = null;
	vector viewPosition;
	MaterialResource material;
	LightResource light;

	Camera camera;
	TerrainRenderer terrainRenderer;
	SkyboxRenderer skyboxRenderer;
	DefaultRenderer defaultRenderer;

	ArcBall arcball;

public:
	static const unsigned char ID = 101;

public:
	TerrainDemoRunner() : material(vector(1.0f, 0.5f, 0.31f), vector(1.0f, 0.5f, 0.31f),
			vector(0.5f, 0.5f, 0.5f), 32.0f), light(viewPosition,
			vector(0.2f, 0.2f, 0.2f), vector(0.5f, 0.5f, 0.5f),
			vector(1.0f, 1.0f, 1.0f), 1.0f){
	}
	virtual unsigned char getInterests() {
		return KEY_DOWN | KEY_UP | MOUSE_MOVE | MOUSE_WHEEL | MOUSE_BUTTON_DOWN | MOUSE_BUTTON_UP | RESIZE;
	}

	virtual unsigned char getId() {
		return GeneralDemoRunner::ID;
	}

	void resize(unsigned int height, unsigned int width) {
		camera.setProjectionMatrix(camera.perspectiveProjection(45.0, (GLfloat) width / (GLfloat) height, 0.1, 400.0));
	}

	void reset() {
		viewPosition = vector(0.0, -0.5f, -6.0);
		camera.setViewMatrix(matriz_4x4::matrizTraslacion(viewPosition));
	}

	virtual bool init() {
		//TODO: Review why can´t use public static IDs properties from each class
		openGl = (OpenGLRunner*) this->getContainer()->getRunner(1);
		audio = (AudioRunner*) this->getContainer()->getRunner(3);

		if (!openGl) {
			logger->error("Could not find openGl runner");
			return false;
		}

		ResourceManager *resourceManager = this->getContainer()->getResourceManager();
		openGl->setClearColor(0.0, 0.5, 0.0, 0.0);
		openGl->setAttribute(DEPTH_TEST, true);
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

		//openGl->setAttribute(CULL_FACE, CULL_FACE_BACK);


		terrainRenderer.setVideoRunner(openGl);
		terrainRenderer.setTerrain((TerrainResource *)resourceManager->load("geometry/terrain/terrain.json", "video/terrain"));

		skyboxRenderer.setVideoRunner(openGl);
		skyboxRenderer.setSize(300);

		defaultRenderer.setVideoRunner(openGl);

		reset();

		logger->info("Done configuring!");
		return true;
	}
	virtual LoopResult doLoop() {
	    defaultRenderer.clearObjects();
	    defaultRenderer.drawAxis(matriz_4x4::Identidad);
	    defaultRenderer.render(camera);

		terrainRenderer.render(camera);
		skyboxRenderer.render(camera);

		return CONTINUE;
	}

	void mouseButtonDown(unsigned char button, int x, int y)
	{
		logger->info("MouseButtonDown %d at <%d, %d>", button, x, y);
		if(button == SDL_BUTTON_LEFT) {
			arcball.startDrag(vector2(x, y), (matriz_3x3)camera.getViewMatrix(), openGl->getScreenWidth(), openGl->getScreenHeight());
		}
	}

	void mouseButtonUp(unsigned char button, int x, int y)
	{
		logger->info("MouseButtonUp %d at <%d, %d>", button, x, y);

		if(button == SDL_BUTTON_LEFT) {
			camera.setViewMatrix(matriz_4x4::matrizBase(arcball.endDrag(vector2(x, y)), viewPosition));
			//logger->info("%s", openGl->getViewMatrix().toString().c_str());
		}
	}

	void mouseWheel(int wheel) {
		logger->verbose("%s", viewPosition.toString().c_str());
		viewPosition += vector(0.0f, 0.0f, wheel);

		camera.setViewMatrix(matriz_4x4::matrizBase((matriz_3x3)camera.getViewMatrix(), viewPosition));
	}

	virtual void mouseMove(int dx, int dy) {
		if(!arcball.isDragging()) {
			viewPosition += vector(0.1f * dx, 0.1f * dy, 0);
			camera.setViewMatrix(matriz_4x4::matrizBase((matriz_3x3)camera.getViewMatrix(), viewPosition));
		} else {
		    camera.setViewMatrix(matriz_4x4::matrizBase(arcball.drag(vector2(dx, dy)), viewPosition));
		}

		//logger->info("%s", openGl->getViewMatrix().toString().c_str());
	}
	virtual void keyDown(unsigned int key) {
		switch (key) {
		case SDLK_SPACE:
			reset();
			break;
		case SDLK_ESCAPE:
			this->getContainer()->stop();
			break;
		case 'f':
		case 'F':
			this->openGl->setFullscreen(!this->openGl->getFullscreen());
			break;
		}
	}

};

class PlaygroundTerrainDemo: public Playground {
public:
	PlaygroundTerrainDemo(const String &resourcesBasePath) :
			Playground(resourcesBasePath) {
	}
	void init() {
		Playground::init();
		this->addRunner(new TerrainDemoRunner());
		this->addRunner(new OpenGLRunner());
		this->addRunner(new AudioRunner());

	}
};


#endif /* SRC_DEMO_TERRAINDEMO_H_ */
