/*
 * VertexBufferAdapter.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#ifndef VERTEXBUFFERADAPTER_H_
#define VERTEXBUFFERADAPTER_H_
#include <ResourceAdapter.h>
#include <resources/VertexArrayResource.h>

#include <OpenGl/gl3.h>
#include <OpenGl/glext.h>

class VertexArrayResourceAdapter: public ResourceAdapter {
private:
	bool addBuffer(ShaderAttributeLocation attributeLocation,
			VertexArrayResource *resource, GLenum bufferDestination,
			const std::vector<vector2> &data) {
		if (data.size() > 0) {
			logger->verbose("Creating [%d] vector2 buffer for attribute [%d]",
					data.size(), attributeLocation);

			// create vertex buffer
			unsigned int buffer;
			glGenBuffers(1, &buffer); // Generate our Vertex Buffer Object
			glBindBuffer(bufferDestination, buffer); // bind to GL_ARAY_BUFFER, as we're about to call glBufferData(GL_ARAY_BUFFER)
			glBufferData(bufferDestination, data.size() * sizeof(vector2),
					(real*) data.data(), GL_STATIC_DRAW);

			GLenum glError = glGetError();
			if (glError != GL_NO_ERROR) {
				logger->error("Error loading vertex buffer: 0x[%x]", glError);
				return false;
			}

			if (attributeLocation >= 0) {
				resource->addAttribute(attributeLocation, buffer, 0,
						data.size());
				glVertexAttribPointer((GLuint) attributeLocation, 2, GL_FLOAT,
						GL_FALSE, 0, 0);
				glEnableVertexAttribArray(attributeLocation);
				logger->verbose("Enabled attribute [%d]", attributeLocation);
			}

			glError = glGetError();
			if (glError != GL_NO_ERROR) {
				logger->error(
						"Error binding buffer [%d] to attribute Location [%d]: 0x[%x]",
						attributeLocation, buffer, glError);
				return false;
			}
		}

		return true;
	}

	bool addBuffer(ShaderAttributeLocation attributeLocation,
			VertexArrayResource *resource, GLenum bufferDestination,
			const std::vector<vector3> &data) {
		logger->verbose("Creating [%d] vector3 buffer for attribute [%d]",
				data.size(), attributeLocation);

		// create vertex buffer
		unsigned int buffer;
		glGenBuffers(1, &buffer); // Generate our Vertex Buffer Object
		glBindBuffer(bufferDestination, buffer); // bind to GL_ARAY_BUFFER, as we're about to call glBufferData(GL_ARAY_BUFFER)
		glBufferData(bufferDestination, data.size() * sizeof(vector3),
				(real*) data.data(), GL_STATIC_DRAW);

		GLenum glError = glGetError();
		if (glError != GL_NO_ERROR) {
			logger->error("Error loading vertex buffer: 0x[%x]", glError);
			return false;
		}

		if (attributeLocation >= 0) {
			resource->addAttribute(attributeLocation, buffer, 0, data.size());
			glVertexAttribPointer((GLuint) attributeLocation, 3, GL_FLOAT,
					GL_FALSE, 0, 0);
			glEnableVertexAttribArray(attributeLocation);
			logger->verbose("Enabled attribute [%d]", attributeLocation);
		}

		glError = glGetError();
		if (glError != GL_NO_ERROR) {
			logger->error(
					"Error binding buffer [%d] to attribute location [%d]: 0x[%x]",
					attributeLocation, buffer, glError);
			return false;
		}

		return true;
	}
	bool addBuffer(ShaderAttributeLocation attributeLocation,
			VertexArrayResource *resource, GLenum bufferDestination,
			const std::vector<unsigned int> &data) {
		logger->verbose("Creating [%d] unsigned int buffer for attribute [%d]",
				data.size(), attributeLocation);

		// create vertex buffer
		unsigned int buffer;
		glGenBuffers(1, &buffer); // Generate our Vertex Buffer Object
		glBindBuffer(bufferDestination, buffer); // bind to GL_ARAY_BUFFER, as we're about to call glBufferData(GL_ARRAY_BUFFER)
		glBufferData(bufferDestination, data.size() * sizeof(unsigned int),
				(unsigned int*) data.data(), GL_STATIC_DRAW);

		GLenum glError = glGetError();
		if (glError != GL_NO_ERROR) {
			logger->error("Error loading vertex buffer: 0x[%x]", glError);
			return false;
		}

		if (attributeLocation >= 0) {
			resource->addAttribute(attributeLocation, buffer, 0, data.size());
			glVertexAttribPointer((GLuint) attributeLocation, 1,
					GL_UNSIGNED_INT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(attributeLocation);
			logger->verbose("Enabled attribute [%d]", attributeLocation);
		}

		glError = glGetError();
		if (glError != GL_NO_ERROR) {
			logger->error("Error binding buffer [%d] to location [%d]: 0x[%x]",
					buffer, attributeLocation, glError);
			return false;
		}

		return true;
	}

public:
	VertexArrayResourceAdapter() {
		logger = Logger::getLogger("video/VertexArrayResourceAdapter");
		this->addSupportedMimeType("video/vertexArray");
	}

	GLenum asGlPrimitiveType(const String &typeString) {
		if (typeString == "points")
			return GL_POINTS;
		else if (typeString == "points")
			return GL_LINES;
		else if (typeString == "lineLoop")
			return GL_LINE_LOOP;
		else if (typeString == "lineStrip")
			return GL_LINE_STRIP;
		else if (typeString == "lines")
			return GL_LINES;
		else if (typeString == "triangles")
			return GL_TRIANGLES;
		else if (typeString == "triangleStrip")
			return GL_TRIANGLE_STRIP;
		else if (typeString == "triangleFan")
			return GL_TRIANGLE_FAN;
		else if (typeString == "quads")
			return GL_QUADS;
		else if (typeString == "triangleFan")
			return GL_TRIANGLE_FAN;
		else
			throw InvalidArgumentException("Invalid primitive type: [%s]",
					typeString.c_str());
	}
	virtual Resource* load(FileParser &fileParser, const String &mimeType) {
		GeometryResource *geometry =
				(GeometryResource*) this->getResourceManager()->load(fileParser,
						"video/geometry");

		if (geometry != null) {
			VertexArrayResource *resource = null;

			glGetError();

			//Create vertex Array
			unsigned int vertexArray;
			glGenVertexArrays(1, &vertexArray);

			resource = new VertexArrayResource(vertexArray);
			resource->setPrimitiveType(asGlPrimitiveType(geometry->getType()));

			if (!geometry->getTextureFile().empty()) {
				TextureResource *texture =
						(TextureResource*) this->getResourceManager()->load(
								geometry->getTextureFile(), "video/texture");
				if (texture != null) {
					resource->setTexture(texture);
				}
			}

			glBindVertexArray(resource->getId());

			GLenum glError = glGetError();
			if (glError != GL_NO_ERROR) {
				logger->error("Error creating vertex array [%s]: 0x[%x]",
						fileParser.getFilename().c_str(), glError);
				dispose(resource);
				return null;
			}

			if (geometry->getIndices().size() != 0
					&& !addBuffer(INDEX_LOCATION, resource,
							GL_ELEMENT_ARRAY_BUFFER, geometry->getIndices())) {
				dispose(resource);
				return null;
			}

			if (geometry->getVertices().size() != 0
					&& !addBuffer(VERTEX_LOCATION, resource, GL_ARRAY_BUFFER,
							geometry->getVertices())) {
				dispose(resource);
				return null;
			}

			if (geometry->getNormals().size() != 0
					&& !addBuffer(NORMAL_LOCATION, resource, GL_ARRAY_BUFFER,
							geometry->getNormals())) {
				dispose(resource);
				return null;
			}

			if (geometry->getTextureCoordinates().size() != 0
					&& !addBuffer(TEXTURE_COORDINATES_LOCATION, resource,
							GL_ARRAY_BUFFER,
							geometry->getTextureCoordinates())) {
				dispose(resource);
				return null;
			}

			if (geometry->getColors().size() != 0
					&& !addBuffer(COLOR_LOCATION, resource, GL_ARRAY_BUFFER,
							geometry->getColors())) {
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
		VertexArrayResource *vertexArrayResource =
				(VertexArrayResource*) resource;

		if (vertexArrayResource->getId() != 0) {
			glBindVertexArray(vertexArrayResource->getId());

			for (std::map<unsigned int, VertexAttribPointer*>::iterator currentBuffer =
					vertexArrayResource->getAttributes().begin();
					currentBuffer != vertexArrayResource->getAttributes().end();
					currentBuffer++) {
				if (currentBuffer->second != null) {
					glDisableVertexAttribArray(currentBuffer->first);
					unsigned int buffer = currentBuffer->second->getBuffer();
					glDeleteBuffers(1, &buffer);
					delete currentBuffer->second;
				}
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
