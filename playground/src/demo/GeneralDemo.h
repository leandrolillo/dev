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

#include<renderers/DefaultRenderer.h>

#include<vector>

class GeneralDemoRunner: public PlaygroundRunner {
private:
	Logger *logger = Logger::getLogger("GeneralDemoRunner");
	VideoRunner *video = null;
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

	Camera camera;

	DefaultRenderer defaultRenderer;
	DefaultRenderer toonRenderer;
public:
	GeneralDemoRunner() :
			material(vector(1.0f, 0.5f, 0.31f), vector(1.0f, 0.5f, 0.31f),
					vector(0.5f, 0.5f, 0.5f), 32.0f), light(lightPosition,
					vector(0.2f, 0.2f, 0.2f), vector(0.5f, 0.5f, 0.5f),
					vector(1.0f, 1.0f, 1.0f), 1.0f) {

		currentPosition = &viewPosition;
	}

	virtual unsigned char getInterests() {
		return KEY_DOWN | KEY_UP | MOUSE_MOVE | MOUSE_WHEEL | RESIZE;
	}

	virtual unsigned char getId() {
		return 200;
	}

	void resize(unsigned int height, unsigned int width) {
	    camera.setProjectionMatrix(Camera::perspectiveProjection(45.0, (GLfloat) width / (GLfloat) height, 0.1, 100.0));
	}

	void reset() {
		viewPosition = vector(0.0, 0.0f, -6.0);
		lightPosition = vector(0.0, 0.0, 0.0);
        camera.setViewMatrix(matriz_4x4::matrizTraslacion(viewPosition));
	}

	virtual bool init() {
		video = (VideoRunner*) this->getContainer()->getRequiredRunner(VideoRunner::ID);
		audio = (AudioRunner*) this->getContainer()->getRequiredRunner(AudioRunner::ID);

		if (!video) {
			logger->error("Could not find openGl runner");
			return false;
		}

		ResourceManager *resourceManager = this->getContainer()->getResourceManager();

		// demo stuff
		lightAnnoyingSoundSource = audio->createSource("audio/voltage.wav");
		audio->playSource(lightAnnoyingSoundSource);

		pngTexture = (TextureResource*) resourceManager->load("images/TEXTURA.PNG", "video/texture");
		jpgTexture = (TextureResource*) resourceManager->load("images/irs.JPG", "video/texture");

		//geometryResource = (GeometryResource *)resourceManager->load("geometry/triangle.json", "video/geometry");
		//sphereGeometryResource = (GeometryResource *)resourceManager->load("geometry/sphere.json", "video/geometry");

		sphereVertexArray = (VertexArrayResource*) resourceManager->load(
				"geometry/sphere.json", "video/vertexArray");
		triangleVertexArray = (VertexArrayResource*) resourceManager->load(
				"geometry/triangle.json", "video/vertexArray");

        defaultRenderer.setVideoRunner(video);
        defaultRenderer.setLight(&light);
        defaultRenderer.setMaterial(&material);
        defaultRenderer.setTexture(pngTexture);

        toonRenderer.setShaderProgram((ShaderProgramResource*) resourceManager->load("shaders/toon.330.program.json", "video/shaderProgram"));
        toonRenderer.setVideoRunner(video);
        toonRenderer.setLight(&light);
        toonRenderer.setMaterial(&material);
        toonRenderer.setTexture(pngTexture);


		video->setClearColor(0.0, 0.5, 0.0, 0.0);
		video->setAttribute(DEPTH_TEST, true);
		video->setAttribute(CULL_FACE, CULL_FACE_BACK);

		reset();
		return true;
	}

	virtual LoopResult doLoop() {

	    /**
	     * Update audio
	     */
        lightAnnoyingSoundSource->setPosition(vector3(lightPosition.x, lightPosition.y, -lightPosition.z));
        audio->updateSource(lightAnnoyingSoundSource);
        audio->updateListener(viewPosition);

        /**
         * modulate light
         */
//        vector color(0.25 + 0.75 * sin(radian(rotation)), 0.25 + 0.75 * cos(radian(rotation)), 0.25 + 0.75 * sin(radian(rotation)));
//        light.setDiffuse(color);
//        light.setSpecular(color);
//        light.setAmbient(color);
        light.setPosition(lightPosition);


        /**
         * Render toon objects
         */
        toonRenderer.clearObjects();
	    toonRenderer.drawObject(matriz_4x4::matrizBase(matriz_3x3::matrizRotacion(0.0f, radian(rotation), 0.0f), vector3(2.0, 1.0, 0.0)),
	            triangleVertexArray);
	    toonRenderer.drawObject(matriz_4x4::matrizBase(matriz_3x3::matrizRotacion(0.0f, radian(rotation), 0.0f), vector3(-2.0, 1.0, 0.0)),
	            sphereVertexArray
	    );

	    toonRenderer.render(camera);


        /**
         * Render default renderer objects
         */
	    defaultRenderer.clearObjects();

        //draw axis for viewer reference
        defaultRenderer.drawAxis(matriz_4x4::identidad);

        //draw Light as a sphere
        defaultRenderer.drawSphere(matriz_4x4::matrizTraslacion(lightPosition), 0.1f);

        defaultRenderer.drawObject(matriz_4x4::matrizBase(matriz_3x3::matrizRotacion(0.0f, radian(rotation), 0.0f), vector3(2.0, -1.0, 0.0)),
                triangleVertexArray);

        defaultRenderer.drawSphere(matriz_4x4::matrizBase(matriz_3x3::matrizRotacion(0.0f, radian(rotation), 0.0f), vector3(4.0, -1.0, 0.0)));
        defaultRenderer.drawBox(matriz_4x4::matrizBase(matriz_3x3::matrizRotacion(0.0f, radian(rotation), 0.0f), vector3(-2.0, -1.0, 0.0)));
	    defaultRenderer.render(camera);


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

		if(currentPosition == &viewPosition) {
	        camera.setViewMatrix(matriz_4x4::matrizTraslacion(viewPosition));
		}
	}

	virtual void mouseMove(int dx, int dy) {
		*currentPosition += vector(0.1f * dx, 0.1f * dy, 0);
		logger->verbose("%s", (*currentPosition).toString().c_str());

		if(currentPosition == &viewPosition) {
            camera.setViewMatrix(matriz_4x4::matrizTraslacion(viewPosition));
        }
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
			this->video->setFullscreen(!this->video->getFullscreen());
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
