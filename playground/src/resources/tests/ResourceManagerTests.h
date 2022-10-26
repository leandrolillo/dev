/*
 * ResourceManagerTests.h
 *
 *  Created on: Oct 25, 2022
 *      Author: leandro
 */

#ifndef SRC_RESOURCES_TESTS_RESOURCEMANAGERTESTS_H_
#define SRC_RESOURCES_TESTS_RESOURCEMANAGERTESTS_H_

#include <Tests.h>
#include <ResourceManager.h>

class ResourceManagerTests : public UnitTest
{
public:

	ResourceManagerTests() {
        logger = LoggerFactory::getLogger("ResourceManagerTests");
        logger->addAppender(LoggerFactory::getAppender("stdout"));

        this->addTest("ResourceManagerTests::testInvalidResource", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ResourceManagerTests::testInvalidResource));
        //this->addTest("ResourceManagerTests::testFileParser", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ResourceManagerTests::testFileParser));
    }


	void testInvalidResource(PlaygroundRunner *runner)
	{
		ResourceManager resourceManager(runner->getContainer()->getResourceManager()->getRootFolder());

		Resource *resource = resourceManager.load("tests/fake.wav", MimeTypes::AUDIO);
		assertEquals("Null resource expected", null, resource);

		resource = resourceManager.load("tests/fake.nomimetype", MimeTypes::AUDIO);
		assertEquals("Null resource expected", null, resource);

	}
	void testFileParser(PlaygroundRunner *runner)
	{
		ResourceManager resourceManager(runner->getContainer()->getResourceManager()->getRootFolder());

		String token;

		FileParser commentFileParser(resourceManager.normalize("tests/commentFileToParse.txt"));
		assertEquals("Unexpected token", FileParser::eof, commentFileParser.peekToken());
		assertEquals("Unexpected token", FileParser::eof, commentFileParser.takeToken());
		commentFileParser.close();

		FileParser emptyFileParser(resourceManager.normalize("tests/emptyFileToParse.txt"));
		assertEquals("Unexpected token", FileParser::eof, emptyFileParser.peekToken());
		assertEquals("Unexpected token", FileParser::eof, emptyFileParser.takeToken());
		commentFileParser.close();

		FileParser fileParser(resourceManager.normalize("tests/fileToParse.txt"));
		assertEquals("Unexpected token", "{", fileParser.peekToken());
		assertEquals("Unexpected token", "{", fileParser.takeToken());
		assertEquals("Unexpected token", "\"", fileParser.takeToken());
		assertEquals("Unexpected token", "property", fileParser.takeToken());
		assertEquals("Unexpected token", "\"", fileParser.takeToken());
		assertEquals("Unexpected token", ":", fileParser.takeToken());
		assertEquals("Unexpected token", "\"", fileParser.takeToken());
		assertEquals("Unexpected token", "value", fileParser.takeToken());
		assertEquals("Unexpected token", "\"", fileParser.takeToken());
		assertEquals("Unexpected token", "}", fileParser.takeToken());
		assertEquals("Unexpected token", FileParser::eof, fileParser.peekToken());
		assertEquals("Unexpected token", FileParser::eof, fileParser.takeToken());
	}
};



#endif /* SRC_RESOURCES_TESTS_RESOURCEMANAGERTESTS_H_ */
