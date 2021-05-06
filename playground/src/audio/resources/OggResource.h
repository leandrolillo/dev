/*
 * OggResource.h
 *
 *  Created on: 15/01/2013
 *      Author: Lean
 */

#ifndef OGGRESOURCE_H_
#define OGGRESOURCE_H_

#include <resources/AudioResource.h>

class OggResource : public AudioResource
{
	public:
		OggResource(unsigned int id) : AudioResource(id, "audio/ogg") {
		}
};



#endif /* OGGRESOURCE_H_ */
