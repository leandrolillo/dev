#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include "Resource.h"
#include "FileParser.h"
#include "ResourceAdapter.h"
#include <Logger.h>
#include <Exception.h>
#include<set>
#include<vector>
#include<map>
#include <stdio.h>
#include <algorithm>

class ResourceManager {
private:
	Logger *logger;
	std::map<String, ResourceAdapter*> adapters;
	std::map<String, Resource*> resourceCache;
	String rootFolder;

public:
	ResourceManager(const String &rootFolder) {
		logger = Logger::getLogger("resources/ResourceManager.h");

		this->rootFolder = rootFolder;

	}

	const String& getRootFolder() {
		return this->rootFolder;
	}

	void addAdapter(ResourceAdapter *adapter) {
		std::vector<String> mimeTypes = adapter->getSupportedMimeTypes();

		for (std::vector<String>::iterator currentMimeTypeIterator =
				mimeTypes.begin(); currentMimeTypeIterator != mimeTypes.end();
				currentMimeTypeIterator++) {
			adapters[*currentMimeTypeIterator] = adapter;
		}

		adapter->setResourceManager(this);
	}

	Resource* load(String fileName) {
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
			String normalizedFileName = normalize(fileName);

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
	 * Loads a file using the parent file path as base for relative paths
	 */
	Resource* load(const String &parentFilePath, const String &fileName, const String &mimeType) {
		return load(this->addPaths(this->getFolderPath(parentFilePath), fileName), mimeType);
	}


	Resource* load(FileParser &fileParser, const String &mimeType) {
		Resource *cached = resourceCache[getCacheKey(fileParser.getFilename(), mimeType)];
		try {
			if (cached == null) {
				logger->verbose("Resource was not cached previously");
				Resource *response = null;

				if (adapters[mimeType] != null) {
					logger->debug("Loading [%s] [%s] with adapter [%s]",
							mimeType.c_str(), fileParser.getFilename().c_str(),
							adapters[mimeType]->toString().c_str());

					response = adapters[mimeType]->load(fileParser, mimeType);
					if (response != null) {
						response->setFileName(fileParser.getFilename());
						response->setMimeType(mimeType);
						addResource(getCacheKey(fileParser.getFilename(),mimeType), response);
						logger->info("Loaded [%s]", response->toString().c_str());
					} else {
						logger->warn(
								"Could not load [%s] [%s] with adapter [%s]",
								mimeType.c_str(),
								fileParser.getFilename().c_str(),
								adapters[mimeType]->toString().c_str());
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

		return cached;
	}

	Resource *addResource(const String &key, Resource *resource) {
	    resourceCache[key] = resource;
	    return resource;
	}

	void dispose(Resource *resource) {
		if (resource != null) {
			resourceCache.erase(resource->getFileName());
			adapters[resource->getMimeType()]->dispose(resource);
			delete resource;
		}
	}

	~ResourceManager() {
		for (std::map<String, Resource*>::iterator currentResourceIterator =
				resourceCache.begin();
				currentResourceIterator != resourceCache.end();
				currentResourceIterator++) {
			if ((*currentResourceIterator).second != null)
				logger->debug("Disposing of resource ['%s' - '%s']",
						(*currentResourceIterator).second->getMimeType().c_str(),
						(*currentResourceIterator).second->getFileName().c_str());

			dispose((*currentResourceIterator).second);
		}

		std::set<ResourceAdapter*> resourceAdaptersSet;
		for (std::map<String, ResourceAdapter*>::iterator currentAdapterIterator =
				adapters.begin(); currentAdapterIterator != adapters.end();
				currentAdapterIterator++) {
			resourceAdaptersSet.insert(currentAdapterIterator->second);
		}

		for (std::set<ResourceAdapter*>::iterator currentAdapterIterator =
				resourceAdaptersSet.begin();
				currentAdapterIterator != resourceAdaptersSet.end();
				currentAdapterIterator++) {
			delete (*currentAdapterIterator);
		}
		adapters.clear();
		resourceAdaptersSet.clear();
	}

	/**
	 * Return an absolute path from root folder and filename.
	 */
	String normalize(const String &fileName) const {
		if(fileName.substr(0, 2) == "~/") {
			return addPaths(rootFolder, fileName.substr(2, fileName.size() - 2));
		}
		return addPaths(rootFolder, fileName);
	}

	/**
	 * if the provided postfix is an absolute path, return that.
	 * if the provided postfix starts with ~/ then make it is relative to the repository and we return that as well.
	 * Otherwise concatenate the prefix + postfix and remove extra slashes.
	 */
	String addPaths(const String &prefix, const String &postFix) const {
		String normalizedPrefix = prefix.substr(this->rootFolder.length() - 1, 1) == "/" ? prefix : prefix + "/";
		String normalizedPostfix = postFix.substr(0, 2) == "./" ? postFix.substr(2, postFix.length() - 2) : postFix;

		return normalizedPostfix.substr(0, 1) == "/" || normalizedPostfix.substr(0, 2) == "~/" ?
				normalizedPostfix :
				normalizedPrefix + normalizedPostfix;
	}

	/**
	 * Return the folder part of a path to a file.
	 */
	const String getFolderPath(const String &filePath) const {
		unsigned long location = filePath.find_last_of('/');
		return (location == std::string::npos ? "" : filePath.substr(0, location + 1));

	}

private:

	const String getCacheKey(const String &filename,
			const String &mimeType) const {
		return normalize(filename) + "|" + mimeType;
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
			}
		}

		logger->error("Could not determine mimetype for [%s]",
				fileName.c_str());
		throw InvalidArgumentException("Could not determine mimetype for [%s]",
				fileName.c_str());
	}

};

#endif /* RESOURCEMANAGER_H_ */
