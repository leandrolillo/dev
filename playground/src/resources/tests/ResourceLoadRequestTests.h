/*
 * ResourceManagerTests.h
 *
 *  Created on: Oct 25, 2022
 *      Author: leandro
 */

#pragma once

#include <Tests.h>
#include <ResourceLoadRequest.h>

class ResourceLoadRequestTests: public UnitTest
{
public:

	ResourceLoadRequestTests() {
        logger = LoggerFactory::getLogger("ResourceLoadRequestTests");
        logger->addAppender(LoggerFactory::getAppender("stdout"));

        this->addTest("ResourceLoadRequestTests::testToString", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ResourceLoadRequestTests::testToString));
        this->addTest("ResourceLoadRequestTests::testIsValid", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ResourceLoadRequestTests::testIsValid));
        this->addTest("ResourceLoadRequestTests::testSimpleUri", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ResourceLoadRequestTests::testSimpleUri));
    }

	void testToString(PlaygroundRunner *runner) {
		ResourceLoadRequest request("/test/filename.json");
		String actual = request.acceptMimeType("test/mimetype").withAdditionalLabels(std::set<String> {"additionalLabel"}).toString();
		assertEquals("Unexpected toString", "[application/json]->[test/mimetype] [/test/filename.json]", actual);
	}

	void testIsValid(PlaygroundRunner *runner) {
		ResourceLoadRequest request("/test/filename.json");
		bool actual = request.acceptMimeType("test/mimetype").withAdditionalLabels(std::set<String> {"additionalLabel"}).isValid();
		assertTrue(request.errors(), actual);

		ResourceLoadRequest anotherRequest("");
		actual = anotherRequest.isValid();
		assertTrue(anotherRequest.errors(), !actual);

		ResourceLoadRequest yetAnotherRequest("/test/filename");
		actual = yetAnotherRequest.acceptMimeType("test/mimetype") .isValid();
		assertTrue(yetAnotherRequest.errors(), !actual); //expect could not guess input mimetype
	}

	void testFilePath(PlaygroundRunner *runner) {
		ResourceLoadRequest request("/test/filename.json");
		String actual = request.getFilePath();
		assertEquals(defaultAssertMessage, "/test/filename.json", actual);
	}

	void testSimpleUri(PlaygroundRunner *runner) {
//		Resource parent(0, "test/mimetype");
//		parent.setFileName("/tests/filename.json");
//
//		ResourceLoadRequest request("child/filename.json");
//		//test fileParser
//
//
//
//		ResourceLoadRequest requestWithParent("/test/filename.json");
//		requestWithParent.withParent(&parent);
//		request.getFilePath() == "/tests/child/filename.json";
//		//test fileParser
//
//		ResourceLoadRequest requestFromFileParser(fileparser);
//		requestWithParent.withParent(&parent);
//		request.getFilePath() == "/tests/child/filename.json";
//		//test fileParser
	}


};
