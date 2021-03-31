/*
 * CubeMapResourceAdapter.h
 *
 *  Created on: Mar 31, 2021
 *      Author: leandro
 */

#ifndef SRC_OPENGL_ADAPTERS_CUBEMAPRESOURCEADAPTER_H_
#define SRC_OPENGL_ADAPTERS_CUBEMAPRESOURCEADAPTER_H_

#include<ResourceAdapter.h>
#include <JsonParser.h>
#include <CubeMapResource.h>
#include <OpenGL/gl3.h>


class CubeMapResourceAdapter : public ResourceAdapter {
public:
	CubeMapResourceAdapter() {
		logger = Logger::getLogger("video/CubeMapResourceAdapter");
		this->addSupportedMimeType("video/cubemap");
	}

	Resource *load(FileParser &fileParser, const String &mimeType) {
		JsonParser *jsonParser = new JsonParser(fileParser);

		unsigned int textureHandler = 0;
		glGetError();
		glGenTextures(1, &textureHandler);
		CubeMapResource *resource = new CubeMapResource(textureHandler);

		glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandler);


		jsonParser->readStartObject();

		String token;
		while ((token = jsonParser->readToken()) != END_OBJECT && token != eof) {
			String faceName = token;
			jsonParser->readValueSeparator();
			String faceFileName = jsonParser->readString();
			ImageResource *imageResource = (ImageResource *)this->getResourceManager()->load(faceFileName);

			if(imageResource != null) {
				if(imageResource->getBpp() == 32)
					glTexImage2D(getLocation(faceName), 0, GL_RGBA, imageResource->getAncho(), imageResource->getAlto(), 0, GL_BGRA, GL_UNSIGNED_BYTE, imageResource->getData());
				else
					glTexImage2D(getLocation(faceName), 0, GL_RGB, imageResource->getAncho(), imageResource->getAlto(), 0, GL_BGR, GL_UNSIGNED_BYTE, imageResource->getData());
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if (jsonParser->peekToken() == ",") {
				jsonParser->readToken();
			}
		}

		return resource;
	}

	virtual void dispose(Resource *resource) {
		CubeMapResource *textureResource = (CubeMapResource *)resource;
		unsigned int textureHandler = textureResource->getId();
		glDeleteTextures(1, &textureHandler);
	}
private:
	GLenum getLocation(const String &faceName) {
		if(faceName == "top") {
			return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		} else if(faceName == "bottom") {
			return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
		} else if(faceName == "right") {
			return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		} else if(faceName == "left") {
			return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
		} else if(faceName == "front") {
			return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
		} else if(faceName == "back") {
			return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
		} else {
			return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
		}
	}
};



#endif /* SRC_OPENGL_ADAPTERS_CUBEMAPRESOURCEADAPTER_H_ */
