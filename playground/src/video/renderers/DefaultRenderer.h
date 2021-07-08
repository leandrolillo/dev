/*
 * DefaultRenderer.h
 *
 *  Created on: Apr 1, 2021
 *      Author: leandro
 */

#ifndef SRC_VIDEO_RENDERERS_DEFAULTRENDERER_H_
#define SRC_VIDEO_RENDERERS_DEFAULTRENDERER_H_

#include<Renderer.h>
#include<resources/MaterialResource.h>
#include<resources/LightResource.h>
#include<VideoRunner.h>

#include<vector>

class WorldObject {
protected:
    matriz_4x4 modelMatrix;
    matriz_3x3 normalMatrix;
    const VertexArrayResource *object;
    const MaterialResource *material;
public:
    WorldObject(const matriz_4x4 &modelMatrix, const VertexArrayResource *object, const MaterialResource *material) {
        this->object = object;
        this->modelMatrix = modelMatrix;

        /**
         * Do not calculate inverse if determinant is zero - alternative to throwing the exception and stopping, specially for
         * drawLine functionality which generates singular matrixes.
         */
        matriz_3x3 reducedModelMatrix = (matriz_3x3) modelMatrix;
        if(reducedModelMatrix.determinante() != 0.0f) {
            this->normalMatrix = reducedModelMatrix.inversa().traspuesta();
        } else {
            this->normalMatrix = matriz_3x3::identidad;
        }
        this->material = material;
    }

    const matriz_4x4 &getModelMatrix() const {
        return this->modelMatrix;
    }

    const matriz_3x3 &getNormalMatrix() const {
        return this->normalMatrix;
    }

    const VertexArrayResource *getVertexArray() const {
        return this->object;
    }

    const MaterialResource *getMaterial() const {
        return this->material;
    }
};

class DefaultRenderer : public Renderer {
private:
    Logger *logger = LoggerFactory::getLogger("DefaultRenderer");

    const TextureResource *currentTexture = null;
    const MaterialResource *currentMaterial = null;
    const LightResource *light = null;

    std::map<const TextureResource *, std::vector<const WorldObject>>objectsByTexture;

    const VertexArrayResource *axes = null;
    const VertexArrayResource *sphere = null;
    const VertexArrayResource *box = null;
    const VertexArrayResource *line = null;

    const MaterialResource defaultMaterial = MaterialResource(vector(0.3, 0.3, 0.3), vector(0.3, 0.3, 0.3), vector(0.3, 0.3, 0.3), 32);

public:
    void setMaterial(const MaterialResource *material) {
        this->currentMaterial = (material == null ? &defaultMaterial : material);
    }

    void setLight(const LightResource *light) {
        this->light = light;
    }

    void setTexture(const TextureResource *texture) {
        this->currentTexture = texture;
    }

    bool init() {
        if(this->shader == null) {
            this->shader = (ShaderProgramResource*) this->resourceManager->load("core/simple.program.json", "video/shaderProgram");
        }

        return true;
    }

    void render(const Camera &camera) {
        if(videoRunner != null && shader != null) {
            videoRunner->useProgramResource(shader);

            videoRunner->sendVector("viewPosition", camera.getViewPosition());
            this->sendLight(light);

            const MaterialResource *lastMaterial = null;

            for(std::map<const TextureResource *, std::vector<const WorldObject>>::iterator textureIterator = this->objectsByTexture.begin();
                    textureIterator != this->objectsByTexture.end(); textureIterator++)
            {
                if((*textureIterator).first != null) {
                    videoRunner->setTexture(0, "textureUnit", (*textureIterator).first);
                } else {
                    videoRunner->setTexture(0, "textureUnit", videoRunner->getDefaultTexture());
                }

                for (std::vector<const WorldObject>::const_iterator iterator = (*textureIterator).second.begin();
                        iterator != (*textureIterator).second.end(); iterator++) {
                    const WorldObject &object = *iterator;

                    if(lastMaterial != object.getMaterial()) {
                        lastMaterial = object.getMaterial();
                        if(lastMaterial != null) {
                            this->sendMaterial(lastMaterial);
                        }
                    }
                    videoRunner->sendMatrix("matrices.model", object.getModelMatrix());
                    videoRunner->sendMatrix("matrices.pvm", camera.getProjectionViewMatrix() * object.getModelMatrix());
                    videoRunner->sendMatrix("matrices.normal", object.getNormalMatrix());
                    videoRunner->drawVertexArray(object.getVertexArray());
                }
            }

            videoRunner->setTexture(0, null);
            videoRunner->useProgramResource(null);
        } else {
            logger->error("Not rendering! VideoRunner or Shader not set.");
        }
    }

    void drawObject(const matriz_4x4 &modelMatrix, const VertexArrayResource *object) {
        this->objectsByTexture[this->currentTexture].push_back(WorldObject(modelMatrix, object, currentMaterial));
    }

    void drawAxes(const matriz_4x4 &modelMatrix, real length = 1.0f) {
        this->drawObject(modelMatrix * matriz_4x4::matrizZoom(length, length, length), getAxes());
    }

    void drawSphere(const matriz_4x4 &modelMatrix, real radius = 1.0f) {
        this->drawObject(modelMatrix * matriz_4x4::matrizZoom(radius, radius, radius), getSphere());
    }

    void drawBox(const matriz_4x4 &modelMatrix, real height = 1.0f, real width = 1.0f, real depth = 1.0f) {
        this->drawObject(modelMatrix * matriz_4x4::matrizZoom(height, width, depth), getBox());
    }
    void drawLine(const matriz_4x4 &modelMatrix, const vector &start, const vector &end) {
        matriz_4x4 traslacion = matriz_4x4::matrizTraslacion(start);
        matriz_4x4 zoom = matriz_4x4::matrizZoom(end - start);
        matriz_4x4 lineMatrix = modelMatrix * traslacion * zoom;
        this->drawObject(lineMatrix, getLine());
    }

    void clear() {
        this->setTexture(videoRunner->getDefaultTexture());
        this->setMaterial(&defaultMaterial);
        this->objectsByTexture.clear();
    }
private:
    const VertexArrayResource *getSphere() {
        if(this->sphere == null) {
            this->sphere = (VertexArrayResource*) this->resourceManager->load("core/sphere.json", "video/vertexArray");
        }

        return this->sphere;
    }

    const VertexArrayResource *getLine() {
        if(this->line == null) {
            this->line= (VertexArrayResource*) this->resourceManager->load("core/line.json", "video/vertexArray");
        }

        return this->line;
    }

    const VertexArrayResource *getAxes() {
        if(this->axes == null) {
            this->axes= (VertexArrayResource*) this->resourceManager->load("core/axis.json", "video/vertexArray");
        }

        return this->axes;
    }

    const VertexArrayResource *getBox() {
            if(this->box == null) {
                this->box= (VertexArrayResource*) this->resourceManager->load("core/box.json", "video/vertexArray");
            }

            return this->box;
        }
};


class Drawable {
public:
    virtual ~Drawable() {

    }
    virtual void draw(DefaultRenderer &renderer) const = 0;
};


#endif /* SRC_VIDEO_RENDERERS_DEFAULTRENDERER_H_ */
