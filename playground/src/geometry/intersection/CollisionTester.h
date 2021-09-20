/*
 * IntersectionTester.h
 *
 *  Created on: Jun 29, 2021
 *      Author: leandro
 */

#ifndef SRC_GEOMETRY_INTERSECTIONTESTER_H_
#define SRC_GEOMETRY_INTERSECTIONTESTER_H_

#include <vector>
#include <map>
#include <Geometry.h>
#include <GeometryContact.h>

class CollisionTester {
protected:
    std::map<std::pair<GeometryType, GeometryType>, std::vector<GeometryContact> (CollisionTester::*)(const Geometry &, const Geometry &) const> contactTestsTable;
    std::map<std::pair<GeometryType, GeometryType>, bool (CollisionTester::*)(const Geometry &, const Geometry &) const> intersectionTestsTable;

public:

    CollisionTester() {
        this->addIntersectionTests();
        this->addContactTests();
    }

    virtual ~CollisionTester() {
    }
    
    void addIntersectionTests() {
        this->addIntersectionTest(GeometryType::LINE, GeometryType::SPHERE, &CollisionTester::lineSphere);
        this->addIntersectionTest(GeometryType::LINE, GeometryType::PLANE, &CollisionTester::linePlane);
        this->addIntersectionTest(GeometryType::LINE, GeometryType::LINE, &CollisionTester::lineLine);
        this->addIntersectionTest(GeometryType::LINE, GeometryType::AABB, &CollisionTester::lineAabb);
        this->addIntersectionTest(GeometryType::LINE, GeometryType::HIERARCHY, &CollisionTester::lineHierarchy);

        this->addIntersectionTest(GeometryType::PLANE, GeometryType::SPHERE, &CollisionTester::planeSphere);
        this->addIntersectionTest(GeometryType::PLANE, GeometryType::PLANE, &CollisionTester::planePlane);
        this->addIntersectionTest(GeometryType::PLANE, GeometryType::AABB, &CollisionTester::planeAabb);
        this->addIntersectionTest(GeometryType::PLANE, GeometryType::HIERARCHY, &CollisionTester::planeHierarchy);

        this->addIntersectionTest(GeometryType::SPHERE, GeometryType::SPHERE, &CollisionTester::sphereSphere);
        this->addIntersectionTest(GeometryType::SPHERE, GeometryType::AABB, &CollisionTester::sphereAabb);
        this->addIntersectionTest(GeometryType::SPHERE, GeometryType::HIERARCHY, &CollisionTester::sphereHierarchy);

        this->addIntersectionTest(GeometryType::AABB, GeometryType::AABB, &CollisionTester::aabbAabb);
        this->addIntersectionTest(GeometryType::AABB, GeometryType::HIERARCHY, &CollisionTester::aabbHierarchy);

        this->addIntersectionTest(GeometryType::HIERARCHY, GeometryType::HIERARCHY, &CollisionTester::hierarchyHierarchy);
    }
    void addContactTests() {
        this->addContactTest(GeometryType::LINE, GeometryType::SPHERE, &CollisionTester::lineSphereContact);
        this->addContactTest(GeometryType::LINE, GeometryType::PLANE, &CollisionTester::linePlaneContact);
        this->addContactTest(GeometryType::LINE, GeometryType::LINE, &CollisionTester::lineLineContact);
        this->addContactTest(GeometryType::LINE, GeometryType::AABB, &CollisionTester::lineAabbContact);
        this->addContactTest(GeometryType::LINE, GeometryType::HIERARCHY, &CollisionTester::lineHierarchyContact);

        this->addContactTest(GeometryType::PLANE, GeometryType::SPHERE, &CollisionTester::planeSphereContact);
        this->addContactTest(GeometryType::PLANE, GeometryType::PLANE, &CollisionTester::planePlaneContact);
        this->addContactTest(GeometryType::PLANE, GeometryType::AABB, &CollisionTester::planeAabbContact);
        this->addContactTest(GeometryType::PLANE, GeometryType::HIERARCHY, &CollisionTester::planeHierarchyContact);

        this->addContactTest(GeometryType::SPHERE, GeometryType::SPHERE, &CollisionTester::sphereSphereContact);
        this->addContactTest(GeometryType::SPHERE, GeometryType::AABB, &CollisionTester::sphereAabbContact);
        this->addContactTest(GeometryType::SPHERE, GeometryType::HIERARCHY, &CollisionTester::sphereHierarchyContact);

        this->addContactTest(GeometryType::AABB, GeometryType::AABB, &CollisionTester::aabbAabbContact);
        this->addContactTest(GeometryType::AABB, GeometryType::HIERARCHY, &CollisionTester::aabbHierarchyContact);

        this->addContactTest(GeometryType::HIERARCHY, GeometryType::HIERARCHY, &CollisionTester::hierarchyHierarchyContact);
    }

    virtual void addIntersectionTest(const GeometryType &typeOp1, const GeometryType &typeOp2, bool (CollisionTester::*intersectionTest)(const Geometry &, const Geometry &) const) {
        intersectionTestsTable[std::pair<const GeometryType &, const GeometryType &>(typeOp1, typeOp2)] = intersectionTest;
    }

    virtual void addContactTest(const GeometryType &typeOp1, const GeometryType &typeOp2, std::vector<GeometryContact> (CollisionTester::*contactTest)(const Geometry &, const Geometry &) const) {
        contactTestsTable[std::pair<const GeometryType &, const GeometryType &>(typeOp1, typeOp2)] = contactTest;
    }


    virtual bool intersects(const Geometry &op1, const Geometry & op2) const {
        std::pair<GeometryType, GeometryType > key(op1.getType(), op2.getType());

        if(intersectionTestsTable.count(key) > 0) {
            bool (CollisionTester::*testFunction)(const Geometry &, const Geometry &) const;
            testFunction = intersectionTestsTable.at(key);
            return (this->*testFunction)(op1, op2);
        } else {
            std::pair<GeometryType, GeometryType > inverseKey(op2.getType(), op1.getType());

            if(intersectionTestsTable.count(inverseKey) > 0) {
                bool (CollisionTester::*testFunction)(const Geometry &, const Geometry &) const;
                testFunction = intersectionTestsTable.at(inverseKey);
                return (this->*testFunction)(op2, op1);
            }
        }

        return false;
    }

    virtual std::vector<GeometryContact>  detectCollision(const Geometry &op1, const Geometry &op2) const {
        std::pair<GeometryType, GeometryType > key(op1.getType(), op2.getType());

        if(contactTestsTable.count(key) > 0) {
            std::vector<GeometryContact> (CollisionTester::*testFunction)(const Geometry &, const Geometry &) const;
            testFunction = contactTestsTable.at(key);
            return (this->*testFunction)(op1, op2);
        } else {
            std::pair<GeometryType, GeometryType > inverseKey(op2.getType(), op1.getType());

            if(contactTestsTable.count(inverseKey) > 0) {
                std::vector<GeometryContact> (CollisionTester::*testFunction)(const Geometry &, const Geometry &) const;
                testFunction = contactTestsTable.at(inverseKey);
                return (this->*testFunction)(op2, op1);
            }
        }

        return std::vector<GeometryContact>();
    }


    /*****
     *
     * Intersection Tests
     *
     *****/

    /**
     * point sphere test
     */

    bool pointSphere(const vector &point, const Sphere &sphere) const {
        vector delta = sphere.getOrigin() - point;
        return delta * delta <= sphere.getRadius() * sphere.getRadius();
    }


    /**
     * Line intersection test
     */
    bool lineSphere(const Geometry &lineGeometry, const Geometry &sphereGeometry) const {
        const Line &line = (const Line &) lineGeometry;
        const Sphere &sphere = (const Sphere &) sphereGeometry;

        real projection = (sphere.getOrigin() - line.getOrigin()) * line.getDirection();
        vector projectedSphereCenter = line.getOrigin() + line.getDirection() * projection;
        vector lineToSphere = sphere.getOrigin() - projectedSphereCenter;

        return (lineToSphere * lineToSphere <= sphere.getRadius() * sphere.getRadius());
    }

    bool linePlane(const Geometry &line, const Geometry &plane) const {
       return false;
    }

    bool lineLine(const Geometry &line, const Geometry &anotherLine) const {
       return false;
    }

    bool lineAabb(const Geometry &line, const Geometry &aabb) const {
           return false;
    }

    bool lineHierarchy(const Geometry &line, const Geometry &hierarchy) const {
           return false;
    }

    /**
     * Plane intersection test
     */
    bool planeSphere(const Geometry &planeGeometry, const Geometry &sphereGeometry) const {
        Plane &plane = (Plane &)planeGeometry;
        Sphere &sphere = (Sphere &)sphereGeometry;
        vector delta = ((sphere.getOrigin() - plane.getOrigin()) * plane.getNormal()) * plane.getNormal();
        return (delta * delta <= sphere.getRadius() * sphere.getRadius());
    }

    bool planePlane(const Geometry &planeGeometry, const Geometry &anotherPlaneGeometry) const {
        return false;
    }

    bool planeAabb(const Geometry &planeGeometry, const Geometry &aabbGeometry) const {
            return false;
    }

    bool planeHierarchy(const Geometry &planeGeometry, const Geometry &hierarchyGeometry) const {
           return false;
    }

    /**
     * Sphere intersection test
     */
    bool sphereSphere(const Geometry &sphereGeometry, const Geometry &anotherSphereGeometry) const {
        const Sphere &sphere = (const Sphere &)sphereGeometry;
        const Sphere &anotherSphere = (const Sphere &)anotherSphereGeometry;
        vector delta = sphere.getOrigin() - anotherSphere.getOrigin();
        real radiuses = sphere.getRadius() + anotherSphere.getRadius();

        return (delta * delta <= radiuses * radiuses);
    }

    bool sphereAabb(const Geometry &sphereGeometry, const Geometry &aabbGeometry) const {
        const Sphere &sphere = (const Sphere &)sphereGeometry;
        const AABB &aabb = (const AABB &)aabbGeometry;
        //printf("Checking intersection for %s and %s \n", sphere.toString().c_str(), aabb.toString().c_str());
        vector mins = aabb.getOrigin() - aabb.getHalfSizes();
        vector maxs = aabb.getOrigin() + aabb.getHalfSizes();

        //printf("mins: %s - maxs: %s\n", mins.toString().c_str(), maxs.toString().c_str());


        vector aabbClosestPoint = vector(std::max(mins.x, std::min(sphere.getOrigin().x, maxs.x)),
                std::max(mins.y, std::min(sphere.getOrigin().y, maxs.y)),
                std::max(mins.z, std::min(sphere.getOrigin().z, maxs.z))
                );

        //printf("closest point: %s\n", aabbClosestPoint.toString().c_str());

        return pointSphere(aabbClosestPoint, sphere);
    }

    bool sphereHierarchy(const Geometry &sphere, const Geometry &hierarchy) const {
           return false;
    }


    /**
     * AABB intersection tests
     */
    bool aabbAabb(const Geometry &aabb, const Geometry &anotherAabb) const {
        return false;
    }

    bool aabbHierarchy(const Geometry &aabb, const Geometry &hierarchy) const {
       return false;
    }

    /**
     * Hierarchy intersection tests
     */

    bool hierarchyHierarchy(const Geometry &hierarchy, const Geometry &anotherHierarchy) const {
       return false;
    }


    /*****
     *
     * Contact determination Tests
     *
     *****/

    /**
     * Line contact Determination
     */
    std::vector<GeometryContact> lineSphereContact(const Geometry &lineGeometry, const Geometry &sphereGeometry) const {
        const Line &line = (const Line &)lineGeometry;
        const Sphere &sphere = (const Sphere &)sphereGeometry;

        return std::vector<GeometryContact>();
    }

    std::vector<GeometryContact> linePlaneContact(const Geometry &lineGeometry, const Geometry &planeGeometry) const {
        const Line &line = (const Line &)lineGeometry;
        const Plane &plane= (const Plane &)planeGeometry;

        return std::vector<GeometryContact>();
    }

    std::vector<GeometryContact> lineLineContact(const Geometry &lineGeometry, const Geometry &anotherLineGeometry) const {
        const Line &line = (const Line &)lineGeometry;
        const Line &anotherLine = (const Line &)anotherLineGeometry;

        return std::vector<GeometryContact>();
    }

    std::vector<GeometryContact> lineAabbContact(const Geometry &lineGeometry, const Geometry &aabbGeometry) const {
        const Line &line = (const Line &)lineGeometry;
        const AABB &aabb = (const AABB &)aabbGeometry;

        return std::vector<GeometryContact>();
    }
    std::vector<GeometryContact> lineHierarchyContact(const Geometry &lineGeometry, const Geometry &hierarchyGeometry) const {
        const Line &line = (const Line &)lineGeometry;
        const HierarchicalGeometry &hierarchy = (const HierarchicalGeometry &)hierarchyGeometry;

        return std::vector<GeometryContact>();
    }



    /**
     * Plane contact determination
     */
    std::vector<GeometryContact> planeSphereContact(const Geometry &planeGeometry, const Geometry &sphereGeometry) const {
        const Plane & plane = (const Plane &)planeGeometry;
        const Sphere & sphere= (const Sphere &)sphereGeometry;

        vector normal = plane.getNormal();

        real distance = (sphere.getOrigin() - plane.getOrigin()) * normal;

        if(distance <= sphere.getRadius()) {
            vector delta = distance * normal;
            real penetration = sphere.getRadius() - distance;
            vector intersection = sphere.getOrigin() - (normal * sphere.getRadius());

            return std::vector<GeometryContact> {GeometryContact(&plane, &sphere, intersection, normal, 0.8f, penetration) };
        }

        return std::vector<GeometryContact>();
    }

    std::vector<GeometryContact> planePlaneContact(const Geometry &planeGeometry, const Geometry &anotherPlaneGeometry) const {
        const Plane & plane = (const Plane &)planeGeometry;
        const Plane & anotherPlane = (const Plane &)anotherPlaneGeometry;

        return std::vector<GeometryContact>();
    }

    std::vector<GeometryContact> planeAabbContact(const Geometry &planeGeometry, const Geometry &aabbGeometry) const {
        const Plane & plane = (const Plane &)planeGeometry;
        const AABB & aabb = (const AABB &)aabbGeometry;

        return std::vector<GeometryContact>();
    }

    std::vector<GeometryContact> planeHierarchyContact(const Geometry &planeGeometry, const Geometry &hierarchyGeometry) const {
        const Plane & plane = (const Plane &)planeGeometry;
        const HierarchicalGeometry & hierarchy = (const HierarchicalGeometry &)hierarchyGeometry;

        return std::vector<GeometryContact>();
    }


    /**
     * Sphere contact determination
     */
    std::vector<GeometryContact> sphereSphereContact(const Geometry &sphereGeometry, const Geometry &anotherSphereGeometry) const {
        const Sphere &sphere = (const Sphere &)sphereGeometry;
        const Sphere &anotherSphere = (const Sphere &)anotherSphereGeometry;

        vector delta = sphere.getOrigin() - anotherSphere.getOrigin();
        real radiuses = sphere.getRadius() + anotherSphere.getRadius();

        if(delta * delta <= radiuses * radiuses) {
            real distance = delta.modulo();
            vector normal = delta * (1.0 / distance);
            real penetration = radiuses - distance;
            vector intersection = sphere.getOrigin() + (normal * sphere.getRadius());
            return std::vector<GeometryContact> {GeometryContact(&sphere, &anotherSphere, intersection, normal, 0.8f,  penetration) };
        }

        return std::vector<GeometryContact>();
    }

    std::vector<GeometryContact> sphereAabbContact(const Geometry &sphereGeometry, const Geometry &aabbGeometry) const {
        const Sphere &sphere = (const Sphere &)sphereGeometry;
        const AABB &aabb = (const AABB &)aabbGeometry;

        //printf("Checking intersection for %s and %s \n", sphere.toString().c_str(), aabb.toString().c_str());
        vector mins = aabb.getOrigin() - aabb.getHalfSizes();
        vector maxs = aabb.getOrigin() + aabb.getHalfSizes();

        //printf("mins: %s - maxs: %s\n", mins.toString().c_str(), maxs.toString().c_str());

        vector aabbClosestPoint = vector(std::max(mins.x, std::min(sphere.getOrigin().x, maxs.x)),
                std::max(mins.y, std::min(sphere.getOrigin().y, maxs.y)),
                std::max(mins.z, std::min(sphere.getOrigin().z, maxs.z))
                );

        //printf("closest point: %s\n", aabbClosestPoint.toString().c_str());

        if(pointSphere(aabbClosestPoint, sphere)) {
            vector delta = sphere.getOrigin() - aabbClosestPoint;
            real distance = delta.modulo();
            vector normal = delta * (1.0 / distance);
            real penetration = sphere.getRadius() - distance;

            return std::vector<GeometryContact> {GeometryContact(&sphere, &aabb, aabbClosestPoint, normal, 0.8f,  penetration) };
        }

        return std::vector<GeometryContact>();
    }

    std::vector<GeometryContact> sphereHierarchyContact(const Geometry &sphereGeometry, const Geometry &hierarchyGeometry) const {
        const Sphere &sphere = (const Sphere &)sphereGeometry;
        const HierarchicalGeometry &hierarchy = (const HierarchicalGeometry &)hierarchyGeometry;

        return std::vector<GeometryContact>();
    }



    /**
     * AABB contact determination
     */
    std::vector<GeometryContact> aabbAabbContact(const Geometry &aabbGeometry, const Geometry &anotherAabbGeometry) const {
        const AABB &aabb = (const AABB &)aabbGeometry;
        const AABB &anotherAabb = (const AABB &)anotherAabbGeometry;

        return std::vector<GeometryContact>();
    }

    std::vector<GeometryContact> aabbHierarchyContact(const Geometry &aabbGeometry, const Geometry &hierarchyGeometry) const {
        const AABB &aabb = (const AABB &)aabbGeometry;
        const HierarchicalGeometry &hierarchy = (const HierarchicalGeometry &)hierarchyGeometry;

        return std::vector<GeometryContact>();
    }

    /**
     * Hierarchy contact determination
     */
    std::vector<GeometryContact> hierarchyHierarchyContact(const Geometry &hierarchyGeometry, const Geometry &anotherHierarchyGeometry) const {
        const HierarchicalGeometry &hierarchy = (const HierarchicalGeometry &)hierarchyGeometry;
        const HierarchicalGeometry &anotherHierarchy = (const HierarchicalGeometry &)anotherHierarchyGeometry;

        return std::vector<GeometryContact>();
    }
};



#endif /* SRC_GEOMETRY_INTERSECTIONTESTER_H_ */
