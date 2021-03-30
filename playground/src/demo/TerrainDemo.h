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

class TerrainRenderer {
private:
	ShaderProgramResource *shader = null;
	TerrainResource *terrain = null;

public:
//	TerrainRenderer() {
//	}
//	TerrainRenderer(VideoRunner *videoRunner, ShaderProgramResource *shader, TerrainResource *terrains)
//	{
//		this->videoRunner = videoRunner;
//		this->shader = shader;
//		this->terrain = terrain;
//	}

	void setShaderProgram(ShaderProgramResource *shaderProgramResource) {
			this->shader = shaderProgramResource;
	}

	void setTerrain(TerrainResource *terrain) {
		this->terrain = terrain;
	}

	void render(VideoRunner *videoRunner, const LightResource &light) {
		videoRunner->useProgramResource(shader);


		videoRunner->setTexture(0, terrain->getA());
		videoRunner->sendUnsignedInt("background", 0);

		videoRunner->setTexture(1, terrain->getR());
		videoRunner->sendUnsignedInt("textureR", 1);

		videoRunner->setTexture(2, terrain->getG());
		videoRunner->sendUnsignedInt("textureG", 2);

		videoRunner->setTexture(3, terrain->getB());
		videoRunner->sendUnsignedInt("textureB", 3);

		videoRunner->setTexture(4, terrain->getMap());
		videoRunner->sendUnsignedInt("blendMap", 4);

		videoRunner->setLight(light);

		videoRunner->sendMatrices();
		videoRunner->drawVertexArray(terrain->getModel());
	}
};


class TerrainDemoRunner: public PlaygroundRunner {
private:
	Logger *logger = Logger::getLogger("TerrainDemoRunner");
	OpenGLRunner *openGl = null;
	AudioRunner *audio = null;
	vector viewPosition;
	TerrainRenderer terrainRenderer;
	MaterialResource material;
	LightResource light;

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
		openGl->setProjectionMatrix(openGl->perspectiveProjection(45.0, (GLfloat) width / (GLfloat) height, 0.1, 100.0));
	}

	void reset() {
		viewPosition = vector(0.0, 0.0f, -6.0);
	}

	virtual bool init() {
		//TODO: Review why can´t use public static IDs properties from each class
		openGl = (OpenGLRunner*) this->getContainer()->getRunner(1);
		audio = (AudioRunner*) this->getContainer()->getRunner(3);

		if (!openGl) {
			logger->error("Could not find openGl runner");
			return false;
		}

		ResourceManager *resourceManager =
				this->getContainer()->getResourceManager();
		openGl->setClearColor(0.0, 0.5, 0.0, 0.0);
		openGl->setAttribute(DEPTH_TEST, true);
		//openGl->setAttribute(CULL_FACE, CULL_FACE_BACK);


		terrainRenderer.setShaderProgram((ShaderProgramResource *)resourceManager->load("shaders/terrain/terrain.program.json", "video/shaderProgram"));
		terrainRenderer.setTerrain((TerrainResource *)resourceManager->load("geometry/terrain/terrain.json", "video/terrain"));

		reset();
		return true;
	}
	virtual LoopResult doLoop() {
		openGl->setViewMatrix(matriz_4x4::matrizTraslacion(this->viewPosition));
		openGl->setModelMatrix(matriz_4x4::Identidad);
		openGl->sendMatrices();
		openGl->drawAxis(1.0);

		terrainRenderer.render(openGl, light);
		return CONTINUE;
	}

	void mouseButtonDown(unsigned char button, int x, int y)
	{
		logger->info("MouseButtonDown %d at <%d, %d>", button, x, y);
		if(button == SDL_BUTTON_LEFT) {
			arcball.startDrag(vector2(x, y));
		}
	}

	void mouseButtonUp(unsigned char button, int x, int y)
	{
		logger->info("MouseButtonUp %d at <%d, %d>", button, x, y);

		if(button == SDL_BUTTON_LEFT) {
			arcball.endDrag(vector2(x, y));
		}
	}

	void mouseWheel(int wheel) {
		logger->verbose("%s", viewPosition.toString().c_str());
		viewPosition += vector(0.0f, 0.0f, wheel);
	}

	virtual void mouseMove(int dx, int dy) {
		arcball.drag(vector2(dx, dy));

		viewPosition += vector(0.1f * dx, 0.1f * dy, 0);
		logger->verbose("%s", viewPosition.toString().c_str());
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
