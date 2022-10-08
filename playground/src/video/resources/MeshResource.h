/*
 * MeshResource.h
 *
 *  Created on: Oct 3, 2022
 *      Author: leandro
 */

#ifndef SRC_VIDEO_RESOURCES_MESHRESOURCE_H_
#define SRC_VIDEO_RESOURCES_MESHRESOURCE_H_

#include<Resource.h>
#include "VertexArrayResource.h"
#include "MaterialResource.h"
#include "TextureResource.h"

class MeshResource : public Resource
{
	VertexArrayResource *vertexArray;
	MaterialResource *material;
	TextureResource *texture;
	TextureResource *specularMap;
	TextureResource *normalMap;
public:
	const MaterialResource* getMaterial() const {
		return material;
	}

	void setMaterial(MaterialResource *material) {
		this->material = material;
	}

	TextureResource* getTexture() const {
		return texture;
	}

	void setTexture(TextureResource *texture) {
		this->texture = texture;
	}

	VertexArrayResource* getVertexArray() const {
		return vertexArray;
	}

	void setVertexArray(VertexArrayResource *vertexArray) {
		this->vertexArray = vertexArray;
	}
};


#endif /* SRC_VIDEO_RESOURCES_MESHRESOURCE_H_ */
