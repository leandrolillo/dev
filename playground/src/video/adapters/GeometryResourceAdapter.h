/*
 * GeometryResourceAdapter.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#ifndef GEOMETRYRESOURCEADAPTER_H_
#define GEOMETRYRESOURCEADAPTER_H_
#include "resources/ResourceAdapter.h"
#include "../resources/GeometryResource.h"
#include "parser/JsonParser.h"
#include <gl/gl.h>
#include <gl/glext.h>

class GeometryResourceAdapter: public ResourceAdapter {
	private:
		std::vector<String> supportedMimeTypes;
		Logger *logger;
	public:
		GeometryResourceAdapter() {
			supportedMimeTypes.push_back("video/geometry");
			logger = Logger::getLogger("video/GeometryResourceAdapter.h");
		}
		virtual const std::vector<String> getSupportedMimeTypes() {
			return supportedMimeTypes;
		}
		virtual Resource *load(FileParser &fileParser) {
			JsonParser *parser = new JsonParser(fileParser);

			GeometryResource *resource = new GeometryResource(0);

			String token;
			parser->readStartObject();
			while((token = parser->readToken()) != END_OBJECT && token != eof)
			{
				if(token == "vertices") {
					parser->readValueSeparator();
					resource->setVertices(parser->readVector2Array());
					log("vertices = ", resource->getVertices());
				} else if (token == "textureCoordinates") {
					parser->readValueSeparator();
					resource->setTextureCoordinates(parser->readVector2Array());
				} else if (token == "normals") {
					parser->readValueSeparator();
					resource->setNormals(parser->readVector2Array());
				} else if (token == "texture") {
					parser->readValueSeparator();
					String textureFile = parser->readString();
					logger->debug("texture [%s]", textureFile.c_str());
					resource->setTextureFile(textureFile);
				} else if (token == "colors") {
					parser->readValueSeparator();
					resource->setColors(parser->readVector3Array());
				}
			}
			return resource;
		}
		virtual void dispose(Resource *resource) {
			GeometryResource *geometryResource = (GeometryResource *)resource;
		}

		void log(String prefix, std::vector<vector2> array)
		{
			char vectorBuffer[256];

			prefix.append(" [");

			for(std::vector<vector2>::iterator current = array.begin(); current != array.end(); current++) {
				sprintf(vectorBuffer, "<%.2f, %.2f> ", (*current).x, (*current).y);
				prefix.append(vectorBuffer);
			}

			prefix.append(" ]");
			logger->debug(prefix.c_str());

		}
};

#endif /* GEOMETRYRESOURCEADAPTER_H_ */
