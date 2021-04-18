/*
 * TerrainRenderer.h
 *
 *  Created on: Mar 30, 2021
 *      Author: leandro
 */

#ifndef SRC_VIDEO_RENDERERS_TERRAINRENDERER_H_
#define SRC_VIDEO_RENDERERS_TERRAINRENDERER_H_

#include<ShaderProgramResource.h>
#include<resources/TerrainResource.h>
#include<resources/LightResource.h>
#include<VideoRunner.h>

class TerrainRenderer : public Renderer {
private:
    Logger *logger = Logger::getLogger("TerrainRenderer");

	const TerrainResource *terrain = null;
	const LightResource *light = null;

public:
	void setTerrain(const TerrainResource *terrain) {
		this->terrain = terrain;
	}

	void setLight(const LightResource *light) {
	    this->light = light;
	}

    bool init() {
        if(this->shader == null) {
            this->shader = (ShaderProgramResource *)resourceManager->load("shaders/terrain/terrain.program.json", "video/shaderProgram");
        }

        return true;
    }

	void render(const Camera &camera) {
	    if(videoRunner != null && shader != null && terrain != null) {
            videoRunner->useProgramResource(shader);

            videoRunner->setTexture(0, "background", terrain->getA());
            videoRunner->setTexture(1, "textureR", terrain->getR());
            videoRunner->setTexture(2, "textureG", terrain->getG());
            videoRunner->setTexture(3, "textureB",terrain->getB());
            videoRunner->setTexture(4, "blendMap", terrain->getMap());

            videoRunner->sendMatrix("matrices.model", matriz_4x4::identidad);
            videoRunner->sendMatrix("matrices.pvm", camera.getProjectionViewMatrix() * matriz_4x4::identidad);
            videoRunner->sendMatrix("matrices.normal", matriz_3x3::identidad);

            this->sendLight(light);

            videoRunner->drawVertexArray(terrain->getModel());

            videoRunner->setTexture(0, "background", null);
            videoRunner->setTexture(1, "textureR", null);
            videoRunner->setTexture(2, "textureG", null);
            videoRunner->setTexture(3, "textureB", null);
            videoRunner->setTexture(4, "blendMap", null);

            videoRunner->useProgramResource(null);
	    } else {
	        logger->error("Not rendering! shader or terrain not set.");

	    }
	}
};



#endif /* SRC_VIDEO_RENDERERS_TERRAINRENDERER_H_ */
