/*
 * HeightMapGeometry.h
 *
 *  Created on: Jul 11, 2021
 *      Author: leandro
 */

#ifndef SRC_DEMO_HEIGHTMAP_HEIGHTMAPGEOMETRY_H_
#define SRC_DEMO_HEIGHTMAP_HEIGHTMAPGEOMETRY_H_

#include<Geometry.h>
#include<IntersectionTester.h>
#include<HeightMapResource.h>

class HeightMapGeometry : public Geometry {
    const HeightMapResource *heightMap;
public:
    HeightMapGeometry(const vector &position, const HeightMapResource *heightMap) : Geometry(position){
        this->heightMap = heightMap;
    }
    bool intersects(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const override {
        return op2.intersects(intersectionTester, *this);
    }
    std::vector<GeometryContact> detectCollision(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const override {
        return op2.detectCollision(intersectionTester, *this);
    }
};

class HeightMapIntersectionTester : public IntersectionTester {
public:
    bool intersectsVisit(const AABB &anotherAabb, const Geometry &heightMapGeometry) const override {
        return false;
    }
    bool intersectsVisit(const Plane &plane, const Geometry &heightMapGeometry) const override {
        return false;
    }
    bool intersectsVisit(const Line &anotherLine, const Geometry &heightMapGeometry) const override {
        return false;
    }
    bool intersectsVisit(const Sphere &sphere, const Geometry &heightMapGeometry) const override {
        return false;
    }

    std::vector<GeometryContact>  detectCollisionVisit(const AABB &anotherAabb, const Geometry &heightMapGeometry) const override {
        return std::vector<GeometryContact>();
    }
    std::vector<GeometryContact>  detectCollisionVisit(const Sphere &sphere, const Geometry &heightMapGeometry) const override {
        return std::vector<GeometryContact>();
    }
    std::vector<GeometryContact>  detectCollisionVisit(const Plane &plane, const Geometry &heightMapGeometry) const override {
        return std::vector<GeometryContact>();
    }
    std::vector<GeometryContact>  detectCollisionVisit(const Line &line, const Geometry &heightMapGeometry) const override {
        return std::vector<GeometryContact>();
    }
};


#endif /* SRC_DEMO_HEIGHTMAP_HEIGHTMAPGEOMETRY_H_ */
