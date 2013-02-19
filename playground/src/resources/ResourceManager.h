#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include "Resource.h"
#include "FileParser.h"
#include "ResourceAdapter.h"
#include "log/Logger.h"
#include "exceptions/Exception.h"
#include<set>
#include<vector>
#include<map>
#include <stdio.h>
#include <algorithm>

class ResourceManager {
	private:
		Logger *logger;
		std::map<String, ResourceAdapter *> adapters;
		std::map<String, Resource *> resourceCache;
		String rootFolder;

		public:
		ResourceManager(const String rootFolder)
		{
			logger = Logger::getLogger("resources/ResourceManager.h");
			this->rootFolder = rootFolder;
		}

		void addAdapter(ResourceAdapter *adapter)
		{
			std::vector<String> mimeTypes = adapter->getSupportedMimeTypes();

			for (std::vector<String>::iterator currentMimeTypeIterator =
					mimeTypes.begin(); currentMimeTypeIterator != mimeTypes.end();
					currentMimeTypeIterator++) {
				adapters[*currentMimeTypeIterator] = adapter;
			}

			adapter->setResourceManager(this);
		}

		Resource *load(String fileName)
		{
			try {
				return load(fileName, guessMimeType(fileName));
			} catch (Exception &e) {
				logger->error("Error loading resource [%s]: [%s]", fileName.c_str(), e.getMessage().c_str());
			}

			return null;
		}

		Resource *load(FileParser &fileParser)
		{
			try {
				return load(fileParser, guessMimeType(fileParser.getFilename()));
			} catch (Exception &e) {
				logger->error("Error loading resource [%s]: [%s]", fileParser.getFilename().c_str(), e.getMessage().c_str());
			}
			return null;
		}

		Resource *load(String fileName, const String &mimeType)
		{
			try {
				FileParser fileParser = FileParser(normalize(fileName));
				return load(fileParser, mimeType);
			} catch (Exception &e) {
				logger->error("Error loading resource [%s] [%s]: [%s]", mimeType.c_str(), fileName.c_str(), e.getMessage().c_str());
			}
			return null;
		}

		Resource *load(FileParser &fileParser, const String &mimeType)
		{
				Resource *cached = resourceCache[getCacheKey(fileParser.getFilename(), mimeType)];
				try {

					if(cached == null)
					{
						Resource *response = null;

						if(adapters[mimeType] != null) {
							logger->debug("Loading [%s] [%s]", mimeType.c_str(), fileParser.getFilename().c_str());

								response = adapters[mimeType]->load(fileParser);
								if(response != null) {
									response->setFileName(fileParser.getFilename());
									resourceCache[getCacheKey(fileParser.getFilename(), mimeType)] = response;
								}
						} else
							logger->error("No adapter found for mimetype [%s] - file not loaded [%s]", mimeType.c_str(), fileParser.getFilename().c_str());

						return response;
					} else
						logger->debug("Getting [%s] [%s] from cache", mimeType.c_str(), fileParser.getFilename().c_str());
				} catch (Exception &e) {
					logger->error("Error loading resource [%s] [%s]: [%s]", mimeType.c_str(), fileParser.getFilename().c_str(), e.getMessage().c_str());
				}

				return cached;
		}
		void dispose(Resource *resource)
		{
			if(resource != null) {
				resourceCache.erase(resource->getFileName());
				adapters[resource->getMimeType()]->dispose(resource);
				delete resource;
			}
		}

		~ResourceManager()
		{
			for (std::map<String, Resource *>::iterator currentResourceIterator =
					resourceCache.begin(); currentResourceIterator != resourceCache.end();
					currentResourceIterator++) {
				if((*currentResourceIterator).second != null)
					logger->debug("Disposing of resource ['%s' - '%s']", (*currentResourceIterator).second->getMimeType().c_str(), (*currentResourceIterator).second->getFileName().c_str());

				dispose((*currentResourceIterator).second);
			}

			std::set<ResourceAdapter *> resourceAdaptersSet;
			for (std::map<String, ResourceAdapter *>::iterator currentAdapterIterator =
														adapters.begin(); currentAdapterIterator != adapters.end();
														currentAdapterIterator++) {
				resourceAdaptersSet.insert(currentAdapterIterator->second);
			}

			for (std::set<ResourceAdapter *>::iterator currentAdapterIterator =
																	resourceAdaptersSet.begin(); currentAdapterIterator != resourceAdaptersSet.end();
																	currentAdapterIterator++) {
				delete (*currentAdapterIterator);
			}
			adapters.clear();
			resourceAdaptersSet.clear();
		}
		const String normalize(const String &fileName) const
		{
			if(fileName.substr(0, 1) == "/")
				return fileName;

			return rootFolder + fileName;
		}

	private:

		const String getCacheKey(const String &filename, const String &mimeType) const
		{
			return normalize(filename) + "|" + mimeType;
		}

		String guessMimeType(const String &fileName)
		{
			int position = fileName.find_last_of(".");
			if(position > 0) {
				String extension = fileName.substr(position + 1, fileName.length() - position - 1);
				std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
				if(extension == "ogg") {
					logger->debug("guessMimeType: mime type for [%s] is [%s]", fileName.c_str(), "audio/ogg");
					return "audio/ogg";
				}
				else if (extension == "wav") {
					logger->debug("guessMimeType: mime type for [%s] is [%s]", fileName.c_str(), "audio/wav");
					return "audio/wav";
				}
				else if (extension == "jpeg" || extension == "jpg") {
					logger->debug("guessMimeType: mime type for [%s] is [%s]", fileName.c_str(), "image/jpeg");
					return "image/jpeg";
				}
				else if (extension == "png") {
					logger->debug("guessMimeType: mime type for [%s] is [%s]", fileName.c_str(), "image/png");
					return "image/png";
				}
				else if (extension == "tga") {
					logger->debug("guessMimeType: mime type for [%s] is [%s]", fileName.c_str(), "image/tga");
					return "image/tga";
				}
			}

			logger->error("Could not determine mimetype for [%s]", fileName.c_str());
			throw InvalidArgumentException("Could not determine mimetype for [%s]", fileName.c_str());
		}

};

#endif /* RESOURCEMANAGER_H_ */
