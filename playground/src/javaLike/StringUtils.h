/*
 * StringUtils.h
 *
 *  Created on: Oct 9, 2022
 *      Author: leandro
 */

#ifndef SRC_JAVALIKE_STRINGUTILS_H_
#define SRC_JAVALIKE_STRINGUTILS_H_

#include <JavaLike.h>

class StringUtils {
public:
	//TODO: this should return a new string?
	static void trim(String &text) {
		text.erase(text.begin(), std::find_if(text.begin(), text.end(),
		            std::not1(std::ptr_fun<int, int>(std::isspace))));

		text.erase(std::find_if(text.rbegin(), text.rend(),
		            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), text.end());
	}

};




#endif /* SRC_JAVALIKE_STRINGUTILS_H_ */
