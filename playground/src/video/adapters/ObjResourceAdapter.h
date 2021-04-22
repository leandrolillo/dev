/*
 * ObjResourceAdapter.h
 *
 *  Created on: Apr 13, 2021
 *      Author: leandro
 */

#ifndef SRC_VIDEO_ADAPTERS_OBJRESOURCEADAPTER_H_
#define SRC_VIDEO_ADAPTERS_OBJRESOURCEADAPTER_H_

#include<ResourceAdapter.h>

class ObjResourceAdapter: public ResourceAdapter {
public:
    ObjResourceAdapter() {
        logger = Logger::getLogger("video/ObjResourceAdapter");
        this->addSupportedMimeType("video/obj");
    }

    virtual Resource* load(FileParser &fileParser, const String &mimeType) {
        GeometryResource *geometry = new GeometryResource(0);
        geometry->setType("triangles");
        TextParser textParser(fileParser);

        std::vector<vector> normals;
        std::vector<vector2> textCoords;
        std::vector<vector> indices;
        String token;
        while ((token = fileParser.takeToken()) != FileParser::eof) {
            if (token == "o") {
                //logger->info("Object %s", fileParser.takeLine().c_str());
            } else if (token == "v") {
                geometry->getVertices().push_back(vector(textParser.readReal(), textParser.readReal(), textParser.readReal()));
            } else if (token == "vn") {
                normals.push_back(vector(textParser.readReal(), textParser.readReal(), textParser.readReal()));
            } else if (token == "vt") {
                textCoords.push_back(vector2(textParser.readReal(), textParser.readReal()));
            } else if (token == "f") {
                indices.push_back(readIndicesRow(textParser));
                indices.push_back(readIndicesRow(textParser));
                indices.push_back(readIndicesRow(textParser));
            } else {
                String line = fileParser.takeLine().c_str();
                logger->warn("skipping [%s] [%s]", token.c_str(), line.c_str());
            }
        }

//        logger->info("Parsed [%s] file, converting to geometry...", fileParser.getFilename().c_str());

        for (unsigned int index = 0; index < indices.size(); index++) {
            vector currentIndices = indices[index];
//            logger->info("Processing indices [%u]: %s", index, currentIndices.toString("%.f").c_str());

            vector *previousSameVertexDiffRest = null;
            for (unsigned int duplicateIndex = 0; duplicateIndex < index; duplicateIndex++) {
                if ((int) indices[duplicateIndex].x == (int) currentIndices.x
                        && ((int) indices[duplicateIndex].y != (int) currentIndices.y || (int) indices[duplicateIndex].z != (int) currentIndices.z)) {
                    previousSameVertexDiffRest = &indices[duplicateIndex];
                    break;
                }
            }

            if (previousSameVertexDiffRest != null) {
//                logger->info("Found previous indices for the same vertex [%u] %s", index, previousSameVertexDiffRest->toString().c_str());
                geometry->getVertices().push_back(geometry->getVertices().at((int) currentIndices.x - 1));
                textCoords.push_back(textCoords.at((int) currentIndices.y - 1));
                normals.push_back(normals.at((int) currentIndices.z - 1));

                currentIndices.x = geometry->getVertices().size();
                currentIndices.y = textCoords.size();
                currentIndices.z = normals.size();
            }

            geometry->getIndices().push_back((int) currentIndices.x - 1);
            if (currentIndices.y > 0) {
                ensureTextureCoordinates(geometry);
                geometry->getTextureCoordinates().at((int) currentIndices.x - 1) = textCoords.at((int) currentIndices.y - 1);
            }
            if (currentIndices.z > 0) {
                ensureNormals(geometry);
                geometry->getNormals().at((int) currentIndices.x - 1) = normals.at((int) currentIndices.z - 1);
            }
        }

        normals.clear();
        textCoords.clear();

        printLogInfo(geometry);

        return geometry;
    }
private:
    void printLogInfo(GeometryResource *geometry) {
        logger->info("%d vertices", geometry->getVertices().size());
        logger->info("%d textCoords", geometry->getTextureCoordinates().size());
        logger->info("%d normals", geometry->getNormals().size());
        logger->info("%d indices", geometry->getIndices().size());

        for (std::vector<unsigned int>::iterator indexIterator = geometry->getIndices().begin();
                indexIterator != geometry->getIndices().end(); indexIterator++) {
            logger->info("%d: %s - %s - %s", *indexIterator, geometry->getVertices().at(*indexIterator).toString("%.6f").c_str(),
                    geometry->getTextureCoordinates().at(*indexIterator).toString("%.6f").c_str(),
                    geometry->getNormals().at(*indexIterator).toString("%.6f").c_str());
        }
    }
    void ensureTextureCoordinates(GeometryResource *geometry) {
        for (unsigned int index = geometry->getTextureCoordinates().size(); index < geometry->getVertices().size(); index++) {
            geometry->getTextureCoordinates().push_back(vector2(0, 0));

        }
    }

    void ensureNormals(GeometryResource *geometry) {
        for (unsigned int index = geometry->getNormals().size(); index < geometry->getVertices().size(); index++) {
            geometry->getNormals().push_back(vector3(0, 0, 0));
        }
    }
    vector readIndicesRow(TextParser &textParser) {

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
