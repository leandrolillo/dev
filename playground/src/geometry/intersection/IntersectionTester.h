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

    virtual bool intersectsVisit(const HierarchicalGeometry &hierarchy, const Geometry &op2) const = 0;
    virtual bool intersectsVisit(const AABB &aabb, const Geometry &op2) const { return false; };
    virtual bool intersectsVisit(const Sphere &sphere, const Geometry &op2) const = 0;
    virtual bool intersectsVisit(const Plane &plane, const Geometry &op2) const = 0;
    virtual bool intersectsVisit(const Line &line, const Geometry &op2) const = 0;


    /**
     * Visit methods for contacts
     */


    virtual std::vector<GeometryContact>  detectCollision(const Geometry &op1, const Geometry &op2) const {
        return op1.detectCollision(this, op2);
    }

    virtual std::vector<GeometryContact>  detectCollisionVisit(const HierarchicalGeometry &hierarchy, const Geometry &op2) const { return std::vector<GeometryContact>(); };
    virtual std::vector<GeometryContact>  detectCollisionVisit(const AABB &aabb, const Geometry &op2) const { return std::vector<GeometryContact>(); };
    virtual std::vector<GeometryContact>  detectCollisionVisit(const Sphere &sphere, const Geometry &op2) const = 0;
    virtual std::vector<GeometryContact>  detectCollisionVisit(const Plane &plane, const Geometry &op2) const = 0;
    virtual std::vector<GeometryContact>  detectCollisionVisit(const Line &line, const Geometry &op2) const = 0;
};

class SphereIntersectionTester : public IntersectionTesterBase{
public:
    SphereIntersectionTester() {}

    bool intersectsVisit(const HierarchicalGeometry &hierarchy, const Geometry &sphere) const override;
    bool intersectsVisit(const AABB &aabb, const Geometry &sphere) const override;
    bool intersectsVisit(const Plane &plane, const Geometry &sphere) const override;
    bool intersectsVisit(const Line &line, const Geometry &sphere) const override;
    bool intersectsVisit(const Sphere &sphere, const Geometry &anotherSphere) const override;

    std::vector<GeometryContact>  detectCollisionVisit(const HierarchicalGeometry &hierarchy, const Geometry &sphere) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const AABB &aabb, const Geometry &sphere) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Sphere &anotherSphere, const Geometry &sphere) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Plane &plane, const Geometry &sphere) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Line &line, const Geometry &sphere) const override;
};

class PlaneIntersectionTester : public IntersectionTesterBase {
public:
    PlaneIntersectionTester() {}

    bool intersectsVisit(const HierarchicalGeometry &hierarchy, const Geometry &plane) const override;
    bool intersectsVisit(const AABB &aabb, const Geometry &plane) const override;
    bool intersectsVisit(const Plane &anotherPlane, const Geometry &plane) const override;
    bool intersectsVisit(const Line &line, const Geometry &plane) const override;
    bool intersectsVisit(const Sphere &sphere, const Geometry &plane) const override;

    std::vector<GeometryContact>  detectCollisionVisit(const HierarchicalGeometry &hierarchy, const Geometry &plane) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const AABB &aabb, const Geometry &plane) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Sphere &sphere, const Geometry &plane) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Plane &anotherPlane, const Geometry &plane) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Line &line, const Geometry &plane) const override;
};

class LineIntersectionTester : public IntersectionTesterBase{
public:
    LineIntersectionTester() {}

    bool intersectsVisit(const HierarchicalGeometry &hierarchy, const Geometry &line) const override;
    bool intersectsVisit(const AABB &aabb, const Geometry &line) const override;
    bool intersectsVisit(const Plane &plane, const Geometry &line) const override;
    bool intersectsVisit(const Line &anotherLine, const Geometry &line) const override;
    bool intersectsVisit(const Sphere &sphere, const Geometry &line) const override;

    std::vector<GeometryContact>  detectCollisionVisit(const HierarchicalGeometry &hierarchy, const Geometry &line) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const AABB &aabb, const Geometry &line) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Sphere &sphere, const Geometry &line) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Plane &plane, const Geometry &line) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Line &anotherLine, const Geometry &line) const override;
};

class AABBIntersectionTester : public IntersectionTesterBase{
public:
    AABBIntersectionTester() {}

    bool intersectsVisit(const HierarchicalGeometry &hierarchy, const Geometry &aabb) const override;
    bool intersectsVisit(const AABB &anotherAabb, const Geometry &aabb) const override;
    bool intersectsVisit(const Plane &plane, const Geometry &aabb) const override;
    bool intersectsVisit(const Line &anotherLine, const Geometry &aabb) const override;
    bool intersectsVisit(const Sphere &sphere, const Geometry &aabb) const override;

    std::vector<GeometryContact>  detectCollisionVisit(const HierarchicalGeometry &hierarchy, const Geometry &aabb) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const AABB &anotherAabb, const Geometry &aabb) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Sphere &sphere, const Geometry &aabb) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Plane &plane, const Geometry &aabb) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Line &line, const Geometry &aabb) const override;
};

class HierarchyIntersectionTester : public IntersectionTesterBase{
public:
    HierarchyIntersectionTester() {}

    bool intersectsVisit(const HierarchicalGeometry &anotherHierarchy, const Geometry &hierarchy) const override;
    bool intersectsVisit(const AABB &anotherAabb, const Geometry &hierarchy) const override;
    bool intersectsVisit(const Plane &plane, const Geometry &hierarchy) const override;
    bool intersectsVisit(const Line &anotherLine, const Geometry &hierarchy) const override;
    bool intersectsVisit(const Sphere &sphere, const Geometry &hierarchy) const override;

    std::vector<GeometryContact>  detectCollisionVisit(const HierarchicalGeometry &anotherHierarchy, const Geometry &hierarchy) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const AABB &anotherAabb, const Geometry &hierarchy) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Sphere &sphere, const Geometry &hierarchy) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Plane &plane, const Geometry &hierarchy) const override;
    std::vector<GeometryContact>  detectCollisionVisit(const Line &line, const Geometry &hierarchy) const override;
};


class IntersectionTester : public IntersectionTesterBase{
protected:
    std::unique_ptr<IntersectionTesterBase> sphereIntersectionTester = std::make_unique<SphereIntersectionTester>();
    std::unique_ptr<IntersectionTesterBase> planeIntersectionTester = std::make_unique<PlaneIntersectionTester>();
    std::unique_ptr<IntersectionTesterBase> lineIntersectionTester = std::make_unique<LineIntersectionTester>();
    std::unique_ptr<IntersectionTesterBase> aabbIntersectionTester = std::make_unique<AABBIntersectionTester>();
    std::unique_ptr<IntersectionTesterBase> hierarchyIntersectionTester = std::make_unique<HierarchyIntersectionTester>();

public:

    bool intersectsVisit(const HierarchicalGeometry &hierarchy, const Geometry &op2) const override {
        return op2.intersects(hierarchyIntersectionTester.get(), hierarchy);
    }

    bool intersectsVisit(const AABB &aabb, const Geometry &op2) const override {
        return op2.intersects(aabbIntersectionTester.get(), aabb);
    }
    bool intersectsVisit(const Sphere &sphere, const Geometry &op2) const override{
        return op2.intersects(sphereIntersectionTester.get(), sphere);
    }

    bool intersectsVisit(const Plane &plane, const Geometry &op2) const override {
        return op2.intersects(planeIntersectionTester.get(), plane);
    }

    bool intersectsVisit(const Line &line, const Geometry &op2) const override {
        return op2.intersects(lineIntersectionTester.get(), line);
    }

    std::vector<GeometryContact> detectCollisionVisit(const HierarchicalGeometry &hierarchy, const Geometry &op2) const override {
        return op2.detectCollision(hierarchyIntersectionTester.get(), hierarchy);
    }

    std::vector<GeometryContact>  detectCollisionVisit(const AABB &aabb, const Geometry &op2) const override {
        return op2.detectCollision(aabbIntersectionTester.get(), aabb);
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
