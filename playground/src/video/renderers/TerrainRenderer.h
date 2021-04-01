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
#include<VideoRunner.h>

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

		videoRunner->setTexture(0, "background", terrain->getA());
		videoRunner->setTexture(1, "textureR", terrain->getR());
		videoRunner->setTexture(2, "textureG", terrain->getG());
		videoRunner->setTexture(3, "textureB",terrain->getB());
		videoRunner->setTexture(4, "blendMap", terrain->getMap());

		videoRunner->setLight(light);

		videoRunner->sendMatrices();
		videoRunner->drawVertexArray(terrain->getModel());

        videoRunner->setTexture(0, "background", null);
        videoRunner->setTexture(1, "textureR", null);
        videoRunner->setTexture(2, "textureG", null);
        videoRunner->setTexture(3, "textureB", null);
        videoRunner->setTexture(4, "blendMap", null);

		//videoRunner->useProgramResource(null);
	}
};



#endif /* SRC_VIDEO_RENDERERS_TERRAINRENDERER_H_ */
