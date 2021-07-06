/*
 * IntersectionTester.h
 *
 *  Created on: Jun 29, 2021
 *      Author: leandro
 */

#ifndef SRC_GEOMETRY_INTERSECTIONTESTER_H_
#define SRC_GEOMETRY_INTERSECTIONTESTER_H_


#include <vector>
#include <Geometry.h>
#include <GeometryContact.h>

class IntersectionTesterBase {
public:
    virtual ~IntersectionTesterBase() {

    }

    /**
     * Visit methods for intersection
     */
    virtual bool intersects(const Geometry &op1, const Geometry & op2) const {
        return op1.intersects(this, op2);
    }
    virtual bool intersectsVisit(const Sphere &sphere, const Geometry &op2) const = 0;
    virtual bool intersectsVisit(const Plane &plane, const Geometry &op2) const = 0;
    virtual bool intersectsVisit(const Line &line, const Geometry &op2) const = 0;


    /**
     * Visit methods for contacts
     */

    virtual std::vector<GeometryContact>  detectCollision(const Geometry &op1, const Geometry &op2) const {
        return op1.detectCollision(this, op2);
    }
    virtual std::vector<GeometryContact>  detectCollisionVisit(const Sphere &sphere, const Geometry &op2) const = 0;
    virtual std::vector<GeometryContact>  detectCollisionVisit(const Plane &plane, const Geometry &op2) const = 0;
    virtual std::vector<GeometryContact>  detectCollisionVisit(const Line &line, const Geometry &op2) const = 0;
};

class SphereIntersectionTester : public IntersectionTesterBase{
public:
    SphereIntersectionTester() {

    }

    bool intersectsVisit(const Plane &plane, const Geometry &sphere) const override;
    bool intersectsVisit(const Line &line, const Geometry &sphere) const override;
    bool intersectsVisit(const Sphere &sphere, const Geometry &anotherSphere) const override;

    std::vector<GeometryContact>  detectCollisionVisit(const Sphere &sphere, const Geometry &op2) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Plane &plane, const Geometry &op2) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Line &line, const Geometry &op2) const override;
};

class PlaneIntersectionTester : public IntersectionTesterBase {
public:
    PlaneIntersectionTester() {

        }
    bool intersectsVisit(const Plane &plane, const Geometry &anotherPlane) const override;
    bool intersectsVisit(const Line &line, const Geometry &plane) const override;
    bool intersectsVisit(const Sphere &sphere, const Geometry &plane) const override;

    std::vector<GeometryContact>  detectCollisionVisit(const Sphere &sphere, const Geometry &op2) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Plane &plane, const Geometry &op2) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Line &line, const Geometry &op2) const override;
};

class LineIntersectionTester : public IntersectionTesterBase{
public:
    LineIntersectionTester() {

        }
    bool intersectsVisit(const Plane &plane, const Geometry &line) const override;
    bool intersectsVisit(const Line &anotherLine, const Geometry &line) const override;
    bool intersectsVisit(const Sphere &sphere, const Geometry &line) const override;

    std::vector<GeometryContact>  detectCollisionVisit(const Sphere &sphere, const Geometry &op2) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Plane &plane, const Geometry &op2) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Line &line, const Geometry &op2) const override;
};

class IntersectionTester : public IntersectionTesterBase{
protected:
    std::unique_ptr<IntersectionTesterBase> sphereIntersectionTester = std::make_unique<SphereIntersectionTester>();
    std::unique_ptr<IntersectionTesterBase> planeIntersectionTester = std::make_unique<PlaneIntersectionTester>();
    std::unique_ptr<IntersectionTesterBase> lineIntersectionTester = std::make_unique<LineIntersectionTester>();

public:
    bool intersectsVisit(const Sphere &sphere, const Geometry &op2) const override{
        return op2.intersects(sphereIntersectionTester.get(), sphere);
    }

    bool intersectsVisit(const Plane &plane, const Geometry &op2) const override {
        return op2.intersects(planeIntersectionTester.get(), plane);
    }

    bool intersectsVisit(const Line &line, const Geometry &op2) const override {
        return op2.intersects(lineIntersectionTester.get(), line);
    }

    std::vector<GeometryContact>  detectCollisionVisit(const Sphere &sphere, const Geometry &op2) const override {
        return op2.detectCollision(sphereIntersectionTester.get(), sphere);
    }
    std::vector<GeometryContact>  detectCollisionVisit(const Plane &plane, const Geometry &op2) const override {
        return op2.detectCollision(planeIntersectionTester.get(), plane);
    }
    std::vector<GeometryContact>  detectCollisionVisit(const Line &line, const Geometry &op2) const override {
        return op2.detectCollision(lineIntersectionTester.get(), line);
    }

};


#endif /* SRC_GEOMETRY_INTERSECTIONTESTER_H_ */
