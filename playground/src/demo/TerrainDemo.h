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


class TerrainDemoRunner: public PlaygroundRunner {
private:
    /**
     * Playground Stuff
     */
	Logger *logger = LoggerFactory::getLogger("TerrainDemoRunner");
	VideoRunner *video = nullptr;
	AudioRunner *audio = nullptr;
    PhysicsRunner *physics = nullptr;

    bool debug = false;

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

	VertexArrayResource *tree = nullptr;
	TextureResource *treeTexture = nullptr;
	std::vector<matriz_4x4> treePositions;
	std::vector<std::unique_ptr<AABB>> treeBoundingVolumes;

	TerrainResource *terrain = nullptr;
	std::unique_ptr<HierarchicalGeometry> terrainBoundingVolume;

	std::vector<std::unique_ptr<BulletParticle>> particles;
	Gravity gravity = Gravity(vector(0.0, -9.8, 0.0));

    MaterialResource black {vector(0, 0, 0), vector(0, 0, 0), vector(0, 0, 0), 1.0, 0.2 };
    MaterialResource white {vector(1, 1, 1), vector(1, 1, 1), vector(1, 1, 1), 1.0, 0.2 };
	MaterialResource red {vector(1, 0, 0), vector(1, 0, 0), vector(1, 0, 0), 1.0, 0.2 };
	MaterialResource green {vector(0, 1, 0), vector(0, 1, 0), vector(0, 1, 0), 1.0, 0.2 };
	MaterialResource blue {vector(0, 0, 1), vector(0, 0, 1), vector(0, 0, 1), 1.0, 0.2 };

	MaterialResource *materials[3] = { &red, &green, &blue};

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

        treeTexture = (TextureResource *)this->getContainer()->getResourceManager()->load("images/lowPolyTree.png", "video/texture");
        tree = (VertexArrayResource *)this->getContainer()->getResourceManager()->load("geometry/lowPolyTree.obj", "video/vertexArray");



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


        /**
         * Setup terrain hierarchical bounding volume - aabbs per heightmap are actually unnecessary since its current intersection test does aabb check as well.
         */
        terrainBoundingVolume = std::unique_ptr<HierarchicalGeometry>(new HierarchicalGeometry(
        		new AABB(vector(0, 0, 0),
        				vector(terrain->getHeightMap()->getWidth(),
        						terrain->getHeightMap()->getHeight(), terrain->getHeightMap()->getDepth()))));

        vector terrainHalfSizes = vector(terrain->getHeightMap()->getWidth() * 0.5,
                terrain->getHeightMap()->getHeight() * 0.5,
                terrain->getHeightMap()->getDepth() * 0.5);

        terrainBoundingVolume->addChildren(new HeightMapGeometry(vector(0, 0, 0), terrain->getHeightMap()));
        terrainBoundingVolume->addChildren(new HeightMapGeometry(vector(-terrain->getHeightMap()->getWidth(), 0, 0), terrain->getHeightMap()));
        terrainBoundingVolume->addChildren(new HeightMapGeometry(vector(0, 0, -terrain->getHeightMap()->getDepth()), terrain->getHeightMap()));
        terrainBoundingVolume->addChildren(new HeightMapGeometry(vector(-terrain->getHeightMap()->getWidth(), 0, -terrain->getHeightMap()->getDepth()), terrain->getHeightMap()));

        skyboxRenderer.setVideoRunner(video);
        skyboxRenderer.setSize(500);

        defaultRenderer.setVideoRunner(video);

        physics->getParticleManager()->addForce(&this->gravity);
        physics->getParticleManager()->addScenery(terrainBoundingVolume.get());

        for(int index = 0; index < numberOfParticles; index++) {
            particles.push_back(std::unique_ptr<BulletParticle>(new BulletParticle()));
            particles.back()->setStatus(false);
            //particles.back()->setRunner(this);

            physics->getParticleManager()->addParticle(particles.back().get());
        }

        for(int index = 0; index < 20; index++) {
        	vector position(terrain->getHeightMap()->positionAt(rrand() * terrain->getHeightMap()->getWidth(), rrand() * terrain->getHeightMap()->getDepth()));
        	this->treePositions.push_back(matriz_4x4::traslacion(position) * matriz_4x4::rotacion(0, radian(rrand() * 360.0), 0));

        	vector halfsizes(tree->getSize() * 0.5);
        	this->treeBoundingVolumes.push_back(std::unique_ptr<AABB>(new AABB(position + vector(0, halfsizes.y, 0), vector(halfsizes.x * 0.15, halfsizes.y, halfsizes.z * 0.15))));
        	physics->getParticleManager()->addScenery(treeBoundingVolumes.back().get());
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


//    vector normalAt(real x, real z) const {
//        unsigned int i = std::max(0, std::min((int)floor(x / voxelSize.x), (int)this->getGridWidth()));
//        unsigned int j = std::max(0, std::min((int)floor(z / voxelSize.z), (int)this->getGridHeight()));
//
//        real di = (x - (real) i * voxelSize.x) / voxelSize.x;
//        real dj = (z - (real) j * voxelSize.z) / voxelSize.z;
//
//        if (di <= ((real) 1 - dj)) {
//			vector A = positionAtGrid(i, j);
//			vector B = positionAtGrid(i + 1, j);
//			vector C = positionAtGrid(i, j+1);
//
//			return ((A-B) ^ (A-C)).normalizado();
//        } else {
//            vector A = positionAtGrid(i + 1, j);
//			vector B = positionAtGrid(i + 1, j + 1);
//            vector C = positionAtGrid(i, j + 1);
//			return ((A-B) ^ (A-C)).normalizado();
//        }
//    }

	void renderContact(const ParticleContact &contact) {
	    //video->setOption(LINE_WIDTH, 10.0f); // not working
    	defaultRenderer.setMaterial(&black);
    	defaultRenderer.drawLine(matriz_4x4::traslacion(contact.getIntersection()), vector(0, 0, 0), contact.getNormal());

    	const HeightMapResource *heightmap = this->terrain->getHeightMap();
    	vector position = contact.getIntersection();
    	vector heightmapOrigin = vector(floor(position.x / heightmap->getWidth()), 0, floor(position.z / heightmap->getDepth()));
    	vector relativePosition = position - heightmapOrigin;
    	vector voxelSize = heightmap->getVoxelSize();

		unsigned int i = heightmap->getI(relativePosition.x);
		unsigned int j = heightmap->getJ(relativePosition.z);

		real di = (relativePosition.x - (real) i * voxelSize.x) / voxelSize.x;
		real dj = (relativePosition.z - (real) j * voxelSize.z) / voxelSize.z;

		if (di <= ((real) 1 - dj)) {
			vector A = heightmap->positionAtGrid(i, j);
			vector B = heightmap->positionAtGrid(i + 1, j);
			vector C = heightmap->positionAtGrid(i, j+1);
			vector normal = ((B-A) ^ (C-A)).normalizado();
			vector heightmapNormal = heightmap->normalAt(relativePosition.x, relativePosition.z);

			defaultRenderer.drawLine(matrix_4x4::traslacion(heightmapOrigin + A), vector(0, 0.01, 0), B-A + vector(0, 0.01, 0));
			defaultRenderer.drawLine(matrix_4x4::traslacion(heightmapOrigin + A), vector(0, 0.012, 0), B-A + vector(0, 0.012, 0));
			defaultRenderer.drawLine(matrix_4x4::traslacion(heightmapOrigin + A), vector(0, 0.013, 0), B-A + vector(0, 0.013, 0));

			defaultRenderer.drawLine(matrix_4x4::traslacion(heightmapOrigin + A), vector(0, 0.01, 0), C-A + vector(0, 0.01, 0));
			defaultRenderer.drawLine(matrix_4x4::traslacion(heightmapOrigin + A), vector(0, 0.012, 0), C-A + vector(0, 0.012, 0));
			defaultRenderer.drawLine(matrix_4x4::traslacion(heightmapOrigin + A), vector(0, 0.013, 0), C-A + vector(0, 0.013, 0));

			defaultRenderer.drawLine(matrix_4x4::traslacion(heightmapOrigin + A), vector(0, 0, 0), normal);
			defaultRenderer.drawLine(matrix_4x4::traslacion(position), vector(0, 0, 0), heightmapNormal);


		} else {
			vector A = heightmap->positionAtGrid(i + 1, j);
			vector B = heightmap->positionAtGrid(i + 1, j + 1);
			vector C = heightmap->positionAtGrid(i, j + 1);
			vector normal = ((B-A) ^ (C-A)).normalizado();
			vector heightmapNormal = heightmap->normalAt(relativePosition.x, relativePosition.z);

			defaultRenderer.drawLine(matrix_4x4::traslacion(heightmapOrigin + A), vector(0, 0.001, 0), B-A + vector(0, 0.001, 0));
			defaultRenderer.drawLine(matrix_4x4::traslacion(heightmapOrigin + A), vector(0, 0.0012, 0), B-A + vector(0, 0.002, 0));
			defaultRenderer.drawLine(matrix_4x4::traslacion(heightmapOrigin + A), vector(0, 0.0013, 0), B-A + vector(0, 0.003, 0));

			defaultRenderer.drawLine(matrix_4x4::traslacion(heightmapOrigin + A), vector(0, 0.001, 0), C-A + vector(0, 0.001, 0));
			defaultRenderer.drawLine(matrix_4x4::traslacion(heightmapOrigin + A), vector(0, 0.0012, 0), C-A + vector(0, 0.0012, 0));
			defaultRenderer.drawLine(matrix_4x4::traslacion(heightmapOrigin + A), vector(0, 0.0013, 0), C-A + vector(0, 0.0013, 0));

			defaultRenderer.drawLine(matrix_4x4::traslacion(heightmapOrigin + A), vector(0, 0, 0), normal);
			defaultRenderer.drawLine(matrix_4x4::traslacion(position), vector(0, 0, 0), heightmapNormal);
		}




	    //video->setOption(LINE_WIDTH, 1.0f); // not working

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
                defaultRenderer.drawSphere(matriz_4x4::traslacion(particle->getPosition()), 0.1);
            }
        }

        defaultRenderer.setTexture(treeTexture);
        for(auto &position : this->treePositions) {
        	defaultRenderer.drawObject(position, tree);
        }


        if(debug){
        	defaultRenderer.setTexture(null);
			/**
			 * Manually render contacts (for troubleshooting normal)
			 */
			for(auto &contact : physics->getParticleManager()->getContacts()) {
				renderContact(contact);
			}

			defaultRenderer.setMaterial(&white);
			for(auto &treeBV : this->treeBoundingVolumes) {
				defaultRenderer.drawBox(matriz::traslacion(treeBV->getOrigin()), treeBV->getHalfSizes() * 2);
			}



			/**
			 * manually render heightmap hierarchical bounding box (2 levels)
			 */
			for(auto &particle : this->particles) {
					if(particle->getStatus()) {
					Sphere &sphere = *((Sphere *)particle->getGeometry());

					for(int index = 0; index < terrainBoundingVolume->getChildren().size(); index++)
					{
						const HeightMapGeometry *child = (const HeightMapGeometry *)terrainBoundingVolume->getChildren()[index].get();
						if(physics->getParticleManager()->getCollisionDetector().getIntersectionTester()->sphereAabb(sphere, *child)) {
							vector halfSizes = child->getHalfSizes();
							defaultRenderer.setMaterial(materials[index % 3]);
							defaultRenderer.drawBox(matriz::traslacion(child->getOrigin()),
											(real)1.99 * halfSizes.x,
											(real)1.99 * halfSizes.y,
											(real)1.99 * halfSizes.z);
						}

					}

					defaultRenderer.setMaterial(materials[0]);
					if(physics->getParticleManager()->getCollisionDetector().getIntersectionTester()->sphereAabb(sphere, (AABB &)terrainBoundingVolume->getBoundingVolume())) {
						vector halfSizes = ((AABB &)terrainBoundingVolume->getBoundingVolume()).getHalfSizes();
						defaultRenderer.drawBox(matriz::traslacion(terrainBoundingVolume->getOrigin()),
										(real)2 * halfSizes.x,
										(real)2 * halfSizes.y,
										(real)2 * halfSizes.z);

					}

				}
			}
			defaultRenderer.setMaterial(null);

			/**
			 * render normals - this kills the cpu
			 */

	//		for(int index = 0; index < terrainBoundingVolume->getChildren().size(); index++)
	//		{
	//			const HeightMapGeometry *child = (const HeightMapGeometry *)terrainBoundingVolume->getChildren()[index].get();
	//			const HeightMapResource *heightMap = child->getHeightMap();
	//
	//			for(unsigned int i = 0; i < heightMap->getGridWidth(); i++) {
	//				for(unsigned int j = 0; j < child->getHeightMap()->getGridHeight(); j++) {
	//					defaultRenderer.drawLine(matriz_4x4::matrizTraslacion(child->getPosition()), heightMap->positionAtGrid(i, j), heightMap->positionAtGrid(i, j) + heightMap->normalAtGrid(i, j));
	//					//defaultRenderer.drawBox(matriz_4x4::matrizTraslacion(child->getPosition() + heightMap->positionAtGrid(i, j)), 0.5f, 0.5f, 0.5f);
	//				}
	//			}
	//		}
        }

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
        	case SDLK_F1:
        		this->debug = !this->debug;
        		break;
            case '1':
                this->inputController = &fpsInputController;
                break;
            case '2':
                this->inputController = &thirdPersonController;
                break;
            case SDLK_SPACE:
                reset();
                break;
            case SDLK_BACKSPACE:
            	if(physics->getEnabled()) {
            		physics->stop();
            	} else {
            		physics->start();
            	}
            	break;
            case SDLK_RIGHT:
            	physics->step(0.01f);
            	break;
            case SDLK_LEFT:
				physics->step(-0.01f);
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
