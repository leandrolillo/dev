/*
 * HeightMapGeometry.h
 *
 *  Created on: Jul 11, 2021
 *      Author: leandro
 */

#ifndef SRC_DEMO_HEIGHTMAP_HEIGHTMAPGEOMETRY_H_
#define SRC_DEMO_HEIGHTMAP_HEIGHTMAPGEOMETRY_H_

#include<Geometry.h>
#include<HeightMapResource.h>
#include "../../geometry/intersection/CollisionTester.h"

class HeightMapGeometry : public AABB {
    const HeightMapResource *heightMap;
public:
    HeightMapGeometry(const vector &position, const HeightMapResource *heightMap) : AABB(position + vector(heightMap->getWidth() * 0.5, heightMap->getHeight() * 0.5, heightMap->getDepth() * 0.5), vector(heightMap->getWidth() * 0.5, heightMap->getHeight() * 0.5, heightMap->getDepth() * 0.5)){
        this->heightMap = heightMap;
    }

    const HeightMapResource *getHeightMap() const {
    	return this->heightMap;
    }

    GeometryType getType() const override {
        return GeometryType::HEIGHTMAP;
    }
};

#endif /* SRC_DEMO_HEIGHTMAP_HEIGHTMAPGEOMETRY_H_ */
