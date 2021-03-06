/*
 * audioRunner.h
 *
 *  Created on: 30/12/2012
 *      Author: Lean
 */

#ifndef AUDIORUNNER_H_
#define AUDIORUNNER_H_

#include "core/playground.h"
#include "adapters/SourceResourceAdapter.h"
#include "adapters/BufferResourceAdapter.h"
#include "adapters/OggResourceAdapter.h"
#include "adapters/WavResourceAdapter.h"
#include "math/Math3d.h"

#include <al/al.h>
#include <al/alc.h>

class AudioRunner: public PlaygroundRunner {
	private:
		Logger *logger;
		ALCdevice *device;
		ALCcontext *context;
	public:
		static const unsigned char ID = 3;
	public:
		AudioRunner()
		{
			device = null;
			context = null;
			logger = Logger::getLogger("audio/AudioRunner.h");
		}
		virtual unsigned char getId() {
			return ID;
		}

		virtual boolean init() {
			this->getContainer()->getResourceManager()->addAdapter(new SourceResourceAdapter);
			this->getContainer()->getResourceManager()->addAdapter(new BufferResourceAdapter);
			this->getContainer()->getResourceManager()->addAdapter(new OggResourceAdapter);
			this->getContainer()->getResourceManager()->addAdapter(new WavResourceAdapter);

			device = alcOpenDevice(null);
			if(device == null) {
				logger->error("Error opening alcDevice");
				return false;
			}

			logger->debug("OpenAL device opened");

			context = alcCreateContext(device, null);
			if(context == null)
			{
				logger->error("Error creating context");
				return false;
			}

			logger->debug("OpenAL context created");

			alcMakeContextCurrent(context);

			logger->debug("OpenAL context set");
			alGetError(); // clear error code

			updateListener(vector(0, 0, 0), vector(0, 0, 0));

			return true;
		}

		String normalize(const String &fileName)
		{
			if(fileName.substr(0, 1) == "/")
			return fileName;

			return "audio/" + fileName;
		}

		Source *createSource(String fileName, const vector &position = vector(0, 0, 0), const vector &velocity = vector(0, 0, 0), boolean loop = true)
		{
			Source *source = (Source *)this->getContainer()->getResourceManager()->load(normalize(fileName), "audio/source");
			if(source != null)
			{
				source->setPosition(position);
				source->setVelocity(velocity);
				source->setLoop(loop);
			}

			return this->updateSource(source);
		}
		Source * updateSource(Source *source)
		{
			if(source != null) {
				vector position = source->getPosition();

				alSourcefv(source->getId(), AL_POSITION, (float *)position);
				alSourcefv(source->getId(), AL_VELOCITY, (float *)source->getVelocity());
				alSourcef (source->getId(), AL_PITCH, source->getPitch());
				alSourcef (source->getId(), AL_GAIN, source->getGain());
				alSourcef (source->getId(), AL_ROLLOFF_FACTOR, source->getRolloff());
				alSourcei (source->getId(), AL_LOOPING, source->getLoop());
			}
			return source;
		}

		Source * playSource(Source *source)
		{
			if(source != null)
			alSourcePlay(source->getId());
			return source;
		}

		Source *stopSource(Source *source)
		{
			if(source != null)
			alSourceStop(source->getId());
			return source;
		}

		Source *pauseSource(Source *source)
		{
			if(source != null)
			alSourcePause(source->getId());
			return source;
		}

		boolean updateListener(vector position, vector velocity, vector to = vector(0, 0, 1), vector up = vector(0, 1, 0))
		{
			float orientation[] = {to.x, to.y, to.z, up.x, up.y, up.z};
			alListenerfv(AL_POSITION, position);
			alListenerfv(AL_VELOCITY, velocity);
			alListenerfv(AL_ORIENTATION, orientation);

			if(alGetError() != AL_NO_ERROR) {
				logger->error("Error updating Listener");
				return(false);
			}
			return(true);

		}

		virtual ~AudioRunner()
		{
			alcMakeContextCurrent(NULL);
			if(context != null) {
				alcDestroyContext(context);
				logger->debug("OpenAL context destroyed");
			}

			if(device != null) {
				alcCloseDevice(device);
				logger->debug("OpenAL device destroyed");
			}
		}

	};

#endif /* AUDIORUNNER_H_ */
