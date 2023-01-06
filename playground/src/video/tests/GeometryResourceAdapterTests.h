/*
 * GeometryResourceAdapterTests.h
 *
 *  Created on: Oct 25, 2022
 *      Author: leandro
 */

#ifndef SRC_VIDEO_TESTS_GEOMETRYRESOURCEADAPTERTESTS_H_
#define SRC_VIDEO_TESTS_GEOMETRYRESOURCEADAPTERTESTS_H_

#include <Tests.h>
#include <ResourceManager.h>
#include <GeometryResourceAdapter.h>

class GeometryResourceAdapterTests : public UnitTest
{
public:
	GeometryResourceAdapterTests() {
        logger = LoggerFactory::getLogger("GeometryResourceAdapterTests");
        logger->addAppender(LoggerFactory::getAppender("stdout"));

        this->addTest("GeometryResourceAdapterTests::testLoad", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&GeometryResourceAdapterTests::testLoad));
        this->addTest("GeometryResourceAdapterTests::testLoadWithResourceManager", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&GeometryResourceAdapterTests::testLoadWithResourceManager));
    }


	/**
	 * Needs to provide a resource manager since the adapter might make usage of it.
	 */
	void testLoad(PlaygroundRunner *runner){
		ResourceManager resourceManager(runner->getContainer()->getResourceManager()->getRootFolder());
		GeometryResourceAdapter *resourceAdapter = new GeometryResourceAdapter();

		resourceManager.addAdapter(resourceAdapter);

		ResourceLoadRequest request(Paths::normalize("tests/geometry.json", runner->getContainer()->getResourceManager()->getRootFolder()));
		Resource *resource = resourceAdapter->load(request.acceptMimeType(MimeTypes::GEOMETRYCOLLECTION));
		assertNotNull(defaultAssertMessage, resource);
	}

	/*
	 * Note that resource Adapters are owned by resource manager, thus we should send a new() and not a heap variable.
	 * Otherwise we get crazy things like accessing deleted memory in logs appenders.
	 */
	void testLoadWithResourceManager(PlaygroundRunner *runner) {
		ResourceManager resourceManager(runner->getContainer()->getResourceManager()->getRootFolder());

		resourceManager.addAdapter(new GeometryResourceAdapter());

		Resource *resource = resourceManager.load("tests/geometry.json", MimeTypes::GEOMETRYCOLLECTION);
		assertNotNull(defaultAssertMessage, resource);
	}
};




#endif /* SRC_VIDEO_TESTS_GEOMETRYRESOURCEADAPTERTESTS_H_ */
