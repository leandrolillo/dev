/*
 * ResourceAdapter.h
 *
 *  Created on: 30/12/2012
 *      Author: Lean
 */

#ifndef RESOURCEADAPTER_H_
#define RESOURCEADAPTER_H_

#include <Resource.h>
#include <ResourceManager.h>
#include<vector>

class ResourceManager;

class ResourceAdapter {
private:
	ResourceManager *resourceManager = null;
	std::vector<String> supportedMimeTypes;
protected:
	Logger *logger = null;
public:
	virtual ~ResourceAdapter() {
	}

	ResourceManager* getResourceManager() {
		return resourceManager;
	}

	void setResourceManager(ResourceManager *resourceManager) {
		this->resourceManager = resourceManager;
	}

	void addSupportedMimeType(String mimeType) {
		supportedMimeTypes.push_back(mimeType);
	}

	void clearSupportedMimeTypes() {
	    supportedMimeTypes.clear();
	}

	const std::vector<String> getSupportedMimeTypes() const {
		return supportedMimeTypes;
	}

	virtual Resource* load(FileParser &fileParser, const String &mimeType) = 0;
	virtual void dispose(Resource *resource) const {};

	virtual String toString() const {
		if (logger == null) {
			return "Unknown resource adapter";
		}

		return logger->getBasename();
	}
};

#endif /* RESOURCEADAPTER_H_ */
