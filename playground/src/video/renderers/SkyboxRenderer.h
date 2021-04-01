/*
 * SkyboxRenderer.h
 *
 *  Created on: Mar 31, 2021
 *      Author: leandro
 */

#ifndef SRC_VIDEO_RENDERERS_SKYBOXRENDERER_H_
#define SRC_VIDEO_RENDERERS_SKYBOXRENDERER_H_

#include<resources/ShaderProgramResource.h>
#include<resources/CubeMapResource.h>

#include<VideoRunner.h>

class SkyboxRenderer {
private:
	const ShaderProgramResource *shader = null;
	const CubeMapResource *cubeMap;
	const VertexArrayResource *box;
	real size = 300;
public:

	void setShaderProgram(const ShaderProgramResource *shaderProgramResource) {
			this->shader = shaderProgramResource;
	}

	void setCubeMap(const CubeMapResource *cubeMap) {
		this->cubeMap = cubeMap;
	}

	void setBox(const VertexArrayResource *box) {
	    this->box = box;
	}

	void setSize(real size) {
	    this->size = size;
	}

	void render(VideoRunner *videoRunner) {
//	    videoRunner->useProgramResource(((OpenGLRunner *)videoRunner)->getDefaultShaderProgram());
//	    videoRunner->sendMatrices();

	    videoRunner->useProgramResource(shader);
		videoRunner->setTexture(0, "textureUnit", cubeMap, GL_TEXTURE_CUBE_MAP);
		videoRunner->sendMatrix("matrices.p", videoRunner->getProjectionMatrix());
		videoRunner->sendMatrix("matrices.v", videoRunner->getViewMatrix());
		videoRunner->sendReal("boxSize", this->size);

		videoRunner->drawVertexArray(box);

		//videoRunner->useProgramResource(null);
	}
};



#endif /* SRC_VIDEO_RENDERERS_SKYBOXRENDERER_H_ */
