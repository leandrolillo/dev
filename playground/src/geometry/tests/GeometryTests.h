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
        this->addTest("GeometryTests::testGeometryIntersectionTester", static_cast<void (UnitTest::*)()>(&GeometryTests::testGeometryIntersectionTests));
        logger = LoggerFactory::getLogger("GeometryTests");
        logger->addAppender(LoggerFactory::getAppender("stdout"));
    }

    void testGeometryIntersectionTests()
    {
        Sphere sphere(vector(0, 0, 0), 2);
        Sphere anotherSphere(vector(3, 1, 1), 2);
        Plane plane(vector(0, 0, 0), vector(0, 1, 0));

        IntersectionTester intersectionTester;

        assertTrue("Spheres are intersecting", intersectionTester.intersects((Geometry &)sphere, (Geometry &)anotherSphere));
        assertTrue("Sphere is intersecting plane", intersectionTester.intersects((Geometry &)sphere, (Geometry &)plane));
        assertTrue("Sphere is intersecting plane", intersectionTester.intersects((Geometry &)plane, (Geometry &)sphere));

        anotherSphere.setOrigin(vector(0, 6, 0));
        assertFalse("Spheres are not intersecting", intersectionTester.intersects((Geometry &)sphere, (Geometry &)anotherSphere));
        assertFalse("Sphere is not intersecting plane", intersectionTester.intersects((Geometry &)anotherSphere, (Geometry &)plane));
        assertFalse("Sphere is not intersecting plane", intersectionTester.intersects((Geometry &)plane, (Geometry &)anotherSphere));
    }
};


#endif /* SRC_GEOMETRY_TESTS_GEOMETRYTESTS_H_ */
