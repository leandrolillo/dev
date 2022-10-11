/*
 * Resource.h
 *
 *  Created on: 30/12/2012
 *      Author: Lean
 */

#ifndef RESOURCE_H_
#define RESOURCE_H_

#include "JavaLike.h"
#include "Logger.h"
#include <string>
#include<set>

class Resource {
private:
	Logger *logger = LoggerFactory::getLogger("resources/Resource");
	unsigned long id; // id makes sense to business only - thus can be repeated. It is not a sequential primary key for all resources
	String fileName;
	String mimeType;
	std::set<String> labels;

public:
	Resource(unsigned long id, const String &mimeType) {
		setId(id);
		setMimeType(mimeType);
	}
	virtual ~Resource() {}

	unsigned long getId() const {
		return id;
	}

	void setId(unsigned long id) {
		this->id = id;
	}

	const String& getMimeType() const {
		return mimeType;
	}

	void setMimeType(const String & mimeType) {
		this->mimeType = mimeType;
	}

	const String& getFileName() const {
		return fileName;
	}

	void setFileName(const String & fileName) {
		this->fileName = fileName;
	}

	const std::set<String>& getLabels() const {
		return labels;
	}

	void setLabels(const std::set<String> &labels) {
		this->labels = labels;
	}

	void addLabel(const String &label) {
		this->labels.insert(label);
	}

	virtual String toString() const {
		return String("Resource(id:" + std::to_string(this->getId()) + ") [" + this->getMimeType() + "] [" + this->fileName + "]");
	}
};

#endif /* RESOURCE_H_ */
