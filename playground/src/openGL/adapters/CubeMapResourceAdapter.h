/*
 * CubeMapResourceAdapter.h
 *
 *  Created on: Mar 31, 2021
 *      Author: leandro
 */

#ifndef SRC_OPENGL_ADAPTERS_CUBEMAPRESOURCEADAPTER_H_
#define SRC_OPENGL_ADAPTERS_CUBEMAPRESOURCEADAPTER_H_

#include <adapters/TextureResourceAdapter.h>
#include <resources/CubeMapResource.h>
#include <JsonParser.h>


class CubeMapResourceAdapter : public TextureResourceAdapter {
public:
	CubeMapResourceAdapter() : TextureResourceAdapter() {
		logger = LoggerFactory::getLogger("video/CubeMapResourceAdapter");
		this->clearSupportedMimeTypes();
		this->addSupportedMimeType("video/cubemap");
	}

	Resource *load(FileParser &fileParser, const String &mimeType) {
		JsonParser *jsonParser = new JsonParser(fileParser);

		unsigned int textureHandler = 0;
		getGlError();
		glGenTextures(1, &textureHandler);

		CubeMapResource *resource = new CubeMapResource(textureHandler);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandler);


		jsonParser->readStartObject();

		String token;
		while ((token = jsonParser->readToken()) != END_OBJECT && token != FileParser::eof) {
			String faceName = token;
			jsonParser->readValueSeparator();
			String faceFileName = jsonParser->readString();
			ImageResource *imageResource = (ImageResource *)this->getResourceManager()->load(faceFileName);

			if(imageResource != null) {
				if(imageResource->getBpp() == 32) {
					glTexImage2D(getLocation(faceName), 0, GL_RGBA, imageResource->getAncho(), imageResource->getAlto(), 0, GL_BGRA, GL_UNSIGNED_BYTE, imageResource->getData());
				}
				else {
					glTexImage2D(getLocation(faceName), 0, GL_RGB, imageResource->getAncho(), imageResource->getAlto(), 0, GL_BGR, GL_UNSIGNED_BYTE, imageResource->getData());
				}

				this->getResourceManager()->dispose(imageResource);
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            String errorMessage;
            if (!(errorMessage = getGlError()).empty()) {
                logger->error("Error loading [%s] cubeMap: face [%s (%u)]: [%s]",
                        fileParser.getFilename().c_str(),
                        faceFileName.c_str(),
                        getLocation(faceName),
                        errorMessage.c_str());
                dispose(resource);
                return null;
            }

			if (jsonParser->peekToken() == ",") {
				jsonParser->readToken();
			}
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return resource;
	}
};



#endif /* SRC_OPENGL_ADAPTERS_CUBEMAPRESOURCEADAPTER_H_ */
