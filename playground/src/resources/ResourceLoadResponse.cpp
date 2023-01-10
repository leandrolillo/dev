/*
 * ResourceLoadResponse.cpp
 *
 *  Created on: Jan 9, 2023
 *      Author: leandro
 */
#include "ResourceLoadResponse.h"

Resource *ResourceLoadResponse::addResource(Resource *resource) {
	if(resource != null) {
		if(resource->getUri().empty()) {
			resource->setUri(request.getUri());
		}

		if(resource->getMimeType().empty()) {
			resource->setMimeType(request.getOutputMimeType());
		}

		resource->setLabels(request.getLabels());
		logger->debug("Loaded [%s]", resource->toString().c_str());

		return resourceManager.addResource(resource);
	}
}




