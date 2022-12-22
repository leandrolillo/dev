/*
 * TerrainResourceAdapter.h
 *
 *  Created on: Mar 26, 2021
 *      Author: leandro
 */

#ifndef SRC_VIDEO_ADAPTERS_TERRAINRESOURCEADAPTER_H_
#define SRC_VIDEO_ADAPTERS_TERRAINRESOURCEADAPTER_H_

#include <resources/TerrainResource.h>
#include <ResourceAdapter.h>
#include <JsonParser.h>
#include <OpenGLUtilities.h>

class TerrainResourceAdapter: public ResourceAdapter {
public:
	TerrainResourceAdapter() {
		logger = LoggerFactory::getLogger("video/TerrainResourceAdapter");
		this->produces(MimeTypes::TERRAIN);
	}

	Resource *load(ResourceLoadRequest &request) const override {
		JsonParser jsonParser(request.getFileParser());
		TerrainResource *resource = new TerrainResource();

		jsonParser.readStartObject();

		String token;
		while ((token = jsonParser.readToken()) != END_OBJECT && token != FileParser::eof) {
            jsonParser.readValueSeparator();

			if (token == "r") {
				resource->setR((TextureResource *)this->getResourceManager()->load(request.getFilePath(), jsonParser.readString(), MimeTypes::TEXTURE));
			} else if (token == "g") {
				resource->setG((TextureResource *)this->getResourceManager()->load(request.getFilePath(), jsonParser.readString(), MimeTypes::TEXTURE));
			} else if (token == "b") {
				resource->setB((TextureResource *)this->getResourceManager()->load(request.getFilePath(), jsonParser.readString(), MimeTypes::TEXTURE));
			} else if (token == "a") {
				resource->setA((TextureResource *)this->getResourceManager()->load(request.getFilePath(), jsonParser.readString(), MimeTypes::TEXTURE));
			} else if (token == "map") {
				resource->setMap((TextureResource *)this->getResourceManager()->load(request.getFilePath(), jsonParser.readString(), MimeTypes::TEXTURE));
			} else if (token == "model") {
				resource->setModel((VertexArrayResource *)this->getResourceManager()->load(request.getFilePath(), jsonParser.readString(), MimeTypes::VERTEXARRAY));
			} else if (token == "heightmap") {
			    resource->setHeightmap((HeightMapResource *)this->getResourceManager()->load(request.getFilePath(), jsonParser.readString(), MimeTypes::HEIGHTMAP));
			    resource->setModel(buildModel(resource->getHeightMap()));
			} else {
				logger->error("Unexpected token: [%s] at (%d, %d)",
						token.c_str(), jsonParser.getLine(), jsonParser.getColumn());
			}

			if (jsonParser.peekToken() == ",") {
				jsonParser.readToken();
			}
		}

		return resource;
	}

protected:
	VertexArrayResource *buildModel(const HeightMapResource *heightMap) const {
	    VertexArrayResource *model = null;
        if(heightMap != null) {
            GeometryResource heightMapGeometry(0);
            heightMapGeometry.setName(Paths::getBasename(heightMap->getFileName()));
            heightMapGeometry.setType("triangles");

            for(unsigned int i = 0; i < heightMap->getGridWidth(); i++) {
                for(unsigned int j = 0; j < heightMap->getGridHeight(); j++) {
                    heightMapGeometry.getVertices().push_back(heightMap->positionAtGrid(i, j));
                    heightMapGeometry.getNormals().push_back(heightMap->normalAtGrid(i, j));
                    heightMapGeometry.getTextureCoordinates().push_back(heightMap->textCoordAtGrid(i, j));
                }
            }

            for(unsigned int i = 1; i < heightMap->getGridWidth(); i++) {
                for(unsigned int j = 1; j < heightMap->getGridHeight(); j++) {
                    heightMapGeometry.getIndices().push_back(i * heightMap->getGridWidth() + j);
                    heightMapGeometry.getIndices().push_back(i * heightMap->getGridWidth() + (j - 1));
                    heightMapGeometry.getIndices().push_back((i - 1) * heightMap->getGridWidth() + (j - 1));

                    heightMapGeometry.getIndices().push_back((i - 1) * heightMap->getGridWidth() + j);
                    heightMapGeometry.getIndices().push_back(i * heightMap->getGridWidth() + j);
                    heightMapGeometry.getIndices().push_back((i - 1) * heightMap->getGridWidth() + (j - 1));
                }
            }

			model = OpenGLUtilites::generateVertexBuffer(&heightMapGeometry);
			model->setFileName(heightMap->getFileName());
			this->getResourceManager()->addResource(model);
        }

        return model;
    }


};

#endif /* SRC_VIDEO_ADAPTERS_TERRAINRESOURCEADAPTER_H_ */
