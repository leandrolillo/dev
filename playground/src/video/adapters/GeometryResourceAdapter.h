/*
 * GeometryResourceAdapter.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#ifndef GEOMETRYRESOURCEADAPTER_H_
#define GEOMETRYRESOURCEADAPTER_H_
#include <InvalidArgumentException.h>
#include <ResourceAdapter.h>
#include <resources/GeometryResource.h>
#include <JsonParser.h>
#include "normalGenerators/NormalGenerator.h"
#include <GeometryCollection.h>


class GeometryResourceAdapter: public ResourceAdapter {
public:
	GeometryResourceAdapter() {
		logger = LoggerFactory::getLogger("video/GeometryResourceAdapter");
		this->addSupportedMimeType("video/geometry");
	}

	virtual Resource *load(FileParser &fileParser, const String &mimeType) const override {
		JsonParser parser(fileParser);
		GeometryCollection *geometryCollection = new GeometryCollection;
		GeometryResource *resource = new GeometryResource(0);
		resource->setName(Paths::getBasename(fileParser.getFilename()));

		bool generateNormals = false;
		bool generateIndexes = false;

		String token;
		parser.readStartObject();
		while ((token = parser.readToken()) != END_OBJECT && token != FileParser::eof) {
            parser.readValueSeparator();

			if (token == "vertices") {
				resource->setVertices(parser.readVector3Array());
			} else if (token == "textureCoordinates") {
				resource->setTextureCoordinates(parser.readVector2Array());
			} else if (token == "normals") {
				resource->setNormals(parser.readVector3Array());
			} else if (token == "colors") {
				resource->setColors(parser.readVector3Array());
			} else if (token == "type") {
				String typeString = parser.readString();
				resource->setType(typeString);
			} else if (token == "indices") {
				resource->setIndices(parser.readUnsignedIntegerArray());
			} else if (token == "generateNormals") {
				generateNormals = parser.readBoolean();
			} else if (token == "generateIndexes") {
				generateIndexes = parser.readBoolean();
			} else {
				logger->error("Unexpected token: [%s] at (%d, %d)",
						token.c_str(), parser.getLine(), parser.getColumn());
			}

			if (parser.peekToken() == ",") {
				parser.readToken();
			}
		}

		if (generateNormals && resource->getNormals().size() == 0) {
			ensureNormals(resource);
		}

		if (generateIndexes && resource->getIndices().size() == 0) {
			buildIndicesArray(resource);
		}

		logger->debug(
				"Primitive %s: [%d] vertices, [%d] indices, [%d] colors, [%d] normals, [%d] textureCoordinates",
				resource->getType().c_str(), resource->getVertices().size(),
				resource->getIndices().size(), resource->getColors().size(),
				resource->getNormals().size(),
				resource->getTextureCoordinates().size());

		log("vertices ", resource->getVertices());
		log("indices ", resource->getIndices());
		log("colors ", resource->getColors());
		log("normals ", resource->getNormals());
		log("textureCoordinates ", resource->getTextureCoordinates());

		getResourceManager()->addResource(resource);
		geometryCollection->addObject(resource);

		return geometryCollection;
	}

	/**
	 * This is not really necessary since vector takes care of its own data.
	 */
//	virtual void dispose(Resource *resource) const override {
//	    logger->info("Disposing of [%s]", resource->toString().c_str());
//
//		GeometryResource *geometryResource = (GeometryResource*) resource;
//		geometryResource->getVertices().clear();
//		geometryResource->getColors().clear();
//		geometryResource->getNormals().clear();
//		geometryResource->getTextureCoordinates().clear();
//
//		logger->info("Disposed of %s", resource->toString().c_str());
//	}

private:
	//TODO: Review vertex normals generation. Review amount of needed vertex normals (according to number of indexes or unique vertices?). May need to build adapters and factories based on primitive type.
	void ensureNormals(GeometryResource *resource) const {
		if (resource->getNormals().size() == 0) {
			NormalGenerator *generator = NormalGeneratorFactory::getGenerator(
					resource->getType());

			if (generator != null)
				generator->generateNormals(resource);
		}
	}

	void buildIndicesArray(GeometryResource *resource) const {
		//create one to one index array.
		unsigned int currentIndex = 0;

		for (std::vector<vector3>::iterator source =
				resource->getVertices().begin();
				source != resource->getVertices().end(); source++) {
			resource->getIndices().push_back(currentIndex++);
		}
	}

	void ensureColors(GeometryResource *resource) const {
		unsigned int numberOfColors = (resource->getVertices().size()
				- resource->getColors().size());
		for (unsigned int currentIndex = 0; currentIndex < numberOfColors;
				currentIndex++)
			resource->getColors().push_back(vector3(1.0f, 1.0f, 1.0f));
	}

	void buildVerticesArray(GeometryResource *resource,
			std::vector<vector3> &vertices) const {
		if (resource != null && vertices.size() > 0) {
			for (std::vector<vector3>::iterator source = vertices.begin();
					source != vertices.end(); source++) {
				unsigned int index = 0;
				bool preExisting = false;
				for (std::vector<vector3>::iterator destination =
						resource->getVertices().begin();
						destination != resource->getVertices().end();
						destination++) {
					if (*source == *destination) {
						resource->getIndices().push_back(index);
						preExisting = true;
						break;
					}
					index++;
				}
				if (!preExisting) {
					resource->getVertices().push_back(*source);
					resource->getIndices().push_back(
							resource->getVertices().size() - 1);
				}
			}
		}
	}

	void log(String prefix, std::vector<vector2> array) const {
		char vectorBuffer[256];

		sprintf(vectorBuffer, "[%lu] ", array.size());
		prefix.insert(0, vectorBuffer);
		prefix.append(" [");

		for (std::vector<vector2>::iterator current = array.begin();
				current != array.end(); current++) {
			sprintf(vectorBuffer, "<%.2f, %.2f> ", (*current).x, (*current).y);
			prefix.append(vectorBuffer);
		}

		prefix.append(" ]");
		logger->verbose(prefix.c_str());

	}

	void log(String prefix, std::vector<unsigned int> array) const {
		char vectorBuffer[256];

		sprintf(vectorBuffer, "[%lu] ", array.size());
		prefix.insert(0, vectorBuffer);
		prefix.append(" [");

		for (std::vector<unsigned int>::iterator current = array.begin();
				current != array.end(); current++) {
			sprintf(vectorBuffer, "%u, ", *current);
			prefix.append(vectorBuffer);
		}

		prefix.append(" ]");
		logger->verbose(prefix.c_str());

	}

	void log(String prefix, std::vector<vector3> array) const {
		char vectorBuffer[256];

		sprintf(vectorBuffer, "[%lu] ", array.size());
		prefix.insert(0, vectorBuffer);
		prefix.append(" [");

		for (std::vector<vector3>::iterator current = array.begin();
				current != array.end(); current++) {
			sprintf(vectorBuffer, "<%.2f, %.2f, %.2f> ", (*current).x,
					(*current).y, (*current).z);
			prefix.append(vectorBuffer);
		}

		prefix.append(" ]");
		logger->verbose(prefix.c_str());

	}
};
#endif /* GEOMETRYRESOURCEADAPTER_H_ */
