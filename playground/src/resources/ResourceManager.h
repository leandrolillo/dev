#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include "ResourceLoadRequest.h"
#include "FileParser.h"
#include "ResourceAdapter.h"
#include "Resource.h"
#include "Paths.h"
#include <Logger.h>
#include <Exception.h>
#include <set>
#include <vector>
#include <unordered_map>
#include <stdio.h>
#include <algorithm>

class ResourceManager {
private:
	Logger *logger = LoggerFactory::getLogger("resources/ResourceManager");
	std::set<std::unique_ptr<ResourceAdapter>> resourceAdapters; // Define adapter before resources so that they are initialized before them, and deleted after them.
	std::unordered_map<String, std::unique_ptr<Resource>> resourceCache;
	std::unordered_map<String, ResourceAdapter *> adaptersCache;

	String rootFolder;
public:
	static const String EphemeralLabel;

public:
	ResourceManager(const String &rootFolder) {
		logger->setLogLevel(LogLevel::DEBUG);
		this->rootFolder = rootFolder;
	}

	const String& getRootFolder() {
		return this->rootFolder;
	}

	void addAdapter(ResourceAdapter *adapter);

	/**
	 * Find Adapter matching output MimeType and input mimetype - give priority to those adapters with empty input mimecard (kind of a wildcard)
	 */
	ResourceAdapter *getAdapter(ResourceLoadRequest &request) const {
		String key = request.getOutputMimeType() + "|";

		auto adaptersPair = adaptersCache.find(request.getOutputMimeType() + "|");
		if(adaptersPair != adaptersCache.end()) {
			return adaptersPair->second;
		}

		key = request.getOutputMimeType() + "|" + request.getInputMimeType();

		adaptersPair = adaptersCache.find(request.getOutputMimeType() + "|" + request.getInputMimeType());
		return adaptersPair == adaptersCache.end() ? null : adaptersPair->second;
	}

	Resource* load(const String &fileName) {
		logger->debug("Load [%s]", fileName.c_str());

		FileParser fileParser(Paths::normalize(fileName, this->rootFolder));
		ResourceLoadRequest request(fileParser);
		return this->load(request);
	}

	Resource* load(FileParser &fileParser, const String &outputMimeType, std::set<String> labels = {}) {
		logger->debug("Load [%s] [%s]", outputMimeType.c_str(), fileParser.getFilename().c_str());

		ResourceLoadRequest request(fileParser);
		request.acceptMimeType(outputMimeType).withLabels(labels);

		return load(request);
	}

	Resource* load(const String &fileName, const String &outputMimeType, std::set<String> labels = {}) {
		logger->debug("Load [%s] [%s]", outputMimeType.c_str(), fileName.c_str(), fileName.c_str());

		FileParser fileParser(Paths::normalize(fileName, this->rootFolder));
		ResourceLoadRequest request(fileParser);
		request.acceptMimeType(outputMimeType).withLabels(labels);
		return load(request);
	}

	/**
	 * Loads a file using the parent file path as base for relative paths - TODO: Replace by load with parent resource
	 */
	Resource* load(const String &parentFilePath, const String &fileName, const String &outputMimeType) {
		logger->debug("Load [%s] [%s] relative to [%s]", outputMimeType.c_str(), fileName.c_str(), parentFilePath.c_str());

		FileParser fileParser(Paths::relative(parentFilePath, fileName, this->rootFolder));
		ResourceLoadRequest request(fileParser);
		request.acceptMimeType(outputMimeType);
		return load(request);
	}

	Resource* load(ResourceLoadRequest &loadRequest);

	/**
	 * Beware with adding a key that already exists: it will trigger deletion of the unique_ptr and exceptions
	 */
	Resource *addResource(const String &key, Resource *resource) {
		resourceCache[key] = std::unique_ptr<Resource>(resource);
		return resource;
	}

	Resource *addResource(Resource *resource) {
		String key = getCacheKey(resource);
		if(!key.empty()) {
			resourceCache[key] = std::unique_ptr<Resource>(resource);
		}
	    return resource;
	}

	void unload(Resource *resource) {
		if(resource != null) {
			this->dispose(resource);
			this->resourceCache.erase(getCacheKey(resource));
		}
	}

	void unload(String label) {
		auto iterator = resourceCache.begin();
		while (iterator != resourceCache.end()) {
			if(iterator->second.get() != null && iterator->second->getLabels().find(label) != iterator->second->getLabels().end()) {
				this->dispose(iterator->second.get());
				iterator = this->resourceCache.erase(iterator);
			} else {
		       ++iterator;
		    }
		}
	}

	void unload(std::vector<String>labels) {
		auto iterator = resourceCache.begin();
		while (iterator != resourceCache.end()) {
			bool hasAllLabels = true;
			for(auto &label : labels) {
				if(iterator->second->getLabels().find(label) == iterator->second->getLabels().end()) {
					hasAllLabels = false;
					break;
				}
			}

			if(hasAllLabels) {
				this->dispose(iterator->second.get());
				iterator = this->resourceCache.erase(iterator);
			} else {
		       ++iterator;
		    }
		}
	}

	void dispose(Resource *resource) const;
	~ResourceManager();

	String normalize(const String &filePath) const {
		return Paths::normalize(filePath, this->rootFolder);
	}

private:
	const String getCacheKey(const String &filename, const String &mimeType) const {
		return Paths::normalize(filename, this->rootFolder) + "|" + mimeType;
	}

	const String getCacheKey(Resource *resource) {
	    if(resource != null) {
	        return getCacheKey(resource->getFileName(), resource->getMimeType());
	    }

	    return "";
	}

	Resource *getCacheResource(const String &cacheKey) {
		auto pair = resourceCache.find(cacheKey);

		if(pair != resourceCache.end()) {
			return pair->second.get();
		}

		return null;
	}
};

#endif /* RESOURCEMANAGER_H_ */
