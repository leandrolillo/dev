/*
 * ResourceAdapter.h
 *
 *  Created on: 30/12/2012
 *      Author: Lean
 */

#ifndef RESOURCEADAPTER_H_
#define RESOURCEADAPTER_H_

#include "Resource.h"
#include "ResourceManager.h"
#include<vector>

class ResourceManager;

class ResourceAdapter {
private:
	ResourceManager *resourceManager = null;
	std::set<String> supportedMimeTypes;
protected:
	Logger *logger = null;
public:
	virtual ~ResourceAdapter() {
		logger->debug("Destroying resource adapter %s", this->toString().c_str());
	}

	ResourceManager* getResourceManager() const {
		return resourceManager;
	}

	void setResourceManager(ResourceManager *resourceManager) {
		this->resourceManager = resourceManager;
	}

	void addSupportedMimeType(const String &mimeType) {
		supportedMimeTypes.insert(mimeType);
	}

	void setSupportedMimeTypes(const std::set<String> &supportedMimeTypes) {
	    this->supportedMimeTypes = supportedMimeTypes;
	}

	const std::set<String> &getSupportedMimeTypes() const {
		return supportedMimeTypes;
	}

	virtual Resource* load(FileParser &fileParser, const String &mimeType) const = 0;
	virtual void dispose(Resource *resource) const {};

	virtual String toString() const {
		if (logger == null) {
			return "Unknown resource adapter";
		}

		return logger->getBasename();
	}
};

#endif /* RESOURCEADAPTER_H_ */
