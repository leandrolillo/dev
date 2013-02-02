/*
 * BufferResourceAdapter.h
 *
 *  Created on: 14/01/2013
 *      Author: Lean
 */

#ifndef BUFFERRESOURCEADAPTER_H_
#define BUFFERRESOURCEADAPTER_H_

#include "al/al.h"
#include "../resources/AudioResource.h"
#include "../resources/BufferResource.h"
#include "resources/ResourceAdapter.h"

class BufferResourceAdapter: public ResourceAdapter {
	private:
		std::vector<String> supportedMimeTypes;
		Logger *logger;
		public:
		BufferResourceAdapter() {
			supportedMimeTypes.push_back("audio/buffer");
			logger = Logger::getLogger("audio/BufferResourceAdapter.h");
		}
		virtual const std::vector<String> getSupportedMimeTypes() {
			return supportedMimeTypes;
		}
		virtual Resource *load(String filename) {
			ALenum error = 0;

			AudioResource * audioResource = (AudioResource *)this->getResourceManager()->load(filename);
			if(audioResource == null) {
				logger->error("Error loading bufferResource: could not load audio from [%s]", filename.c_str());
				return(null);
			}
			ALuint ALbuffer = 0;

			alGenBuffers(1, &ALbuffer);
			if((error = alGetError()) != AL_NO_ERROR) {
				logger->error("Error creating resource for [%s]: %d", filename.c_str(), error);
				return null;
			}

			std::vector<char> *data = (std::vector<char> *)audioResource->getData();

			logger->debug("About to read %d bytes", data->size());

			alBufferData(ALbuffer, audioResource->getFormat(), &(*data)[0], audioResource->getSize(),audioResource->getFrequency());
			if(alGetError() != AL_NO_ERROR) {
				logger->error("Error setting buffer data for [%s]: %d", filename.c_str(), error);
				alDeleteBuffers(1, &ALbuffer);
				return(null);
			}

			return new BufferResource(ALbuffer);

		}
		virtual void dispose(Resource *resource) {
			logger->debug("Deleting buffer [%ld]", resource->getId());
			unsigned int bufferId = resource->getId();

			alDeleteBuffers(1, &bufferId);
		}
	};

#endif /* BUFFERRESOURCEADAPTER_H_ */
