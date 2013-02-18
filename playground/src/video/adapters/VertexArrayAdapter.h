/*
 * VertexBufferAdapter.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#ifndef VERTEXBUFFERADAPTER_H_
#define VERTEXBUFFERADAPTER_H_
#include "resources/ResourceAdapter.h"
#include "../resources/VertexArrayResource.h"
#include <gl/gl.h>
#include <gl/glext.h>

class VertexArrayResourceAdapter: public ResourceAdapter {
	private:
		std::vector<String> supportedMimeTypes;
		Logger *logger;
	public:
		VertexArrayResourceAdapter() {
			supportedMimeTypes.push_back("video/vertexArray");
			logger = Logger::getLogger("video/VertexArrayResourceAdapter.h");
		}
		virtual const std::vector<String> getSupportedMimeTypes() {
			return supportedMimeTypes;
		}
		virtual Resource *load(FileParser &fileParser) {
			GeometryResource *geometry = (GeometryResource *) this->getResourceManager()->load(fileParser, "video/geometry");

			if(geometry != null) {
				logger->debug("Loaded geometry: [%d] vertices, [%d] normals, [%d] texture coordinates", geometry->getVertices().size(), geometry->getNormals().size(), geometry->getTextureCoordinates().size());

				glGetError();

				//Create vertex Array
				unsigned int vertexArray;
				glGenVertexArrays(1, &vertexArray);
				VertexArrayResource *resource = new VertexArrayResource(vertexArray);

				glBindVertexArray(resource->getId());

				GLenum glError = glGetError();
				if(glError != GL_NO_ERROR) {
					logger->error("Error creating vertex array [%s]: 0x[%x]", fileParser.getFilename().c_str(), glError);
					dispose(resource);
					return null;
				}

				// create vertex buffer
				unsigned int vertexBuffer;
				glGenBuffers(1, &vertexBuffer); // Generate our Vertex Buffer Object
				glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); // bind to GL_ARAY_BUFFER, as we're about to call glBufferData(GL_ARAY_BUFFER)
				glBufferData(GL_ARRAY_BUFFER, geometry->getVertices().size() * sizeof(vector2), (float *)geometry->getVertices().data(),  GL_STATIC_DRAW);

				glError = glGetError();
				if(glError != GL_NO_ERROR) {
					logger->error("Error loading vertex buffer [%s]: 0x[%x]", fileParser.getFilename().c_str(), glError);
					dispose(resource);
					return null;
				}

				unsigned int vertexPointer = resource->addBuffer(vertexBuffer);
				glVertexAttribPointer((GLuint)vertexPointer, 2, GL_FLOAT, GL_FALSE, 0, 0);
				glEnableVertexAttribArray(vertexPointer);

				// remove objects from context.
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);

				glError = glGetError();
				if(glError != GL_NO_ERROR) {
					logger->error("Error relating vertex array and vertex buffer [%s]: 0x[%x]", fileParser.getFilename().c_str(), glError);
					dispose(resource);
					return null;
				}

				return resource;
			}
			return null;
		}
		virtual void dispose(Resource *resource) {
			VertexArrayResource *vertexArrayResource = (VertexArrayResource *)resource;

			if(vertexArrayResource->getId() != 0) {
				glBindVertexArray(vertexArrayResource->getId());
				for(unsigned int index = 0; index < vertexArrayResource->getVertexAttribPointers().size(); index++) {
					glDisableVertexAttribArray(index);
					unsigned int vertexBuffer = vertexArrayResource->getVertexAttribPointers()[index];
					glDeleteBuffers(1, &vertexBuffer);
				}

				vertexArrayResource->clearVertexAttribPointers();

				unsigned int vertexArray = vertexArrayResource->getId();
				glDeleteVertexArrays(1, &vertexArray);

				glBindVertexArray(0);
				vertexArrayResource->setId(0);
			}
		}
};
#endif /* VERTEXBUFFERADAPTER_H_ */
