/*
 * WavResourceAdapter.h
 *
 *  Created on: 01/01/2013
 *      Author: Lean
 */

#ifndef WAVRESOURCEADAPTER_H_
#define WAVRESOURCEADAPTER_H_

#include "resources/ResourceAdapter.h"

class WavResourceAdapter: public ResourceAdapter {
	private:
		std::vector<String> supportedMimeTypes;
		Logger *logger;
		public:
		WavResourceAdapter() {
			supportedMimeTypes.push_back("audio/wav");

			logger = Logger::getLogger("audio/WavResourceAdapter.h");
		}
		virtual const std::vector<String> getSupportedMimeTypes() {
			return supportedMimeTypes;
		}

		#define WAV_BUFFER_SIZE 4096 * 4
		virtual Resource *load(FileParser &fileParser) {
			AudioResource *audioResource = new AudioResource(0, "audio/wav");

			struct wavHeader {
				DWORD rId;	       //tiene que decir RIFF
				DWORD rLen;       //longitud archivo

				DWORD waveId; 	//Dice WAVE

				DWORD formatId;	//Dice fmt --> format del chunk de datos
				DWORD formatLen;	//longitud de los datos en fmt
				WORD formatTag;	//Wave format. Tipo de encriptamiento.
				WORD numberOfChannels;		//Canales 1 = mono, 2 stereo
				DWORD numberOfSamplesPerSec;	//Playback Frecuency
				DWORD numberOfAvgBytesPerSec;	// =nChannels * nSamplesPerSec * (nBitsPerSample/8)
				WORD numberBlockAlign;	//nBlockAlign = nChannels * (nBitsPerSample / 8)
				WORD numberBytesPerSample;	//Format specific data area

				DWORD dataId;	//Dice data
				DWORD dataLen;	//longitud de los datos;
			} header;

			if(fileParser.read(&header, sizeof(header), 1) != 1) {
				logger->error("[%s] has an incomplete WAV header", fileParser.getFilename().c_str());
				return null;
			}

			char array[WAV_BUFFER_SIZE];
			std::vector <char> *bufferData = new std::vector<char>();

			long bytesRead;

			while((bytesRead = fileParser.read(array, sizeof(char), WAV_BUFFER_SIZE)) > 0 && bufferData->size() < header.dataLen)
					bufferData->insert(bufferData->end(), array, array+bytesRead);

			if(bufferData->size() != header.dataLen)
			{
				logger->error("[%s] Incomplete buffer data", fileParser.getFilename().c_str());
				bufferData->clear();
				delete bufferData;
				return null;
			}

			audioResource->setFormat(header.numberOfChannels == 1 ? header.numberBytesPerSample == 1 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16 : header.numberBytesPerSample == 1 ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16);
			audioResource->setFrequency(header.numberOfSamplesPerSec);
			audioResource->setSize(bufferData->size());
			audioResource->setData(bufferData);

			return audioResource;
		}
		virtual void dispose(Resource *resource) {
			AudioResource *audioResource = (AudioResource *)resource;

			if(audioResource->getData() != null)
			{
				std::vector<char> *bufferData = (std::vector<char> *)audioResource->getData();
				bufferData->clear();
				delete bufferData;
				audioResource->setData(null);
			}
		}
	};

#endif /* WAVRESOURCEADAPTER_H_ */
