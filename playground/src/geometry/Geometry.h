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

class IntersectionTesterBase;

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

    /**
     * Visitor pattern for intersection testing and contact determination
     */
    virtual bool intersects(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const = 0;
    virtual std::vector<GeometryContact>  detectCollision(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const = 0;

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
        return "Sphere(origin: " + this->getOrigin().toString("%.2f") + ", radius: " + std::to_string(this->radius) + ")";
    }

    bool intersects(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const override;
    std::vector<GeometryContact> detectCollision(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const override;
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

    bool intersects(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const override;
    std::vector<GeometryContact> detectCollision(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const override;
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
        return "Line(origin: " + this->getOrigin().toString("%.2f") + ", dir: " + this->direction.toString("%.2f") + ")";
    }

    bool intersects(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const override;
    std::vector<GeometryContact>  detectCollision(const IntersectionTesterBase *intersectionTester, const Geometry &op2) const override;
};

#endif /* SRC_PHYSICS_GEOMETRY_GEOMETRY_H_ */
