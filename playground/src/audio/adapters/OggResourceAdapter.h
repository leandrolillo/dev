/*
 * OggResourceAdapter.h
 *
 *  Created on: 15/01/2013
 *      Author: Lean
 */

#ifdef OGGRESOURCEADAPTER_H_ //disabled momentarily. Please try again later.
#define OGGRESOURCEADAPTER_H_

#include <ogg\ogg.h>
#include <vorbis\codec.h>
#include <vorbis\vorbisfile.h>
#include <stdlib.h>
#include "al.h"

#include "../resources/Resource.h"
#include <ResourceAdapter.h>

class OggResourceAdapter : public ResourceAdapter {
	private:
		std::vector<String> supportedMimeTypes;
		Logger *logger;
	public:
		OggResourceAdapter() {
			supportedMimeTypes.push_back("audio/ogg");
			logger = LoggerFactory::getLogger("audio/OggResourceAdapter.h");
		}
		virtual const std::vector<String> getSupportedMimeTypes() {
			return supportedMimeTypes;
		}
		String toString() const {
			return logger->getBasename();
		}
		constexpr unsigned int OGG_BUFFER_SIZE = 4096 * 4;
		virtual Resource *load(FileParser &fileParser, const String &mimeType) const override {
			OggVorbis_File oggStream;
			vorbis_info *vorbisInfo;
			vorbis_comment *vorbisComment;

			if(ov_open(fileParser.getStream(), &oggStream, NULL, 0)  < 0) {
				logger->error("Error opening ogg stream [%s]", fileParser.getFilename().c_str());
				return(null);
			}

			vorbisInfo = ov_info(&oggStream, -1);
			vorbisComment = ov_comment(&oggStream, -1);

			logger->debug("Vorbis file info:");
			for(int index = 0; index < vorbisComment->comments; index++)
				logger->debug("	%s", vorbisComment->user_comments[index]);

			AudioResource * resource = new AudioResource(0);

			resource->setFormat(vorbisInfo->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16);
			resource->setFrequency(vorbisInfo->rate);

			char array[OGG_BUFFER_SIZE];
			int endian = 0;                         // 0 for Little-Endian, 1 for Big-Endian
			int word_size = 2;
			int isSigned = 1;
			int bitStream = 1;
			std::vector <char> *bufferData = new std::vector<char>();
			long bytes;

			while ( (bytes = ov_read(&oggStream, array, OGG_BUFFER_SIZE, endian, word_size, isSigned, &bitStream)) > 0) {
				bufferData->insert(bufferData->end(), array, array + bytes);
			}

			logger->debug("Read %d bytes", bufferData->size());

			ov_clear(&oggStream);

			if(bytes < 0) {
				logger->error("Error reading ogg file [%s] data", fileParser.getFilename().c_str());
				return(null);
			}

			resource->setData(bufferData);
			resource->setSize(bufferData->size());
			return(resource);
		}
};



#endif /* OGGRESOURCEADAPTER_H_ */

