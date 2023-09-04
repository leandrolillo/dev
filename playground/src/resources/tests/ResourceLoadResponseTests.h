/*
 * ResourceLoadResponseTest.h
 *
 *  Created on: Sep 4, 2023
 *      Author: llillo
 */

#pragma once

#include <Tests.h>
#include <ResourceLoadResponse.h>
#include "mock/ResourceManagerMock.h"


class ResourceLoadResponseTests: public UnitTest
{
public:

	ResourceLoadResponseTests() {
        logger = LoggerFactory::getLogger("ResourceLoadResponseTests");
        logger->addAppender(LoggerFactory::getAppender("stdout"));

        this->addTest("ResourceLoadResponseTests::testFilePath", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ResourceLoadResponseTests::testFilePath));
    }

	void testFilePath(PlaygroundRunner *runner) {
		ResourceManagerMock resourceManager("./target/../../media");
		ResourceLoadRequest request("geometry/basketball.json");

		ResourceLoadResponse response(request, resourceManager);

		String actual = response.getFullPath("~/images/basketball.png");

		assertEquals(defaultAssertMessage, (String)std::__fs::filesystem::absolute("./target/../../media") + "/images/basketball.png", actual);

	}

};
