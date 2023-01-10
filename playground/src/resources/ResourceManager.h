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


struct resourceAdapterComparator
{
    using is_transparent = void;
    bool operator()(std::unique_ptr<ResourceAdapter> const& left, std::unique_ptr<ResourceAdapter> const& right) const
    {
        return left.get() < right.get();
    }
    bool operator()(ResourceAdapter *left, std::unique_ptr<ResourceAdapter> const& right) const
    {
        return left < right.get();
    }
    bool operator()(std::unique_ptr<ResourceAdapter>const& left, ResourceAdapter *right) const
    {
        return left.get() < right;
    }
};


struct resourceComparator
{
    using is_transparent = void;
    bool operator()(std::unique_ptr<Resource> const& left, std::unique_ptr<Resource> const& right) const
    {
        return left.get() < right.get();
    }
    bool operator()(Resource *left, std::unique_ptr<Resource> const& right) const
    {
        return left < right.get();
    }
    bool operator()(std::unique_ptr<Resource>const& left, Resource *right) const
    {
        return left.get() < right;
    }
};


class ResourceManager {
protected:
	Logger *logger = LoggerFactory::getLogger("resources/ResourceManager");

	std::set<std::unique_ptr<ResourceAdapter>, resourceAdapterComparator> resourceAdapters; // Define adapter before resources so that they are initialized before them, and deleted after them.
	std::set<std::unique_ptr<Resource>, resourceComparator> resources; // Define adapter before resources so that they are initialized before them, and deleted after them.

	std::unordered_map<String, Resource *> resourceCache;
	std::unordered_map<String, ResourceAdapter *> loadAdaptersCache;
	std::unordered_map<String, ResourceAdapter *> disposeAdaptersCache;

	String rootFolder;
public:
	static const String EphemeralLabel;

public:
	ResourceManager (const ResourceManager&) = delete;
	ResourceManager& operator= (const ResourceManager&) = delete;

	ResourceManager(const String &rootFolder) {
		logger->setLogLevel(LogLevel::DEBUG);
		this->rootFolder = rootFolder;
	}

	void logStatus() {
		logger->debug("Resource Manager status:");
		logger->debug("Resources %d", resourceCache.size());
		logger->debug("Resource adapters: %d", resourceAdapters.size());
	}

	const String& getRootFolder() {
		return this->rootFolder;
	}

	ResourceAdapter *addAdapter(std::unique_ptr<ResourceAdapter> adapter);

	/**
	 * Find Adapter matching output MimeType and input mimetype - give priority to those adapters with empty input mimecard (kind of a wildcard)
	 */
	ResourceAdapter *getAdapter(ResourceLoadRequest &request) const {
		String key = request.getOutputMimeType() + "|";

		auto adaptersPair = loadAdaptersCache.find(request.getOutputMimeType() + "|");
		if(adaptersPair != loadAdaptersCache.end()) {
			return adaptersPair->second;
		}

		key = request.getOutputMimeType() + "|" + request.getInputMimeType();

		adaptersPair = loadAdaptersCache.find(request.getOutputMimeType() + "|" + request.getInputMimeType());
		return adaptersPair == loadAdaptersCache.end() ? null : adaptersPair->second;
	}

	Resource* load(const String &fileName) {
		logger->debug("Load [%s]", fileName.c_str());

		ResourceLoadRequest request(Paths::normalize(fileName, this->rootFolder));
		return this->load(request);
	}

	Resource* load(std::shared_ptr<FileParser> &fileParser, const String &outputMimeType, std::set<String> labels = {}) {
		logger->debug("Load [%s] [%s]", outputMimeType.c_str(), fileParser.get()->getFilename().c_str());

		ResourceLoadRequest request(fileParser);
		return load(request.acceptMimeType(outputMimeType).withLabels(labels));
	}

	Resource* load(const String &fileName, const String &outputMimeType, std::set<String> labels = {}, std::map<String, String> options = {}) {
		logger->debug("Load [%s] [%s]", outputMimeType.c_str(), fileName.c_str(), fileName.c_str());

		ResourceLoadRequest request(Paths::normalize(fileName, this->rootFolder));
		return load(request.acceptMimeType(outputMimeType).withLabels(labels).withOptions(options));
	}

	/**
	 * Loads a file using the parent file path as base for relative paths - TODO: Replace by load with parent resource
	 */
	Resource* load(const String &parentFilePath, const String &fileName, const String &outputMimeType) {
		logger->debug("Load [%s] [%s] relative to [%s]", outputMimeType.c_str(), fileName.c_str(), parentFilePath.c_str());

		ResourceLoadRequest request(Paths::relative(parentFilePath, fileName, this->rootFolder));
		return load(request.acceptMimeType(outputMimeType));
	}

	Resource* load(ResourceLoadRequest &loadRequest);

	Resource *addResource(Resource *resource) {
		if(resource != null) {
			String key = getCacheKey(*resource);
			if(!key.empty()) {
				if(resources.find(resource) == resources.end()) {
					resources.insert(std::unique_ptr<Resource>(resource));
				}
				resourceCache[key] = resource;
			}
		}

		return resource;
	}

	std::set<std::unique_ptr<Resource>>::iterator unload(Resource *resource) {
		if(resource != null) {
			auto iterator = std::find_if(
			    resources.begin(), resources.end(),
			    [&resource](const std::unique_ptr<Resource>& resourceUniquePtr) { return resourceUniquePtr.get() == resource; });

			if(iterator != resources.end()) {
				this->dispose(resource);
				this->resourceCache.erase(getCacheKey(*resource));
				return resources.erase(iterator);
			}
		}

		return resources.end();
	}

	void unload(String label) {
		auto iterator = std::find_if(
		    resources.begin(), resources.end(),
		    [&label](const std::unique_ptr<Resource>& resourceUniquePtr) {
				return resourceUniquePtr.get() != null && resourceUniquePtr->getLabels().find(label) != resourceUniquePtr->getLabels().end();
			});

		if(iterator != resources.end()) {
			this->dispose(iterator->get());
			this->resourceCache.erase(getCacheKey(*iterator->get()));
			resources.erase(iterator);
		}
	}

	void unload(std::set<String>labels) {
		std::set<std::unique_ptr<Resource>>::iterator iterator = resources.begin();

		while (iterator != resources.end()) {
			bool hasAllLabels = true;
			if(*iterator) {
				for(auto &label : labels) {
					const std::set<String> labels = iterator->get()->getLabels();
					if(labels.find(label) == labels.end()) {
						hasAllLabels = false;
						break;
					}
				}
			}

			if(hasAllLabels) {
				iterator = this->unload(iterator->get());
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
		if(filename.empty() || mimeType.empty()) {
			throw InvalidArgumentException("Can not get cache key from empty values - filename: [%s] mimeType[%s]", filename.c_str(), mimeType.c_str());
		}

		return Paths::normalize(filename, this->rootFolder) + "|" + mimeType;
	}

	const String getCacheKey(const Resource &resource) {
		return getCacheKey(resource.getUri(), resource.getMimeType());
	}

	const String getCacheKey(const ResourceLoadRequest &resourceLoadRequest) {
		return getCacheKey(resourceLoadRequest.getUri(), resourceLoadRequest.getOutputMimeType());
	}

	Resource *getCacheResource(const String &cacheKey) {
		auto pair = resourceCache.find(cacheKey);

		if(pair != resourceCache.end()) {
			return pair->second;
		}

		return null;
	}
};

#endif /* RESOURCEMANAGER_H_ */
