/*
 * ShaderResourceAdapter.h
 *
 *  Created on: 23/02/2013
 *      Author: Lean
 */

#ifndef SHADERRESOURCEADAPTER_H_
#define SHADERRESOURCEADAPTER_H_
#include <adapters/OpenGLResourceAdapter.h>
#include <resources/ShaderResource.h>

class ShaderResourceAdapter: public OpenGLResourceAdapter {
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
			logger = LoggerFactory::getLogger("video/ShaderResourceAdapter");
			this->addSupportedMimeType("video/vertexShader");
			this->addSupportedMimeType("video/fragmentShader");
		}

		#define BUFFER_SIZE 256
		virtual Resource *load(FileParser &fileParser, const String &mimeType) {
			ShaderResource *resource = null;

			int shaderSize = fileParser.size();
			char *shaderCode = new char[shaderSize + 1];

			if(fileParser.read(shaderCode, sizeof(char), shaderSize) != (unsigned int)shaderSize)
			{
				logger->error("Error reading shader source [%s]", fileParser.getFilename().c_str());
				return null;
			}
			shaderCode[fileParser.size()] = '\0';

			unsigned int shaderType = (mimeType == "video/vertexShader" ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);

			unsigned int shaderId = glCreateShader(shaderType);
			resource = new ShaderResource(shaderId);

			glShaderSource(resource->getId(), 1, (const char**)&shaderCode, &shaderSize);

			glCompileShader(resource->getId());

			int compilationSuccessfull = 0;
			glGetShaderiv(resource->getId(), GL_COMPILE_STATUS, &compilationSuccessfull);
			if (!compilationSuccessfull) {
				logger->error("Failed to compile [%s] [%s]: [%s]\n", shaderType == GL_VERTEX_SHADER ? "Vertex Shader" : "Fragment Shader", fileParser.getFilename().c_str(), getInfoLog(shaderId).c_str());
				logger->debug("Shader code: \n%s", shaderCode);
				dispose(resource);
				return null;
			} else {
				logger->debug("[%s] compilation sucessful.", shaderType == GL_VERTEX_SHADER ? "Vertex Shader" : "Fragment Shader");
			}

			return resource;
		}

		virtual void dispose(Resource *resource) {
			ShaderResource *shaderResource = (ShaderResource *)resource;
			if(shaderResource != null && shaderResource->getId() != 0) {
				glDeleteShader(shaderResource->getId());
				shaderResource->setId(0);
			}
		}
};
#endif /* SHADERRESOURCEADAPTER_H_ */
