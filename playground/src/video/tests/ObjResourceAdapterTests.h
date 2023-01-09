/*
 * GeometryResourceAdapterTests.h
 *
 *  Created on: Oct 25, 2022
 *      Author: leandro
 */

#pragma once

#include <Tests.h>
#include "../../resources/tests/ResourceManagerMock.h"
#include <ObjResourceAdapter.h>

class ObjResourceAdapterTests : public UnitTest
{
public:
	ObjResourceAdapterTests() {
        logger = LoggerFactory::getLogger("ObjResourceAdapterTests");
        logger->addAppender(LoggerFactory::getAppender("stdout"));

        this->addTest("ObjResourceAdapterTests::testLoad", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ObjResourceAdapterTests::testLoad));
        this->addTest("ObjResourceAdapterTests::testLoadWithResourceManager", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ObjResourceAdapterTests::testLoadWithResourceManager));
    }

	/**
	 * Needs to provide a resource manager since the adapter might make usage of it.
	 */
	void testLoad(PlaygroundRunner *runner){
		ResourceManagerMock resourceManager(runner->getContainer()->getResourceManager()->getRootFolder());
		ResourceAdapter *resourceAdapter = resourceManager.addAdapter(std::unique_ptr<ResourceAdapter>(new ObjResourceAdapter()));

		assertNotNull(defaultAssertMessage, resourceAdapter);
		assertNotNull(defaultAssertMessage, resourceAdapter->getResourceManager());
		logger->info("Testing resource adapter [%s]", resourceAdapter->toString().c_str());

		ResourceLoadRequest request(Paths::normalize("tests/axes.obj", runner->getContainer()->getResourceManager()->getRootFolder()));
		Resource *resource = resourceAdapter->load(request.acceptMimeType(MimeTypes::GEOMETRYCOLLECTION));
		assertNotNull(defaultAssertMessage, resource);
	}

	/*
	 * Note that resource Adapters are owned by resource manager, thus we should send a new() and not a heap variable.
	 * Otherwise we get crazy things like accessing deleted memory in logs appenders.
	 */
	void testLoadWithResourceManager(PlaygroundRunner *runner) {
		ResourceManagerMock resourceManager(runner->getContainer()->getResourceManager()->getRootFolder());

		resourceManager.addAdapter(std::unique_ptr<ResourceAdapter>(new ObjResourceAdapter()));

		Resource *resource = resourceManager.load("tests/axes.obj", MimeTypes::GEOMETRYCOLLECTION);
		assertNotNull(defaultAssertMessage, resource);
		assertEquals(defaultAssertMessage, 4, resourceManager.getResourcesCount(MimeTypes::GEOMETRY));
		assertEquals(defaultAssertMessage, 1, resourceManager.getResourcesCount(MimeTypes::GEOMETRYCOLLECTION));
	}
};
