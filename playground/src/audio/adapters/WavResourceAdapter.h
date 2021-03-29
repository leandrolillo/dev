/*
 * WavResourceAdapter.h
 *
 *  Created on: 01/01/2013
 *      Author: Lean
 */

#ifndef WAVRESOURCEADAPTER_H_
#define WAVRESOURCEADAPTER_H_

#include <ResourceAdapter.h>

class WavResourceAdapter: public ResourceAdapter {
public:
	WavResourceAdapter() {
		logger = Logger::getLogger("audio/WavResourceAdapter");
		this->addSupportedMimeType("audio/wav");
	}

	uint32_t asDword(char *string) {
		uint32_t result = 0;

		char *current = string;
		while (*current != '\0' && current < string + 4) {
			result = (result << 8) + *current;
			current++;
		}

		return result;
	}

	String asString(uint32_t dword) {
		String result;
		while (dword > 0) {
			result += (char) (dword - ((dword >> 8) << 8));
			dword = dword >> 8;
		}

		return result;
	}

#define WAV_BUFFER_SIZE 4096 * 4
	virtual Resource* load(FileParser &fileParser, const String &mimeType) {
		AudioResource *audioResource = null;

		struct ChunkHeader {
			uint32_t id;
			uint32_t length;
		} chunkHeader;

		struct RiffChunk {
			uint32_t waveId; 	//Dice WAVE
		} *riffChunk = null;

		struct FormatChunk {
			uint16_t formatTag;	//Wave format. Tipo de encriptamiento.
			uint16_t numberOfChannels;		//Canales 1 = mono, 2 stereo
			uint32_t numberOfSamplesPerSec;	//Playback Frecuency
			uint32_t numberOfAvgBytesPerSec;// =nChannels * nSamplesPerSec * (nBitsPerSample/8)
			uint16_t numberBlockAlign;//nBlockAlign = nChannels * (nBitsPerSample / 8)
			uint16_t numberBytesPerSample;	//Format specific data area
		} *formatChunk = null;

		struct DataChunk {
			std::vector<char> data;
		} *dataChunk = null;

		while (fileParser.read(&chunkHeader, sizeof(ChunkHeader), 1) == 1) {
			if (asString(chunkHeader.id) == "RIFF") {
				logger->verbose("Reading riff chunk");
				riffChunk = new RiffChunk;
				if (fileParser.read(riffChunk, sizeof(RiffChunk), 1) != 1) { // riff chunk length is the file size, so read based on struct size instead.
					logger->error("Could not read RIFF chunk");
					delete riffChunk;
					riffChunk = null;
				}
			} else if (asString(chunkHeader.id) == "fmt ") { //watch out this includes a space in the end
				logger->verbose("Reading format chunk");
				if (sizeof(FormatChunk) >= chunkHeader.length) {
					formatChunk = new FormatChunk;
					if (fileParser.read(formatChunk, sizeof(char),
							chunkHeader.length) != chunkHeader.length) {
						logger->error("Could not read fmt chunk");
						delete formatChunk;
						formatChunk = null;
					}
				} else {
					logger->error(
							"Unsupported format chunk size: expected %u, found %u",
							sizeof(FormatChunk), chunkHeader.length);
				}
			} else if (asString(chunkHeader.id) == "data") {
				logger->verbose("Reading data chunk");
				dataChunk = new DataChunk;
				/**
				 * This case is slightly different, instead of reading directly into a chunk struct, we read the raw data and store it in the
				 * chunk as a vector.
				 */
				char buffer[chunkHeader.length];

				if (fileParser.read(buffer, sizeof(char), chunkHeader.length)
						!= chunkHeader.length) {
					logger->error("Could not read data chunk");
					delete dataChunk;
					dataChunk = null;
				}

				dataChunk->data.insert(dataChunk->data.end(), buffer,
						buffer + chunkHeader.length);
			} else { // skip chunk
				logger->verbose("Skipping unsupported chunk: [%s] of size [%u]",
						asString(chunkHeader.id).c_str(), chunkHeader.length);
				fileParser.skip(sizeof(char), chunkHeader.length);
			}
		}

		if (riffChunk != null && asString(riffChunk->waveId) == "WAVE") {
			if (formatChunk != null) {
				if (dataChunk != null && !dataChunk->data.empty()) {
					logger->verbose(
							"Loading [%s] file [%s] [%u]bps [%u] bytes]",
							asString(riffChunk->waveId).c_str(),
							formatChunk->numberOfChannels == 1 ?
									"mono" : "stereo",
							formatChunk->numberOfSamplesPerSec
									* formatChunk->numberOfChannels,
							dataChunk->data.size());

					audioResource = new AudioResource(0, "audio/wav");
					audioResource->setFormat(
							formatChunk->numberOfChannels == 1 ?
									formatChunk->numberBytesPerSample == 1 ?
											MONO8 : MONO16
							: formatChunk->numberBytesPerSample == 1 ?
									STEREO8 : STEREO16);
					audioResource->setFrequency(
							formatChunk->numberOfSamplesPerSec);
					audioResource->setData(dataChunk->data);
				} else {
					logger->error("Missing or invalid data chunk");
				}
			} else {
				logger->error("Missing format chunk");
			}
		} else {
			logger->error("Missing or unsupported RIFF chunk: [%s]",
					riffChunk != null ?
							asString(riffChunk->waveId).c_str() : "N/A");
		}

		if (riffChunk != null) {
			delete riffChunk;
		}

		if (formatChunk != null) {
			delete formatChunk;
		}

		if (dataChunk != null) {
			delete dataChunk;
		}

		return audioResource;
	}
	virtual void dispose(Resource *resource) {
		AudioResource *audioResource = (AudioResource*) resource;

		if (!audioResource->getData().empty()) {
			audioResource->getData().clear();
		}
	}
};

#endif /* WAVRESOURCEADAPTER_H_ */