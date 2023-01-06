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
		ResourceLoadRequest geometryCollectionRLR(request);
		GeometryCollection *geometryCollection = (GeometryCollection*) this->getResourceManager()->load(
				geometryCollectionRLR.acceptMimeType(MimeTypes::GEOMETRYCOLLECTION).withAdditionalLabels(std::set<String> { ResourceManager::EphemeralLabel })
		);
		if (geometryCollection == null || geometryCollection->getObjects().empty()) {
		    logger->error("Could not load geometry from %s with mimetype %s", request.getFilePath().c_str(), MimeTypes::GEOMETRYCOLLECTION.c_str());
		    return null;
		}

		//VertexArrayResource *result = OpenGLUtilites::generateVertexBuffer(geometryCollection->getObjects().begin()->second);

		Resource *result = null;
		for(auto & geometry : geometryCollection->getObjects()) {
			Resource *resource = this->getResourceManager()->addResource(OpenGLUtilites::generateVertexBuffer(geometry.second)); //make sure we add it to resource manager to avoid leaks
			result = (result == null ? resource: result);
		}

		return result;
	}

	virtual void dispose(Resource *resource) const override {
		OpenGLUtilites::disposeVertexArray((VertexArrayResource *)resource);
	}
};
#endif /* VERTEXBUFFERADAPTER_H_ */
