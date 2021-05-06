#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <JavaLike.h>
#include<string>
#include <stdio.h>
#include <stdarg.h>

class Exception {
	protected:
		String msg;

		void setMessageWithVarArgs(const char *format, va_list *args)
		{
		    char *tempBuffer = null; //TODO: review if it is better to use a fixed buffer and truncate logs length
            if(vasprintf(&tempBuffer, format, *args)) {
                this->msg = tempBuffer;
                printf("Exception initialized with error message: %s", this->msg.c_str());
                free(tempBuffer);
            }
		}

		public:
		Exception()
		{
			this->msg = "Unhandled Exception";
		}
		Exception(const char *format, ...) {
			va_list args;
			va_start(args, format);
			this->setMessageWithVarArgs(format, &args);
			va_end(args);
		}
		std::string toString() {
			return getMessage();
		}

		std::string getMessage()
		{
			return this->msg;
		}
		void setMessage(const char *format, ...)
		{
			va_list args;
			va_start(args, format);

			this->setMessageWithVarArgs(format, &args);
			va_end(args);

		}
	};
#endif
