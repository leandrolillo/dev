/*
 * BufferResource.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#ifndef VERTEXBUFFERRESOURCE_H_
#define VERTEXBUFFERRESOURCE_H_

#include "resources/Resource.h"

class VertexAttribPointer {
	public:
		VertexAttribPointer(unsigned int buffer, unsigned int start, unsigned int count)
		{
			this->buffer = buffer;
			this->start = start;
			this->count = count;
		}
		unsigned int getCount() const {
			return count;
		}

		void setCount(unsigned int count) {
			this->count = count;
		}

		unsigned int getBuffer() const {
			return buffer;
		}

		void setBuffer(unsigned int pointer) {
			this->buffer = pointer;
		}

		unsigned int getStart() const {
			return start;
		}

		void setStart(unsigned int start) {
			this->start = start;
		}

	private:
		unsigned int buffer;
		unsigned int start;
		unsigned int count;
};

class VertexArrayResource : public Resource
{
	private:
		std::vector<VertexAttribPointer> vertexAttribPointers;
	public:
		VertexArrayResource(unsigned int id) : Resource(id, "video/vertexArray")
		{

		}

		/**
		 * stores vertex buffer in the next free vertexAttribPointer, and returns the pointer.
		 */
		unsigned int addBuffer(unsigned int vertexBuffer, unsigned int start, unsigned int count)
		{
			unsigned int vertexAttribPointer = vertexAttribPointers.size();
			vertexAttribPointers.push_back(VertexAttribPointer(vertexBuffer, start, count));

			return vertexAttribPointer;
		}

		const std::vector<VertexAttribPointer>& getVertexAttribPointers() const {
			return vertexAttribPointers;
		}

		void clearVertexAttribPointers()
		{
			vertexAttribPointers.clear();
		}
};



#endif /* BUFFERRESOURCE_H_ */
