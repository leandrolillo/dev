/*
 * ResourceManager.cpp
 *
 *  Created on: Aug 15, 2022
 *      Author: leandro
 */
#include "ResourceManager.h"

void ResourceManager::addAdapter(ResourceAdapter *adapter) {
	std::vector<String> mimeTypes = adapter->getSupportedMimeTypes();

	bool found = false;
	for (auto &resourceAdapterPtr : this-> resourceAdapters) {
		if(resourceAdapterPtr.get() == adapter) {
			found = true;
			break;
		}
	}

	if(!found) {
		resourceAdapters.insert(std::unique_ptr<ResourceAdapter>(adapter));
	}

	for (auto &currentMimeTypeIterator : mimeTypes) {
		adaptersCache[currentMimeTypeIterator] = adapter;
	}

	adapter->setResourceManager(this);
}

Resource* ResourceManager::load(FileParser &fileParser, const String &mimeType) {
	Resource *cached = null;
	if(!mimeType.empty()) {
        try {
            cached = getCacheResource(fileParser.getFilename(), mimeType);
            if (cached == null) {
                logger->verbose("Resource was not cached previously");
                Resource *response = null;

                if (adaptersCache[mimeType] != null) {
                    logger->debug("Loading [%s] [%s] with adapter [%s]",
                            mimeType.c_str(), fileParser.getFilename().c_str(),
                            adaptersCache[mimeType]->toString().c_str());

                    response = adaptersCache[mimeType]->load(fileParser, mimeType);
                    if (response != null) {
                        response->setFileName(fileParser.getFilename());
                        response->setMimeType(mimeType);
                        response->setAdapter(adaptersCache[mimeType]);
                        addResource(response);
                        logger->debug("Loaded [%s]", response->toString().c_str());
                    } else {
                        logger->warn(
                                "Could not load [%s] [%s] with adapter [%s]",
                                mimeType.c_str(),
                                fileParser.getFilename().c_str(),
                                adaptersCache[mimeType]->toString().c_str());
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

ResourceManager::~ResourceManager() {
    logger->debug("Shutting down resource manager");
//	for (std::map<String, Resource*>::iterator currentResourceIterator = resourceCache.begin();
//			currentResourceIterator != resourceCache.end();
//			currentResourceIterator++) {
//		if ((*currentResourceIterator).second != null) {
//			delete (*currentResourceIterator).second;
//			logger->verbose("Done disposing resource");
//		}
//	}

	logger->debug("Disposing of %d resources", resourceCache.size());
	//resourceCache.clear();

	logger->debug("Disposing of %d resource adapters", adaptersCache.size());
//	std::set<ResourceAdapter*> resourceAdaptersSet;
//	for (std::map<String, ResourceAdapter*>::iterator currentAdapterIterator =
//			adapters.begin(); currentAdapterIterator != adapters.end();
//			currentAdapterIterator++) {
//	    logger->verbose("Scheduling for removal [%s]", currentAdapterIterator->second->toString().c_str());
//		resourceAdaptersSet.insert(currentAdapterIterator->second);
//	}
//
//	for (std::set<ResourceAdapter*>::iterator currentAdapterIterator =
//			resourceAdaptersSet.begin();
//			currentAdapterIterator != resourceAdaptersSet.end();
//			currentAdapterIterator++) {
//	    logger->verbose("Deleting adapter [%s]", (*currentAdapterIterator)->toString().c_str());
//		delete (*currentAdapterIterator);
//	}
//
	logger->debug("Disposing of resource adapters");

	//adapters.clear();

	logger->info("Resource manager shutdown complete");
}
