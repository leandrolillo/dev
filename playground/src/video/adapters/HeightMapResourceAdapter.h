/*
 * HeightMapResourceAdapter.h
 *
 *  Created on: Apr 2, 2021
 *      Author: leandro
 */

#ifndef SRC_OPENGL_ADAPTERS_HEIGHTMAPRESOURCEADAPTER_H_
#define SRC_OPENGL_ADAPTERS_HEIGHTMAPRESOURCEADAPTER_H_

#include <ResourceAdapter.h>
#include<resources/ImageResource.h>
#include<Math3d.h>
#include "../resources/HeightMapResource.h"

class HeightMapResourceAdapter : public ResourceAdapter {
private:
    real boxSize = 1.0;
public:
    HeightMapResourceAdapter() {
        logger = LoggerFactory::getLogger("video/HeightMapResourceAdapter");
        this->addSupportedMimeType("video/heightmap");
    }

    virtual Resource* load(FileParser &fileParser, const String &mimeType) {
        ImageResource *image = (ImageResource *)this->getResourceManager()->load(fileParser);
        HeightMapResource *resource = new HeightMapResource(image, vector(1, 20, 1));
        return resource;
    }
private:



};



#endif /* SRC_OPENGL_ADAPTERS_HEIGHTMAPRESOURCEADAPTER_H_ */