#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

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

	ResourceAdapter *getAdapter(const String &mimetype) {
		return adaptersCache[mimetype];
	}

	Resource* load(const String &fileName) {
		try {
			return load(fileName, guessMimeType(fileName));
		} catch (Exception &e) {
			logger->error("Error loading resource [%s]: [%s]", fileName.c_str(),
					e.getMessage().c_str());
		}

		return null;
	}

	Resource* load(FileParser &fileParser) {
		try {
			return load(fileParser, guessMimeType(fileParser.getFilename()));
		} catch (Exception &e) {
			logger->error("Error loading resource [%s]: [%s]",
					fileParser.getFilename().c_str(), e.getMessage().c_str());
		}
		return null;
	}

	Resource* load(const String &fileName, const String &mimeType) {
		try {
			String normalizedFileName = Paths::normalize(fileName, this->rootFolder);

			if (!normalizedFileName.empty()) {
				FileParser fileParser = FileParser(normalizedFileName);
				return load(fileParser, mimeType);
			}

			return null;
		} catch (Exception &e) {
			logger->error("Error loading resource [%s] [%s]: [%s]",
					mimeType.c_str(), fileName.c_str(), e.getMessage().c_str());
		}
		return null;
	}

	/**
	 * Loads a file using the parent file path as base for relative paths - TODO: Replace by load with parent resource
	 */
	Resource* load(const String &parentFilePath, const String &fileName, const String &mimeType) {
		return load(Paths::add(Paths::getDirname(parentFilePath), fileName), mimeType);
	}

	Resource *addResource(Resource *resource) {
		if(resource != null) {
			resourceCache[getCacheKey(resource)] = std::unique_ptr<Resource>(resource);
		}
	    return resource;
	}

	void unload(Resource *resource) {
		this->dispose(resource);
		this->resourceCache.erase(getCacheKey(resource));
	}

	void unload(String label) {
		auto iterator = resourceCache.begin();
		while (iterator != resourceCache.end()) {
			if(iterator->second->getLabels().find(label) != iterator->second->getLabels().end()) {
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

	Resource* load(FileParser &fileParser, const String &mimeType);
	void dispose(Resource *resource) const;
	~ResourceManager();

	String normalize(const String &filePath) const {
		return Paths::normalize(filePath, this->rootFolder);
	}

    String guessMimeType(const String &fileName) const {
        int position = fileName.find_last_of(".");
        if (position > 0) {
            String extension = fileName.substr(position + 1,
                    fileName.length() - position - 1);
            std::transform(extension.begin(), extension.end(),
                    extension.begin(), ::tolower);
            if (extension == "ogg") {
                logger->verbose("guessMimeType: mime type for [%s] is [%s]",
                        fileName.c_str(), "audio/ogg");
                return "audio/ogg";
            } else if (extension == "wav") {
                logger->verbose("guessMimeType: mime type for [%s] is [%s]",
                        fileName.c_str(), "audio/wav");
                return "audio/wav";
            } else if (extension == "jpeg" || extension == "jpg") {
                logger->verbose("guessMimeType: mime type for [%s] is [%s]",
                        fileName.c_str(), "image/jpeg");
                return "image/jpeg";
            } else if (extension == "png") {
                logger->verbose("guessMimeType: mime type for [%s] is [%s]",
                        fileName.c_str(), "image/png");
                return "image/png";
            } else if (extension == "tga") {
                logger->verbose("guessMimeType: mime type for [%s] is [%s]",
                        fileName.c_str(), "image/tga");
                return "image/tga";
            } else if (extension == "obj") {
                logger->verbose("guessMimeType: mime type for [%s] is [%s]",
                        fileName.c_str(), "video/obj");
                return "video/obj";
            }
        }


        return "";

//        logger->error("Could not determine mimetype for [%s]",
//                fileName.c_str());
//        throw InvalidArgumentException("Could not determine mimetype for [%s]",
//                fileName.c_str());
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

	Resource *getCacheResource(const String &filename, const String &mimeType) {
		String cacheKey = getCacheKey(filename, mimeType);

		auto pair = resourceCache.find(cacheKey);

		if(pair != resourceCache.end()) {
			return pair->second.get();
		}

		return null;
	}
};

#endif /* RESOURCEMANAGER_H_ */
