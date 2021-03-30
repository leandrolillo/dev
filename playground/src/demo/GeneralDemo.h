/*
 * PlaygroundGeneralDemo.h
 *
 *  Created on: Mar 19, 2021
 *      Author: leandro
 */

#ifndef SRC_GENERALDEMO_H_
#define SRC_GENERALDEMO_H_

#include<Playground.h>
#include<OpenGLRunner.h>
#include<AudioRunner.h>

#include<vector>

class GeneralDemoRunner: public PlaygroundRunner {
private:
	Logger *logger = null;
	OpenGLRunner *openGl = null;
	AudioRunner *audio = null;

	//Graphical stuff
	vector viewPosition;
	vector lightPosition;
	vector *currentPosition;
	real rotation = 0;

	Source *lightAnnoyingSoundSource = null;

	TextureResource *pngTexture = null;
	TextureResource *jpgTexture = null;

	VertexArrayResource *triangleVertexArray = null;
	VertexArrayResource *sphereVertexArray = null;

	GeometryResource *sphereGeometry = null;
	GeometryResource *triangleGeometry = null;

	ShaderProgramResource *toonShaderProgram = null;

	MaterialResource material;
	LightResource light;
public:
	static const unsigned char ID = 101;

public:
	GeneralDemoRunner() :
			material(vector(1.0f, 0.5f, 0.31f), vector(1.0f, 0.5f, 0.31f),
					vector(0.5f, 0.5f, 0.5f), 32.0f), light(lightPosition,
					vector(0.2f, 0.2f, 0.2f), vector(0.5f, 0.5f, 0.5f),
					vector(1.0f, 1.0f, 1.0f), 1.0f) {

		logger = Logger::getLogger("Main.cpp");

		currentPosition = &viewPosition;
	}

	virtual unsigned char getInterests() {
		return KEY_DOWN | KEY_UP | MOUSE_MOVE | MOUSE_WHEEL | RESIZE;
	}

	virtual unsigned char getId() {
		return GeneralDemoRunner::ID;
	}

	void resize(unsigned int height, unsigned int width) {
		openGl->setProjectionMatrix(openGl->perspectiveProjection(45.0, (GLfloat) width / (GLfloat) height, 0.1, 100.0));
	}

	void reset() {
		viewPosition = vector(0.0, 0.0f, -6.0);
		lightPosition = vector(0.0, 0.0, 0.0);
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

		// demo stuff
		lightAnnoyingSoundSource = audio->createSource("audio/voltage.wav");
		audio->playSource(lightAnnoyingSoundSource);

		pngTexture = (TextureResource*) resourceManager->load(
				"images/TEXTURA.PNG", "video/texture");
		jpgTexture = (TextureResource*) resourceManager->load("images/irs.JPG",
				"video/texture");

		//geometryResource = (GeometryResource *)resourceManager->load("geometry/triangle.json", "video/geometry");
		//sphereGeometryResource = (GeometryResource *)resourceManager->load("geometry/sphere.json", "video/geometry");

		sphereVertexArray = (VertexArrayResource*) resourceManager->load(
				"geometry/sphere.json", "video/vertexArray");
		triangleVertexArray = (VertexArrayResource*) resourceManager->load(
				"geometry/triangle.json", "video/vertexArray");

		toonShaderProgram = (ShaderProgramResource*) resourceManager->load(
				"shaders/toon.330.program.json", "video/shaderProgram");

		openGl->setClearColor(0.0, 0.5, 0.0, 0.0);
		openGl->setAttribute(DEPTH_TEST, true);
		openGl->setAttribute(CULL_FACE, CULL_FACE_BACK);

		reset();
		return true;
	}

	virtual LoopResult doLoop() {

		openGl->setTexture(0, jpgTexture);

		if (toonShaderProgram != null) {
			openGl->useProgramResource(toonShaderProgram);
		}

		openGl->setModelMatrix(matriz_4x4::Identidad);
		openGl->sendMatrices();
		openGl->drawAxis();

		openGl->setModelMatrix(matriz_4x4::matrizTraslacion(lightPosition));
		openGl->sendMatrices();
		openGl->drawSphere(0.1f);

		lightAnnoyingSoundSource->setPosition(
				vector3(lightPosition.x, lightPosition.y, -lightPosition.z));
		audio->updateSource(lightAnnoyingSoundSource);
		audio->updateListener(viewPosition);

		openGl->setMaterial(material);
		vector color(0.25 + 0.75 * sin(radian(rotation)), 0.25 + 0.75 * cos(radian(rotation)), 0.25 + 0.75 * sin(radian(rotation)));
		light.setDiffuse(color);
		light.setSpecular(color);
		light.setAmbient(color);
		light.setPosition(lightPosition);
		openGl->setLight(light);
		openGl->sendVector("viewPosition", viewPosition);

		openGl->setViewMatrix(matriz_4x4::matrizTraslacion(viewPosition));

		openGl->setModelMatrix(
				matriz_4x4::matrizBase(
						matriz_3x3::matrizRotacion(0.0f, radian(rotation),
								0.0f), vector3(2.0, 1.0, 0.0)));
		openGl->sendMatrices();

		openGl->sendUnsignedInt("textureUnit", 0);
		openGl->drawVertexArray(triangleVertexArray);

		openGl->setModelMatrix(
				matriz_4x4::matrizBase(
						matriz_3x3::matrizRotacion(0.0f, radian(rotation),
								0.0f), vector3(-2.0, 1.0, 0.0)));
		openGl->sendMatrices();
		//openGl->drawBox(1, 1, 1);
		openGl->drawVertexArray(sphereVertexArray);

		openGl->useProgramResource(openGl->getDefaultShaderProgram());

		openGl->setMaterial(material);
		openGl->setLight(light);
		openGl->sendVector("viewPosition", viewPosition);

		openGl->setModelMatrix(
				matriz_4x4::matrizBase(
						matriz_3x3::matrizRotacion(0.0f, radian(rotation),
								0.0f), vector3(2.0, -1.0, 0.0)));
		openGl->sendMatrices();
		openGl->sendUnsignedInt("textureUnit", 0);
		openGl->drawBox(1, 1, 1);
		//openGl->drawVertexArray(triangleVertexArray);

		openGl->setModelMatrix(
				matriz_4x4::matrizBase(
						matriz_3x3::matrizRotacion(0.0f, radian(rotation),
								0.0f), vector3(-2.0, -1.0, 0.0)));
		openGl->sendMatrices();
		openGl->drawVertexArray(sphereVertexArray);

		//				printf("p:\n %s\n", projection.toString().c_str());
		//				printf("v:\n %s\n", view.toString().c_str());
		//				printf("m:\n %s\n", model.toString().c_str());
		//				printf("pvm:\n%s\n", pvm.toString().c_str());
		//				printf("-----------\n");
		//				printf("%s\r", viewPosition.toString().c_str());

		rotation += 1;



		return CONTINUE;
	}
	void mouseWheel(int wheel) {
		*currentPosition += vector(0.0f, 0.0f, wheel);
		logger->verbose("%s", (*currentPosition).toString().c_str());
	}

	virtual void mouseMove(int dx, int dy) {
		*currentPosition += vector(0.1f * dx, 0.1f * dy, 0);
		logger->verbose("%s", (*currentPosition).toString().c_str());
	}
	virtual void keyUp(unsigned int key) {
		switch (key) {
		case SDLK_RCTRL:
		case SDLK_LCTRL:
			currentPosition = &viewPosition;
			break;
		}
	}
	virtual void keyDown(unsigned int key) {
		switch (key) {
		case SDLK_RCTRL:
		case SDLK_LCTRL:
			currentPosition = &lightPosition;
			break;
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

class PlaygroundGeneralDemo: public Playground {
public:
	PlaygroundGeneralDemo(const String &resourcesBasePath) :
			Playground(resourcesBasePath) {
	}
	void init() {
		Playground::init();
		this->addRunner(new GeneralDemoRunner());
		this->addRunner(new OpenGLRunner());
		this->addRunner(new AudioRunner());

	}
};

#endif /* SRC_GENERALDEMO_H_ */
