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

class SkyboxRenderer: public Renderer {
private:
    Logger *logger = LoggerFactory::getLogger("SkyboxRenderer");

    const CubeMapResource *cubeMap;
    const VertexArrayResource *box;
    real size = 300;
public:
    void setCubeMap(const CubeMapResource *cubeMap) {
        this->cubeMap = cubeMap;
    }

    void setBox(const VertexArrayResource *box) {
        this->box = box;
    }

    void setSize(real size) {
        this->size = size;
    }

    bool init() {
        if (this->shader == null) {
            this->shader = (ShaderProgramResource*) this->resourceManager->load("shaders/skybox/skybox.program.json",
                    "video/shaderProgram");
        }

        if (this->cubeMap == null) {
            this->cubeMap = (CubeMapResource*) this->resourceManager->load("geometry/skybox/skybox.json", "video/cubemap");
        }

        if (this->box == null) {
            this->box = (VertexArrayResource*) this->resourceManager->load("geometry/skybox/skybox_geometry.json", "video/vertexArray");
        }

        return true;
    }

    void render(const Camera &camera) {
        if (videoRunner != null && this->shader != null && this->box != null && this->cubeMap != null) {
            videoRunner->useProgramResource(shader);
            videoRunner->setTexture(0, "textureUnit", cubeMap, GL_TEXTURE_CUBE_MAP);
            videoRunner->sendMatrix("matrices.p", camera.getProjectionMatrix());
            videoRunner->sendMatrix("matrices.v", camera.getViewMatrix());
            videoRunner->sendReal("boxSize", this->size);

            videoRunner->drawVertexArray(box);

            videoRunner->useProgramResource(null);
            videoRunner->setTexture(0, cubeMap, GL_TEXTURE_CUBE_MAP);
        } else {
            logger->error("not rendering!: videoRunner, shader, box or cubeMap are not set");
        }
    }
};

#endif /* SRC_VIDEO_RENDERERS_SKYBOXRENDERER_H_ */
