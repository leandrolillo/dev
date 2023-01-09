/*
 * ResourceManager.cpp
 *
 *  Created on: Aug 15, 2022
 *      Author: leandro
 */
#include "ResourceManager.h"

const String ResourceManager::EphemeralLabel = {"ephemeral"};

ResourceAdapter * ResourceManager::addAdapter(std::unique_ptr<ResourceAdapter> adapterUniquePtr) { //should this accept a unique_ptr and move it?
	logger->info("Adding adapter");
	if(adapterUniquePtr.get() != null && adapterUniquePtr->isValid()) {
		logger->info("Adapter [%s] is valid - proceeding to add it", adapterUniquePtr->toString().c_str());

		ResourceAdapter * adapter = adapterUniquePtr.get();
		if(resourceAdapters.find(adapter) == resourceAdapters.end()) {
			/* std::move makes this an l-value so it behaves like it is a temporary variable - it makes adapterUniquePtr variable point to null so it should not be referenced after this. */
			resourceAdapters.insert(std::move(adapterUniquePtr));
			adapter->setResourceManager(this);

			logger->info("Adapter [%s] added to set with resourceManager [%d]", adapter->toString().c_str(), adapter->getResourceManager());
		} else {
			logger->warn("Skipping adapter [%s] - already managed", adapter->toString().c_str());
		}

		for(auto &outputMimeType : adapter->getOutputMimeTypes()) {
			String key = adapter->getInputMimeType().empty() ? outputMimeType  + "|" : outputMimeType + "|" + adapter->getInputMimeType();
			loadAdaptersCache[key] = adapter;

			logger->info("Adapter [%s] added to manage [%s] with key [%s]", adapter->toString().c_str(), outputMimeType.c_str(), key.c_str());
		}

		return adapter;
	}

	logger->error("NOT adding invalid adapter [%s]: [%s] ",
			adapterUniquePtr.get() == null ? "null" : adapterUniquePtr->toString().c_str(),
			adapterUniquePtr.get() == null ? "N/A" : adapterUniquePtr->errors().c_str());

	return null;
}

Resource* ResourceManager::load(ResourceLoadRequest &resourceLoadRequest) {
	Resource *response = null;
	try {
		response = getCacheResource(getCacheKey(resourceLoadRequest));
		if (response == null) {
			if(resourceLoadRequest.isValid()) {
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
						if (response != null) { // TODO: add a response object that takes care of all this?
							if(response->getFileName() == "") {
								response->setFileName(resourceLoadRequest.getFilePath());
							}

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
				addResource(response);
			} else {
				logger->error("Invalid Resource load request %s: %s ", resourceLoadRequest.toString().c_str(), resourceLoadRequest.errors().c_str());
			}
		} else {
			logger->debug("Getting %s from cache", resourceLoadRequest.toString().c_str());
		}
	} catch (Exception &e) {
		logger->error("Error loading [%s]: [%s]",
				resourceLoadRequest.toString().c_str(),
				e.getMessage().c_str());
	}

	return response;
}

/**
 * Disposes of the resource by calling the appropriate adapter dispose method. This frees allocated resources but does not remove the resource from internal caches - caller needs to do so as it would break iteration logic.
 */
void ResourceManager::dispose(Resource *resource) const {
	if(resource != null && resource->getMimeType() != "") {
		logger->info("Disposing of resource [%s]", resource->toString().c_str());

		auto adapterIterator = disposeAdaptersCache.find(resource->getMimeType());
		if(adapterIterator != disposeAdaptersCache.end()) {
			adapterIterator->second->dispose(resource);
		}

	} else {
		logger->warn("Skipping resource disposal due to missing required information (resource not null and mimetype not empty)");
	}
}

ResourceManager::~ResourceManager() {
    logger->debug("Shutting down resource manager");
    logStatus();

    /**
     * We could not move dispose logic to Resource destructor since it conflicts with the destructor order.
     * As coded, adapter.dispose(resource) was being called in Resource::~Resource() - The issue was dispose usually needs to access member properties
     * as part of cleanup, but those had already been deleted by the subclass destructor.
     *
     */
	for(const auto &resource : resources) {
		if(resource != null) {
			logger->info("Disposing of resource [%s]", resource.get()->toString().c_str());
			dispose(resource.get());
		}
	}

//	logger->debug("Disposing of %d resources", resourceCache.size());
//	resourceCache.clear(); // Not really required, just used to show logs for troubleshooting memory exceptions
//
//	logger->debug("Disposing of %d resource adapters", resourceAdapters.size());
//	resourceAdapters.clear(); // Not really required, just used to show logs for troubleshooting memory exceptions. This has to happen after resources are deleted, but that is managed by the member definition order.
//	adaptersCache.clear();

	logger->info("Resource manager shutdown complete");
}
