/*
 * Resource.h
 *
 *  Created on: 30/12/2012
 *      Author: Lean
 */

#ifndef RESOURCE_H_
#define RESOURCE_H_

#include "../javaLike/JavaLike.h"
#include <string>

class Resource {
	private:
		unsigned long id;
		String fileName;
		String mimeType;

	public:
		Resource(unsigned long id, const String &mimeType)
		{
			setId(id);
			setMimeType(mimeType);
		}
		unsigned long getId() const {
			return id;
		}

		void setId(unsigned long id) {
			this->id = id;
		}

		const String& getMimeType() const
		{
			return mimeType;
		}

		void setMimeType(const String & mimeType)
		{
			this->mimeType = mimeType;
		}

		const String& getFileName() const
		{
			return fileName;
		}

		void setFileName(const String & fileName)
		{
			this->fileName = fileName;
		}

		virtual const String &toString() const {
			return "Resource(id:" + std::to_string(this->getId()) + ") [" + this->getMimeType() + "] [" + this->fileName + "]";
		}

	};

#endif /* RESOURCE_H_ */
