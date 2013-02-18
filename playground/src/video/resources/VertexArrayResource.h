/*
 * BufferResource.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#ifndef VERTEXBUFFERRESOURCE_H_
#define VERTEXBUFFERRESOURCE_H_

#include "resources/Resource.h"

class VertexArrayResource : public Resource
{
	private:
		std::vector<unsigned int> vertexAttribPointers;
	public:
		VertexArrayResource(unsigned int id) : Resource(id, "video/vertexArray")
		{

		}

		/**
		 * stores vertex buffer in the next free vertexAttribPointer, and returns the pointer.
		 */
		unsigned int addBuffer(unsigned int vertexBuffer)
		{
			unsigned int vertexAttribPointer = vertexAttribPointers.size();
			vertexAttribPointers.push_back(vertexBuffer);

			return vertexAttribPointer;
		}

		const std::vector<unsigned int>& getVertexAttribPointers() const {
			return vertexAttribPointers;
		}

		void clearVertexAttribPointers()
		{
			vertexAttribPointers.clear();
		}
};



#endif /* BUFFERRESOURCE_H_ */
