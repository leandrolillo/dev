/*
 * BufferResource.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#ifndef VERTEXBUFFERRESOURCE_H_
#define VERTEXBUFFERRESOURCE_H_

#include "../resources/Resource.h"
#include "TextureResource.h"
#include <OpenGL/gl3.h>

enum ShaderAttributeLocation
{
	VERTEX_LOCATION = 0,
	INDEX_LOCATION,
	NORMAL_LOCATION,
	TEXTURE_COORDINATES_LOCATION,
	COLOR_LOCATION
};


class VertexAttribPointer {
	public:
		VertexAttribPointer(unsigned int buffer, unsigned int start, unsigned int count)
		{
			this->buffer = buffer;
			this->start = start;
			this->count = count;
		}
		VertexAttribPointer()
		{
			this->buffer = -1;
			this->start = 0;
			this->count = 0;
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

		String toString() const {
			return "[buffer: " + std::to_string(this->buffer) + " start: " + std::to_string(this->start) + " count: " + std::to_string(this->count);
		}

	private:
		unsigned int buffer;
		unsigned int start;
		unsigned int count;
};

class VertexArrayResource : public Resource
{
	private:
		std::map<unsigned int, VertexAttribPointer *> attributes;
		TextureResource *texture;
		unsigned int primitiveType;
	public:
		VertexArrayResource(unsigned int id) : Resource(id, "video/vertexArray")
		{
			texture = null;
			primitiveType = GL_TRIANGLES;
		}

		~VertexArrayResource()
		{
			for(auto attribute : this->attributes) {
					delete attribute.second;
			}
		}

		/**
		 * stores vertex buffer in the next free vertexAttribPointer, and returns the pointer.
		 */
		void addAttribute(unsigned int location, unsigned int vertexBuffer, unsigned int start, unsigned int count)
		{
			attributes[location] = new VertexAttribPointer(vertexBuffer, start, count);
		}


		VertexAttribPointer *getAttribute(unsigned int location) {
			return attributes[location];
		}

		std::map<unsigned int, VertexAttribPointer *> &getAttributes() {
			return attributes;
		}

		void clearVertexAttribPointers()
		{
			attributes.clear();
		}

		TextureResource* getTexture() const {
			return texture;
		}

		void setTexture(TextureResource *texture) {
			this->texture = texture;
		}

		unsigned int getPrimitiveType() const {
			return primitiveType;
		}

		void setPrimitiveType(unsigned int primitiveType) {
			this->primitiveType = primitiveType;
		}

		String toString() const {
			String result = Resource::toString();

//			for(auto attribute : this->attributes) {
//				result += "location = " + std::to_string(attribute.first) + ": " + attribute.second.toString().c_str() + "\n";
//			}

			return result;
		}
};



#endif /* BUFFERRESOURCE_H_ */
