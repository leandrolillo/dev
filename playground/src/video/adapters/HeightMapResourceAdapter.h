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
public:
    HeightMapResourceAdapter() {
        logger = LoggerFactory::getLogger("video/HeightMapResourceAdapter");
        this->addSupportedMimeType("video/heightmap");
    }

    virtual Resource *load(FileParser &fileParser, const String &mimeType) const override {
        ImageResource *image = (ImageResource *)this->getResourceManager()->load(fileParser);
        HeightMapResource *resource = new HeightMapResource(image, vector(2, 80, 2));
        return resource;
    }
private:



};



#endif /* SRC_OPENGL_ADAPTERS_HEIGHTMAPRESOURCEADAPTER_H_ */
