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
	private:
		void buildVerticesArray(GeometryResource *resource, std::vector<vector2> &vertices)
		{
			if(resource != null && vertices.size() > 0) {
				for(std::vector<vector2>::iterator source = vertices.begin(); source != vertices.end(); source ++)
				{
					unsigned int index = 0;
					boolean preExisting = false;
					for(std::vector<vector2>::iterator destination = resource->getVertices().begin(); destination != resource->getVertices().end(); destination++)
					{
						if(*source == *destination) {
							resource->getIndexes().push_back(index);
							preExisting = true;
							break;
						}
						index++;
					}
					if(!preExisting) {
						resource->getVertices().push_back(*source);
						resource->getIndexes().push_back(resource->getIndexes().size());
					}
				}
			}
		}
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
					std::vector<vector2> vertices = parser->readVector2Array();
					buildVerticesArray(resource, vertices);
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
