/*
 * ResourceLoadRequest.h
 *
 *  Created on: Oct 21, 2022
 *      Author: leandro
 */

#ifndef SRC_RESOURCES_RESOURCELOADREQUEST_H_
#define SRC_RESOURCES_RESOURCELOADREQUEST_H_

#include "FileParser.h"
#include "Resource.h"
#include "Paths.h"
#include <set>

class ResourceLoadRequest {
private:
	Logger *logger = LoggerFactory::getLogger("ResourceLoadRequest");
	FileParser &fileParser;
	String inputMimeType;
	String outputMimeType;
	std::set<String> labels = {};

public:
	ResourceLoadRequest(FileParser &fileParser) : fileParser(fileParser) {
		this->inputMimeType = MimeTypes::guessMimeType(fileParser.getFilename());
	}

	ResourceLoadRequest &acceptMimeType(const String &mimeType) {
		this->outputMimeType = mimeType;
		return *this;
	}

	ResourceLoadRequest &produceMimeType(const String &mimeType) {
		this->outputMimeType = mimeType;
		return *this;
	}

	ResourceLoadRequest &withLabels(const std::set<String> &labels) {
		this->labels = labels;
		return *this;
	}

	ResourceLoadRequest &withAdditionalLabels(const std::set<String> &labels) {
		this->labels.insert(labels.begin(), labels.end());

		return *this;
	}

	ResourceLoadRequest &withParent(const Resource *resource) {
		if(resource != null) {
			fileParser = FileParser(Paths::relative(resource->getFileName(), fileParser.getFilename()));
		}
		return *this;
	}

	FileParser& getFileParser() const {
		return fileParser;
	}

	const String &getFilePath() const {
		return fileParser.getFilename();
	}

	const String& getInputMimeType() const {
		return inputMimeType;
	}

	const std::set<String>& getLabels() const {
		return labels;
	}

	const String& getOutputMimeType() const {
		return outputMimeType;
	}

	String errors() const {
		String errors;

		if(inputMimeType.empty()) {
			errors.append("Could not determine input mimeType");
		}

		return errors;
	}

	bool isValid() const {
		return errors().empty();
	}

	String toString() const {
		return StringFormatter::format("[%s]->[%s] [%s]", getInputMimeType().c_str(), getOutputMimeType().c_str(), getFilePath().c_str());
	}
};




#endif /* SRC_RESOURCES_RESOURCELOADREQUEST_H_ */
