/*
 * ResourceManager.cpp
 *
 *  Created on: Aug 15, 2022
 *      Author: leandro
 */
#include "ResourceManager.h"

const String ResourceManager::EphemeralLabel = {"ephemeral"};

void ResourceManager::addAdapter(ResourceAdapter *adapter) {
	bool found = false;
	for (auto &resourceAdapterPtr : this->resourceAdapters) {
		if(resourceAdapterPtr.get() == adapter) {
			found = true;
			break;
		}
	}

	if(!found) {
		resourceAdapters.insert(std::unique_ptr<ResourceAdapter>(adapter));
	}

	for (auto &currentMimeTypeIterator : adapter->getSupportedMimeTypes()) {
		adaptersCache[currentMimeTypeIterator] = adapter;
	}

	adapter->setResourceManager(this);
}

Resource* ResourceManager::load(FileParser &fileParser, const String &mimeType, std::set<String> labels) {
	Resource *cached = null;
	if(!mimeType.empty()) {
        try {
            cached = getCacheResource(fileParser.getFilename(), mimeType);
            if (cached == null) {
                logger->verbose("Resource was not cached previously");
                Resource *response = null;

                auto adaptersPair = adaptersCache.find(mimeType);
                if (adaptersPair != adaptersCache.end()) {
                    logger->debug("Loading [%s] [%s] with adapter [%s]",
                            mimeType.c_str(), fileParser.getFilename().c_str(),
                            adaptersPair->second->toString().c_str());

                    response = adaptersPair->second->load(fileParser, mimeType);
                    if (response != null) {
                        response->setFileName(fileParser.getFilename());
                        response->setMimeType(mimeType);
                        response->setLabels(labels);
                        addResource(response);
                        logger->debug("Loaded [%s]", response->toString().c_str());
                    } else {
                        logger->warn(
                                "Could not load [%s] [%s] with adapter [%s]",
                                mimeType.c_str(),
                                fileParser.getFilename().c_str(),
                                adaptersPair->second->toString().c_str());
                    }
                } else {
                    logger->error(
                            "No adapter found for mimetype [%s] - file not loaded [%s]",
                            mimeType.c_str(), fileParser.getFilename().c_str());
                }

                return response;
            } else {
                logger->debug("Getting [%s] [%s] from cache", mimeType.c_str(),
                        fileParser.getFilename().c_str());
            }
        } catch (Exception &e) {
            logger->error("Error loading resource [%s] [%s]: [%s]",
                    mimeType.c_str(), fileParser.getFilename().c_str(),
                    e.getMessage().c_str());
        }
	} else {
	    logger->error("Error loading resource [%s]: Could not determine mimetype", fileParser.getFilename().c_str());
	}

	return cached;
}

void ResourceManager::dispose(Resource *resource) const {
	if(resource != null && resource->getMimeType() != "") {
		logger->info("Disposing of resource [%s]", resource->toString().c_str());

		auto adapterIterator = adaptersCache.find(resource->getMimeType());
		if(adapterIterator != adaptersCache.end()) {
			adapterIterator->second->dispose(resource);
		}
	} else {
		logger->warn("Skipping resource disposal due to missing required information (resource not null and mimetype not empty)");
	}

}

ResourceManager::~ResourceManager() {
    logger->debug("Shutting down resource manager");

    /**
     * We could not move dispose logic to Resource destructor since it conflicts with the destructor order.
     * As coded, adapter.dispose(resource) was being called in Resource::~Resource() - The issue was dispose usually needs to access member properties
     * as part of cleanup, but those had already been deleted by the subclass destructor.
     *
     */
	for(const auto &[key, resource] : resourceCache) {
		logger->info("Disposing of resource [%s]", key.c_str());
		if(resource != null) {
			dispose(resource.get());
		}
	}

	logger->debug("Disposing of %d resources", resourceCache.size());
	resourceCache.clear(); // Not really required, just used to show logs for troubleshooting memory exceptions

	logger->debug("Disposing of %d resource adapters", resourceAdapters.size());
	resourceAdapters.clear(); // Not really required, just used to show logs for troubleshooting memory exceptions. This has to happen after resources are deleted, but that is managed by the member definition order.
	adaptersCache.clear();

	logger->info("Resource manager shutdown complete");
}
