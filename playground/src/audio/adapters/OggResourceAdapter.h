/*
 * OggResourceAdapter.h
 *
 *  Created on: 15/01/2013
 *      Author: Lean
 */

#ifndef OGGRESOURCEADAPTER_H_
#define OGGRESOURCEADAPTER_H_

#include <ogg\ogg.h>
#include <vorbis\codec.h>
#include <vorbis\vorbisfile.h>
#include <stdlib.h>
#include <al/al.h>

#include "../resources/OggResource.h"
#include "resources/ResourceAdapter.h"

class OggResourceAdapter : public ResourceAdapter {
	private:
		std::vector<String> supportedMimeTypes;
		Logger *logger;
	public:
		OggResourceAdapter() {
			supportedMimeTypes.push_back("audio/ogg");
			logger = Logger::getLogger("audio/OggResourceAdapter.h");
		}
		virtual const std::vector<String> getSupportedMimeTypes() {
			return supportedMimeTypes;
		}

		#define OGG_BUFFER_SIZE 4096 * 4
		virtual Resource *load(String filename) {
			FILE * oggFile;

				OggVorbis_File oggStream;
				vorbis_info *vorbisInfo;
				vorbis_comment *vorbisComment;


				if((oggFile = fopen("d:/Lean/dev/media/audio/background.ogg", "rb")) == null) {
					logger->error("Error opening ogg file [%s]", filename.c_str());
					return(null);
				}
				if(ov_open(oggFile, &oggStream, NULL, 0)  < 0) {
					logger->error("Error opening ogg stream [%s]", filename.c_str());
					fclose(oggFile);
					return(null);
				}

				vorbisInfo = ov_info(&oggStream, -1);
				vorbisComment = ov_comment(&oggStream, -1);

				logger->debug("Vorbis file info:");
				for(int index = 0; index < vorbisComment->comments; index++)
					logger->debug("	%s", vorbisComment->user_comments[index]);

				OggResource * resource = new OggResource(0);

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
					logger->error("Error reading ogg file [%s] data", filename.c_str());
					fclose(oggFile);
					return(null);
				}

				fclose(oggFile);

				resource->setData(bufferData);
				resource->setSize(bufferData->size());
				return(resource);
		}
		virtual void dispose(Resource *resource) {
			OggResource *oggResource = (OggResource *)resource;

			((std::vector <char> *)oggResource->getData())->clear();

			delete ((std::vector <char> *)oggResource->getData());
		}
};



#endif /* OGGRESOURCEADAPTER_H_ */
