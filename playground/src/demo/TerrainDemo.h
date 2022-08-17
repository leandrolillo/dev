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

#include<vector>

#include<renderers/DefaultRenderer.h>
#include<renderers/TerrainRenderer.h>
#include<renderers/SkyboxRenderer.h>


#include "inputController/FPSInputController.h"
#include "inputController/ThirdPersonInputController.h"

#include<PhysicsRunner.h>
#include<forces/Gravity.h>

class TerrainFPSInputController : public FPSInputController {
protected:
    TerrainResource *terrain = null;

public:
    TerrainFPSInputController(Camera &cameraReference, matriz &playerTransformReference) : FPSInputController(cameraReference, playerTransformReference) {
    }

    void setTerrain(TerrainResource *terrain) {
        this->terrain = terrain;
    }

protected:
    vector constrainPosition(const vector &position) override {
        vector result = position;
        if(terrain != null) {
            real altitude = 0.5 + terrain->getHeightMap()->heightAt(
                    position.x - floor(position.x / terrain->getHeightMap()->getWidth()) * terrain->getHeightMap()->getWidth(),
                    position.z - floor(position.z / terrain->getHeightMap()->getDepth()) * terrain->getHeightMap()->getDepth()
                    );

            if(result.y < altitude) {
                result.y = altitude;
            }
        }
        return result;
    }
};

class TerrainThirdPersonInputController : public ThirdPersonInputController {
protected:
    TerrainResource *terrain = null;

public:
    TerrainThirdPersonInputController(Camera &cameraReference, matriz &playerTransformReference) : ThirdPersonInputController(cameraReference, playerTransformReference) {
    }

    void setTerrain(TerrainResource *terrain) {
        this->terrain = terrain;
    }

protected:
    vector constrainPosition(const vector &position) override {
        vector result = position;
        if(terrain != null) {
            real altitude = 0.5 + terrain->getHeightMap()->heightAt(
                    position.x - floor(position.x / terrain->getHeightMap()->getWidth()) * terrain->getHeightMap()->getWidth(),
                    position.z - floor(position.z / terrain->getHeightMap()->getDepth()) * terrain->getHeightMap()->getDepth()
                    );

            if(result.y < altitude) {
                result.y = altitude;
            }
        }
        return result;
    }
};

class TerrainCollisionTester : public CollisionTester
{
public:
    Logger *logger = LoggerFactory::getLogger("TerrainCollisionTester");

    bool sphereHeightmap(const Geometry &sphereGeometry, const Geometry &heightMapGeometry) const {
		const Sphere &sphere = (const Sphere &)sphereGeometry;
		const HeightMapGeometry &heightMap = (const HeightMapGeometry &)heightMapGeometry;

		return false;
	}

    std::vector<GeometryContact> sphereHeightmapContact(const Geometry &sphereGeometry, const Geometry &heightMapGeometry) const {
		const Sphere &sphere = (const Sphere &)sphereGeometry;
		const HeightMapGeometry &heightmap = (const HeightMapGeometry &)heightMapGeometry;

        //printf("Checking intersection for %s and %s \n", sphere.toString().c_str(), aabb.toString().c_str());
        vector mins = heightmap.getOrigin() - heightmap.getHalfSizes();
        vector maxs = heightmap.getOrigin() + heightmap.getHalfSizes();

        //printf("mins: %s - maxs: %s\n", mins.toString().c_str(), maxs.toString().c_str());

        vector aabbClosestPoint = vector(std::max(mins.x, std::min(sphere.getOrigin().x, maxs.x)),
                0,
                std::max(mins.z, std::min(sphere.getOrigin().z, maxs.z))
                );

        aabbClosestPoint.y = heightmap.getHeightMap()->heightAt(heightmap.getPosition().x - aabbClosestPoint.x, heightmap.getPosition().z - aabbClosestPoint.z);
        //printf("closest point: %s\n", aabbClosestPoint.toString().c_str());

        vector delta = sphere.getOrigin() - aabbClosestPoint;
        if(delta * delta <= sphere.getRadius() * sphere.getRadius()) {
			delta = sphere.getOrigin() - aabbClosestPoint;
			real distance = delta.modulo();
			vector normal = delta * (1.0 / distance);
			real penetration = sphere.getRadius() - distance;

			return std::vector<GeometryContact> {GeometryContact(&sphere, &heightMapGeometry, aabbClosestPoint, normal, 0.8f,  penetration) };
        }


		return std::vector<GeometryContact>();
	}

};

class TerrainDemoRunner: public PlaygroundRunner {
private:
    /**
     * Playground Stuff
     */
	Logger *logger = LoggerFactory::getLogger("TerrainDemoRunner");
	VideoRunner *video = null;
	AudioRunner *audio = null;
    PhysicsRunner *physics = null;

    /**
     * Input controllers
     */

    TerrainFPSInputController fpsInputController;
    TerrainThirdPersonInputController thirdPersonController;
    InputController *inputController = &fpsInputController;

    Camera camera;
    matriz playerTransform;

    /**
     * This demo stuff
     */

	LightResource light;

	TerrainRenderer terrainRenderer;
	SkyboxRenderer skyboxRenderer;
	DefaultRenderer defaultRenderer;

	TerrainResource *terrain = null;
	std::unique_ptr<HierarchicalGeometry> terrainBoundingVolume;

	std::vector<std::unique_ptr<BulletParticle>> particles;
	Gravity gravity = Gravity(vector(0.0, -9.8, 0.0));
public:
	TerrainDemoRunner() : light(vector(0, 0, 0),
			vector(0.2f, 0.2f, 0.2f), vector(0.2f, 0.2f, 0.2f),
			vector(0.1f, 0.1f, 0.1f), 1.0f),
            fpsInputController(camera, playerTransform),
            thirdPersonController(camera, playerTransform) {
	}
	virtual unsigned char getInterests() {
		return KEY_DOWN | KEY_UP | MOUSE_MOVE | MOUSE_WHEEL | MOUSE_BUTTON_DOWN | MOUSE_BUTTON_UP | RESIZE;
	}

	virtual unsigned char getId() {
		return 200;
	}

    virtual bool init() {
        video = (VideoRunner*) this->getContainer()->getRequiredRunner(VideoRunner::ID);
        audio = (AudioRunner*) this->getContainer()->getRequiredRunner(AudioRunner::ID);
        physics = (PhysicsRunner *)this->getContainer()->getRequiredRunner(PhysicsRunner::ID);


        if (!video) {
            logger->error("Could not find openGl runner");
            return false;
        }

        video->enable(RELATIVE_MOUSE_MODE, 0);
        video->setClearColor(0.0, 0.5, 0.0, 0.0);
        video->enable(DEPTH_TEST, true);
        //glPolygonMode( GL_BACK, GL_LINE );
        video->enable(CULL_FACE, CULL_FACE_BACK);
        video->enable(BLEND, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



        ResourceManager *resourceManager = this->getContainer()->getResourceManager();
        terrain = (TerrainResource *)resourceManager->load("geometry/terrain/terrain.json", "video/terrain");

        fpsInputController.setTerrain(terrain);
        thirdPersonController.setTerrain(terrain);

        terrainRenderer.setVideoRunner(video);
        terrainRenderer.setLight(&light);
        terrainRenderer.addTerrain(vector(0, 0, 0), terrain);
        terrainRenderer.addTerrain(vector(-terrain->getHeightMap()->getWidth(), 0, 0), terrain);
        terrainRenderer.addTerrain(vector(0, 0, -terrain->getHeightMap()->getDepth()), terrain);
        terrainRenderer.addTerrain(vector(-terrain->getHeightMap()->getWidth(), 0, -terrain->getHeightMap()->getDepth()), terrain);

        terrainBoundingVolume = std::unique_ptr<HierarchicalGeometry>(new HierarchicalGeometry(
        		new AABB(vector(0, 0, 0),
        				vector(terrain->getHeightMap()->getWidth(),
        						terrain->getHeightMap()->getHeight(), terrain->getHeightMap()->getDepth()))));

        vector terrainHalfSizes = vector(terrain->getHeightMap()->getWidth() * 0.5,
                terrain->getHeightMap()->getHeight() * 0.5,
                terrain->getHeightMap()->getDepth() * 0.5);

        terrainBoundingVolume->addChildren(new HierarchicalGeometry(
                new AABB(vector(0, 0, 0) + terrainHalfSizes, terrainHalfSizes),
				new HeightMapGeometry(vector(0, 0, 0) + terrainHalfSizes, terrain->getHeightMap()))
        );

        terrainBoundingVolume->addChildren(new HierarchicalGeometry(
                new AABB(vector(-terrain->getHeightMap()->getWidth(), 0, 0) + terrainHalfSizes, terrainHalfSizes),
				new HeightMapGeometry(vector(-terrain->getHeightMap()->getWidth(), 0, 0) + terrainHalfSizes, terrain->getHeightMap()))
        );

        terrainBoundingVolume->addChildren(new HierarchicalGeometry(
                new AABB(vector(0, 0, -terrain->getHeightMap()->getDepth()) + terrainHalfSizes, terrainHalfSizes),
				new HeightMapGeometry(vector(0, 0, -terrain->getHeightMap()->getDepth()) + terrainHalfSizes, terrain->getHeightMap()))
        );

        terrainBoundingVolume->addChildren(new HierarchicalGeometry(
                new AABB(vector(-terrain->getHeightMap()->getWidth(), 0, -terrain->getHeightMap()->getDepth()) + terrainHalfSizes, terrainHalfSizes),
				new HeightMapGeometry(vector(-terrain->getHeightMap()->getWidth(), 0, -terrain->getHeightMap()->getDepth()) + terrainHalfSizes, terrain->getHeightMap()))
        );

        skyboxRenderer.setVideoRunner(video);
        skyboxRenderer.setSize(500);

        defaultRenderer.setVideoRunner(video);

        physics->getParticleManager()->getCollisionDetector().getIntersectionTester()->addIntersectionTest(GeometryType::SPHERE, GeometryType::HEIGHTMAP, static_cast<bool (CollisionTester::*)(const Geometry &, const Geometry &) const> (&TerrainCollisionTester::sphereHeightmap));
        physics->getParticleManager()->getCollisionDetector().getIntersectionTester()->addContactTest(GeometryType::SPHERE, GeometryType::HEIGHTMAP, static_cast<std::vector<GeometryContact> (CollisionTester::*)(const Geometry &, const Geometry &) const> (&TerrainCollisionTester::sphereHeightmapContact));

        physics->getParticleManager()->addForce(&this->gravity);
        physics->getParticleManager()->addScenery(terrainBoundingVolume.get());

        for(int index = 0; index < numberOfParticles; index++) {
            particles.push_back(std::unique_ptr<BulletParticle>(new BulletParticle()));
            particles.back()->setStatus(false);
            //particles.back()->setRunner(this);

            physics->getParticleManager()->addParticle(particles.back().get());
        }


        reset();

        logger->info("Done configuring!");
        return true;
    }

	void resize(unsigned int height, unsigned int width) {
		camera.setProjectionMatrix(Camera::perspectiveProjection(45.0, (GLfloat) width / (GLfloat) height, 0.1, 1000.0));
	}

	void reset() {
		//light.setPosition(position);
		inputController->reset();

        for(auto &particle : this->particles) {
            particle->setStatus(false);
        }

	}

	virtual LoopResult doLoop() {
	    defaultRenderer.clear();
	    defaultRenderer.drawAxes(matriz_4x4::identidad);

	    // draw avatar
	    defaultRenderer.drawSphere(playerTransform, 0.5);

	    // draw spheres
        for(auto &particle : this->particles)
        {
            if(particle->getStatus() == true) {
//                defaultRenderer.setMaterial(&material);
                defaultRenderer.drawSphere(matriz_4x4::matrizTraslacion(particle->getPosition()), 0.1);
            }
        }


	    // draw terrain bounding boxes.
	    static MaterialResource materials[] = {
	    		MaterialResource(vector(1, 0, 0), vector(1, 0, 0), vector(1, 0, 0), 1.0, 0.2),
				MaterialResource(vector(0, 1, 0), vector(0, 1, 0), vector(0, 1, 0), 1.0, 0.2),
				MaterialResource(vector(0, 0, 1), vector(0, 0, 1), vector(0, 0, 1), 1.0, 0.2)
	    };


/**
 * manually render heightmap hierarchical bounding box (2 levels)
 */
		for(auto &particle : this->particles) {
				if(particle->getStatus()) {
				Sphere &sphere = *((Sphere *)particle->getGeometry());

				for(int index = 0; index < terrainBoundingVolume->getChildren().size(); index++)
				{
					const AABB &childBB = ((const AABB &)((HierarchicalGeometry *)terrainBoundingVolume->getChildren()[index].get())->getBoundingVolume());
					if(physics->getParticleManager()->getCollisionDetector().getIntersectionTester()->sphereAabb(sphere, childBB)) {
						vector halfSizes = childBB.getHalfSizes();
						defaultRenderer.setMaterial(&materials[index % 3]);
						defaultRenderer.drawBox(matriz::matrizTraslacion(childBB.getOrigin()),
										(real)1.99 * halfSizes.x,
										(real)1.99 * halfSizes.y,
										(real)1.99 * halfSizes.z);
					}

				}

				defaultRenderer.setMaterial(&materials[0]);
				if(physics->getParticleManager()->getCollisionDetector().getIntersectionTester()->sphereAabb(sphere, ((AABB &)terrainBoundingVolume->getBoundingVolume()))) {
					vector halfSizes = ((AABB &)terrainBoundingVolume->getBoundingVolume()).getHalfSizes();
					defaultRenderer.drawBox(matriz::matrizTraslacion(terrainBoundingVolume->getOrigin()),
									(real)2 * halfSizes.x,
									(real)2 * halfSizes.y,
									(real)2 * halfSizes.z);

				}

			}
		}
		defaultRenderer.setMaterial(null);


		terrainRenderer.render(camera);
		skyboxRenderer.render(camera);
	    defaultRenderer.render(camera);

		inputController->update(0);

		return LoopResult::CONTINUE;
	}

    void fire(const vector &position, bool booble = false) {
        BulletParticle *bullet = null;

        logger->debug("Iterating particles");
        for(auto &particle : particles)
        {
            if(!particle->getStatus()) {
                bullet = particle.get();
                break;
            }
        }

        if(bullet != null) {
            bullet->setPosition(position);

            if(booble) {
                bullet->setVelocity(camera.getOrientation().columna(2).normalizado() * -20);
                bullet->setMass(0.1);
                bullet->setDamping(0.99f);
            } else {
                bullet->setVelocity(camera.getOrientation().columna(2).normalizado() * -4);
                bullet->setDamping(0.8f);
                bullet->setMass(-0.001);
            }

            bullet->setAcceleration(vector(0, 0, 0));

            bullet->setStatus(true);

            //logger->info("bullet at position: %s", bullet->getPosition().toString("%.2f").c_str());

        } else {
            logger->debug("no particle found");
        }
    }

	void mouseButtonDown(unsigned char button, int x, int y)
	{
	    if(button == SDL_BUTTON_LEFT) {
	        fire(camera.getPosition(), true);
	    } else {
	        fire(camera.getPosition());
	    }

	    inputController->mouseButtonDown(button, x, y);
	}

	void mouseButtonUp(unsigned char button, int x, int y)
	{
	    inputController->mouseButtonUp(button, x, y);
	}

    virtual void mouseMove(int x, int y, int dx, int dy) {
        if(dx != 0 || dy != 0) {
            inputController->mouseMove(x, y, dx, dy);
            this->video->setMousePosition(video->getScreenWidth() >> 1, video->getScreenHeight() >> 1);
        }
    }

	void mouseWheel(int wheel) {
	    inputController->mouseWheel(wheel);
	}

    virtual void keyDown(unsigned int key, unsigned int keyModifier) {
        inputController->keyDown(key, keyModifier);
    }

    virtual void keyUp(unsigned int key, unsigned int keyModifier) {
        inputController->keyUp(key, keyModifier);
        switch (key) {
            case '1':
                this->inputController = &fpsInputController;
                break;
            case '2':
                this->inputController = &thirdPersonController;
                break;
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
		this->addRunner(new OpenGLRunner());
		this->addRunner(new AudioRunner());
        this->addRunner(new PhysicsRunner());
        this->addRunner(new TerrainDemoRunner());
	}
};


#endif /* SRC_DEMO_TERRAINDEMO_H_ */
