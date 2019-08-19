/*
 * AudioResourceAdapter.h
 *
 *  Created on: 01/01/2013
 *      Author: Lean
 */

#ifndef AUDIORESOURCEADAPTER_H_
#define AUDIORESOURCEADAPTER_H_

#include "al/al.h"
#include "../resources/SourceResource.h"
#include "../resources/BufferResource.h"
#include "resources/ResourceAdapter.h"

class SourceResourceAdapter: public ResourceAdapter {
	private:
		std::vector<String> supportedMimeTypes;
		Logger *logger;
	public:
		SourceResourceAdapter() {
			supportedMimeTypes.push_back("audio/source");
			logger = Logger::getLogger("audio/SourceResourceAdapter.h");
		}
		virtual const std::vector<String> getSupportedMimeTypes() {
			return supportedMimeTypes;
		}
		virtual Resource *load(FileParser &fileParser, const String &mimeType) {
			ALenum error = 0;

			logger->debug("loading audio/source from [%s]", fileParser.getFilename().c_str());

			BufferResource * buffer = (BufferResource *)getResourceManager()->load(fileParser.getFilename().c_str(), "audio/buffer");
			if(buffer == null) {
				logger->error("Error creating source: could not load buffer for [%s]", fileParser.getFilename().c_str());
				return null;
			}

			ALuint sourceId;
			alGenSources(1, &sourceId);
			if((error = alGetError()) != AL_NO_ERROR) {
				logger->error("Error creating source for [%s]: %d", fileParser.getFilename().c_str(), error);
				return null;
			}

			alSourcei (sourceId, AL_BUFFER, (ALuint) buffer->getId());
			if((error = alGetError()) != AL_NO_ERROR) {
				logger->error("Error setting properties for source [%s]: %d", fileParser.getFilename().c_str(), error);
				return(null);
			}

			return(new Source(sourceId));
		}
		virtual void dispose(Resource *resource) {
			logger->debug("Deleting source [%ld]", resource->getId());

			unsigned int sourceId = resource->getId();

			alDeleteSources(1, &sourceId);
		}
};
#endif /* AUDIORESOURCEADAPTER_H_ */
