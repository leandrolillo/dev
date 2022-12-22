/*
 * ResourceManager.cpp
 *
 *  Created on: Aug 15, 2022
 *      Author: leandro
 */
#include "ResourceManager.h"

const String ResourceManager::EphemeralLabel = {"ephemeral"};

void ResourceManager::addAdapter(ResourceAdapter *adapter) {
	if(adapter != null && !adapter->getOutputMimeType().empty()) {
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

		String key = adapter->getInputMimeType().empty() ? adapter->getOutputMimeType() + "|" : adapter->getOutputMimeType() + "|" + adapter->getInputMimeType();

		adaptersCache[key] = adapter;

		adapter->setResourceManager(this);
	} else {
		logger->error("NOT adding invalid adapter [%s] - either null or empty output mime type", adapter == null ? "null" : adapter->toString().c_str());
	}
}

Resource* ResourceManager::load(ResourceLoadRequest &resourceLoadRequest) {
	Resource *response = null;
	String key = getCacheKey(resourceLoadRequest.getFileParser().getFilename(), resourceLoadRequest.getOutputMimeType());

	if(resourceLoadRequest.isValid()) {
		response = getCacheResource(key);
		if (response == null) {
			try {
				logger->verbose("Resource was not cached previously");

				ResourceAdapter *adapter = getAdapter(resourceLoadRequest);
				if (adapter != null) {
					logger->debug("Loading %s with adapter [%s]",
							resourceLoadRequest.toString().c_str(),
							adapter->toString().c_str());

					try {
						response = adapter->load(resourceLoadRequest);
					} catch(Exception &exception) {
						logger->error("Could not load %s with adapter [%s]: %s", resourceLoadRequest.toString().c_str(), adapter->toString().c_str(), exception.getMessage().c_str());
					} catch(...) {
						;
					}
					if (response != null) {
						response->setFileName(resourceLoadRequest.getFileParser().getFilename());
						response->setMimeType(resourceLoadRequest.getOutputMimeType());
						response->setLabels(resourceLoadRequest.getLabels());
						logger->debug("Loaded [%s]", response->toString().c_str());
					} else {
						logger->warn(
								"Could not load %s with adapter [%s]",
								resourceLoadRequest.toString().c_str(),
								adapter->toString().c_str());
					}
				} else {
					logger->error("No adapter found for loading %s", resourceLoadRequest.toString().c_str());
				}
			} catch (Exception &e) {
				logger->error("Error loading [%s]: [%s]",
						resourceLoadRequest.toString().c_str(),
						e.getMessage().c_str());
			}

			addResource(key, response);

		} else {
			logger->debug("Getting %s from cache", resourceLoadRequest.toString().c_str());
		}
	} else {
	    logger->error("Invalid Resource load request %s: %s ", resourceLoadRequest.toString().c_str(), resourceLoadRequest.errors().c_str());
	}
	return response;
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
//	resourceAdapters.clear(); // Not really required, just used to show logs for troubleshooting memory exceptions. This has to happen after resources are deleted, but that is managed by the member definition order.
//	adaptersCache.clear();

	logger->info("Resource manager shutdown complete");
}
