/*
 * Renderer.h
 *
 *  Created on: Apr 1, 2021
 *      Author: leandro
 */

#ifndef SRC_VIDEO_RENDERER_H_
#define SRC_VIDEO_RENDERER_H_

#include<resources/ShaderProgramResource.h>
#include<VideoRunner.h>
#include<Camera.h>

class Renderer {
protected:
    const ShaderProgramResource *shader = null;
    VideoRunner *videoRunner = null;
    ResourceManager *resourceManager = null;
public:

//    Renderer(VideoRunner *videoRunner) {
//        this->videoRunner = videoRunner;
//    }

    virtual ~Renderer() {}

    void setVideoRunner(VideoRunner *videoRunner) {
        this->videoRunner = videoRunner;
        this->resourceManager = videoRunner->getContainer()->getResourceManager();
        this->init();
    }

    void setShaderProgram(const ShaderProgramResource *shaderProgramResource) {
        this->shader = shaderProgramResource;
    }

    void sendMaterial(const MaterialResource *material) const {
        if(material != null) {
            videoRunner->sendVector("material.ambient", material->getAmbient());
            videoRunner->sendVector("material.diffuse", material->getDiffuse());
            videoRunner->sendVector("material.specular", material->getSpecular());
            videoRunner->sendReal("material.shininess", material->getShininess());
        }
    }

    void sendLight(const LightResource *light) const {
        if(light) {
            videoRunner->sendVector("light.ambient", light->getAmbient() * light->getShininess());
            videoRunner->sendVector("light.diffuse", light->getDiffuse() * light->getShininess());
            videoRunner->sendVector("light.specular", light->getSpecular() * light->getShininess());
            videoRunner->sendVector("light.position", light->getPosition());
        }
    }

    virtual bool init() {
        return true;
    };

    virtual void render(const Camera &camera) = 0;

};



#endif /* SRC_VIDEO_RENDERER_H_ */
