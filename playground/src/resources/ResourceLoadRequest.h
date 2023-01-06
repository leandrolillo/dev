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
	String uri;
	String inputMimeType;
	String outputMimeType;
	std::set<String> labels = {};
	std::shared_ptr<FileParser> fileParser;
	const Resource *parent = null;

public:
	ResourceLoadRequest(const String &uri) {
		this->uri = uri;
		this->inputMimeType = MimeTypes::guessMimeType(this->uri);
	}

	ResourceLoadRequest(std::shared_ptr<FileParser> &fileParser) : fileParser(fileParser) {
		this->uri = fileParser.get()->getFilename();
		this->inputMimeType = MimeTypes::guessMimeType(this->uri);
	}

//    /* Rule of five and copy-and-swap -- this is only necessary if fileParser is a resource not to be shared*/
//    friend void swap(ResourceLoadRequest& first, ResourceLoadRequest& second)
//	{
//		// enable ADL (not necessary in our case, but good practice)
//		using std::swap;
//
//		// by swapping the members of two objects, the two objects are effectively swapped
//		swap(first.uri, second.uri);
//		swap(first.inputMimeType, second.inputMimeType);
//		swap(first.outputMimeType, second.outputMimeType);
//		swap(first.labels, second.labels);
//		swap(first.parent, second.parent);
//		swap(first.fileParser, second.fileParser);
//	}
//
//
//	ResourceLoadRequest(const ResourceLoadRequest &other) {
//		this->uri = other.uri;
//		this->inputMimeType = other.inputMimeType;
//		this->outputMimeType = other.outputMimeType;
//		this->labels = other.labels;
//		this->parent = other.parent;
//		this->fileParser = other.fileParser; // copy or share?
//	}
//
//	ResourceLoadRequest(ResourceLoadRequest && other) {
//		swap(*this, other);
//	}
//
//	ResourceLoadRequest &operator=(ResourceLoadRequest other) {
//		swap(*this, other);
//		return *this;
//	}

	//TODO: review if it would be better to return new objects - would be safer and shorter syntax while giving up performance
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
		this->parent = resource;
		return *this;
	}

	const String &getUri() const {
		return this->uri;
	}

	FileParser& getFileParser() {
		if(!fileParser) {
			fileParser = std::shared_ptr<FileParser>(new FileParser(this->getFilePath()));
		}
		return *fileParser.get();
	}

	const String getFilePath() const {
		return Paths::getActualPath(Paths::relative(parent == null ? "" : parent->getFileName(), this->uri));
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

		if(uri.empty()) {
			errors.append("uri is required");
		}

		if(outputMimeType.empty()) {
			errors.append("Output mimetype is required");
		}

		if(inputMimeType.empty()) {
			errors.append("Could not determine input mimeType and it was not provided");
		}

		return errors;
	}

	bool isValid() const {
		return errors().empty();
	}

	String toString() const {
		return StringFormatter::format("[%s]->[%s] [%s]", getInputMimeType().c_str(), getOutputMimeType().c_str(), getUri().c_str());
	}
};




#endif /* SRC_RESOURCES_RESOURCELOADREQUEST_H_ */
