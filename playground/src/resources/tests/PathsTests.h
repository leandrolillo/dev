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
        this->addTest("Paths::getDirname", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&PathsTests::testGetDirname));
        this->addTest("Paths::getBasename", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&PathsTests::testGetBasename));
    }

	void testGetDirname(PlaygroundRunner *runner) {
		String actual = Paths::getDirname("");
		assertEquals(defaultAssertMessage, ".", actual);
	}

	void testGetBasename(PlaygroundRunner *runner) {
		String actual = Paths::getBasename("");
		assertEquals(defaultAssertMessage, "", actual);
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
