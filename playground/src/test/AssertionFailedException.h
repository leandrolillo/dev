/*
 * AssertionFailedException.h
 *
 *  Created on: 14/10/2013
 *      Author: Lean
 */

#ifndef ASSERTIONFAILEDEXCEPTION_H_
#define ASSERTIONFAILEDEXCEPTION_H_

#include "exceptions/Exception.h"

class AssertionFailedException: public Exception {
	public:
		AssertionFailedException(const char *format, ...) {
			va_list args;
			va_start(args, format);
			this->setMessageWithVarArgs(format, &args);
			va_end(args);
		}

		AssertionFailedException(const char *format, va_list *args) {
			this->setMessageWithVarArgs(format, args);
		}

	private:
};

#endif /* ASSERTIONFAILEDEXCEPTION_H_ */
