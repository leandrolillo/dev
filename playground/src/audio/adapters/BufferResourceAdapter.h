/*
 * BufferResourceAdapter.h
 *
 *  Created on: 14/01/2013
 *      Author: Lean
 */

#ifndef BUFFERRESOURCEADAPTER_H_
#define BUFFERRESOURCEADAPTER_H_

#include "al.h"
#include "../resources/AudioResource.h"
#include "../resources/BufferResource.h"
#include <ResourceAdapter.h>
#include <set>

class BufferResourceAdapter: public ResourceAdapter {
public:
	BufferResourceAdapter() {
		logger = LoggerFactory::getLogger("audio/BufferResourceAdapter");
		this->produces(MimeTypes::AUDIOBUFFER);
	}

	ALenum asOpenALFormat(AudioFormat format) const {
		switch (format) {
		case MONO8:
			return AL_FORMAT_MONO8;
		case MONO16:
			return AL_FORMAT_MONO16;
		case STEREO8:
			return AL_FORMAT_STEREO8;
		case STEREO16:
			return AL_FORMAT_STEREO16;
		}

		return AL_FORMAT_STEREO16;
	}
	virtual Resource* load(ResourceLoadRequest &request) const override {
		ALenum error = 0;

		AudioResource *audioResource =
				(AudioResource*) this->getResourceManager()->load(request.getFileParser(),
						MimeTypes::AUDIO,
						std::set<String> {ResourceManager::ResourceManager::EphemeralLabel});
		if (audioResource == null) {
			logger->error(
					"Error loading bufferResource: could not load audio from [%s]",
					request.getFilePath().c_str());
			return (null);
		}

		ALuint ALbuffer = 0;

		alGenBuffers(1, &ALbuffer);
		if ((error = alGetError()) != AL_NO_ERROR) {
			logger->error("Error creating resource for [%s]: %d",
					request.getFilePath().c_str(), error);
			return null;
		}

		alBufferData(ALbuffer, asOpenALFormat(audioResource->getFormat()),
				audioResource->getData().data(),
				audioResource->getData().size(), audioResource->getFrequency());
		if (alGetError() != AL_NO_ERROR) {
			logger->error("Error setting buffer data for [%s]: %d",
					request.getFilePath().c_str(), error);
			alDeleteBuffers(1, &ALbuffer);
			return (null);
		}

		return new BufferResource(ALbuffer);

	}
	virtual void dispose(Resource *resource) const override {
		logger->debug("Deleting buffer [%ld]", resource->getId());
		unsigned int bufferId = resource->getId();

		alDeleteBuffers(1, &bufferId);
	}
};

#endif /* BUFFERRESOURCEADAPTER_H_ */
