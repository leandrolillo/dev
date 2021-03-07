/*
 * ShaderProgramResourceAdapter.h
 *
 *  Created on: 23/02/2013
 *      Author: Lean
 */

#ifndef SHADERPROGRAMRESOURCEADAPTER_H_
#define SHADERPROGRAMRESOURCEADAPTER_H_


/*
 * ShaderResourceAdapter.h
 *
 *  Created on: 23/02/2013
 *      Author: Lean
 */

#include "../../resources/ResourceAdapter.h"
#include "../resources/ShaderProgramResource.h"
#include "../../parser/JsonParser.h"
#include "VideoAdapter.h"
#include <OpenGL/gl.h>

class ShaderProgramResourceAdapter: public ResourceAdapter {
	private:
		std::vector<String> supportedMimeTypes;
		Logger *logger;
	private:
		String getInfoLog(GLuint object)
		{
			GLint log_length = 0;
			char *log;

			glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
			log = new char[log_length];
			glGetProgramInfoLog(object, log_length, null, log);
			String response(log);
			delete log;

			return response;
		}
	public:

		ShaderProgramResourceAdapter() {
			supportedMimeTypes.push_back("video/shaderProgram");
			logger = Logger::getLogger("video/ShaderProgramResourceAdapter.h");
		}
		String toString() const {
			return logger->getBasename();
		}

		virtual const std::vector<String> getSupportedMimeTypes() {
			return supportedMimeTypes;
		}
		#define BUFFER_SIZE 256
		virtual Resource *load(FileParser &fileParser, const String &mimeType) {
			GLenum glError = glGetError();

			JsonParser *parser = new JsonParser(fileParser);

			ShaderProgramResource *resource = null;

			unsigned int programId = glCreateProgram();
			resource = new ShaderProgramResource(programId);

			String token;
			parser->readStartObject();
			while((token = parser->readToken()) != END_OBJECT && token != eof)
			{
				if(token == "vertexShaders") {
					parser->readValueSeparator();
					std::vector<String> vertexShadersFiles = parser->readStringArray();

					for(std::vector<String>::iterator stringIterator = vertexShadersFiles.begin(); stringIterator != vertexShadersFiles.end(); stringIterator++) {
						ShaderResource *shader = (ShaderResource *)getResourceManager()->load(*stringIterator, "video/vertexShader");
						if(shader != null)
							resource->getShaders().push_back(shader);
						else
							logger->error("Could not load shader [%s]", (*stringIterator).c_str());
					}

				} else if (token == "fragmentShaders") {
					parser->readValueSeparator();
					std::vector<String> vertexShadersFiles = parser->readStringArray();

					for(std::vector<String>::iterator stringIterator = vertexShadersFiles.begin(); stringIterator != vertexShadersFiles.end(); stringIterator++) {
						ShaderResource *shader = (ShaderResource *)getResourceManager()->load(*stringIterator, "video/fragmentShader");
						if(shader != null)
							resource->getShaders().push_back(shader);
						else
							logger->error("Could not load shader [%s]", (*stringIterator).c_str());
					}
				}
			}

			for(std::vector<ShaderResource *>::iterator shaderIterator = resource->getShaders().begin(); shaderIterator != resource->getShaders().end(); shaderIterator++)
				glAttachShader(resource->getId(), (*shaderIterator)->getId());

			glBindAttribLocation(resource->getId(), VERTEX_LOCATION, "vertex");
			glBindAttribLocation(resource->getId(), INDEX_LOCATION, "index");
			glBindAttribLocation(resource->getId(), NORMAL_LOCATION, "normal");
			glBindAttribLocation(resource->getId(), TEXTURE_COORDINATES_LOCATION, "textureCoordinate");
			glBindAttribLocation(resource->getId(), COLOR_LOCATION, "color");

			glLinkProgram(resource->getId());

			int operationSuccessful = 0;
			glGetProgramiv(resource->getId(), GL_LINK_STATUS, &operationSuccessful);
			if (!operationSuccessful) {
				logger->error("Failed to link program - [%d]: [%s]\n", operationSuccessful, getInfoLog(resource->getId()).c_str());
				dispose(resource);
				return null;
			}

			glValidateProgram(resource->getId());
			glGetProgramiv(resource->getId(), GL_VALIDATE_STATUS, &operationSuccessful);
			if (!operationSuccessful) {
				logger->error("Failed to validate program - [%d]: [%s]\n", operationSuccessful, getInfoLog(resource->getId()).c_str());
				dispose(resource);
				return null;
			}

			logger->debug("Shader executable [%s] successfully linked and validated", fileParser.getFilename().c_str());

			for(std::vector<ShaderResource *>::iterator shaderIterator = resource->getShaders().begin(); shaderIterator != resource->getShaders().end(); shaderIterator++)
				glDetachShader(resource->getId(), (*shaderIterator)->getId());


			glError = glGetError();
			if(glError != GL_NO_ERROR) {
				logger->error("Error loading texture [%s]: 0x[%x]", fileParser.getFilename().c_str(), glError);
				dispose(resource);
				return null;
			}

			glUseProgram(resource->getId());
			logger->debug("vertex attrib location = [%d]", glGetAttribLocation(resource->getId(), "vertex"));
			logger->debug("index location = [%d]", glGetAttribLocation(resource->getId(), "index"));
			logger->debug("normal attrib location = [%d]", glGetAttribLocation(resource->getId(), "normal"));
			logger->debug("textureCoordinate attrib location = [%d]", glGetAttribLocation(resource->getId(), "textureCoordinate"));
			logger->debug("color attrib location = [%d]", glGetAttribLocation(resource->getId(), "color"));

			return resource;
		}

		virtual void dispose(Resource *resource) {
			ShaderProgramResource *shaderProgramResource = (ShaderProgramResource *)resource;
			if(shaderProgramResource->getId() != 0) {
				glDeleteProgram(shaderProgramResource->getId());
				shaderProgramResource->setId(0);
			}
		}
};

#endif /* SHADERPROGRAMRESOURCEADAPTER_H_ */
