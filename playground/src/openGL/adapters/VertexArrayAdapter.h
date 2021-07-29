/*
 * VertexBufferAdapter.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#ifndef VERTEXBUFFERADAPTER_H_
#define VERTEXBUFFERADAPTER_H_
#include <adapters/OpenGLResourceAdapter.h>
#include <resources/VertexArrayResource.h>


class VertexArrayResourceAdapter: public OpenGLResourceAdapter {
public:
	VertexArrayResourceAdapter() {
		logger = LoggerFactory::getLogger("video/VertexArrayResourceAdapter");
		this->addSupportedMimeType("video/vertexArray");
	}

	virtual Resource* load(FileParser &fileParser, const String &mimeType) {
	    String geometryMimeType;

        geometryMimeType = this->getResourceManager()->guessMimeType(fileParser.getFilename());
        geometryMimeType = geometryMimeType.empty() ? "video/geometry" : geometryMimeType;

		GeometryResource *geometry = (GeometryResource*) this->getResourceManager()->load(fileParser, geometryMimeType);
		if (geometry != null) {
		    return generateVertexBuffer(geometry);
		} else {
		    logger->error("Could not load geometry from %s with mimetype %s", fileParser.getFilename().c_str(), geometryMimeType.c_str());
		}

		return null;
	}
	virtual void dispose(Resource *resource) {
	    logger->debug("Deleting [%s]", resource->toString().c_str());
		VertexArrayResource *vertexArrayResource = (VertexArrayResource*) resource;

		if (vertexArrayResource->getId() != 0) {
		    logger->debug("VAB [%s] bound", resource->getId());
			glBindVertexArray(vertexArrayResource->getId());

			for (std::map<unsigned int, const VertexAttribPointer*>::iterator currentBuffer =
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

			logger->debug("VAB [%s] buffers deleted", resource->getId());

			vertexArrayResource->clearVertexAttribPointers();

			unsigned int vertexArray = vertexArrayResource->getId();
			glDeleteVertexArrays(1, &vertexArray);

			logger->debug("VAB [%s] deleted", resource->getId());

			glBindVertexArray(0);
			vertexArrayResource->setId(0);
		}
	}

    VertexArrayResource *generateVertexBuffer(GeometryResource *geometry) {
        VertexArrayResource *resource = null;

        glGetError();

        //Create vertex Array
        unsigned int vertexArray;
        glGenVertexArrays(1, &vertexArray);

        resource = new VertexArrayResource(vertexArray);
        resource->setPrimitiveType(asGlPrimitiveType(geometry->getType()));

        glBindVertexArray(resource->getId());
        GLenum glError = glGetError();
        if (glError != GL_NO_ERROR) {
            logger->error("Error creating vertex array  0x[%x]", glError);
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

        /**
         * Indices need to be loaded after vertices
         */
        if (geometry->getIndices().size() != 0
                && !addBuffer(INDEX_LOCATION, resource,
                        GL_ELEMENT_ARRAY_BUFFER, geometry->getIndices())) {
            dispose(resource);
            return null;
        }


        glBindVertexArray(0);

        // remove objects from context.
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        return resource;
    }
protected:
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
		glBindBuffer(bufferDestination, buffer); // bind to GL_ELEMENT_ARRAY_BUFFER, as we're about to call glBufferData(GL_ELEMENT_ARRAY_BUFFER)
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
};
#endif /* VERTEXBUFFERADAPTER_H_ */
