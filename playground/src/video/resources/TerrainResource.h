/*
 * TerrainResource.h
 *
 *  Created on: Mar 26, 2021
 *      Author: leandro
 */

#ifndef SRC_VIDEO_RESOURCES_TERRAINRESOURCE_H_
#define SRC_VIDEO_RESOURCES_TERRAINRESOURCE_H_

#include <Resource.h>
#include <resources/TextureResource.h>
#include <resources/VertexArrayResource.h>

class TerrainResource: public Resource {
	TextureResource *r = null;
	TextureResource *g = null;
	TextureResource *b = null;
	TextureResource *a = null;

	TextureResource *map = null;

	VertexArrayResource *model = null;

public:
	TerrainResource() :
			Resource(0, "video/terrain") {

	}

	const TextureResource* getA() const {
		return a;
	}

	void setA(TextureResource *a) {
		this->a = a;
	}

	const TextureResource* getB() const {
		return b;
	}

	void setB(TextureResource *b) {
		this->b = b;
	}

	const TextureResource* getG() const {
		return g;
	}

	void setG(TextureResource *g) {
		this->g = g;
	}

	const TextureResource* getMap() const {
		return map;
	}

	void setMap(TextureResource *map) {
		this->map = map;
	}

	const TextureResource* getR() const {
		return r;
	}

	void setR(TextureResource *r) {
		this->r = r;
	}

	const VertexArrayResource* getModel() const {
		return model;
	}

	void setModel(VertexArrayResource *model) {
		this->model = model;
	}
};

#endif /* SRC_VIDEO_RESOURCES_TERRAINRESOURCE_H_ */