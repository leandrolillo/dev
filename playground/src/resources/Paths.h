/*
 * Paths.h
 *
 *  Created on: Aug 15, 2022
 *      Author: leandro
 */

#ifndef SRC_RESOURCES_PATHS_H_
#define SRC_RESOURCES_PATHS_H_

#include "JavaLike.h"
#include "StringUtils.h"

class Paths {
public:
	/**
	 * Return an absolute path from root folder and filename.
	 */
	static String normalize(const String &fileName, const String &rootFolder) {
		if(fileName.substr(0, 2) == "~/") {
			return Paths::add(rootFolder, fileName.substr(2, fileName.size() - 2));
		}
		return Paths::add(rootFolder, fileName);
	}

	/**
	 * TODO: Unify with normalize
	 */
	static String relative(const String &prefix, const String &postFix, const String &home = "") {
		String normalizedPostfix = StringUtils::trim(postFix);

		if(normalizedPostfix.length() >= 2 && normalizedPostfix.substr(0, 2) == "~/") {
			return Paths::add(home, normalizedPostfix.substr(2, normalizedPostfix.size() - 2));
		} else {
			return Paths::add(Paths::getDirname(prefix), postFix);
		}
	}

	/**
	 * if the provided postfix is an absolute path, return that.
	 * if the provided postfix starts with ~/ then it is relative to the "home" directory.
	 * Otherwise concatenate the prefix + postfix and remove extra slashes.
	 */
	static String add(const String &prefix, const String &postFix) {
		String normalizedPrefix = StringUtils::trim(prefix);
		String normalizedPostfix = StringUtils::trim(postFix);

		if(normalizedPostfix.rfind("/", 0) == 0) {
			return normalizedPostfix;
		}

		if(normalizedPrefix.empty()) {
			return normalizedPostfix;
		} else {

			size_t position = 0;
			if((position = normalizedPostfix.rfind("./", 0)) == 0) {
				normalizedPostfix.replace(0, 2, "");
			}
			if((position = normalizedPostfix.rfind("~/", 0)) == 0) {
				normalizedPostfix.replace(0, 2, "");
			}

			normalizedPrefix = normalizedPrefix.substr(normalizedPrefix.length() - 1, 1) == "/" ? normalizedPrefix : normalizedPrefix + "/";

			return normalizedPrefix + normalizedPostfix;
		}
	}

	/**
	 * Return the folder part of a path to a file.
	 */
	static String getDirname(const String &filePath) {
		unsigned long location = filePath.find_last_of('/');
		return (location == std::string::npos ? "." : filePath.substr(0, location + 1));

	}

	/**
	 * Return the file part of a path to a file.
	 */
	static String getBasename(const String &filePath) {
		unsigned long location = filePath.find_last_of('/');
		return (location == std::string::npos ? filePath : filePath.substr(location + 1, filePath.size() - location));
	}

	//TODO: This should inspect filePath and return the longest path that is a real file/directory
	static String getActualPath(const String &uri) {
		return uri;
	}
private:

};
#endif /* SRC_RESOURCES_PATHS_H_ */
