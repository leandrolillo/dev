/*
 * BufferResource.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#ifndef VERTEXBUFFERRESOURCE_H_
#define VERTEXBUFFERRESOURCE_H_

#include "resources/Resource.h"

class VertexBufferResource : public Resource
{
	public:
		VertexBufferResource(unsigned int id) : Resource(id, "video/vertexBuffer")
		{

		}
};



#endif /* BUFFERRESOURCE_H_ */
