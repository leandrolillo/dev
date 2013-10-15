/*
 * StringUtils.h
 *
 *  Created on: 14/10/2013
 *      Author: Lean
 */

#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_

#include "core/javaLike.h"
#include <stdarg.h>

#define char_buffer_size 256

class StringUtils
{
public:
		static String format(const char *format, ...)
		{
			char buffer[char_buffer_size];

			va_list args;
			va_start(args, format);
			vsprintf(buffer, format, args);
			va_end(args);

			return String(buffer);
		}
};



#endif /* STRINGUTILS_H_ */
