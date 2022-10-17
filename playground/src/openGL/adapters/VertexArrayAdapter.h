/*
 * VertexBufferAdapter.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#ifndef VERTEXBUFFERADAPTER_H_
#define VERTEXBUFFERADAPTER_H_
#include <adapters/OpenGLResourceAdapter.h>
#include <GeometryCollection.h>
#include <set>
#include<OpenGLUtilities.h>


class VertexArrayResourceAdapter: public OpenGLResourceAdapter {
public:
	VertexArrayResourceAdapter() {
		logger = LoggerFactory::getLogger("video/VertexArrayResourceAdapter");
		this->addSupportedMimeType("video/vertexArray");
	}

	virtual Resource *load(FileParser &fileParser, const String &mimeType) const override {
	    String geometryMimeType;

        geometryMimeType = this->getResourceManager()->guessMimeType(fileParser.getFilename());
        geometryMimeType = geometryMimeType.empty() ? "video/geometry" : geometryMimeType;

		GeometryCollection *geometry = (GeometryCollection*) this->getResourceManager()->load(fileParser, geometryMimeType, std::set<String> { ResourceManager::EphemeralLabel });
		if (geometry == null || geometry->getObjects().empty()) {
		    logger->error("Could not load geometry from %s with mimetype %s", fileParser.getFilename().c_str(), geometryMimeType.c_str());
		    return null;
		}

		return OpenGLUtilites::generateVertexBuffer(geometry->getObjects().begin()->second);
	}

	virtual void dispose(Resource *resource) const override {
		OpenGLUtilites::disposeVertexArray((VertexArrayResource *)resource);
	}
};
#endif /* VERTEXBUFFERADAPTER_H_ */
