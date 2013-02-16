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

				// create vertex buffer
				unsigned int vertexBuffer;
				glGenBuffers(1, &vertexBuffer); // Generate our Vertex Buffer Object
				for(std::vector<vector2>::iterator iterator = geometry->getVertices().begin(); iterator != geometry->getVertices().end(); iterator++)
					glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(real), (float *)*iterator,  GL_STATIC_DRAW);

				// bind vertex buffer to vertex array
				glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
				unsigned int vertexPointer = resource->addBuffer(vertexBuffer);
				glVertexAttribPointer((GLuint)vertexPointer, 2, GL_FLOAT, GL_FALSE, 0, 0);
				glEnableVertexAttribArray(vertexPointer);

				// remove objects from context.
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);

				GLenum glError = glGetError();
				if(glError != GL_NO_ERROR) {
					logger->error("Error loading vertex array [%s]: 0x[%x]", fileParser.getFilename().c_str(), glError);
					dispose(resource);
					return null;
				}

				return resource;
			}
			return null;
		}
		virtual void dispose(Resource *resource) {
			VertexArrayResource *vertexArrayResource = (VertexArrayResource *)resource;

			glBindVertexArray(vertexArrayResource->getId());
			for(unsigned int index = 0; index < vertexArrayResource->getVertexAttribPointers().size(); index++) {
				glDisableVertexAttribArray(index);
				unsigned int vertexBuffer = vertexArrayResource->getVertexAttribPointers()[index];
				glDeleteBuffers(1, &vertexBuffer);
			}

			unsigned int vertexArray = vertexArrayResource->getId();
			glDeleteVertexArrays(1, &vertexArray);

			glBindVertexArray(0);
		}
};
#endif /* VERTEXBUFFERADAPTER_H_ */
