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
	static String trim(String text) {
		text.erase(text.begin(), std::find_if(text.begin(), text.end(),
				[](unsigned char ch) {
		        	return !std::isspace(ch);
		    	}
		));

		text.erase(std::find_if(text.rbegin(), text.rend(),
				[](unsigned char ch) {
					return !std::isspace(ch);
				}
		).base(), text.end());

		return text;
	}
};
#endif /* SRC_JAVALIKE_STRINGUTILS_H_ */
