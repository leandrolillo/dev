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

#include "resources/ResourceAdapter.h"
#include "../resources/ShaderProgramResource.h"
#include <gl/gl.h>
#include <gl/glew.h>

class ShaderProgramResourceAdapter: public ResourceAdapter {
	private:
		std::vector<String> supportedMimeTypes;
		Logger *logger;
	private:
		String getInfoLog(GLuint object)
		{
			GLint log_length;
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
		virtual const std::vector<String> getSupportedMimeTypes() {
			return supportedMimeTypes;
		}
		#define BUFFER_SIZE 256
		virtual Resource *load(FileParser &fileParser, const String &mimeType) {
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

			glLinkProgram(resource->getId());
			glValidateProgram(resource->getId());

			int linkSuccessfull = 0;
			glGetProgramiv(resource->getId(), GL_LINK_STATUS, &linkSuccessfull);
			if (!linkSuccessfull) {
				logger->error("Failed to link program - [%d]: [%s]\n", linkSuccessfull, getInfoLog(resource->getId()).c_str());
				dispose(resource);
				return 0;
			}

			for(std::vector<ShaderResource *>::iterator shaderIterator = resource->getShaders().begin(); shaderIterator != resource->getShaders().end(); shaderIterator++)
				glDetachShader(resource->getId(), (*shaderIterator)->getId());

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
