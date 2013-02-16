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
			GeometryResource *geometry = (GeometryResource *) this->getResourceManager()->load(fileParser, "video/geometry");

			unsigned int vertexArray;
			glGenVertexArrays(1, &vertexArray); // Create our Vertex Array Object
			glBindVertexArray(vertexArray); // Bind our Vertex Array Object so we can use it

			unsigned int vertexBuffer;
			glGenBuffers(1, &vertexBuffer); // Generate our Vertex Buffer Object
			glBindBuffer(GL_ARRAY_BUFFER, vertexArray); // Bind our Vertex Buffer Object

			for(std::vector<vector2>::iterator iterator = geometry->getVertices().begin(); iterator != geometry->getVertices().begin(); iterator++)
				glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(real), (float *)*iterator, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

			glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer

			glEnableVertexAttribArray(0); // Disable our Vertex Array Object
			glBindVertexArray(0); // Disable our Vertex Buffer Object

			VertexBufferResource *resource = new VertexBufferResource(vertexArray);
			return resource;
		}
		virtual void dispose(Resource *resource) {
			VertexBufferResource *vertexBufferResource = (VertexBufferResource *)resource;
		}
};
#endif /* VERTEXBUFFERADAPTER_H_ */
