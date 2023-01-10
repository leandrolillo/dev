#pragma once

#include "ResourceManager.h"

class ResourceManager;

class ResourceLoadResponse {
private:
	Logger *logger = LoggerFactory::getLogger("ResourceLoadResponse");
	ResourceLoadRequest &request;
	ResourceManager &resourceManager;

public:
	ResourceLoadResponse(ResourceLoadRequest &request, ResourceManager &resourceManager) : request(request), resourceManager(resourceManager) {
	}

	virtual Resource *addResource(Resource *resource);

	virtual ~ResourceLoadResponse() {

	}
};
