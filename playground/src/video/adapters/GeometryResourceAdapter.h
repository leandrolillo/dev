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
			logger = Logger::getLogger("video/VertexBufferResourceAdapter.h");
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
				if(token == "vertex") {
					parser->readValueSeparator();
					parser->readVectorArray();
				} else if (token == "textureCoordinates") {
					parser->readValueSeparator();
					parser->readVectorArray();
				}
				else if (token == "normals") {
					parser->readValueSeparator();
					parser->readVectorArray();
				}
			}
			return resource;
		}
		virtual void dispose(Resource *resource) {
			GeometryResource *geometryResource = (GeometryResource *)resource;
		}
};

#endif /* GEOMETRYRESOURCEADAPTER_H_ */
