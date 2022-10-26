/*
 * Math3dTests.h
 *
 *  Created on: Jul 28, 2021
 *      Author: leandro
 */

#ifndef SRC_MATH_TESTS_MATH3DTESTS_H_
#define SRC_MATH_TESTS_MATH3DTESTS_H_

#include<Tests.h>

class Math3dTests : public UnitTest {
public:
    Math3dTests() {
        logger = LoggerFactory::getLogger("Math3dTests");
        logger->addAppender(LoggerFactory::getAppender("stdout"));

        this->addTest("Math3dTests::testBasis", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&Math3dTests::testBasis));
    }

    /**
     * Check if rotation * translation equals manually building with orientation matrix top-left 3x3 and top-right column vector
     */
    void testBasis(PlaygroundRunner *runner)
    {
        vector angles(radian(45), radian(45), radian(45));
        vector position(1, 2, 3);
        matriz_3x3 orientation = matriz_3x3::matrizRotacion(angles);


        matriz expected = matriz::base(orientation, position);
        matriz actual = matriz::rotacion(angles) * matriz::traslacion(position);

        assertEquals("Expected and actual do not match", expected, actual);
    }

};



#endif /* SRC_MATH_TESTS_MATH3DTESTS_H_ */
