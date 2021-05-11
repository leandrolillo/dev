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
public:
    WorldObject(const matriz_4x4 &modelMatrix, const VertexArrayResource *object) {
        this->object = object;
        this->modelMatrix = modelMatrix;
        this->normalMatrix = ((matriz_3x3) modelMatrix).inversa().traspuesta();
    }

    const matriz_4x4 getModelMatrix() const {
        return this->modelMatrix;
    }

    const matriz_3x3 getNormalMatrix() const {
        return this->normalMatrix;
    }

    const VertexArrayResource *getVertexArray() const {
        return this->object;
    }
};

class DefaultRenderer : public Renderer {
private:
    Logger *logger = LoggerFactory::getLogger("DefaultRenderer");

    const TextureResource *texture = null;
    const MaterialResource *material = null;
    const LightResource *light = null;

    std::vector<const WorldObject> objects;

    const VertexArrayResource *axis = null;
    const VertexArrayResource *sphere = null;
    const VertexArrayResource *box = null;

public:
    void setMaterial(const MaterialResource *material) {
        this->material = material;
    }

    void setLight(const LightResource *light) {
        this->light = light;
    }

    void setTexture(const TextureResource *texture) {
        this->texture = texture;
    }

    bool init() {
        if(this->shader == null) {
            this->shader = (ShaderProgramResource*) this->resourceManager->load("core/simple.program.json", "video/shaderProgram");
        }

        this->axis = (VertexArrayResource*) this->resourceManager->load("core/axis.json", "video/vertexArray");
        this->box = (VertexArrayResource*) this->resourceManager->load("core/box.json", "video/vertexArray");
        this->sphere = (VertexArrayResource*) this->resourceManager->load("core/sphere.json", "video/vertexArray");

        return true;
    }

    void render(const Camera &camera) {
        if(videoRunner != null && shader != null) {
            videoRunner->useProgramResource(shader);

            if(this->texture != null) {
                videoRunner->setTexture(0, "textureUnit", this->texture);
            } else {
                videoRunner->setTexture(0, "textureUnit", videoRunner->getDefaultTexture());
            }

            videoRunner->sendVector("viewPosition", camera.getViewPosition());

            this->sendMaterial(material);
            this->sendLight(light);

            for (std::vector<const WorldObject>::const_iterator iterator = objects.begin(); iterator != objects.end(); iterator++) {
                videoRunner->sendMatrix("matrices.model", (*iterator).getModelMatrix());
                videoRunner->sendMatrix("matrices.pvm", camera.getProjectionViewMatrix() * (*iterator).getModelMatrix());
                videoRunner->sendMatrix("matrices.normal", (*iterator).getNormalMatrix());
                videoRunner->drawVertexArray((*iterator).getVertexArray());
            }

            videoRunner->setTexture(0, null);
            videoRunner->useProgramResource(null);
        } else {
            logger->error("Not rendering! VideoRunner or Shader not set.");
        }
    }

    void drawObject(const matriz_4x4 &modelMatrix, const VertexArrayResource *object) {
        this->objects.push_back(WorldObject(modelMatrix, object));
    }

    void drawAxis(const matriz_4x4 &modelMatrix, real length = 1.0f) {
        this->drawObject(modelMatrix * matriz_4x4::matrizZoom(length, length, length), axis);
    }

    void drawSphere(const matriz_4x4 &modelMatrix, real radius = 1.0f) {
        this->drawObject(modelMatrix * matriz_4x4::matrizZoom(radius, radius, radius), sphere);
    }

    void drawBox(const matriz_4x4 &modelMatrix, real height = 1.0f, real width = 1.0f, real depth = 1.0f) {
        this->drawObject(modelMatrix * matriz_4x4::matrizZoom(height, width, depth), box);
    }

    void clearObjects() {
        this->objects.clear();
    }


//    void drawPlane(vector posicion, vector normal, vector origen, float nro_grids, float ancho) const {
//  //          int pos_x, pos_z;
//  //          pos_x = (int) (((int) (posicion.x / ancho + 0.5f)) * ancho);
//  //          pos_z = (int) (((int) (posicion.z / ancho + 0.5f)) * ancho);
//
//          float pos_x = posicion.x;
//          float pos_z = posicion.z;
//
//          real ancho_plano = nro_grids * ancho;
//          real D = -(normal * origen);
//          real oneOverB = 1.0 / normal.y;
//
//          glBegin(GL_TRIANGLE_FAN);
//              real x = (real)(pos_x - ancho_plano);
//              real z = (real) (pos_z - ancho_plano);
//              real y = D - (normal.x * (x - ancho_plano) + normal.z * z) * oneOverB;
//
//              glTexCoord2f(0.0f, 0.0f);
//              glVertex3f(x, y, z);
//
//
//              x = (real)(pos_x - ancho_plano);
//              z = (real) (pos_z + ancho_plano);
//              y = D - (normal.x * (x - ancho_plano) + normal.z * z) * oneOverB;
//
//              glTexCoord2f(ancho_plano, 0.0f);
//              glVertex3f(x, y, z);
//
//              x = (real)(pos_x + ancho_plano);
//              z = (real) (pos_z + ancho_plano);
//              y = D - (normal.x * (x - ancho_plano) + normal.z * z) * oneOverB;
//
//              glTexCoord2f(ancho_plano, ancho_plano);
//              glVertex3f(x, y, z);
//
//              x = (real)(pos_x + ancho_plano);
//              z = (real) (pos_z - ancho_plano);
//              y = D - (normal.x * (x - ancho_plano) + normal.z * z) * oneOverB;
//
//              glTexCoord2f(0.0f, ancho_plano);
//              glVertex3f(x, y, z);
//          glEnd();
//    }
};

#endif /* SRC_VIDEO_RENDERERS_DEFAULTRENDERER_H_ */
