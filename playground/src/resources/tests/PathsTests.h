/*
 * PathsTests.h
 *
 *  Created on: Oct 24, 2022
 *      Author: leandro
 */

#ifndef SRC_RESOURCES_TESTS_PATHSTESTS_H_
#define SRC_RESOURCES_TESTS_PATHSTESTS_H_

#include <Tests.h>
#include <Paths.h>

class PathsTests : public UnitTest {
public:
	PathsTests() {
        logger = LoggerFactory::getLogger("PathsTests");
        logger->addAppender(LoggerFactory::getAppender("stdout"));

        this->addTest("Paths::add", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&PathsTests::testAdd));
    }

    void testAdd(PlaygroundRunner *runner)
    {
    	//relative path
    	String actual = Paths::add("/home/assets", "resource.json");
    	assertEquals(defaultAssertMessage, "/home/assets/resource.json", actual);

    	actual = Paths::add(" /home/assets/ ", " resource.json ");
    	assertEquals(defaultAssertMessage, "/home/assets/resource.json", actual);

    	actual = Paths::add("/home/assets/", "./resource.json");
    	assertEquals(defaultAssertMessage, "/home/assets/resource.json", actual);

    	actual = Paths::add("/home/assets", "./resource.json");
    	assertEquals(defaultAssertMessage, "/home/assets/resource.json", actual);

    	//Absolute path
    	actual = Paths::add("/home/assets", "/resource.json");
    	assertEquals(defaultAssertMessage, "/resource.json", actual);

    	actual = Paths::add("/home/assets/", "/resource.json");
    	assertEquals(defaultAssertMessage, "/resource.json", actual);

    	//relative to home
    	actual = Paths::add("/home/assets", "~/resource.json");
    	assertEquals(defaultAssertMessage, "/home/assets/resource.json", actual);

    	actual = Paths::add("/home/assets/", "~/resource.json");
    	assertEquals(defaultAssertMessage, "/home/assets/resource.json", actual);

    	// edge cases
    	actual = Paths::add("", "resource.json");
    	assertEquals(defaultAssertMessage, "resource.json", actual);

    	actual = Paths::add("/home/assets/", "");
    	assertEquals(defaultAssertMessage, "/home/assets/", actual);

    	actual = Paths::add("", "");
    	assertEquals(defaultAssertMessage, "", actual);

    }

};



#endif /* SRC_RESOURCES_TESTS_PATHSTESTS_H_ */
