/*
 * TextureResourceAdapter.h
 *
 *  Created on: 28/01/2013
 *      Author: Lean
 */

#ifndef TEXTURERESOURCEADAPTER_H_
#define TEXTURERESOURCEADAPTER_H_

#include "resources/ResourceAdapter.h"
#include "../resources/ImageResource.h"
#include "../resources/TextureResource.h"
#include <gl/gl.h>

class TextureResourceAdapter: public ResourceAdapter {
	private:
		std::vector<String> supportedMimeTypes;
		Logger *logger;
	public:
		TextureResourceAdapter() {
			supportedMimeTypes.push_back("video/texture");
			logger = Logger::getLogger("video/TextureResourceAdapter.h");
		}
		virtual const std::vector<String> getSupportedMimeTypes() {
			return supportedMimeTypes;
		}
		virtual Resource *load(FileParser &fileParser) {
			ImageResource *imageResource = (ImageResource *)this->getResourceManager()->load(fileParser);
			TextureResource *resource = null;

			if(imageResource != null) {
				unsigned int textureHandler = 0;
				glGetError();
				glGenTextures(1, &textureHandler);
				resource = new TextureResource(textureHandler);
				glBindTexture(GL_TEXTURE_2D, textureHandler);
				if(imageResource->getBpp() == 32)
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageResource->getAncho(), imageResource->getAlto(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, imageResource->getData());
				else
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageResource->getAncho(), imageResource->getAlto(), 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, imageResource->getData());

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glBindTexture(GL_TEXTURE_2D, 0);

				GLenum glError = glGetError();
				if(glError != GL_NO_ERROR) {
					logger->error("Error loading texture [%s]: 0x[%x]", fileParser.getFilename().c_str(), glError);
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
