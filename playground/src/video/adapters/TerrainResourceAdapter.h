/*
 * TerrainResourceAdapter.h
 *
 *  Created on: Mar 26, 2021
 *      Author: leandro
 */

#ifndef SRC_VIDEO_ADAPTERS_TERRAINRESOURCEADAPTER_H_
#define SRC_VIDEO_ADAPTERS_TERRAINRESOURCEADAPTER_H_

#include <ResourceAdapter.h>
#include <resources/TerrainResource.h>
#include <JsonParser.h>

class TerrainResourceAdapter: public ResourceAdapter {
public:
	TerrainResourceAdapter() {
		logger = Logger::getLogger("video/TerrainResourceAdapter");
		this->addSupportedMimeType("video/terrain");
	}

	Resource* load(FileParser &fileParser, const String &mimeType) {
		JsonParser *jsonParser = new JsonParser(fileParser);
		TerrainResource *resource = new TerrainResource();

		jsonParser->readStartObject();

		String token;
		while ((token = jsonParser->readToken()) != END_OBJECT && token != eof) {
			if (token == "r") {
				jsonParser->readValueSeparator();
				resource->setR((TextureResource *)this->getResourceManager()->load(fileParser.getFilename(), jsonParser->readString(), "video/texture"));
			} else if (token == "g") {
				jsonParser->readValueSeparator();
				resource->setG((TextureResource *)this->getResourceManager()->load(fileParser.getFilename(), jsonParser->readString(), "video/texture"));
			} else if (token == "b") {
				jsonParser->readValueSeparator();
				resource->setB((TextureResource *)this->getResourceManager()->load(fileParser.getFilename(), jsonParser->readString(), "video/texture"));
			} else if (token == "a") {
				jsonParser->readValueSeparator();
				resource->setA((TextureResource *)this->getResourceManager()->load(fileParser.getFilename(), jsonParser->readString(), "video/texture"));
			} else if (token == "map") {
				jsonParser->readValueSeparator();
				resource->setMap((TextureResource *)this->getResourceManager()->load(fileParser.getFilename(), jsonParser->readString(), "video/texture"));
			} else if (token == "model") {
				jsonParser->readValueSeparator();
				resource->setModel((VertexArrayResource *)this->getResourceManager()->load(fileParser.getFilename(), jsonParser->readString(), "video/vertexArray"));
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

};

#endif /* SRC_VIDEO_ADAPTERS_TERRAINRESOURCEADAPTER_H_ */
