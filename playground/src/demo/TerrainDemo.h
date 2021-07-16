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

#include<renderers/DefaultRenderer.h>
#include<renderers/TerrainRenderer.h>
#include<renderers/SkyboxRenderer.h>


class InputController {
public:
    virtual ~InputController() {}
    virtual void update(real dt) {}
    virtual void reset() {}
    virtual void mouseButtonDown(unsigned char button, int x, int y) {}
    virtual void mouseButtonUp(unsigned char button, int x, int y) {}
    virtual void mouseMove(int x, int y, int dx, int dy) { }
    virtual void mouseWheel(int wheel) {}
    virtual void keyDown(unsigned int key, unsigned int keyModifier) {}
    virtual void keyUp(unsigned int key, unsigned int keyModifier) {}
};

class FPSInputController : public InputController {
    Camera &camera;
    TerrainResource *terrain = null;
    real pitch = 0;
    real yaw = 0;
    vector viewPosition;
    matriz_3x3 viewMatrix;
    vector viewVelocity;
public:

    FPSInputController(Camera &cameraReference) : camera(cameraReference) {
        reset();
    }

    void setPitch(real pitch) {
        this->pitch = pitch;
        refreshViewMatrix();
    }

    real getPitch() const {
        return this->pitch;
    }

    void setYaw(real yaw) {
        this->yaw = yaw;
        refreshViewMatrix();
    }

    real getYaw() const {
        return this->yaw;
    }

    void setTerrain(TerrainResource *terrain) {
        this->terrain = terrain;
    }

    void setPosition(const vector &viewPosition) {
        this->viewPosition = viewPosition;
    }

    void setVelocity(const vector &viewVelocity) {
        this->viewVelocity = viewVelocity;
    }

    void update(real dt) override {
        viewPosition += viewMatrix.traspuesta() * viewVelocity;
        viewPosition.y = terrain->getHeightMap()->heightAt(
                viewPosition.x - floor(viewPosition.x / terrain->getHeightMap()->getWidth()) * terrain->getHeightMap()->getWidth(),
                viewPosition.z - floor(viewPosition.z / terrain->getHeightMap()->getDepth()) * terrain->getHeightMap()->getDepth()
                );

        camera.setViewMatrix((matriz_4x4)viewMatrix * matriz_4x4::matrizTraslacion(-viewPosition - vector(0, 1, 0)));
    }

    void reset() override {
        this->viewPosition = vector(0, 0, 0);
        this->viewVelocity = vector(0, 0, 0);
        this->setPitch(0);
        this->setYaw(0);
    }

    void mouseButtonDown(unsigned char button, int x, int y) override
    {
//        logger->info("MouseButtonDown %d at <%d, %d>", button, x, y);
//        if(button == SDL_BUTTON_LEFT) {
//            arcball.startDrag(vector2(x, y), (matriz_3x3)camera.getViewMatrix(), video->getScreenWidth(), video->getScreenHeight());
//        }
    }

    void mouseButtonUp(unsigned char button, int x, int y) override
    {
//        logger->info("MouseButtonUp %d at <%d, %d>", button, x, y);
//
//        if(button == SDL_BUTTON_LEFT) {
//            camera.setViewMatrix(matriz_4x4::matrizBase(arcball.endDrag(vector2(x, y)), viewPosition));
//            //logger->info("%s", openGl->getViewMatrix().toString().c_str());
//        }
    }

    void mouseMove(int x, int y, int dx, int dy) override {
        this->setPitch(this->getPitch() + dy);
        this->setYaw(this->getYaw() + dx);
    }


    void mouseWheel(int wheel) override {
//        logger->verbose("%s", viewPosition.toString().c_str());
//        move(vector(0.0f, 0.0f, wheel));
    }

    void keyUp(unsigned int key, unsigned int keyModifier) override {
        switch (key) {
            case 'w':
            case 'W':
            case 's':
            case 'S':
                viewVelocity.z = 0;;
                break;
            case 'a':
            case 'A':
            case 'd':
            case 'D':
                viewVelocity.x = 0;
                break;
        }
    }

    void keyDown(unsigned int key, unsigned int keyModifier) override {
        switch (key) {
            case 'w':
            case 'W':
                viewVelocity.z = -0.1;
                break;
            case 's':
            case 'S':
                viewVelocity.z = 0.1;
                break;
            case 'a':
            case 'A':
                viewVelocity.x = -0.1;
                break;
            case 'd':
            case 'D':
                viewVelocity.x = 0.1;
                break;
        }
    }

private:
    void refreshViewMatrix() {
        viewMatrix = matriz_3x3::matrizRotacion(radian(pitch), vector(1, 0, 0)) * matriz_3x3::matrizRotacion(radian(yaw), vector(0, 1, 0));
    }


};

class ThirdPersonController : public InputController {

};

class TerrainDemoRunner: public PlaygroundRunner {
private:
	Logger *logger = LoggerFactory::getLogger("TerrainDemoRunner");
	VideoRunner *video = null;
	AudioRunner *audio = null;
	LightResource light;

	Camera camera;
	TerrainRenderer terrainRenderer;
	SkyboxRenderer skyboxRenderer;
	DefaultRenderer defaultRenderer;

	ArcBall arcball;

	TerrainResource *terrain = null;

	FPSInputController fpsInputController;
	ThirdPersonController thirdPersonController;
	InputController &inputController = fpsInputController;
public:
	TerrainDemoRunner() : light(vector(0, 0, 0),
			vector(0.2f, 0.2f, 0.2f), vector(0.2f, 0.2f, 0.2f),
			vector(0.1f, 0.1f, 0.1f), 1.0f), fpsInputController(camera) {
	}
	virtual unsigned char getInterests() {
		return KEY_DOWN | KEY_UP | MOUSE_MOVE | MOUSE_WHEEL | MOUSE_BUTTON_DOWN | MOUSE_BUTTON_UP | RESIZE;
	}

	virtual unsigned char getId() {
		return 200;
	}

	void resize(unsigned int height, unsigned int width) {
		camera.setProjectionMatrix(Camera::perspectiveProjection(45.0, (GLfloat) width / (GLfloat) height, 0.1, 1000.0));
	}

	void reset() {
		//light.setPosition(viewPosition);
		inputController.reset();
	}

	virtual bool init() {
		//TODO: Review why can´t use public static IDs properties from each class
		video = (VideoRunner*) this->getContainer()->getRequiredRunner(VideoRunner::ID);
		audio = (AudioRunner*) this->getContainer()->getRequiredRunner(AudioRunner::ID);

		if (!video) {
			logger->error("Could not find openGl runner");
			return false;
		}

		ResourceManager *resourceManager = this->getContainer()->getResourceManager();
		video->setClearColor(0.0, 0.5, 0.0, 0.0);
		video->enable(DEPTH_TEST, true);
		glPolygonMode( GL_BACK, GL_LINE );

		video->enable(CULL_FACE, CULL_FACE_BACK);


		terrainRenderer.setVideoRunner(video);
		terrainRenderer.setLight(&light);

		terrain = (TerrainResource *)resourceManager->load("geometry/terrain/terrain.json", "video/terrain");

		fpsInputController.setTerrain(terrain);

		terrainRenderer.addTerrain(vector(0, 0, 0), terrain);
		terrainRenderer.addTerrain(vector(-terrain->getHeightMap()->getWidth(), 0, 0), terrain);
		terrainRenderer.addTerrain(vector(0, 0, -terrain->getHeightMap()->getDepth()), terrain);
		terrainRenderer.addTerrain(vector(-terrain->getHeightMap()->getWidth(), 0, -terrain->getHeightMap()->getDepth()), terrain);

		skyboxRenderer.setVideoRunner(video);
		skyboxRenderer.setSize(700);

		defaultRenderer.setVideoRunner(video);

		reset();

		logger->info("Done configuring!");
		return true;
	}
	virtual LoopResult doLoop() {
	    defaultRenderer.clear();
	    defaultRenderer.drawAxes(matriz_4x4::identidad);
	    defaultRenderer.render(camera);

		terrainRenderer.render(camera);
		skyboxRenderer.render(camera);

		inputController.update(0);

		return LoopResult::CONTINUE;
	}

	void mouseButtonDown(unsigned char button, int x, int y)
	{
	    inputController.mouseButtonDown(button, x, y);
	}

	void mouseButtonUp(unsigned char button, int x, int y)
	{
	    inputController.mouseButtonUp(button, x, y);
	}

    virtual void mouseMove(int x, int y, int dx, int dy) {
        inputController.mouseMove(x, y, dx, dy);
    }


	void mouseWheel(int wheel) {
	    inputController.mouseWheel(wheel);
	}

    virtual void keyDown(unsigned int key, unsigned int keyModifier) {
        inputController.keyDown(key, keyModifier);
    }

    virtual void keyUp(unsigned int key, unsigned int keyModifier) {
        inputController.keyUp(key, keyModifier);
        switch (key) {
            case SDLK_SPACE:
                reset();
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
