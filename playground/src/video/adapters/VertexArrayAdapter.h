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
#include "VideoAdapter.h"
#include <gl/gl.h>
#include <gl/glext.h>

class VertexArrayResourceAdapter: public ResourceAdapter {
	private:
		std::vector<String> supportedMimeTypes;
		Logger *logger;
	private:
		boolean addBuffer(ShaderAttributeLocation attributeLocation, VertexArrayResource *resource, GLenum bufferDestination, const std::vector<vector2> &data)
		{
			if(data.size() > 0) {
				logger->debug("Creating [%d] floats buffer", data.size());

				// create vertex buffer
				unsigned int buffer;
				glGenBuffers(1, &buffer); // Generate our Vertex Buffer Object
				glBindBuffer(bufferDestination, buffer); // bind to GL_ARAY_BUFFER, as we're about to call glBufferData(GL_ARAY_BUFFER)
				glBufferData(bufferDestination, data.size() * sizeof(vector2), (real *)data.data(),  GL_STATIC_DRAW);

				GLenum glError = glGetError();
				if(glError != GL_NO_ERROR) {
					logger->error("Error loading vertex buffer: 0x[%x]", glError);
					return false;
				}

				if(attributeLocation >= 0) {
					resource->addAttribute(attributeLocation, buffer, 0, data.size());
					glVertexAttribPointer((GLuint)attributeLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
					glEnableVertexAttribArray(attributeLocation);
				}

				glError = glGetError();
				if(glError != GL_NO_ERROR) {
					logger->error("Error binding buffer [%d] to attribute Location [%d]: 0x[%x]", attributeLocation, buffer, glError);
					return false;
				}
			}

			return true;
		}
		boolean addBuffer(ShaderAttributeLocation attributeLocation, VertexArrayResource *resource, GLenum bufferDestination, const std::vector<vector3> &data)
		{
				logger->debug("Creating [%d] floats buffer", data.size());

				// create vertex buffer
				unsigned int buffer;
				glGenBuffers(1, &buffer); // Generate our Vertex Buffer Object
				glBindBuffer(bufferDestination, buffer); // bind to GL_ARAY_BUFFER, as we're about to call glBufferData(GL_ARAY_BUFFER)
				glBufferData(bufferDestination, data.size() * sizeof(vector3), (real *)data.data(),  GL_STATIC_DRAW);

				GLenum glError = glGetError();
				if(glError != GL_NO_ERROR) {
					logger->error("Error loading vertex buffer: 0x[%x]", glError);
					return false;
				}

				if(attributeLocation >= 0) {
					resource->addAttribute(attributeLocation, buffer, 0, data.size());
					glVertexAttribPointer((GLuint)attributeLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
					glEnableVertexAttribArray(attributeLocation);
				}

				glError = glGetError();
				if(glError != GL_NO_ERROR) {
					logger->error("Error binding buffer [%d] to attribute location [%d]: 0x[%x]", attributeLocation, buffer, glError);
					return false;
				}

				return true;
		}
		boolean addBuffer(ShaderAttributeLocation attributeLocation, VertexArrayResource *resource, GLenum bufferDestination, const std::vector<unsigned int> &data)
		{
				logger->debug("Creating [%d] floats buffer", data.size());

				// create vertex buffer
				unsigned int buffer;
				glGenBuffers(1, &buffer); // Generate our Vertex Buffer Object
				glBindBuffer(bufferDestination, buffer); // bind to GL_ARAY_BUFFER, as we're about to call glBufferData(GL_ARRAY_BUFFER)
				glBufferData(bufferDestination, data.size() * sizeof(unsigned int), (unsigned int *)data.data(),  GL_STATIC_DRAW);

				GLenum glError = glGetError();
				if(glError != GL_NO_ERROR) {
					logger->error("Error loading vertex buffer: 0x[%x]", glError);
					return false;
				}

				if(attributeLocation >= 0) {
					resource->addAttribute(attributeLocation, buffer, 0, data.size());
					glVertexAttribPointer((GLuint)attributeLocation, 1, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
					glEnableVertexAttribArray(attributeLocation);
				}

				glError = glGetError();
				if(glError != GL_NO_ERROR) {
					logger->error("Error binding buffer [%d] to location [%d]: 0x[%x]", buffer, attributeLocation, glError);
					return false;
				}

				return true;
		}

	public:
		VertexArrayResourceAdapter() {
			supportedMimeTypes.push_back("video/vertexArray");
			logger = Logger::getLogger("video/VertexArrayResourceAdapter.h");
		}
		virtual const std::vector<String> getSupportedMimeTypes() {
			return supportedMimeTypes;
		}
		virtual Resource *load(FileParser &fileParser, const String &mimeType) {
			GeometryResource *geometry = (GeometryResource *) this->getResourceManager()->load(fileParser, "video/geometry");

			if(geometry != null) {
				logger->debug("Loaded geometry: [%d] vertices, [%d] normals, [%d] texture coordinates", geometry->getVertices().size(), geometry->getNormals().size(), geometry->getTextureCoordinates().size());

				glGetError();

				//Create vertex Array
				unsigned int vertexArray;
				glGenVertexArrays(1, &vertexArray);
				VertexArrayResource *resource = new VertexArrayResource(vertexArray);
				resource->setPrimitiveType(geometry->getType());
				TextureResource *texture = (TextureResource *)this->getResourceManager()->load(geometry->getTextureFile(), "video/texture");
				if(texture != null)
					resource->setTexture(texture);

				glBindVertexArray(resource->getId());

				GLenum glError = glGetError();
				if(glError != GL_NO_ERROR) {
					logger->error("Error creating vertex array [%s]: 0x[%x]", fileParser.getFilename().c_str(), glError);
					dispose(resource);
					return null;
				}

				if(!addBuffer(VERTEX_LOCATION, resource, GL_ARRAY_BUFFER, geometry->getVertices()))
				{
					dispose(resource);
					return null;
				}

				if(!addBuffer(INDEX_LOCATION, resource, GL_ELEMENT_ARRAY_BUFFER, geometry->getIndexes()))
				{
					dispose(resource);
					return null;
				}

				if(!addBuffer(NORMAL_LOCATION, resource, GL_ARRAY_BUFFER, geometry->getNormals()))
				{
					dispose(resource);
					return null;
				}

				if(!addBuffer(TEXTURE_COORDINATES_LOCATION, resource, GL_ARRAY_BUFFER, geometry->getTextureCoordinates()))
				{
					dispose(resource);
					return null;
				}

				if(!addBuffer(COLOR_LOCATION, resource, GL_ARRAY_BUFFER, geometry->getColors()))
				{
					dispose(resource);
					return null;
				}

				// remove objects from context.
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);

				return resource;
			}
			return null;
		}
		virtual void dispose(Resource *resource) {
			VertexArrayResource *vertexArrayResource = (VertexArrayResource *)resource;

			if(vertexArrayResource->getId() != 0) {
				glBindVertexArray(vertexArrayResource->getId());

				for(std::map<unsigned int, VertexAttribPointer>::iterator currentBuffer = vertexArrayResource->getAttributes().begin(); currentBuffer != vertexArrayResource->getAttributes().end(); currentBuffer++)
				{
					glDisableVertexAttribArray(currentBuffer->first);
					unsigned int buffer = currentBuffer->second.getBuffer();
					glDeleteBuffers(1, &buffer);
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
