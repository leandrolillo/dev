/*
 * VertexBufferAdapter.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#ifndef VERTEXBUFFERADAPTER_H_
#define VERTEXBUFFERADAPTER_H_
#include "resources/ResourceAdapter.h"
#include "../resources/VertexBufferResource.h"
#include <gl/gl.h>
#include <gl/glext.h>

class VertexBufferResourceAdapter: public ResourceAdapter {
	private:
		std::vector<String> supportedMimeTypes;
		Logger *logger;
	public:
		VertexBufferResourceAdapter() {
			supportedMimeTypes.push_back("video/vertexBuffer");
			logger = Logger::getLogger("video/VertexBufferResourceAdapter.h");
		}
		virtual const std::vector<String> getSupportedMimeTypes() {
			return supportedMimeTypes;
		}
		virtual Resource *load(FileParser &fileParser) {
			VertexBufferResource *resource = new VertexBufferResource(0);
			return resource;
		}
		virtual void dispose(Resource *resource) {
			VertexBufferResource *vertexBufferResource = (VertexBufferResource *)resource;
		}
};
#endif /* VERTEXBUFFERADAPTER_H_ */
