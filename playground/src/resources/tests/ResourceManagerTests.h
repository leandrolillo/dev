/*
 * ResourceManagerTests.h
 *
 *  Created on: Oct 25, 2022
 *      Author: leandro
 */

#ifndef SRC_RESOURCES_TESTS_RESOURCEMANAGERTESTS_H_
#define SRC_RESOURCES_TESTS_RESOURCEMANAGERTESTS_H_

#include <Tests.h>
#include "ResourceManagerMock.h"

class ResourceAdapterMock : public ResourceAdapter {
public:
	ResourceAdapterMock(const std::set<String> &outputMimeTypes, String inputMimeType) {
		this->setOutputMimeTypes(outputMimeTypes);
		this->accepts(inputMimeType);
	}

	virtual Resource* load(ResourceLoadRequest &request) const override {
		return null;
	}
};


class ResourceManagerTests : public UnitTest
{
public:

	ResourceManagerTests() {
        logger = LoggerFactory::getLogger("ResourceManagerTests");
        logger->addAppender(LoggerFactory::getAppender("stdout"));

        this->addTest("ResourceManagerTests::testInvalidResource", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ResourceManagerTests::testInvalidResource));
        this->addTest("ResourceManagerTests::testFileParser", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ResourceManagerTests::testFileParser));

        this->addTest("ResourceManagerTests::testAddResourceAdapter", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ResourceManagerTests::testAddResourceAdapter));
        this->addTest("ResourceManagerTests::testAddResource", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ResourceManagerTests::testAddResource));
        this->addTest("ResourceManagerTests::testUnload", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ResourceManagerTests::testUnload));
    }

	void testAddResourceAdapter(PlaygroundRunner *runner) {
		ResourceManagerMock resourceManager(runner->getContainer()->getResourceManager()->getRootFolder());

		ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::unique_ptr<ResourceAdapter>(new ResourceAdapterMock(std::set<String>{"test/outputMimeType"}, "test/inputMimeType")));
		assertNotNull(defaultAssertMessage, resourceAdapter);
		assertNotNull(defaultAssertMessage, resourceAdapter->getResourceManager());
		logger->info("Testing resource adapter [%s]", resourceAdapter->toString().c_str());

		assertEquals(defaultAssertMessage, 1, resourceManager.getAdaptersCount());
	}


	void testAddResource(PlaygroundRunner *runner) {
		ResourceManagerMock resourceManager(runner->getContainer()->getResourceManager()->getRootFolder());

		Resource *resource = new Resource(1, "test/mimetype");
		resource->setFileName("/test/filename");
		resourceManager.addResource(resource);
		resource->toString(); //any better way to try to trigger illegal access?
		assertEquals(defaultAssertMessage, 1, resourceManager.getResourcesCount());

		resource = new Resource(2, "test/mimetype");
		resource->setFileName("/test/filename2");
		resourceManager.addResource(resource);
		resource->toString(); //any better way to try to trigger illegal access?
		assertEquals(defaultAssertMessage, 2, resourceManager.getResourcesCount());

		resource = resourceManager.load("/test/filename", "test/mimetype");
		assertNotNull("Resource not found in cache", resource);
		assertEquals("filename does not match", "/test/filename", resource->getFileName());
		assertEquals("mimetype does not match", "test/mimetype", resource->getMimeType());

		/**
		 * Add a duplicated resource to check nothing gets deleted
		 */
		resourceManager.addResource(resource);
		resource->toString(); //any better way to try to trigger illegal access?
		assertEquals(defaultAssertMessage, 2, resourceManager.getResourcesCount());

	}

	void testUnload(PlaygroundRunner *runner) {
		ResourceManagerMock resourceManager(runner->getContainer()->getResourceManager()->getRootFolder());

		Resource *resource = new Resource(1, "test/mimetype");
		resource->setFileName("test/filename1");
		resource->addLabel("ephemeral");
		resourceManager.addResource(resource);

		resource = new Resource(2, "test/mimetype");
		resource->setFileName("test/filename2");
		resource->addLabel("not ephemeral");
		resource->addLabel("level 1");
		resourceManager.addResource(resource);

		resource = new Resource(3, "test/mimetype");
		resource->setFileName("test/filename3");
		resource->addLabel("permanent");
		resourceManager.addResource(resource);

		resource = new Resource(4, "test/mimetype");
		resource->setFileName("test/filename4");
		resourceManager.addResource(resource);

//		/*
//		 * This one should evict the previous one from cache and keep resources count to 4
//		 */
//		resource = new Resource(4, "test/mimetype");
//		resource->setFileName("test/filename4");
//		resourceManager.addResource(resource);

		assertEquals("Resources mismatch", 4, resourceManager.getResourcesCount());

		/*
		 * Test unload by resource
		 */

		resourceManager.unload(resource);
		assertEquals("Resources mismatch", 3, resourceManager.getResourcesCount());

		/*
		 * Test unload by single label
		 */
		resourceManager.unload("ephemeral");
		assertEquals("Resources mismatch", 2, resourceManager.getResourcesCount());

		/*
		 * Test unload by multiple labels
		 */
		resourceManager.unload(std::set<String> {"not ephemeral", "level 1"});
		assertEquals("Resources mismatch", 1, resourceManager.getResourcesCount());

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
