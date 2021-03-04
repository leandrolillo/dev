/*
 * FileResource.h
 *
 *  Created on: 01/01/2013
 *      Author: Lean
 */

#ifndef FILERESOURCE_H_
#define FILERESOURCE_H_

#include "Resource.h"

class FileResource : public Resource
{
	private:
		String name;
		String path;

	public:
		const String& getName() const
		{
			return name;
		}

		void setName(const String &name)
		{
			this->name = name;
		}

		const String& getPath() const
		{
			return path;
		}

		void setPath(const String& path)
		{
			this->path = path;
		}
};


#endif /* FILERESOURCE_H_ */
