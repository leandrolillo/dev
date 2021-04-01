/*
 * TextureResourceAdapter.h
 *
 *  Created on: 28/01/2013
 *      Author: Lean
 */

#ifndef TEXTURERESOURCEADAPTER_H_
#define TEXTURERESOURCEADAPTER_H_

#include <adapters/OpenGLResourceAdapter.h>
#include <resources/ImageResource.h>
#include <resources/TextureResource.h>

class TextureResourceAdapter: public OpenGLResourceAdapter {
	public:
		TextureResourceAdapter() {
			logger = Logger::getLogger("video/TextureResourceAdapter");
			this->addSupportedMimeType("video/texture");
		}

		virtual Resource *load(FileParser &fileParser, const String &mimeType) {
			ImageResource *imageResource = (ImageResource *)this->getResourceManager()->load(fileParser);
			TextureResource *resource = null;

			if(imageResource != null) {
				unsigned int textureHandler = 0;
				glGetError();
				glGenTextures(1, &textureHandler);
				resource = new TextureResource(textureHandler);
				glBindTexture(GL_TEXTURE_2D, textureHandler);
				if(imageResource->getBpp() == 32)
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageResource->getAncho(), imageResource->getAlto(), 0, GL_BGRA, GL_UNSIGNED_BYTE, imageResource->getData());
				else
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageResource->getAncho(), imageResource->getAlto(), 0, GL_BGR, GL_UNSIGNED_BYTE, imageResource->getData());

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glBindTexture(GL_TEXTURE_2D, 0);

				String errorMessage;
                if (!(errorMessage = getGlError()).empty()) {
                    logger->error("Error loading texture [%s]: [%s]", fileParser.getFilename().c_str(), errorMessage.c_str());
                    dispose(resource);
                    return null;
                }
			}

			return resource;
		}
		virtual void dispose(Resource *resource) {
			TextureResource *textureResource = (TextureResource *)resource;
			unsigned int textureHandler = textureResource->getId();
			glDeleteTextures(1, &textureHandler);
		}
};
#endif /* TEXTURERESOURCEADAPTER_H_ */
