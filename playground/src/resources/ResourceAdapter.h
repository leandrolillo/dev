/*
 * ResourceAdapter.h
 *
 *  Created on: 30/12/2012
 *      Author: Lean
 */

#ifndef RESOURCEADAPTER_H_
#define RESOURCEADAPTER_H_

#include "Resource.h"
#include "ResourceLoadRequest.h"
#include "ResourceManager.h"
#include<vector>

class ResourceManager;

class ResourceAdapter {
private:
	ResourceManager *resourceManager = null;
	String inputMimeType = "";
	String outputMimeType = "";

	std::set<String> disposesMimeTypes;

protected:
	Logger *logger = null;
public:
	virtual ~ResourceAdapter() {
		if(logger != null) {
			logger->debug("Destroying resource adapter %s", this->toString().c_str());
		}
	}

	ResourceManager* getResourceManager() const {
		return resourceManager;
	}

	void setResourceManager(ResourceManager *resourceManager) {
		this->resourceManager = resourceManager;
	}


	virtual Resource* load(ResourceLoadRequest &request) const = 0;
	virtual void dispose(Resource *resource) const {};

	virtual String toString() const {
		if (logger == null) {
			return "Unknown resource adapter";
		}

		return logger->getBasename();
	}

	const String& getInputMimeType() const {
		return inputMimeType;
	}

	const String& getOutputMimeType() const {
		return outputMimeType;
	}

	ResourceAdapter *accepts(const String &inputMimeType) {
		this->inputMimeType = inputMimeType;
		return this;
	}

	ResourceAdapter * produces(const String &outputMimeType) {
		this->outputMimeType = outputMimeType;
		return this;
	}



	const std::set<String>& getDisposesMimeTypes() const {
		return disposesMimeTypes;
	}

	ResourceAdapter *disposes(const String &mimeType) {
		this->disposesMimeTypes.insert(this->disposesMimeTypes.begin(), mimeType);
		return this;
	}

	void setDisposesMimeTypes(const std::set<String> &disposesMimeTypes) {
		this->disposesMimeTypes = disposesMimeTypes;
	}
};

#endif /* RESOURCEADAPTER_H_ */
