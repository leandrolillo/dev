/*
 * ShaderResourceAdapter.h
 *
 *  Created on: 23/02/2013
 *      Author: Lean
 */

#ifndef SHADERRESOURCEADAPTER_H_
#define SHADERRESOURCEADAPTER_H_
#include "resources/ResourceAdapter.h"
#include "../resources/ShaderResource.h"
#include <gl/gl.h>
#include <gl/glew.h>

class ShaderResourceAdapter: public ResourceAdapter {
	private:
		std::vector<String> supportedMimeTypes;
		Logger *logger;
	private:
		String getInfoLog(GLuint object)
		{
			GLint log_length;
			char *log;

			glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
			log = new char[log_length];
			glGetShaderInfoLog(object, log_length, null, log);
			String response(log);
			delete log;

			return response;
		}
	public:
		ShaderResourceAdapter() {
			supportedMimeTypes.push_back("video/vertexShader");
			supportedMimeTypes.push_back("video/fragmentShader");
			logger = Logger::getLogger("video/ShaderResourceAdapter.h");
		}
		virtual const std::vector<String> getSupportedMimeTypes() {
			return supportedMimeTypes;
		}
		#define BUFFER_SIZE 256
		virtual Resource *load(FileParser &fileParser) {
			ShaderResource *resource = null;

			int shaderSize = fileParser.size();
			char *shaderCode = new char[shaderSize + 1];

			if(fileParser.read(shaderCode, sizeof(char), shaderSize) != (unsigned int)shaderSize)
			{
				logger->error("Error reading shader source [%s]", fileParser.getFilename().c_str());
				return null;
			}
			shaderCode[fileParser.size()] = '\0';

			logger->debug("Shader code: \n%s", shaderCode);

			unsigned int shaderId = glCreateShader(GL_VERTEX_SHADER);
			resource = new ShaderResource(shaderId);

			glShaderSource(resource->getId(), 1, (const char**)&shaderCode, &shaderSize);

			glCompileShader(resource->getId());

			int glError = 0;
			glGetShaderiv(resource->getId(), GL_COMPILE_STATUS, &glError);
			if (!glError) {
				logger->error("Failed to compile [%s] - [%d]: [%s]\n", fileParser.getFilename().c_str(), glError, getInfoLog(shaderId).c_str());
				dispose(resource);
				return null;
			}

			return resource;
		}

		virtual void dispose(Resource *resource) {
			ShaderResource *shaderResource = (ShaderResource *)resource;
			if(shaderResource->getId() != 0) {
				glDeleteShader(shaderResource->getId());
				shaderResource->setId(0);
			}
		}
};
#endif /* SHADERRESOURCEADAPTER_H_ */
