/*
 * GeometryTests.h
 *
 *  Created on: Jun 29, 2021
 *      Author: leandro
 */

#ifndef SRC_GEOMETRY_TESTS_GEOMETRYTESTS_H_
#define SRC_GEOMETRY_TESTS_GEOMETRYTESTS_H_

#include <Tests.h>
#include <Geometry.h>
#include <IntersectionTester.h>

class GeometryTests : public UnitTest {
public:
    GeometryTests() {
        logger = LoggerFactory::getLogger("GeometryTests");
        logger->addAppender(LoggerFactory::getAppender("stdout"));

        this->addTest("GeometryTests::testSphereIntersectionTester", static_cast<void (UnitTest::*)()>(&GeometryTests::testSphereIntersectionTester));
        this->addTest("GeometryTests::testPlaneIntersectionTester", static_cast<void (UnitTest::*)()>(&GeometryTests::testPlaneIntersectionTester));
        this->addTest("GeometryTests::testAabbIntersectionTester", static_cast<void (UnitTest::*)()>(&GeometryTests::testAabbIntersectionTester));
    }

    void testSphereIntersectionTester()
    {
        Sphere sphere(vector(0, 0, 0), 2);
        Sphere anotherSphere(vector(3, 1, 1), 2);
        Plane plane(vector(0, 0, 0), vector(0, 1, 0));

        AABB aabb(vector(-1, 0, 0), vector(1, 1, 1));

        IntersectionTester intersectionTester;

        assertTrue("Spheres are intersecting", intersectionTester.intersects((Geometry &)sphere, (Geometry &)anotherSphere));
        assertTrue("Sphere is intersecting plane", intersectionTester.intersects((Geometry &)sphere, (Geometry &)plane));
        assertTrue("Sphere is intersecting aabb", intersectionTester.intersects((Geometry &)sphere, (Geometry &)aabb));

        anotherSphere.setOrigin(vector(0, 6, 0));
        assertFalse("Spheres are not intersecting", intersectionTester.intersects((Geometry &)sphere, (Geometry &)anotherSphere));
        assertFalse("Sphere is not intersecting plane", intersectionTester.intersects((Geometry &)anotherSphere, (Geometry &)plane));
        assertFalse("Sphere is not intersecting aabb", intersectionTester.intersects((Geometry &)anotherSphere, (Geometry &)aabb));
    }

    void testPlaneIntersectionTester()
    {
        Sphere sphere(vector(0, 0, 0), 2);
        Sphere anotherSphere(vector(0, 6, 0), 2);
        Plane plane(vector(0, 0, 0), vector(0, 1, 0));

        IntersectionTester intersectionTester;

        assertTrue("Plane is intersecting sphere", intersectionTester.intersects((Geometry &)plane, (Geometry &)sphere));
        assertFalse("Plane is not intersecting sphere", intersectionTester.intersects((Geometry &)plane, (Geometry &)anotherSphere));
    }

    void testAabbIntersectionTester()
    {
        Sphere sphere(vector(0, 0, 0), 2);
        AABB aabb(vector(1, 0, 0), vector(1, 1, 1));

        IntersectionTester intersectionTester;

        assertTrue("Aabb is intersecting sphere", intersectionTester.intersects((Geometry &)aabb, (Geometry &)sphere));
        aabb.setOrigin(vector(4, 0, 0));
        assertFalse("Aabb is not intersecting sphere", intersectionTester.intersects((Geometry &)aabb, (Geometry &)sphere));
    }
};


#endif /* SRC_GEOMETRY_TESTS_GEOMETRYTESTS_H_ */
