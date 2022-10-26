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


	void testLoad(PlaygroundRunner *runner){
		GeometryResourceAdapter resourceAdapter;

		FileParser fileParser(Paths::normalize("tests/geometry.json", runner->getContainer()->getResourceManager()->getRootFolder()));
		ResourceLoadRequest request(fileParser);
		Resource *resource = resourceAdapter.load(request.acceptMimeType(MimeTypes::GEOMETRYCOLLECTION));
		assertNotNull(defaultAssertMessage, resource);
	}

	void testLoadWithResourceManager(PlaygroundRunner *runner) {
		ResourceManager resourceManager(runner->getContainer()->getResourceManager()->getRootFolder());
		GeometryResourceAdapter resourceAdapter;

		resourceManager.addAdapter(&resourceAdapter);

		Resource *resource = resourceManager.load("tests/geometry.json", MimeTypes::GEOMETRYCOLLECTION);
		assertNotNull(defaultAssertMessage, resource);
	}
};




#endif /* SRC_VIDEO_TESTS_GEOMETRYRESOURCEADAPTERTESTS_H_ */
