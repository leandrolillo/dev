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
        this->produces(MimeTypes::HEIGHTMAP);
    }

    virtual Resource *load(ResourceLoadRequest &request) const override {
    	ResourceLoadRequest imageRequest(request);
        ImageResource *image = (ImageResource *)this->getResourceManager()->load(imageRequest.acceptMimeType(MimeTypes::IMAGE));
        if(image == null) {
        	logger->error("Could not load image [%s] - skipping heightmap creation", request.getFilePath().c_str());
        }

        HeightMapResource *resource = new HeightMapResource(image, vector(2, 80, 2));
        return resource;
    }
private:



};



#endif /* SRC_OPENGL_ADAPTERS_HEIGHTMAPRESOURCEADAPTER_H_ */
