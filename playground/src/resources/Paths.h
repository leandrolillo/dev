/*
 * Paths.h
 *
 *  Created on: Aug 15, 2022
 *      Author: leandro
 */

#ifndef SRC_RESOURCES_PATHS_H_
#define SRC_RESOURCES_PATHS_H_

#include "JavaLike.h"

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
	 * if the provided postfix is an absolute path, return that.
	 * if the provided postfix starts with ~/ then make it is relative to the repository and we return that as well.
	 * Otherwise concatenate the prefix + postfix and remove extra slashes.
	 */
	static String add(const String &prefix, const String &postFix) {
		String normalizedPrefix = prefix.substr(prefix.length() - 1, 1) == "/" ? prefix : prefix + "/";
		String normalizedPostfix = postFix.substr(0, 2) == "./" ? postFix.substr(2, postFix.length() - 2) : postFix;

		return normalizedPostfix.substr(0, 1) == "/" || normalizedPostfix.substr(0, 2) == "~/" ?
				normalizedPostfix :
				normalizedPrefix + normalizedPostfix;
	}

	/**
	 * Return the folder part of a path to a file.
	 */
	static const String getDirname(const String &filePath) {
		unsigned long location = filePath.find_last_of('/');
		return (location == std::string::npos ? "." : filePath.substr(0, location + 1));

	}

	/**
	 * Return the folder part of a path to a file.
	 */
	static const String getBasename(const String &filePath) {
		unsigned long location = filePath.find_last_of('/');
		return (location == std::string::npos ? filePath : filePath.substr(location + 1, filePath.size() - location));
	}

};
#endif /* SRC_RESOURCES_PATHS_H_ */
