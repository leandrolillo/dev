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
#include <gl/glext.h>

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
		virtual Resource *load(String filename) {
			ImageResource *imageResource = (ImageResource *)this->getResourceManager()->load(filename);

			if(imageResource != null) {
				unsigned int textureHandler = 0;
				glGenTextures(1, &textureHandler);
				glBindTexture(GL_TEXTURE_2D, textureHandler);
				if(imageResource->getBpp() == 32)
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageResource->getAncho(), imageResource->getAlto(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, imageResource->getData());
				else
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageResource->getAncho(), imageResource->getAlto(), 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, imageResource->getData());

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				if(glGetError() == null)
					return new TextureResource(textureHandler);
				else {
					logger->error("Error loading texture [%s]: [%s]", filename.c_str(), glGetError());
					glDeleteTextures(1, &textureHandler);
				}
			}

			return null;
		}
		virtual void dispose(Resource *resource) {
			TextureResource *textureResource = (TextureResource *)resource;
			unsigned int textureHandler = textureResource->getId();
			glDeleteTextures(1, &textureHandler);
		}
};
#endif /* TEXTURERESOURCEADAPTER_H_ */
