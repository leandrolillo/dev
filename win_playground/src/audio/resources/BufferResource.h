/*
 * BufferResource.h
 *
 *  Created on: 14/01/2013
 *      Author: Lean
 */

#ifndef BUFFERRESOURCE_H_
#define BUFFERRESOURCE_H_

#include "resources/Resource.h"

class BufferResource : public Resource
{
	public:
		BufferResource(unsigned int id) : Resource(id, "audio/buffer") {
		}
};



#endif /* BUFFERRESOURCE_H_ */
