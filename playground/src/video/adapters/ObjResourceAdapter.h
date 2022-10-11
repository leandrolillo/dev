/*
 * ObjResourceAdapter.h
 *
 *  Created on: Apr 13, 2021
 *      Author: leandro
 */

#ifndef SRC_VIDEO_ADAPTERS_OBJRESOURCEADAPTER_H_
#define SRC_VIDEO_ADAPTERS_OBJRESOURCEADAPTER_H_

#include<ResourceAdapter.h>
#include<GeometryResource.h>

class ObjResourceAdapter: public ResourceAdapter {
public:
    ObjResourceAdapter() {
        logger = LoggerFactory::getLogger("video/ObjResourceAdapter");
        this->addSupportedMimeType("video/obj");
        this->addSupportedMimeType("model/obj");
    }

    virtual Resource *load(FileParser &fileParser, const String &mimeType) const override {
        TextParser textParser(fileParser);

    	/**
    	 * Auxiliar variables for loading. Obj file format define one global list of vertices, one of normals and one of textCoords. All objects within the file reference this uber-list when using indices.
    	 */
    	std::vector<vector> vertices;
        std::vector<vector> normals;
        std::vector<vector2> textCoords;

        std::vector<GeometryResource *>objects;

        String token;
        while ((token = textParser.peekToken()) != FileParser::eof) {
            if (token == "o" || token == "v" || token == "vn" || token == "vt" || token == "f") {
            	objects.push_back(parseObject(textParser, vertices, normals, textCoords));
            } else if (token == "mtllib") {
            	textParser.takeToken();

            	String materialLibraryName = textParser.takeLine();
            	Resource *materials = this->getResourceManager()->load(textParser.getFilename(), materialLibraryName, "model/mtl");
            	if(materials == null) {
            		logger->warn("Could not load material library [%s] referenced from [%s]", materialLibraryName.c_str(), textParser.getFilename().c_str());
            	}
            } else {
                String line = textParser.takeLine().c_str();
                logger->warn("skipping [%s] [%s]", token.c_str(), line.c_str());
            }
        }

//        logger->info("Parsed [%s] file, converting to geometry...", fileParser.getFilename().c_str());



        if(objects.empty()) {
        	return null;
        }

        return *(objects.begin());
    }

    GeometryResource *parseObject(TextParser &textParser,
		std::vector<vector> &vertices,
		std::vector<vector> &normals,
		std::vector<vector2> &textCoords) const {

        GeometryResource *geometry = new GeometryResource(0);
        geometry->setName(Paths::getBasename(textParser.getFilename()));
        geometry->setType("triangles");
        geometry->setName(Paths::getBasename(textParser.getFilename()));


        logger->info("Parsing object");

        String token;
        while ((token = textParser.takeToken()) != FileParser::eof) {
        	if (token == "o") {
        		String name = textParser.takeLine();
        		logger->info("Name: %s", name.c_str());
        		geometry->setName(name);
        	} else if (token == "v") {
				vertices.push_back(vector(textParser.readReal(), textParser.readReal(), textParser.readReal()));
			} else if (token == "vn") {
				normals.push_back(vector(textParser.readReal(), textParser.readReal(), textParser.readReal()));
			} else if (token == "vt") {
				textCoords.push_back(vector2(textParser.readReal(), textParser.readReal()));
			} else if (token == "f") {
				vector indices = readIndicesRow(textParser);
	            geometry->getIndices().push_back(geometry->getIndices().size());
				geometry->getVertices().push_back(vertices.at((int) indices.x - 1));
				geometry->getTextureCoordinates().push_back(indices.y > 0 ? textCoords.at((int) indices.x - 1) : vector2(0, 0));
	            geometry->getNormals().push_back(indices.z > 0 ? normals.at((int) indices.z - 1) : vector3(0, 0, 0));

				indices = readIndicesRow(textParser);
	            geometry->getIndices().push_back(geometry->getIndices().size());
				geometry->getVertices().push_back(vertices.at((int) indices.x - 1));
				geometry->getTextureCoordinates().push_back(indices.y > 0 ? textCoords.at((int) indices.x - 1) : vector2(0, 0));
	            geometry->getNormals().push_back(indices.z > 0 ? normals.at((int) indices.z - 1) : vector3(0, 0, 0));

				indices = readIndicesRow(textParser);
	            geometry->getIndices().push_back(geometry->getIndices().size());
				geometry->getVertices().push_back(vertices.at((int) indices.x - 1));
				geometry->getTextureCoordinates().push_back(indices.y > 0 ? textCoords.at((int) indices.x - 1) : vector2(0, 0));
	            geometry->getNormals().push_back(indices.z > 0 ? normals.at((int) indices.z - 1) : vector3(0, 0, 0));

				String remaining = textParser.takeLine();
				StringUtils::trim(remaining);
				if(!remaining.empty()) {
					logger->error("Expected triangulated obj faces - got extra [%s] at line %d, column %d", remaining.c_str(), textParser.getLine(), textParser.getColumn());
					delete geometry;
					return null;
				}

			} else {
                String line = textParser.takeLine();
                logger->warn("skipping [%s] [%s]", token.c_str(), line.c_str());
            }

        	if(textParser.peekToken() == "o") {
        		break;
        	}
        }

        logger->info("Read %d vertices, %d textCoords, %d normals", geometry->getVertices().size(), textCoords.size(), normals.size());

        printLogInfo(geometry);

        return geometry;
    }
private:
    void printLogInfo(GeometryResource *geometry) const {
    	logger->info("Object ", geometry->getName().c_str());
        logger->info("%d vertices", geometry->getVertices().size());
        logger->info("%d textCoords", geometry->getTextureCoordinates().size());
        logger->info("%d normals", geometry->getNormals().size());
        logger->info("%d indices", geometry->getIndices().size());

        for (std::vector<unsigned int>::iterator indexIterator = geometry->getIndices().begin();
                indexIterator != geometry->getIndices().end(); indexIterator++) {
            logger->verbose("%d: %s - %s - %s", *indexIterator, geometry->getVertices().at(*indexIterator).toString("%.6f").c_str(),
                    geometry->getTextureCoordinates().at(*indexIterator).toString("%.6f").c_str(),
                    geometry->getNormals().at(*indexIterator).toString("%.6f").c_str());
        }
    }

    vector readIndicesRow(TextParser &textParser) const {

        int vertexIndex = textParser.readInteger();
        int normalIndex = 0;
        int textCoordIndex = 0;

        if (textParser.takeToken() == "/") {
            if (textParser.peekToken() != "/") {
                textCoordIndex = textParser.readInteger();
            }
        }

        if (textParser.takeToken() == "/") {
            normalIndex = textParser.readInteger();
        }

        return vector(vertexIndex, textCoordIndex, normalIndex);
    }

};

#endif /* SRC_VIDEO_ADAPTERS_OBJRESOURCEADAPTER_H_ */
