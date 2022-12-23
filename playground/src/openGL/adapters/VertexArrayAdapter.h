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
		this->produces(MimeTypes::VERTEXARRAY);
	}

	virtual Resource *load(ResourceLoadRequest &request) const override {
		GeometryCollection *geometry = (GeometryCollection*) this->getResourceManager()->load(request.getFileParser(),
				MimeTypes::GEOMETRYCOLLECTION,
				std::set<String> { ResourceManager::EphemeralLabel });
		if (geometry == null || geometry->getObjects().empty()) {
		    logger->error("Could not load geometry from %s with mimetype %s", request.getFilePath().c_str(), MimeTypes::GEOMETRYCOLLECTION.c_str());
		    return null;
		}

		return OpenGLUtilites::generateVertexBuffer(geometry->getObjects().begin()->second); // no need to explicitly add this object to resource manager since it is the result of load method
	}

	virtual void dispose(Resource *resource) const override {
		OpenGLUtilites::disposeVertexArray((VertexArrayResource *)resource);
	}
};
#endif /* VERTEXBUFFERADAPTER_H_ */
