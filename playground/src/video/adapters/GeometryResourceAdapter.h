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
#include "normalGenerators/NormalGenerator.h"
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>

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
		virtual Resource *load(FileParser &fileParser, const String &mimeType) {
			JsonParser *parser = new JsonParser(fileParser);

			GeometryResource *resource = new GeometryResource(0);

			String token;
			parser->readStartObject();
			while((token = parser->readToken()) != END_OBJECT && token != EOF)
			{
				if(token == "vertices") {
					parser->readValueSeparator();
					resource->setVertices(parser->readVector3Array());
				} else if (token == "textureCoordinates") {
					parser->readValueSeparator();
					resource->setTextureCoordinates(parser->readVector2Array());
				} else if (token == "normals") {
					parser->readValueSeparator();
					resource->setNormals(parser->readVector3Array());
				} else if (token == "texture") {
					parser->readValueSeparator();
					String textureFile = parser->readString();
					logger->debug("texture [%s]", textureFile.c_str());
					resource->setTextureFile(textureFile);
				} else if (token == "colors") {
					parser->readValueSeparator();
					resource->setColors(parser->readVector3Array());
				} else if (token == "type") {
					parser->readValueSeparator();
					String typeString = parser->readString();
					resource->setType(valueOf(typeString));
				} else
					logger->warn("Unexpected token: [%s]", token.c_str());
			}

			postProcess(resource);

			log("vertices ", resource->getVertices());
			log("indices ", resource->getIndices());
			log("colors ", resource->getColors());
			log("normals ", resource->getNormals());
			log("textureCoordinates ", resource->getTextureCoordinates());
			logger->debug("type = [%d]", resource->getType());
			return resource;
		}
		virtual void dispose(Resource *resource) {
			GeometryResource *geometryResource = (GeometryResource *)resource;
			geometryResource->getVertices().clear();
			geometryResource->getColors().clear();
			geometryResource->getNormals().clear();
			geometryResource->getTextureCoordinates().clear();
		}

			private:
		//TODO: Review vertex normals generation. Review amount of needed vertex normals (according to number of indexes or unique vertices?). May need to build adapters and factories based on primitive type.
		void ensureNormals(GeometryResource *resource)
		{
			if(resource->getNormals().size() == 0) {
				NormalGenerator *generator = NormalGeneratorFactory::getGenerator(resource->getType());

				if(generator != null)
					generator->generateNormals(resource);
			}

		}

		void buildIndicesArray(GeometryResource *resource)
		{
			//create one to one index array.
			unsigned int currentIndex = 0;

			for(std::vector<vector3>::iterator source = resource->getVertices().begin(); source != resource->getVertices().end(); source ++)
				resource->getIndices().push_back(currentIndex++);
		}

		void ensureColors(GeometryResource *resource)
		{
			unsigned int numberOfColors = (resource->getVertices().size() - resource->getColors().size());
			for(unsigned int currentIndex = 0; currentIndex < numberOfColors; currentIndex++)
				resource->getColors().push_back(vector3(1.0f, 1.0f, 1.0f));
		}

		//TODO: maybe this should go to resource manager framework
		void postProcess(GeometryResource *resource)
		{
			if(resource != null) {
				ensureNormals(resource);
				ensureColors(resource);
				buildIndicesArray(resource);
			}
		}

		void buildVerticesArray(GeometryResource *resource, std::vector<vector3> &vertices)
		{
			if(resource != null && vertices.size() > 0) {
				for(std::vector<vector3>::iterator source = vertices.begin(); source != vertices.end(); source ++)
				{
					unsigned int index = 0;
					boolean preExisting = false;
					for(std::vector<vector3>::iterator destination = resource->getVertices().begin(); destination != resource->getVertices().end(); destination++)
					{
						if(*source == *destination) {
							resource->getIndices().push_back(index);
							preExisting = true;
							break;
						}
						index++;
					}
					if(!preExisting) {
						resource->getVertices().push_back(*source);
						resource->getIndices().push_back(resource->getVertices().size() - 1);
					}
				}
			}
		}
		unsigned int valueOf(const String &typeString) {
			if(typeString == "points")
				return GL_POINTS;
			else if(typeString == "points")
				return GL_LINES;
			else if(typeString == "lineLoop")
				return GL_LINE_LOOP;
			else if(typeString == "lineStrip")
				return GL_LINE_STRIP;
			else if(typeString == "triangles")
				return GL_TRIANGLES;
			else if(typeString == "triangleStrip")
				return GL_TRIANGLE_STRIP;
			else if(typeString == "triangleFan")
				return GL_TRIANGLE_FAN;
			else if(typeString == "quads")
				return GL_QUADS;
			else if(typeString == "quadStrip")
				return GL_QUAD_STRIP;
			else return GL_POLYGON;
		}


		void log(String prefix, std::vector<vector2> array)
		{
			char vectorBuffer[256];

			sprintf(vectorBuffer, "[%d] ", array.size());
			prefix.insert(0, vectorBuffer);
//			prefix.append(" [");
//
//			for(std::vector<vector2>::iterator current = array.begin(); current != array.end(); current++) {
//				sprintf(vectorBuffer, "<%.2f, %.2f> ", (*current).x, (*current).y);
//				prefix.append(vectorBuffer);
//			}
//
//			prefix.append(" ]");
			logger->debug(prefix.c_str());

		}

		void log(String prefix, std::vector<unsigned int> array)
		{
			char vectorBuffer[256];

			sprintf(vectorBuffer, "[%d] ", array.size());
			prefix.insert(0, vectorBuffer);
//			prefix.append(" [");
//
//			for(std::vector<unsigned int>::iterator current = array.begin(); current != array.end(); current++) {
//				sprintf(vectorBuffer, "%d, ", *current);
//				prefix.append(vectorBuffer);
//			}
//
//			prefix.append(" ]");
			logger->debug(prefix.c_str());

		}

		void log(String prefix, std::vector<vector3> array)
		{
			char vectorBuffer[256];

			sprintf(vectorBuffer, "[%d] ", array.size());
			prefix.insert(0, vectorBuffer);
//			prefix.append(" [");
//
//			for(std::vector<vector3>::iterator current = array.begin(); current != array.end(); current++) {
//				sprintf(vectorBuffer, "<%.2f, %.2f, %.2f> ", (*current).x, (*current).y, (*current).z);
//				prefix.append(vectorBuffer);
//			}
//
//			prefix.append(" ]");
			logger->debug(prefix.c_str());

		}
};
#endif /* GEOMETRYRESOURCEADAPTER_H_ */
