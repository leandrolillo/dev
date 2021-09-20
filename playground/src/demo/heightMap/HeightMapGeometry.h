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

class HeightMapGeometry : public Geometry {
    const HeightMapResource *heightMap;
public:
    HeightMapGeometry(const vector &position, const HeightMapResource *heightMap) : Geometry(position){
        this->heightMap = heightMap;
    }
};

//class HeightMapIntersectionTester : public IntersectionTester {
//public:
//    bool intersectsVisit(const AABB &anotherAabb, const Geometry &heightMapGeometry) const override {
//        return false;
//    }
//    bool intersectsVisit(const Plane &plane, const Geometry &heightMapGeometry) const override {
//        return false;
//    }
//    bool intersectsVisit(const Line &anotherLine, const Geometry &heightMapGeometry) const override {
//        return false;
//    }
//    bool intersectsVisit(const Sphere &sphere, const Geometry &heightMapGeometry) const override {
//        return false;
//    }
//
//    std::vector<GeometryContact>  detectCollisionVisit(const AABB &anotherAabb, const Geometry &heightMapGeometry) const override {
//        return std::vector<GeometryContact>();
//    }
//    std::vector<GeometryContact>  detectCollisionVisit(const Sphere &sphere, const Geometry &heightMapGeometry) const override {
//        return std::vector<GeometryContact>();
//    }
//    std::vector<GeometryContact>  detectCollisionVisit(const Plane &plane, const Geometry &heightMapGeometry) const override {
//        return std::vector<GeometryContact>();
//    }
//    std::vector<GeometryContact>  detectCollisionVisit(const Line &line, const Geometry &heightMapGeometry) const override {
//        return std::vector<GeometryContact>();
//    }
//};


#endif /* SRC_DEMO_HEIGHTMAP_HEIGHTMAPGEOMETRY_H_ */
