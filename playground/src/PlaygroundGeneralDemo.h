/*
 * PlaygroundGeneralDemo.h
 *
 *  Created on: Mar 19, 2021
 *      Author: leandro
 */

#ifndef SRC_PLAYGROUNDGENERALDEMO_H_
#define SRC_PLAYGROUNDGENERALDEMO_H_

#include<Playground.h>
#include<OpenGLRunner.h>
#include<AudioRunner.h>

class PlaygroundGeneralDemoRunner: public PlaygroundRunner {
private:
	Logger *logger;
	OpenGLRunner *openGl;
	AudioRunner *audio;

	//Graphical stuff
	vector viewPosition;
	vector lightPosition;
	vector *currentPosition;
	real rotation;

	Source *lightAnnoyingSoundSource;

	TextureResource *pngTexture;
	TextureResource *jpgTexture;

	VertexArrayResource *triangleVertexArray;
	VertexArrayResource *sphereVertexArray;

	GeometryResource *sphereGeometry;
	GeometryResource *triangleGeometry;

	ShaderProgramResource *toonShaderProgram;

	MaterialResource material;
	LightResource light;

public:
	static const unsigned char ID = 101;

public:
	PlaygroundGeneralDemoRunner() :
			material(vector(1.0f, 0.5f, 0.31f), vector(1.0f, 0.5f, 0.31f),
					vector(0.5f, 0.5f, 0.5f), 32.0f), light(lightPosition,
					vector(0.2f, 0.2f, 0.2f), vector(0.5f, 0.5f, 0.5f),
					vector(1.0f, 1.0f, 1.0f), 1.0f) {
		openGl = null;
		audio = null;

		logger = Logger::getLogger("Main.cpp");

		currentPosition = &viewPosition;
		rotation = 0;
		pngTexture = null;
		jpgTexture = null;
		triangleVertexArray = null;
		toonShaderProgram = null;
		triangleGeometry = null;
		sphereVertexArray = null;
		sphereGeometry = null;

		lightAnnoyingSoundSource = null;
	}

	virtual unsigned char getInterests() {
		return KEY_DOWN | KEY_UP | MOUSE_MOVE | MOUSE_WHEEL;
	}

	virtual unsigned char getId() {
		return PlaygroundGeneralDemoRunner::ID;
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
		lightAnnoyingSoundSource = audio->createSource("voltage.wav");
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

		glClearColor(0.0, 0.5, 0.0, 0.0);
		glEnable(GL_DEPTH_TEST);
//			glEnable(GL_CULL_FACE);
//			glCullFace(GL_BACK);

		glUseProgram(null);

		reset();
		return true;
	}

	void logGlError(char *format) {
		GLenum glError;
		while ((glError = glGetError()) != GL_NO_ERROR) {
			logger->error(format, glError);
		}
	}

	virtual LoopResult doLoop() {
		//glBindTexture(GL_TEXTURE_2D, 0);

//			glDisable(GL_LIGHTING);
//			glColor3f(1.0, 1.0, 0.0);
//			glPushMatrix();
//			//light w = 1: spotlight; w = 0 directional light
//			glLightfv(GL_LIGHT0, GL_POSITION, (float *)vector4(lightPosition.x, lightPosition.y, lightPosition.z, 0));
//			glTranslatef(lightPosition.x, lightPosition.y, lightPosition.z);
//			openGl->glSphere(.2);
//			glPopMatrix();

//			glEnable(GL_LIGHTING);

		if (jpgTexture != null) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, jpgTexture->getId());
		}
//
//			glPushMatrix();
//			glTranslatef(-1.5, 2.0, 0.0);
//			glRotatef(rotation, 0.0, 1.0, 0.0);
//			openGl->glDrawGeometry(geometryResource);
//			glPopMatrix();
//
//			glPushMatrix();
//			glTranslatef(1.5, 2.0, 0.0);
//			glRotatef(rotation, 0.0, 1.0, 0.0);
//			openGl->glDrawGeometry(sphereGeometryResource);
//			glPopMatrix();

		if (toonShaderProgram != null) {
			openGl->useProgramResource(toonShaderProgram);
		}

		openGl->setModelMatrix(matriz_4x4::Identidad);
		openGl->sendMatrices();
		openGl->glAxis();

		openGl->setModelMatrix(matriz_4x4::matrizTraslacion(lightPosition));
		openGl->sendMatrices();
		openGl->glSphere(0.1f);

		lightAnnoyingSoundSource->setPosition(
				vector3(lightPosition.x, lightPosition.y, -lightPosition.z));
		audio->updateSource(lightAnnoyingSoundSource);
		audio->updateListener(viewPosition);

		openGl->setMaterial(material);
		vector color(sin(radian(rotation)), cos(radian(rotation)),
				sin(radian(rotation)));
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
		openGl->drawVertexArray(triangleVertexArray);

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
		this->addRunner(new PlaygroundGeneralDemoRunner());
		this->addRunner(new OpenGLRunner());
		this->addRunner(new AudioRunner());

	}
};

#endif /* SRC_PLAYGROUNDGENERALDEMO_H_ */
