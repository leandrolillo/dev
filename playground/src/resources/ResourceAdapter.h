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

class ResourceAdapter
{
	private:
		ResourceManager *resourceManager;
	public:
		virtual const std::vector<String> getSupportedMimeTypes() = 0;
		virtual Resource *load(FileParser &fileParser, const String &mimeType) = 0;
		virtual void dispose(Resource *resource) = 0;
		virtual ~ResourceAdapter() {}

		ResourceManager* getResourceManager()
		{
			return resourceManager;
		}

		void setResourceManager(ResourceManager* resourceManager)
		{
			this->resourceManager = resourceManager;
		}

		;
};


#endif /* RESOURCEADAPTER_H_ */
