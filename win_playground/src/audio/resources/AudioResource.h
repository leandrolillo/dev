#ifndef AUDIORESOURCE_H_
#define AUDIORESOURCE_H_

#include"resources/Resource.h"
#include<al/al.h>

class AudioResource : public Resource
{
	private:
		ALenum format;
		ALsizei size;
		ALsizei frequency;
		void *data;

	public:
		AudioResource(unsigned int id, String mimeType) : Resource(id, mimeType) {
			data = null;
			frequency = 0;
			size = 0;
			format = null;
		}
		void* getData() const {
			return data;
		}

		void setData(ALvoid* data) {
			this->data = data;
		}

		ALenum getFormat() const {
			return format;
		}

		void setFormat(ALenum format) {
			this->format = format;
		}

		ALsizei getFrequency() const {
			return frequency;
		}

		void setFrequency(ALsizei freq) {
			this->frequency = freq;
		}

		ALsizei getSize() const {
			return size;
		}

		void setSize(ALsizei size) {
			this->size = size;
		}

};
#endif
