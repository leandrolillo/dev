/*
 * HeightMapResourceAdapter.h
 *
 *  Created on: Apr 2, 2021
 *      Author: leandro
 */

#ifndef SRC_OPENGL_ADAPTERS_HEIGHTMAPRESOURCEADAPTER_H_
#define SRC_OPENGL_ADAPTERS_HEIGHTMAPRESOURCEADAPTER_H_

#include<adapters/VertexArrayAdapter.h>
#include<resources/ImageResource.h>
#include<Math3d.h>

#define max_height 20.0
#define one_over_max_color_over_two 1.0 / (256.0 * 256.0 * 256.0 * 0.5)

class HeightMapCalculator {
private:
    Logger *logger = Logger::getLogger("video/HeightMapCalculator");
    real boxSize;
    unsigned int maxPoints;
    ImageResource *heightMap;
public:
    HeightMapCalculator(ImageResource *heightMap, real boxSize, unsigned int maxPoints) {
        this->boxSize = boxSize;
        this->maxPoints = maxPoints;
        this->heightMap = heightMap;
    }

    vector position(unsigned int i, unsigned int j) {
        return vector(  (real)i * boxSize,
                        height(i, j),
                        (real)j * boxSize
                     );
    }

    vector normal(unsigned int i, unsigned int j) {

        //return vector(0, 1, 0);
        real hL = height(i-1 , j);
        real hR = height(i+1 , j);
        real hD = height(i, j+1);
        real hU = height(i, j-1);

        return vector(hL - hR, 2.0, hD - hU).normalizado();
    }

    vector2 textCoord(unsigned int i, unsigned int j) {
        return vector2((real)i / (real)(maxPoints - 1), (real)j / (real)(maxPoints - 1));
    }

    real height(unsigned int i, unsigned int j) {
        vector pixel = this->heightMap->getPixel(i, j);

//        logger->info("<%u, %u) = <%.0f, %.0f, %.0f>", i, j, pixel.x, pixel.y, pixel.z);


        return (pixel.x * pixel.y * pixel.z * one_over_max_color_over_two - 1.0) * max_height;
    }
};

class HeightMapResourceAdapter : public VertexArrayResourceAdapter {
private:
    real boxSize = 1.0;
public:
    HeightMapResourceAdapter() : VertexArrayResourceAdapter() {
        logger = Logger::getLogger("video/HeightMapResourceAdapter");
        this->clearSupportedMimeTypes();
        this->addSupportedMimeType("video/heightmap");
    }

    virtual Resource* load(FileParser &fileParser, const String &mimeType) {
        ImageResource *image = (ImageResource *)this->getResourceManager()->load(fileParser);
        if(image != null) {
            unsigned int maxPoints = image->getAlto();

            GeometryResource heightMapGeometry(0);
            heightMapGeometry.setType("triangles");

            HeightMapCalculator calculator(image, boxSize, maxPoints);
            for(unsigned int i = 0; i < maxPoints; i++) {
                for(unsigned int j = 0; j < maxPoints; j++) {
                    heightMapGeometry.getVertices().push_back(calculator.position(i, j));
                    heightMapGeometry.getNormals().push_back(calculator.normal(i, j));
                    heightMapGeometry.getTextureCoordinates().push_back(calculator.textCoord(i, j));
//                    heightMapGeometry.getColors().push_back(vector3(
//                            0.0,
//                            (real)i / (maxPoints - 1),
//                            (real)j / (maxPoints -1 ))
//                            );
                    //logger->info("<%d, %d>--->%s", i, j, calculator.normal(i, j).toString().c_str());


                }
            }

            for(unsigned int i = 1; i < maxPoints; i++) {
                for(unsigned int j = 1; j < maxPoints; j++) {
                    heightMapGeometry.getIndices().push_back((i - 1) * maxPoints + (j - 1));
                    heightMapGeometry.getIndices().push_back(i * maxPoints + (j - 1));
                    heightMapGeometry.getIndices().push_back(i * maxPoints + j);

                    heightMapGeometry.getIndices().push_back((i - 1) * maxPoints + (j - 1));
                    heightMapGeometry.getIndices().push_back(i * maxPoints + j);
                    heightMapGeometry.getIndices().push_back((i - 1) * maxPoints + j);

                }
            }

            return this->generateVertexBuffer(&heightMapGeometry);
        }

        return null;
    }
private:



};



#endif /* SRC_OPENGL_ADAPTERS_HEIGHTMAPRESOURCEADAPTER_H_ */
