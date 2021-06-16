/*
 * TerrainResourceAdapter.h
 *
 *  Created on: Mar 26, 2021
 *      Author: leandro
 */

#ifndef SRC_VIDEO_ADAPTERS_TERRAINRESOURCEADAPTER_H_
#define SRC_VIDEO_ADAPTERS_TERRAINRESOURCEADAPTER_H_

#include <ResourceAdapter.h>
#include"VertexArrayAdapter.h"
#include <resources/TerrainResource.h>
#include <JsonParser.h>

class TerrainResourceAdapter: public ResourceAdapter {
public:
	TerrainResourceAdapter() {
		logger = LoggerFactory::getLogger("video/TerrainResourceAdapter");
		this->addSupportedMimeType("video/terrain");
	}

	Resource* load(FileParser &fileParser, const String &mimeType) {
		JsonParser *jsonParser = new JsonParser(fileParser);
		TerrainResource *resource = new TerrainResource();

		jsonParser->readStartObject();

		String token;
		while ((token = jsonParser->readToken()) != END_OBJECT && token != FileParser::eof) {
            jsonParser->readValueSeparator();

			if (token == "r") {
				resource->setR((TextureResource *)this->getResourceManager()->load(fileParser.getFilename(), jsonParser->readString(), "video/texture"));
			} else if (token == "g") {
				resource->setG((TextureResource *)this->getResourceManager()->load(fileParser.getFilename(), jsonParser->readString(), "video/texture"));
			} else if (token == "b") {
				resource->setB((TextureResource *)this->getResourceManager()->load(fileParser.getFilename(), jsonParser->readString(), "video/texture"));
			} else if (token == "a") {
				resource->setA((TextureResource *)this->getResourceManager()->load(fileParser.getFilename(), jsonParser->readString(), "video/texture"));
			} else if (token == "map") {
				resource->setMap((TextureResource *)this->getResourceManager()->load(fileParser.getFilename(), jsonParser->readString(), "video/texture"));
			} else if (token == "model") {
				resource->setModel((VertexArrayResource *)this->getResourceManager()->load(fileParser.getFilename(), jsonParser->readString(), "video/vertexArray"));
			} else if (token == "heightmap") {
			    resource->setHeightmap((HeightMapResource *)this->getResourceManager()->load(fileParser.getFilename(), jsonParser->readString(), "video/heightmap"));
			    resource->setModel(buildModel(resource->getHeightMap()));
			} else {
				logger->error("Unexpected token: [%s] at (%d, %d)",
						token.c_str(), jsonParser->getLine(), jsonParser->getColumn());
			}

			if (jsonParser->peekToken() == ",") {
				jsonParser->readToken();
			}
		}

		return resource;
	}

protected:
	VertexArrayResource *buildModel(const HeightMapResource *heightMap) {
	    VertexArrayResource *model = null;
        if(heightMap != null) {
            GeometryResource heightMapGeometry(0);
            heightMapGeometry.setType("triangles");

            for(unsigned int i = 0; i < heightMap->getWidthInVoxels(); i++) {
                for(unsigned int j = 0; j < heightMap->getHeightInVoxels(); j++) {
                    heightMapGeometry.getVertices().push_back(heightMap->position(i, j));
                    heightMapGeometry.getNormals().push_back(heightMap->normal(i, j));
                    heightMapGeometry.getTextureCoordinates().push_back(heightMap->textCoord(i, j));
                }
            }

            for(unsigned int i = 1; i < heightMap->getWidthInVoxels(); i++) {
                for(unsigned int j = 1; j < heightMap->getHeightInVoxels(); j++) {
                    heightMapGeometry.getIndices().push_back(i * heightMap->getWidthInVoxels() + j);
                    heightMapGeometry.getIndices().push_back(i * heightMap->getWidthInVoxels() + (j - 1));
                    heightMapGeometry.getIndices().push_back((i - 1) * heightMap->getWidthInVoxels() + (j - 1));

                    heightMapGeometry.getIndices().push_back((i - 1) * heightMap->getWidthInVoxels() + j);
                    heightMapGeometry.getIndices().push_back(i * heightMap->getWidthInVoxels() + j);
                    heightMapGeometry.getIndices().push_back((i - 1) * heightMap->getWidthInVoxels() + (j - 1));
                }
            }

            VertexArrayResourceAdapter *adapter = (VertexArrayResourceAdapter *)this->getResourceManager()->getAdapter("video/vertexArray");
            if(adapter != null) {
                model = adapter->generateVertexBuffer(&heightMapGeometry);
                model->setFileName(heightMap->getFileName());
                this->getResourceManager()->addResource(model);
            }
        }

        return model;
    }


};

#endif /* SRC_VIDEO_ADAPTERS_TERRAINRESOURCEADAPTER_H_ */
