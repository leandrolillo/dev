/*
 * Geometry.h
 *
 *  Created on: May 9, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_GEOMETRY_GEOMETRY_H_
#define SRC_PHYSICS_GEOMETRY_GEOMETRY_H_

#include<vector>
#include<Math3d.h>
#include<GeometryContact.h>


enum class GeometryType {
    SPHERE,
    PLANE,
    LINE,
    AABB,
    HIERARCHY,
    HEIGHTMAP
};


class Geometry {
    vector origin;
public:
    Geometry(const vector &origin) {
        this->origin = origin;
    }

    virtual ~Geometry() {}

    const vector& getOrigin() const {
        return this->origin;
    }

    void setOrigin(const vector &origin) {
        this->origin = origin;
    }

    virtual String toString() const {
        return "Geometry(origin: " + origin.toString() + ")";
    }

    virtual GeometryType getType() const = 0;
};

class Sphere: public Geometry {
    real radius;
public:
    Sphere(const vector &origin, real radius) : Geometry(origin) {
        this->radius = radius;
    }

    real getRadius() const {
        return this->radius;
    }

    void setRadius(real radius) {
        this->radius = radius;
    }

    String toString() const override {
        return "Sphere(origin: " + this->getOrigin().toString() + ", radius: " + std::to_string(this->radius) + ")";
    }

    GeometryType getType() const override {
        return GeometryType::SPHERE;
    }
};

class Plane: public Geometry {
    vector normal;
public:
    Plane(const vector &origin, const vector &normal) : Geometry(origin) {
        this->normal = normal.normalizado();
    }

    const vector &getNormal() const {
        return this->normal;
    }

    String toString() const override {
        return "Plane(origin: " + this->getOrigin().toString() + ", normal: " + this->normal.toString() + ")";
    }

    GeometryType getType() const override {
        return GeometryType::PLANE;
    }
};

class Line: public Geometry {
    vector direction;
public:
    Line(const vector &origin, const vector &direction) : Geometry(origin){
        this->direction = direction.normalizado();
    }

    const vector& getDirection() const {
        return this->direction;
    }

    String toString() const override {
        return "Line(origin: " + this->getOrigin().toString() + ", dir: " + this->direction.toString() + ")";
    }

    GeometryType getType() const override {
        return GeometryType::LINE;
    }
};

class AABB : public Geometry {
    vector halfSizes;
public:
    AABB(const vector &origin, const vector &halfSizes) : Geometry(origin) {
        this->halfSizes = halfSizes;
    }

    const vector &getHalfSizes() const {
        return this->halfSizes;
    }

    void setHalfSizes(const vector &halfSizes) {
        this->halfSizes = halfSizes;
    }

    String toString() const override {
        return "AABB(origin: " + this->getOrigin().toString() + ", halfSizes: " + this->halfSizes.toString() + ")";
    }

    GeometryType getType() const override {
        return GeometryType::AABB;
    }
};

class HierarchicalGeometry : public Geometry {
    std::unique_ptr<Geometry> boundingVolume;
    std::vector<std::unique_ptr<Geometry>> children;
public:
    HierarchicalGeometry(Geometry *boundingVolume) : Geometry(boundingVolume->getOrigin()) {
        this->boundingVolume = std::unique_ptr<Geometry>(boundingVolume);
    }
    const vector& getOrigin() const {
        return this->boundingVolume->getOrigin();
    }

    void setOrigin(const vector &origin) {
        this->boundingVolume->setOrigin(origin);
    }

    void addChildren(Geometry *children) {

    }

    String toString() const override {
        return "HierarchicalGeometry(origin: " + this->getOrigin().toString() + ", children: " + std::to_string(this->children.size()) + ")";
    }

    GeometryType getType() const override {
        return GeometryType::HIERARCHY;
    }
};
#endif /* SRC_PHYSICS_GEOMETRY_GEOMETRY_H_ */
